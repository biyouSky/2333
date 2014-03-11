#include "stdafx.h"
#include "UIConfigPub.h"

using namespace std;


void UIConfigBase::OnCreate()
{
	string str;

	// 串口设置
	_lable1.Create( 0, this );
	_lable1.SetString( "Serial NO:" );
	_comBox1.Create( 1, this );
	for( int i=0; i<10; ++i )
	{
		STRING_FORMAT( str, "COM%d", i+1 );
		_comBox1.AddString( str );
	}
	_status1.Create( 0, this );

	// inport
	_lable3.Create( 0, this );
	_lable3.SetString( "Inport:" );
	_edit1.Create( 0, this );

	// outport
	_lable4.Create( 0, this );
	_lable4.SetString( "Outport:" );
	_edit2.Create( 0, this );

	// freq
	_lable5.Create( 0, this );
	_lable5.SetString( "Frequency:" );
	_comboBox2.Create( 2, this );
	_comboBox2.AddString( "50G" );
	_comboBox2.AddString( "100G" );

	// outport
	_lable6.Create( 0, this );
	_lable6.SetString( "Channel Num:" );
	_edit3.Create( 0, this );

	// 设置类型
	_but1.Create( 3, this );
	_but1.SetString( "Set Type" );

	// 设置布局信息
	_layoutGrid1.InitPoint( GenPoint()( 0, 0 ) );
	_layoutGrid1.SetRowColumn( 7, 3 );
	_layoutGrid1.SetColumnWidthInterval( 0, 110, 10 );
	_layoutGrid1.SetColumnWidthInterval( 2, 30 );
	_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_comBox1 );
	_layoutGrid1.SetCell( 0, 2, &_status1 );

	_layoutGrid1.SetCell( 1, 0, &_lable3 );
	_layoutGrid1.SetCell( 1, 1, &_edit1 );
	_layoutGrid1.SetCell( 2, 0, &_lable4 );
	_layoutGrid1.SetCell( 2, 1, &_edit2 );
	_layoutGrid1.SetCell( 3, 0, &_lable5 );
	_layoutGrid1.SetCell( 3, 1, &_comboBox2 );
	_layoutGrid1.SetCell( 4, 0, &_lable6 );
	_layoutGrid1.SetCell( 4, 1, &_edit3 );
	_layoutGrid1.SetCell( 5, 0, 5, 1, &_but1 );

	_layoutGrid1.CalcGridPos();

	// 
	UIInitPortFreq();
}

void UIConfigBase::OnConnectSerial( int param )
{
	string str;
	if( OpenSerial( param+1, 115200 )==true )
	{
		_status1.SetImageIDAndDLL( IDB_OK );
		_status1.PlayAnimateHitDrum();
		STRING_FORMAT( str, "Serial Status: Success to connect COM%d..", param+1 );	
	}
	else
	{
		_status1.SetImageIDAndDLL( IDB_CLOSE );
		_status1.PlayAnimateHitDrum();
		STRING_FORMAT( str, "Serial Status: Fail to connect COM%d..", param+1 );
	}

	UIWinShell::_pWinShell->ShowPlugINMsg( str );
}

void UIConfigBase::OnSetWssType( int param )
{
	string str;
	_edit3.GetString( str );
	int c = atoi( str.c_str() );

	_edit1.GetString( str );
	int ip = atoi( str.c_str() );

	_edit2.GetString( str );
	int op = atoi( str.c_str() );

	int freq = _comboBox2.GetSelectIndex()==0 ? 50:100;

	InitWSSType( c, ip, op, freq );

	UIWinShell::_pWinShell->ShowPlugINMsg( "Success to set wss type.." );
}

void UIConfigBase::OnNotify( int id, int param ) 
{
	if( id==1 )
		OnConnectSerial( param );
	else if( id==3 )
		OnSetWssType( param );
}


void UIConfigBase::UIInitPortFreq()
{
	string str;

	STRING_FORMAT( str, "%d", INPORT_NUM );
	_edit1.SetString( str );

	STRING_FORMAT( str, "%d", OUTPORT_NUM );
	_edit2.SetString( str );

	_comboBox2.SetSelectIndex( FREQ_GHZ==50?0:1 );

	STRING_FORMAT( str, "%d", CHANNEL_NUM );
	_edit3.SetString( str );
}