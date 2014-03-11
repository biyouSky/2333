#include "stdafx.h"
#include "UIT3.h"
using namespace std;

UIT3 gUIT3;

void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void View101( int port, int row, int column )
{
	gUIT3.SetIndicatorGrid( port, row, column );
}

void View102( int port, int row, int column, const char* v, COLORREF color )
{
	gUIT3.SetIndicatorGridCell( port, row, column, v, color );
}


void UIT3::OnCreate()
{
	//
	PCB_View_ShowMsg = View0;
	PCB_View_SetIndicatorGrid = View101;
	PCB_View_SetIndicatorGridCell = View102;

	string str;

	//
	_tabX1.Create( 0, &_uiContainer, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 3 );
	_tabX1.SetCell( 0, "采集", &_canvas2 );
	_tabX1.SetCell( 1, "数据", &_canvas1 );
	_tabX1.SetCell( 2, "指标", &_tabY1 );

	{
		_canvas1.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
		
		_grid1.Create( 0, &_canvas1, GenRect()( 20, 20, 700, 900 ), UILayoutCalc::SIZE_Y );
		_grid1.SetRowColumn( 1, 2 );
		_grid1.SetRowFix();
		_grid1.SetColumnFix();
		_grid1.SetCellString( 0, 0, "序号" );
		_grid1.SetCellString( 0, 1, "文件名" );
		_grid1.SetColumnWidth( 1, 600 );
		//
		_but1.Create( 1, &_canvas1, GenRect()( 120, 930, 220, 980 ), UILayoutCalc::MOVE_Y );
		_but1.SetString( "载入文件" );
		//
		_but2.Create( 2, &_canvas1, GenRect()( 320, 930, 420, 980 ), UILayoutCalc::MOVE_Y );
		_but2.SetString( "分析指标" );

		//
		_grid2.Create( 3, &_canvas1, GenRect()( 720, 20, 900, 900 ), UILayoutCalc::SIZE_Y );
		//SetIndicatorITU();
	}

	{
		_tabY1.Create( 0, &_tabX1 );
		_tabY1.SetY();

		for( int i=0; i<9; ++i )
		{
			_indicatorShowList[i].CreateDirect( &_tabY1, GenRect()( 0, 0, 1000, 1000 ) );
			_indicatorShowList[i].ShowWindowDirect( false );
		}
	}
	{
		_canvas2.CreateDirect( &_tabX1 );

		_lable21.Create( 0, &_canvas2 );
		_lable21.SetString( "光谱仪IP:" );
		_edit21.Create( 0, &_canvas2 );
		_edit21.SetString( "192.16.22.195" );
		_lable22.Create( 0, &_canvas2 );
		_lable22.SetString( "光谱仪PORT:" );
		_edit22.Create( 0, &_canvas2 );
		_edit22.SetString( "10001" );

		/*_lable23.Create( 0, &_canvas2 );
		_lable23.SetString( "测试模块个数:" );
		_combo21.Create( 0, &_canvas2 );
		_combo21.AddString( "1个" );
		_combo21.AddString( "2个" );
		_combo21.AddString( "3个" );
		_combo21.AddString( "4个" );
		_combo21.AddString( "5个" );
		_combo21.AddString( "6个" );
		_combo21.SetSelectIndex( 0 );*/

		_lable24.Create( 0, &_canvas2 );
		_lable24.SetString( "模块通信方式:" );
		_combo22.Create( 0, &_canvas2 );
		_combo22.AddString( "COM" );
		_combo22.AddString( "UDP" );
		_combo22.SetSelectIndex( 0 );

		_grid21.Create( 0, &_canvas2 );
		_grid21.SetRowColumn( 7, 5 );
		_grid21.SetRowFix();
		_grid21.SetCellString( 0, 0, "序号" );
		_grid21.SetCellString( 0, 1, "IP/COM" );
		_grid21.SetCellString( 0, 2, "通道数" );
		_grid21.SetCellString( 0, 3, "序列号" );
		_grid21.SetCellString( 0, 4, "温度" );
		_grid21.SetColumnWidth( 1, 110 );
		_grid21.SetColumnWidth( 3, 80 );
		for( int r=1; r<7; ++r )
		{
			_grid21.SetRowHeight( r, 25 );
			STRING_FORMAT( str, "%d", r );
			_grid21.SetCellCheckButton( 0, r, 0, str );
			_grid21.SetCellCheckState( r, 0, true );
		}

		_grid22.Create( 0, &_canvas2 );
		_grid22.SetRowColumn( 7, 4 );
		_grid22.SetRowFix();
		_grid22.SetColumnFix();
		_grid22.SetCellString( 0, 0, "频率" );
		_grid22.SetCellString( 0, 1, "通道数" );
		_grid22.SetCellString( 0, 2, "起始频率" );
		_grid22.SetCellString( 0, 3, "终止频率" );
		_grid22.SetCellString( 1, 0, "50G" );
		_grid22.SetCellString( 1, 1, "80ch" );
		_grid22.SetCellString( 1, 2, "192100" );
		_grid22.SetCellString( 1, 3, "196050" );
		_grid22.SetCellString( 2, 0, "50G" );
		_grid22.SetCellString( 2, 1, "96ch" );
		_grid22.SetCellString( 2, 2, "191300" );
		_grid22.SetCellString( 2, 3, "196050" );
		_grid22.SetCellString( 3, 0, "50G" );
		_grid22.SetCellString( 3, 1, "99ch" );
		_grid22.SetCellString( 3, 2, "191200" );
		_grid22.SetCellString( 3, 3, "196100" );			
		_grid22.SetCellString( 4, 0, "100G" );
		_grid22.SetCellString( 4, 1, "40ch" );
		_grid22.SetCellString( 4, 2, "192100" );
		_grid22.SetCellString( 4, 3, "196000" );	
		_grid22.SetCellString( 5, 0, "100G" );
		_grid22.SetCellString( 5, 1, "48ch" );
		_grid22.SetCellString( 5, 2, "191300" );
		_grid22.SetCellString( 5, 3, "196000" );	
		_grid22.SetCellString( 6, 0, "100G" );
		_grid22.SetCellString( 6, 1, "52ch" );
		_grid22.SetCellString( 6, 2, "191300" );
		_grid22.SetCellString( 6, 3, "196400" );	
		_grid22.SetColumnWidth( 2, 80 );
		_grid22.SetColumnWidth( 3, 80 );
		for( int r=1; r<7; ++r )
			_grid22.SetRowHeight( r, 25 );

		_grid23.Create( 0, &_canvas2 );
		_grid23.SetRowColumn( 4, 2 );
		_grid23.SetRowFix();
		_grid23.SetColumnFix();
		_grid23.SetCellString( 0, 0, "温度点" );
		_grid23.SetCellString( 0, 1, "温度(℃)" );
		_grid23.SetCellString( 1, 0, "1" );
		_grid23.SetCellString( 2, 0, "2" );
		_grid23.SetCellString( 3, 0, "3" );
		_grid23.SetCellString( 1, 1, "-5" );
		_grid23.SetCellString( 2, 1, "25" );
		_grid23.SetCellString( 3, 1, "75" );
		_grid23.SetColumnWidth( 0, 80 );
		_grid23.SetColumnWidth( 1, 80 );
		for( int r=1; r<4; ++r )
			_grid23.SetRowHeight( r, 25 );

		_layoutGrid1.InitPoint( GenPoint()( 20, 20 ) );
		_layoutGrid1.SetRowColumn( 14, 10 );
		_layoutGrid1.SetColumnWidthInterval( 0, 130, 10 );
		_layoutGrid1.SetColumnWidthInterval( 1, 130, 10 );
		_layoutGrid1.SetCell( 0, 0, &_lable21 );
		_layoutGrid1.SetCell( 0, 1, &_edit21 );
		_layoutGrid1.SetCell( 1, 0, &_lable22 );
		_layoutGrid1.SetCell( 1, 1, &_edit22 );
		_layoutGrid1.SetCell( 2, 0, &_lable24 );
		_layoutGrid1.SetCell( 2, 1, &_combo22 );
		_layoutGrid1.SetCell( 3, 0, 6, 2, &_grid21 );
		_layoutGrid1.SetCell( 7, 0, 10, 2, &_grid22 );
		_layoutGrid1.SetCell( 0, 3, 2, 4, &_grid23 );
		_layoutGrid1.CalcGridPos();
	}
}

