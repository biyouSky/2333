#include "stdafx.h"

#include "WinShell.h"
#include "UIRender.h"
#include "UIBase.h"

using namespace std;

UIContainer UIWinShell::_uiTopContainer;
UIWinShell* UIWinShell::_pWinShell = 0;

UIWinShell::UIWinShell()
{
	_hInstance = NULL;
	_hWnd = NULL;

	_iconID = -1;
	_iconSmID = -1;

	_windowClass = _T("GXUI");

	_pWinShell = this;
}

UIWinShell::~UIWinShell() {}

void UIWinShell::SetParam( HINSTANCE hInstance, UINT iconID, UINT iconSmID, string titleStr )
{
	_hInstance = hInstance;

#ifdef _UNICODE
	std::wstring wStr;
	MByteToWChar()( titleStr, wStr );
	_title = wStr;
#else
	_title = titleStr;
#endif

	_iconID = iconID;
	_iconSmID = iconSmID; 
}

ATOM UIWinShell::RegisterWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= UIWinShell::WndProc;									// 消息回调处理函数
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= _hInstance;
	wcex.hIcon			= LoadIcon( _hInstance, MAKEINTRESOURCE( _iconID ) );
	wcex.hCursor		= LoadCursorW( NULL, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)( COLOR_WINDOW+1 );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _windowClass.c_str();
	wcex.hIconSm		= LoadIcon( _hInstance, MAKEINTRESOURCE( _iconSmID ) );

	return RegisterClassEx( &wcex );
}

bool UIWinShell::InitInstance( int width, int height )
{
	_hWnd = ::CreateWindowEx( WS_EX_WINDOWEDGE /* | WS_EX_TOOLWINDOW*/, _windowClass.c_str(), 
				_title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, _hInstance, this );

	if( !_hWnd ) return false;

	// 窗口移动到中间
	{
		HWND parentHwnd;
		RECT rc, rc2;
		int    x,y;
		//
		if( ( parentHwnd = GetParent( _hWnd ) )==NULL )
			SystemParametersInfo( SPI_GETWORKAREA, 0, &rc, 0 );
		else
			GetClientRect( parentHwnd, &rc );

		GetWindowRect( _hWnd, &rc2 );
		x = ( ( rc.right-rc.left ) - ( rc2.right-rc2.left ) )/2 +rc.left;
		y = ( ( rc.bottom-rc.top ) - ( rc2.bottom-rc2.top ) )/2 +rc.top;
		//
		SetWindowPos( _hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE );
	}

	ShowWindow( _hWnd, SW_SHOW );
	UpdateWindow( _hWnd );

	return true;
}

int UIWinShell::RunMessageLoop()
{
	// 启动dui界面的消息循环
	UIMessageLoopManage::GetSingleton()->RunMessageLoop();

	// 启动win32的消息循环
	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK UIWinShell::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	// 传递给GXUI消息循环
	UIPostMessage( NULL, message, wParam, lParam );

	// win32处理
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		UIPostMessage( NULL, WM_SIZERESET, wParam, lParam );
		break;
	case WM_TIMER:
		::SendMessageA( hWnd, WM_PAINT, 0, 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

void UIWinShell::AddUIElements()
{
}

void UIWinShell::HandleMessageGame( UINT message, WPARAM wParam, LPARAM lParam )
{
}

bool UIWinShell::CreateWindowShell( int width, int height )
{
	RegisterWindowClass();
	if( InitInstance( width, height )==false ) 
		return false;

	_uiTopContainer.BindWin( _hWnd );

	return true;
}

int UIWinShell::GetWidth()
{ 
	RECT rc;
	if( ::GetClientRect( _hWnd, &rc )==TRUE )
		return rc.right-rc.left; 
	else
		return -1;
}

int UIWinShell::GetHeight()
{ 
	RECT rc;
	if( ::GetClientRect( _hWnd, &rc )==TRUE )
		return rc.bottom-rc.top; 
	else
		return -1;
}