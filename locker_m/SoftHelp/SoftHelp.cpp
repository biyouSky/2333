
// wss_t1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include "SoftHelp.h"
#include "UISoftHelp.h"

__declspec(dllexport) char* PlugINInfo()
{
	return "";
}

// 初始化 创建
__declspec(dllexport) void PlugINInit( UIDXInit* pPara1, UIAnimateManage* pPara2, UIMessageLoopManage* pPara3, UICamera* pPara4, UIRender* pPara5, UIWinShell* pPara6 )
{
	UIDXInit::Assign( (UIDXInit*)pPara1 );
	UIAnimateManage::Assign( (UIAnimateManage*)pPara2 ); 
	UIMessageLoopManage::Assign( (UIMessageLoopManage*)pPara3 ); 
	UICamera::Assign( (UICamera*)pPara4 ); 
	UIRender::Assign( (UIRender*)pPara5 );
	UIWinShell::_pWinShell = (UIWinShell*)pPara6;
}

__declspec(dllexport) void PlugINCreate( UIContainer* pPara1, RECT* pPara2 )
{
	gUISoftHelp.CreateDirect( pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
}

// 绘制相关
__declspec(dllexport) void PlugINSetShow( bool flag )
{


	gUISoftHelp.ShowWindowDirect( flag );
}

__declspec(dllexport) void PlugINDraw()
{
	gUISoftHelp.Draw();
}

// menu信息
__declspec(dllexport) string* PlugINMenu( int& num )
{
	num = 2;
	static string mList[3] = { "帮助", "说明" };

	return mList;
}

__declspec(dllexport) void PlugINMenuNotify( int param )
{
	UIPostMessage( &gUISoftHelp, WM_NOTIFY, ID_MAINBAR, param );
}

// tile信息
__declspec(dllexport) char* PlugINTileTitle()
{
	return "软件说明";
}

__declspec(dllexport) int PlugINTileImageID()
{
	return IDB_IMAGE;
}


