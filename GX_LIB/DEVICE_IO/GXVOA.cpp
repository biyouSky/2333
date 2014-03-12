

#include "stdafx.h"
#include "GXVOA.h"
using namespace std;

VOAXX::VOAXX() 
{ 
	_waveLen=="1550"; 
}

float VOAXX::GetVOA()
{
	char buf[6];
	memset( buf, 0, 6 );

	if( _waveLen=="1550")
		memcpy( buf, ":0?000", 6 );
	else 
		memcpy( buf, ":1?000", 6 );

	_voaValue = 0;
	ComSend( buf, 6 );
	return _voaValue;
}

void VOAXX::ClearVOA()
{
	char buf[6];
	memset( buf, 0, 6 );

	if( _waveLen=="1550")
		memcpy( buf, ":0S000", 6 );
	else 
		memcpy( buf, ":1S000", 6 );

	ComSend( buf, 6 );
}

void VOAXX::SendVOACmd()
{
	char buf[6];
	memset( buf, 0, 6 );

	if( _waveLen=="1550")
		memcpy( buf, ":0F001", 6 );
	else 
		memcpy( buf, ":1F001", 6 );

	ComSend( buf, 6 );
}

void VOAXX::SetVOARelativeAtten( float attenuation )
{
	char buf[6];
	memset( buf, 0, 6 );

	buf[0] = ':';
	if( _waveLen=="1550")
		buf[1] = '0';
	else 
		buf[1] = '1';

	if( attenuation>0 )
	{
		buf[2] = 'F';
	}
	else
	{
		buf[2] = 'B';
		attenuation = -attenuation;
	}
	// 
	int atten = ( int )( attenuation*10 );
	buf[5] = '0'+atten%10;
	atten /= 10;
	buf[4] = '0'+atten%10;
	atten /= 10;
	buf[3] = '0'+atten%10;

	ComSend( buf, 6 );
}

void VOAXX::SetVOAAbsoluteAtten( float attenuation )
{
	if( attenuation<0 )
		return;

	char buf[6];
	memset( buf, 0, 6 );

	buf[0] = ':';
	if( _waveLen=="1550")
		buf[1] = '0';
	else 
		buf[1] = '1';
	buf[2] = '!';
	// 
	int atten = ( int )( attenuation*10 );
	buf[5] = '0'+atten%10;
	atten /= 10;
	buf[4] = '0'+atten%10;
	atten /= 10;
	buf[3] = '0'+atten%10;

	ComSend( buf, 6 );
}

bool VOAXX::ParaseRes( string& res )
{
	const char* pBuf;
	int bufLen;
	pBuf = 	res.c_str();
	bufLen = res.size();

	_voaValue = ( float )strtod( string( pBuf+2, pBuf+5 ).c_str(), 0 )/10;
	
	return true;
}