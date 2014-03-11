
#include "stdafx.h"
#include "UIOS.h"


UIOS gUIScan;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void View5( UCHAR portNum, string* waveLenList )
{
	gUIScan.ShowOSWaveLenList( portNum, waveLenList );
}

void View6( UCHAR portNum, float* pmList )
{
	gUIScan.ShowOSOutPutPMList( portNum, pmList );
}

void View101( float k, float b )
{
	gUIScan.ShowKB( k, b );
}

bool View102( ULONG IBMON, ULONG IMPDMON, ULONG resistance )
{
	return gUIScan.ShowResistance( IBMON, IMPDMON, resistance );
}

void View103( float* readPM, float* referPM )
{
	gUIScan.ShowVerifyKB( readPM, referPM );
}

void UIOS::OnCreate()
{
	//
	SetCB_View_ShowMsg( View0 );
	SetCB_View_ShowOSWaveLenList( View5 );
	SetCB_View_ShowOSOutPutPMList( View6 );

	PCB_View_ShowMsg = View0;
	PCB_View_ShowKB = View101;
	PCB_View_ShowResistance = View102;
	PCB_View_ShowVerifyKB = View103;

	//
	string str;


	_tabX1.Create( 0, this, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 2 );
	_tabX1.SetCell( 0, "定标", &_canvas1 );
	_tabX1.SetCell( 1, "监视", &_canvas2 );
	_tabX1.SetCurCell( 0 );

	_canvas1.CreateDirect( &_tabX1 );

	_but1.Create( 101, &_canvas1 );
	_but1.SetString( "获取采样值" );
	_but2.Create( 102, &_canvas1 );
	_but2.SetString( "获取功率值" );
	_but3.Create( 103, &_canvas1 );
	_but3.SetString( "打开光源" );
	_but4.Create( 104, &_canvas1 );
	_but4.SetString( "关闭光源" );
	_but5.Create( 105, &_canvas1 );
	_but5.SetString( "读取波长" );
	_but6.Create( 106, &_canvas1 );
	_but6.SetString( "设置波长" );
	_but7.Create( 107, &_canvas1 );
	_but7.SetString( "读取出光功率" );
	_but8.Create( 108, &_canvas1 );
	_but8.SetString( "获取电流、电阻值" );
	_but9.Create( 109, &_canvas1 );
	_but9.SetString( "计算KB" );
	_but10.Create( 110, &_canvas1 );
	_but10.SetString( "设置定标数据" );
	_but11.Create( 111, &_canvas1 );
	_but11.SetString( "验证定标数据" );
	_but12.Create( 112, &_canvas1 );
	_but12.SetString( "写数据库" );
	_but13.Create( 113, &_canvas1 );
	_but13.SetString( "读数据库" );

	_grid1.Create( 0, &_canvas1, GenRect()( 280, 20, 480, 400 ) );
	_grid1.SetRowColumn( 18, 3 );
	_grid1.SetRowFix();
	_grid1.SetColumnFix();
	_grid1.SetColumnWidth( 0, 50 );
	_grid1.SetColumnWidth( 1, 70 );
	_grid1.SetCellString( 0, 0, "序号" );
	_grid1.SetCellString( 0, 1, "出光功率" );
	_grid1.SetCellString( 0, 2, "波长" );
	_grid1.SetCellString( 1, 0, "端口数" );
	for( int i=0; i<16; ++i )
	{
		STRING_FORMAT( str, "%d", i+1 );
		_grid1.SetCellString( i+2, 0, str );
	}

	_grid2.Create( 0, &_canvas1 );
	_grid2.SetRowColumn( 3, 2 );
	_grid2.SetRowFix();
	_grid2.SetColumnFix();
	_grid2.SetColumnWidth( 1, 80 );
	_grid2.SetColumnWidth( 2, 80 );
	_grid2.SetCellString( 0, 0, "序号" );
	_grid2.SetCellString( 1, 0, "K" );
	_grid2.SetCellString( 2, 0, "B" );

	_grid3.Create( 0, &_canvas1 );
	_grid3.SetRowColumn( 4, 4 );
	_grid3.SetRowFix();
	_grid3.SetColumnFix();
	_grid3.SetColumnWidth( 0, 90 );
	_grid3.SetColumnWidth( 1, 90 );
	_grid3.SetCellString( 0, 0, "项" );
	_grid3.SetCellString( 0, 1, "k值" );
	_grid3.SetCellString( 0, 2, "当前值" );
	_grid3.SetCellString( 0, 3, "告警阈值" );
	_grid3.SetCellString( 1, 0, "驱动电流(mA)" );
	_grid3.SetCellString( 2, 0, "背光电流(uA)" );
	_grid3.SetCellString( 3, 0, "电阻值(欧姆)" );
	_grid3.SetCellString( 1, 3, "100" );
	_grid3.SetCellString( 2, 3, "1000" );
	_grid3.SetCellString( 3, 3, "1000" );
	_grid3.SetCellString( 1, 1, OSLogic::GetSingleton()->k1[0] );
	_grid3.SetCellString( 2, 1, OSLogic::GetSingleton()->k1[1] );

	_grid4.Create( 0, &_canvas1 );
	_grid4.SetRowColumn( 3, 4 );
	_grid4.SetRowFix();
	_grid4.SetColumnFix();
	_grid4.SetCellString( 0, 0, "点" );
	_grid4.SetCellString( 0, 1, "读取值" );
	_grid4.SetCellString( 0, 2, "参考值" );
	_grid4.SetCellString( 0, 3, "差值" );
	_grid4.SetCellString( 1, 0, "1" );
	_grid4.SetCellString( 2, 0, "2" );

	_check1.Create( 121, &_canvas1 );
	_check1.SetString( "cyoptics" );
	_check1.SetCheck( true );
	_check2.Create( 122, &_canvas1 );
	_check2.SetString( "accelink" );
	UISetCheckButtonMutex( &_check1, &_check2 );

	_lable1.Create( 0, &_canvas1 );
	_lable1.SetString( "Step 1:" );
	_lable1.SetColor( _BLUE_ );
	_lable2.Create( 0, &_canvas1 );
	_lable2.SetString( "Step 2:" );
	_lable2.SetColor( _BLUE_ );
	_lable3.Create( 0, &_canvas1 );
	_lable3.SetString( "Step 3:" );
	_lable3.SetColor( _BLUE_ );
	_lable4.Create( 0, &_canvas1 );
	_lable4.SetString( "Step 4:" );
	_lable4.SetColor( _BLUE_ );
	_lable5.Create( 0, &_canvas1 );
	_lable5.SetString( "( 端口值=端口X功率-端口1功率 )自动计算差值，只填写功率值." );
	_lable6.Create( 0, &_canvas1 );
	_lable6.SetString( "设备SN号:" );

	_edit1.Create( 0, &_canvas1 );
	

	_canvas2.CreateDirect( &_tabX1 );
	//
	//
	_but21.Create( 21, &_canvas2 );
	_but21.SetString( "开始监视" );
	_but22.Create( 22, &_canvas2 );
	_but22.SetString( "停止监视" );
	_but23.Create( 23, &_canvas2 );
	_but23.SetString( "读取最大最小值" );
	_but24.Create( 24, &_canvas2 );
	_but24.SetString( "添加设备" );
	_but25.Create( 25, &_canvas2 );
	_but25.SetString( "删除选中设备" );
	_but26.Create( 26, &_canvas2 );
	_but26.SetString( "清除所有设备" );
	//
	_lable21.Create( 0, &_canvas2 );
	_lable21.SetString( "注意：设备IP地址需与主机位于同一网段内.." );
	_edit21.Create( 0, &_canvas2 );
	_grid21.Create( 0, &_canvas2 );
	_grid21.SetRowColumn( 1, 5 );
	_grid21.SetRowFix();
	_grid21.SetColumnFix();
	_grid21.SetCellString( 0, 0, "序号" );
	_grid21.SetColumnWidth( 0, 100 );
	_grid21.SetCellString( 0, 1, "IP地址" );
	_grid21.SetColumnWidth( 1, 150 );
	_grid21.SetCellString( 0, 2, "监测标志" );
	_grid21.SetColumnWidth( 2, 120 );
	_grid21.SetCellString( 0, 3, "MAX" );
	_grid21.SetColumnWidth( 3, 120 );
	_grid21.SetCellString( 0, 4, "MIN" );
	_grid21.SetColumnWidth( 4, 120 );


	_layoutGrid1.InitPoint( GenPoint()( 20, 20 ) );
	_layoutGrid1.SetRowColumn( 10, 2 );
	for( int r=0; r<10; ++r )
		_layoutGrid1.SetRowHeightInterval( r, 50 );
	//
	_layoutGrid1.SetCell( 0, 0, &_but1 );
	_layoutGrid1.SetCell( 0, 1, &_but2 );
	_layoutGrid1.SetCell( 1, 0, &_but3 );
	_layoutGrid1.SetCell( 1, 1, &_but4 );
	_layoutGrid1.SetCell( 3, 0, &_but5 );
	_layoutGrid1.SetCell( 3, 1, &_but6 );
	_layoutGrid1.SetCell( 4, 0, 4, 1, &_but7 );
	_layoutGrid1.SetCell( 7, 0, 7, 1, &_but8 );
	//
	_layoutGrid1.CalcGridPos();

	_layoutGrid2.InitPoint( GenPoint()( 280, 450 ) );
	_layoutGrid2.SetRowColumn( 4, 3 );
	//
	_layoutGrid2.SetCell( 0, 0, &_check1 );
	_layoutGrid2.SetCell( 0, 1, &_check2 );
	_layoutGrid2.SetCell( 1, 0, 3, 2, &_grid3 );
	//
	_layoutGrid2.CalcGridPos();


	_layoutGrid3.InitPoint( GenPoint()( 500, 20 ) );
	_layoutGrid3.SetRowColumn( 10, 4 );
	_layoutGrid3.SetColumnWidthInterval( 0, 60 );
	_layoutGrid3.SetColumnWidthInterval( 1, 120 );
	_layoutGrid3.SetColumnWidthInterval( 2, 120 );
	_layoutGrid3.SetColumnWidthInterval( 3, 120 );
	//
	_layoutGrid3.SetCell( 0, 0, &_lable1 );
	_layoutGrid3.SetCell( 0, 1, &_but9 );
	_layoutGrid3.SetCell( 0, 2, 1, 3, &_grid2 );
	_layoutGrid3.SetCell( 2, 0, 2, 3, &_lable5 );
	_layoutGrid3.SetCell( 3, 0, &_lable2 );
	_layoutGrid3.SetCell( 3, 1, &_but10 );
	_layoutGrid3.SetCell( 4, 0, &_lable3 );
	_layoutGrid3.SetCell( 4, 1, &_but11 );
	_layoutGrid3.SetCell( 4, 2, 5, 3, &_grid4 );
	_layoutGrid3.SetCell( 6, 0, &_lable4 );
	_layoutGrid3.SetCell( 6, 1, &_but12 );
	_layoutGrid3.SetCell( 6, 2, &_but13 );
	_layoutGrid3.SetCell( 7, 1, &_lable6 );
	_layoutGrid3.SetCell( 7, 2, &_edit1 );
	//
	_layoutGrid3.CalcGridPos();	

	_layoutGrid4.InitPoint( GenPoint()( 20, 20 ) );
	_layoutGrid4.SetRowColumn( 12, 8 );
	//
	_layoutGrid4.SetCell( 0, 0, 0, 5, &_lable21 );
	_layoutGrid4.SetCell( 1, 0, 8, 5, &_grid21 );
	_layoutGrid4.SetCell( 9, 0, 10, 1, &_but21 );
	_layoutGrid4.SetCell( 9, 2, 10, 3, &_but22 );
	_layoutGrid4.SetCell( 9, 4, 10, 5, &_but23 );
	_layoutGrid4.SetCell( 1, 6, 2, 7, &_but24 );
	_layoutGrid4.SetCell( 3, 6, 3, 7, &_edit21 );
	//_layoutGrid4.SetCell( 5, 6, 6, 7, &_but25 );
	_layoutGrid4.SetCell( 7, 6, 8, 7, &_but26 );
	//
	_layoutGrid4.CalcGridPos();
}

