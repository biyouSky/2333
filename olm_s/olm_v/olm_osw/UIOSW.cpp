
#include "stdafx.h"
#include "UIOSW.h"

UIOSW gUIOSW;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void View3( UCHAR oswType, USHORT temp, USHORT inportNum, USHORT outportNum, USHORT inportNo, USHORT outportNo )
{
	gUIOSW.ShowOSWInfo( oswType, temp, inportNum, outportNum, inportNo, outportNo );
}


void UIOSW::ShowOSWInfo( UCHAR oswType, USHORT temp, USHORT inportNum, USHORT outportNum, USHORT inportNO, USHORT outportNO )
{
	_combo1.SetSelectIndex( oswType-1 );

	string str;
	STRING_FORMAT( str, "%d", inportNum );
	_lable4.SetString( str );
	
	STRING_FORMAT( str, "%d", outportNum );
	_lable5.SetString( str );
	
	STRING_FORMAT( str, "%d", inportNO );
	_lable2.SetString( str );
	
	STRING_FORMAT( str, "%d", outportNO );
	_lable3.SetString( str );
	
	float t;
	t = (float)( ( temp&0xFF00 )>>8 );
	t += (float)( ( temp&0x00F0 )>>4 ) / 16;
	STRING_FORMAT( str, "%.2f", t );
	_lable1.SetString( str );
}

void UIOSW::OnCreate()
{
	SetCB_View_ShowOSWInfo( View3 );

	//
	string str;

	_combo1.Create( 0, this );
	_combo1.AddString( "1 JDSU_32" );
	_combo1.AddString( "2 Accelink_16" );
	_combo1.AddString( "3 GLSUN_32" );
	_combo1.AddString( "4 JDSU_48" );
	_combo1.AddString( "5 MEMS_16" );
	_combo1.AddString( "6 GLSUN_16" );	
	_combo1.AddString( "7 OptiWorks_8" );	
	_combo1.AddString( "8 Accelink_8" );
	_combo1.AddString( "9 PON" );
	//
	_but1.Create( 1, this );
	_but1.SetString( "Set_type" );

	_lable1.Create( 0, this );
	_lable1.SetString( "temperature(℃):" );
	//
	_edit1.Create( 0, this );
	_edit1.SetString( "" );

	_lable2.Create( 0, this );
	_lable2.SetString( "Current_Input：" );
	//
	_edit2.Create( 0, this );
	_edit2.SetString( "" );

	_lable3.Create( 0, this );
	_lable3.SetString( "Current_Output：" );
	//
	_edit3.Create( 0, this );
	_edit3.SetString( "" );

	_lable4.Create( 0, this );
	_lable4.SetString( "Input_Num：" );
	//
	_edit4.Create( 0, this );
	_edit4.SetString( "" );

	_lable5.Create( 0, this );
	_lable5.SetString( "Output_Num：" );
	//
	_edit5.Create( 0, this );
	_edit5.SetString( "" );

	_but2.Create( 2, this );
	_but2.SetString( "Set_State" );

	_but3.Create( 3, this );
	_but3.SetString( "Get_State" );
	
	_but4.Create( 4, this );
	_but4.SetString( "数据库相关功能" );


	//
	_layoutGrid1.InitPoint( GenPoint()( 100, 100 ) );
	_layoutGrid1.SetRowColumn( 10, 4 );
	/*for( int r=0; r<10; ++r )
		_layoutGrid1.SetRowHeightInterval( r, 50 );*/
	_layoutGrid1.SetColumnWidthInterval( 0, 180 );
	_layoutGrid1.SetColumnWidthInterval( 1, 180 );
	//
	_layoutGrid1.SetCell( 0, 0, &_combo1 );
	_layoutGrid1.SetCell( 0, 1, &_but1 );
	_layoutGrid1.SetCell( 1, 0, &_lable1 );
	_layoutGrid1.SetCell( 1, 1, &_edit1 );
	_layoutGrid1.SetCell( 2, 0, &_lable2 );
	_layoutGrid1.SetCell( 2, 1, &_edit2 );
	_layoutGrid1.SetCell( 3, 0, &_lable3 );
	_layoutGrid1.SetCell( 3, 1, &_edit3 );
	_layoutGrid1.SetCell( 4, 0, &_lable4 );
	_layoutGrid1.SetCell( 4, 1, &_edit4 );
	_layoutGrid1.SetCell( 5, 0, &_lable5 );
	_layoutGrid1.SetCell( 5, 1, &_edit5 );
	_layoutGrid1.SetCell( 6, 0, &_but2 );
	_layoutGrid1.SetCell( 6, 1, &_but3 );
	_layoutGrid1.SetCell( 6, 0, 7, 1, &_but4 );
	//
	_layoutGrid1.CalcGridPos();
}

void UIOSW::OnNotify( int id, int param )
{
	if( id==1 )
		OnSetOSWType();
	if( id==2 )
		OnGetOSWState();
	if( id==3 )
		OnSetOSWState();
	if( id==4 )
		OnOSWDB();
}

void UIOSW::OnSetOSWType()
{
	int curSel = _combo1.GetSelectIndex();
	if(curSel==-1)
	{
		MessageBoxA( 0, "Select Type First", "告警", 0 );
		return;
	}
	
	UCHAR oswType;
	switch( curSel )
	{
		case 0:
			oswType = 0x01;
			break;
		case 1:
			oswType = 0x02;
			break;
		case 2:
			oswType = 0x03;
			break;
		case 3:
			oswType = 0x04;
			break;
		case 4:
			oswType = 0x05;
			break;
		case 5:
			oswType = 0x06;
			break;
		case 6:
			oswType = 0x07;
			break;
		case 7:
			oswType = 0x08;
			break;
		case 8:
			oswType = 0x09;
			break;
		default:
			break;
	};

	View0( "Set Type.." );
	SetOSWType( oswType, SELDEV_IP );
}

void UIOSW::OnGetOSWState()
{
	_combo1.SetSelectIndex( -1 );
	_edit1.SetString( "" );
	_edit2.SetString( "" );
	_edit3.SetString( "" );
	_edit4.SetString( "" );
	_edit5.SetString( "" );

	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
		GetOSWStatus( devInfo._mainType, devInfo._subType, SELDEV_IP );
}

void UIOSW::OnSetOSWState()
{
	View0( "Set Port.." );
	
	string str;
	USHORT inPortNO, outPortNO;
	_edit2.GetString( str );
	inPortNO = (USHORT)atol( str.c_str() );
	_edit3.GetString( str );
	outPortNO = (USHORT)atol( str.c_str() );

	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
		SetOSWStatus( devInfo._extraBuf[0], devInfo._mainType, devInfo._subType, inPortNO, outPortNO, SELDEV_IP );
}

void UIOSW::OnOSWDB()
{
	::ShellExecuteA( 0,"open","OSWTest.exe","","", SW_SHOW );
}