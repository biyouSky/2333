// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"
using namespace std;


class UIUIMain : public UIWinShell
{
public:
	void AddUIElements()
	{
		RECT rc;
		::GetClientRect( _hWnd, &rc );

		HMODULE _handle = ::LoadLibraryA( "plugIN.dll" );
		//assert( _handle!=0 );

		if( _handle!=0 )
		{
			PFunc_PlugIN_Init PlugINInit = ( PFunc_PlugIN_Init )GetProcAddress( _handle, "PlugINInit" );
			PlugINInit( UIDXInit::GetSingleton(), UIAnimateManage::GetSingleton(), UIMessageLoopManage::GetSingleton(), 
						UICamera::GetSingleton(), UIRender::GetSingleton(), UIWinShell::_pWinShell );

			PFunc_PlugIN_Create PlugINCreate = ( PFunc_PlugIN_Create )GetProcAddress( _handle, "PlugINCreate" );
			PlugINCreate( &_uiTopContainer, &rc );
		}
		else
		{
			::MessageBoxA( 0, "plugIN.dll载入失败", "Warning", 0 );
		}
	}
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo10_1" );

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