void UIOS::ShowSelDev()
{
	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
	{
		string str;
		STRING_FORMAT( str, "0x%08X", devInfo._singleDiscSN );
		_edit1.SetString( str );
	}
}

bool UIOS::ShowResistance( ULONG IBMON, ULONG IMPDMON, ULONG resistance )
{
	string str;
	bool rt = true;

	float IBMONLimit, IMPDMONLimit, resistanceLimit;
	//
	_grid3.GetCellString( 1, 3, str );
	IBMONLimit = ( float )atof( str.c_str() );
	//
	_grid3.GetCellString( 2, 3, str );
	IMPDMONLimit = ( float )atof( str.c_str() );
	//
	_grid3.GetCellString( 3, 3, str );
	resistanceLimit = ( float )atof( str.c_str() );

	float k1, k2;
	//
	_grid3.GetCellString( 1, 1, str );
	k1 = ( float )atof( str.c_str() );
	//
	_grid3.GetCellString( 2, 1, str );
	k2 = ( float )atof( str.c_str() );

	float IBMONTemp, IMPDMONTemp, resistanceTemp;
	IBMONTemp = IBMON*k1;
	IMPDMONTemp = IMPDMON*k2;
	resistanceTemp = 25+( resistance+1 )/1024.f*25000;

	STRING_FORMAT( str, "%0.2f", IBMONTemp );
	_grid3.SetCellString( 1, 2, str );
	if( IBMONTemp>IBMONLimit )
	{
		rt = false;
		_grid3.SetCellColor( 1, 2, _RED_ );
	}

	STRING_FORMAT( str, "%0.2f", IMPDMONTemp );
	_grid3.SetCellString( 2, 2, str );
	if( IMPDMONTemp>IMPDMONLimit )
	{
		rt = false;
		_grid3.SetCellColor( 2, 2, _RED_ );
	}
	
	STRING_FORMAT( str, "%0.2f", resistanceTemp );
	_grid3.SetCellString( 3, 2, str );
	if( resistanceTemp<resistanceLimit )
	{
		rt = false;
		_grid3.SetCellColor( 3, 2, _RED_ );
	}

	return rt;
}

