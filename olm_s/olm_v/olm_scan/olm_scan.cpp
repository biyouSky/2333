// olm_scan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "olm_scan.h"
#include "UIScan.h"
#include "resource.h"


__declspec( dllexport ) char* PlugINInfo()
{
	return "";
}

// ��ʼ�� ����
__declspec( dllexport ) void PlugINInit(  UIDXInit* pPara1, UIAnimateManage* pPara2, UIMessageLoopManage* pPara3, UICamera* pPara4, UIRender* pPara5, UIWinShell* pPara6  )
{
	UIDXInit::Assign(  ( UIDXInit* )pPara1  );
	UIAnimateManage::Assign(  ( UIAnimateManage* )pPara2  ); 
	UIMessageLoopManage::Assign(  ( UIMessageLoopManage* )pPara3  ); 
	UICamera::Assign(  ( UICamera* )pPara4  ); 
	UIRender::Assign(  ( UIRender* )pPara5  );
	UIWinShell::_pWinShell = ( UIWinShell* )pPara6;
}

__declspec( dllexport ) void PlugINCreate(  UIContainer* pPara1, RECT* pPara2  )
{
	// ��������config
	if(  !LoadOLMConfigDLL()  )
		::MessageBoxA(  0, "Fail to load olm_config.dll !", "Scan Warning", 0  );

	// ��������base
	if(  !LoadOLMProDLL()  )
		::MessageBoxA(  0, "Fail to load olm_pro.dll !", "Scan Warning", 0  );

	gUIScan.CreateDirect(  pPara1, *pPara2, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y  );
}

// �������
__declspec( dllexport ) void PlugINSetShow(  bool flag  )
{
	gUIScan.ShowWindowDirect(  flag  );
}

__declspec( dllexport ) void PlugINDraw()
{
	gUIScan.Draw();
}

// menu��Ϣ
__declspec( dllexport ) string* PlugINMenu(  int& num  )
{
	num = 1;
	static string mList[1] = { "ɨ��" };

	return mList;
}

__declspec( dllexport ) void PlugINMenuNotify(  int param  )
{
	UIPostMessage(  &gUIScan, WM_NOTIFY, ID_MAINBAR, param  );
}

// tile��Ϣ
__declspec( dllexport ) char* PlugINTileTitle()
{
	return "ɨ���豸";
}

__declspec( dllexport ) int PlugINTileImageID()
{
	return IDB_IMAGE;
}