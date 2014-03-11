// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"
using namespace std;


class UIUIMain : public UIWinShell
{
	UIStartScreen _startScreen;

public:
	void AddUIElements()
	{
		RECT rc;
		::GetClientRect( _hWnd, &rc );

		string path;
		GetExeDirectory( path );

		_startScreen.Create( 0, &_uiTopContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_startScreen.LoadPlugINs( path+"\\PlugIN" );
	}
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// ����win32���ڲ���
	winShell.SetParam( hInstance, 0, 0, "Demo10_2" );

	// ����win32����
	winShell.CreateWindowShell();

	// ������Ϣѭ��
	int rt = winShell.RunMessageLoop();

	// ����ģʽ��Դ�ͷ�
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}

