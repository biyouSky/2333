#pragma once

#include "resource.h"
using namespace std;

class UIOLM : public UIWinShell
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
	UIOLM olmShell;

	// 设置win32窗口参数
	olmShell.SetParam( hInstance, IDI_OLM, IDI_OLM, "OLM v4.0    ACCELINK" );

	// 创建win32窗口
	olmShell.CreateWindowShell( 1024, 720 );

	// 启动消息循环
	int rt = olmShell.RunMessageLoop();

	// 单件模式资源释放
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}