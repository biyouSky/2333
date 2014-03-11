/*
	Add:	2011-6-22
	Update:	2011-6-22


	安利 ML9001A 光功率计
*/
#pragma once

#include "..\Utility\GXUtil.h"
#include "visa.h"
#pragma comment( lib, "visa32.lib")

class OPMML9001A
{
protected:
	typedef int ( __stdcall INIT ) ();
	// typedef int ( __stdcall SEND ) ( int, int, char *, int, int );
	typedef int ( __stdcall RECEIVE ) ( int, int, char *, int, int );

	INIT* _init;
	// SEND * _send;
	RECEIVE* _receive;

	HINSTANCE _dllHandle;

	int _iAddr;

public:
	bool Init( int addr )
	{
		HINSTANCE _dllHandle = LoadLibraryA( "Gpib32.dll");
		if( _dllHandle==NULL )
			return false;

		_init = ( INIT * )GetProcAddress( _dllHandle,"Init");
		//_send=( SEND * )GetProcAddress( _dllHandle,"Send");
		_receive = ( RECEIVE * )GetProcAddress( _dllHandle,"Receive");

		assert( _init!=NULL );
		assert( _receive!=NULL );

		_iAddr = addr;

		return true;
	}

	void Close()
	{
		_init = NULL;
		//_send = NULL;
		_receive = NULL;
		FreeLibrary( _dllHandle );
	}

	float ReadPower()
	{
_FLAG:
		int rs;
		char recvBuf[100], result[30];
		memset( recvBuf, 0, 100 );
		memset( result, 0, 30 );

		// 初始化并读取
		_init();
		//rs=_send( 0, _iAddr, sendBuf, 0, 0 );
		rs=_receive( 0, _iAddr, recvBuf, 0, 0 );
		rs=_receive( 0, _iAddr, recvBuf, 0, 0 );			// ??一遍读取失败 设置问题??

		// 找到第一个空格  找到初始位置
		int start, end;
		int k;
		for( k=0; k<100; ++k )
		{	
			if( recvBuf[k]!='+' && recvBuf[k]!='-' )
				continue;
			
			start = k;
			break;
		}
		if( k==100 )
			goto _FLAG;

		// 找到第一个ASSCI码为0的作为结束位置
		int i=start;
		while( recvBuf[i++]!=0 );
		end = i;

		int j=0;
		for( int i=start; i<end; ++i )
		{
			if( recvBuf[i]==' ' )
				continue;

			result[j++] = recvBuf[i];
		}

		return (float)atof( result );
	}
};