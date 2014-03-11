// olm_os.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "olm_opm.h"
#include "UIOPM.h"
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
	if( !LoadOLMConfigDLL() )
		::MessageBoxA( 0, "Fail to load olm_config.dll !", "OS Warning", 0 );

	// ��������base
	if( !LoadOLMProDLL() )
		::MessageBoxA( 0, "Fail to load olm_pro.dll !", "OS Warning", 0 );

	// ��������base
	if( !LoadOLMDBDLL() )
		::MessageBoxA( 0, "Fail to load olm_db.dll !", "OS Warning", 0 );

	gUIOPM.CreateDirect( pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
}

// �������
__declspec(dllexport) void PlugINSetShow( bool flag )
{
	if( flag )
		gUIOPM.ShowSelDev();

	gUIOPM.ShowWindowDirect( flag );
}

__declspec(dllexport) void PlugINDraw()
{
	gUIOPM.Draw();
}

// menu��Ϣ
__declspec(dllexport) string* PlugINMenu( int& num )
{
	num = 2;

	static string menu[2] = { "״̬", "����" };
	return menu;
}

__declspec(dllexport) void PlugINMenuNotify( int param )
{
	UIPostMessage( &gUIOPM, WM_NOTIFY, ID_MAINBAR, param );
}

// tile��Ϣ
__declspec(dllexport) char* PlugINTileTitle()
{
	return "OPM���ʼ�";
}

__declspec(dllexport) int PlugINTileImageID()
{
	return IDB_IMAGE;
}