void UIOS::ShowKB( float k, float b )
{
	string str;
	STRING_FORMAT( str, "%f", k );
	_grid2.SetCellString( 1, 1, str );

	STRING_FORMAT( str, "%f", b );
	_grid2.SetCellString( 2, 1, str );
}

void UIOS::ShowVerifyKB( float* readPM, float* referPM )
{
	string str;
	for( int i=0; i<3; ++i )
	{
		STRING_FORMAT( str, "%0.2f", readPM[i] );
		_grid4.SetCellString( i+1, 1, str );

		STRING_FORMAT( str, "%0.2f", referPM[i] );
		_grid4.SetCellString( i+1, 2, str );

		STRING_FORMAT( str, "%0.2f", readPM[i]-referPM[i] );
		_grid4.SetCellString( i+1, 3, str );
	}
}

void UIOS::ShowOSWaveLenList( UCHAR portNum, string* waveLenList )
{
	string str;
	STRING_FORMAT( str, "%d", portNum );
	_grid1.SetCellString( 1, 2, str, false );

	for( int i=0; i<portNum; ++i )
		_grid1.SetCellString( 2+i, 2, waveLenList[i], false );

	UIRefresh();
}

void UIOS::ShowOSOutPutPMList( UCHAR portNum, float* pmList )
{
	string str;
	STRING_FORMAT( str, "%d", portNum );
	_grid1.SetCellString( 1, 2, str, false );

	for( int i=0; i<portNum; ++i )
	{
		
		STRING_FORMAT( str, "%0.2f", pmList[i] );
		_grid1.SetCellString( 2+i, 1, str, false );
	}

	UIRefresh();
}

