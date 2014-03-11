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

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo10_2" );

	// 创建win32窗口
	winShell.CreateWindowShell();

	// 启动消息循环
	int rt = winShell.RunMessageLoop();

	// 单件模式资源释放
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}

