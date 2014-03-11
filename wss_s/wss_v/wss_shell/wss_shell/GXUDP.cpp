
#include "stdafx.h"
#include "GXUDP.h"
using namespace std;



UdpOBJ::UdpOBJ( int localPort, int remotePort )
{
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD( 2, 2 );

	// ����winsock
	int err;
	err = WSAStartup( version, &wsaData );
	if ( err != 0 ) 
	{
		::MessageBoxA( NULL, "can't find a useable WinSock DLL", "Warning", 0 );
		return;
	}

	// ȷ��winsock�汾
	if ( LOBYTE( wsaData.wVersion )!=2 || HIBYTE( wsaData.wVersion )!=2 ) 
	{    
		::MessageBoxA( NULL, "wsaData.Version!=2.2", "Warning", 0 );
		WSACleanup();
		return; 
	}

	// ��ʼ������ַ��Ϣ
	// 
	_sendAddr.sin_family = AF_INET;
	_sendAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	_sendAddr.sin_port = htons( remotePort );
	// 
	_sendBroadCastAddr.sin_family = AF_INET;
	_sendBroadCastAddr.sin_addr.S_un.S_addr = htonl( 0xFFFFFFFF );
	_sendBroadCastAddr.sin_port = htons( remotePort );

	// ����socket��־
	_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	// �����ù㲥����
	char para = 1;
	setsockopt( _socket, SOL_SOCKET, SO_BROADCAST, (const char *)&para, 1 );

	// �󶨱���port
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons( localPort );
	err = ::bind( _socket, (struct sockaddr *)&addr, sizeof(SOCKADDR_IN) );
	//if( err!=0 )
	//{
	//	//::MessageBoxA( NULL, GetSysErrorMsg()( WSAGetLastError() ).c_str(), "Warning", 0 );
	//	//::MessageBoxA( NULL, "bind local port fail!", "Warning", 0 );
	//}
}

UdpOBJ::~UdpOBJ()
{
	//closesocket( _socket );				// �ͷ����ӵ��׽���
	//WSACleanup();						// ֹͣʹ��winsock
}

//// �������󷵻�-1 �ɹ������յ����ֽ���
//int UdpOBJ::RecvFrom( char * recvBuf, int recvLen, SOCKADDR_IN& recvAddr, int waitTime )
//{
//	// ���õȴ�ʱ��
//	if( waitTime!=0 )
//		setsockopt( _socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&waitTime, 4 );
//
//	int addrSize = sizeof( SOCKADDR );
//	int ret = recvfrom( _socket, recvBuf, recvLen, 0, (SOCKADDR*)&recvAddr, &addrSize );
//	//if( ret == SOCKET_ERROR )
//	//	::MessageBoxA( NULL, GetSysErrorMsg()( WSAGetLastError() ).c_str(), "Warning", 0 );
//	
//	return ret;
//}
//
//int UdpOBJ::RecvFrom( char * recvBuf, int recvLen, int waitTime, ULONG* pRemoteIP, USHORT* pRemotePort )
//{
//	SOCKADDR_IN recvAddr;
//	int rt = RecvFrom( recvBuf, recvLen, recvAddr, waitTime );
//	if( rt<0 )
//		return rt;
//
//	if( pRemoteIP!=NULL )
//		*pRemoteIP = ntohl( recvAddr.sin_addr.S_un.S_addr );
//	if( pRemotePort!=NULL )
//		*pRemotePort = ntohs( recvAddr.sin_port );
//	return rt;
//}
//
//int UdpOBJ::SendTo( const char* sendBuf, int sendLen, const ULONG& addrDst )
//{
//	_sendAddr.sin_addr.S_un.S_addr = htonl( addrDst );
//	return sendto( _socket, sendBuf, sendLen, 0, (SOCKADDR*)&_sendAddr, sizeof( SOCKADDR ) );
//}
//
//int UdpOBJ::SendToBroadCast( const char* sendBuf, int sendLen, const ULONG broadCastAddrDst )
//{
//	_sendBroadCastAddr.sin_addr.S_un.S_addr = htonl( broadCastAddrDst );
//	return sendto( _socket, sendBuf, sendLen, 0, (SOCKADDR*)&_sendBroadCastAddr, sizeof( SOCKADDR ) );
//}
