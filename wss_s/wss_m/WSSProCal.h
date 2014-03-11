
#pragma once


class WSSProCal
{	
	UdpOBJ _udpOBJ;

public:
	std::string _debugMsg;

public:
	// �趨LSB��ֵ
	bool SetLSB( ULONG ip, UCHAR lsb );

	// ͨ��wss����
	bool NotifyWssType( ULONG ip, UCHAR in, UCHAR out );

	// �ɼ�����
	bool StartCollectDataSyn( ULONG ip, UCHAR c, UCHAR p );
	bool GetCollectDataSyn( ULONG ip, UCHAR c, UCHAR p, int n, int max, int& rt, std::string& rtMsg, VECTOR_INT& volList, VECTOR_FLOAT& pmList );

	// ���Ͷ�������
	bool SendCalChannelPortDataSyn( ULONG ip, UCHAR temp, int c, int port, VECTOR_INT& curveCalcX, int yVol, int blockX2 );

	// ��ȡport����
	bool GetChannelPortPMSyn( ULONG ip, int p, float& portPM );

	// ͨ���ԴʧЧ
	bool NotifyLaserDying( ULONG ip );
};



/*-------------------------------------------�⿪��-------------------------------------------*/
class WSSOSW
{
	UdpOBJ _udpOBJ;
	ULONG _addr;

	enum { BufSize=32 };

public:
	WSSOSW( ULONG ip ) { _addr = ip; }

	bool Reset()
	{
		UCHAR recvBuf[BufSize], sendBuf[BufSize]; 	// ����ͽ��ջ���
		memset( sendBuf, 0, sizeof( sendBuf ) );
		memset( recvBuf, 0, sizeof( recvBuf ) );

		sendBuf[0] = 0x11;
		sendBuf[1] = 0x22;
		sendBuf[2] = 0x33;
		sendBuf[3] = 0x22;
		_udpOBJ.SendTo( (char*)sendBuf, 4, _addr );

		if( _udpOBJ.RecvFrom( (char*)recvBuf, BufSize, 3000 )!=4 )
			return false;

		if( recvBuf[3]!=0x22 )
			return false;

		return true;
	}

	bool SetSwitch( int port )
	{
		UCHAR recvBuf[BufSize], sendBuf[BufSize]; 	// ����ͽ��ջ���
		memset( sendBuf, 0, sizeof( sendBuf ) );
		memset( recvBuf, 0, sizeof( recvBuf ) );

		sendBuf[0] = 0x11;
		sendBuf[1] = 0x22;
		sendBuf[2] = 0x33;
		sendBuf[3] = 0x20;
		sendBuf[4] = port;
		_udpOBJ.SendTo( (char*)sendBuf, 5, _addr );

		int rt = _udpOBJ.RecvFrom( (char*)recvBuf, BufSize, 3000 );
		if( rt!=5 )
		{
			if( rt!=-1 )
				STRING_FORMAT( _debugMsg, "receive size %d != 5", rt );
			else
				_debugMsg = _udpOBJ.GetLastErrMsg();
			return false;
		}

		if( recvBuf[3]!=0x20 || recvBuf[4]!=port )
		{
			STRING_FORMAT( _debugMsg, "recvBuf[3](%0x)!=0x20 || recvBuf[4](%d)!=port", recvBuf[3], recvBuf[4] );
			return false;
		}

		return true;
	}

	bool GetSwitch( int& port )
	{
		UCHAR recvBuf[BufSize], sendBuf[BufSize]; 	// ����ͽ��ջ���
		memset( sendBuf, 0, sizeof( sendBuf ) );
		memset( recvBuf, 0, sizeof( recvBuf ) );

		sendBuf[0] = 0x11;
		sendBuf[1] = 0x22;
		sendBuf[2] = 0x33;
		sendBuf[3] = 0x21;
		_udpOBJ.SendTo( (char*)sendBuf, 4, _addr );

		if( _udpOBJ.RecvFrom( (char*)recvBuf, BufSize, 3000 )!=5 )
			return false;

		if( recvBuf[3]!=0x21 )
			return false;

		port = recvBuf[4];
		return true;
	}

	std::string _debugMsg;
};