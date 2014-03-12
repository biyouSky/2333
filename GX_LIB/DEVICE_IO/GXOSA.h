/*
 2012.12.24 网口形式一定要装gpib库 原因未知

 每一次询问命令 一定要及时获取数据 否则很可能获得上次的询问数据

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