#include "stdafx.h"
#include "UIT1.h"
using namespace std;

UIT1 gUIT1;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );

	//gUILog.WriteLog( msg );
	UIRefresh();
}

void View101( VECTOR_FLOAT& xList, VECTOR_FLOAT& yList, VECTOR_FLOAT& xYList, VECTOR_FLOAT& yYList )
{
	VECTOR_DOUBLE xList2, yList2, xYList2, yYList2;
	for( UINT i=0; i<xList.size(); ++i )
	{
		xList2.push_back( xList[i] );
		yList2.push_back( yList[i] );
	}
	for( UINT i=0; i<xYList.size(); ++i )
	{
		xYList2.push_back( xYList[i] );
		yYList2.push_back( yYList[i] );
	}

	gUIT1.ShowYLCurve( xList2, yList2, xYList2, yYList2 );
	UIRefresh();
}

void View102()
{
	gUIT1.ShowYLGrid();
	UIRefresh();
}


void UIT1::OnCreate()
{
	//
	PCB_View_ShowMsg = View0;
	PCB_View_ShowYLCurve = View101;
	PCB_View_ShowYLGrid = View102;

	//
	string str;

	_tabX1.Create( 0, this, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 4 );
	_tabX1.SetCell( 0, "设置", &_canvas1 );
	_tabX1.SetCell( 1, "粗电", &_canvas3 );
	_tabX1.SetCell( 2, "自电", &_canvas4 );
	_tabX1.SetCell( 3, "黄线", &_canvas2 );

	{
		_canvas3.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );

		_lable31.Create( 0, &_canvas3 );
		_lable31.SetString( "提示：对单通道的电压设置直接在对应表格单元内按Enter键即可！" );

		_grid31.Create( 31, &_canvas3, GenRect()( 30, 130, 320, 980 ), UILayoutCalc::SIZE_Y );
		_grid32.Create( 32, &_canvas3, GenRect()( 360, 130, 660, 980 ), UILayoutCalc::SIZE_Y );

		_checkBut33.Create( 0, &_canvas3 );
		_checkBut33.SetString( "绝对电压:" );
		_checkBut33.SetCheck( true );
		_edit31.Create( 0, &_canvas3 );
		_edit31.SetString( "0" );
		_checkBut34.Create( 0, &_canvas3 );
		_checkBut34.SetString( "相对电压:" );
		_edit33.Create( 0, &_canvas3 );
		_edit33.SetString( "50" );
		UISetCheckButtonMutex( &_checkBut33, &_checkBut34 );

		_checkBut31.Create( 0, &_canvas3 );
		_checkBut31.SetString( "X轴" );
		_checkBut31.SetCheck( true );
		_checkBut32.Create( 0, &_canvas3 );
		_checkBut32.SetString( "Y轴" );
	
		_but31.Create( 33, &_canvas3 );
		_but31.SetString( "设置全部通道电压" );
		_but32.Create( 34, &_canvas3 );
		_but32.SetString( "设置奇数通道电压" );
		_but33.Create( 35, &_canvas3 );
		_but33.SetString( "设置偶数通道电压" );
		_but34.Create( 36, &_canvas3 );
		_but34.SetString( "设置选中通道电压" );
		_but35.Create( 37, &_canvas3 );
		_but35.SetString( "清除结果" );
		_but36.Create( 38, &_canvas3 );
		_but36.SetString( "load精确电压" );
		_but37.Create( 39, &_canvas3 );
		_but37.SetString( "save电压" );
		_but40.Create( 42, &_canvas3 );
		_but40.SetString( "load粗略电压" );

		_but38.Create( 41, &_canvas3 );
		_but38.SetString( "电压路径" );
		_edit32.Create( 0, &_canvas3 );
		_edit32.SetString( "" );

		_but39.Create( 1001, &_canvas3 );
		_but39.SetString( "port:" );
		_edit34.Create( 0, &_canvas3 );
		_edit34.SetString( "1" );

		_layoutGrid32.InitPoint( GenPoint()( 30, 20 ) );
		_layoutGrid32.SetRowColumn( 2, 8 );
		//
		_layoutGrid32.SetCell( 0, 0, &_but38 );
		_layoutGrid32.SetCell( 0, 1, 0, 3, &_edit32 );
		_layoutGrid32.SetCell( 1, 0, 1, 3, &_lable31 );
		_layoutGrid32.SetCell( 0, 4, &_but36 );
		_layoutGrid32.SetCell( 0, 5, &_but37 );
		_layoutGrid32.SetCell( 0, 6, &_but40 );
		_layoutGrid32.SetCell( 1, 4, &_but39 );
		_layoutGrid32.SetCell( 1, 5, &_edit34 );
		_layoutGrid32.SetColumnWidthInterval( 3, 180 );
		for( int c=4; c<7; ++c )
			_layoutGrid32.SetColumnWidthInterval( c, 110 );
		//
		_layoutGrid32.CalcGridPos();

		_layoutGrid31.InitPoint( GenPoint()( 710, 20 ) );
		_layoutGrid31.SetRowColumn( 15, 2 );
		//
		_layoutGrid31.SetCell( 3, 0, &_checkBut33 );
		_layoutGrid31.SetCell( 3, 1, &_edit31 );
		_layoutGrid31.SetCell( 4, 0, &_checkBut34 );
		_layoutGrid31.SetCell( 4, 1, &_edit33 );
		_layoutGrid31.SetCell( 5, 0, &_checkBut31 );
		_layoutGrid31.SetCell( 5, 1, &_checkBut32 );
		_layoutGrid31.SetCell( 6, 0, 6, 1, &_but31 );
		_layoutGrid31.SetCell( 7, 0, 7, 1, &_but32 );
		_layoutGrid31.SetCell( 8, 0, 8, 1, &_but33 );
		_layoutGrid31.SetCell( 9, 0, 9, 1, &_but34 );
		_layoutGrid31.SetCell( 10, 0, 10, 1, &_but35 );
		_layoutGrid31.SetRowHeightInterval( 0, 35, 25 );
		for( int r=4; r<=10; ++r )
			_layoutGrid31.SetRowHeightInterval( r, 40, 10 );
		//
		_layoutGrid31.CalcGridPos();
	}

	{
		_canvas4.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );

		_lable41.Create( 0, &_canvas4 );
		_lable41.SetString( "当前衰减值:" );
		_edit41.Create( 0, &_canvas4 );
		_edit41.SetString( "0" );

		_lable42.Create( 0, &_canvas4 );
		_lable42.SetString( "Step:" );
		_lable43.Create( 0, &_canvas4 );
		_lable43.SetString( "扫描范围:   -" );
		_lable44.Create( 0, &_canvas4 );
		_lable44.SetString( "~   +" );

		_edit42.Create( 0, &_canvas4 );
		_edit42.SetString( "50" );
		_edit43.Create( 0, &_canvas4 );
		_edit43.SetString( "0" );
		_edit44.Create( 0, &_canvas4 );
		_edit44.SetString( "300" );

		_but41.Create( 45, &_canvas4 );
		_but41.SetString( "自动加电压" );

		_layoutGrid41.InitPoint( GenPoint()( 30, 50 ) );
		_layoutGrid41.SetRowColumn( 8, 8 );
		//
		_layoutGrid41.SetCell( 0, 0, &_lable41 );
		_layoutGrid41.SetCell( 0, 1, &_edit41 );
		_layoutGrid41.SetCell( 2, 0, &_lable43 );
		_layoutGrid41.SetCell( 2, 1, &_edit43 );
		_layoutGrid41.SetCell( 2, 2, &_lable44 );
		_layoutGrid41.SetCell( 2, 3, &_edit44 );
		_layoutGrid41.SetCell( 3, 0, &_lable42 );
		_layoutGrid41.SetCell( 3, 1, &_edit42 );
		_layoutGrid41.SetCell( 4, 0, 4, 1, &_but41 );
		//
		//_layoutGrid41.SetColumnWidthInterval( 0, 90, 10 );
		_layoutGrid41.SetColumnWidthInterval( 2, 40, 10 );
		//
		_layoutGrid41.CalcGridPos();
	}

	{
		_canvas1.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );

		_baseConfig.CreateDirect( &_canvas1, GenRect()( 500, 200, 900, 900 ) );	

		_but11.Create( 19, &_canvas1, GenRect()( 500, 80, 600, 120 ) );
		_but11.SetString( "存光" );
		_edit13.Create( 0, &_canvas1, GenRect()( 620, 80, 700, 120 ) );
		_edit13.SetString( "-1" );

		_lable11.Create( 0, &_canvas1 );
		_lable11.SetString( "光谱仪IP:" );

		_edit11.Create( 0, &_canvas1 );
		_edit11.SetString( "192.16.22.211" );

		_lable12.Create( 0, &_canvas1 );
		_lable12.SetString( "光谱仪PORT:" );

		_edit12.Create( 0, &_canvas1 );
		_edit12.SetString( "10001" );

		_lable13.Create( 0, &_canvas1 );
		_lable13.SetString( "光开关COM:" );
		_combox11.Create( 0, &_canvas1 );
		for( int i=1; i<=10; ++i )
		{
			STRING_FORMAT( str, "COM%d", i );
			_combox11.AddString( str );
		}
		_combox11.SetSelectIndex( 0 );

		_but12.Create( 11, &_canvas1 );
		_but12.SetString( "连接设备" );

		_but13.Create( 12, &_canvas1 );
		_but13.SetString( "断开设备" );

		_lable14.Create( 0, &_canvas1 );
		_lable14.SetString( "设备未连接.." );
		_lable14.SetColor( _BLUE_ );

		_lable15.Create( 0, &_canvas1 );
		_lable15.SetString( "设备未连接.." );
		_lable15.SetColor( _BLUE_ );

		_layoutGrid11.InitPoint( GenPoint()( 50, 80 ) );
		_layoutGrid11.SetRowColumn( 8, 2 );
		//
		_layoutGrid11.SetCell( 0, 0, &_lable11 );
		_layoutGrid11.SetCell( 0, 1, &_edit11 );
		_layoutGrid11.SetCell( 1, 0, &_lable12 );
		_layoutGrid11.SetCell( 1, 1, &_edit12 );
		_layoutGrid11.SetCell( 2, 1, &_lable14 );
		_layoutGrid11.SetCell( 3, 0, &_lable13 );
		_layoutGrid11.SetCell( 3, 1, &_combox11 );
		_layoutGrid11.SetCell( 4, 1, &_lable15 );
		_layoutGrid11.SetCell( 6, 0, 7, 0, &_but12 );
		_layoutGrid11.SetCell( 6, 1, 7, 1, &_but13 );
		_layoutGrid11.SetColumnWidthInterval( 0, 145 );
		_layoutGrid11.SetColumnWidthInterval( 1, 145 );
		//
		_layoutGrid11.CalcGridPos();
	}

	{
		_canvas2.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );

		_chart21.Create( 20, &_canvas2, GenRect()( 20, 120, 650, 980 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_chart21.SetY1CoordLimit( -30, 100 );
		_chart21.SetY2CoordSymmetry();

		_grid21.Create( 20, &_canvas2, GenRect()( 670, 120, 980, 980 ), UILayoutCalc::MOVE_X|UILayoutCalc::SIZE_Y );
		_grid21.SetRowColumn( 1, 7 );
		_grid21.SetRowFix();
		_grid21.SetCellString( 0, 0, "ITU" );
		_grid21.SetCellString( 0, 1, "Cycle" );
		_grid21.SetCellString( 0, 2, "BW" );
		_grid21.SetCellString( 0, 3, "Ripple" );
		_grid21.SetCellString( 0, 4, "IL" );
		_grid21.SetCellString( 0, 5, "CFreq" );
		_grid21.SetCellString( 0, 6, "DX" );
		_grid21.SetColumnWidth( 5, 80 );

		_button21.Create( 21, &_canvas2 );
		_button21.SetString( "开始监视" );

		_button22.Create( 22, &_canvas2 );
		_button22.SetString( "停止监视" );

		_button23.Create( 23, &_canvas2 );
		_button23.SetString( "0.5db" );
		_button23.SetCheck( true );
		_button24.Create( 24, &_canvas2 );
		_button24.SetString( "3db" );
		UISetCheckButtonMutex( &_button23, &_button24 );

		_lable21.Create( 0, &_canvas2 );
		_lable21.SetString( "ITU偏移:" );
		_combox21.Create( 29, &_canvas2 );
		for( int i=0; i<=20; ++i )
		{
			STRING_FORMAT( str, "%d", i );
			_combox21.AddString( str );
		}
		_combox21.SetSelectIndex( 0 );

		_button25.Create( 25, &_canvas2 );
		_button25.SetString( "ripple范围" );
		_edit21.Create( 0, &_canvas2 );
		_edit21.SetString( "10" );

		_button26.Create( 1000, &_canvas2 );
		_button26.SetString( "osw通道" );
		_edit22.Create( 0, &_canvas2 );
		_edit22.SetString( "1" );

		_button27.Create( 27, &_canvas2 );
		_button27.SetString( "最低峰值" );
		_edit23.Create( 0, &_canvas2 );
		_edit23.SetString( "-15" );

		_grid22.Create( 0, &_canvas2 );
		_grid22.SetRowColumn( 3, 4 );
		_grid22.SetRowFix();
		_grid22.SetColumnFix();
		_grid22.SetCellString( 0, 0, "轴" );
		_grid22.SetCellString( 0, 1, "min" );
		_grid22.SetCellString( 0, 2, "max" );
		_grid22.SetCellString( 1, 0, "y1" );
		_grid22.SetCellString( 2, 0, "x" );
		_grid22.SetCellButton( 28, 1, 3, "设置" );
		_grid22.SetCellButton( 29, 2, 3, "设置" );
		_grid22.SetColumnWidth( 1, 80 );
		_grid22.SetColumnWidth( 2, 80 );
		_grid22.SetRowHeight( 1, 30 );
		_grid22.SetRowHeight( 2, 30 );
		_grid22.SetCellString( 1, 1, "-30" );
		_grid22.SetCellString( 1, 2, "100" );

		_layoutGrid21.InitPoint( GenPoint()( 20, 20 ) );
		_layoutGrid21.SetRowColumn( 2, 8 );
		//
		_layoutGrid21.SetCell( 0, 0, &_button21 );
		_layoutGrid21.SetCell( 1, 0, &_button22 );
		_layoutGrid21.SetCell( 0, 1, &_button23 );
		_layoutGrid21.SetCell( 1, 1, &_button24 );
		_layoutGrid21.SetCell( 0, 2, &_lable21 );
		_layoutGrid21.SetCell( 1, 2, &_button27 );
		_layoutGrid21.SetCell( 0, 3, &_combox21 );
		_layoutGrid21.SetCell( 1, 3, &_edit23 );
		_layoutGrid21.SetCell( 0, 4, &_button26 );
		_layoutGrid21.SetCell( 1, 4, &_button25 );
		_layoutGrid21.SetCell( 0, 5, &_edit22 );
		_layoutGrid21.SetCell( 1, 5, &_edit21 );
		_layoutGrid21.SetCell( 0, 6, 1, 6, &_grid22 );
		_layoutGrid21.SetRowHeightInterval( 0, 40, 10 );
		_layoutGrid21.SetRowHeightInterval( 1, 40, 10 );
		_layoutGrid21.SetColumnWidthInterval( 1, 70 );
		_layoutGrid21.SetColumnWidthInterval( 3, 60 );
		_layoutGrid21.SetColumnWidthInterval( 5, 60 );
		_layoutGrid21.SetColumnWidthInterval( 6, 300 );
		//
		_layoutGrid21.CalcGridPos();
	}
}