void UIOS::OnNotify( int id, int param )
{
	string str;

	if( id==ID_MAINBAR )
		_tabX1.SetCurCell( param-1 );
	else if( id==101 )
		OnGetSample();
	else if( id==102 )
		OnGetPM();
	else if( id==103 )
		OnOpenOS();
	else if( id==104 )
		OnCloseOS();
	else if( id==105 )
		OnGetOSWaveLen();
	else if( id==106 )
		OnSetOSWaveLen();
	else if( id==107 )
		OnGetOutPM();
	else if( id==108 )
		OnGetResistance();
	else if( id==109 )
		OnCalcKB();
	else if( id==110 )
		OnSetCalData();
	else if( id==111 )
		OnVerifyKB();
	else if( id==112 )
		OnSaveToOSDB();
	else if( id==113 )
		OnLoadFromOSDB();
	else if( id==121 )
		OnR1();
	else if( id==122 )
		OnR2();
	else if( id==21 )
		OnStartMonitor();
	else if( id==22 )
		OnStopMonitor();
	else if( id==23 )
		OnReadOSMaxMin();
	else if( id==24 )
		OnAddOS();
	else if( id==25 )
		OnDelOS();
	else if( id==26 )
		OnClearOSList();
}

void UIOS::OnGetSample()
{
	GetOSSampleOrPM( false, SELDEV_IP );
}

void UIOS::OnGetPM()
{
	GetOSSampleOrPM( true, SELDEV_IP );
}

void UIOS::OnOpenOS()
{
	SwitchOS( true, SELDEV_IP );	
}

void UIOS::OnCloseOS()
{
	SwitchOS( false, SELDEV_IP );	
}

