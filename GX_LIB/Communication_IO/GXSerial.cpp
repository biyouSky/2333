
#include "stdafx.h"
#include "GXSerial.h"
using namespace std;

SerialBase::SerialBase()
{
	_comHandle = INVALID_HANDLE_VALUE;

	memset( &_dcb, 0, sizeof( _dcb ) );
	_dcb.DCBlength = sizeof( _dcb );
	
	memset( &_readOverlapped, 0, sizeof( _readOverlapped ) );
	_readOverlapped.hEvent = ::CreateEvent( NULL, true, false, NULL );
	assert( _readOverlapped.hEvent != INVALID_HANDLE_VALUE );

	memset( &_writeOverlapped, 0, sizeof( _writeOverlapped ) );
	_writeOverlapped.hEvent = ::CreateEvent( NULL, true, false, NULL );
	assert( _writeOverlapped.hEvent != INVALID_HANDLE_VALUE );

	memset( &_waitOverlapped, 0, sizeof( _waitOverlapped ) );
	_waitOverlapped.hEvent = ::CreateEvent( NULL, true, false, NULL );
	assert( _waitOverlapped.hEvent != INVALID_HANDLE_VALUE );
	
	_threadRunFlag = false;
	_helpThreadHandle = NULL;
}

SerialBase::~SerialBase()
{
	Close(); 
	
	if( _readOverlapped.hEvent != INVALID_HANDLE_VALUE )
		CloseHandle( _readOverlapped.hEvent );
	
	if( _writeOverlapped.hEvent != INVALID_HANDLE_VALUE )
		CloseHandle( _writeOverlapped.hEvent );
	
	if( _waitOverlapped.hEvent != INVALID_HANDLE_VALUE )
		CloseHandle( _waitOverlapped.hEvent );
}

bool SerialBase::SetComm( DWORD baudRate )
{
	if( IsOpen()==false )
		return false;

	if( ::GetCommState( _comHandle, &_dcb ) != TRUE )
		return false;
    _dcb.BaudRate = baudRate;

	return ::SetCommState( _comHandle, &_dcb ) == TRUE;
}

bool SerialBase::Open( DWORD port, DWORD dwBaudRate )
{
	if( port<1 || port>1024 )
		return false;

	// �����Ѿ�����ر�
	if( IsOpen() )
		Close();

	// ��֯buf
	_comPort = port;
	char comPortBuf[32];
	memset( comPortBuf, 0, 32 );
	sprintf_s( comPortBuf, 32, "COM%d", port );

	// �������ھ��
	_comHandle = ::CreateFileA( 
		comPortBuf,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL
		 );
	if( _comHandle==( HANDLE )0xffffffff ) // ��Ч���
		return false;

	// ���ô�������
	if( !::GetCommState( _comHandle, &_dcb ) )
		return false;
	_dcb.BaudRate = dwBaudRate;
	_dcb.ByteSize = 8;
	_dcb.Parity   = NOPARITY;
	_dcb.StopBits = ONESTOPBIT;
	::SetCommState( _comHandle, &_dcb );
	
	// ���ô��ڻ����С ��ջ���
	if( !::SetupComm( _comHandle, 4096, 4096 ) )
		return false; 
	if( !::PurgeComm( _comHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) )
		return false; 
	
	// ���ô��ڳ�ʱ��Ϣ
	if( !::GetCommTimeouts( _comHandle, &_co ) )
		return false;
	_co.ReadIntervalTimeout = 0xFFFFFFFF;
	_co.ReadTotalTimeoutMultiplier = 0;
	_co.ReadTotalTimeoutConstant = 0;
	_co.WriteTotalTimeoutMultiplier = 0;
	_co.WriteTotalTimeoutConstant = 2000;
	if( !::SetCommTimeouts( _comHandle, &_co ) )
		return false;

	// �����߳�
	if( BeginThread()==false )
	{
		Close(); 
		return false;
	}

	return true;
}

void SerialBase::Close()
{
	if( IsOpen() )  
	{
		PurgeComm( _comHandle, PURGE_TXABORT | PURGE_TXCLEAR );
		
		EndThread();

		::CloseHandle( _comHandle );
		_comHandle = INVALID_HANDLE_VALUE;
	}
}

DWORD SerialBase::Read( LPVOID Buffer, DWORD dwBufferLength )
{
	if( !IsOpen() )
		return 0;
	
	COMSTAT  Stat;
	DWORD dwError;
	
	if( ::ClearCommError( _comHandle, &dwError, &Stat ) && dwError > 0 )
	{
		::PurgeComm( _comHandle, PURGE_RXABORT | PURGE_RXCLEAR );
		return 0;
	}
	
	if( !Stat.cbInQue ) // ������������
		return 0;
	
	unsigned long uReadLength = 0;
	dwBufferLength = dwBufferLength > Stat.cbInQue ? Stat.cbInQue : dwBufferLength;
	
	if( !::ReadFile( _comHandle, Buffer, dwBufferLength, &uReadLength, &_readOverlapped ) )
	{
		if( ::GetLastError() == ERROR_IO_PENDING )
		{
			WaitForSingleObject( _readOverlapped.hEvent, 20 ); // �����첽I/O
			if( !::GetOverlappedResult( _comHandle, &_readOverlapped, &uReadLength, false ) )
			{
				if( ::GetLastError() != ERROR_IO_INCOMPLETE )
					uReadLength = 0;
			}
		}
		else
			uReadLength = 0;
	}
	
	return uReadLength;
}

