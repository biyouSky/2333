
#include "stdafx.h"
#include "WSSProBase.h"
using namespace std;


PFunc_View_Type0 PCB_View_ShowMsg				= NULL;
PFunc_View_Type1 PCB_View_ShowSerialNumber		= NULL;
PFunc_View_Type2 PCB_View_ShowType				= NULL;
PFunc_View_Type3 PCB_View_ShowVersion			= NULL;
PFunc_View_Type4 PCB_View_ShowHardDateCode		= NULL;
PFunc_View_Type5 PCB_View_ShowFirmwareDateCode	= NULL;
PFunc_View_Type6 PCB_View_ShowChannelAtten		= NULL;
PFunc_View_Type7 PCB_View_ShowAllChannelAtten	= NULL;
PFunc_View_Type8 PCB_View_ShowProfileID			= NULL;
PFunc_View_Type9 PCB_View_ShowAlarm				= NULL;
PFunc_View_Type10 PCB_View_ShowVoltage			= NULL;
PFunc_View_Type11 PCB_View_ShowTemperature		= NULL;
PFunc_View_Type12 PCB_View_ShowLowTemperature	= NULL;
PFunc_View_Type13 PCB_View_ShowHighTemperature	= NULL;
PFunc_View_Type14 PCB_View_ShowMemWear			= NULL;
PFunc_View_Type15 PCB_View_ShowManufactureID	= NULL;


__declspec(dllexport) void SetCB_View_ShowMsg( PFunc_View_Type0 func )				{ PCB_View_ShowMsg = func; }
__declspec(dllexport) void SetCB_View_ShowSerialNumber( PFunc_View_Type1 func )		{ PCB_View_ShowSerialNumber = func; }
__declspec(dllexport) void SetCB_View_ShowType( PFunc_View_Type2 func )				{ PCB_View_ShowType = func; }
__declspec(dllexport) void SetCB_View_ShowVersion( PFunc_View_Type3 func )			{ PCB_View_ShowVersion = func; }
__declspec(dllexport) void SetCB_View_ShowHardDateCode( PFunc_View_Type4 func )		{ PCB_View_ShowHardDateCode = func; }
__declspec(dllexport) void SetCB_View_ShowFirmwareDateCode( PFunc_View_Type5 func ) { PCB_View_ShowFirmwareDateCode = func; }
__declspec(dllexport) void SetCB_View_ShowChannelAtten( PFunc_View_Type6 func )		{ PCB_View_ShowChannelAtten = func; }
__declspec(dllexport) void SetCB_View_ShowAllChannelAtten( PFunc_View_Type7 func )	{ PCB_View_ShowAllChannelAtten = func; }
__declspec(dllexport) void SetCB_View_ShowProfileID( PFunc_View_Type8 func )		{ PCB_View_ShowProfileID = func; }
__declspec(dllexport) void SetCB_View_ShowAlarm( PFunc_View_Type9 func )			{ PCB_View_ShowAlarm = func; }
__declspec(dllexport) void SetCB_View_ShowVoltage( PFunc_View_Type10 func )			{ PCB_View_ShowVoltage = func; }
__declspec(dllexport) void SetCB_View_ShowTemperature( PFunc_View_Type11 func )		{ PCB_View_ShowTemperature = func; }
__declspec(dllexport) void SetCB_View_ShowLowTemperature( PFunc_View_Type12 func )	{ PCB_View_ShowLowTemperature = func; }
__declspec(dllexport) void SetCB_View_ShowHighTemperature( PFunc_View_Type13 func ) { PCB_View_ShowHighTemperature = func; }
__declspec(dllexport) void SetCB_View_ShowMemWear( PFunc_View_Type14 func )			{ PCB_View_ShowMemWear = func; }
__declspec(dllexport) void SetCB_View_ShowManufactureID( PFunc_View_Type15 func )	{ PCB_View_ShowManufactureID = func; }












WSSProBase* WSSProBase::_pSingleton				= NULL;




WSSCommand WSSBasePro::GetSerialNumber()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x01;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetType()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x02;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetVersion()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x03;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::SetRS232BaudRate( int baudRate )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x04;
	cmd._dataLen = 0x02;

	cmd._dataBuf[0] = ( baudRate/100 )>>8;
	cmd._dataBuf[1] = baudRate/100;

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetHardwareDateCode()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x05;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetFirmwareDateCode()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x06;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::SetChannelPortAtten( int c, UCHAR p, float d )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x11;
	cmd._dataLen = 0x03;

	cmd._dataBuf[0] = c;
	cmd._dataBuf[1] = p;
	cmd._dataBuf[2] = (UCHAR)( ( d+0.01 )*10 );

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetChannelPortAtten( int c )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x12;
	cmd._dataLen = 0x01;

	cmd._dataBuf[0] = c;

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::SetAllChannelsPortAtten( UCHAR *p, float *d, int n )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x13;
	cmd._dataLen = 2*n;

	for( int i=0; i<n; ++i )
	{
		cmd._dataBuf[i] = p[i];
		cmd._dataBuf[n+i] = (UCHAR)( 10*( d[i]+0.01 ) );
	}

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetAllChannelsPortAtten()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x14;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetAlarm()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x21;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetVoltage()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x22;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetTemperature()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x23;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::SetLowTemperatureAlarmThreshold( float temperature )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x31;
	cmd._dataLen = 0x02;

	short temp = static_cast<short>( temperature*10 );
	cmd._dataBuf[0] = (UCHAR)( temp>>8 );	
	cmd._dataBuf[1] = (UCHAR)( temp );	

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetLowTemperatureAlarmThreshold()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x32;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::SetHighTemperatureAlarmThreshold( float temperature )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x33;
	cmd._dataLen = 0x02;

	short temp = static_cast<short>( temperature*10 );
	cmd._dataBuf[0] = (UCHAR)( temp>>8 );	
	cmd._dataBuf[1] = (UCHAR)( temp );			

	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::GetHighTemperatureAlarmThreshold()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x34;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();

	return cmd;
}

