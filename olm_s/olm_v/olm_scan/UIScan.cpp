
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
	// ���õ�һ��
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
	_grid1.SetCellString( 0, 11, "��ѯ���" );
	_grid1.SetCellString( 0, 12, "��ҵ��" );
	_grid1.SetCellString( 0, 13, "Э���" );
	_grid1.SetCellString( 0, 14, "�̼��汾" );
	_grid1.SetCellString( 0, 15, "Ӳ���汾" );
	_grid1.SetCellString( 0, 16, "Slot_Width" );
	// ���õ�һ��
	_grid1.SetRowHeight( 1, 25 );
	_grid1.SetCellString( 1, 0, "����" );
	for( int i=2; i<18; ++i )
	{
		_grid1.SetRowHeight( i, 25 );
		STRING_FORMAT( str, "%d", i-1 );
		_grid1.SetCellString( i, 0, str );	
	}

	_lable1.Create( 0, this );
	_lable1.SetString( "        �㲥��ַ: " );
	_edit1.Create( 0, this );
	_edit1.SetString( "192.168.255.255" );
	_but1.Create( 101, this );
	_but1.SetString( "ɨ��" );
	_but2.Create( 102, this );
	_but2.SetString( "��ת" );

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

	// ��ʼ���������߳�
	StartRecvResponseThread();
}

