
#include "stdafx.h"
#include "UIOPM.h"

UIOPM gUIOPM;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void View4( UCHAR pmType, UCHAR beginPort, UCHAR endPort, ULONG* pmList )
{
	gUIOPM.ShowOPMPMList( pmType, beginPort, endPort, pmList );
}

void View101( vector<pair<float, float> >& exceptionPointList, UCHAR channel )
{

}

void View102()
{
	gUIOPM.ShowDXPM();
}

void UIOPM::ShowDXPM()
{
	OPMConfig& config = OPMLogic::GetSingleton()->_configInfor;
	
	string str;
	for( UINT i=0; i<config._channelList.size(); ++i )
	{
		STRING_FORMAT( str, "%0.02f", config._dxPM[config._channelList[i]] );
		_grid22.SetCellString( config._channelList[i]+1, 1, str );
	}
}

void UIOPM::ShowVerifyResult( vector<pair<float, float> >& exceptionPointList, UCHAR channel )
{
	_grid24.ClearUnfixRows();

	string str;
	for( UINT i=0; i<exceptionPointList.size(); ++i )
	{
		VECTOR_STRING row;

		STRING_FORMAT( str, "%d", i+1 );
		row.push_back( str );
		STRING_FORMAT( str, "%0.2f", exceptionPointList[i].first );
		row.push_back( str );
		STRING_FORMAT( str, "%0.2f", exceptionPointList[i].second );
		row.push_back( str );
		STRING_FORMAT( str, "%0.2f", exceptionPointList[i].second-exceptionPointList[i].first );
		row.push_back( str );

		_grid24.AddRow( row );
	}
}

void UIOPM::ShowOPMPMList( UCHAR pmType, UCHAR beginPort, UCHAR endPort, ULONG* pmList )
{
	_combo1.SetSelectIndex( pmType-1 );

	for(UCHAR i=beginPort; i<=endPort; ++i)
	{
		UCHAR *pV = reinterpret_cast<UCHAR*>( pmList+i-beginPort );

		// 
		short pm;
		memcpy(&pm, pV+2, 2);
		pm = (short)ntohs( pm );
		// 
		string str;
		STRING_FORMAT( str, "%0.2lf",  static_cast<int>( pm )/100.0 );
		_grid1.SetCellString( i, 2, str );

		UCHAR waveFlag = *pV;
		if( waveFlag==1 )
			_grid1.SetCellString( i, 3, "1625" );
		else if( waveFlag==2 )
			_grid1.SetCellString( i, 3, "1550" );
		else if( waveFlag==3 )
			_grid1.SetCellString( i, 3, "1310" );
	}
}

void UIOPM::ShowSelDev()
{
	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
	{
		string str;
		STRING_FORMAT( str, "0x%08X", devInfo._singleDiscSN );
		_edit22.SetString( str );
	}
}

