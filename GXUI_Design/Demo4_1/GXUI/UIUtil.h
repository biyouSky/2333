
#pragma once

/*-------------------------------------------------- ��ܻ��� --------------------------------------------------*/
#define _0X_		1			// ���԰汾
#define _WIN32_		1			// ϵͳ
#define _MFC_		0			// ������
#define _CONSOLE_	0			// ����ģʽ


/*-------------------------------------------------- ����ͷ�ļ� --------------------------------------------------*/
// C��
#include <ctime>				// 
#include <cmath>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>				// only win����

// STL
#include <string>				// �ַ�������
#include <vector>				// ��������
#include <deque>
#include <list>
#include <algorithm>			// �㷨
#include <numeric>
#include <functional>			// �º���			
// �������
#include <iostream>
#include <fstream>

// win32
#ifdef _WIN32_
//
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

#endif

// directx9ͷ�ļ�
#include <d3dx9.h>											
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib,"d3dx9.lib" )
#pragma comment( lib, "winmm.lib" )


/*-------------------------------------------------------��ɫ��-------------------------------------------------------*/
#define _RED_			D3DCOLOR_XRGB(255, 0, 0)
#define _GREEN_			D3DCOLOR_XRGB(0, 255, 0)
#define _BLUE_			D3DCOLOR_XRGB(0, 0, 255)
#define _BLACK_			D3DCOLOR_XRGB(0, 0, 0)
#define _WHITE_			D3DCOLOR_XRGB(255, 255, 255)
#define _GOLD_			D3DCOLOR_XRGB(255, 215, 0 )
#define _ORANGE_		D3DCOLOR_XRGB(255, 231, 157)
#define _GRAY_			D3DCOLOR_XRGB(192, 192, 192)

#define _FRAMEBLUE_		D3DCOLOR_XRGB(89, 176, 255)
#define _FRAMEWHITE_	D3DCOLOR_XRGB(246, 250, 252)
#define _FRAMEPREPLE_	D3DCOLOR_XRGB(165, 153, 222)
#define _FRAMEGREEN_	D3DCOLOR_XRGB(108, 226, 108)

#define D3DCOLOR_MIX( alphy, rgb ) ((D3DCOLOR)((((alphy)&0xff)<<24)|((rgb)&0x00ffffff)))




/*-------------------------------------------------- UI���ߺ��� --------------------------------------------------*/

struct GenRect
{
	RECT operator()( POINT point, SIZE size );
	RECT operator()( LONG left, LONG top, LONG right, LONG bottom );
};

struct GenPoint
{
	POINT operator()( LONG x, LONG y );
};

struct GenSize
{
	SIZE operator()( LONG x, LONG y );
};

struct GetWidth
{
	int operator()( RECT& rc );
};

struct GetHeight
{
	int operator()( RECT& rc );
};

struct CompareRect
{
	bool operator()( RECT& r1, RECT& r2 );
};

struct CrossRect
{
	bool operator()( RECT& rc1, RECT& rc2, RECT& crossRC );
};


#define _NULLRECT_		GenRect()( 0, 0, 0, 0 )
#define _NULLPOINT_		GenPoint()( 0, 0 )
#define _NULLSIZE_		GenSize()( 0, 0 )




















/*-------------------------------------------------- ���Ͷ��� --------------------------------------------------*/
#ifdef _UNICODE
	typedef std::wstring			tstring;
#else
	typedef std::string				tstring;
#endif


/*-------------------------------------------------- ���ߺ��� --------------------------------------------------*/

// �ַ���ת��
struct WCharToMByte			// UNICODE16 -> ���ֽ�
{   
	void operator()( std::wstring wStr, std::string &str );
};

struct MByteToWChar			// ���ֽ� -> UNICODE16
{
	void operator()( std::string str, std::wstring &wStr );
};

// ��ʽ��string ����CString fromat
struct StringFormat
{
	void operator()( std::string& str, const char* format, ... );
};
#define STRING_FORMAT	StringFormat()
