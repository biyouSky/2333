#include "stdafx.h"

#include "UIUtil.h"
using namespace std;

void WCharToMByte::operator()( wstring wStr, string &str )
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

void MByteToWChar::operator()( string str, wstring &wStr )
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
	static char buf[MAX_SIZE];
	memset( buf, 0, MAX_SIZE );
	
	// 格式化 
	va_list vptr; 
	va_start( vptr, format ); 
	vsprintf( buf, format, vptr );
	va_end( vptr );

	str = buf;
}