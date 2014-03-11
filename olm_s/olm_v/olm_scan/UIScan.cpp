
#include "stdafx.h"
#include "UIScan.h"

UIScan gUIScan;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void View1( void* pDev )
{
	gUIScan.ShowProduct( (DeviceInfo*)pDev );
}


void UIScan::OnCreate()
{
	//
	SetCB_View_ShowMsg( View0 );
	SetCB_View_ShowProduct( View1 );

	//
	string str;

	RECT rc = _clientRC;
	rc.bottom = rc.top+480;

	_grid1.Create( 100, this, rc, UILayoutCalc::SIZE_X );
	_grid1.SetRowColumn( 18, 17 );
	_grid1.SetRowFix();
	_grid1.SetColumnFix();
	// 设置第一行
	_grid1.SetRowHeight( 0, 30 );
	_grid1.SetCellString( 0, 0, "Slot" );
	_grid1.SetColumnWidth( 0, 40 );
	_grid1.SetCellString( 0, 1, "Dev_Type" );
	_grid1.SetColumnWidth( 1, 85 );
	_grid1.SetCellString( 0, 2, "Main_Type" );
	_grid1.SetColumnWidth( 2, 65 );
	_grid1.SetCellString( 0, 3, "Sub_Type" );
	_grid1.SetColumnWidth( 3, 65 );
	_grid1.SetCellString( 0, 4, "MAC" );
	_grid1.SetColumnWidth( 4, 120 );
	_grid1.SetCellString( 0, 5, "IP" );
	_grid1.SetColumnWidth( 5, 110 );
	_grid1.SetCellString( 0, 6, "MASK" );
	_grid1.SetColumnWidth( 6, 110 );
	_grid1.SetCellString( 0, 7, "GATE" );
	_grid1.SetColumnWidth( 7, 110 );
	_grid1.SetCellString( 0, 8, "SN" );
	_grid1.SetColumnWidth( 8, 80 );
	_grid1.SetCellString( 0, 9, "Frame_SN" );
	_grid1.SetColumnWidth( 9, 80 );
	_grid1.SetCellString( 0, 10, "temper" );
	_grid1.SetCellString( 0, 11, "轮询间隔" );
	_grid1.SetCellString( 0, 12, "企业号" );
	_grid1.SetCellString( 0, 13, "协议号" );
	_grid1.SetCellString( 0, 14, "固件版本" );
	_grid1.SetCellString( 0, 15, "硬件版本" );
	_grid1.SetCellString( 0, 16, "Slot_Width" );
	// 设置第一列
	_grid1.SetRowHeight( 1, 25 );
	_grid1.SetCellString( 1, 0, "主控" );
	for( int i=2; i<18; ++i )
	{
		_grid1.SetRowHeight( i, 25 );
		STRING_FORMAT( str, "%d", i-1 );
		_grid1.SetCellString( i, 0, str );	
	}

	_lable1.Create( 0, this );
	_lable1.SetString( "        广播地址: " );
	_edit1.Create( 0, this );
	_edit1.SetString( "192.168.255.255" );
	_but1.Create( 101, this );
	_but1.SetString( "扫描" );
	_but2.Create( 102, this );
	_but2.SetString( "跳转" );

	_combo1.Create( 0, this );
	_combo1.AddString( "Use_Costum" );
	_combo1.AddString( "Use_Default" );
	_but3.Create( 103, this );
	_but3.SetString( "ACTIVE IP" );

	_layoutGrid1.InitPoint( GenPoint()( 50, 500 ) );
	_layoutGrid1.SetRowColumn( 2, 5 );
	for( int c=0; c<5; ++c )
		_layoutGrid1.SetColumnWidthInterval( c, 150, 10 );
	_layoutGrid1.SetRowHeightInterval( 0, 50, 10 );
	_layoutGrid1.SetRowHeightInterval( 1, 50, 10 );
	_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_edit1 );
	_layoutGrid1.SetCell( 1, 0, &_but2 );
	_layoutGrid1.SetCell( 1, 1, &_but1 );
	_layoutGrid1.SetCell( 0, 3, &_combo1 );
	_layoutGrid1.SetCell( 0, 4, &_but3 );
	_layoutGrid1.CalcGridPos();

	// 开始接受数据线程
	StartRecvResponseThread();
}

