// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIWinShell winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo5_1" );

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

