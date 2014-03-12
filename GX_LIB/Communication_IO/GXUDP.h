/*
	Add:	2011-6-17
	Update:	2011-6-22

	只提供最简单的 发送 接收功能
	支持广播 接收延时等待
	不分客户、服务器端  只分local remote
*/

#pragma once
#include "..\Utility\GXUtil.h"


#ifdef _WIN32_

#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib")

#include "Iptypes.h"
#include "Iphlpapi.h"
#pragma comment( lib, "Iphlpapi.lib")

#endif


class UdpOBJ
{
public:
	// localPort:	本地发送port; 本地监听port; 若为-1, 则本地发送port是第一次发送时随机指定
	// remotePort:	本地发送到远端的目标port
	UdpOBJ( int localPort=-1, int remotePort=9000 );
	~UdpOBJ();

	// 公用接口
	// 接收数据
	int RecvFrom( char * recvBuf, int recvLen, SOCKADDR_IN& recvAddr, int waitTime=500 );
	int RecvFrom( char * recvBuf, int recvLen, int waitTime=500, ULONG* pRemoteIP=NULL, USHORT* pRemotePort=NULL );
	// 发送数据
	int SendTo( const char* sendBuf, int sendLen, const ULONG& addrDst );
	int SendToBroadCast( const char* sendBuf, int sendLen, const ULONG broadCastAddrDst=0xFFFFFFFF );

	std::string GetLastErrMsg();

protected:
	SOCKET _socket;
	SOCKADDR_IN _sendAddr;				// 服务器地址
	SOCKADDR_IN _sendBroadCastAddr;		// 广播地址

	std::string _errMsg;
};