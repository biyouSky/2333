#pragma once


class OSW32X : public SerialASYN
{
public:
	bool ComModeCall()		//ef ef 04 ff 07 01 e9
	{
		_recvFlag = false;

		UCHAR command[7];
		command[0] = 0xEF;
		command[1] = 0xEF;
		command[2] = 0x04;
		command[3] = 0xFF;
		command[4] = 0x07;
		command[5] = 0x01;
		command[6] = 0xE9;
		//
		ComSend( (char*)command, 7 );

		return _recvFlag;
	}

	//函数名：读OSW通道号
	bool GetChannel( int& channel )
	{
		_recvFlag = false;

		UCHAR command[6];
		command[0] = 0xEF;
		command[1] = 0xEF;
		command[2] = 0x03;
		command[3] = 0xFF;
		command[4] = 0x02;
		command[5] = 0xE2;
		//
		ComSend( (char*)command, 6 );

		if( _recvFlag )
			channel = strtol( _recvStr.substr( 15, 2 ).c_str(), 0, 16 );
	
		return _recvFlag;
	}

	bool SetChannel( int channel )
	{ 
		if(channel >32 || channel < 1 )
			return false;

		_recvFlag = false;
		
		UCHAR command[7]; 
		command[0] = 0xEF;
		command[1] = 0xEF;
		command[2] = 0x04;
		command[3] = 0xFF;
		command[4] = 0x04;
		command[5] = channel; 
		command[6] = ( 0xE5+channel )&0xFF;
		//
		ComSend( (char*)command, 7 );

		return _recvFlag;
	}

protected:
	bool ParaseRes( std::string& res )
	{
		_recvStr = res;
		_recvFlag = true;
		return true;
	}

	std::string _recvStr;
	bool _recvFlag;
};
