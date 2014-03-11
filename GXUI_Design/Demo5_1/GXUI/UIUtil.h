
#pragma once

/*-------------------------------------------------- ��ܻ��� --------------------------------------------------*/
#define _0X_		1			// ���԰汾
#define _WIN32_		1			// ϵͳ
#define _MFC_		0			// ������
#define _CONSOLE_	0			// ����ģʽ


/*-------------------------------------------------- ����ͷ�ļ� --------------------------------------------------*/
// C��
#include <ctime>				// 
#include <cmath>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>				// only win����

// STL
#include <string>				// �ַ�������
#include <vector>				// ��������
#include <deque>
#include <list>
#include <algorithm>			// �㷨
#include <numeric>
#include <functional>			// �º���			
// �������
#include <iostream>
#include <fstream>

// win32
#ifdef _WIN32_
//
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

#endif

// directx9ͷ�ļ�
#include <d3dx9.h>											
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib,"d3dx9.lib" )
#pragma comment( lib, "winmm.lib" )


/*-------------------------------------------------------��ɫ��-------------------------------------------------------*/
#define _RED_			D3DCOLOR_XRGB(255, 0, 0)
#define _GREEN_			D3DCOLOR_XRGB(0, 255, 0)
#define _BLUE_			D3DCOLOR_XRGB(0, 0, 255)
#define _BLACK_			D3DCOLOR_XRGB(0, 0, 0)
#define _WHITE_			D3DCOLOR_XRGB(255, 255, 255)
#define _GOLD_			D3DCOLOR_XRGB(255, 215, 0 )
#define _ORANGE_		D3DCOLOR_XRGB(255, 231, 157)
#define _GRAY_			D3DCOLOR_XRGB(192, 192, 192)

#define _FRAMEBLUE_		D3DCOLOR_XRGB(89, 176, 255)
#define _FRAMEWHITE_	D3DCOLOR_XRGB(246, 250, 252)
#define _FRAMEPREPLE_	D3DCOLOR_XRGB(165, 153, 222)
#define _FRAMEGREEN_	D3DCOLOR_XRGB(108, 226, 108)

#define D3DCOLOR_MIX( alphy, rgb ) ((D3DCOLOR)((((alphy)&0xff)<<24)|((rgb)&0x00ffffff)))



/*-------------------------------------------------------������Ϣ����-------------------------------------------------------*/
//#define WM_ADJUSTSIZE	WM_USER+1003
#define WM_UPDATE		WM_USER+1004




/*-------------------------------------------------- UI���ߺ��� --------------------------------------------------*/

struct GenRect
{
	RECT operator()( POINT point, SIZE size );
	RECT operator()( LONG left, LONG top, LONG right, LONG bottom );
};

struct GenPoint
{
	POINT operator()( LONG x, LONG y );
};

struct GenSize
{
	SIZE operator()( LONG x, LONG y );
};

struct GetWidth
{
	int operator()( RECT& rc );
};

struct GetHeight
{
	int operator()( RECT& rc );
};

struct CompareRect
{
	bool operator()( RECT& r1, RECT& r2 );
};

struct CrossRect
{
	bool operator()( RECT& rc1, RECT& rc2, RECT& crossRC );
};


#define _NULLRECT_		GenRect()( 0, 0, 0, 0 )
#define _NULLPOINT_		GenPoint()( 0, 0 )
#define _NULLSIZE_		GenSize()( 0, 0 )




















/*-------------------------------------------------- ���Ͷ��� --------------------------------------------------*/
#ifdef _UNICODE
	typedef std::wstring			tstring;
#else
	typedef std::string				tstring;
#endif


/*-------------------------------------------------- ���ߺ��� --------------------------------------------------*/

// �ַ���ת��
struct WCharToMByte			// UNICODE16 -> ���ֽ�
{   
	void operator()( std::wstring wStr, std::string &str );
};

struct MByteToWChar			// ���ֽ� -> UNICODE16
{
	void operator()( std::string str, std::wstring &wStr );
};

// ��ʽ��string ����CString fromat
struct StringFormat
{
	void operator()( std::string& str, const char* format, ... );
};
#define STRING_FORMAT	StringFormat()

// ʱ��ȴ�����
#define BEGIN_WAIT()						DWORD beginTick = ::GetTickCount(); 
#define END_WAIT( iWate )				{	DWORD endTick = ::GetTickCount(); \
											DWORD dxTick = abs( (int)( endTick-beginTick ) ); \
											if( dxTick<iWate ) \
												Sleep( iWate-dxTick ); \
										}
#define BREAK_WAIT( iWate, flag )		{	DWORD endTick = ::GetTickCount(); \
											DWORD dxTick = abs( (int)( endTick-beginTick ) ); \
											flag = dxTick<iWate ? false:true; \
										}










/*-------------------------------------------------- �߳�ͬ�� --------------------------------------------------*/

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