WSSCommand WSSBasePro::StartFirmwareLoading()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x41;
	cmd._dataLen = 0x02;
	cmd._dataBuf[0] = (UCHAR)0xA5;	
	cmd._dataBuf[1] = (UCHAR)0xC3;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::LoadNewFirmware()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x42;
	cmd._dataLen = 0;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SwitchtoNewFirmware()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x43;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::GetManufactureID()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = (UCHAR)0xAA;
	cmd._dataLen = 0x00;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::GetAD5535( int c )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = (UCHAR)0xF4;
	cmd._dataLen = 0x01;
	cmd._dataBuf[0] = c;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SetDACVoltage( UINT channel, int coord, USHORT vol )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = (UCHAR)0xF0;
	cmd._dataLen = 0x04;
	cmd._dataBuf[0] = coord;
	cmd._dataBuf[1] = channel;
	cmd._dataBuf[2] = ( vol&0xFF00 )>>8;
	cmd._dataBuf[3] = (UCHAR)vol;	
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x15;
	cmd._dataLen = 2*CHANNEL_NUM+1;
	cmd._dataBuf[0] = pID;
	memcpy( cmd._dataBuf+1, portList, CHANNEL_NUM );
	memcpy( cmd._dataBuf+1+CHANNEL_NUM, attenList, CHANNEL_NUM );
	cmd.CalcCheckSum();
	  
	return cmd;
}

WSSCommand WSSBasePro::ReadChannelProfile( UCHAR pID )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x16;
	cmd._dataLen = 1;
	cmd._dataBuf[0] = pID;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SelectChannelProfile( UCHAR pID )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x17;
	cmd._dataLen = 1;
	cmd._dataBuf[0] = pID;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::ReadProfileID()
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0x18;
	cmd._dataLen = 0;
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0xF3;
	cmd._dataLen = 0x0A;
	cmd._dataBuf[0] = 0x01;
	//
	cmd._dataBuf[1] = (UCHAR)( sn>>24 );
	cmd._dataBuf[2] = (UCHAR)( sn>>16 );
	cmd._dataBuf[3] = (UCHAR)( sn>>8 );
	cmd._dataBuf[4] = (UCHAR)( sn );
	//
	cmd._dataBuf[5] = type;
	//
	cmd._dataBuf[6] = hv1;
	cmd._dataBuf[7] = hv2;
	cmd._dataBuf[8] = hd1;
	cmd._dataBuf[9] = hd2;
	//
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0xF3;
	cmd._dataLen = 0x06;
	cmd._dataBuf[0] = 0x02;
	//
	cmd._dataBuf[1] = flag1;
	cmd._dataBuf[2] = flag2;
	cmd._dataBuf[3] = tempNum;
	cmd._dataBuf[4] = lowTemp;
	cmd._dataBuf[5] = highTemp;
	//
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SetModuleInfo3( USHORT volList[104] )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0xF3;
	cmd._dataLen = 0xD1;
	cmd._dataBuf[0] = 0x03;
	//
	for( int i=0; i<104; i++ )
	{
		volList[i] = htons( volList[i] );
		cmd._dataBuf[2*i+1] = (UCHAR)( volList[i]>>8 );
		cmd._dataBuf[2*i+2] = (UCHAR)( volList[i] );
	}
	//
	cmd.CalcCheckSum();
	
	return cmd;
}

WSSCommand WSSBasePro::SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] )
{
	WSSCommand cmd;
	memset( &cmd, 0, sizeof( WSSCommand ) );
	cmd._code = 0xF3;
	cmd._dataLen = 0xD3;
	cmd._dataBuf[0] = 0x04;
	cmd._dataBuf[1] = port;
	cmd._dataBuf[2] = f;
	//
	for( int i=0; i<104; i++ )
	{
		volList[i] = htons( volList[i] );
		cmd._dataBuf[2*i+3] = (UCHAR)( volList[i]>>8 );
		cmd._dataBuf[2*i+4] = (UCHAR)( volList[i] );
	}
	//
	cmd.CalcCheckSum();
	
	return cmd;
}

