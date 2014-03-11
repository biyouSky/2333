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
	winShell.SetParam( hInstance, 0, 0, "Demo1_1" );

	// 创建win32窗口
	winShell.CreateWindowShell();

	// 启动消息循环
	return winShell.RunMessageLoop();
}

