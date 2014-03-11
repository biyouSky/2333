#include "stdafx.h"
#include "UIInfo.h"

using namespace std;

UIWSSInfo gUIInfo;


void View0( const char* msg )
{
	gUIInfo.ShowMsg( msg );
}

void View1( int serialNumber )
{
	gUIInfo.ShowSerialNumber( serialNumber );
}

void View2( int type )
{
	gUIInfo.ShowType( type );
}

void View3( UCHAR hardVH, UCHAR hardVL, UCHAR firmVH, UCHAR firmVL )
{
	gUIInfo.ShowVersion( hardVH, hardVL, firmVH, firmVL );
}

void View4( UCHAR hardDateCodeH, UCHAR hardDateCodeL )
{
	gUIInfo.ShowHardDateCode( hardDateCodeH, hardDateCodeL );
}

void View5( UCHAR FirmwareDateCodeH, UCHAR FirmwareDateCodeL )
{
	gUIInfo.ShowFirmwareDateCode( FirmwareDateCodeH, FirmwareDateCodeL );
}

void View9( UCHAR d )
{
	gUIInfo.ShowAlarm( d );
}

void View10( USHORT vol )
{
	gUIInfo.ShowVoltage( vol );
}

void View11( float temp )
{
	gUIInfo.ShowTemperature( temp );
}

void View12( float temp )
{
	gUIInfo.ShowLowTemperature( temp );
}

void View13( float temp )
{
	gUIInfo.ShowHighTemperature( temp );
}

void View14( UCHAR d )
{
	//gUIInfo.ShowMemWear( d );
}

void View15( UCHAR d )
{
	gUIInfo.ShowManufactureID( d );
}