void UIOPM::OnCreate()
{
	PCB_View_ShowMsg = View0;
	PCB_View_ShowVerifyResult = View101;
	PCB_View_ShowDXPM = View102;

	//
	string str;

	_tabX1.Create( 0, this, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 2 );
	_tabX1.SetCell( 0, "定标", &_canvas1 );
	_tabX1.SetCell( 1, "监视", &_canvas2 );
	_tabX1.SetCurCell( 0 );

	{
		_canvas1.CreateDirect( &_tabX1, _clientRC );

		_combo1.Create( 0, &_canvas1 );
		_combo1.AddString( "光功率计" );
		_combo1.AddString( "光源" );
		_combo1.AddString( "光功率计+光源" );
		_combo1.SetSelectIndex( 0 );

		_but1.Create( 1, &_canvas1 );
		_but1.SetString( "设置端口数" );
		_but2.Create( 2, &_canvas1 );
		_but2.SetString( "获取功率值" );
		_but3.Create( 3, &_canvas1 );
		_but3.SetString( "设置测试波长" );

		_lable1.Create( 0, &_canvas1 );
		_lable1.SetString( "功率计类型：" );

		_edit1.Create( 0, &_canvas1 );

		_grid1.Create( 0, &_canvas1, GenRect()( 50, 20, 400, _clientRC.bottom-20 ), UILayoutCalc::SIZE_Y );
		_grid1.SetRowColumn( 33, 4 );
		_grid1.SetRowFix();
		_grid1.SetColumnFix();
		_grid1.SetColumnWidth( 1, 85 );
		_grid1.SetColumnWidth( 2, 85 );
		_grid1.SetColumnWidth( 3, 85 );
		_grid1.SetCellString( 0, 0, "端口号" );
		_grid1.SetCellString( 0, 1, "保留" );
		_grid1.SetCellString( 0, 2, "功率值" );
		_grid1.SetCellString( 0, 3, "波长(nm)" );
		for( int r=1; r<=32; ++r )
		{
			STRING_FORMAT( str, "%d", r );
			_grid1.SetCellString( r, 0, str );
		}

		_layoutGrid1.InitPoint( GenPoint()( 420, 100 ) );
		_layoutGrid1.SetRowColumn( 10, 4 );
		_layoutGrid1.SetRowHeightInterval( 0, 50 );
		_layoutGrid1.SetRowHeightInterval( 1, 50 );
		_layoutGrid1.SetRowHeightInterval( 2, 50 );
		_layoutGrid1.SetColumnWidthInterval( 0, 150 );
		_layoutGrid1.SetColumnWidthInterval( 1, 150 );
		//
		_layoutGrid1.SetCell( 0, 0, &_but1 );
		_layoutGrid1.SetCell( 0, 1, &_edit1 );
		_layoutGrid1.SetCell( 2, 0, &_lable1 );
		_layoutGrid1.SetCell( 2, 1, &_combo1 );
		_layoutGrid1.SetCell( 1, 0, &_but2 );
		_layoutGrid1.SetCell( 1, 1, &_but3 );
		//
		_layoutGrid1.CalcGridPos();
	}

	{
		_canvas2.CreateDirect( &_tabX1, _clientRC );

		_lable21.Create( 0, &_canvas2 );
		_lable21.SetString( "VOA串口号：" );
		//
		_combo21.Create( 100, &_canvas2 );
		for( int i=1; i<9; ++i )
		{
			STRING_FORMAT( str, "COM%d", i );
			_combo21.AddString( str.c_str() );
		}

		_lable22.Create( 0, &_canvas2 );
		_lable22.SetString( "参考OPM地址：" );
		_edit21.Create( 0, &_canvas2 );
		_edit21.SetString( "9" );

		_lable23.Create( 0, &_canvas2 );
		_lable23.SetString( "波长选择：" );
		//
		_combo22.Create( 0, &_canvas2 );
		_combo22.AddString( "1310" );
		_combo22.AddString( "1550" );
		_combo22.AddString( "1625" );
		_combo22.SetSelectIndex( 1 );

		_lable24.Create( 0, &_canvas2 );
		_lable24.SetString( "设备SN号:" );
		_edit22.Create( 0, &_canvas2 );

		_grid21.Create( 0, &_canvas2 );
		_grid21.SetRowColumn( 4, 2 );
		_grid21.SetRowFix();
		_grid21.SetColumnFix();
		_grid21.SetColumnWidth( 0, 95 );
		_grid21.SetColumnWidth( 1, 95 );
		_grid21.SetRowHeight( 0, 25 );
		_grid21.SetRowHeight( 1, 25 );
		_grid21.SetRowHeight( 2, 25 );
		_grid21.SetRowHeight( 3, 25 );
		_grid21.SetCellString( 0, 0, "验证区间" );
		_grid21.SetCellString( 0, 1, "验证精度(db)" );
		_grid21.SetCellString( 1, 0, "[0,-50]" );
		_grid21.SetCellString( 2, 0, "(-50,-65]" );
		_grid21.SetCellString( 1, 1, "0.2" );
		_grid21.SetCellString( 2, 1, "0.4" );
		_grid21.SetCellString( 3, 0, "验证步进" );
		_grid21.SetCellString( 3, 1, "5" );

		_but21.Create( 21, &_canvas2 );
		_but21.SetString( "通道无光初始化" );
		_but22.Create( 22, &_canvas2 );
		_but22.SetString( "采集并检验" );
		_but23.Create( 23, &_canvas2 );
		_but23.SetString( "单通道检查" );
		_but25.Create( 25, &_canvas2 );
		_but25.SetString( "载入数据" );
		_but26.Create( 26, &_canvas2 );
		_but26.SetString( "监测PM" );

		_lable25.Create( 0, &_canvas2 );
		_lable25.SetString( "填写器件功率即可.." );
		_but24.Create( 24, &_canvas2 );
		_but24.SetString( "自动计算差值" );

		_grid22.Create( 200, &_canvas2 );
		_grid22.SetRowColumn( 17, 2 );
		_grid22.SetRowFix();
		_grid22.SetColumnFix();
		_grid22.SetColumnWidth( 1, 110 );
		_grid22.SetCellString( 0, 0, "通道" );
		_grid22.SetCellString( 0, 1, "dxPM(标准-器件)" );
		for( int i=1; i<=16; ++i )
		{
			STRING_FORMAT( str, "%d", i );
			_grid22.SetCellString( i, 0, str );
		}

		_grid23.Create( 0, &_canvas2 );
		_grid23.SetRowColumn( 6, 2 );
		_grid23.SetRowFix();
		_grid23.SetColumnFix();
		_grid22.SetColumnWidth( 1, 110 );
		_grid23.SetCellString( 0, 0, "档位" );
		_grid23.SetCellString( 0, 1, "dx" );
		_grid23.SetCellString( 1, 0, "1" );
		_grid23.SetCellString( 2, 0, "2" );
		_grid23.SetCellString( 3, 0, "3" );
		_grid23.SetCellString( 4, 0, "4" );
		_grid23.SetCellString( 5, 0, "偏移" );

		_grid24.Create( 0, &_canvas2 );
		_grid24.SetRowColumn( 1, 4 );
		_grid24.SetRowFix();
		_grid24.SetColumnWidth( 1, 90 );
		_grid24.SetColumnWidth( 2, 90 );
		_grid24.SetColumnWidth( 3, 90 );
		_grid24.SetCellString( 0, 0, "序号" );
		_grid24.SetCellString( 0, 1, "参考PM" );
		_grid24.SetCellString( 0, 2, "器件PM" );
		_grid24.SetCellString( 0, 3, "差值" );

		_layoutGrid2.InitPoint( GenPoint()( 20, 20 ) );
		_layoutGrid2.SetRowColumn( 10, 4 );
		/*_layoutGrid2.SetRowHeightInterval( 0, 50 );
		_layoutGrid2.SetRowHeightInterval( 1, 50 );
		_layoutGrid2.SetRowHeightInterval( 2, 50 );*/
		_layoutGrid2.SetColumnWidthInterval( 0, 120 );
		_layoutGrid2.SetColumnWidthInterval( 1, 150 );
		//
		_layoutGrid2.SetCell( 0, 0, &_lable21 );
		_layoutGrid2.SetCell( 0, 1, &_combo21 );
		_layoutGrid2.SetCell( 1, 0, &_lable22 );
		_layoutGrid2.SetCell( 1, 1, &_edit21 );
		_layoutGrid2.SetCell( 2, 0, &_lable23 );
		_layoutGrid2.SetCell( 2, 1, &_combo22 );
		_layoutGrid2.SetCell( 3, 0, 5, 1, &_grid21 );
		//
		_layoutGrid2.CalcGridPos();


		_layoutGrid3.InitPoint( GenPoint()( 380, 20 ) );
		_layoutGrid3.SetRowColumn( 15, 4 );
		_layoutGrid3.SetColumnWidthInterval( 0, 180 );
		//
		_layoutGrid3.SetCell( 0, 0, 7, 0, &_grid22 );
		_layoutGrid3.SetCell( 8, 0, 10, 0, &_grid23 );
		//
		_layoutGrid3.CalcGridPos();

		_layoutGrid4.InitPoint( GenPoint()( 620, 20 ) );
		_layoutGrid4.SetRowColumn( 10, 4 );
		for( int i=0; i<10; ++i )	
			_layoutGrid4.SetRowHeightInterval( i, 50 );
		_layoutGrid4.SetColumnWidthInterval( 0, 155 );
		_layoutGrid4.SetColumnWidthInterval( 1, 155 );
		//
		_layoutGrid4.SetCell( 0, 0, &_lable25 );
		_layoutGrid4.SetCell( 0, 1, &_but24 );
		_layoutGrid4.SetCell( 1, 0, &_but21 );
		_layoutGrid4.SetCell( 1, 1, &_but26 );
		_layoutGrid4.SetCell( 2, 0, &_but22 );
		_layoutGrid4.SetCell( 2, 1, &_but23 );
		_layoutGrid4.SetCell( 3, 0, 6, 1, &_grid24 );
		_layoutGrid4.SetCell( 7, 0, &_lable24 );
		_layoutGrid4.SetCell( 7, 1, &_edit22 );
		_layoutGrid4.SetCell( 8, 0, &_but25 );
		//
		_layoutGrid4.CalcGridPos();
	}
}

