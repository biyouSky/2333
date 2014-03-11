#pragma once

#include "resource.h"
using namespace std;

class UIWSS : public UIWinShell
{
public:
	void AddUIElements();
	void ShowPlugINMsg( const std::string msg );

private:
	UIStartScreen _startScreen;
};





int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIWSS winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, IDI_WSS, IDI_WSS, "WSS v6.3    ACCELINK" );

	// 创建win32窗口
	winShell.CreateWindowShell( 1000, 680 );

	// 启动消息循环
	int rt = winShell.RunMessageLoop();

	// 单件模式资源释放
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}