void UIOS::OnGetOSWaveLen()
{
	for( UINT r=1; r<18; ++r )
		_grid1.SetCellString( r, 2, "" );

	GetOSWaveLen( SELDEV_IP  );
}

void UIOS::OnSetOSWaveLen()
{
	string str;
	string waveLen[16];
	
	_grid1.GetCellString( 1, 2, str );
	UINT portNum = atoi( str.c_str() );

	for( UINT i=0; i<portNum; ++i )
	{
		_grid2.GetCellString( 2+i, 2, waveLen[i] );
	}

	SetOSWaveLen( portNum, waveLen, SELDEV_IP );
}

void UIOS::OnGetOutPM()
{
	for( UINT r=1; r<18; ++r )
		_grid1.SetCellString( r, 1, "" );

	GetOSOutPutPM( SELDEV_IP );
}

void UIOS::OnGetResistance()
{
	USHORT IBMON, IMPDMON, resistanceLSB;
	if( SetOSResistanceSyn( 0x66, IBMON, IMPDMON, resistanceLSB, SELDEV_IP ) )
		ShowResistance( IBMON, IMPDMON, resistanceLSB );
	else
		View0( "读取电流、电阻失败.." );
}

void UIOS::OnCalcKB()
{
	OSLogic::GetSingleton()->StartCalcOSKBThread();
}

void UIOS::OnSetCalData()
{
	float k, b;
	int n;
	float pData[32];
	memset( pData, 0, 32 );

	string str;
	_grid2.GetCellString( 1, 1, str );
	k = ( float )atof( str.c_str() );
	// 
	_grid2.GetCellString( 2, 1, str );
	b = ( float )atof( str.c_str() );
	// 
	_grid1.GetCellString( 1, 2, str );
	n = atoi( str.c_str() );
	
	// 读取pin管功率值
	_grid1.GetCellString( 2, 1, str );
	float pm1 = ( float )atof( str.c_str() );

	for( int i=0; i<n; ++i )
	{	
		_grid1.GetCellString( 2+i, 1, str );
		pData[i] = pm1-( float )atof( str.c_str() );
	}

	SetOSCalData( k, b, n, pData, SELDEV_IP );
}

void UIOS::OnVerifyKB()
{
	_grid4.ClearAllCells();

	OSLogic::GetSingleton()->StartVerifyOSKBThread();
}

void UIOS::OnSaveToOSDB()
{
	float pm[16];
	float k=0, b=0, IBMON=0, IMPDMON=0, resistance=0;

	string str;
	for(int i=0; i<16; ++i)
	{
		_grid1.GetCellString( i+2, 1, str );
		pm[i] = (float)atof( str.c_str() );
	}
	//
	_grid2.GetCellString( 1, 1, str );
	k = (float)atof( str.c_str() );
	_grid2.GetCellString( 2, 1, str );
	b = (float)atof( str.c_str() );
	//
	_grid3.GetCellString( 1, 2, str );
	IBMON = (float)atof( str.c_str() );
	_grid3.GetCellString( 2, 2, str );
	IMPDMON = (float)atof( str.c_str() );
	_grid3.GetCellString( 3, 2, str );
	resistance = (float)atof( str.c_str() );

	DeviceInfo devInfo;
	ReadProductInfoSyn( SELDEV_IP, devInfo );

	bool rt = InsertDBOS( devInfo._singleDiscSN, devInfo._ipAddr, (UCHAR*)devInfo._mac, 
						  devInfo._softVer, devInfo._hardwareVer, pm, k, b, IBMON, IMPDMON, resistance);
	if( !rt )
		rt = UpdateDBOS( devInfo._singleDiscSN, devInfo._ipAddr, (UCHAR*)devInfo._mac, 
					   devInfo._softVer, devInfo._hardwareVer, pm, k, b, IBMON, IMPDMON, resistance);

	//
	if( rt==true )
		View0( "存入数据库成功.." );
	else
		View0( "存入数据库失败.." );
}

