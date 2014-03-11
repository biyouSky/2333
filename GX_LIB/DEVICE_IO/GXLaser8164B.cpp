#include "stdafx.h"
#include "GXLaser8164B.h"
using namespace std;

Laser816XBase::Laser816XBase() 
{
	_defaultRM=0;
	_vi=0;

	_addrStr = "GPIB0::20::INSTR";
}

Laser816XBase::~Laser816XBase()  { /*Close();*/ }

bool Laser816XBase::IsOpenGPIB() { return _vi!=0; }

bool Laser816XBase::OpenGPIB()
{
	if( IsOpenGPIB() ) return true;

	ViStatus rt; 
	rt = viOpenDefaultRM( &_defaultRM );
	if( rt!=VI_SUCCESS ) return false;

	rt = viOpen( _defaultRM, (ViRsrc)_addrStr.c_str(), VI_NULL, 20000, &_vi );
	return rt==VI_SUCCESS ? true:false;
}

void Laser816XBase::CloseGPIB()
{
	if( _vi ) viClose( _vi ); 
	if( _defaultRM ) viClose( _defaultRM );

	_vi=0;
	_defaultRM=0;  
}

inline bool Laser816XBase::SendCmd( string cmd )
{
	ViStatus rt; 
	rt = viPrintf( _vi, ( ViString )cmd.c_str() );
	return rt!=VI_SUCCESS ?  false:true;
}


bool Laser8164B::Open()
{
	if( !OpenGPIB() ) return false;

	if( !UnLock() ) return false;
	if( !PowStat() ) return false;

	return true;
}

void Laser8164B::Close()
{
	PowStat( false );

	CloseGPIB();
}

bool Laser8164B::Lock()
{
	return SendCmd( "lock 1,1234\n");
}

bool Laser8164B::UnLock()
{
	return SendCmd( "lock 0,1234\n");
}

bool Laser8164B::PowStat( bool stat )
{
	if( stat )
		return SendCmd( "sour0:pow:stat 1\n");
	else
		return SendCmd( "sour0:pow:stat 0\n");
}