DWORD SerialBase::Write( LPVOID Buffer, DWORD dwBufferLength )
{
	if( !IsOpen() )
		return 0;
	
	DWORD dwError;
	
	if( ::ClearCommError( _comHandle, &dwError, NULL ) && dwError > 0 ) 
		::PurgeComm( _comHandle, PURGE_TXABORT | PURGE_TXCLEAR ); 
	
	unsigned long uWriteLength = 0;
	if( !::WriteFile( _comHandle, Buffer, dwBufferLength, &uWriteLength, &_writeOverlapped ) )
	{
		DWORD err = ::GetLastError();
        if( err!= ERROR_IO_PENDING )
            uWriteLength = 0;
	}
	return uWriteLength;
}

	// �����߳̿��� �������߳�
bool SerialBase::BeginThread()
{
	if( _threadRunFlag==false )
	{
		_helpThreadHandle = ::CreateThread( NULL, 0, CommThreadProc, this, 0, NULL );
		if( _helpThreadHandle==NULL )
			return false;

		_threadRunFlag = true;
		return true;
	}
	return false;
}

	// ��ֹ�߳�
bool SerialBase::EndThread( DWORD dwWaitTime )
{
	if( _threadRunFlag==true )
	{
		::SetCommMask( _comHandle, 0 );
		::SetEvent( _waitOverlapped.hEvent );
		if( ::WaitForSingleObject( _helpThreadHandle, dwWaitTime ) != WAIT_OBJECT_0 )
		{
			if( !::TerminateThread( _helpThreadHandle, 0 ) )
				return false;
		}

		::CloseHandle( _helpThreadHandle );
		::ResetEvent( _waitOverlapped.hEvent );
	
		_threadRunFlag = false;
		_helpThreadHandle = NULL;
		return true;
	}
	return false;
}

DWORD SerialBase::ThreadFunc()
{
	if( !::SetCommMask( _comHandle, EV_RXCHAR ) )
	{
		char szBuffer[256];
		_snprintf( szBuffer, 255, "%s( %d ) : COM%d Call WINAPI SetCommMask( %x, %x ) Fail, thread work invalid! GetLastError() = %d;",
			__FILE__, __LINE__, _comPort, _comHandle, EV_RXCHAR, GetLastError() );
		MessageBoxA( NULL, szBuffer, "Class SerialBase", MB_OK );
		return 1;
	}
	
	COMSTAT Stat;
	DWORD dwError;
	
	for( DWORD dwLength, dwMask = 0; _threadRunFlag && IsOpen(); dwMask = 0 )
	{
		if( !::WaitCommEvent( _comHandle, &dwMask, &_waitOverlapped ) )
		{
			if( ::GetLastError() == ERROR_IO_PENDING )// asynchronous
				::GetOverlappedResult( _comHandle, &_waitOverlapped, &dwLength, TRUE );
			else
				continue;
		}
		
		if( dwMask == 0 )
			continue;
		
		switch( dwMask )
		{
		case EV_RXCHAR :
			::ClearCommError( _comHandle, &dwError, &Stat );
			if( Stat.cbInQue >= 1 )
				ComRecv();
			break;
			
		}// case
    }// for
	return 0;
}




#define BEGIN_COM()			_sendWaitFlag = false;
#define WAIT_COM( iWait )	{	if( iWait>3 ) \
								{ \
									DWORD dwTk=GetTickCount(); \
									Sleep( 1 ); \
									while( GetTickCount()-dwTk<iWait ) \
									{ \
										if( _sendWaitFlag )  \
											break; \
										Sleep( 1 ); \
									} \
									_sendWaitFlag = false; \
								} \
								else { Sleep( iWait ); } \
							}





SerialASYN::SerialASYN()
{
	// ��ʼ����־
	//_recvDelay = 50;
	_sendWaitFlag = false;
}

SerialASYN::~SerialASYN()
{
}

inline bool SerialASYN::SendCheck()
{
	if( IsOpen()==false )
	{
		::MessageBoxA( NULL, "Serial port is close!", "warning", MB_OK );
		return false;
	}

	return true;
}

void SerialASYN::ComSend( char *pBuf, int bufLen, ULONG recvDelay, ULONG sendDelay )
{
	if( SendCheck()==false )
		return;

	// ���첽תͬ���ķ�װ��  ��ȴ����յ����
	if( sendDelay<recvDelay+10 )
		sendDelay = recvDelay+10;

	// ����ͬ���ظ����ȴ�ʱ��
	_recvDelay = recvDelay;

	BEGIN_COM()
	Write( ( LPVOID )pBuf, bufLen );
	WAIT_COM( sendDelay )
}

void SerialASYN::ComRecv()
{
	// ���ڽϴ������� ��ֹ�������ʹ��쵼�µ�һ���Խ��ղ����� ��ʱһ��ʱ��
	Sleep( _recvDelay );

	// �Ӵ��ڶ�ȡ����	
	memset( _cacheBuf, 0, 4096 );
	int readLen = Read( (LPVOID)_cacheBuf, 4096 );
	if( readLen>0 )
	{
		if( ParaseRes( string( _cacheBuf, readLen ) ) )
			_sendWaitFlag = true;	// �����ɹ�
	}

	//memset( _cacheBuf, 0, 4096 );
	//DWORD dwTk=GetTickCount(); 
	//Sleep( 3 ); 
	//int readLen = 0;
	//do
	//{ 
	//	int len = Read( (LPVOID)( _cacheBuf+readLen ), 4096-readLen );
	//	readLen += len;
	//	Sleep( 1 ); 
	//}
	//while( GetTickCount()-dwTk<_recvDelay ) ;

	//if( readLen>0 )
	//{
	//	if( ParaseRes( string( _cacheBuf, readLen ) ) )
	//		_sendWaitFlag = true;	// �����ɹ�
	//}
}