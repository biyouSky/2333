/*
 2012.12.24 ������ʽһ��Ҫװgpib�� ԭ��δ֪

 ÿһ��ѯ������ һ��Ҫ��ʱ��ȡ���� ����ܿ��ܻ���ϴε�ѯ������

*/



#pragma once

#include "..\Utility\GXUtil.h"
#include "visa.h"
#pragma comment( lib, "visa32.lib" )


class OSA
{
public:
	OSA();
	~OSA();

	void (*pCB_ShowMsg)( const char* msg );

	bool ConnnectAgilent( std::string addr="GPIB0::23::INSTR" );
	bool ConnnectYokogawa( std::string addr="GPIB0::1::INSTR" );
	bool ConnnectTCPIP( ULONG ip, ULONG port );
	void DisConnect();

	bool GetWavRange( float& start, float& end );

	bool GetTraceA( std::string& infor );
	bool GetTraceB( std::string& infor );
	bool GetTraceC( std::string& infor );

	void SendCmd( std::string cmd );
	void RecvRes( std::string& res );

protected:
	inline void ShowMsg( std::string str );
	bool Connect( std::string osaAddr, int connectFlag=1 );  // 1: gpib  2: network

	bool GetTrace( char flag, std::string& info );

	ViSession _defaultRM;
	ViSession _vi; 

	std::string _addrStr;
};