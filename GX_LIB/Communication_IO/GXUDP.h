/*
	Add:	2011-6-17
	Update:	2011-6-22

	ֻ�ṩ��򵥵� ���� ���չ���
	֧�ֹ㲥 ������ʱ�ȴ�
	���ֿͻ�����������  ֻ��local remote
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
	// localPort:	���ط���port; ���ؼ���port; ��Ϊ-1, �򱾵ط���port�ǵ�һ�η���ʱ���ָ��
	// remotePort:	���ط��͵�Զ�˵�Ŀ��port
	UdpOBJ( int localPort=-1, int remotePort=9000 );
	~UdpOBJ();

	// ���ýӿ�
	// ��������
	int RecvFrom( char * recvBuf, int recvLen, SOCKADDR_IN& recvAddr, int waitTime=500 );
	int RecvFrom( char * recvBuf, int recvLen, int waitTime=500, ULONG* pRemoteIP=NULL, USHORT* pRemotePort=NULL );
	// ��������
	int SendTo( const char* sendBuf, int sendLen, const ULONG& addrDst );
	int SendToBroadCast( const char* sendBuf, int sendLen, const ULONG broadCastAddrDst=0xFFFFFFFF );

	std::string GetLastErrMsg();

protected:
	SOCKET _socket;
	SOCKADDR_IN _sendAddr;				// ��������ַ
	SOCKADDR_IN _sendBroadCastAddr;		// �㲥��ַ

	std::string _errMsg;
};