void UIT3::OnNotify( int id, int param )
{
	if( id==1 )
	{
		string suffixLine = "txt|CSV";
		VECTOR_STRING fileList;
		string folderPath;
		ReadFolderAllFiles1( suffixLine, folderPath, fileList );

		_grid1.ClearUnfixRows();
			
		for( UINT i=0; i<fileList.size(); ++i )
		{
			VECTOR_STRING row;
			row.resize( 2 );
			STRING_FORMAT( row[0], "%d", i+1 );
			row[1] = folderPath+"\\"+fileList[i];
			_grid1.AddRow( row );
		}
	}
	else if( id==2 )
	{
		// 设置分析类型信息
		WSST3::GetSingleton()->_parseType = 0;

		// 获取文件并载入
		int rowNum, columnNum;
		_grid1.GetRowColumn( rowNum, columnNum );
		--rowNum;
		//
		VECTOR_STRING fileList;		
		for( int i=0; i<rowNum; ++i )
		{
			string str;
			_grid1.GetCellString( i+1, 1, str );
			fileList.push_back( str );
		}
		//
		WSST3::GetSingleton()->StartLoadFileListThread( fileList );

		// 设置要分析的波长
		VECTOR_INT ituList;
		_grid2.GetRowColumn( rowNum, columnNum );
		for( int r=1; r<rowNum; ++r )
		{
			bool flag;
			_grid2.GetCellCheckState( r, 0, flag );
			if( flag )
			{
				string str;
				_grid2.GetCellString( r, 0, str );
				ituList.push_back( atoi( str.c_str() ) );
			}
		}
		WSST3::GetSingleton()->StartSetParseITUListThread( ituList );

		// 开始分析
		WSST3::GetSingleton()->StartParseFileListThread();
		_tabX1.SetCurCell( 1 );
	}
	else if( id==3 )
	{
		NM_GRID* pNMGrid = (NM_GRID*)param;
		if( pNMGrid->_code==3 )
		{
			int rowNum;
			int columnNum;
			_grid2.GetRowColumn( rowNum, columnNum );

			bool flag;
			_grid2.GetCellCheckState( 1, 0, flag );

			for( int r=1; r<rowNum; ++r )
				_grid2.SetCellCheckState( r, 0, !flag );
		}
	}
	else if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
	{
		_tabX1.SetCurCell( param-1 );
	}
}

