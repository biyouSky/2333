// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"


class UIGhost : public UIAnimateBase
{
public:
	UIGhost()
	{
		_frameIndex = 1;
		_posX = 100;
		_posY = 300;
	}

	bool IsRun()
	{
		return true;
	}

	bool IsDrawFrame()
	{
		DWORD nowTick = ::GetTickCount();
		DWORD dxTick = abs( (int)( nowTick-_oldTick ) );
		if( dxTick<1000 )
			return false;

		_oldTick = nowTick; 
		return true;
	}

	void DrawAuto()
	{
		UI2DImage image;
		image.Init( "ghosts24.bmp", 0.5f, _BLACK_ );

		int dx = ( image._srcInfo.Width-MAX_FRAME-1 )/MAX_FRAME;

		int srcX = ( _frameIndex-1 )*dx+_frameIndex;
		RECT rc = GenRect()( srcX, 1, srcX+dx-1, image._srcInfo.Height-1 );
		image.operator()( GenRect()( srcX, 1, srcX+dx-1, image._srcInfo.Height-1 ), (ULONG)_posX, (ULONG)_posY );

		if( ++_frameIndex>3 )
			_frameIndex = 1;
	}

private:
	enum 
	{
		MAX_FRAME = 3
	};

	DWORD _oldTick;
	int _frameIndex;

	int _posX;
	int _posY;
	
};

class UIUIMain : public UIWinShell
{
public:
	void AddUIElements()
	{
		_win1.CreateDirect( &_uiTopContainer, GenRect()( 10, 10, 100, 100 ), UILayoutCalc::SIZE_X|UILayoutCalc::MOVE_Y );
		_win2.CreateDirect( &_uiTopContainer, GenRect()( 200, 10, 300, 100 ) );

		UIRegisterAnimate( &ghost );
	}

private:
	UIWinBase _win1;
	UIWinBase _win2;
	UIGhost ghost;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo7_1" );

	// 创建win32窗口
	winShell.CreateWindowShell();

	// 启动定时器
	//SetTimer( winShell._hWnd, 1, 33, NULL );

	// 启动消息循环
	int rt = winShell.RunMessageLoop();

	// 单件模式资源释放
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}

