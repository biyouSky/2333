#include "stdafx.h"

#include "UIUtil.h"
using namespace std;


RECT GenRect::operator()( POINT point, SIZE size )
{
	RECT rc;
	rc.right = ( rc.left=point.x ) + size.cx;
	rc.bottom = ( rc.top=point.y ) + size.cy;
	return rc;
}

RECT GenRect::operator()( LONG left, LONG top, LONG right, LONG bottom )
{
	RECT rc;
	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;
	return rc;
}

POINT GenPoint::operator()( LONG x, LONG y )
{
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}

SIZE GenSize::operator()( LONG x, LONG y )
{
	SIZE s;
	s.cx = x;
	s.cy = y;
	return s;
}

int GetWidth::operator()( RECT& rc ) 
{ 
	return rc.right - rc.left; 
}

int GetHeight::operator()( RECT& rc ) 
{ 
	return rc.bottom - rc.top; 
}

bool CompareRect::operator()( RECT& r1, RECT& r2 )
{
	return ( r1.left==r2.left )&&( r1.right==r2.right )&&( r1.top==r2.top )&&( r1.bottom==r2.bottom );
}

bool CrossRect::operator()( RECT& rc1, RECT& rc2, RECT& crossRC )
{
	if( ( rc1.right<=rc2.left )||( rc2.right<=rc1.left )||( rc1.bottom<=rc2.top )||( rc2.bottom<=rc1.top ) )
		return false;

	crossRC.left = rc1.left>rc2.left ? rc1.left:rc2.left;
	crossRC.right = rc1.right<rc2.right ? rc1.right:rc2.right;
	crossRC.top = rc1.top>rc2.top ? rc1.top:rc2.top;
	crossRC.bottom = rc1.bottom<rc2.bottom ? rc1.bottom:rc2.bottom;
	return true;
}












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