// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"


class UIUIMain : public UIWinShell
{
public:
	void AddUIElements()
	{
		_win1.CreateDirect( &_uiTopContainer, GenRect()( 10, 10, 100, 100 ) );
		_win2.CreateDirect( &_uiTopContainer, GenRect()( 200, 10, 300, 100 ) );
	}

private:
	UIWinBase _win1;
	UIWinBase _win2;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo6_1" );

	// 创建win32窗口
	winShell.CreateWindowShell();

	// 与gxui绑定
	UIDXInit::GetSingleton()->InitD3D( winShell._hWnd, winShell.GetWidth(), winShell.GetHeight() );

	// 启动定时器
	SetTimer( winShell._hWnd, 1, 33, NULL );

	// 启动消息循环
	int rt = winShell.RunMessageLoop();

	// 单件模式资源释放
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();
	UIDXInit::GetSingleton()->Destroy();

	return rt;
}