void WSSBasePro::HandleParseRes( string& strRes, UCHAR op, bool& rt, string& strMsg, vector<Var>& _resultList )
{
	rt = true;

	// 构造res
	const char* pBuf = strRes.c_str();
	UINT bufLen = strRes.size();
	// 
	WSSResponse res;
	memcpy( &res, pBuf, bufLen );

	// 检查包完整性
	if( strRes.size()<3 || strRes.size()!=(UCHAR)strRes[1]+3 )
	{
		STRING_FORMAT( strMsg , "RecvLen: %d.. Packet is not complete..", strRes.size() );
		// 
		rt = false;
		return;
	}

	// 检查校验和
	if( res.CalcCheckSum()==false )
	{
		strMsg = "Checksum error..";
		// 
		rt = false;
		return;
	}
	
	// 检查状态
	if( res.status!=0 )
	{
		if( ( res.status&0x02 )==1 )
			strMsg = "Checksum error";
		else if( ( res.status&0x01 )==1 )
			strMsg = "Syntax error";
		else if( ( res.status&0x08 )==1 )
			strMsg = "Switch error";
		// 
		rt = false;
		return;
	}
	
	switch( op )
	{
	case 0x01: // Read Module Serial Number
		{
			strMsg = "Read serial number successfully..";

			int serialNumber;
			serialNumber = res._dataBuf[0]<<24;
			serialNumber += res._dataBuf[1]<<16;
			serialNumber += res._dataBuf[2]<<8;
			serialNumber += res._dataBuf[3];
			// 
			_resultList.push_back( serialNumber );
		}
		break;
	case 0x02: // Read Module Type
		{
			strMsg = "Read moudle type successfully..";
		
			_resultList.push_back( (int)res._dataBuf[0] );
		}
		break;
	case 0x03: // Read Module Version
		{
			strMsg = "Read module version successfully..";

			_resultList.push_back( res._dataBuf[0] );			// hardVH
			_resultList.push_back( res._dataBuf[1] );			// hardVL
			_resultList.push_back( res._dataBuf[2] );			// firmVH
			_resultList.push_back( res._dataBuf[3] );			// firmVL
		}
		break;
	case 0x04: // Set RS232 Baud Rate
		{
			strMsg = "Set baud rate successfully..";
		}
		break;
	case 0x05: // Read Hardware Date Code
		{
			strMsg = "Read hardware date successfully..";

			UCHAR hardDateCodeH, hardDateCodeL;
			hardDateCodeH = res._dataBuf[0];
			hardDateCodeL = res._dataBuf[1]; 
			// 
			_resultList.push_back( hardDateCodeH );
			_resultList.push_back( hardDateCodeL );
		}
		break;
	case 0x06: // Read Firmware Date Code
		{
			strMsg = "Read Firmware Date successfully..";

			UCHAR FirmwareDateCodeH, FirmwareDateCodeL;
			FirmwareDateCodeH = res._dataBuf[0];
			FirmwareDateCodeL = res._dataBuf[1]; 
			// 
			_resultList.push_back( FirmwareDateCodeH );
			_resultList.push_back( FirmwareDateCodeL );
		}
		break;
	case 0x11: // Read Attenuation/Port of a Specified Channel
		strMsg = "Set channel attenuation successfully..";
		break;
	case 0x12: // Read Status of a Specified Channel
		{
			strMsg = "Read channel attenuation successfully..";
			UCHAR p, d;
			p = res._dataBuf[0];
			d = res._dataBuf[1]; 
			// 
			_resultList.push_back( p );
			_resultList.push_back( d );
		}
		break;
	case 0x13: // Set Attenuation/Port of All Channels
		strMsg = "Set all channels attenuation successfully..";
		break;
	case 0x14: // Read Status of All Channels
		{
			strMsg = "Read all channels attenuation successfully..";
			// 
			UCHAR *p = res._dataBuf;
			UCHAR *d = res._dataBuf+CHANNEL_NUM;
			// 
			for( int i=0; i<CHANNEL_NUM; ++i )
			{
				_resultList.push_back( p[i] );
				_resultList.push_back( (float)d[i]/10 );

				if( p[i]>PORT_NUM || p[i]==0 )
				{
					strMsg = "Receive wrong serial data..";
					rt = false;
				}
			}
		}
		break;
	case 0x15:
		strMsg = "Generate Channel Profile successfully..";
		break;
	case 0x16:
		{
			strMsg = "Read Channel Profile successfully..";
			UCHAR *p = res._dataBuf;
			UCHAR *d = res._dataBuf+CHANNEL_NUM;
			// 
			for( int i=0; i<CHANNEL_NUM; ++i )
			{
				_resultList.push_back( p[i] );
				_resultList.push_back( (float)d[i]/10 );
			}
		}
		break;
	case 0x17:
		strMsg = "Select Channel Profile successfully..";
		break;
	case 0x18:
		{
			strMsg = "Read Channel Profile ID successfully..";
			_resultList.push_back( res._dataBuf[0] );
		}
		break;
	case 0x21: // Read Alarm
		{
			strMsg = "Read alarms successfully..";
			// 
			_resultList.push_back( res._dataBuf[0] );
		}
		break;
	case 0x22: // Read Power Supply Voltage
		{
			strMsg = "Read voltage successfully..";
			// 
			USHORT vol;
			vol = res._dataBuf[0]<<8;
			vol += res._dataBuf[1];
			_resultList.push_back( vol );
		}
		break;
	case 0x23: // Read Temperature
		{
			strMsg = "Read temperature successfully..";
			// 
			USHORT temper = ( res._dataBuf[0]<<8 ) + res._dataBuf[1];   
			float temp = (short)temper;
			temp /= 10;
			_resultList.push_back( temp );
		}
		break;
	case 0x31: // Read Low Temperature Alarm Threshold
		strMsg = "Read temperature lower limit successfully..";
		break;
	case 0x32: // Read Low Temperature Alarm Threshold
		{
			strMsg = "Read temperature lower limit successfully..";
			// 
			USHORT temper = ( res._dataBuf[0]<<8 ) + res._dataBuf[1];   
			float temp = (short)temper;
			temp /= 10;
			_resultList.push_back( temp );
		}
		break;
	case 0x33: // Read High Temperature Alarm Threshold
		strMsg = "Read temperature upper limit successfully..";
		break;
	case 0x34: // Read High Temperature Alarm Threshold
		{
			strMsg = "Read temperature upper limit successfully..";
			// 
			USHORT temper = ( res._dataBuf[0]<<8 ) + res._dataBuf[1];   
			float temp = (short)temper;
			temp /= 10;
			_resultList.push_back( temp );
		}
		break;
	case 0x41: // Start Firmware Loading
		strMsg = "Start firmware update..";
		break;
	case 0x42: // Load New Firmware
		strMsg = "Ready for load file..";
		break;
	case 0x43: // Switch to New Firmware
		strMsg = "Switch to new firmware..";
		break;
	case (UCHAR)0xAA: // Manufacture ID   
		{
			strMsg = "Read manufacture ID successfully..";
			_resultList.push_back( res._dataBuf[1] );
		}
		break;
	case (UCHAR)0xF0: // Set DAC Voltage
		strMsg = "Set voltage successfully..";
		break;
	case (UCHAR)0xF4:
		{
			USHORT v = ( res._dataBuf[0]<<8 ) + res._dataBuf[1];
			_resultList.push_back( v );
		}
		break;
	case (UCHAR)0xF3:
		strMsg = "Set 0xF3 successfully..";
		break;
	default:
		rt = false;
	};
}

