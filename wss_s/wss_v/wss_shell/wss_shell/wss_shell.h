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

	// ����win32���ڲ���
	winShell.SetParam( hInstance, IDI_WSS, IDI_WSS, "WSS v6.3    ACCELINK" );

	// ����win32����
	winShell.CreateWindowShell( 1000, 680 );

	// ������Ϣѭ��
	int rt = winShell.RunMessageLoop();

	// ����ģʽ��Դ�ͷ�
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}