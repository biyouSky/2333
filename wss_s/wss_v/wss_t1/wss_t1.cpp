
// wss_t1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include "UIT1.h"
#include "wss_t1.h"


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
		::MessageBoxA( 0, "Fail to load wss_config.dll !", "Warning", 0 );

	// ��������basePro
	if( !LoadWSSProBaseDLL() )
		::MessageBoxA( 0, "Fail to load wss_basePro.dll !", "Warning", 0 );

	gUIT1.CreateDirect( pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
}

// �������
__declspec(dllexport) void PlugINSetShow( bool flag )
{
	if( flag )
		gUIT1.UIInitPortFreq();

	gUIT1.ShowWindowDirect( flag );
}

__declspec(dllexport) void PlugINDraw()
{
	gUIT1.Draw();
}

// menu��Ϣ
__declspec(dllexport) string* PlugINMenu( int& num )
{
	num = 4;
	static string mList[4] = { "����", "�ֵ�", "�Ե�", "����" };

	return mList;
}

__declspec(dllexport) void PlugINMenuNotify( int param )
{
	UIPostMessage( &gUIT1, WM_NOTIFY, ID_MAINBAR, param );
}

// tile��Ϣ
__declspec(dllexport) char* PlugINTileTitle()
{
	return "����1";
}

__declspec(dllexport) int PlugINTileImageID()
{
	return IDB_IMAGE;
}


