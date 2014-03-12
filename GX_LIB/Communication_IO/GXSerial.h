/*-------------------------------------------------------------------
串口通信类：已线程方式封装了串口数据的收 发 以及解析过程
使用时只需重载函数ParaseRes()
public接口用于公用操作

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
	
	// 判断串口是或打开
	inline bool IsOpen() { return _comHandle!=INVALID_HANDLE_VALUE; }
	bool Open( DWORD dwPort, DWORD dwBaudRate=115200 );	// 打开串口 缺省 115200, 8, n, 1
	void Close();										// 关闭串口 同时也关闭关联线程

	bool SetComm( DWORD baudRate );

protected:
	static DWORD WINAPI CommThreadProc( LPVOID lpPara )
	{
		return ( ( SerialBase * )lpPara )->ThreadFunc();
	}

	// 读取串口 dwBufferLength个字符到 Buffer 返回实际读到的字符数  可读任意数据
	DWORD Read( LPVOID Buffer, DWORD dwBufferLength );
	DWORD Write( LPVOID Buffer, DWORD dwBufferLength );
	
	// 辅助线程控制
	bool BeginThread();
	bool EndThread( DWORD dwWaitTime=100 );
	DWORD ThreadFunc();

	virtual void ComRecv()=0;		// 接受处理

	DWORD _comPort;					// 串口号
	HANDLE _comHandle;				// 串口句柄
	
	DCB _dcb;						// 波特率，停止位，等  
	COMMTIMEOUTS _co;				// 超时结构  
	
	DWORD _dwIOMode;				// 0 同步  默认 FILE_FLAG_OVERLAPPED 重叠I/O 异步
	OVERLAPPED _readOverlapped, _writeOverlapped; // 重叠I/O
	OVERLAPPED _waitOverlapped;		// WaitCommEvent use

	HANDLE _helpThreadHandle;		// 辅助线程
	bool _threadRunFlag;			// 线程运行循环标志
};

// 异步串口处理
class SerialASYN : public SerialBase
{
public:
	// 析构 构造
	SerialASYN();
	virtual ~SerialASYN();

	// 
	bool SendCheck();
	void ComSend( char *pBuf, int bufLen, ULONG recvDelay=10, ULONG sendDelay=30 );

protected:
	virtual bool ParaseRes( std::string& res ) = 0;
	void ComRecv();

	char _cacheBuf[4096];			// 缓冲大小
	DWORD _recvDelay;				// 接收延时

	bool _sendWaitFlag;
};