bool Laser8164B::SetWavelength( string waveLen )
{
	ViStatus rt; 
	rt = viPrintf( _vi, "sour0:wav %snm\n", waveLen.c_str() );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8164B::SetPowerDB( float db )
{
	ViStatus rt; 
	rt = viPrintf( _vi, "sour0:pow %0.2fdBm\n", db );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8164B::SetTrigOutpStf()
{
	return SendCmd( "trig0:outp stf\n");
}

bool Laser8164B::SetSweMode( int flag )			// flag= 0 cont|1 step|2 man
{
	if( flag==0 )
		return SendCmd( "wav:swe:mode cont\n");
	else if( flag==1 )
		return SendCmd( "wav:swe:mode step\n");
	else if( flag==2 )
		return SendCmd( "wav:swe:mode man\n");
	else
		return false;
}

bool Laser8164B::SetSweCyc( int cyc )
{
	ViStatus rt; 
	rt = viPrintf( _vi, "wav:swe:cycles %d\n", cyc );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8164B::SetSweStarStop( float star, float stop )
{	// 设置起始波长  注意超过激光器支持范围无效
	ViStatus rt; 
	rt = viPrintf( _vi, "wav:swe:star %fnm\n", star );
	if( rt!=VI_SUCCESS )
		return false;	

	rt = viPrintf( _vi, "wav:swe:stop %fnm\n", stop );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8164B::SetSweSpeedStep( float spe, float step )
{	// 8164A支持的速率
	if( spe!=0.5 && spe!=5 && spe!=10 && spe!=20 && spe!=40 )
	{
		string str;
		STRING_FORMAT( str, "Laser8164B sweep not support %f speed..", spe );
		//_debugTrace.PushMsg( str );
		return false;
	}

	ViStatus rt; 
	rt = viPrintf( _vi, "wav:swe:spe %fnm/s \n", spe );
	if( rt!=VI_SUCCESS )
		return false;	

	rt = viPrintf( _vi, "wav:swe:step %fnm\n", step );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8164B::SetSweStart()
{
	return SendCmd( "wav:swe STAR\n");
}

bool Laser8164B::SetSweStop()
{
	return SendCmd( "wav:swe STOP\n");
}

bool Laser8164B::SetSwePause()
{
	return SendCmd( "wav:swe PAUS\n");
}

bool Laser8164B::SetSweContinue()
{
	return SendCmd( "wav:swe CONT\n");
}

bool Laser8164B::OpenLog()
{
	return SendCmd( "wav:swe:llog 1\n");
}

bool Laser8164B::CloseLog()
{
	return SendCmd( "wav:swe:llog 0\n");
}

int Laser8164B::ReadLogDataPointNum()
{
	ViStatus rt;
	int num=0;
	rt = viQueryf( _vi, "source0:read:points? llog\n", "%d", &num );
	return rt!=VI_SUCCESS ?  -1:num;
}

// 读取点数目已2进制流的形式读取llog中记录的每个波长，每个2进制块都是英特尔序( 主机序 )8-byte长，类型为double
/*
	1byte 1byte xbyte 8byte( 1 ) 8byte( 2 ) …… 8byte( n )
	'#'   'x'	"n*8" 波长1    波长2         波长n
*/
bool Laser8164B::ReadLogData( vector<double>& channelList )
{
	int num = ReadLogDataPointNum();
	if( num==-1 )
		return false;

	ViStatus rt; 
	rt = viPrintf( _vi, "source0:read:data? llog\n");
	if( rt!=VI_SUCCESS )
		return false;

	// 分配缓存
	char *pCache = NULL;
	pCache = new char[num*8+10];
	memset( pCache, 0, num*8+10 );
	Sleep( 5000 );		// 防止老型号激光器反应不过来
	
	// 读取数据
	rt = viScanf( _vi, "%t", pCache );
	if( rt!=VI_SUCCESS )
	{
		delete []pCache;
		return false;
	}		

	int j = 1;
	for( int i=0; i<num; ++i )
	{
		double d;
		int offset = 2+pCache[1]-48;
		memcpy( &d, pCache+offset+i*8, 8 );
		channelList.push_back( d*1000000000 );
	}
	
	delete []pCache;
	return true;
}

bool Laser8164B::SweContModeTest( float start, float stop, float speed, float step, vector<double>& channelList )
{
	if( Open()==false )
		return false;

	// 设置脉冲触发
	if( SetTrigOutpStf()==false )
		return false;

	// 设置sweep模式
	if( SetSweMode( 0 )==false )
		return false;
	if( SetSweCyc( 1 )==false ) 
		return false;
	if( SetSweStarStop( start, stop )==false ) 
		return false;
	if( SetSweSpeedStep( speed, step )==false ) 
		return false;
	if( OpenLog()==false ) 
		return false;
	if( SetSweStart()==false ) 
		return false;

	// 等待sweep模式运行完成
	DWORD waitTime = static_cast<DWORD>( ( stop-start )/speed + 10 );
	Sleep( waitTime*1000 );

	// 读取数据
	channelList.clear();
	return ReadLogData( channelList );
}



bool Laser8163B::Open()
{
	return OpenGPIB();
}

void Laser8163B::Close()
{
	CloseGPIB();
}

bool Laser8163B::SetWavelength( string waveLen )
{
	ViStatus rt; 
	rt = viPrintf( _vi, "SOUR1:CHAN1:wav %snm\n", waveLen.c_str() );
	return rt!=VI_SUCCESS ?  false:true;
}

bool Laser8163B::SetPowerDB( float db )
{
	ViStatus rt; 
	rt = viPrintf( _vi, "SOUR1:CHAN1:pow %0.2fdBm\n", db );
	return rt!=VI_SUCCESS ?  false:true;
}
