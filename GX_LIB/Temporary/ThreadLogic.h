#pragma once

//#ifdef _WIN32_

#include "..\Utility\GXUtil.h"

// 同步对象
/*  
临界区( Critical Section )是Win32中提供的一种轻量级的同步机制，
与互斥( Mutex )和事件( Event )等内核同步对象相比，临界区是完全在用户态维护的，所以仅能在同一进程内供线程同步使用，
但也因此无需在使用时进行用户态和核心态之间的切换，工作效率大大高于其它同步机制。
互斥对象可以在进程间使用, 而临界区对象只能在同一进程的各线程间使用。
当然，互斥对象也可以用于同一进程的各个线程间，但是在这种情况下，使用临界区会更节省系统资源，更有效率。
*/
class cnCriticalSection
{
	CRITICAL_SECTION m_criticalSection;

public:
	cnCriticalSection() { ::InitializeCriticalSection( &m_criticalSection ); }
	~cnCriticalSection() { ::DeleteCriticalSection( &m_criticalSection ); }

	void Lock() { ::EnterCriticalSection( &m_criticalSection ); }
	void UnLock() { ::LeaveCriticalSection( &m_criticalSection ); }
};

class cnCriticalSectionAuto
{
	cnCriticalSection *m_p;

	public:
		cnCriticalSectionAuto( cnCriticalSection& object ) 
		{ 
			m_p = &object;
			m_p->Lock();
		}
		~cnCriticalSectionAuto() 
		{ 
			m_p->UnLock();
		}
};


class cnMutex
{
	HANDLE m_mutex;

public:
	cnMutex() { m_mutex = ::CreateMutex( NULL, FALSE, NULL ); }
	~cnMutex() { CloseHandle( m_mutex ); }

	void Lock() {  ::WaitForSingleObject( m_mutex, INFINITE ); }
	void UnLock() { :: ReleaseMutex( m_mutex ); }
};

class cnMutexAuto
{
	cnMutex *m_p;

	public:
		cnMutexAuto( cnMutex& object ) 
		{ 
			m_p = &object;
			m_p->Lock();
		}
		~cnMutexAuto() 
		{ 
			m_p->UnLock();
		}
};

// 线程管理 其他类从该管理类继承 暂只支持一个线程
template<class T>
class _SingleThreadManage
{
	// 线程相关
	HANDLE m_threadHandle;
	typedef void ( T::*pThreadFuncType )();
	pThreadFuncType m_threadFuncAddr;
	T* m_pOwner;

protected:
	_SingleThreadManage()
	{
		m_threadHandle = NULL;
		m_threadFuncAddr = NULL;

		m_pOwner = static_cast<T*>( this ); 
	}

	~_SingleThreadManage() { StopThread(); }

	// 线程相关函数
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		// 执行函数指针所指的函数  类指针+地址偏移
		( ( ( (_SingleThreadManage<T>*)pParam )->m_pOwner )->*( ( (_SingleThreadManage<T>*)pParam )->m_threadFuncAddr ) )();

		// 线程句柄置0
		( (_SingleThreadManage<T>* )pParam)->m_threadHandle = NULL;

		return 1;
	}
	
	// 启动线程
	bool StartThread( pThreadFuncType pFunc )
	{
		if( pFunc==NULL )
			return false;

		if( m_threadHandle!=NULL )
		{
			::MessageBoxA( NULL, "Last Working Thread is running now..Wait a moment..", "Warning", 0 );
			return false;
		}

		m_threadFuncAddr = pFunc;
		m_threadHandle = ::CreateThread( NULL, 0, ThreadFunc, (LPVOID)this, 0, NULL );
		return true;
	}

	// 停止线程
	bool StopThread()
	{
		if( m_threadHandle==NULL )
			return false;
		
		if( !::TerminateThread( m_threadHandle, 0 ) )
			return false;
                
		::CloseHandle( m_threadHandle );
		m_threadHandle = NULL;

		return true;
	}
};



// 线程管理 其他类从该管理类继承
template<class T>
class _MultiThreadManage
{
	T* m_pOwner;
	typedef void ( T::*pParamFuncType )( void* );
	typedef void ( T::*pFuncType )();
	
