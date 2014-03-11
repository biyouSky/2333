#pragma once

//#ifdef _WIN32_

#include "..\Utility\GXUtil.h"

// ͬ������
/*  
�ٽ���( Critical Section )��Win32���ṩ��һ����������ͬ�����ƣ�
�뻥��( Mutex )���¼�( Event )���ں�ͬ��������ȣ��ٽ�������ȫ���û�̬ά���ģ����Խ�����ͬһ�����ڹ��߳�ͬ��ʹ�ã�
��Ҳ���������ʹ��ʱ�����û�̬�ͺ���̬֮����л�������Ч�ʴ���������ͬ�����ơ�
�����������ڽ��̼�ʹ��, ���ٽ�������ֻ����ͬһ���̵ĸ��̼߳�ʹ�á�
��Ȼ���������Ҳ��������ͬһ���̵ĸ����̼߳䣬��������������£�ʹ���ٽ��������ʡϵͳ��Դ������Ч�ʡ�
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

// �̹߳��� ������Ӹù�����̳� ��ֻ֧��һ���߳�
template<class T>
class _SingleThreadManage
{
	// �߳����
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

	// �߳���غ���
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		// ִ�к���ָ����ָ�ĺ���  ��ָ��+��ַƫ��
		( ( ( (_SingleThreadManage<T>*)pParam )->m_pOwner )->*( ( (_SingleThreadManage<T>*)pParam )->m_threadFuncAddr ) )();

		// �߳̾����0
		( (_SingleThreadManage<T>* )pParam)->m_threadHandle = NULL;

		return 1;
	}
	
	// �����߳�
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

	// ֹͣ�߳�
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



// �̹߳��� ������Ӹù�����̳�
template<class T>
class _MultiThreadManage
{
	T* m_pOwner;
	typedef void ( T::*pParamFuncType )( void* );
	typedef void ( T::*pFuncType )();
	
	struct _ThreadData
	{
		std::string name;							// �߳�����Ϊ�̵߳�Ψһ��ʶ��
		pParamFuncType paramFuncAddr;				// �̺߳���ָ��
		pFuncType funcAddr;
		HANDLE threadHandle;						// �߳̾��
		
		_ThreadData() 
		{
			paramFuncAddr = NULL;
			funcAddr = NULL;
			threadHandle = NULL;
		}
	};

	// �߳�����  �����Ĵ�Сʵ��Ӧ��OS�����ƾ���
	enum { THREADLIST_MAXSIZE = 255 };
	_ThreadData m_threadList[THREADLIST_MAXSIZE];	// �̳߳�								
	cnCriticalSection m_threadListMutex;			// �̳߳��ٽ���

	// �̲߳���
	struct _threadParam
	{
		_MultiThreadManage<T> *pThis;				// 
		int curIndex;								// ��ǰ�߳�index
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

	// �߳���غ���
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		_threadParam* pThreadParam= (_threadParam*)pParam;
		_MultiThreadManage<T> *pThis = pThreadParam->pThis;
		int index = pThreadParam->curIndex;
		void* pData = pThreadParam->pData;
		delete pThreadParam;

		// ִ�к���ָ����ָ�ĺ���  ��ָ��+��ַƫ��
		_ThreadData& threadData = pThis->m_threadList[index];
		if( threadData.paramFuncAddr!=NULL )
			( ( pThis->m_pOwner )->*( pThis->m_threadList[index].paramFuncAddr ) )( pData );
		else
			( ( pThis->m_pOwner )->*( pThis->m_threadList[index].funcAddr ) )();

		// �߳̾����0
		pThis->m_threadList[index].threadHandle = NULL;
		pThis->m_threadList[index].name = "";

		return 1;
	}
	
	// �����߳�
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

	// ֹͣ�߳�
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

	// Ѱ�ҿ�λ
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

// �̹߳��� ������Ӹù�����̳� �ṩ������ִ�з�ʽ FIFO
template<class T>
class _QueueThreadManage
{
	T* m_pOwner;

	// �߳����
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

	// �߳���غ���
	static DWORD WINAPI ThreadFunc( LPVOID pParam )
	{	
		// ִ�к���ָ����ָ�ĺ���  ��ָ��+��ַƫ��
		( ( ( (_QueueThreadManage<T>*)pParam )->m_pOwner )->*( ( (_QueueThreadManage<T>*)pParam )->m_threadFuncAddr ) )();
		
		// ִ����һ��
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
	
	// �����߳�
	bool StartThread( std::string threadName, pThreadFuncType pFunc )
	{
		if( pFunc==NULL )
			return false;

		PushBack( pFunc );	
		if( m_threadHandle==NULL )
			Run();
		return true;
	}

	// ֹͣ�߳�
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