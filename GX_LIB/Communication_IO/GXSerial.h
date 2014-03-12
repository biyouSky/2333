/*-------------------------------------------------------------------
����ͨ���ࣺ���̷߳�ʽ��װ�˴������ݵ��� �� �Լ���������
ʹ��ʱֻ�����غ���ParaseRes()
public�ӿ����ڹ��ò���

--------------------------------------------------------------------*/
#pragma once
#include "..\Utility\GXUtil.h"
#include <assert.h>
#include <process.h>

class SerialBase   
{
public:
	SerialBase();
	virtual ~SerialBase();
	
	// �жϴ����ǻ��
	inline bool IsOpen() { return _comHandle!=INVALID_HANDLE_VALUE; }
	bool Open( DWORD dwPort, DWORD dwBaudRate=115200 );	// �򿪴��� ȱʡ 115200, 8, n, 1
	void Close();										// �رմ��� ͬʱҲ�رչ����߳�

	bool SetComm( DWORD baudRate );

protected:
	static DWORD WINAPI CommThreadProc( LPVOID lpPara )
	{
		return ( ( SerialBase * )lpPara )->ThreadFunc();
	}

	// ��ȡ���� dwBufferLength���ַ��� Buffer ����ʵ�ʶ������ַ���  �ɶ���������
	DWORD Read( LPVOID Buffer, DWORD dwBufferLength );
	DWORD Write( LPVOID Buffer, DWORD dwBufferLength );
	
	// �����߳̿���
	bool BeginThread();
	bool EndThread( DWORD dwWaitTime=100 );
	DWORD ThreadFunc();

	virtual void ComRecv()=0;		// ���ܴ���

	DWORD _comPort;					// ���ں�
	HANDLE _comHandle;				// ���ھ��
	
	DCB _dcb;						// �����ʣ�ֹͣλ����  
	COMMTIMEOUTS _co;				// ��ʱ�ṹ  
	
	DWORD _dwIOMode;				// 0 ͬ��  Ĭ�� FILE_FLAG_OVERLAPPED �ص�I/O �첽
	OVERLAPPED _readOverlapped, _writeOverlapped; // �ص�I/O
	OVERLAPPED _waitOverlapped;		// WaitCommEvent use

	HANDLE _helpThreadHandle;		// �����߳�
	bool _threadRunFlag;			// �߳�����ѭ����־
};

// �첽���ڴ���
class SerialASYN : public SerialBase
{
public:
	// ���� ����
	SerialASYN();
	virtual ~SerialASYN();

	// 
	bool SendCheck();
	void ComSend( char *pBuf, int bufLen, ULONG recvDelay=10, ULONG sendDelay=30 );

protected:
	virtual bool ParaseRes( std::string& res ) = 0;
	void ComRecv();

	char _cacheBuf[4096];			// �����С
	DWORD _recvDelay;				// ������ʱ

	bool _sendWaitFlag;
};