void UIWSSInfo::OnCreate()
{
	// view回调函数注册
	SetCB_View_ShowMsg( View0 );
	SetCB_View_ShowSerialNumber( View1 );
	SetCB_View_ShowType( View2 );
	SetCB_View_ShowVersion( View3 );
	SetCB_View_ShowHardDateCode( View4 );
	SetCB_View_ShowFirmwareDateCode( View5 );
	
	SetCB_View_ShowAlarm( View9 );
	SetCB_View_ShowVoltage( View10 );
	SetCB_View_ShowTemperature( View11 );
	SetCB_View_ShowLowTemperature( View12 );
	SetCB_View_ShowHighTemperature( View13 );
	SetCB_View_ShowMemWear( View14 );
	SetCB_View_ShowManufactureID( View15 );


	// 创建过程
	string str;

	_tabX1.Create( 0, this, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
#if _UICULT_
	_tabX1.SetCellNum( 1 );
	_tabX1.SetCell( 0, "Module", &_canvas1 );
#else
	_tabX1.SetCellNum( 2 );
	_tabX1.SetCell( 0, "Module", &_canvas1 );
	_tabX1.SetCell( 1, "其它", &_canvas2 );
#endif

	_canvas1.CreateDirect( &_tabX1, _NULLRECT_ );
	//
	_lable0.Create( 0, &_canvas1 );
	_lable0.SetString( "Serial Statu:" );
	_comBox0.Create( 20, &_canvas1 );
	for( int i=0; i<9; ++i )
	{
		STRING_FORMAT( str, "COM%d", i+1 );
		_comBox0.AddString( str );
	}
	_status0.Create( 0, &_canvas1 );
	_status0.SetImageIDAndDLL( 0 );
	//
	_lable1.Create( 0, &_canvas1 );
	_lable1.SetString( "SN:" );
	_edit1.Create( 0, &_canvas1 );
	_lable2.Create( 0, &_canvas1 );
	_lable2.SetString( "Type:" );
	_edit2.Create( 0, &_canvas1 );
	_lable3.Create( 0, &_canvas1 );
	_lable3.SetString( "Version:" );
	_edit3.Create( 0, &_canvas1 );
	_lable4.Create( 0, &_canvas1 );
	_lable4.SetString( "Hardware Date:" );
	_edit4.Create( 0, &_canvas1 );
	_lable5.Create( 0, &_canvas1 );
	_lable5.SetString( "Firmware Date:" );
	_edit5.Create( 0, &_canvas1 );
	_lable6.Create( 0, &_canvas1 );
	_lable6.SetString( "Manufacturer ID:" );
	_edit6.Create( 0, &_canvas1 );
	_but1.Create( 1, &_canvas1 );
	_but1.SetString( "Read Module Infomation" );
	//
	_lable7.Create( 0, &_canvas1 );
	_lable7.SetString( "Voltage:" );
	_edit7.Create( 0, &_canvas1 );
	_lable8.Create( 0, &_canvas1 );
	_lable8.SetString( "Temperature:" );
	_edit8.Create( 0, &_canvas1 );
	_lable9.Create( 0, &_canvas1 );
	_lable9.SetString( "Warning:" );
	_edit9.Create( 0, &_canvas1 );
	_lable10.Create( 0, &_canvas1 );
	_lable10.SetString( "Low Temperature Threshold:" );
	_edit10.Create( 0, &_canvas1 );
	_lable11.Create( 0, &_canvas1 );
	_lable11.SetString( "High Temperature Threshold:" );
	_edit11.Create( 0, &_canvas1 );
	_but2.Create( 2, &_canvas1 );
	_but2.SetString( "Read Warning Infomation" );
#if _UICULT_
#else
	_but10.Create( 10, &_canvas1 );
	_but10.SetString( "Set Threshold" );
#endif

	_canvas2.CreateDirect( &_tabX1, _NULLRECT_ );
	//
	_lable12.Create( 0, &_canvas2 );
	_lable12.SetString( "固件升级:" );
	_but3.Create( 3, &_canvas2 );
	_but3.SetString( "开始升级" );
	_but4.Create( 4, &_canvas2 );
	_but4.SetString( "加载固件" );
	_but5.Create( 5, &_canvas2 );
	_but5.SetString( "切换固件" );
	//
	_lable13.Create( 0, &_canvas2 );
	_lable13.SetString( "模块信息:" );
	_but6.Create( 6, &_canvas2 );
	_but6.SetString( "设置" );
	_grid1.Create( 0, &_canvas2 );
	_grid1.SetRowColumn( 5, 2 );
	_grid1.SetRowFix();
	_grid1.SetColumnFix();
	_grid1.SetCellString( 0, 0, "项" );
	_grid1.SetCellString( 0, 1, "值" );
	_grid1.SetCellString( 1, 0, "SN(0x)" );
	_grid1.SetCellString( 2, 0, "Type" );
	_grid1.SetCellString( 2, 1, "4" );
	_grid1.SetCellString( 3, 0, "硬件版本" );
	_grid1.SetCellString( 4, 0, "硬件日期(Y W)" );
	TimeFunc curTime;
	STRING_FORMAT( str, "%d %d", curTime.GetYear()%100, curTime.GetYearDay()/7+1 );
	_grid1.SetCellString( 4, 1, str );
	_grid1.SetColumnWidth( 0, 120 );
	_grid1.SetColumnWidth( 1, 160 );
	for( int i=0; i<5; ++i )
		_grid1.SetRowHeight( i, 30 );
	//
	_lable14.Create( 0, &_canvas2 );
	_lable14.SetString( "定标温度:" );
	_but7.Create( 7, &_canvas2 );
	_but7.SetString( "设置" );
	_grid2.Create( 0, &_canvas2 );
	_grid2.SetRowColumn( 6, 2 );
	_grid2.SetRowFix();
	_grid2.SetColumnFix();
	_grid2.SetCellString( 0, 0, "项" );
	_grid2.SetCellString( 0, 1, "值" );
	_grid2.SetCellString( 1, 0, "载入配置文件" );
	_grid2.SetCellString( 1, 1, "1" );
	_grid2.SetCellString( 2, 0, "温度变化插值" );
	_grid2.SetCellString( 2, 1, "1" );
	_grid2.SetCellString( 3, 0, "定标温度点个数" );
	_grid2.SetCellString( 4, 0, "定标最低温度点" );
	_grid2.SetCellString( 5, 0, "定标最高温度点" );
	_grid2.SetColumnWidth( 0, 120 );
	_grid2.SetColumnWidth( 1, 160 );
	for( int i=0; i<6; ++i )
		_grid2.SetRowHeight( i, 30 );
	//
	_lable15.Create( 0, &_canvas2 );
	_lable15.SetString( "定标限压值以及采集范围设置:" );
	_but8.Create( 8, &_canvas2 );
	_but8.SetString( "限压值" );
	_but9.Create( 9, &_canvas2 );
	_but9.SetString( "采集范围" );

	// 设置布局信息
	_layoutGrid0.InitPoint( GenPoint()( 70, 50 ) );
	_layoutGrid0.SetRowColumn( 1, 3 );
	_layoutGrid0.SetColumnWidthInterval( 0, 125 );
	_layoutGrid0.SetColumnWidthInterval( 1, 130 );
	_layoutGrid0.SetCell( 0, 0, &_lable0 );
	_layoutGrid0.SetCell( 0, 1, &_comBox0 );
	_layoutGrid0.SetCell( 0, 2, &_status0 );
	_layoutGrid0.CalcGridPos();

	_layoutGrid1.InitPoint( GenPoint()( 70, 100 ) );
	_layoutGrid1.SetRowColumn( 7, 2 );
	_layoutGrid1.SetColumnWidthInterval( 0, 125 );
	_layoutGrid1.SetColumnWidthInterval( 1, 130 );
	_layoutGrid1.SetRowHeightInterval( 6, 50, 0 );
	_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_edit1 );
	_layoutGrid1.SetCell( 1, 0, &_lable2 );
	_layoutGrid1.SetCell( 1, 1, &_edit2 );
	_layoutGrid1.SetCell( 2, 0, &_lable3 );
	_layoutGrid1.SetCell( 2, 1, &_edit3 );
	_layoutGrid1.SetCell( 3, 0, &_lable4 );
	_layoutGrid1.SetCell( 3, 1, &_edit4 );
	_layoutGrid1.SetCell( 4, 0, &_lable5 );
	_layoutGrid1.SetCell( 4, 1, &_edit5 );
	_layoutGrid1.SetCell( 5, 0, &_lable6 );
	_layoutGrid1.SetCell( 5, 1, &_edit6 );
	_layoutGrid1.SetCell( 6, 0, 6, 1, &_but1 );
	_layoutGrid1.CalcGridPos();

	_layoutGrid2.InitPoint( GenPoint()( 400, 100 ) );
	_layoutGrid2.SetRowColumn( 7, 2 );
	_layoutGrid2.SetColumnWidthInterval( 0, 215 );
	_layoutGrid2.SetColumnWidthInterval( 1, 220 );
	_layoutGrid2.SetRowHeightInterval( 6, 50, 0 );
	_layoutGrid2.SetCell( 0, 0, &_lable7 );
	_layoutGrid2.SetCell( 0, 1, &_edit7 );
	_layoutGrid2.SetCell( 1, 0, &_lable8 );
	_layoutGrid2.SetCell( 1, 1, &_edit8 );
	_layoutGrid2.SetCell( 2, 0, &_lable9 );
	_layoutGrid2.SetCell( 2, 1, &_edit9 );
	_layoutGrid2.SetCell( 3, 0, &_lable10 );
	_layoutGrid2.SetCell( 3, 1, &_edit10 );
	_layoutGrid2.SetCell( 4, 0, &_lable11 );
	_layoutGrid2.SetCell( 4, 1, &_edit11 );
	_layoutGrid2.SetCell( 6, 0, 6, 1, &_but2 );
	_layoutGrid2.SetCell( 5, 1, &_but10 );
	_layoutGrid2.CalcGridPos();

	_layoutGrid3.InitPoint( GenPoint()( 20, 20 ) );
	_layoutGrid3.SetRowColumn( 5, 4 );
	//
	_layoutGrid3.SetCell( 0, 0, &_lable12 );
	_layoutGrid3.SetCell( 0, 1, &_but3 );
	_layoutGrid3.SetCell( 0, 2, &_but4 );
	_layoutGrid3.SetCell( 0, 3, &_but5 );
	_layoutGrid3.SetCell( 1, 0, &_lable13 );
	_layoutGrid3.SetCell( 1, 1, &_but6 );
	_layoutGrid3.SetCell( 2, 1, 2, 3, &_grid1 );
	_layoutGrid3.SetRowHeightInterval( 2, 160 );
	_layoutGrid3.SetCell( 3, 0, &_lable14 );
	_layoutGrid3.SetCell( 3, 1, &_but7 );
	_layoutGrid3.SetCell( 4, 1, 4, 3, &_grid2 );
	_layoutGrid3.SetRowHeightInterval( 4, 190 );
	//
	_layoutGrid3.CalcGridPos();

	_layoutGrid4.InitPoint( GenPoint()( 560, 20 ) );
	_layoutGrid4.SetRowColumn( 3, 2 );
	//
	_layoutGrid4.SetCell( 0, 0, 0, 1, &_lable15 );
	_layoutGrid4.SetCell( 1, 0, &_but8 );
	_layoutGrid4.SetCell( 1, 1, &_but9 );
	_layoutGrid4.SetColumnWidthInterval( 0, 120, 30 );
	_layoutGrid4.SetColumnWidthInterval( 1, 120, 30 );
	_layoutGrid4.SetRowHeightInterval( 1, 40 );
	//
	_layoutGrid4.CalcGridPos();
}