void WSSBasePro::HandleShowResult( UCHAR op, bool rt, string strMsg, vector<Var>& _resultList )
{
	if( PCB_View_ShowMsg!=NULL ) (*PCB_View_ShowMsg)( strMsg.c_str() );
	if( rt==false ) return;

	switch( op )
	{
	case 0x01: // Read Module Serial Number
		if( PCB_View_ShowSerialNumber!=NULL ) (*PCB_View_ShowSerialNumber)( _resultList[0] );
		break;
	case 0x02: // Read Module Type
		if( PCB_View_ShowType!=NULL ) (*PCB_View_ShowType)( _resultList[0] );
		break;
	case 0x03: // Read Module Version
		if( PCB_View_ShowVersion!=NULL ) (*PCB_View_ShowVersion)( _resultList[0], _resultList[1], _resultList[2], _resultList[3] );
		break;
	case 0x04: // Set RS232 Baud Rate
		break;
	case 0x05: // Read Hardware Date Code
		if( PCB_View_ShowHardDateCode!=NULL ) (*PCB_View_ShowHardDateCode)( _resultList[0], _resultList[1] );
		break;
	case 0x06: // Read Firmware Date Code
		if( PCB_View_ShowFirmwareDateCode!=NULL ) (*PCB_View_ShowFirmwareDateCode)( _resultList[0], _resultList[1] );
		break;
	case 0x11: // Set Attenuation/Port of a Specified Channel
		break;
	case 0x12: // Read Status of a Specified Channel
		if( PCB_View_ShowChannelAtten!=NULL ) (*PCB_View_ShowChannelAtten) ( _resultList[0], _resultList[1] );
		break;
	case 0x13: // Set Attenuation/Port of All Channels
		break;
	case 0x14: // Read Status of All Channels
	case 0x16:
		{
			UCHAR p[MAX_CHANNEL_NUM];
			float d[MAX_CHANNEL_NUM];
			// 
			for( int i=0; i<CHANNEL_NUM; ++i )
			{
				p[i] = _resultList[2*i];
				d[i] = _resultList[2*i+1];
			}
			if( PCB_View_ShowAllChannelAtten!=NULL ) (*PCB_View_ShowAllChannelAtten)( p, d );
		}
		break;
	case 0x18:
		if( PCB_View_ShowProfileID!=NULL ) (*PCB_View_ShowProfileID)( (UCHAR)_resultList[0] );
		break;
	case 0x21: // Read Alarm
		if( PCB_View_ShowAlarm!=NULL ) (*PCB_View_ShowAlarm)( _resultList[0] );
		break;
	case 0x22: // Read Power Supply Voltage
		if( PCB_View_ShowVoltage!=NULL ) (*PCB_View_ShowVoltage)( _resultList[0] );
		break;
	case 0x23: // Read Temperature
		if( PCB_View_ShowTemperature!=NULL ) (*PCB_View_ShowTemperature)( _resultList[0] );
		break;
	case 0x31: // Set Low Temperature Alarm Threshold
		break;
	case 0x32: // Read Low Temperature Alarm Threshold
		if( PCB_View_ShowLowTemperature!=NULL ) (*PCB_View_ShowLowTemperature)( _resultList[0] );
		break;
	case 0x33: // Set High Temperature Alarm Threshold
		break;
	case 0x34: // Read High Temperature Alarm Threshold
		if( PCB_View_ShowHighTemperature!=NULL ) (*PCB_View_ShowHighTemperature)( _resultList[0] );
		break;
	case 0x41: // Start Firmware Loading
		break;
	case 0x43: // Switch to New Firmware
		break;
	case 0x42: // Load New Firmware
		break;
	case 0x55: // Save Current Channel Configuration to Startup Profile
		if( PCB_View_ShowMemWear!=NULL ) (*PCB_View_ShowMemWear)( _resultList[0] );
		break;
	case (UCHAR)0xAA: // Manufacture ID   
		if( PCB_View_ShowManufactureID!=NULL ) (*PCB_View_ShowManufactureID)( _resultList[0] );
		break;
	case (UCHAR)0xF0: // Set DAC Voltage
		break;

	};
}





WSSProSeria::WSSProSeria()
{
	_code = 0;
}