	struct _ThreadData
	{
		std::string name;							// 线程名作为线程的唯一标识符
		pParamFuncType paramFuncAddr;				// 线程函数指针
		pFuncType funcAddr;
		HANDLE threadHandle;						// 线程句柄
		
		_ThreadData() 
		{
			paramFuncAddr = NULL;
			funcAddr = NULL;
			threadHandle = NULL;
		}
	};

	// 线程容器  容器的大小实际应由OS的限制决定
	enum { THREADLIST_MAXSIZE = 255 };
	_ThreadData m_threadList[THREADLIST_MAXSIZE];	// 线程池								
	cnCriticalSection m_threadListMutex;			// 线程池临界区

	// 线程参数
	struct _threadParam
	{
		_MultiThreadManage<T> *pThis;				// 
		int curIndex;								// 当前线程index
		void* pData;

		_threadParam( _MultiThreadManage<T>* _pThis, int _curIndex, void* _pData=NULL )
		{
			pThis = _pThis;
			curIndex = _curIndex;
			pData = _pData;
		}
	};

protected:
	_MultiThreadManage() { m_pOwner = static_cast<T*>( this ); }
	~_MultiThreadManage() { StopAllThreads(); }

	// 线程相关函数
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		_threadParam* pThreadParam= (_threadParam*)pParam;
		_MultiThreadManage<T> *pThis = pThreadParam->pThis;
		int index = pThreadParam->curIndex;
		void* pData = pThreadParam->pData;
		delete pThreadParam;

		// 执行函数指针所指的函数  类指针+地址偏移
		_ThreadData& threadData = pThis->m_threadList[index];
		if( threadData.paramFuncAddr!=NULL )
			( ( pThis->m_pOwner )->*( pThis->m_threadList[index].paramFuncAddr ) )( pData );
		else
			( ( pThis->m_pOwner )->*( pThis->m_threadList[index].funcAddr ) )();

		// 线程句柄置0
		pThis->m_threadList[index].threadHandle = NULL;
		pThis->m_threadList[index].name = "";

		return 1;
	}
	
	// 启动线程
	bool StartThread( std::string threadName, pParamFuncType pFunc, void* pData=NULL )
	{
		if( pFunc==NULL )
			return false;

		if( SearchThreadPos( threadName )!=-1 )
		{
			string str;
			STRING_FORMAT( str, "The same name %s thread is running now..", threadName.c_str() );
			::MessageBoxA( NULL, str.c_str(), "Warning", 0 );
			return false;
		}

		int index = SearchEmptyPos();
		if( index==-1 )
		{
			::MessageBoxA( NULL, "Thread Pool is full..Wait a moment..", "Warning", 0 );
			return false;
		}

		m_threadListMutex.Lock();
		// 
		_ThreadData& threadData = m_threadList[index];
		threadData.paramFuncAddr = pFunc;
		threadData.funcAddr = NULL;
		threadData.name = threadName;
		_threadParam *pParam = new _threadParam( this, index, pData );
		threadData.threadHandle = ::CreateThread( NULL, 0, ThreadFunc, (LPVOID)pParam, 0, NULL );
		// 
		m_threadListMutex.UnLock();
		return true;
	}

	bool StartThread( std::string threadName, pFuncType pFunc )
	{
		if( pFunc==NULL )
			return false;

		if( SearchThreadPos( threadName )!=-1 )
		{
			string str;
			STRING_FORMAT( str, "The same name %s thread is running now..", threadName.c_str() );
			::MessageBoxA( NULL, str.c_str(), "Warning", 0 );
			return false;
		}

		int index = SearchEmptyPos();
		if( index==-1 )
		{
			::MessageBoxA( NULL, "Thread Pool is full..Wait a moment..", "Warning", 0 );
			return false;
		}

		m_threadListMutex.Lock();
		// 
		_ThreadData& threadData = m_threadList[index];
		threadData.paramFuncAddr = NULL;
		threadData.funcAddr = pFunc;
		threadData.name = threadName;
		_threadParam *pParam = new _threadParam( this, index, NULL );
		threadData.threadHandle = ::CreateThread( NULL, 0, ThreadFunc, (LPVOID)pParam, 0, NULL );
		// 
		m_threadListMutex.UnLock();
		return true;
	}

	// 停止线程
	bool StopThread( std::string threadName )
	{
		int index = SearchThreadPos( threadName );
		return StopThread( index );
	}
	
	bool  StopThread( int index )
	{
		if( index<0 || index>THREADLIST_MAXSIZE )
			return false;

		_ThreadData& threadData = m_threadList[index];
		if( threadData.threadHandle==NULL )
			return false;

		if( !::TerminateThread( threadData.threadHandle, 0 ) )
			return false;
		::CloseHandle( threadData.threadHandle );
		threadData.threadHandle = NULL;
		threadData.name = "";

		return true;
	}

	void StopAllThreads()
	{
		for( int i=0; i<THREADLIST_MAXSIZE; ++i )
		{
			if( m_threadList[i].threadHandle!=NULL )
				StopThread( i );
		}
	}

	// 寻找空位
	int SearchEmptyPos()
	{
		for( int i=0; i<THREADLIST_MAXSIZE; ++i )
		{
			if( m_threadList[i].threadHandle==NULL )
				return i;
		}
		return -1;
	}

	int SearchThreadPos( std::string threadName )
	{
		for( int i=0; i<THREADLIST_MAXSIZE; ++i )
		{
			if( m_threadList[i].name==threadName )
				return i;
		}
		return -1;
	}
};

