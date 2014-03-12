/*
	Add:	2011-6-22
	Update:	2011-6-22


	安捷伦 N774x系列 8通道 光功率计
*/
#pragma once

#include "..\Utility\GXUtil.h"
#include "visa.h"
#pragma comment( lib, "visa32.lib")

class OpmN774x
{
protected:
	ViSession _defaultRM, _vi;
	ViStatus _sta; 

protected:
	bool CheckViStatus( std::string str )
	{
		if( VI_SUCCESS!=_sta ) 
		{ 
			char desc[1024] = {0}; 
			viStatusDesc( _vi, _sta, desc ); 

			std::string strMsg = "PM or GPIB wrong!\n";
			strMsg += str.c_str();
			strMsg += "\n";
			strMsg += desc;
			::MessageBoxA( NULL, strMsg.c_str(), "Warning", 0 );
			return false;
		}

		return true;
	}

public:
	OpmN774x()
	{
		_defaultRM=0;
		_vi=0; 
		_sta=0;
	}

	~OpmN774x() { Close(); }
 
public:
	bool Init( std::string gpibAddr, std::string waveBrand )
	{
		Close();

		if( 0==_defaultRM )
		{
			_sta = viOpenDefaultRM( &_defaultRM );
			if( CheckViStatus( "_defaultRM")==false )
				return false;
		}

		if( 0==_vi )
		{
			ViUInt32 timeOut = 20000;
			_sta = viOpen( _defaultRM, (ViRsrc)gpibAddr.c_str(), VI_NULL, timeOut, &_vi );
			if( CheckViStatus( "_vi")==false )
				return false;

			ViReal64 averagingTime = 0.1;
			_sta = viPrintf( _vi,"*CLS\n" );
			Sleep( 50 );

			byte addrs[8][2] = { {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1} };   

			for( int x=0; x<8; ++x )
			{
				if( addrs[x][0]==0 )
				 continue;

				_sta = viPrintf( _vi, "SENS%d:CHAN%d:POW:REF:STATE 0\n", addrs[x][0], addrs[x][1] );    
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );

				// ????????????????????????????auto mode代替手动mode
				// _sta = viPrintf( _vi, "SENS%d:CHAN%d:POW:RANGE  0\n", addrs[x][0], addrs[x][1] );
				// if( CheckViStatus( "PM INIT")==false )
				// 	return false;
				// Sleep( 50 );

				_sta = viPrintf( _vi, "SENS%d:CHAN%d:POW:UNIT dBm\n", addrs[x][0], addrs[x][1] );    
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );

				_sta = viPrintf( _vi, "SENS%d:CHAN%d:POW:ATIME %f\n", addrs[x][0], addrs[x][1], averagingTime );
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );

				_sta = viPrintf( _vi, "INIT%d:CHAN%d:CONT 1\n", addrs[x][0], addrs[x][1] );
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );

				_sta = viPrintf( _vi, "INIT%d:CHAN%d:IMM\n", addrs[x][0], addrs[x][1] );
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );

				_sta = viPrintf( _vi, "SENS%d:POW:WAV %snm\n", addrs[x][0], waveBrand.c_str() );
				if( CheckViStatus( "PM INIT")==false )
					return false;
				Sleep( 50 );
			}
		}
   
		return true;
	 } 

	void Close()
	{
		if( _vi )
			viClose( _vi ); 

		if( _defaultRM )
			viClose( _defaultRM );

		_vi=0;
		_defaultRM=0;  
	}

	bool IsOpen() { return NULL!=_vi; }
 
	bool ReadPower( double fPower[], UCHAR portFlag )
	{
		char strPOWER[8][32] = {"FETC1:CHAN1:pow?\n","FETC2:CHAN1:pow?\n","FETC3:CHAN1:pow?\n",
								"FETC4:CHAN1:pow?\n","FETC5:CHAN1:pow?\n","FETC6:CHAN1:pow?\n",
								"FETC7:CHAN1:pow?\n","FETC8:CHAN1:pow?\n"};

		memset( fPower, 0, sizeof( double )*8 );

		ViReal32 power;
		for( int x=0; x<8; ++x )
		{
			power = 0;
			if( portFlag&( 1<<x ) )
			{
				_sta = viQueryf( _vi,"%s","%f", strPOWER[x], &power );    
				CheckViStatus( "PM read power");
				if ( VI_SUCCESS!=_sta || power<-1000 || power>1000 )
				{
					Sleep( 50 );     
					_sta = viQueryf( _vi,"%s","%f", strPOWER[x], &power );    
					if( VI_SUCCESS!=_sta || power<-1000 || power>1000 )
						return false;
				}

				fPower[x] = power;
				Sleep( 50 );
			}
		}

		return true;
	}
};