void WSSProSeria::GetSerialNumber()
{
	WSSCommand cmd = _wssBasePro.GetSerialNumber();

	// send
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetType()
{
	WSSCommand cmd = _wssBasePro.GetType();
		
	// send
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetVersion()
{
	WSSCommand cmd = _wssBasePro.GetVersion();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetRS232BaudRate( int baudRate )
{
	WSSCommand cmd = _wssBasePro.SetRS232BaudRate( baudRate );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetHardwareDateCode()
{
	WSSCommand cmd = _wssBasePro.GetHardwareDateCode();

	// send 
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetFirmwareDateCode()
{
	WSSCommand cmd = _wssBasePro.GetFirmwareDateCode();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetChannelPortAtten( int c, UCHAR p, float d )
{
	WSSCommand cmd = _wssBasePro.SetChannelPortAtten( c, p, d );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetChannelPortAtten( int c )
{
	WSSCommand cmd = _wssBasePro.GetChannelPortAtten( c );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetAllChannelsPortAtten( UCHAR *p, float *d, int n )
{
	WSSCommand cmd = _wssBasePro.SetAllChannelsPortAtten( p, d, n );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetAllChannelsPortAtten()
{
	WSSCommand cmd = _wssBasePro.GetAllChannelsPortAtten();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen, 100 );
}

void WSSProSeria::GetAlarm()
{
	WSSCommand cmd = _wssBasePro.GetAlarm();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetVoltage()
{
	WSSCommand cmd = _wssBasePro.GetVoltage();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetTemperature()
{
	WSSCommand cmd = _wssBasePro.GetTemperature();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

bool WSSProSeria::GetTemperatureSyn( float& temp )
{
	if( _synData._useFlag==true )
		return false;

	_synData._useFlag = true;
	_synData._stopWaitFlag = false;
	
	GetTemperature();

	BEGIN_WAIT()
	END_WAIT_FLAG( 50, _synData._stopWaitFlag );
	

	temp = _synData._resultList[0];
	bool rt = _synData._stopWaitFlag;	
	_synData._useFlag = false;
	return rt;
}

void WSSProSeria::SetLowTemperatureAlarmThreshold( float temperature )
{
	WSSCommand cmd = _wssBasePro.SetLowTemperatureAlarmThreshold( temperature );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetLowTemperatureAlarmThreshold()
{
	WSSCommand cmd = _wssBasePro.GetLowTemperatureAlarmThreshold();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetHighTemperatureAlarmThreshold( float temperature )
{
	WSSCommand cmd = _wssBasePro.SetHighTemperatureAlarmThreshold( temperature );

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetHighTemperatureAlarmThreshold()
{
	WSSCommand cmd = _wssBasePro.GetHighTemperatureAlarmThreshold();

	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::StartFirmwareLoading()
{
	WSSCommand cmd = _wssBasePro.StartFirmwareLoading();
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen, 100 );
}

void WSSProSeria::LoadNewFirmware()
{
	WSSCommand cmd = _wssBasePro.LoadNewFirmware();
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen, 100 );
}

bool WSSProSeria::LoadNewFirmwareSyn()
{
	// 设置同步数据
	if( _synData._useFlag==true )
		return false;
	_synData._useFlag = true;
	_synData._stopWaitFlag = false;
	
	LoadNewFirmware();

	BEGIN_WAIT()
	END_WAIT_FLAG( 10000, _synData._stopWaitFlag );
	
	// 释放同步数据并返回
	_synData._useFlag = false;
	bool rt = _synData._stopWaitFlag;	
	return rt;
}

void WSSProSeria::SwitchtoNewFirmware()
{
	WSSCommand cmd = _wssBasePro.SwitchtoNewFirmware();
	
	// send 
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::GetManufactureID()
{
	WSSCommand cmd = _wssBasePro.GetManufactureID();
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

bool WSSProSeria::GetAD5535Syn( int c, USHORT& v )
{
	if( _synData._useFlag==true )
		return false;

	_synData._useFlag = true;
	_synData._stopWaitFlag = false;
	
	// 发送数据
	WSSCommand cmd = _wssBasePro.GetAD5535( c );
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );

	BEGIN_WAIT()
	END_WAIT_FLAG( 50, _synData._stopWaitFlag );
	
	bool rt = _synData._stopWaitFlag;	
	if( rt==true )
		v = _synData._resultList[0];

	_synData._useFlag = false;
	return rt;
}

void WSSProSeria::SetDACVoltage( UINT channel, int coord, USHORT vol )
{
	WSSCommand cmd = _wssBasePro.SetDACVoltage( channel, coord, vol );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

bool WSSProSeria::SetDACVoltageSyn( UINT channel, int coord, USHORT vol )
{
	if( _synData._useFlag==true )
		return false;

	_synData._useFlag = true;
	_synData._stopWaitFlag = false;
	
	SetDACVoltage( channel, coord, vol );

	BEGIN_WAIT()
	END_WAIT_FLAG( 50, _synData._stopWaitFlag );
	
	bool rt = _synData._stopWaitFlag;	
	_synData._useFlag = false;
	return rt;
}

void WSSProSeria::GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] )
{
	WSSCommand cmd = _wssBasePro.GenerateChannelProfile( pID, portList, attenList );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::ReadChannelProfile( UCHAR pID )
{
	WSSCommand cmd = _wssBasePro.ReadChannelProfile( pID );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen, 100 );
}

void WSSProSeria::SelectChannelProfile( UCHAR pID )
{
	WSSCommand cmd = _wssBasePro.SelectChannelProfile( pID );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::ReadProfileID()
{
	WSSCommand cmd = _wssBasePro.ReadProfileID();
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo1( sn, type, hv1, hv2, hd1, hd2  );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo2( flag1, flag2, tempNum, lowTemp, highTemp );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetModuleInfo3( USHORT volList[104] )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo3( volList );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

void WSSProSeria::SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo4( port, f, volList );
	
	// send  
	_code = cmd._code;
	ComSend( (char*)&cmd, 3+cmd._dataLen );
}

bool WSSProSeria::ParaseRes( string& strRes )
{
	UCHAR op = _code;
	string strMsg;
	bool rt;
	vector<Var> _resultList;

	// 解析
	_wssBasePro.HandleParseRes( strRes, op, rt, strMsg, _resultList );

	// 同步处理
	if( rt==true && _synData._useFlag==true )
	{
		_synData._resultList = _resultList;
		_synData._stopWaitFlag = true;
		return true;
	}

	// 显示到view
	_wssBasePro.HandleShowResult( op, rt, strMsg, _resultList );
	return rt;
}



WSSProUDP::WSSProUDP()
{
	//ip = 0xACA8587B;
}

bool WSSProUDP::Func1( UCHAR op, vector<Var>& _resultList )							// 接收 解析通用处理
{
	UCHAR recvBuf[512];
	memset( recvBuf, 0, sizeof( recvBuf ) );

	int recvLen = _udpOBJ.RecvFrom( (char*)recvBuf, 512, 1000 );
	if( recvLen<=0 )
	{
		if( PCB_View_ShowMsg!=NULL ) (*PCB_View_ShowMsg)( _udpOBJ.GetLastErrMsg().c_str() );
		return false;
	}
	
	// 解析
	string strMsg;
	bool rt;
	_wssBasePro.HandleParseRes( string( recvBuf, recvBuf+recvLen ), op, rt, strMsg, _resultList );

	// 显示到view
	_wssBasePro.HandleShowResult( op, rt, strMsg, _resultList );
	return rt;
}	

bool WSSProUDP::GetSerialNumber( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetSerialNumber();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::GetSerialNumberSyn( ULONG ip, int& sn )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetSerialNumber();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	if( Func1( cmd._code, _resultList )==false )
		return false;

	sn = _resultList[0];
	return true;
}

// 获得类型
bool WSSProUDP::GetType( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetType();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获得版本号
bool WSSProUDP::GetVersion( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetVersion();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 设置串口速率
bool WSSProUDP::SetRS232BaudRate( ULONG ip, int baudRate )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetRS232BaudRate( baudRate );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获得硬件版本号
bool WSSProUDP::GetHardwareDateCode( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetHardwareDateCode();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获得固件版本号
bool WSSProUDP::GetFirmwareDateCode( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetFirmwareDateCode();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获得制造ID
bool WSSProUDP::GetManufactureID( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetManufactureID();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 设置通道port衰减
bool WSSProUDP::SetChannelPortAtten( ULONG ip, int c, UCHAR p, float d )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetChannelPortAtten( c, p, d );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取通道衰减
bool WSSProUDP::GetChannelPortAtten( ULONG ip, int c )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetChannelPortAtten( c );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 设置所有通道port衰减
bool WSSProUDP::SetAllChannelsPortAtten( ULONG ip, UCHAR *p, float *d, int n )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetAllChannelsPortAtten( p, d, n );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取所有通道衰减
bool WSSProUDP::GetAllChannelsPortAtten( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetAllChannelsPortAtten();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// Module Operation Status
// 获取告警
bool WSSProUDP::GetAlarm( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetAlarm();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取电压
bool WSSProUDP::GetVoltage( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetVoltage();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取温度
bool WSSProUDP::GetTemperature( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetTemperature();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::GetTemperatureSyn( ULONG ip, float& temp )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetTemperature();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	if( Func1( cmd._code, _resultList )==false )
		return false;

	temp = _resultList[0];
	return true;
}

// 告警信息
// 设置低温度告警门限
bool WSSProUDP::SetLowTemperatureAlarmThreshold( ULONG ip, float temperature )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetLowTemperatureAlarmThreshold( temperature );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取低温度告警门限
bool WSSProUDP::GetLowTemperatureAlarmThreshold( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetLowTemperatureAlarmThreshold();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 设置高温度告警门限
bool WSSProUDP::SetHighTemperatureAlarmThreshold( ULONG ip, float temperature )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetHighTemperatureAlarmThreshold( temperature );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// 获取高温度告警门限
bool WSSProUDP::GetHighTemperatureAlarmThreshold( ULONG ip )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.GetHighTemperatureAlarmThreshold();
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

// Set DAC Voltage
// 设置通道 轴(x:0 y:1) 电压
bool WSSProUDP::SetDACVoltage( ULONG ip, UINT channel, int coord, USHORT vol )
{
	cnCriticalSectionAuto csAuto( _cs );

	WSSCommand cmd = _wssBasePro.SetDACVoltage( channel, coord, vol );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::SetModuleInfo1( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo1( sn, type, hv1, hv2, hd1, hd2  );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::SetModuleInfo2( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo2( flag1, flag2, tempNum, lowTemp, highTemp );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::SetModuleInfo3( ULONG ip, USHORT volList[104] )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo3( volList );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}

bool WSSProUDP::SetModuleInfo4( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] )
{
	WSSCommand cmd = _wssBasePro.SetModuleInfo4( port, f, volList );
	_udpOBJ.SendTo( (char*)&cmd, 3+cmd._dataLen, ip );

	vector<Var> _resultList;
	return Func1( cmd._code, _resultList );
}




bool WSSProEX::StartLoadNewFirmwareThread()
{
	return StartThread( "LoadNewFirmware", &WSSProEX::LoadNewFirmwareThread );
}

void WSSProEX::LoadNewFirmwareThread()
{
	if( WSSProBase::GetSingleton()->_wssProSeria.IsOpen()==false )
	{
		::MessageBoxA( NULL, "Serial port is close!", "Warning", 0 );
		return;
	}

	if( PCB_View_ShowMsg!=NULL ) (*PCB_View_ShowMsg)( "Ready for loading file.." );

	if( WSSProBase::GetSingleton()->_wssProSeria.LoadNewFirmwareSyn()==false )
	{
		::MessageBoxA( NULL, "Wait Timeout!", "Warning", 0 );
		return;
	}

	string fileName;
	if( OpenFolderFile( fileName )==false ) return;

	TxtOBJ txt;
	if( txt.OpenFile( fileName )==false ) return;

	bool flag=true;
	char sendBuf[1024];
	ULONG bufSize = 128;
	int maxCount = (int)( txt.GetFileSize()/bufSize );
	int count = 0;
	while( flag )
	{
		ULONG readSize = txt.ReadBuf( sendBuf, bufSize );
		if( readSize<bufSize )
			flag = false;
		WSSProBase::GetSingleton()->_wssProSeria.ComSend( sendBuf, readSize, 0, 10 );

		float percent = (float)( ++count )/maxCount;
		percent *= 100;
		string str;
		STRING_FORMAT( str, "Loading file percentage: %0.3f", percent );
		str += "%";
		if( PCB_View_ShowMsg!=NULL ) (*PCB_View_ShowMsg)( str.c_str() );
	}

	if( PCB_View_ShowMsg!=NULL ) (*PCB_View_ShowMsg)( "Loading bin file successfully.." );
}                                           
	
	
	
	
	
WSSProBase* WSSProBase::GetSingleton()
{
	#define CheckNULL( obj )  if( obj==NULL ) return NULL;

	if( _pSingleton==NULL )  
	{
		LoadWSSConfigDLL();

		_pSingleton = new WSSProBase;			// new第一个对象
	}

	return _pSingleton; 
}

void WSSProBase::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}      
	
WSSProBase::WSSProBase()
{
}
	
	
	
	

	
	
__declspec(dllexport) bool OpenSerial( DWORD dwPort, DWORD dwBaudRate )
{
	return WSSProBase::GetSingleton()->_wssProSeria.Open( dwPort, dwBaudRate );
}

__declspec(dllexport) bool SetSerialRate( DWORD baudRate )
{
	return WSSProBase::GetSingleton()->_wssProSeria.SetComm( baudRate );
}

__declspec(dllexport) bool IsSerialOpen()
{
	return WSSProBase::GetSingleton()->_wssProSeria.IsOpen();
}

__declspec(dllexport) void SetDACVoltage_Com( UINT channel, int coord, USHORT vol )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetDACVoltage( channel, coord, vol );
}

__declspec(dllexport) bool SetDACVoltageSyn_Com( UINT channel, int coord, USHORT vol )
{
	Sleep( 20 );
	return WSSProBase::GetSingleton()->_wssProSeria.SetDACVoltageSyn( channel, coord, vol );
}

// 获得序号
__declspec(dllexport) void GetSerialNumber_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetSerialNumber();
}

// 获得类型
__declspec(dllexport) void GetType_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetType();
}

// 获得版本号
__declspec(dllexport) void GetVersion_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetVersion();
}

// 获得串口速率
__declspec(dllexport) void SetRS232BaudRate_Com( int baudRate )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetRS232BaudRate( baudRate );
}

// 获得硬件版本号
__declspec(dllexport) void GetHardwareDateCode_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetHardwareDateCode();
}

// 获得固件版本号
__declspec(dllexport) void GetFirmwareDateCode_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetFirmwareDateCode();
}

// 获得制造ID
__declspec(dllexport) void GetManufactureID_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetManufactureID();
}

__declspec(dllexport) bool GetAD5535Syn_Com( int c, USHORT& v )
{
	Sleep( 20 );
	return WSSProBase::GetSingleton()->_wssProSeria.GetAD5535Syn( c, v );
}

// 设置通道port衰减
__declspec(dllexport) void SetChannelPortAtten_Com( int c, UCHAR p, float d )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetChannelPortAtten( c, p, d );
}

// 获取通道衰减
__declspec(dllexport) void GetChannelPortAtten_Com( int c )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetChannelPortAtten( c );
}

// 设置所有通道port衰减
__declspec(dllexport) void SetAllChannelsPortAtten_Com( UCHAR *p, float *d, int n )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetAllChannelsPortAtten( p, d, n );
}

// 获取所有通道衰减
__declspec(dllexport) void GetAllChannelsPortAtten_Com()
{
	Sleep( 100 );
	WSSProBase::GetSingleton()->_wssProSeria.GetAllChannelsPortAtten();
}

// 获取告警
__declspec(dllexport) void GetAlarm_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetAlarm();
}

// 获取电压
__declspec(dllexport) void GetVoltage_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetVoltage();
}

// 获取温度
__declspec(dllexport) void GetTemperature_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetTemperature();
}

extern "C" __declspec(dllexport) bool GetTemperatureSyn_Com( float& temp )
{
	Sleep( 20 );
	return WSSProBase::GetSingleton()->_wssProSeria.GetTemperatureSyn( temp );
}

// 设置低温度告警门限
__declspec(dllexport) void SetLowTemperatureAlarmThreshold_Com( float temperature )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetLowTemperatureAlarmThreshold( temperature );
}

// 获取低温度告警门限
__declspec(dllexport) void GetLowTemperatureAlarmThreshold_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetLowTemperatureAlarmThreshold();
}

// 设置高温度告警门限
__declspec(dllexport) void SetHighTemperatureAlarmThreshold_Com( float temperature )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetHighTemperatureAlarmThreshold( temperature );
}

// 获取高温度告警门限
__declspec(dllexport) void GetHighTemperatureAlarmThreshold_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GetHighTemperatureAlarmThreshold();
}

// 开始固件加载
__declspec(dllexport) void StartFirmwareLoading_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.StartFirmwareLoading();
}

// 加载新固件
__declspec(dllexport) void LoadNewFirmware_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.LoadNewFirmware();
}

// 切换新固件
__declspec(dllexport) void SwitchtoNewFirmware_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SwitchtoNewFirmware();
}

__declspec(dllexport) void GenerateChannelProfile_Com( UCHAR pID, UCHAR portList[], UCHAR attenList[] )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.GenerateChannelProfile( pID, portList, attenList );
}

__declspec(dllexport) void ReadChannelProfile_Com( UCHAR pID )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.ReadChannelProfile( pID );
}

