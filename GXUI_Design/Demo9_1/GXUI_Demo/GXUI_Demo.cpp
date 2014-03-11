// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"
using namespace std;

class WindowTT : public UIWindow<WindowTT>
{
friend UIWindow;

private:
	void OnCreate()
	{
		_canvas.CreateDirect( &_tab1, GenRect()( 0, 0, 0, 0 ) );
		//
		{
			_lable1.Create( 0, &_canvas );
			_lable1.SetString( "Test 测试" );
			_lable1.SetColor( _RED_ );
			_lable1.SetFontHeight( 24 );

			_image1.Create( 0, &_canvas );
			_image1.SetImageIDAndDLL( IDB_OK );
			//_image1.PlayAnimateHitDrum();

			_but1.Create( 1, &_canvas );
			_but1.SetString( "Button 按钮" );

			_checkBut1.Create( 0, &_canvas );
			_checkBut1.SetString( "Check1" );
			_checkBut1.SetCheck( true );
			_checkBut2.Create( 0, &_canvas );
			_checkBut2.SetString( "Check2" );
			UISetCheckButtonMutex( &_checkBut1, &_checkBut2 );

			_edit1.Create( 0, &_canvas );
			_edit1.SetString( "Edit 编辑框" );

			_combo1.Create( 0, &_canvas );
			for( int i=0; i<14; ++i )
			{
				string str;

				STRING_FORMAT( str, "Item%d", i+1 );
				_combo1.AddString( str );
			}

			_scrollBarX.Create( 0, &_canvas );
			_scrollBarY.Create( 0, &_canvas );
			_scrollBarY.SetCoordXY( 1 );
		}

		{
			_grid1.Create( 0, &_tab1, GenRect()( 10, 150, 350, 550 ) );
			_grid1.SetRowColumn( 300, 10 );
			_grid1.SetRowFix();
			_grid1.SetColumnFix();
			for( int i=0; i<300; ++i )
			{
				for( int j=0; j<10; ++j )
				{
					string str;
					STRING_FORMAT( str, "%d, %d", i, j );
					_grid1.SetCellString( i, j, str );
				}
			}
		}

		{
			_chart1.Create( 0, &_tab1, GenRect()( 400, 150, 900, 550 ) );
			VECTOR_DOUBLE xList, y1List, y2List;
			for( int i=0; i<20; ++i )
			{
				xList.push_back( i );
				y1List.push_back( rand()%1000 );
				y2List.push_back( rand()%100 );
			}
			_chart1.AddCurve1( "y11", xList, y1List );
			_chart1.AddCurve2( "y21", xList, y2List );
			_chart1.SetCurve2Color( "y21", _RED_ );
			_chart1.SetCurve2Select( "y21" );
			_chart1.CalcXYCoordRange();
		}

		_tab1.Create( 0, this, GenRect()( 20, 20, 900, 550 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_tab1.SetCellNum( 3 );
		_tab1.SetCell( 0, "grid", &_grid1 );
		_tab1.SetCell( 1, "chart", &_chart1 );
		_tab1.SetCell( 2, "canvas", &_canvas );

		// 布局计算
		_layoutGrid1.InitPoint( GenPoint()( 50, 50 ) );
		_layoutGrid1.SetRowColumn( 8, 2, 150, 20, 35, 20 );
		//
		_layoutGrid1.SetCell( 0, 0, &_lable1 );
		_layoutGrid1.SetCell( 1, 0, &_image1 );
		_layoutGrid1.SetCell( 1, 1, &_but1 );
		_layoutGrid1.SetCell( 2, 0, &_checkBut1 );
		_layoutGrid1.SetCell( 2, 1, &_checkBut2 );
		_layoutGrid1.SetCell( 3, 0, &_edit1 );
		_layoutGrid1.SetCell( 3, 1, &_combo1 );
		_layoutGrid1.SetCell( 4, 0, &_scrollBarX );
		_layoutGrid1.SetCell( 4, 1, 5, 1, &_scrollBarY );
		//
		_layoutGrid1.CalcGridPos();
	}

	void OnNotify( WPARAM wParam, LPARAM lParam )
	{
		if( wParam==1 )
			_image1.PlayAnimateHitDrum();
	}

	UIGrid _grid1;
	//
	UIChart _chart1;
	UITab _tab1;
	//
	UICanvas _canvas;
	//
	UILable _lable1;
	UIImage _image1;
	UIButton _but1;
	UICheckButton _checkBut1;
	UICheckButton _checkBut2;
	UIEdit _edit1;
	UIComboBox _combo1;
	UIScrollBar _scrollBarX;
	UIScrollBar _scrollBarY;

	UILayoutGrid _layoutGrid1;
};

class UIUIMain : public UIWinShell
{
public:
	void AddUIElements()
	{
		RECT rc;
		::GetClientRect( _hWnd, &rc );

		winTT.CreateDirect( &_uiTopContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	}

private:
	WindowTT winTT;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo9_1" );

	// 创建win32窗口
	winShell.CreateWindowShell();

	// 启动消息循环
	int rt = winShell.RunMessageLoop();

	// 单件模式资源释放
	UIAnimateManage::GetSingleton()->Destroy();
	UIMessageLoopManage::GetSingleton()->Destroy();
	UICamera::GetSingleton()->Destroy();
	UIRender::GetSingleton()->Destroy();

	return rt;
}

