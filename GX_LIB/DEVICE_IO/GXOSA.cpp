#include "stdafx.h"
#include "GXOSA.h"
using namespace std;

#define CHECK_RT( rt ) if( rt!=VI_SUCCESS ) return false;


OSA::OSA()
{
	_defaultRM = NULL;
	_vi = NULL;
	pCB_ShowMsg = NULL;
}

OSA::~OSA()
{
}

void OSA::ShowMsg( std::string str )
{
	if( pCB_ShowMsg!=NULL )
		pCB_ShowMsg( str.c_str() );
}

void OSA::DisConnect()
{
	if( _vi ) 
		viClose( _vi );						// ？？

	if( _defaultRM ) 
		viClose( _defaultRM );

	_vi=0;
	_defaultRM=0;  
}

bool OSA::Connect( string osaAddr, int connectFlag )
{
	// 如果连接则先断开
	DisConnect();

	ViStatus rt;
	rt = viOpenDefaultRM( &_defaultRM );
	//CHECK_RT( rt )
	if( rt!=VI_SUCCESS) return false;

	rt = viOpen( _defaultRM, (ViRsrc)osaAddr.c_str(), VI_NULL, VI_TMO_INFINITE, &_vi );
	//CHECK_RT( rt )
	if( rt!=VI_SUCCESS) return false;

	char buf[1024];
	if( connectFlag==2 )		// 匿名用户名 密码
	{
		rt = viPrintf( _vi, "open \"anonymous\"\n" );
		rt = viSetAttribute( _vi, VI_ATTR_TERMCHAR_EN, TRUE );

		viScanf( _vi, "%t", &buf );
		ShowMsg( buf );	
			
		viPrintf( _vi, " \n" );
		viScanf( _vi, "%t", &buf);
		ShowMsg( buf );	
	}

	// 显示连接信息
	memset( buf, 0, 1024 );
	viPrintf( _vi, "*IDN?\n" );	
	viScanf( _vi, "%t", &buf );
	ShowMsg( string("*IDN? ")+buf );		

	return true;
}

bool OSA::ConnnectAgilent( string osaAddr )
{
	return Connect( osaAddr );
}

bool OSA::ConnnectYokogawa( string osaAddr )
{
	return Connect( osaAddr );
}

bool OSA::ConnnectTCPIP( ULONG ip, ULONG port )
{
	string osaAddr;
	STRING_FORMAT( osaAddr, "TCPIP0::%s::%d::SOCKET", IP4_ULONGToStr()( ip ), port );
	return Connect( osaAddr, 2 );
}

bool OSA::GetWavRange( float& wavStart, float& wavEnd )
{
	char buf[256];
	memset( buf, 0, 256 );

	ViStatus rt;

	rt = viPrintf( _vi, "sens:wav:star?\n" );
	rt = viScanf( _vi, "%t", &buf );
	wavStart = (float)( atof( buf )*1E9 );
		
	Sleep( 100 );
	memset( buf, 0, 256 );
	rt = viPrintf( _vi, "sens:wav:stop?\n" );
	rt = viScanf( _vi, "%t", &buf );
	wavEnd = (float)( atof( buf )*1E9 );

	return true;
}

bool OSA::GetTrace( char flag, std::string& info )
{	
	/*
	viPrintf( _vi, "syst:err:next?\n" );
	viScanf( _vi, "%t", &buf);
	ShowMsg( buf );	*/

	char buf[204800];

	ViStatus  rt;
	rt = viSetAttribute( _vi, VI_ATTR_TMO_VALUE, 10000 );		// set the time out 
	rt = viPrintf( _vi, "syst:comm:gpib:buff on\n" );			// 打开光谱仪buf	
	rt = viPrintf( _vi, "INIT:CONT OFF\n" );					// Put OSA not in continuous sweep mode
	
	rt = viPrintf( _vi, "INIT:IMM;*OPC?\n" );					// Take a single sweep and wait complete
	rt = viScanf(  _vi, "%t", &buf );

	//rt = viPrintf( _vi, ":abort" ); 
	//rt = viPrintf( _vi, "INIT:SMOD SING" );					// 将OSA扫描模式设为single
	//rt = viPrintf( _vi, "INIT:IMM" );

	string cmd;
	if( flag=='A' )
		cmd = "Trac:Data:Y? TrA\n";
	else if( flag=='B' )
		cmd = "Trac:Data:Y? TrB\n";
	else if( flag=='C' )
		cmd = "Trac:Data:Y? TrC\n";

	//memset( buf, 0, 204800 );
	rt = viPrintf( _vi, (ViString)cmd.c_str() );					// traceA
	rt = viScanf( _vi, "%t", &buf );

	info = buf;
	return true;
}

bool OSA::GetTraceA( std::string& infor )
{
	return GetTrace( 'A', infor );
}

bool OSA::GetTraceB( std::string& infor )
{
	return GetTrace( 'B', infor );
}

bool OSA::GetTraceC( std::string& infor )
{
	return GetTrace( 'C', infor );
}

void OSA::SendCmd( std::string cmd )
{
	ViStatus  rt = viPrintf( _vi, (char*)cmd.c_str() );
}

void OSA::RecvRes( std::string& res )
{
	char buf[204800];
	ViStatus  rt = viScanf( _vi, "%t", &buf );

	res = buf;
}