#include "stdafx.h"
#include "UICal.h"

using namespace std;

UIWSSCal gUICal;

void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
	//gUILog.WriteLog( msg );

	UIRefresh();
}

void View101( ULONG ip, const char* msg )
{
	gUICal.ShowCollectMsg( ip, msg );

	//
	string str;
	STRING_FORMAT( str, "%s:%s", IP4_ULONGToStr()( ip ), msg );
	//gUILog.WriteLog( str );
	UIRefresh();
}

void View102( ULONG ip, int c, const char* msg )
{
	gUICal.ShowCollectGrid( ip, c, msg );
	UIRefresh();
}

void View103( ULONG ip )
{
	gUICal.ClearCollectChart( ip );
	UIRefresh();
}

void View104( ULONG ip, int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag )
{
	gUICal.AddCollectCurve( ip, curveX, curveY, num, name, color, curveFlag );
	UIRefresh();
}

void View105( int c, const char* msg ) 
{ 
	gUICal.ShowParseGrid( c, msg ); 
	UIRefresh();
}

void View106() 
{ 
	gUICal.ClearParseChart(); 
	UIRefresh();
}

void View107( int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag ) 
{ 
	gUICal.AddParseCurve( curveX, curveY, num, name, color, curveFlag ); 
	UIRefresh();
}

void View108( bool* channelUseFlag ) 
{ 
	gUICal.SetParseGridChannel( channelUseFlag ); 
	UIRefresh();
}