void SetGridChannels( UIGrid& grid, NM_GRID* pNMGrid )
{
	if( pNMGrid->_code==3 )
	{
		if( pNMGrid->_column!=0 ) return;

		int rowNum;
		int columnNum;
		grid.GetRowColumn( rowNum, columnNum );

		bool flag;
		grid.GetCellCheckState( 1, 0, flag );

		for( int r=1; r<rowNum; ++r )
			grid.SetCellCheckState( r, 0, !flag );
	}
	else if( pNMGrid->_code==1 )
	{
		string str;
		int row = pNMGrid->_row;

		STRING_FORMAT( str, "%d", row );
		grid.GetCellString( row, 1, str );
		if( str=="" )
		{
			MessageBoxA( NULL, "请填写电压！", "waring", 0 );
			return;
		}

		int vol = atoi( str.c_str() );
		int xyFlag = grid._id==1?0:1;
		bool rt = SetDACVoltageSyn_Com( row, xyFlag, vol );
		
		grid.SetCellString( row, 2, rt?"设置电压成功..":"设置电压失败.." );
	}
	UIRefresh();
}

void UIT1::OnMainBar( int param )
{
	_tabX1.SetCurCell( param-1 );
}

void UIT1::OnSetGrid1Channels( int param )
{
	SetGridChannels( _grid31, (NM_GRID*)param );
}

