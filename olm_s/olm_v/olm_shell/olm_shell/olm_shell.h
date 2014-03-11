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

	// ����win32���ڲ���
	olmShell.SetParam( hInstance, IDI_OLM, IDI_OLM, "OLM v4.0    ACCELINK" );

	// ����win32����
	olmShell.CreateWindowShell( 1024, 720 );

	// ������Ϣѭ��
	int rt = olmShell.RunMessageLoop();

	// ����ģʽ��Դ�ͷ�
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}