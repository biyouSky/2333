// GXUI_Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GXUI_Demo.h"
using namespace std;

class UIUIMain : public UIWinShell
{
public:
	void AddUIElements()
	{
		_lable1.Create( 0, &_uiTopContainer, GenRect()( 10, 10, 100, 30 ) );
		_lable1.SetString( "Test 测试" );
		_lable1.SetColor( _RED_ );
		_lable1.SetFontHeight( 24 );

		_image1.Create( 0, &_uiTopContainer, GenRect()( 100, 10, 200, 50 ) );
		_image1.SetImageIDAndDLL( IDB_OK );
		//_image1.PlayAnimateHitDrum();

		_but1.Create( 1, &_uiTopContainer, GenRect()( 300, 10, 400, 50 ) );
		_but1.SetString( "Button 按钮" );

		_checkBut1.Create( 0, &_uiTopContainer, GenRect()( 420, 10, 520, 50 ) );
		_checkBut1.SetString( "Check1" );
		_checkBut1.SetCheck( true );
		_checkBut2.Create( 0, &_uiTopContainer, GenRect()( 540, 10, 640, 50 ) );
		_checkBut2.SetString( "Check2" );
		UISetCheckButtonMutex( &_checkBut1, &_checkBut2 );

		_edit1.Create( 0, &_uiTopContainer, GenRect()( 660, 10, 760, 50 ) );
		_edit1.SetString( "Edit 编辑框" );

		_combo1.Create( 0, &_uiTopContainer, GenRect()( 800, 10, 880, 50 ) );
		for( int i=0; i<14; ++i )
		{
			string str;

			STRING_FORMAT( str, "Item%d", i+1 );
			_combo1.AddString( str );
		}

		_scrollBarX.Create( 0, &_uiTopContainer, GenRect()( 10, 50, 200, 70 ) );
		_scrollBarY.Create( 0, &_uiTopContainer, GenRect()( 210, 10, 230, 100 ) );
		_scrollBarY.SetCoordXY( 1 );

		_grid1.Create( 0, &_uiTopContainer, GenRect()( 10, 150, 350, 550 ) );
		_grid1.SetRowColumn( 300, 5 );
		_grid1.SetRowFix();
		_grid1.SetColumnFix();
		for( int i=0; i<300; ++i )
		{
			for( int j=0; j<5; ++j )
			{
				string str;
				STRING_FORMAT( str, "%d, %d", i, j );
				_grid1.SetCellString( i, j, str );
			}
		}

		_chart1.Create( 0, &_uiTopContainer, GenRect()( 400, 150, 900, 550 ) );
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

	void HandleMessageGame( UINT message, WPARAM wParam, LPARAM lParam )
	{
		if( message==WM_NOTIFY )
		{
			if( wParam==1 )
				_image1.PlayAnimateHitDrum();
		}
	}

private:
	UILable _lable1;
	UIImage _image1;
	UIButton _but1;
	UICheckButton _checkBut1;
	UICheckButton _checkBut2;
	UIEdit _edit1;
	UIComboBox _combo1;
	UIScrollBar _scrollBarX;
	UIScrollBar _scrollBarY;
	UIGrid _grid1;
	UIChart _chart1;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UIUIMain winShell;

	// 设置win32窗口参数
	winShell.SetParam( hInstance, 0, 0, "Demo8_1" );

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

