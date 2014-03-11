// wss_configBase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include "wss_configBase.h"
#include "UIConfigBase.h"



__declspec(dllexport) char* PlugINInfo()
{
	return "";
}

// ��ʼ�� ����
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
	// ��������baseConfig
	if( !LoadWSSConfigDLL() )
		::MessageBoxA( 0, "Fail to load wss_config.dll !", "Config Warning", 0 );

	// ��������basePro
	if( !LoadWSSProBaseDLL() )
		::MessageBoxA( 0, "Fail to load wss_basePro.dll !", "Config Warning", 0 );

	gUIConfig.CreateDirect( pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
}

// �������
__declspec(dllexport) void PlugINSetShow( bool flag )
{
	gUIConfig.ShowWindowDirect( flag );
}

__declspec(dllexport) void PlugINDraw()
{
	gUIConfig.Draw();
}

// menu��Ϣ
__declspec(dllexport) string* PlugINMenu( int& num )
{
	num = 0;
	return NULL;
}

__declspec(dllexport) void PlugINMenuNotify( int param )
{
}

// tile��Ϣ
__declspec(dllexport) char* PlugINTileTitle()
{
	return "Setting";
}

__declspec(dllexport) int PlugINTileImageID()
{
	return IDB_IMAGE;
}