__declspec(dllexport) void SelectChannelProfile_Com( UCHAR pID )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SelectChannelProfile( pID );
}

__declspec(dllexport) void ReadProfileID_Com()
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.ReadProfileID();
}

__declspec(dllexport) void SetModuleInfo1_Com( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetModuleInfo1( sn, type, hv1, hv2, hd1, hd2 );
}

__declspec(dllexport) void SetModuleInfo2_Com( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp )
{
	Sleep( 20 );
	WSSProBase::GetSingleton()->_wssProSeria.SetModuleInfo2( flag1, flag2, tempNum, lowTemp, highTemp );
}

__declspec(dllexport) void SetModuleInfo3_Com( USHORT volList[104] )
{
	Sleep( 50 );
	WSSProBase::GetSingleton()->_wssProSeria.SetModuleInfo3( volList );
}

__declspec(dllexport) void SetModuleInfo4_Com( UCHAR port, UCHAR f, USHORT volList[104] )
{
	Sleep( 50 );
	WSSProBase::GetSingleton()->_wssProSeria.SetModuleInfo4( port, f, volList );
}

__declspec(dllexport) void GetSerialNumber_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetSerialNumber( ip );
}

__declspec(dllexport) bool GetSerialNumberSyn_Net( ULONG ip, int* sn )
{
	return WSSProBase::GetSingleton()->_wssProUDP.GetSerialNumberSyn( ip, *sn );
}