void UIScan::OnNotify( int id, int param )
{
	string str;

	if( id==100 )
	{
		NM_GRID* pNMGrid = (NM_GRID*)param;

		// 选中的为行或cell 设定选中设备信息
		if( pNMGrid->_code==2 || pNMGrid->_code==4 )		
		{	
			// 获得ip
			_grid1.GetCellString( pNMGrid->_row, 5, str );
			ULONG selIP = IP4_StrToULONG()( str.c_str() );

			// 保存选中的设备信息
			if( selIP!=0 )
			{
				SELDEV_IP = selIP;
				_grid1.GetCellString( pNMGrid->_row, 1, SELDEV_TYPE );
			}
		}
		else if( pNMGrid->_code==1 )
		{	// 回车消息

			// 未选中设备不做其它处理
			if( SELDEV_IP==0 )  return;

			// 获得cell中的string
			string strCell;
			_grid1.GetCellString( pNMGrid->_row, pNMGrid->_column, strCell );

			if( pNMGrid->_column==4 ) // 设置MAC地址
			{
				vector<string> wordList;
				VECTOR_CHAR splitList;
				splitList.push_back( '-' );
				SplitWords( strCell, wordList, splitList );
				if( wordList.size()!=6 )
					return;

				UCHAR pMacBuf[6];
				for(int i=0; i<6; ++i)
				{
					pMacBuf[i] = (UCHAR)strtol( wordList[i].c_str(), 0, 16 );
				}

				STRING_FORMAT( str, "Set_MAC: %02X-%02X-%02X-%02X-%02X-%02X", 
							   pMacBuf[0], pMacBuf[1], pMacBuf[2], pMacBuf[3], pMacBuf[4], pMacBuf[5] );
				View0( str.c_str() );

				SetMAC( pNMGrid->_row-1, pMacBuf, SELDEV_IP );
				//
				system("arp -d");
				Sleep(10);
				UIPostMessage( this, WM_NOTIFY, 101, 0 );
			}
			else if( pNMGrid->_column==5 || pNMGrid->_column==6 || pNMGrid->_column==7 ) // 设置IP 掩码 网关
			{
				// 对条码枪打的IP地址判定格式是否正确 ??
				vector<string> wordList;
				VECTOR_CHAR splitList;
				splitList.push_back( '.' );
				SplitWords( strCell, wordList, splitList );
				if( wordList.size()!=4 )
					return;
				// if( wordList[0]!=_T("192") || wordList[1]!=_T("168") )
				// 	return 0;

				string ipStr;
				_grid1.GetCellString( pNMGrid->_row, 5, ipStr );

				string maskStr;
				_grid1.GetCellString( pNMGrid->_row, 6, maskStr );

				string netGateStr;
				_grid1.GetCellString( pNMGrid->_row, 7, netGateStr );

				STRING_FORMAT( str, "Set_IP:%s, Mask:%s, Gate:%s", ipStr, maskStr, netGateStr );
				View0( str.c_str() );

				SetIP( pNMGrid->_row-1, IP4_StrToULONG()( ipStr.c_str() ), 
					   IP4_StrToULONG()( netGateStr.c_str() ), IP4_StrToULONG()( maskStr.c_str() ), SELDEV_IP );

				//
				system("arp -d");
				Sleep(10);
				UIPostMessage( this, WM_NOTIFY, 101, 0 );
			}
			else if( pNMGrid->_column==8 ) // 设置SN
			{
				ULONG sn = (ULONG)strtoul( strCell.c_str(), 0, 16 );

				STRING_FORMAT( str, "Set_SN=0x%X", sn );
				View0( str.c_str() );

				SetSN( pNMGrid->_row-1, sn, SELDEV_IP );
			}
		}
	}
	else if( id==101 )			// 广播读取设备信息
	{
		_grid1.ClearAllCells();
		
		_edit1.GetString( str );
		ReadProductInfo( IP4_StrToULONG()( str.c_str() ) );
	}
	else if( id==102 )		// 跳转
	{

	}
	else if( id==103 )
	{
		if( SELDEV_IP==0 )
		{
			::MessageBoxA( NULL, "请选择一个设备..", "告警", 0 );
			return;
		}

		int curSel = _combo1.GetSelectIndex();
		if( curSel==-1 )
		{
			::MessageBoxA( NULL, "请选择一项..", "告警", 0 );
			return;
		}

		ActiveIP( curSel==0?true:false, SELDEV_IP );
	}
}

