#include "stdafx.h"
#include "GXUtil.h"
using namespace std;

void WCharToMByte::operator()( std::wstring wStr, std::string &str )
{
#ifndef _WIN32_
	throw 0;
#endif

	DWORD dwNum = WideCharToMultiByte( CP_OEMCP, NULL, wStr.c_str(), -1, NULL, 0, NULL, FALSE );
	char *psText;
	psText = new char[dwNum];

	WideCharToMultiByte ( CP_OEMCP, NULL, wStr.c_str() ,-1, psText, dwNum, NULL, FALSE );
	str = psText;
	delete []psText;
}

void MByteToWChar::operator()( std::string str, std::wstring &wStr )
{
#ifndef _WIN32_
	throw 0;
#endif

	DWORD dwNum = MultiByteToWideChar ( CP_ACP, 0, str.c_str(), -1, NULL, 0 );
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];

	MultiByteToWideChar ( CP_ACP, 0, str.c_str(), -1, pwText, dwNum );
	wStr = pwText;
	delete []pwText;
}


void StringFormat::operator()( string& str, const char* format, ... )
{
	// 最大缓存1024
	enum { MAX_SIZE = 1024 };
	char buf[MAX_SIZE] = { 0 };
	
	// 格式化 
	va_list vptr; 
	va_start( vptr, format ); 
	vsprintf( buf, format, vptr );
	va_end( vptr );

	str = buf;
}

unsigned long IP4_StrToULONG::operator()( const char* ipStr )
{
	if( string( ipStr )=="" )
		return 0;
	else
		return ntohl( inet_addr( ipStr ) );
}

char* IP4_ULONGToStr::operator()( const unsigned long ip )
{
	in_addr ipAddr;
	ipAddr.S_un.S_addr = htonl( ip );	

	return inet_ntoa( ipAddr );
}

TimeFunc::TimeFunc()
{
	GetCurTime();
}

void TimeFunc::GetCurTime()
{
	time_t t = time( 0 );
	_tm = *localtime( &t ); 
}

std::string TimeFunc::GetString()
{
	std::string str;
	STRING_FORMAT(	str, "%4d-%02d-%02d %02d:%02d:%02d", _tm.tm_year+1900, _tm.tm_mon+1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec); 
	return str;
}

void TimeFunc::SetDate( int year, int month, int monthday )
{
	memset( &_tm, 0, sizeof( tm ) );
	_tm.tm_year = year-1900;
	_tm.tm_mon = month-1;
	_tm.tm_mday = monthday;

	time_t t = mktime( &_tm );
	_tm = *localtime( &t ); 
}

void TimeFunc::GetTimeSpan( VECTOR_STRING& timeSpan )
{
	// 获得比较日期
	TimeFunc cmpTime;
	int cmpYear = cmpTime.GetYear();
	int cmpMonth = cmpTime.GetMonth();
	int cmpMonthDay = cmpTime.GetMonthDay();

	// 判断设定日期与比较日期的大小
	bool isSetDateLess = true;
	if( GetYear()>cmpYear )
	{
		isSetDateLess = false;
	}
	else if( GetYear()==cmpYear )
	{
		if( GetMonth()>cmpMonth )
		{
			isSetDateLess = false;
		}
		else if( GetMonth()==cmpMonth )
		{
			if( GetMonthDay()>cmpMonthDay )
			{
				isSetDateLess = false;
			}
			else if( GetMonthDay()==cmpMonthDay )
			{
				return;
			}
		}
	}

	// 计算中间日期
	const time_t dx = 24*3600;
	time_t tTemp = mktime( &_tm );
	while( 1 )
	{
		tTemp += isSetDateLess?dx:-dx;
		struct tm* pTM = localtime( &tTemp ); 

		// 判断是否到达目标比较日期
		if( pTM->tm_year+1900==cmpYear && pTM->tm_mon+1==cmpMonth && pTM->tm_mday==cmpMonthDay )
			break;

		string str;
		STRING_FORMAT( str, "%4d-%02d-%02d", pTM->tm_year+1900, pTM->tm_mon+1, pTM->tm_mday );
		timeSpan.push_back( str );
	}
}




#ifdef _WIN32_
std::string GetSysErrorMsg::operator()( int errCode )
{
	char buffer[256];
	memset( buffer, 0, 256 );
	::FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		errCode,
		0,
		buffer,
		256,
		NULL );
	return buffer;
}
#endif