void UIWSSInfo::OnMainBar( int param )
{
	_tabX1.SetCurCell( param-1 );
}

void UIWSSInfo::OnReadModuleInfo( int param )
{
	_edit1.SetString( "" );
	_edit2.SetString( "" );
	_edit3.SetString( "" );
	_edit4.SetString( "" );
	_edit5.SetString( "" );
	_edit6.SetString( "" );

	if( GetCommMethod()==0 )
	{	
		// 串口
		if( IsSerialOpen()==false )
		{
			::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
			return;
		}

		GetSerialNumber_Com();
		GetType_Com();
		GetVersion_Com();
		GetHardwareDateCode_Com();
		GetFirmwareDateCode_Com();
		GetManufactureID_Com();
	}
	else
	{	// 网口

		ULONG ip = GetWSSIP();

		GetSerialNumber_Net( ip );
		GetType_Net( ip );
		GetVersion_Net( ip );
		GetHardwareDateCode_Net( ip );
		GetFirmwareDateCode_Net( ip );
		GetManufactureID_Net( ip );
	}
}

void UIWSSInfo::OnReadWarningInfo( int param )
{
	_edit7.SetString( "" );
	_edit8.SetString( "" );
	_edit9.SetString( "" );
	_edit10.SetString( "" );
	_edit11.SetString( "" );

	if( GetCommMethod()==0 )
	{	// 串口
		if( IsSerialOpen()==false )
		{
			::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
			return;
		}

		GetVoltage_Com();
		GetTemperature_Com();
		GetAlarm_Com();
		GetLowTemperatureAlarmThreshold_Com();
		GetHighTemperatureAlarmThreshold_Com();
	}
	else
	{	// 网口

		ULONG ip = GetWSSIP();

		GetVoltage_Net( ip );
		GetTemperature_Net( ip );
		GetAlarm_Net( ip );
		GetLowTemperatureAlarmThreshold_Net( ip );
		GetHighTemperatureAlarmThreshold_Net( ip );
	}
}

