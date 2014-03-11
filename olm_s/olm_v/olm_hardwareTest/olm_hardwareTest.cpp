// olm_scan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "olm_hardwareTest.h"
#include "UIHardwareTest.h"
#include "resource.h"


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
	// ��������config
	//if( !LoadOLMConfigDLL() )
	//	::MessageBoxA( 0, "Fail to load olm_config.dll !", "HardwareTest Warning", 0 );

	// ��������base
	if( !LoadOLMProDLL() )
		::MessageBoxA( 0, "Fail to load olm_pro.dll !", "HardwareTest Warning", 0 );

	gUIHardwareTest.CreateDirect( pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
}

// �������
__declspec(dllexport) void PlugINSetShow( bool flag )
{
	gUIHardwareTest.ShowWindowDirect( flag );
}

__declspec(dllexport) void PlugINDraw()
{
	gUIHardwareTest.Draw();
}

// menu��Ϣ
__declspec(dllexport) string* PlugINMenu( int& num )
{
	num = 0;
	return 0;
}

__declspec(dllexport) void PlugINMenuNotify( int param )
{
	UIPostMessage( &gUIHardwareTest, WM_NOTIFY, ID_MAINBAR, param );
}

// tile��Ϣ
__declspec(dllexport) char* PlugINTileTitle()
{
	return "Ӳ������";
}

__declspec(dllexport) int PlugINTileImageID()
{
	return IDB_IMAGE;
}