void UIOPM::GetConfigData()
{
	View0( "获取配置信息.." );

	string str;

	OPMConfig& config = OPMLogic::GetSingleton()->_configInfor;

	_edit21.GetString( str );
	config._pmGPIBAddr = atol( str.c_str() );

	config._attenBegin = 0;
	config._attenEnd = -70;

	int sel = _combo22.GetSelectIndex();

	if( sel==0 )
		config._waveLen = "1310";
	else if( sel==1 )
		config._waveLen = "1550";
	else if( sel==2 )
		config._waveLen = "1625";

	_grid21.GetCellString( 1, 1, str );
	config._rang1VerifyAcc = (float)atof( str.c_str() );

	_grid21.GetCellString( 2, 1, str );
	config._rang2VerifyAcc = (float)atof( str.c_str() );

	_grid21.GetCellString( 3, 1, str );
	config._verifyStep = (float)atof( str.c_str() );

	//GetDlgItem(IDC_OPM_SN).GetWindowText( str );
	//config._sn = strtol( str, 0, 16 );
}

void UIOPM::GetCalParam()
{
	View0( "获取选中通道.." );

	string str;

	OPMConfig& config = OPMLogic::GetSingleton()->_configInfor;

	// 获取选中的通道
	vector<UINT> selectList;
	_grid22.GetSelectRows( selectList );
	//
	config._channelList.clear();
	for( UINT i=0; i<selectList.size(); ++i )
		config._channelList.push_back( selectList[i] );

	// 获取16个通道的差值
	for(UINT i=0; i<16; ++i)
	{
		_grid22.GetCellString( i+1, 1, str );
		config._dxPM[i] = (float)atof( str.c_str() );
	}
}