void UIWSSInfo::OnBeginLoad( int param )
{
	StartFirmwareLoading_Com();
}

void UIWSSInfo::OnLoadingNow( int param )
{
	StartLoadNewFirmwareProcess();
}

void UIWSSInfo::OnSwitchNew( int param )
{
	SwitchtoNewFirmware_Com();
}

void UIWSSInfo::OnSetModuleInfo1( int param )
{
	string str;
	_grid1.GetCellString( 1, 1, str );
	str = "0x"+str;
	ULONG sn = (ULONG)strtol( str.c_str(), 0, 16 );
	//
	_grid1.GetCellString( 2, 1, str );
	UCHAR type = (UCHAR)atoi( str.c_str() );
	//
	_grid1.GetCellString( 3, 1, str );
	float hv = (float)atof( str.c_str() );
	UCHAR hv1 = (UCHAR)hv;
	UCHAR hv2 = (UCHAR)( (ULONG)( hv*100 )%100 );
	//
	_grid1.GetCellString( 4, 1, str );
	UCHAR hd1=0, hd2=0;
	VECTOR_STRING wordList;
	SplitWords( str, wordList );
	if( wordList.size()==2 )
	{
		hd1 = (UCHAR)atoi( wordList[0].c_str() );
		hd2 = (UCHAR)atoi( wordList[1].c_str() );
	}

	if( GetCommMethod()==0 )
		SetModuleInfo1_Com( sn, type, hv1, hv2, hd1, hd2 );
	else
		SetModuleInfo1_Net( GetWSSIP(), sn, type, hv1, hv2, hd1, hd2 );
}

