
#include "stdafx.h"
#include "WSSProCal.h"


bool WSSProCal::SetLSB( ULONG ip, UCHAR lsb )
{
	UCHAR recvBuf[512], sendBuf[512]; 	// 命令发送接收缓存
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	//sendBuf[3] = 0;
	//sendBuf[4] = 0;
	//sendBuf[5] = 0;
	//sendBuf[6] = 0;
	//sendBuf[7] = 0;
	//sendBuf[8] = 0;
	//sendBuf[9] = 0;
	sendBuf[10] = 0;
	sendBuf[11] = lsb;
	_udpOBJ.SendTo( (char*)sendBuf, 12, ip );

	// 接收命令
	if( _udpOBJ.RecvFrom( (char*)recvBuf, 512, 200 )!=12 )
		return false;

	return true;
}


bool WSSProCal::NotifyWssType( ULONG ip, UCHAR in, UCHAR out )
{
	UCHAR recvBuf[512], sendBuf[512]; 	// 命令发送接收缓存
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x0A;
	sendBuf[4] = in;
	sendBuf[5] = out;
	_udpOBJ.SendTo( (char*)sendBuf, 6, ip );

	// 接收命令
	if( _udpOBJ.RecvFrom( (char*)recvBuf, 512, 200 )!=6 )
		return false;
	
	// 检验
	if( recvBuf[3]!=0x0A || recvBuf[4]!=in || recvBuf[5]!=out )
		return false;

	return true;
}

bool WSSProCal::StartCollectDataSyn( ULONG ip, UCHAR c, UCHAR p )
{
	UCHAR recvBuf[512], sendBuf[512]; 	// 命令发送接收缓存
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x01;
	sendBuf[4] = c;
	sendBuf[5] = p;
	_udpOBJ.SendTo( (char*)sendBuf, 6, ip );

	// 接收命令
	if( _udpOBJ.RecvFrom( (char*)recvBuf, 512, 200 )!=6 )
		return false;
	
	// 检验
	if( recvBuf[3]!=0x01 || recvBuf[4]!=c || recvBuf[5]!=p )
		return false;

	return true;
}

bool WSSProCal::GetCollectDataSyn( ULONG ip, UCHAR c, UCHAR p, int n, int max, int& rt, string& rtMsg, VECTOR_INT& volList, VECTOR_FLOAT& pmList )
{
	UCHAR recvBuf[512], sendBuf[512]; 	// 命令发送接收缓存
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x02;
	sendBuf[4] = c;
	sendBuf[5] = p;
	sendBuf[6] = n;
	_udpOBJ.SendTo( (char*)sendBuf, 7, ip );
	
	int recvLen = _udpOBJ.RecvFrom( (char*)recvBuf, 512, 3000 );
	if( recvLen==-1 )
	{
		rt = -1;
		rtMsg = "未收到帧 超时错误..";
		return false;
	}

	if( recvLen==7 )
	{
		if( recvBuf[6]==0xFF )
		{	
			rtMsg = "未定标 严重错误 重新采集..";
			rt = 2;
		}
		else if( recvBuf[6]==0xFE )
		{	
			rtMsg = "帧序号越界 严重错误 重新采集..";
			rt = 2;
		}
		else if( recvBuf[6]==0xF0 )
		{	
			rtMsg = "模块串口通讯错误 严重错误 重新采集..";
			rt = 2;
		}
		else if( recvBuf[6]==0xF1 )
		{	
			rtMsg = "该通道错误 严重错误 重新采集.."; 
			rt = 2;
		}
		else
		{	// 定标完成度
			STRING_FORMAT( rtMsg, "定标通道%d port%d 完成度%d..", c, p, recvBuf[6] );
			rt = 0;
		}

		return true;
	}
	else if( recvLen==507 )
	{	// 解析收到的第i帧
		if( recvBuf[3]!=0x02 || recvBuf[4]!=c || recvBuf[5]!=p || recvBuf[6]!=n )
		{
			rt = -1;
			rtMsg = "帧头解析错误.."; 
			return false;
		}

		STRING_FORMAT( rtMsg, "收到 %d/%d 帧数据..", n+1, max );
		for( int j=0; j<500; j+=2 )
		{
			int index = ( n*250+j/2 )*LSB;
			short power = (short)( (USHORT)( recvBuf[j+7]<<8 ) + (USHORT)( recvBuf[j+8] ) );
			float powerV = static_cast<float>( power )/100.f;
			// 
			volList.push_back( index );
			pmList.push_back( powerV );
		}
		rt = 1;
		return true;
	}
	else
	{
		rt = -1;
		rtMsg = "帧长度错误.."; 
		return false;
	}
}

bool WSSProCal::SendCalChannelPortDataSyn( ULONG ip, UCHAR temp, int c, int port, VECTOR_INT& curveCalcX, int yVol, int blockX2 )
{
	// 命令发送接收缓存
	UCHAR recvBuf[512], sendBuf[512];		
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	// 组织
	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x03;
	sendBuf[4] = c;
	sendBuf[5] = temp;
	sendBuf[6] = port;
	for( int j=0; j<250; ++j )
		memcpy( sendBuf+7+j*2, &curveCalcX[j], 2 );

	// y电压
	memcpy( sendBuf+507, &yVol, 2 );

	memcpy( sendBuf+509, &blockX2, 2 );
	
	// 发送接收
	_udpOBJ.SendTo( (char*)sendBuf, 511, ip );
	int recvLen = _udpOBJ.RecvFrom( (char*)recvBuf, 512, 1000 );
	if( recvLen!=7 )
		return false;
	if( recvBuf[4]!=c || recvBuf[5]!=temp || recvBuf[6]!=port )
		return false;

	return true;
}

bool WSSProCal::GetChannelPortPMSyn( ULONG ip, int p, float& portPM )
{
	UCHAR recvBuf[512], sendBuf[512];
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	// 发送命令
	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x06;
	sendBuf[4] = p;
	_udpOBJ.SendTo( (char*)sendBuf, 5, ip );

	// 接收命令
	int rt = _udpOBJ.RecvFrom( (char*)recvBuf, 512, 5000 );
	if( rt!=7 )
	{
		if( rt==-1 )
		{
			_debugMsg = _udpOBJ.GetLastErrMsg();
		}
		else
		{
			string str;
			for( int i=0; i<7; ++i )
			{
				STRING_FORMAT( str, "%02x ", sendBuf[i] );
				_debugMsg += str;
			}
			STRING_FORMAT( str, "rt(%d)!=7", rt );
			_debugMsg += str;
		}
		return false;
	}

	USHORT temp = ( recvBuf[5]<<8 ) + recvBuf[6];
	short pm;
	memcpy( &pm, &temp, 2 );
	portPM = pm;
	portPM /= 100;

	return true;
}

bool WSSProCal::NotifyLaserDying( ULONG ip )
{
	UCHAR recvBuf[512], sendBuf[512];
	memset( sendBuf, 0, sizeof( sendBuf ) );
	memset( recvBuf, 0, sizeof( recvBuf ) );

	// 发送命令
	sendBuf[0] = 0x11;
	sendBuf[1] = 0x22;
	sendBuf[2] = 0x33;
	sendBuf[3] = 0x09;
	_udpOBJ.SendTo( (char*)sendBuf, 4, ip );

	// 接收命令
	if( _udpOBJ.RecvFrom( (char*)recvBuf, 512, 5000 )!=4 )
		return false;

	return recvBuf[3]==0x09 ? true:false;
}