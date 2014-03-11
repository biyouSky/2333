
#pragma once

#include "UIUtil.h"
#include "UIBase.h"

/*
	windows窗口类: 提供一个外壳
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

	virtual void AddUIElements();		// 添加子元素于窗口上

	HWND _hWnd;							// 窗口句柄

	static UIContainer _uiTopContainer;	// 顶层容器
	static UIWinShell* _pWinShell;

protected:
	ATOM RegisterWindowClass();
	bool InitInstance( int width, int height );

	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	HINSTANCE _hInstance;				// 当前实例
	UINT _iconID;						// 图标
	UINT _iconSmID;
	tstring _windowClass;				// 主窗口类名
	tstring _title;						// 标题栏文本
};
	