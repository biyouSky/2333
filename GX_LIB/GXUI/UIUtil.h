
#pragma once

#pragma once
#include "..\Utility\GXUtil.h"								// GX_LIB头文件
#include "..\Temporary\ThreadLogic.h"						// 获得线程支持
#include "..\FILE_IO\GXTXT.h"								// 获得文本支持支持

#include <d3dx9.h>											// 获得directx9支持
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib,"d3dx9.lib" )
#pragma comment( lib, "winmm.lib" )


/*-------------------------------------------------------颜色宏-------------------------------------------------------*/
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


#define IDR_MAIN_BAR					80001
#define IDB_HOT_EFFECT					80002
#define IDB_HOME_MENU					80003
#define IDB_HOT_EFFECT2					80004

#define IDB_BUTTON1_NORMAL              80011
#define IDB_BUTTON1_HOT                 80012
#define IDB_BUTTON1_DOWN                80013

#define IDB_CHECKBOX1_NOR     			80020
#define IDB_CHECKBOX1_HOT				80021
#define IDB_CHECKBOX1_TICK_NOR			80022
#define IDB_CHECKBOX1_TICK_HOT			80023

#define IDB_ARROW1_DOWN					80032

#define IDB_ARROW2_UP					80035
#define IDB_ARROW2_DOWN					80036
#define IDB_ARROW2_LEFT					80037
#define IDB_ARROW2_RIGHT				80038

#define IDB_OK							80041
#define IDB_CLOSE						80042


/*-------------------------------------------------------公用消息定义-------------------------------------------------------*/
#define WM_SIZERESET	WM_USER+1003
#define WM_UPDATE		WM_USER+1004
#define WM_REGANIMATE	WM_USER+1005



/*-------------------------------------------------- UI工具函数 --------------------------------------------------*/

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

struct ComparePoint
{
	bool operator()( POINT& p1, POINT& p2 );
};

struct IsPointInRect			
{	
	bool operator()( const POINT& point, RECT& rect );
};

struct IsKeyDown
{
	bool operator()( int key );
};

struct CalcZoomRect
{
	RECT operator()( RECT& rc, float zoomScale );
};

#define _NULLRECT_		GenRect()( 0, 0, 0, 0 )
#define _NULLPOINT_		GenPoint()( 0, 0 )
#define _NULLSIZE_		GenSize()( 0, 0 )



// 控件动态缩放辅助类
/*	父窗口
	-------------------------
	|			|			|
	|		   dy1			|
	|			|			|
	|		--------		|
	|-dx1-	|子窗口	|-dx2-	|
	|		|		|		|
	|		--------		|
	|			|			|
	|		   dy2			|
	|			|			|
	-------------------------
*/
struct UILayoutCalc
{
	// 设置时x、y分别只能设一次 
	enum zoomMode
	{
		NO_ZOOM = 0,
		MOVE_X = 1,			// bit 1	dx2不变  dx1改变  控件x方向上大小不变
		MOVE_Y = 2,			// bit 2	dy2不变  dy1改变  控件y方向上大小不变
		SIZE_X = 4,			// bit 3	dx1和dx2都不变    控件x方向上大小改变
		SIZE_Y = 8,			// bit 4	dy1和dy2都不变    控件y方向上大小改变
		SCALE_X = 16,		// bit 5	dx1，dx2按比例变
		SCALE_Y = 32,		// bit 6	dy1，dy2按比例变
	};

	UILayoutCalc( int flag=SIZE_X|SIZE_Y );

	void SetLayoutMode( int flag );							// 设置缩放模式
	void InitLayout( RECT& parentRect, RECT& rect );		// 初始计算 普通模式、directUI模式
	RECT CalcLayout( LONG cx, LONG cy );					// 计算新状态
	
	int _zoomModeflag;		// 布局模式

	RECT _parentRect;		// 父窗体的区域
	RECT _rect;				// 控件相对父窗体区域
};


struct StringPasteFromClipboard
{
	void operator()( std::wstring &strBuf, HWND dstHwnd );
};

struct StringCopyToClipboard
{
	void operator()( std::wstring &strBuf, HWND srcHwnd );
};