void UIOPM::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )
		_tabX1.SetCurCell( param-1 );
	else if( id==1 )
		OnSetPortNum();
	else if( id==2 )
		OnGetPowerMeter();
	else if( id==3 )
		OnSetPMWaveLen();
	else if( id==21 )
		OnNoOpticInit();
	else if( id==22 )
		OnCollectData();
	else if( id==23 )
		OnVerifyData();
	else if( id==24 )
		OnAutoCalcDX();
	else if( id==24 )
		OnLoadData();
	else if( id==26 )
		OnMonitorPM();
	else if( id==100 )
		OnConnectVOA();
	else if( id==200 )
		OnGrid22( param );
}

void UIOPM::OnConnectVOA()
{
	if( OPMLogic::GetSingleton()->OpenVOAComm( _combo21.GetSelectIndex()+1 ) )
		View0( "连接VOA成功.." );
	else
		View0( "连接VOA失败.." );
}

void UIOPM::OnNoOpticInit()
{
	char v[32];
	memset( v, 0, 32 );

	if( SetOPM2ClearSyn( v, SELDEV_IP )==true )
	{
		for( int i=0; i<16; ++i )
			_grid22.SetCellString( i+1, 1, "" );

		View0( "通道无光初始成功.." );
	}
	else
	{
		View0( "通道无光初始失败.." );
	}
}

void UIOPM::OnCollectData()
{
	GetConfigData();
	GetCalParam();

	OPMLogic::GetSingleton()->StartDataCollectThreadThread();
}

void UIOPM::OnVerifyData()
{
	GetConfigData();
	GetCalParam();

	OPMLogic::GetSingleton()->StartVerifyChannelThread();
}

void UIOPM::OnAutoCalcDX()
{
	GetConfigData();
	GetCalParam();

	OPMLogic::GetSingleton()->StartDataCollectThreadThread();
}

void UIOPM::OnSetPortNum()
{
	string str;
	_edit1.GetString( str );
	SetOPMPortNum( (UCHAR)atol( str.c_str() ), SELDEV_IP );
}

void UIOPM::OnGetPowerMeter()
{
	for(UINT i=1; i<=32; ++i)
	{
		_grid1.SetCellString( i, 2, "" );
		_grid1.SetCellString( i, 3, "" );
	}

	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
		GetOPMPowerMeter( devInfo._mainType, devInfo._subType, SELDEV_IP );
}

void UIOPM::OnSetPMWaveLen()
{
	string str;
	UCHAR waveFlag[32];
	for(int i=0; i<32; ++i)
	{
		_grid1.GetCellString( i+1, 3, str );
		if( str=="1625" )
			waveFlag[i] = 1;
		else if( str=="1550" )
			waveFlag[i] = 2;
		else if( str=="1310" )
			waveFlag[i] = 3;
		else 
			waveFlag[i] = 1;
	}

	SetOPMWaveLen( waveFlag, SELDEV_IP );
}

void UIOPM::OnMonitorPM()
{
	GetConfigData();

	OPMLogic::GetSingleton()->StartMonitorPMThread();
}

void UIOPM::OnLoadData()
{
	GetConfigData();

	OPMLogic::GetSingleton()->StartLoadDataThread();
}

void UIOPM::OnGrid22( int param )
{
	NM_GRID* pNMGrid = (NM_GRID*)param;

	if( pNMGrid->_code==2 || pNMGrid->_code==4 )
	{
		//绘制选中通道的曲线
		UCHAR channel = pNMGrid->_row;
		
		string str;
		if( channel==1 )
		{
			VECTOR_FLOAT dxList = OPMLogic::GetSingleton()->_channelDXList[channel-1];
			if( dxList.size()!=4 ) return;

			for( int i=1; i<=4; ++i )
			{
				STRING_FORMAT( str, "%0.2f", dxList[i-1] );
				_grid23.SetCellString( i, 1, str );
			}
			_grid23.SetCellString( 5, 1, "" );
		}
		else
		{
			STRING_FORMAT( str, "%0.2f", OPMLogic::GetSingleton()->_channelOffsetList[channel-1] );
			_grid23.SetCellString( 5, 1, str );
			//
			for( int i=1; i<=4; ++i )
				_grid23.SetCellString( i, 1, "" );
		}
	}
}