__declspec(dllexport) void GetType_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetType( ip );
}

__declspec(dllexport) void GetVersion_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetVersion( ip );
}

__declspec(dllexport) void SetRS232BaudRate_Net(  ULONG ip, int baudRate )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetRS232BaudRate( ip, baudRate );
}

__declspec(dllexport) void GetHardwareDateCode_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetHardwareDateCode( ip );
}

__declspec(dllexport) void GetFirmwareDateCode_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetFirmwareDateCode( ip );
}

__declspec(dllexport) void GetManufactureID_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetManufactureID( ip );
}

__declspec(dllexport) void SetChannelPortAtten_Net( ULONG ip, int c, UCHAR p, float d )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetChannelPortAtten( ip, c, p, d );
}

__declspec(dllexport) void GetChannelPortAtten_Net( ULONG ip, int c )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetChannelPortAtten( ip, c );
}

__declspec(dllexport) void SetAllChannelsPortAtten_Net( ULONG ip, UCHAR *p, float *d, int n )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetAllChannelsPortAtten( ip, p, d, n );
}

__declspec(dllexport) void GetAllChannelsPortAtten_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetAllChannelsPortAtten( ip );
}

__declspec(dllexport) void GetAlarm_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetAlarm( ip );
}

__declspec(dllexport) void GetVoltage_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetVoltage( ip );
}