void UIT3::UIInitPortFreq()
{
	// 频率更新
	SetIndicatorITU();

	// 要显示指标的port
	_tabY1.SetCellNum( PORT_NUM );
	
	for( int p=1; p<=PORT_NUM; ++p )
	{
		string str;
		STRING_FORMAT( str, "p%d", p );
		_tabY1.SetCell( p-1, str, &_indicatorShowList[p-1] );
	}
}

void UIT3::SetIndicatorITU()
{
	VECTOR_INT& ituList = FREQ_GHZ==50?WSST3::GetSingleton()->_parseITUGhzList50G:WSST3::GetSingleton()->_parseITUGhzList100G;
	//
	_grid2.SetRowColumn( 1+ituList.size(), 1 );
	_grid2.SetRowFix();
	_grid2.SetCellString( 0, 0, "频率" );
	_grid2.SetColumnWidth( 0, 150 );
	for( UINT i=0; i<ituList.size(); ++i )
	{
		string str;
		STRING_FORMAT( str, "%d", ituList[i] );
		_grid2.SetCellCheckButton( 0, i+1, 0, str );
		_grid2.SetCellCheckState( i+1, 0, true );
	}
}

void UIT3::ClearIndicatorChart()
{
}

void UIT3::AddIndicatorCurve( double* curveX,  double* curveY, int num, const char* name, COLORREF color, bool curveFlag )
{
}

void UIT3::SetIndicatorGrid( int port, int row, int column )
{
	UIIndicatorShow&  indicatorShow = _indicatorShowList[port-1];
	indicatorShow._grid1.SetRowColumn( row, column );
	indicatorShow._grid1.SetRowFix();
	indicatorShow._grid1.SetColumnFix();
}

void UIT3::SetIndicatorGridCell( int port, int row, int column, const char* v, COLORREF color )
{
	UIIndicatorShow&  indicatorShow = _indicatorShowList[port-1];
	indicatorShow._grid1.SetCellStringAutoWidth( row, column, v );
}