void UIWSSInfo::OnSetModuleInfo2( int param )
{
	string str;
	_grid2.GetCellString( 1, 1, str );
	UCHAR flag1 = (UCHAR)atoi( str.c_str() );
	//
	_grid2.GetCellString( 2, 1, str );
	UCHAR flag2 = (UCHAR)atoi( str.c_str() );
	//
	_grid2.GetCellString( 3, 1, str );
	UCHAR tempNum = (UCHAR)atoi( str.c_str() );
	//
	_grid2.GetCellString( 4, 1, str );
	char temp = (char)atoi( str.c_str() );
	UCHAR lowTemp;
	memcpy( &lowTemp, &temp, 1 );
	//
	_grid2.GetCellString( 5, 1, str );
	temp = (char)atoi( str.c_str() );
	UCHAR highTemp;
	memcpy( &highTemp, &temp, 1 );

	if( GetCommMethod()==0 )
		SetModuleInfo2_Com( flag1, flag2, tempNum, lowTemp, highTemp );
	else
		SetModuleInfo2_Net( GetWSSIP(), flag1, flag2, tempNum, lowTemp, highTemp );
}

void UIWSSInfo::OnSetModuleInfo3( int param )
{
	string str;
	OpenFolderFile( str );

	TxtOBJ txt;
	if( !txt.OpenFile( str ) ) return;

	vector<VECTOR_STRING > allLineWordsList;
	txt.ReadAllLineWords( allLineWordsList );
	if( allLineWordsList.size()<(UINT)CHANNEL_NUM )
	{
FLAG:
		MessageBoxA( NULL, "文件格式不符..", "warning", 0 );
		return;
	}

	USHORT volList[104];
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		VECTOR_STRING& line = allLineWordsList[c];
		if( line.size()<3 ) goto FLAG;

		volList[c-1] = (USHORT)atoi( line[1].c_str() );
	}

	if( GetCommMethod()==0 )
		SetModuleInfo3_Com( volList );
	else
		SetModuleInfo3_Net( GetWSSIP(), volList );
}

void UIWSSInfo::OnSetModuleInfo4( int param )
{
	StartSetCalibrateVoltageRangeThread();
}

void UIWSSInfo::OnSetTemperatureThreshold( int param )
{
	if( GetCommMethod()==0 && IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
		return;
	}

	string str;
	_edit10.GetString( str );
	float lowTemp = (float)atof( str.c_str() );
	_edit11.GetString( str );
	float highTemp = (float)atof( str.c_str() );

	if( GetCommMethod()==0 )
	{
		SetLowTemperatureAlarmThreshold_Com( lowTemp );
		SetHighTemperatureAlarmThreshold_Com( highTemp );
	}
	else
	{
		SetLowTemperatureAlarmThreshold_Net( GetWSSIP(), lowTemp );
		SetHighTemperatureAlarmThreshold_Net( GetWSSIP(), highTemp );
	}
}

void UIWSSInfo::OnConnectSerial( int param )
{
	string str;
	if( OpenSerial( param+1, 115200 )==true )
	{
		_status0.SetImageIDAndDLL( IDB_OK );
		_status0.PlayAnimateHitDrum();
		STRING_FORMAT( str, "Serial Status: Success to connect COM%d..", param+1 );	
	}
	else
	{
		_status0.SetImageIDAndDLL( IDB_CLOSE );
		_status0.PlayAnimateHitDrum();
		STRING_FORMAT( str, "Serial Status: Fail to connect COM%d..", param+1 );
	}

	ShowMsg( str );
}

void UIWSSInfo::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
		OnMainBar( param );
	else if( id==1 )
		OnReadModuleInfo( param );
	else if( id==2 )
		OnReadWarningInfo( param );
	else if( id==3 )
		OnBeginLoad( param );
	else if( id==4 )
		OnLoadingNow( param );
	else if( id==5 )
		OnSwitchNew( param );
	else if( id==6 )
		OnSetModuleInfo1( param );
	else if( id==7 )
		OnSetModuleInfo2( param );
	else if( id==8 )
		OnSetModuleInfo3( param );
	else if( id==9 )
		OnSetModuleInfo4( param );
	else if( id==10 )
		OnSetTemperatureThreshold( param );
	else if( id==20 )
		OnConnectSerial( param );
}