void UIScan::ShowProduct( DeviceInfo* pDev )
{
	// 判断槽位
	if( pDev->_slot<0 || pDev->_slot>16 )
		return;
	
	string strMsg;

	// 显示设备类型
	ULONG sdSN = pDev->_singleDiscSN;
	UCHAR sn2 = (UCHAR)( sdSN>>16 );
	if( sn2==0x50 )		// OLM机框
	{
		strMsg = "OLM_Frame";
	}
	else if(sn2==0x51)	// OTDR
	{
		strMsg = "OTDR";
	}
	else if(sn2==0x52)	// 光开关
	{
		strMsg = "OSW";
	}
	else if(sn2==0x53)	// 光源
	{
		strMsg = "OS";
	}
	else if(sn2==0x54)	// 光功率计
	{
		strMsg = "OPM";
	}
	else if(sn2==0x55)	// 光源光功率计
	{
		strMsg = "OS_OPM";
	}
	else if(sn2==0x56)	// 光性能监控器
	{
		strMsg = "Performance_Monitor";
	}
	else if( sn2>0x01&&sn2<0x3F ) // OLP
	{
		strMsg = "OLP";
	}
	else if( sn2==0x59 )
	{
		strMsg = "Router";
	}
	else
	{
		strMsg = "Unknown";
	}
	_grid1.SetCellString( pDev->_slot+1, 1, strMsg );

	// 显示主类型	
	STRING_FORMAT( strMsg, "%d", pDev->_mainType );
	_grid1.SetCellString( pDev->_slot+1, 2, strMsg );

	// 显示子类型
	STRING_FORMAT( strMsg, "%d", pDev->_subType );
	_grid1.SetCellString( pDev->_slot+1, 3, strMsg );

	// 显示MAC
	STRING_FORMAT( strMsg, "%02X %02X %02X %02X %02X %02X", pDev->_mac[0], pDev->_mac[1], 
				   pDev->_mac[2], pDev->_mac[3], pDev->_mac[4], pDev->_mac[5], pDev->_mac[6]);
	_grid1.SetCellString( pDev->_slot+1, 4, strMsg );

	// 显示IP
	_grid1.SetCellString( pDev->_slot+1, 5, IP4_ULONGToStr()( pDev->_ipAddr ) );

	// 显示MASK
	_grid1.SetCellString( pDev->_slot+1, 6, IP4_ULONGToStr()( pDev->_mask ) );

	// 显示gate
	_grid1.SetCellString( pDev->_slot+1, 7, IP4_ULONGToStr()( pDev->_gateAddr ) );

	// 显示单盘序列号sn
	STRING_FORMAT( strMsg, "0x%08X", pDev->_singleDiscSN );
	_grid1.SetCellString( pDev->_slot+1, 8, strMsg );
	
	// 显示机框序列号
	STRING_FORMAT( strMsg, "0x%08X", pDev->_frameSN );
	_grid1.SetCellString( pDev->_slot+1, 9, strMsg );

	// 显示温度  
	float temp;
	temp = (char)( ((pDev->_temprature)&0xFF00)>>8 );
	temp += (float)( ((pDev->_temprature)&0x00F0)>>4 ) / 16;
	STRING_FORMAT( strMsg, "%.2f ℃", temp );
	_grid1.SetCellString( pDev->_slot+1, 10, strMsg );

	// 显示最小轮询时间
	STRING_FORMAT( strMsg, "%d", pDev->_udpMinPollingInterval );
	_grid1.SetCellString( pDev->_slot+1, 11, strMsg );

	// 显示企业编号
	STRING_FORMAT( strMsg, "%d", pDev->_corNO );
	_grid1.SetCellString( pDev->_slot+1, 12, strMsg );

	// 显示通信协议编号
	STRING_FORMAT( strMsg, "%d", pDev->_proNO );
	_grid1.SetCellString( pDev->_slot+1, 13, strMsg );

	// 显示软件版本号
	STRING_FORMAT( strMsg, "%0.1f", (float)pDev->_softVer/10 );
	_grid1.SetCellString(pDev->_slot+1, 14, strMsg);

	// 显示硬件版本号
	STRING_FORMAT( strMsg, "%d", pDev->_hardwareVer );
	_grid1.SetCellString(pDev->_slot+1, 15, strMsg);

	// 显示槽宽
	STRING_FORMAT( strMsg, "%d", pDev->_slotWidth );
	_grid1.SetCellString(pDev->_slot+1, 16, strMsg);
}