void UIScan::OnNotify( int id, int param )
{
	string str;

	if( id==100 )
	{
		NM_GRID* pNMGrid = (NM_GRID*)param;

		// ѡ�е�Ϊ�л�cell �趨ѡ���豸��Ϣ
		if( pNMGrid->_code==2 || pNMGrid->_code==4 )		
		{	
			// ���ip
			_grid1.GetCellString( pNMGrid->_row, 5, str );
			ULONG selIP = IP4_StrToULONG()( str.c_str() );

			// ����ѡ�е��豸��Ϣ
			if( selIP!=0 )
			{
				SELDEV_IP = selIP;
				_grid1.GetCellString( pNMGrid->_row, 1, SELDEV_TYPE );
			}
		}
		else if( pNMGrid->_code==1 )
		{	// �س���Ϣ

			// δѡ���豸������������
			if( SELDEV_IP==0 )  return;

			// ���cell�е�string
			string strCell;
			_grid1.GetCellString( pNMGrid->_row, pNMGrid->_column, strCell );

			if( pNMGrid->_column==4 ) // ����MAC��ַ
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
			else if( pNMGrid->_column==5 || pNMGrid->_column==6 || pNMGrid->_column==7 ) // ����IP ���� ����
			{
				// ������ǹ���IP��ַ�ж���ʽ�Ƿ���ȷ ??
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
			else if( pNMGrid->_column==8 ) // ����SN
			{
				ULONG sn = (ULONG)strtoul( strCell.c_str(), 0, 16 );

				STRING_FORMAT( str, "Set_SN=0x%X", sn );
				View0( str.c_str() );

				SetSN( pNMGrid->_row-1, sn, SELDEV_IP );
			}
		}
	}
	else if( id==101 )			// �㲥��ȡ�豸��Ϣ
	{
		_grid1.ClearAllCells();
		
		_edit1.GetString( str );
		ReadProductInfo( IP4_StrToULONG()( str.c_str() ) );
	}
	else if( id==102 )		// ��ת
	{

	}
	else if( id==103 )
	{
		if( SELDEV_IP==0 )
		{
			::MessageBoxA( NULL, "��ѡ��һ���豸..", "�澯", 0 );
			return;
		}

		int curSel = _combo1.GetSelectIndex();
		if( curSel==-1 )
		{
			::MessageBoxA( NULL, "��ѡ��һ��..", "�澯", 0 );
			return;
		}

		ActiveIP( curSel==0?true:false, SELDEV_IP );
	}
}

void UIScan::ShowProduct( DeviceInfo* pDev )
{
	// �жϲ�λ
	if( pDev->_slot<0 || pDev->_slot>16 )
		return;
	
	string strMsg;

	// ��ʾ�豸����
	ULONG sdSN = pDev->_singleDiscSN;
	UCHAR sn2 = (UCHAR)( sdSN>>16 );
	if( sn2==0x50 )		// OLM����
	{
		strMsg = "OLM_Frame";
	}
	else if(sn2==0x51)	// OTDR
	{
		strMsg = "OTDR";
	}
	else if(sn2==0x52)	// �⿪��
	{
		strMsg = "OSW";
	}
	else if(sn2==0x53)	// ��Դ
	{
		strMsg = "OS";
	}
	else if(sn2==0x54)	// �⹦�ʼ�
	{
		strMsg = "OPM";
	}
	else if(sn2==0x55)	// ��Դ�⹦�ʼ�
	{
		strMsg = "OS_OPM";
	}
	else if(sn2==0x56)	// �����ܼ����
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

	// ��ʾ������	
	STRING_FORMAT( strMsg, "%d", pDev->_mainType );
	_grid1.SetCellString( pDev->_slot+1, 2, strMsg );

	// ��ʾ������
	STRING_FORMAT( strMsg, "%d", pDev->_subType );
	_grid1.SetCellString( pDev->_slot+1, 3, strMsg );

	// ��ʾMAC
	STRING_FORMAT( strMsg, "%02X %02X %02X %02X %02X %02X", pDev->_mac[0], pDev->_mac[1], 
				   pDev->_mac[2], pDev->_mac[3], pDev->_mac[4], pDev->_mac[5], pDev->_mac[6]);
	_grid1.SetCellString( pDev->_slot+1, 4, strMsg );

	// ��ʾIP
	_grid1.SetCellString( pDev->_slot+1, 5, IP4_ULONGToStr()( pDev->_ipAddr ) );

	// ��ʾMASK
	_grid1.SetCellString( pDev->_slot+1, 6, IP4_ULONGToStr()( pDev->_mask ) );

	// ��ʾgate
	_grid1.SetCellString( pDev->_slot+1, 7, IP4_ULONGToStr()( pDev->_gateAddr ) );

	// ��ʾ�������к�sn
	STRING_FORMAT( strMsg, "0x%08X", pDev->_singleDiscSN );
	_grid1.SetCellString( pDev->_slot+1, 8, strMsg );
	
	// ��ʾ�������к�
	STRING_FORMAT( strMsg, "0x%08X", pDev->_frameSN );
	_grid1.SetCellString( pDev->_slot+1, 9, strMsg );

	// ��ʾ�¶�  
	float temp;
	temp = (char)( ((pDev->_temprature)&0xFF00)>>8 );
	temp += (float)( ((pDev->_temprature)&0x00F0)>>4 ) / 16;
	STRING_FORMAT( strMsg, "%.2f ��", temp );
	_grid1.SetCellString( pDev->_slot+1, 10, strMsg );

	// ��ʾ��С��ѯʱ��
	STRING_FORMAT( strMsg, "%d", pDev->_udpMinPollingInterval );
	_grid1.SetCellString( pDev->_slot+1, 11, strMsg );

	// ��ʾ��ҵ���
	STRING_FORMAT( strMsg, "%d", pDev->_corNO );
	_grid1.SetCellString( pDev->_slot+1, 12, strMsg );

	// ��ʾͨ��Э����
	STRING_FORMAT( strMsg, "%d", pDev->_proNO );
	_grid1.SetCellString( pDev->_slot+1, 13, strMsg );

	// ��ʾ����汾��
	STRING_FORMAT( strMsg, "%0.1f", (float)pDev->_softVer/10 );
	_grid1.SetCellString(pDev->_slot+1, 14, strMsg);

	// ��ʾӲ���汾��
	STRING_FORMAT( strMsg, "%d", pDev->_hardwareVer );
	_grid1.SetCellString(pDev->_slot+1, 15, strMsg);

	// ��ʾ�ۿ�
	STRING_FORMAT( strMsg, "%d", pDev->_slotWidth );
	_grid1.SetCellString(pDev->_slot+1, 16, strMsg);
}