// 线程管理 其他类从该管理类继承 提供按队列执行方式 FIFO
template<class T>
class _QueueThreadManage
{
	T* m_pOwner;

	// 线程相关
	HANDLE m_threadHandle;

	typedef void ( T::*pThreadFuncType )();
	pThreadFuncType m_threadFuncAddr;

	std::deque<pThreadFuncType> m_threadFuncAddrList;
	cnCriticalSection m_dequeCS;
	//
	void PushBack( pThreadFuncType pFunc )
	{
		m_dequeCS.Lock();
		std::deque<pThreadFuncType>::iterator it = find( m_threadFuncAddrList.begin(), m_threadFuncAddrList.end(), pFunc );
		//if( it==m_threadFuncAddrList.end() )
			m_threadFuncAddrList.push_back( pFunc );
		m_dequeCS.UnLock();
	}
	void PopFront()
	{
		m_dequeCS.Lock();
		m_threadFuncAddrList.pop_front();
		m_dequeCS.UnLock();
	}
	pThreadFuncType GetCurThreadFuncAddr()
	{
		cnCriticalSectionAuto csAuto( m_dequeCS );
		if( m_threadFuncAddrList.size()>0 )
			return m_threadFuncAddrList[0];
		else
			return NULL;
	}
	UINT GetSize()
	{
		cnCriticalSectionAuto csAuto( m_dequeCS );
		return m_threadFuncAddrList.size();
	}
	void Clear()
	{
		cnCriticalSectionAuto csAuto( m_dequeCS );
		m_threadFuncAddrList.clear();
	}

protected:
	_QueueThreadManage()
	{
		m_threadHandle = NULL;
		m_pOwner = static_cast<T*>( this ); 
	}

	~_QueueThreadManage() { StopThread(); }

	// 线程相关函数
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		// 执行函数指针所指的函数  类指针+地址偏移
		( ( ( (_QueueThreadManage<T>*)pParam )->m_pOwner )->*( ( (_QueueThreadManage<T>*)pParam )->m_threadFuncAddr ) )();
		
		// 执行下一个
		( (_QueueThreadManage<T>*)pParam )->PopFront();
		( (_QueueThreadManage<T>*)pParam )->Run();

		return 1;
	}

	void Run()
	{
		m_threadFuncAddr = GetCurThreadFuncAddr();
		if( m_threadFuncAddr!=NULL )
			m_threadHandle = ::CreateThread( NULL, 0, ThreadFunc, (LPVOID)this, 0, NULL );
		else
			m_threadHandle = NULL;
	}
	
	// 启动线程
	bool StartThread( std::string threadName, pThreadFuncType pFunc )
	{
		if( pFunc==NULL )
			return false;

		PushBack( pFunc );	
		if( m_threadHandle==NULL )
			Run();
		return true;
	}

	// 停止线程
	bool StopThread()
	{
		Clear();

		if( m_threadHandle==NULL )
			return true;
		//
		if( !::TerminateThread( m_threadHandle, 0 ) )
			return false;
        //       
		::CloseHandle( m_threadHandle );
		m_threadHandle = NULL;

		return true;
	}
};







//#endif