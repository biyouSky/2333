
#pragma once

#include "UIUtil.h"


/*
	windows������: �ṩһ�����
*/
class UIWinShell
{
public:
	UIWinShell();
	~UIWinShell();

	void SetParam( HINSTANCE hInstance, UINT iconID, UINT iconSmID, std::string titleStr );
	bool CreateWindowShell( int width=960, int height=640 );

	int RunMessageLoop();

	int GetWidth();
	int GetHeight();

	HWND _hWnd;						// ���ھ��

protected:
	ATOM RegisterWindowClass();
	bool InitInstance( int width, int height );

	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	HINSTANCE _hInstance;			// ��ǰʵ��
	UINT _iconID;					// ͼ��
	UINT _iconSmID;
	tstring _windowClass;			// ����������
	tstring _title;					// �������ı�
};