void UIT1::OnSetGrid2Channels( int param )
{
	SetGridChannels( _grid32, (NM_GRID*)param );
}

void UIT1::OnSetAllChannels( int param )
{
	VECTOR_INT xList;
	VECTOR_INT yList;

	VECTOR_INT volXList;
	VECTOR_INT volYList;

	bool fX, fY;
	_checkBut31.GetCheck( fX );
	_checkBut32.GetCheck( fY );

	bool fVol;
	_checkBut33.GetCheck( fVol );
	string str;
	if( fVol )
		_edit31.GetString( str );
	else
		_edit33.GetString( str );
	int vol = atoi( str.c_str() );

	// x轴
	if( fX )
	{
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			// 通道
			xList.push_back( c );

			// 电压
			if( fVol )
			{
				volXList.push_back( vol );
			}
			else
			{
				_grid31.GetCellString( c, 1, str );
				volXList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}
	
	// y轴
	if( fY )
	{
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			// 通道
			yList.push_back( c );

			// 电压
			if( fVol )
			{
				volYList.push_back( vol );
			}
			else
			{
				_grid32.GetCellString( c, 1, str );
				volYList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}

	StartSetChannelsVoltageThread( xList, yList, volXList, volYList );
}

void UIT1::OnSetOddChannels( int param )
{
	VECTOR_INT xList;
	VECTOR_INT yList;

	VECTOR_INT volXList;
	VECTOR_INT volYList;

	bool fX, fY;
	_checkBut31.GetCheck( fX );
	_checkBut32.GetCheck( fY );

	bool fVol;
	_checkBut33.GetCheck( fVol );
	string str;
	if( fVol )
		_edit31.GetString( str );
	else
		_edit33.GetString( str );
	int vol = atoi( str.c_str() );

	// x轴
	if( fX )
	{
		for( int c=1; c<=CHANNEL_NUM; c+=2 )
		{
			// 通道
			xList.push_back( c );

			// 电压
			if( fVol )
			{
				volXList.push_back( vol );
			}
			else
			{
				_grid31.GetCellString( c, 1, str );
				volXList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}
	
	// y轴
	if( fY )
	{
		for( int c=1; c<=CHANNEL_NUM; c+=2 )
		{
			// 通道
			yList.push_back( c );

			// 电压
			if( fVol )
			{
				volYList.push_back( vol );
			}
			else
			{
				_grid32.GetCellString( c, 1, str );
				volYList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}

	StartSetChannelsVoltageThread( xList, yList, volXList, volYList );
}

void UIT1::OnSetEvenChannels( int param )
{
	VECTOR_INT xList;
	VECTOR_INT yList;

	VECTOR_INT volXList;
	VECTOR_INT volYList;

	bool fX, fY;
	_checkBut31.GetCheck( fX );
	_checkBut32.GetCheck( fY );

	bool fVol;
	_checkBut33.GetCheck( fVol );
	string str;
	if( fVol )
		_edit31.GetString( str );
	else
		_edit33.GetString( str );
	int vol = atoi( str.c_str() );

	// x轴
	if( fX )
	{
		for( int c=2; c<=CHANNEL_NUM; c+=2 )
		{
			// 通道
			xList.push_back( c );

			// 电压
			if( fVol )
			{
				volXList.push_back( vol );
			}
			else
			{
				_grid31.GetCellString( c, 1, str );
				volXList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}
	
	// y轴
	if( fY )
	{
		for( int c=2; c<=CHANNEL_NUM; c+=2 )
		{
			// 通道
			yList.push_back( c );

			// 电压
			if( fVol )
			{
				volYList.push_back( vol );
			}
			else
			{
				_grid32.GetCellString( c, 1, str );
				volYList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}

	StartSetChannelsVoltageThread( xList, yList, volXList, volYList );
}

void UIT1::OnSetSelectChannels( int param )
{
	VECTOR_INT xList;
	VECTOR_INT yList;

	VECTOR_INT volXList;
	VECTOR_INT volYList;

	bool fX, fY;
	_checkBut31.GetCheck( fX );
	_checkBut32.GetCheck( fY );

	bool fVol;
	_checkBut33.GetCheck( fVol );
	string str;
	if( fVol )
		_edit31.GetString( str );
	else
		_edit33.GetString( str );
	int vol = atoi( str.c_str() );

	// x轴
	if( fX )
	{
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			bool f;
			_grid32.GetCellCheckState( c, 0, f );
			if( !f )
				continue;

			// 通道
			xList.push_back( c );

			// 电压
			if( fVol )
			{
				volXList.push_back( vol );
			}
			else
			{
				_grid31.GetCellString( c, 1, str );
				volXList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}
	
	// y轴
	if( fY )
	{
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			bool f;
			_grid32.GetCellCheckState( c, 0, f );
			if( !f )
				continue;

			// 通道
			yList.push_back( c );

			// 电压
			if( fVol )
			{
				volYList.push_back( vol );
			}
			else
			{
				_grid32.GetCellString( c, 1, str );
				volYList.push_back( atoi( str.c_str() )+vol );
			}
		}
	}

	StartSetChannelsVoltageThread( xList, yList, volXList, volYList );
}

void UIT1::OnClearResults( int param )
{
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		_grid31.SetCellString( c, 2, "" );
		_grid32.SetCellString( c, 2, "" );
	}
}

void UIT1::OnLoadRoughVol( int param )
{
	StartLoadVoltageFileThread( 0 );
}

void UIT1::OnLoadExactVol( int param )
{
	StartLoadVoltageFileThread( 1 );
}

void UIT1::OnSaveVol( int param )
{
	StartSaveVoltageFileThread();
}

void UIT1::OnChooseFilePath()
{
	string fileName;
	if( SaveFolderFile( fileName ) )
	{
		_edit32.SetString( fileName );
	}
}

void UIT1::OnStartYellowLightProcess()
{
	//
	OnSetYLOffset();
	OnSetYLFlag();
	OnSetRippleDX();
	OnSetOSWChannel1();
	OnSetLowPeakLimit();

	// 开始黄线过程
	WSST1::GetSingleton()->StartYellowLightProcess();
}

void UIT1::OnSetYLOffset()
{
	string str;
	_combox21.GetString( str );
	WSST1::GetSingleton()->_ylOffset = atoi( str.c_str() );
}

void UIT1::OnSetYLFlag()
{
	bool flag;
	_button23.GetCheck( flag );
	WSST1::GetSingleton()->_ylDBFlag = flag?1:2;
}

void UIT1::OnSetRippleDX()
{
	string str;
	_edit21.GetString( str );
	WSST1::GetSingleton()->_rippleDX = (float)atof( str.c_str() );
}

void UIT1::OnSetOSWChannel1()
{
	string str;
	_edit22.GetString( str );
	
	int c = atoi( str.c_str() );

	WSST1::GetSingleton()->_osw.SetChannel( c );
	WSST1::GetSingleton()->_osw.GetChannel( c );
	WSST1::GetSingleton()->_oswChannel = c;

	STRING_FORMAT( str, "%d", c );

	_edit22.SetString( str );
	_edit34.SetString( str );
}

void UIT1::OnSetOSWChannel2()
{
	string str;
	_edit34.GetString( str );

	int c = atoi( str.c_str() );

	WSST1::GetSingleton()->_osw.SetChannel( c );
	WSST1::GetSingleton()->_osw.GetChannel( c );
	WSST1::GetSingleton()->_oswChannel = c;

	STRING_FORMAT( str, "%d", c );

	_edit22.SetString( str );
	_edit34.SetString( str );
}

void UIT1::OnSetLowPeakLimit()
{
	string str;
	_edit23.GetString( str );
	WSST1::GetSingleton()->_lowPeakLimit = (float)atof( str.c_str() );
}

void UIT1::OnStartSaveLightProcess()
{
	GetDeviceAddr();

	// 开始存光过程
	string str;
	_edit13.GetString( str );
	WSST1::GetSingleton()->StartSaveLightProcess( atoi( str.c_str() ) );
}

void UIT1::OnSetY1Range()
{
	string str1, str2;
	_grid22.GetCellString( 1, 1, str1 );
	_grid22.GetCellString( 1, 2, str2 );

	_chart21.SetY1CoordLimit( atof( str1.c_str() ), atof( str2.c_str() ) );
}

void UIT1::OnSetXRange()
{
	string str1, str2;
	_grid22.GetCellString( 2, 1, str1 );
	_grid22.GetCellString( 2, 2, str2 );

	_chart21.SetXCoordLimit( atof( str1.c_str() ), atof( str2.c_str() ) );
}

void UIT1::OnConnectDevice()
{
	GetDeviceAddr();

	VECTOR_STRING msgList;
	WSST1::GetSingleton()->ConnectDevices( msgList );
	_lable14.SetString( msgList[0] );
	_lable15.SetString( msgList[1] );
}

void UIT1::OnDisconnectDevice()
{
	WSST1::GetSingleton()->DisconnectDevices();
	_lable14.SetString( "光谱仪断开连接.." );
	_lable15.SetString( "光开关断开连接.." );
}

void UIT1::OnAutoVol()
{
	StartAutoVolThread();
}

void UIT1::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
		OnMainBar( param );
	else if( id==31 )
		OnSetGrid1Channels( param );
	else if( id==32 )
		OnSetGrid2Channels( param );
	else if( id==33 )
		OnSetAllChannels( param );
	else if( id==34 )
		OnSetOddChannels( param );
	else if( id==35 )
		OnSetEvenChannels( param );
	else if( id==36 )
		OnSetSelectChannels( param );
	else if( id==37 )
		OnClearResults( param );
	else if( id==38 )
		OnLoadRoughVol( param );
	else if( id==39 )
		OnSaveVol( param );
	else if( id==41 )
		OnChooseFilePath();
	else if( id==42 )
		OnLoadExactVol( param );
	else if( id==45 )
		OnAutoVol();
	else if( id==21 )
		OnStartYellowLightProcess();
	else if( id==22 )
		WSST1::GetSingleton()->StopYellowLightProcess();
	else if( id==29 )
		OnSetYLOffset();
	else if( id==23 || id==24 )
		OnSetYLFlag();
	else if( id==25 )
		OnSetRippleDX();
	else if( id==27 )
		OnSetLowPeakLimit();
	else if( id==28 )
		OnSetY1Range();
	else if( id==29 )
		OnSetXRange();
	else if( id==19 )
		OnStartSaveLightProcess();
	else if( id==11 )
		OnConnectDevice();
	else if( id==12 )
		OnDisconnectDevice();
	else if( id==1000 )
		OnSetOSWChannel1();
	else if( id==1001 )
		OnSetOSWChannel2();
}

void UIT1::UIInitPortFreq()
{
	string str;

	_grid31.SetRowColumn( CHANNEL_NUM+1, 3 );
	_grid31.SetRowFix();
	_grid31.SetCellString( 0, 0, "通道X轴" );
	_grid31.SetCellString( 0, 1, "电压" );
	_grid31.SetCellString( 0, 2, "结果" );
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		STRING_FORMAT( str, "%d", c );
		_grid31.SetCellCheckButton( 0, c, 0, str );
		_grid31.SetCellCheckState( c, 0, true );
	}
	_grid31.SetColumnWidth( 1, 80 );
	_grid31.SetColumnWidth( 2, 120 );

	_grid32.SetRowColumn( CHANNEL_NUM+1, 3 );
	_grid32.SetRowFix();
	_grid32.SetCellString( 0, 0, "通道Y轴" );
	_grid32.SetCellString( 0, 1, "电压" );
	_grid32.SetCellString( 0, 2, "结果" );
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		STRING_FORMAT( str, "%d", c );
		_grid32.SetCellCheckButton( 0, c, 0, str );
		_grid32.SetCellCheckState( c, 0, true );
	}
	_grid32.SetColumnWidth( 1, 80 );
	_grid32.SetColumnWidth( 2, 120 );

	_baseConfig.UIInitPortFreq();
}

void UIT1::ShowYLCurve( VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, VECTOR_DOUBLE& xYList, VECTOR_DOUBLE& yYList )
{
	_chart21.Clear();

	_chart21.AddCurve1( "trace", xList, yList );
	_chart21.AddCurve2( "yellowLine", xYList, yYList );
	_chart21.SetCurve2Color( "yellowLine", _RED_ );
	_chart21.SetCurve2Select( "yellowLine" );
	_chart21.CalcXYCoordRange();
}

void UIT1::ShowYLGrid()
{
	//double pos = _grid21._yScroll.GetPosScale();
	_grid21.ClearUnfixRows();

	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	TxtOBJ txt;
	if( !txt.OpenFile( str+"\\cacheIndicator.xls" ) ) 
		return;

	vector<VECTOR_STRING > allLineWordsList;
	txt.ReadAllLineWords( allLineWordsList );

	vector<VECTOR_STRING > rows;
	bool flag;
	_button23.GetCheck( flag );
	int startIndex = flag ? 2:7;
	for( UINT i=1; i<allLineWordsList.size(); ++i )
	{
		VECTOR_STRING& line = allLineWordsList[i];
		VECTOR_STRING row;

		if( line.size()==13 )
		{
			row.push_back( line[0] );
			row.push_back( line[1] );

			for( int j=0; j<5; ++j )
				row.push_back( line[startIndex+j] );
		}
		else if( line.size()==2 )
		{
			row.push_back( line[0] );
			row.push_back( line[1] );

			for( int j=0; j<5; ++j )
				row.push_back( "N" );
		}
		else
		{
			break;
		}

		rows.push_back( row );
	}

	_grid21.AddRows( rows );
	//_grid21._yScroll.SetPosScale( pos );

	// 自动计算x轴范围
	if( allLineWordsList.size()>30 )
	{
		int min = atoi( allLineWordsList[4][0].c_str() )-100;
		int max = atoi( allLineWordsList.back().at(0).c_str() )+100;
		string str1, str2;
		STRING_FORMAT( str1, "%d", min );
		STRING_FORMAT( str2, "%d", max );
		_grid22.SetCellString( 2, 1, str1 );
		_grid22.SetCellString( 2, 2, str2 );
	}
}

void UIT1::ShowSetVolResult( int xyFlag, int c, int vol, bool rt )
{
	UIGrid& grid = xyFlag==0?_grid31:_grid32;

	string str;
	STRING_FORMAT( str, "%d", vol );
	grid.SetCellString( c, 1, str );

	grid.SetCellString( c, 2, rt?"设置电压成功..":"设置电压失败.." );
	UIRefresh();
}

void UIT1::ShowAllChannelsVol( int xVol[104], int yVol[104] )
{
	string str;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		STRING_FORMAT( str, "%d", xVol[c-1] );
		_grid31.SetCellString( c, 1, str );

		STRING_FORMAT( str, "%d", yVol[c-1] );
		_grid32.SetCellString( c, 1, str );
	}
}

void UIT1::GetAllChannelsVol( int xVol[104], int yVol[104] )
{
	string str;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		_grid31.GetCellString( c, 1, str );
		xVol[c-1] = atoi( str.c_str() );

		_grid32.GetCellString( c, 1, str );
		yVol[c-1] = atoi( str.c_str() );
	}
}

void UIT1::GetDeviceAddr()
{
	// 设置OSA地址
	string str;
	_edit11.GetString( str );
	WSST1::GetSingleton()->_osaIP = IP4_StrToULONG()( str.c_str() );

	_edit12.GetString( str );
	WSST1::GetSingleton()->_osaPort = atoi( str.c_str() );

	// 获得osw串口
	WSST1::GetSingleton()->_oswCom = _combox11.GetSelectIndex()+1;
}

bool UIT1::StartSetChannelsVoltageThread( VECTOR_INT xList, VECTOR_INT yList, VECTOR_INT volXList, VECTOR_INT volYList )
{
	if( IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
		return false;
	}

	_xList = xList;
	_yList = yList;
	_volXList = volXList;
	_volYList = volYList;
	return StartThread( "SetChannelsVoltageThread", &UIT1::SetChannelsVoltageThread );
}

void UIT1::SetChannelsVoltageThread()
{
	for( UINT i=0; i<_xList.size(); ++i )
		SetChanneVoltage( _xList[i], 0, _volXList[i] );

	for( UINT i=0; i<_yList.size(); ++i )
		SetChanneVoltage( _yList[i], 1, _volYList[i] );
}

void UIT1::SetChanneVoltage( int c, int xyFlag, int vol )
{
	bool rt = false;

	ShowSetVolResult( xyFlag, c, vol, SetDACVoltageSyn_Com( c, xyFlag, vol ) );
}

bool UIT1::StartLoadVoltageFileThread( int flag )
{
	if( IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
		return false;
	}

	return StartThread( "LoadVolThread", flag? &UIT1::LoadRoughVolThread : &UIT1::LoadExactVolThread );
}

void UIT1::LoadRoughVolThread()
{
	// 获得要载入的port
	string str;
	_edit34.GetString( str );
	int port = atoi( str.c_str() );
	
	// 
	_edit32.GetString( str );
	
	LoadVol( port, str, "粗略电压" );

#if 0
	// 选取文件
	string fileName;
	if( !OpenFolderFile( fileName ) )
		return;

	// 打开文件
	TxtOBJ txt;
	if( !txt.OpenFile( fileName ) )
	{
		::MessageBoxA( NULL, "配置文件不存在！", "告警", 0 );
		return;
	}
	//
	vector<vector<string> > allLineWordsList;
	txt.ReadAllLineWords( allLineWordsList );
	if( allLineWordsList.size()!=CHANNEL_NUM+1 )
	{
		::MessageBoxA( NULL, "配置文件内行数错误！", "告警", 0 );
		return;
	}
	
	// 获取数据
	int xVol[104];
	int yVol[104];
	//
	for( int i=1; i<104+1; ++i )
	{
		vector<string>& row = allLineWordsList[i];
		assert( row.size()==3 );
	
		xVol[i-1] = atoi( row[1].c_str() );
		yVol[i-1] = atoi( row[2].c_str() );
	}
	// 显示到view
	ShowAllChannelsVol( xVol, yVol );

	View0( "设置所有通道电压.." );
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{	
		SetChanneVoltage( c, 0, xVol[c-1] );
		SetChanneVoltage( c, 1, yVol[c-1] );
	}
#endif
}

void UIT1::LoadExactVolThread()
{
	// 获得要载入的port
	string str;
	_edit34.GetString( str );
	int port = atoi( str.c_str() );
	
	// 
	_edit32.GetString( str );
	
	LoadVol( port, str, "精确电压" );
}

void UIT1::LoadVol( int port, string file, string sheet )
{
	VECTOR_INT volXList;
	VECTOR_INT volYList;

	if( XLSOpenFile( file.c_str() ) )
	{
		if( XLSOpenSheet( sheet.c_str() ) )
		{
			vector<VECTOR_STRING> allLineWordsList;
			XLSGetAllCellsString( &allLineWordsList );

			if( allLineWordsList.size()>2 )
			{
				for( UINT i=2; i<allLineWordsList.size(); ++i )
				{
					VECTOR_STRING& line = allLineWordsList[i];

					volXList.push_back( atoi( line[( port-1 )*2].c_str() ) );
					volYList.push_back( atoi( line[( port-1 )*2+1].c_str() ) );
				}

				// 显示到view
				ShowAllChannelsVol( &volXList[0], &volYList[0] );

				View0( "设置所有通道电压.." );
				for( int c=1; c<=CHANNEL_NUM; ++c )
				{	
					SetChanneVoltage( c, 0, volXList[c-1] );
					SetChanneVoltage( c, 1, volYList[c-1] );
				}
			}
		}
	}

	XLSClose();
}


bool UIT1::StartSaveVoltageFileThread()
{
	return StartThread( "SaveVolThread", &UIT1::SaveVolThread );
}

void UIT1::SaveVolThread()
{
	string str;
	_edit34.GetString( str );
	int port = atoi( str.c_str() );

	string dstStr;
	_edit32.GetString( dstStr );

	if( !XLSOpenFile( dstStr.c_str() ) )
	{
		string srcStr;
		GetExeDirectory( srcStr );
		GetUpDirectory( srcStr );
		srcStr += "\\Data\\程序1电压模板.xls";

		CopyFileX( srcStr, dstStr );

		XLSOpenFile( dstStr.c_str() );
	}

	if( XLSOpenSheet( "粗略电压" ) )
	{	// 从grid获得值
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			string str1, str2;
			_grid31.GetCellString( c, 1, str1 );
			_grid32.GetCellString( c, 1, str2 );

			XLSSetCellString( 2+c, port*2-1, str1.c_str() );
			XLSSetCellString( 2+c, port*2, str2.c_str() );
		}
	}

	XLSClose();

	View0( "电压保存成功.." );

#if 0
	string fileName;
	if( !SaveFolderFile( fileName ) )
		return;

	TxtOBJ txt;
	txt.CreateFile( fileName );
	txt.WriteToEnd( "channel	xVol	yVol\r\n" );

	int xVol[104];
	int yVol[104];
	GetAllChannelsVol( xVol, yVol );

	string strLine;
	for( int i=0; i<CHANNEL_NUM; ++i )
	{
		STRING_FORMAT( strLine, "%d	%d	%d\r\n", i+1, xVol[i], yVol[i] );
		txt.WriteToEnd( strLine );
	}

	View0( "电压文件保存完毕.." );
#endif
}

bool UIT1::StartAutoVolThread()
{
	return StartThread( "AutoVolThread", &UIT1::AutoVolThread );
}

void UIT1::AutoVolThread()
{
	string str;

	// 通道对应的波长


	// 对应的port
	WSST1::GetSingleton()->LoadSaveLight( WSST1::GetSingleton()->_oswChannel );

	// 获得当前对应的衰减值
	_edit41.GetString( str );
	int curAtten = atoi( str.c_str() );

	// 扫描范围 步进
	_edit43.GetString( str );
	int start = -atoi( str.c_str() );
	_edit44.GetString( str );
	int end = atoi( str.c_str() );

	_edit42.GetString( str );
	int step = atoi( str.c_str() );

	// 只对应x轴
	// 各通道当前电压值
	VECTOR_INT xVolList;
	UIGrid& grid = _grid31;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		grid.GetCellString( c, 1, str );
		xVolList.push_back( atoi( str.c_str() ) );
	}

	// 循环加电压
	VECTOR_FLOAT tempList[MAX_CHANNEL_NUM];
	int maxCount = ( end-start )/step;
	int count = 0;
	int dxVol = start;
	while( dxVol<=end )
	{
		STRING_FORMAT( str, "第%d/%d次加电压..", ++count, maxCount );
		View0( str.c_str() );

		for( int c=1; c<=CHANNEL_NUM; ++c )
			SetChanneVoltage( c, 0, xVolList[c-1]+start+( count-1 )*step );

		// osa获得值
		VECTOR_FLOAT xNMList, vList;
		if( WSST1::GetSingleton()->GetOSATrace( xNMList, vList )==false )
			return;

		// 分析每个通道的值

	}

	// 计算最佳的电压值
	VECTOR_INT volList;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		int index = max_element( tempList[c-1].begin(), tempList[c-1].end() )-tempList[c-1].begin();
		volList.push_back( xVolList[c-1]+start+index*step );
	}

	// 最佳电压显示到grid上


	// 保存
}