__declspec(dllexport) void GetTemperature_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetTemperature( ip );
}

__declspec(dllexport) bool GetTemperatureSyn_Net( ULONG ip, float* temp )
{
	return WSSProBase::GetSingleton()->_wssProUDP.GetTemperatureSyn( ip, *temp );
}

__declspec(dllexport) void SetLowTemperatureAlarmThreshold_Net( ULONG ip, float temperature )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetLowTemperatureAlarmThreshold( ip, temperature );
}

__declspec(dllexport) void GetLowTemperatureAlarmThreshold_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetLowTemperatureAlarmThreshold( ip );
}

__declspec(dllexport) void SetHighTemperatureAlarmThreshold_Net( ULONG ip, float temperature )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetHighTemperatureAlarmThreshold( ip, temperature );
}

__declspec(dllexport) void GetHighTemperatureAlarmThreshold_Net( ULONG ip )
{
	WSSProBase::GetSingleton()->_wssProUDP.GetHighTemperatureAlarmThreshold( ip );
}

__declspec(dllexport) bool SetDACVoltageSyn_Net( ULONG ip, UINT channel, int coord, USHORT vol )
{
	return WSSProBase::GetSingleton()->_wssProUDP.SetDACVoltage( ip, channel, coord, vol );
}

__declspec(dllexport) void SetModuleInfo1_Net( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetModuleInfo1( ip, sn, type, hv1, hv2, hd1, hd2 );
}

__declspec(dllexport) void SetModuleInfo2_Net( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetModuleInfo2( ip, flag1, flag2, tempNum, lowTemp, highTemp );
}

__declspec(dllexport) void SetModuleInfo3_Net( ULONG ip, USHORT volList[104] )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetModuleInfo3( ip, volList );
}

__declspec(dllexport) void SetModuleInfo4_Net( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] )
{
	WSSProBase::GetSingleton()->_wssProUDP.SetModuleInfo4( ip, port, f, volList );
}

__declspec(dllexport) bool StartLoadNewFirmwareProcess()
{
	return WSSProBase::GetSingleton()->_wssProEX.StartLoadNewFirmwareThread();
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                