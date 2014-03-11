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

	// ����win32���ڲ���
	winShell.SetParam( hInstance, 0, 0, "Demo3_1" );

	// ����win32����
	winShell.CreateWindowShell();

	// ��gxui��
	UIDXInit::GetSingleton()->InitD3D( winShell._hWnd, winShell.GetWidth(), winShell.GetHeight() );

	// ������Ϣѭ��
	int rt = winShell.RunMessageLoop();

	// ����ģʽ��Դ�ͷ�
	UIRender::GetSingleton()->Destroy();
	UIDXInit::GetSingleton()->Destroy();

	return rt;
}