void UIOS::OnLoadFromOSDB()
{
	string str;
	_edit1.GetString( str );

	float pm[16];
	float k=0, b=0, IBMON=0, IMPDMON=0, resistance=0;
	if( !SelectDBOS( strtol( str.c_str(), 0, 16 ), pm, k, b, IBMON, IMPDMON, resistance ) )
	{
		View0( "读取数据失败.." );
		return;
	}

	for(int i=0; i<16; ++i)
	{
		STRING_FORMAT( str, "%0.2f", pm[i] );
		_grid1.SetCellString( i+2, 1, str );
	}

	//
	STRING_FORMAT( str, "%f", k );
	_grid2.SetCellString( 1, 1, str );
	STRING_FORMAT( str, "%f", b );
	_grid2.SetCellString( 2, 1, str );
	//
	STRING_FORMAT( str, "%0.2f", IBMON );
	_grid3.SetCellString( 1, 2, str );
	STRING_FORMAT( str, "%0.2f", IMPDMON );
	_grid3.SetCellString( 2, 2, str );
	STRING_FORMAT( str, "%0.2f", resistance );
	_grid3.SetCellString( 3, 2, str );

	View0( "读取数据成功.." );
}

void UIOS::OnR1()
{
	_grid3.SetCellString( 1, 1, OSLogic::GetSingleton()->k1[0] );
	_grid3.SetCellString( 2, 1, OSLogic::GetSingleton()->k1[1] );
}

void UIOS::OnR2()
{
	_grid3.SetCellString( 1, 1, OSLogic::GetSingleton()->k2[0] );
	_grid3.SetCellString( 2, 1, OSLogic::GetSingleton()->k2[1] );
}

void UIOS::OnStartMonitor()
{
	vector<ULONG> ipList;
	GetIPList(ipList);

	UCHAR monitorFlag;
	float max, min;
	for(UINT i=0; i<ipList.size(); ++i)
	{	
		bool rt = GetOSMaxMinPMSyn( 1, monitorFlag, max, min, ipList[i] );

		if( rt==false )
			_grid21.SetCellString( i+1, 2, "超时" );
		else if( monitorFlag==0 )
			_grid21.SetCellString( i+1, 2, "0 fail" );
		else
			_grid21.SetCellString( i+1, 2, "1 ok" );
	}
}

void UIOS::OnStopMonitor()
{
	vector<ULONG> ipList;
	GetIPList(ipList);

	UCHAR monitorFlag;
	float max, min;
	for(UINT i=0; i<ipList.size(); ++i)
	{
		bool rt = GetOSMaxMinPMSyn( 0, monitorFlag, max, min, ipList[i] );

		if( rt==false )
			_grid21.SetCellString( i+1, 2, "超时" );
		else if( monitorFlag==1 )
			_grid21.SetCellString( i+1, 2, "1 fail" );
		else
			_grid21.SetCellString( i+1, 2, "0 ok" );
	}
}

void UIOS::OnReadOSMaxMin()
{
	vector<ULONG> ipList;
	GetIPList(ipList);
	
	float max, min;
	UCHAR monitorFlag;
	for(UINT i=0; i<ipList.size(); ++i)
	{
		bool rt = GetOSMaxMinPMSyn( 2, monitorFlag, max, min, ipList[i] );
		if( rt==true )
		{
			if( monitorFlag==1 )
				_grid21.SetCellString( i+1, 2, "1" );
			else
				_grid21.SetCellString( i+1, 2, "0" );

			string str;
			STRING_FORMAT( str, "%0.2f", max );
			_grid21.SetCellString( i+1, 3, str );

			STRING_FORMAT( str, "%0.2f", min );
			_grid21.SetCellString( i+1, 4, str );
		}
		else
		{
			_grid21.SetCellString( i+1, 2,"超时" );
		}
	}
}

void UIOS::OnAddOS()
{
	int rowNum, columnNum;
	_grid21.GetRowColumn( rowNum, columnNum );

	string str;
	STRING_FORMAT( str, "%d", rowNum );

	string ipStr;
	_edit21.GetString( ipStr );

	VECTOR_STRING row;
	row.push_back( str );
	row.push_back( ipStr ); 
	row.push_back( "" );
	row.push_back( "" );
	row.push_back( "" );
		
	_grid21.AddRow( row );
}

void UIOS::OnDelOS()
{
	vector<UINT> selectRowList;
	_grid21.GetSelectRows( selectRowList );

	if( selectRowList.size()==0 )
		return;



	//_grid21.DelRow( selectRowList[0] );
}

void UIOS::OnClearOSList()
{
	_grid21.ClearUnfixRows();
}

void UIOS::GetIPList( VECTOR_ULONG& ipList )
{
	int rowNum, columnNum;
	_grid21.GetRowColumn( rowNum, columnNum );

	string str;
	int num = rowNum-1;
	for( int i=0; i<num; ++i )
	{
		_grid21.GetCellString( i+1, 1, str );
		ipList.push_back( IP4_StrToULONG()( str.c_str() ) );
	}
}