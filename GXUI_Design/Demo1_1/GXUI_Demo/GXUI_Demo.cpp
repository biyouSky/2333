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
	winShell.SetParam( hInstance, 0, 0, "Demo1_1" );

	// ����win32����
	winShell.CreateWindowShell();

	// ������Ϣѭ��
	return winShell.RunMessageLoop();
}