void UIWSSCal::OnCreate()
{
	//
	SetCB_View_ShowMsg( View0 );

	PCB_View_ShowMsg				= View0;
	PCB_View_ShowCollectMsg			= View101;				
	PCB_View_ShowCollectGrid		= View102;				
	PCB_View_ClearCollectChart		= View103;			
	PCB_View_AddCollectCurve		= View104;			
	PCB_View_ShowParseGrid			= View105;		
	PCB_View_ClearParseChart		= View106;									
	PCB_View_AddParseCurve			= View107;	
	PCB_View_SetParseGridChannel	= View108;	


	//
	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	str += "\\Data\\";
	str += FREQ_GHZ==50?"ITU50.xls":"ITU100.xls";
	LoadCalITUList( str.c_str() );


	//
	_tabX1.Create( 0, this, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 3 );
	_tabX1.SetCell( 0, "设置", &_canvas1 );
	_tabX1.SetCell( 1, "绘图", &_canvas2 );
	_tabX1.SetCell( 2, "写入", &_canvas3 );
	//
	{
		_canvas1.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
		
		_lable3.Create( 0, &_canvas1 );
		_lable3.SetString( "设置电路板: " );
		_grid1.Create( 1, &_canvas1 );
		_grid1.SetRowColumn( 5, 5 );
		_grid1.SetRowFix();
		_grid1.SetCellString( 0, 0, "序号" );
		_grid1.SetCellString( 0, 1, "IP" );
		_grid1.SetCellString( 0, 2, "光开关IP" );
		_grid1.SetCellString( 0, 3, "SN" );
		_grid1.SetCellString( 0, 4, "当前温度" );
		for( int i=1; i<=4; ++i )
		{
			_grid1.SetRowHeight( i, 25 );
			_grid1.SetColumnWidth( i, 125 );
			STRING_FORMAT( str, "%d", i );
			_grid1.SetCellCheckButton( 0, i, 0, str );
		}
		/*_grid1.SetCellString( 1, 1, "172.168.88.123" );
		_grid1.SetCellString( 2, 1, "172.168.89.126" );
		_grid1.SetCellString( 3, 1, "172.168.88.125" );
		_grid1.SetCellString( 4, 1, "172.168.89.127" );
		_grid1.SetCellString( 1, 2, "172.168.88.173" );
		_grid1.SetCellString( 2, 2, "172.168.88.174" );
		_grid1.SetCellString( 3, 2, "172.168.88.175" );
		_grid1.SetCellString( 4, 2, "172.168.88.176" );*/
		_grid1.SetCellString( 1, 1, "172.168.89.121" );
		_grid1.SetCellString( 2, 1, "172.168.89.128" );
		_grid1.SetCellString( 3, 1, "172.168.88.126" );
		_grid1.SetCellString( 4, 1, "172.168.89.129" );
		_grid1.SetCellString( 1, 2, "172.168.88.177" );
		_grid1.SetCellString( 2, 2, "172.168.88.178" );
		_grid1.SetCellString( 3, 2, "172.168.88.179" );
		_grid1.SetCellString( 4, 2, "172.168.88.180" );

			
		_lable4.Create( 0, &_canvas1 );
		_lable4.SetString( "设置温度: " );
		_grid2.Create( 0, &_canvas1 );
		_grid2.SetRowColumn( 7, 2 );
		_grid2.SetRowFix();
		_grid2.SetColumnFix();
		_grid2.SetColumnWidth( 1, 150 );
		_grid2.SetCellString( 0, 0, "温度点" );
		_grid2.SetCellString( 0, 1, "温度" );
		for( int i=1; i<=6; ++i )
		{
			_grid2.SetRowHeight( i, 25 );
			STRING_FORMAT( str, "%d", i );
			_grid2.SetCellString( i, 0, str );
		}

		_lable1.Create( 0, &_canvas1 );
		_lable1.SetString( "温循时间: " );
		_comBox1.Create( 0, &_canvas1 );
		_comBox1.AddString( "1 min" );
		_comBox1.AddString( "1 sec" );
		if( GetWaitTempStableTime()/1000==60 )
			_comBox1.SetSelectIndex( 0 );
		else
			_comBox1.SetSelectIndex( 1 );

		_lable6.Create( 0, &_canvas1 );
		_lable6.SetString( "LSB: " );
		_comBox3.Create( 20, &_canvas1 );
		_comBox3.AddString( "1" );
		_comBox3.AddString( "2" );
		_comBox3.AddString( "4" );
		if( LSB==1 )
			_comBox3.SetSelectIndex( 0 );
		else if( LSB==2 )
			_comBox3.SetSelectIndex( 1 );
		else if( LSB==4 )
			_comBox3.SetSelectIndex( 2 );

		_lable2.Create( 0, &_canvas1 );
		_lable2.SetString( "设置监控IP: " );
		_edit1.Create( 0, &_canvas1 );
		//_edit1.SetString( "172.168.88.128" );
		_edit1.SetString( "172.168.88.124" );

		_but1.Create( 3, &_canvas1 );
		_but1.SetString( "存光" );
		_but2.Create( 4, &_canvas1 );
		_but2.SetString( "定标" );
		_but3.Create( 2, &_canvas1 );
		_but3.SetString( "读取" );

		// 
		_tabX2.Create( 0, &_canvas1, GenRect()( 700, 20, 900, 980 ), UILayoutCalc::SIZE_Y );
		_tabX2.HideTab();
		_tabX2.SetCellNum( 4 );
		_tabX2.SetCell( 0, "p1", &_gridList1[0] );
		_tabX2.SetCell( 1, "p2", &_gridList1[1] );
		_tabX2.SetCell( 2, "p3", &_gridList1[2] );
		_tabX2.SetCell( 3, "p4", &_gridList1[3] );
		//
		for( int i=0; i<4; ++i )
			_gridList1[i].Create( 30+i, &_tabX2 );
	}
	{
		_canvas2.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
		
		_grid3.Create( 5, &_canvas2, GenRect()( 20, 20, 500, 150 ) );
		_grid3.SetRowColumn( 1, 3 );
		_grid3.SetRowFix();
		_grid3.SetColumnFix();
		_grid3.SetCellString( 0, 0, "序号" );
		_grid3.SetCellString( 0, 1, "电路板IP" );
		_grid3.SetCellString( 0, 2, "信息" );
		_grid3.SetColumnWidth( 1, 120 );
		_grid3.SetColumnWidth( 2, 280 );

		_tabX3.Create( 0, &_canvas2, GenRect()( 620, 20, 980, 980 ), UILayoutCalc::MOVE_X|UILayoutCalc::SIZE_Y );
		_tabX3.HideTab();
		_tabX3.SetX();
		{
			for( int i=0; i<4; ++i )
			{
				_gridList2[i].Create( 0, &_tabX3 );
				_gridList2[i].SetRowColumn( 1, 3 );
				_gridList2[i].SetRowFix();
				_gridList2[i].SetColumnFix();
				_gridList2[i].SetCellString( 0, 0, "通道" );
				_gridList2[i].SetCellString( 0, 1, "波长" );
				_gridList2[i].SetCellString( 0, 2, "信息" );
				_gridList2[i].SetColumnWidth( 1, 80 );
				_gridList2[i].SetColumnWidth( 2, 200 );	
			}
		}

		_tabX4.Create( 0, &_canvas2, GenRect()( 20, 170, 600, 980 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_tabX4.HideTab();
		_tabX4.SetX();
		{
			for( int i=0; i<4; ++i )
				_chartList1[i].Create( 0, &_tabX4 );
		}
	}
	{
		_canvas3.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
		
		_grid4.Create( 6, &_canvas3, GenRect()( 20, 20, 530, 170 ) );
		_grid4.SetRowColumn( 1, 2 );
		_grid4.SetRowFix();
		_grid4.SetColumnFix();
		_grid4.SetCellString( 0, 0, "序号" );
		_grid4.SetCellString( 0, 1, "文件名" );
		_grid4.SetColumnWidth( 1, 430 );

		_grid5.Create( 7, &_canvas3, GenRect()( 20, 190, 370, 980 ), UILayoutCalc::SIZE_Y );
		_chart1.Create( 0, &_canvas3, GenRect()( 390, 190, 980, 980 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_lable5.Create( 0, &_canvas3 );
		_lable5.SetString( "目标写入盘ip: " );
		_edit2.Create( 0, &_canvas3 );
		_edit2.SetString( "172.168.88.123" );
		_but4.Create( 8, &_canvas3 );
		_but4.SetString( "载入数据" );
		_but5.Create( 9, &_canvas3 );
		_but5.SetString( "设置波长" );
		_but6.Create( 10, &_canvas3 );
		_but6.SetString( "分析数据" );
		_but7.Create( 11, &_canvas3 );
		_but7.SetString( "发送数据" );
		_checkBut1.Create( 0, &_canvas3 );
		_checkBut1.SetString( "原始" );
		_checkBut1.SetCheck( true );
		_checkBut2.Create( 0, &_canvas3 );
		_checkBut2.SetString( "分析" );
		UISetCheckButtonMutex( &_checkBut1, &_checkBut2 );
		_comBox2.Create( 0, &_canvas3 );
		_comBox2.AddString( "P1右" );
		_comBox2.AddString( "P1左" );
		_comBox2.SetSelectIndex( IsOddRight?0:1 );
	}


	// 设置布局信息
	_layoutGrid1.InitPoint( GenPoint()( 50, 20 ) );
	_layoutGrid1.SetRowColumn( 11, 5 );
	_layoutGrid1.SetCell( 0, 0, &_lable3 );
	_layoutGrid1.SetCell( 0, 1, &_but3 );
	_layoutGrid1.SetCell( 1, 0, 3, 4, &_grid1 );
	_layoutGrid1.SetCell( 4, 0, &_lable2 );
	_layoutGrid1.SetCell( 4, 1, 4, 2, &_edit1 );
	_layoutGrid1.SetCell( 4, 3, &_lable1 );
	_layoutGrid1.SetCell( 4, 4, &_comBox1 );
	_layoutGrid1.SetCell( 5, 0, &_lable4 );
	_layoutGrid1.SetCell( 5, 1, 8, 2, &_grid2 );
	_layoutGrid1.SetCell( 5, 3, &_lable6 );
	_layoutGrid1.SetCell( 5, 4, &_comBox3 );
	_layoutGrid1.SetCell( 0, 3, &_but1 );
	_layoutGrid1.SetCell( 0, 4, &_but2 );
	//
	_layoutGrid1.CalcGridPos();

	_layoutGrid2.InitPoint( GenPoint()( 550, 20 ) );
	_layoutGrid2.SetRowColumn( 3, 3 );
	_layoutGrid2.SetColumnWidthInterval( 0, 135 );
	_layoutGrid2.SetColumnWidthInterval( 1, 135 );
	_layoutGrid2.SetColumnWidthInterval( 2, 70 );
	_layoutGrid2.SetRowHeightInterval( 0, 35 );
	_layoutGrid2.SetRowHeightInterval( 1, 35 );
	_layoutGrid2.SetRowHeightInterval( 2, 35 );
	_layoutGrid2.SetCell( 0, 0, &_lable5 );
	_layoutGrid2.SetCell( 0, 1, &_edit2 );
	_layoutGrid2.SetCell( 1, 0, &_but4 );
	_layoutGrid2.SetCell( 1, 1, &_but5 );
	_layoutGrid2.SetCell( 2, 0, &_but6 );
	_layoutGrid2.SetCell( 2, 1, &_but7 );
	_layoutGrid2.SetCell( 1, 2, &_checkBut1 );
	_layoutGrid2.SetCell( 2, 2, &_checkBut2 );
	_layoutGrid2.SetCell( 0, 2, &_comBox2 );
	//
	_layoutGrid2.CalcGridPos();

	//
	UIInitPortFreq();
}

void UIWSSCal::OnMainBar( int param )
{
	_tabX1.SetCurCell( param-1 );
}

void UIWSSCal::OnSetCalBoard( int param )
{
	NM_GRID* pNMGrid = (NM_GRID*)param;
	if( pNMGrid->_code==4 )
		_tabX2.SetCurCell( pNMGrid->_row-1 );
}

void UIWSSCal::OnSetCalChannels( int id, int param )
{
	UIGrid& grid = _gridList1[id-30];

	NM_GRID* pNMGrid = (NM_GRID*)param;
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
}

void UIWSSCal::OnReadBoardInfo( int param )
{
	string str;
	for( int i=1; i<=4; ++i )
	{
		bool flag;
		_grid1.GetCellCheckState( i, 0, flag );
		if( !flag ) continue;

		_grid1.GetCellString( i, 1, str );
		ULONG ip = IP4_StrToULONG()( str.c_str() );

		int sn;
		if( GetSerialNumberSyn_Net( ip, &sn ) )
		{
			STRING_FORMAT( str, "0x%X", sn );
			_grid1.SetCellString( i, 3, str );
		}

		float temper;
		if( GetTemperatureSyn_Net( ip, &temper ) )
		{
			STRING_FORMAT( str, "%0.1f(℃)", temper );
			_grid1.SetCellString( i, 4, str );
		}
	}
}

void UIWSSCal::OnStartSaveLight( int param )
{
	Func1();

	// 开始存光
	CollectDataProcess::GetSingleton()->StartSaveLightProcess( -1 );	
	Func2();
}

void UIWSSCal::OnStartCal( int param )
{
	Func1();

	// 设置等待温循箱稳定时间
	int index = _comBox1.GetSelectIndex();
	if( index==0 )
		GetWaitTempStableTime() = 60*1000;
	else
		GetWaitTempStableTime() = 1000;

	// 设置要循环的温度点
	string str;
	VECTOR_FLOAT temList;
	for( int i=1; i<=6; ++i )
	{
		_grid2.GetCellString( i, 1, str );
		if( str!="" )
			temList.push_back( (float)atof( str.c_str() ) );
	}
	//
	if( temList.size()==0 ) return;
	CollectDataProcess::GetSingleton()->SetTemPointsList( temList );

	// 开始定标
	CollectDataProcess::GetSingleton()->StartCollectAllTemPointsProcess();
	Func2();
}

void UIWSSCal::OnSetDrawBoard1( int param )
{
	NM_GRID* pNMGrid = (NM_GRID*)param;
	if( pNMGrid->_code==4 )
	{
		_tabX3.SetCurCell( pNMGrid->_row-1 );
		_tabX4.SetCurCell( pNMGrid->_row-1 );
	}
}

void UIWSSCal::OnGetFileList( int param )
{
	NM_GRID* pNMGrid = (NM_GRID*)param;
	if( pNMGrid->_code==3 )
	{
		if( pNMGrid->_column==1 )
		{
			string suffixLine = "txt";
			VECTOR_STRING fileList;
			string folderPath;
			ReadFolderAllFiles1( suffixLine, folderPath, fileList );

			_grid4.ClearUnfixRows();
			_fileNum = (int)fileList.size();
			for( int i=0; i<_fileNum; ++i )
			{
				VECTOR_STRING row;
				row.resize( 2 );
				STRING_FORMAT( row[0], "%d", i+1 );
				row[1] = folderPath+"\\"+fileList[i];
				_grid4.AddRow( row );
			}
		}
	}
}

void UIWSSCal::OnLoadFileList( int param )
{
	// 获取写入目标ip
	string str;
	_edit2.GetString( str );
	ULONG ip = IP4_StrToULONG()( str.c_str() );

	// 从grid获取文件名列表
	VECTOR_STRING fileList;
	for( int i=0; i<_fileNum; ++i )
	{
		string str;
		_grid4.GetCellString( i+1, 1, str );
		fileList.push_back( str );
	}

	// 文件载入
	UCHAR tempList[32];
	for( int i=1; i<=6; ++i )
		tempList[i-1] = i;
	
	ParseDataProcess::GetSingleton()->LoadCollectData( ip, fileList, tempList );
}

void UIWSSCal::OnGrid5( int param )
{
	NM_GRID* pNMGrid = (NM_GRID*)param;
	if( pNMGrid->_code==3 )
	{	// 全选全不选
		if( pNMGrid->_column==0 )
		{
			int rowNum;
			int columnNum;
			_grid5.GetRowColumn( rowNum, columnNum );

			bool flag;
			_grid5.GetCellCheckState( 1, 0, flag );

			for( int r=1; r<rowNum; ++r )
				_grid5.SetCellCheckState( r, 0, !flag );
		}
		else if( pNMGrid->_column==2 )
		{
			string fileName;
			if( OpenFolderFile( fileName ) )
			{
				TxtOBJ txt;
				if( txt.OpenFile( fileName ) )
				{
					std::vector<VECTOR_STRING > allLineWordsList;
					txt.ReadAllLineWords( allLineWordsList );
					if( allLineWordsList.size()==CHANNEL_NUM )
					{
						for( int c=1; c<=CHANNEL_NUM; ++c )
						{
							VECTOR_STRING& line =  allLineWordsList[c-1];
							_grid5.SetCellString( c, 2, line[0] );
						}
					}
					UIRefresh();
				}
			}
		}
	}
	else if( pNMGrid->_code==4 )
	{	// 选中某个通道 绘图
		UINT row, column;
		bool rt = _grid4.GetSelectCell( row, column );
		if( rt==false ) return;

		int port = PORT_NUM;
		bool flag;
		_checkBut1.GetCheck( flag );

		_chart1.Clear();
		for( int p=1; p<=port; ++p )
		{
			string str;
			STRING_FORMAT( str, "f%d_port%d", row, p );

			VECTOR_INT* pXList=NULL;
			VECTOR_FLOAT* pYList=NULL;

			if( flag )
				ParseDataProcess::GetSingleton()->GetLoadData( row-1, pNMGrid->_row, p, pXList, pYList );
			else
				ParseDataProcess::GetSingleton()->GetParseData( row-1, pNMGrid->_row, p, pXList, pYList );

			VECTOR_DOUBLE xListT, yListT;
			for( UINT i=0; i<pXList->size(); ++i )
			{
				xListT.push_back( (*pXList)[i] );
				yListT.push_back( (*pYList)[i] );
			}
	
			_chart1.AddCurve1( str, xListT, yListT );
			_chart1.SetCurve1Color( str, p%2!=0?_BLUE_:_RED_ );
		}
		_chart1.CalcXYCoordRange();
	}
}

void UIWSSCal::OnSetParseChannels( int param )
{
	for( int r=1; r<=CHANNEL_NUM; ++r )
	{
		bool flag;
		_grid5.GetCellCheckState( r, 0, flag );
		_parseChannelList[r-1] = flag;
	}

	ParseDataProcess::GetSingleton()->SetParseChannels( -1, _parseChannelList, CHANNEL_NUM );
}

void UIWSSCal::OnParseData( int param )
{
	if( PORT_NUM>2 )
	{
		int sel = _comBox2.GetSelectIndex();
		IsOddRight() = sel==0?true:false;
	}

	ParseDataProcess::GetSingleton()->ParseCollectData();
}

void UIWSSCal::OnSendData( int param )
{
	string str;
	VECTOR_INT yVolList;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		_grid5.GetCellString( c, 2, str );
		yVolList.push_back( atoi( str.c_str() ) );
	}

	ParseDataProcess::GetSingleton()->SendParseData( yVolList );
}

void UIWSSCal::OnSetLSB( int param )
{
	int index = _comBox3.GetSelectIndex();
	if( index==0 )
		LSB = 1;
	else if( index==1 )
		LSB = 2;
	else if( index==2 )
		LSB = 4;
}

void UIWSSCal::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
		OnMainBar( param );
	else if( id>=30 && id<=34 )
		OnSetCalChannels( id, param );
	else if( id==1 )
		OnSetCalBoard( param );
	else if( id==2 )
		OnReadBoardInfo( param );
	else if( id==3 )
		OnStartSaveLight( param );
	else if( id==4 )
		OnStartCal( param );
	else if( id==5 )
		OnSetDrawBoard1( param );
	else if( id==6 )
		OnGetFileList( param );
	else if( id==7 )
		OnGrid5( param );
	else if( id==8 )
		OnLoadFileList( param );
	else if( id==9 )
		OnSetParseChannels( param );
	else if( id==10 )
		OnParseData( param );
	else if( id==11 )
		OnSendData( param );
	else if( id==20 )
		OnSetLSB( param );
}

void UIWSSCal::UIInitPortFreq()
{
	string str;
	string* ituList = WSS_ITULIST;

	// 定标设置
	for( int i=0; i<4; ++i )
	{
		_gridList1[i].SetRowColumn( CHANNEL_NUM+1, 2 );
		_gridList1[i].SetRowFix();
		_gridList1[i].SetColumnWidth( 0, 80 );
		_gridList1[i].SetCellString( 0, 0, "通道" );
		_gridList1[i].SetColumnWidth( 1, 100 );
		STRING_FORMAT( str, "波长%d", i+1 );
		_gridList1[i].SetCellString( 0, 1, str );
		for( int j=1; j<=CHANNEL_NUM; ++j )
		{
			STRING_FORMAT( str, "%d", j );
			_gridList1[i].SetCellCheckButton( 0,  j, 0, str.c_str() );
			//_gridList1[i].SetCellCheckState( j, 0, true );

			STRING_FORMAT( str, "%0.2f", atof( ituList[j-1].c_str() ) );
			_gridList1[i].SetCellString( j, 1, str.c_str() );
		}
	}

	// 数据分析
	_grid5.SetRowColumn( CHANNEL_NUM+1, 4 );
	_grid5.SetRowFix();
	_grid5.SetCellString( 0, 0, "通道" );
	_grid5.SetCellString( 0, 1, "波长" );
	_grid5.SetCellString( 0, 2, "Y_定标" );
	_grid5.SetCellString( 0, 3, "信息" );
	_grid5.SetColumnWidth( 3, 153 );
	for( int j=1; j<=CHANNEL_NUM; ++j )
	{
		STRING_FORMAT( str, "%d", j );
		_grid5.SetCellCheckButton( 0,  j, 0, str.c_str() );

		STRING_FORMAT( str, "%0.2f", atof( ituList[j-1].c_str() ) );
		_grid5.SetCellString( j, 1, str.c_str() );
	}
}

void UIWSSCal::Func1()
{
	string str;

	// 添加监控
	_edit1.GetString( str );
	CollectDataProcess::GetSingleton()->SetInspectOPMAddr( IP4_StrToULONG()( str.c_str() ) );

	// 设置子电路板
	for( int i=1; i<=4; ++i )
	{
		bool flag;
		_grid1.GetCellCheckState( i, 0, flag );
		if( !flag ) continue;

		_grid1.GetCellString( i, 1, str );
		ULONG ip = IP4_StrToULONG()( str.c_str() );

		// 添加电路板
		CollectDataProcess::GetSingleton()->AddChildThread( ip );

		// 添加光开关
		_grid1.GetCellString( i, 2, str );
		ULONG oswIP = IP4_StrToULONG()( str.c_str() );
		CollectDataProcess::GetSingleton()->AddChildThread( ip, oswIP );

		// 设置要定标的波长
		UIGrid& grid = _gridList1[i-1];
		VECTOR_INT channelList;
		int rowNum, columnNum;
		grid.GetRowColumn( rowNum, columnNum );
		for( int r=1; r<rowNum; ++r )
		{
			bool flag;
			grid.GetCellCheckState( r, 0, flag );
			if( flag )
				channelList.push_back( r );
		}
		//
		if( channelList.size()==0 ) continue;
		CollectDataProcess::GetSingleton()->AddChildThread( ip, channelList );
	}
}

void UIWSSCal::Func2()
{
	VECTOR_INT ipList;
	VECTOR_INT indexList;

	string str;
	for( int i=1; i<=4; ++i )
	{
		bool flag;
		_grid1.GetCellCheckState( i, 0, flag );
		if( !flag ) continue;

		_grid1.GetCellString( i, 1, str );
		ipList.push_back( IP4_StrToULONG()( str.c_str() ) );
		indexList.push_back( i-1 );
	}

	if( ipList.size()==0 ) return;

	// 初始化_grid3
	_grid3.ClearUnfixRows();
	for( UINT i=0; i<ipList.size(); ++i )
	{
		VECTOR_STRING row;
		STRING_FORMAT( str, "%d", i+1 );
		row.push_back( str );

		STRING_FORMAT( str, "%s", IP4_ULONGToStr()( ipList[i] ) );
		row.push_back( str );

		row.push_back( "" );
		_grid3.AddRow( row );
	}

	//  初始化
	for( int i=0; i<4; ++i )
	{
		_gridList2[i].ShowWindowDirect( false );
		_chartList1[i].ShowWindowDirect( false );
	}

	_tabX3.SetCellNum( indexList.size() );
	_tabX4.SetCellNum( indexList.size() );
	for( UINT i=0; i<indexList.size(); ++i )
	{	
		// 初始化 _gridList2
		UIGrid& grid1 = _gridList1[indexList[i]];
		UIGrid& grid2 = _gridList2[i];
		grid2.ClearUnfixRows();
		//
		int rowNum, columnNum;
		grid1.GetRowColumn( rowNum, columnNum );
		for( int r=1; r<rowNum; ++r )
		{
			bool flag;
			grid1.GetCellCheckState( r, 0, flag );
			if( !flag ) continue;

			VECTOR_STRING row;
			grid1.GetCellString( r, 0, str );
			row.push_back( str );
			grid1.GetCellString( r, 1, str );
			row.push_back( str );
			row.push_back( "" );

			grid2.AddRow( row );
		}
		//
		_tabX3.SetCell( i, "", &grid2 );
		_tabX4.SetCell( i, "", &_chartList1[i] );
	}

	_tabX1.SetCurCell( 1 );
}

int UIWSSCal::SearchIPIndex( ULONG ip )
{
	int rowNum, columnNum;
	_grid3.GetRowColumn( rowNum, columnNum );
	string str;
	for( int r=1; r<rowNum; ++r )
	{
		_grid3.GetCellString( r, 1, str );
		if( IP4_StrToULONG()( str.c_str() )==ip )
			return r;
	}

	return -1;
}

int UIWSSCal::SearchChannelIndex( ULONG ip, int c )
{
	UIGrid& grid = _gridList2[SearchIPIndex( ip )-1];

	int rowNum, columnNum;
	grid.GetRowColumn( rowNum, columnNum );
	string str;
	for( int r=1; r<rowNum; ++r )
	{
		grid.GetCellString( r, 0, str );
		if( atoi( str.c_str() )==c )
			return r;
	}

	return -1;
}

void UIWSSCal::ShowCollectMsg( ULONG ip, const char* msg )
{
	_grid3.SetCellString( SearchIPIndex( ip ), 2, msg );
}

void UIWSSCal::ShowCollectGrid( ULONG ip, int c, const char* msg )
{
	UIGrid& grid = _gridList2[SearchIPIndex( ip )-1];
	grid.SetCellString( SearchChannelIndex( ip, c ), 2, msg );
}

void UIWSSCal::ClearCollectChart( ULONG ip )
{
	UIChart& chart = _chartList1[SearchIPIndex( ip )-1];
	chart.Clear();
}

void UIWSSCal::AddCollectCurve( ULONG ip, int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag )
{
	UIChart& chart = _chartList1[SearchIPIndex( ip )-1];
	VECTOR_DOUBLE xList, yList;
	for( int i=0; i<num; ++i )
	{
		xList.push_back( (double)curveX[i] );
		yList.push_back( (double)curveY[i] );
	}

	chart.AddCurve1( name, xList, yList );
	chart.SetCurve1Color( name, color );
	chart.CalcXYCoordRange();
}

void UIWSSCal::SetParseGridChannel( bool* channelUseFlag ) 
{ 
	for( int i=1; i<=CHANNEL_NUM; ++i )
		_grid5.SetCellCheckState( i, 0, channelUseFlag[i-1] );
}

void UIWSSCal::ShowParseGrid( int c, const char* msg ) 
{ 
	_grid5.SetCellString( c, 3, msg );
}

void UIWSSCal::ClearParseChart() 
{ 
	_chart1.Clear();
}

void UIWSSCal::AddParseCurve( int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag ) 
{ 
	VECTOR_DOUBLE xList, yList;
	for( int i=0; i<num; ++i )
	{
		xList.push_back( (double)curveX[i] );
		yList.push_back( (double)curveY[i] );
	}

	_chart1.AddCurve1( name, xList, yList );

	_chart1.SetCurve1Color( name, color );
	_chart1.CalcXYCoordRange();
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   