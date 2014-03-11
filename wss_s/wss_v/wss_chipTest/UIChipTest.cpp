#include "stdafx.h"
#include "UIChipTest.h"

#include "..\..\wss_m\WSSCal.h"
//#include "..\..\wss_m\WSSProCal.h"

using namespace std;

UIChipTest gUITChipTest;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
	UIRefresh();
}


void ViewCollectMsg( ULONG ip, const char* msg )
{
	View0( msg );
}

void UIChipTest::OnCreate()
{
	/*_lable1.Create( 0, this );
	_lable1.SetString( "WSS IP:" );
	_edit1.Create( 0, this );
	_edit1.SetString( "172.168.89.121" );*/

	_checkBut1.Create( 4, this );
	_checkBut1.SetString( "奇曲线" );
	_checkBut1.SetCheck( true );

	_checkBut2.Create( 5, this );
	_checkBut2.SetString( "偶曲线" );
	_checkBut2.SetCheck( true );

	_lable2.Create( 0, this );
	_lable2.SetString( "当前通道:" );
	_edit2.Create( 0, this );
	_edit2.SetString( "1" );

	_but1.Create( 1, this );
	_but1.SetString( "前一通道" );

	_but2.Create( 2, this );
	_but2.SetString( "后一通道" );

	_but3.Create( 3, this );
	_but3.SetString( "进行测试" );

	_chart1.Create( 0, this, GenRect()( 20, 90, 960, 580 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );

	// 设置布局信息
	_layoutGrid1.InitPoint( GenPoint()( 20, 20 ) );
	_layoutGrid1.SetRowColumn( 1, 11 );
	/*_layoutGrid1.SetColumnWidthInterval( 0, 80 );
	_layoutGrid1.SetColumnWidthInterval( 1, 150, 30 );*/
	_layoutGrid1.SetColumnWidthInterval( 1, 100, 30 );
	_layoutGrid1.SetColumnWidthInterval( 2, 80 );
	_layoutGrid1.SetColumnWidthInterval( 3, 60, 30 );
	_layoutGrid1.SetColumnWidthInterval( 5, 100, 30 );
	/*_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_edit1 );*/
	_layoutGrid1.SetCell( 0, 0, &_checkBut1 );
	_layoutGrid1.SetCell( 0, 1, &_checkBut2 );
	_layoutGrid1.SetCell( 0, 2, &_lable2 );
	_layoutGrid1.SetCell( 0, 3, &_edit2 );
	_layoutGrid1.SetCell( 0, 4, &_but1 );
	_layoutGrid1.SetCell( 0, 5, &_but2 );
	_layoutGrid1.SetCell( 0, 6, &_but3 );
	//
	_layoutGrid1.CalcGridPos();
}

void UIChipTest::OnMainBar( int param )
{
	//_tabX1.SetCurCell( param-1 );
}

void UIChipTest::OnPreChannel( int param )
{
	string str;
	_edit2.GetString( str );

	int curC = atoi( str.c_str() );
	--curC;
	if( curC<1 )
		curC=1;

	STRING_FORMAT( str, "%d", curC );
	_edit2.SetString( str );
}

void UIChipTest::OnNxtChannel( int param )
{
	string str;
	_edit2.GetString( str );

	int curC = atoi( str.c_str() );
	++curC;
	if( curC>CHANNEL_NUM )
		curC=CHANNEL_NUM;

	STRING_FORMAT( str, "%d", curC );
	_edit2.SetString( str );
}

void UIChipTest::OnStartChipTest( int param )
{
	StartThread( "ChipTestThread", &UIChipTest::ChipTestThread );
}

void UIChipTest::OnShowOdd( int param )
{
	ShowChart();
}

void UIChipTest::OnShowEven( int param )
{
	ShowChart();
}

void UIChipTest::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
		OnMainBar( param );
	else if( id==1 )
		OnPreChannel( param );
	else if( id==2 )
		OnNxtChannel( param );
	else if( id==3 )
		OnStartChipTest( param );
	else if( id==4 )
		OnShowOdd( param );
	else if( id==5 )
		OnShowEven( param );
}

void UIChipTest::ShowChart()
{
	_chart1.Clear();

	bool flag;
	_checkBut1.GetCheck( flag );
	if( flag )
	{
		_chart1.AddCurve1( "奇1", _xList1[0], _yList1[0] );
		//_chart1.AddCurve1( "奇2", _xList1[1], _yList1[1] );
		//_chart1.SetCurve1Color( "奇2", _RED_ );
	}

	_checkBut2.GetCheck( flag );
	if( flag )
	{
		_chart1.AddCurve1( "偶1", _xList2[0], _yList2[0] );
		//_chart1.AddCurve1( "偶2", _xList2[1], _yList2[1] );
		_chart1.SetCurve1Color( "偶1", _RED_ );
	}
	_chart1.CalcXYCoordRange();
	UIRefresh();
}

void UIChipTest::ChipTestThread()
{
	PCB_View_ShowCollectMsg = ViewCollectMsg;

	// 获取要测试的wss的ip
	string str;
	/*_edit1.GetString( str );
	ULONG wssIP = IP4_StrToULONG()( str.c_str() );*/
	ULONG wssIP = GetWSSIP();

	// 获取当前要测试的通道
	_edit2.GetString( str );
	int curC = atoi( str.c_str() );

	// 测试按1*2定标方法进行
	WSSProCal proUDP;
	while( proUDP.NotifyWssType( wssIP, 1, 2 )==false ) { Sleep( 100 ); }
	while( proUDP.SetLSB( wssIP, 4 )==false );

	// 设置当前波长
	View0( "设置当前波长.." );


	// 2次获取采集数据
	VECTOR_INT volList1[2], volList2[2];
	VECTOR_FLOAT pmList1[2], pmList2[2];

	for( int i=0; i<1; ++i )
	{
		STRING_FORMAT( str, "第%d次采集..", i+1 );
		View0( str.c_str() );

		View0( "奇数通道数据采集.." );
		CollectDataProcess::GetSingleton()->CollecteData_Core( wssIP, curC, 1, volList1[i], pmList1[i] );

		View0( "偶数通道数据采集.." );
		CollectDataProcess::GetSingleton()->CollecteData_Core( wssIP, curC, 2, volList2[i], pmList2[i] );

		Sleep( 500 );
	}

	// 保存数据
	for( int i=0; i<1; ++i )
	{
		_xList1[i].clear();
		_yList1[i].clear();
		_xList2[i].clear();
		_yList2[i].clear();

		for( UINT j=0; j<volList1[0].size(); ++j )
		{
			_xList1[i].push_back( volList1[i][j] );
			_yList1[i].push_back( pmList1[i][j] );

			_xList2[i].push_back( volList2[i][j] );
			_yList2[i].push_back( pmList2[i][j] );
		}
	}

	// 绘图
	ShowChart();

	// 对数据进行检验

}