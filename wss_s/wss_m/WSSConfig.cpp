
#include "stdafx.h"
#include "WSSConfig.h"
using namespace std;

WSSConfig* WSSConfig::_pSingleton = NULL;

WSSConfig* WSSConfig::GetSingleton()
{
	if( _pSingleton==NULL )  
		_pSingleton = new WSSConfig;

	return _pSingleton; 
}

void WSSConfig::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

WSSConfig::WSSConfig()
{	
	int channelNum = 99;
	int inPort = 9;
	int outPort = 1;
	int freq = 50;
	_startChannelFreq = 191300;

	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	str += "\\Data\\";
	str += "BaseConfig.txt";
	TxtOBJ txt;
	if( txt.OpenFile( str ) )
	{
		vector<VECTOR_STRING > allLineWordsList;
		txt.ReadAllLineWords( allLineWordsList );
		if( allLineWordsList.size()==5 )
		{
			VECTOR_STRING& wordList0 = allLineWordsList[0];
			if( wordList0.size()==2 )
				inPort = atoi( wordList0[1].c_str() );

			VECTOR_STRING& wordList1 = allLineWordsList[1];
			if( wordList1.size()==2 )
				outPort = atoi( wordList1[1].c_str() );

			VECTOR_STRING& wordList2 = allLineWordsList[2];
			if( wordList2.size()==2 )
				freq = atoi( wordList2[1].c_str() );

			VECTOR_STRING& wordList3 = allLineWordsList[3];
			if( wordList3.size()==2 )
				channelNum = atoi( wordList3[1].c_str() );

			VECTOR_STRING& wordList4 = allLineWordsList[4];
			if( wordList4.size()==2 )
				_startChannelFreq = atoi( wordList4[1].c_str() );
		}
	}

	// 公共
	Init( channelNum, inPort, outPort, freq );
	_commMethod = WSSComm::COM;
	_deviceIP = IP4_StrToULONG()( "172.168.89.130" );

	// 定标
	_waitTemStableTime = 60*1000;
	_isOddRight = true;
	//
	_lsbRange = 16383;
	_lsb = 4;
}

void WSSConfig::Init( int channelNum, int inPortNum, int outPortNum, int freqGHz )
{
	_channelNum = channelNum;
	_inPortNum = inPortNum;
	_outPortNum = outPortNum;
	_freqGHz = freqGHz;

	CalcWSSType();
}

void WSSConfig::CalcWSSType()
{
	if( _inPortNum==1 && _outPortNum==1 )
		_wssType = TYPE_11;
	else if( _inPortNum==1 && _outPortNum==2 )
		_wssType = TYPE_12;
	else if( _inPortNum==2 && _outPortNum==1 )
		_wssType = TYPE_21;
	else if( _inPortNum==1 && _outPortNum>2 )
		_wssType = TYPE_1X;
	else if( _inPortNum>2 && _outPortNum==1 )
		_wssType = TYPE_X1;
	else
		_wssType = NOT_EXIST;
}

/*
	WSS_ITU.xls
	频率(GHz)	波长(nm)
*/
void WSSConfig::LoadCalITUList( string ituFile )
{
	// 载入定标用波长信息
	TxtOBJ txtOBJ;
	if( !txtOBJ.OpenFile( ituFile ) )
		return;

	vector<VECTOR_STRING > allLineWordsList;
	txtOBJ.ReadAllLineWords( allLineWordsList );
	//
	for( UINT i=0; i<allLineWordsList.size(); ++i )
	{
		VECTOR_STRING& wordList = allLineWordsList[i];
		if( wordList.size()==2 )
		{
			_ITUNMList[i] = wordList[0];
		}
	}
}

void WSSConfig::LoadITUGhzList( string ItuFile )
{
	// 载入分析用itu波长信息
	TxtOBJ txtOBJ;
	if( txtOBJ.OpenFile( ItuFile )==true )
	{
		_ITUGhz50G.clear();

		vector<VECTOR_STRING > allLineWordsList;
		txtOBJ.ReadAllLineWords( allLineWordsList );
		//
		UINT allSize = allLineWordsList.size();
		for( UINT i=1; i<allSize; ++i )
		{
			VECTOR_STRING& wordList = allLineWordsList[allSize-i];

			if( atoi( wordList[0].c_str() )%50==0 )
			{
				_ITUGhz50G.push_back( atoi( wordList[0].c_str() ) );

				if( _ITUGhz50G.back()%100==0 )
					_ITUGhz100G.push_back( _ITUGhz50G.back() );
			}
		}
	}
}


__declspec(dllexport) void InitWSSType( int channelNum, int inPortNum, int outPortNum, int freqGHz )
{
	WSSConfig::GetSingleton()->Init( channelNum, inPortNum, outPortNum, freqGHz );
}

__declspec(dllexport) int GetChannelNum()
{
	return WSSConfig::GetSingleton()->_channelNum;
}

__declspec(dllexport) int GetInPortNum()
{
	return WSSConfig::GetSingleton()->_inPortNum;
}

__declspec(dllexport) int GetOutPortNum()
{
	return WSSConfig::GetSingleton()->_outPortNum;
}

__declspec(dllexport) int GetPortNum()
{
	int& inPort = WSSConfig::GetSingleton()->_inPortNum;
	int& outPort = WSSConfig::GetSingleton()->_outPortNum;
	return inPort>outPort ? inPort:outPort;
}

__declspec(dllexport) int GetFreqGHz()
{
	return WSSConfig::GetSingleton()->_freqGHz;
}

__declspec(dllexport) int& GetCommMethod()
{
	return WSSConfig::GetSingleton()->_commMethod;
}

__declspec(dllexport) void SetCommMethod( int way )
{
	WSSConfig::GetSingleton()->_commMethod = way; 
}

__declspec(dllexport) ULONG& GetWSSIP()
{
	return WSSConfig::GetSingleton()->_deviceIP;
}

__declspec(dllexport) int GetWSSType()
{
	return WSSConfig::GetSingleton()->_wssType;
}

__declspec(dllexport) int GetStartChannelFreqGHz()
{
	return WSSConfig::GetSingleton()->_startChannelFreq;
}

__declspec(dllexport) void LoadCalITUList( const char* file )
{
	WSSConfig::GetSingleton()->LoadCalITUList( file );
}

__declspec(dllexport) string* GetITUNMList()
{
	return WSSConfig::GetSingleton()->_ITUNMList;
}

__declspec(dllexport) int GetLsbRange()
{
	return WSSConfig::GetSingleton()->_lsbRange;
}

__declspec(dllexport) int& GetLsb()
{
	return WSSConfig::GetSingleton()->_lsb;
}

__declspec(dllexport) bool& IsOddRight()
{
	return WSSConfig::GetSingleton()->_isOddRight;
}

__declspec(dllexport) int& GetWaitTempStableTime()
{
	return WSSConfig::GetSingleton()->_waitTemStableTime;
}

__declspec(dllexport) void LoadITUGhzList( const char* file )
{
	WSSConfig::GetSingleton()->LoadITUGhzList( file );
}

__declspec(dllexport) void* GetITUGhzList( int ghz )
{
	if( ghz!=50 && ghz!=100 )
		return 0;

	return ghz==50 ? &( WSSConfig::GetSingleton()->_ITUGhz50G ) : &( WSSConfig::GetSingleton()->_ITUGhz100G );
}