void UIWSSInfo::ShowMsg( const string msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void UIWSSInfo::ShowSerialNumber( int serialNumber )
{
	string str;
	STRING_FORMAT( str, "0x%X", serialNumber );

	_edit1.SetString( str );
}

void UIWSSInfo::ShowType( int type )
{
	string str;
	if( type==1 )
		str = "WB";
	else if( type==2 )
		str ="DCE";
	else if( type==3 )
		str = "DSE";
	else if( type==4 )
		str = "WSS";

	_edit2.SetString( str );
}

void UIWSSInfo::ShowVersion( UCHAR hardVH, UCHAR hardVL, UCHAR firmVH, UCHAR firmVL )
{
	string str;
	STRING_FORMAT( str, "HV:%d.%d FV:%d.%d", hardVH, hardVL, firmVH, firmVL );

	_edit3.SetString( str );
}

void UIWSSInfo::ShowHardDateCode( UCHAR hardDateCodeH, UCHAR hardDateCodeL )
{
	string str;
	if( hardDateCodeH<10 )
		STRING_FORMAT( str, "0%dY %dW", hardDateCodeH, hardDateCodeL );
	else
		STRING_FORMAT( str, "%dY %dW", hardDateCodeH, hardDateCodeL );

	_edit4.SetString( str );
}

void UIWSSInfo::ShowFirmwareDateCode( UCHAR FirmwareDateCodeH, UCHAR FirmwareDateCodeL )
{
	string str;
	if( FirmwareDateCodeH<10 )
		STRING_FORMAT( str, "0%dY %dW", FirmwareDateCodeH, FirmwareDateCodeL );
	else
		STRING_FORMAT( str, "%dY %dW", FirmwareDateCodeH, FirmwareDateCodeL );

	_edit5.SetString( str );
}

void UIWSSInfo::ShowManufactureID( UCHAR d )
{
	string str;
	STRING_FORMAT( str, "%d", d );

	_edit6.SetString( str );
}

void UIWSSInfo::ShowAlarm( UCHAR d )
{
	char voltageAlarm = d&0x01;
	char temperAlarm = d&0x02;

	string str;
	if( voltageAlarm!=0 )
		str = "voltage out the range";
	else 
		str = "voltage in the range";

	if( temperAlarm!=0 )
		str += " && temperature out the range";
	else 
		str += " && temperature in the range";

	_edit9.SetString( str );
}

void UIWSSInfo::ShowVoltage( USHORT vol )
{
	string str;
	STRING_FORMAT( str, "%d(mv)", vol );

	_edit7.SetString( str );
}

void UIWSSInfo::ShowTemperature( float temp )
{
	string str;
	STRING_FORMAT( str, "%0.1f(℃)", temp );

	_edit8.SetString( str );
}

void UIWSSInfo::ShowLowTemperature( float temp )
{
	string str;
	STRING_FORMAT( str, "%0.1f(℃)", temp );

	_edit10.SetString( str );
}

void UIWSSInfo::ShowHighTemperature( float temp )
{
	string str;
	STRING_FORMAT( str, "%0.1f(℃)", temp );

	_edit11.SetString( str );
}


bool UIWSSInfo::StartSetCalibrateVoltageRangeThread()
{
	return StartThread( "LoadVoltageFile", &UIWSSInfo::SetCalibrateVoltageRangeThread );
}

void UIWSSInfo::SetCalibrateVoltageRangeThread()
{
	// 检查串口状态
	if( GetCommMethod()==0 && IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
		return;
	}

	string str;
	OpenFolderFile( str );

	TxtOBJ txt;
	if( !txt.OpenFile( str ) ) return;

	vector<VECTOR_STRING > allLineWordsList;
	txt.ReadAllLineWords( allLineWordsList );
	if( allLineWordsList.size()<(UINT)CHANNEL_NUM )
	{
FLAG:
		MessageBoxA( NULL, "文件格式不符..", "warning", 0 );
		return;
	}

	USHORT volPortList[18][104];
	int port = PORT_NUM;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		VECTOR_STRING& line = allLineWordsList[c];
		if( (int)line.size()<1+port*2 ) goto FLAG;

		for( int p=1; p<=port*2; ++p )
			volPortList[p-1][c-1] = (USHORT)atoi( line[p].c_str() );
	}
	ShowMsg( "读取文件完毕.." );

	for( int p=1; p<=port; ++p )
	{
		for( int f=1; f<=2; ++f )
		{
			STRING_FORMAT( str, "设置port%d%s电压..", p, f==1?"开始":"结束" );
			ShowMsg( str );

			if( GetCommMethod()==0 )
				SetModuleInfo4_Com( p, f, volPortList[2*( p-1 )+f-1] );
			else
				SetModuleInfo4_Net( GetWSSIP(), p, f, volPortList[2*( p-1 )+f-1] );
		}
	}
}