#include "stdafx.h"
#include "UIAtt.h"

using namespace std;


UIWSSAtt gUIWSSAtt;

void View0( const char* msg )
{
	gUIWSSAtt.ShowMsg( msg );
}

void View6( UCHAR p, UCHAR d )
{
	gUIWSSAtt.ShowChannelAtten( p, d );
}

void View7( UCHAR *p, float *d )
{
	gUIWSSAtt.ShowAllChannelAtten( p, d );
}

void View8( UCHAR pID )
{
	gUIWSSAtt.ShowProfileID( pID );
}



void UIWSSAtt::OnCreate()
{
	// 协议view回调函数注册
	SetCB_View_ShowMsg( View0 );
	SetCB_View_ShowChannelAtten( View6 );
	SetCB_View_ShowAllChannelAtten( View7 );
	SetCB_View_ShowProfileID( View8 );

	// 创建UI
	_grid1.Create( 0, this, GenRect()( 20, 20, 370, _clientRC.bottom-20 ), UILayoutCalc::SIZE_Y );

	RECT rc = GenRect()( 390, 20, _clientRC.right-20, _clientRC.bottom-20 );

	_tabX1.Create( 0, this, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX1.HideTab();
	_tabX1.SetCellNum( 2 );
	_tabX1.SetCell( 0, "Set", &_canvas1 );
	_tabX1.SetCell( 1, "Profile", &_canvas2 );

	_canvas1.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
	{
		_lable1.Create( 0, &_canvas1 );
		_lable1.SetString( "Operating Table:" );
		_lable1.SetColor( _BLUE_ );
		_but1.Create( 1, &_canvas1 );
		_but1.SetString( "Get All Channels" );
		/*_but2.Create( 2, &_canvas1 );
		_but2.SetString( "Set All Channels" );*/

		_lable2.Create( 0, &_canvas1 );
		_lable2.SetString( "Operating Single Channel:" );
		_lable2.SetColor( _BLUE_ );
		_grid2.Create( 0, &_canvas1 );
		_grid2.SetRowColumn( 2, 3 );
		_grid2.SetRowFix();
		_grid2.SetCellString( 0, 0, "Channel" );
		_grid2.SetCellString( 0, 1, "Port" );
		_grid2.SetCellString( 0, 2, "Attenuation" );
		//_grid2.SetCellComboBox( 0, 1, 1 );
		_grid2.SetColumnWidth( 0, 110 );
		_grid2.SetColumnWidth( 1, 110 );
		_grid2.SetColumnWidth( 2, 180 );
		_grid2.SetRowHeight( 1, 30 );
		_but3.Create( 3, &_canvas1 );
		_but3.SetString( "Set Single Channel" );

		_lable3.Create( 0, &_canvas1 );
		_lable3.SetString( "Operating Odd/Even:" );
		_lable3.SetColor( _BLUE_ );
		_grid3.Create( 0, &_canvas1 );
		_grid3.SetRowColumn( 3, 3 );
		_grid3.SetRowFix();
		_grid3.SetColumnFix();
		_grid3.SetCellString( 0, 0, "Channel" );
		_grid3.SetCellString( 0, 1, "Port" );
		_grid3.SetCellString( 0, 2, "Attenuation" );
		_grid3.SetCellString( 1, 0, "Odd" );
		_grid3.SetCellString( 2, 0, "Even" );
		//_grid3.SetCellComboBox( 0, 1, 1 );
		//_grid3.SetCellComboBox( 0, 2, 1 );
		_grid3.SetColumnWidth( 0, 110 );
		_grid3.SetColumnWidth( 1, 110 );
		_grid3.SetColumnWidth( 2, 180 );
		_grid3.SetRowHeight( 1, 30 );
		_grid3.SetRowHeight( 2, 30 );
		_but4.Create( 4, &_canvas1 );
		_but4.SetString( "Set Odd/Even" );

		_lable4.Create( 0, &_canvas1 );
		_lable4.SetString( "Operating All Channels:" );
		_lable4.SetColor( _BLUE_ );
		_grid4.Create( 0, &_canvas1 );
		_grid4.SetRowColumn( 2, 2 );
		_grid4.SetRowFix();
		_grid4.SetCellString( 0, 0, "Port" );
		_grid4.SetCellString( 0, 1, "Attenuation" );
		//_grid4.SetCellComboBox( 0, 1, 0 );
		_grid4.SetColumnWidth( 0, 110 );
		_grid4.SetColumnWidth( 1, 180 );
		_grid4.SetRowHeight( 1, 30 );
		_but5.Create( 5, &_canvas1 );
		_but5.SetString( "Set All Channels" );
	}

	_canvas2.CreateDirect( &_tabX1, GenRect()( 0, 0, 1000, 1000 ) );
	{
		_lable5.Create( 0, &_canvas2 );
		_lable5.SetString( "Profile ID:" );
		_lable5.SetColor( _BLUE_ );
		_edit1.Create( 0, &_canvas2 );
		_edit1.SetString( "0" );
		_but6.Create( 6, &_canvas2 );
		_but6.SetString( "Read Profile ID" );
		_but7.Create( 7, &_canvas2 );
		_but7.SetString( "Read Profile" );
		_but8.Create( 8, &_canvas2 );
		_but8.SetString( "Select Profile" );
		_but9.Create( 9, &_canvas2 );
		_but9.SetString( "Generate Profile" );
	}

	//
	_layoutGrid1.InitPoint( GenPoint()( 0, 0 ) );
	_layoutGrid1.SetRowColumn( 10, 2, 200, 20, 35, 10 );
	//
	_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_but1 );
	_layoutGrid1.SetRowHeightInterval( 0, 35, 40 );
	//
	_layoutGrid1.SetCell( 1, 0, &_lable2 );
	_layoutGrid1.SetCell( 1, 1, &_but3 );
	_layoutGrid1.SetCell( 2, 0, 2, 1, &_grid2 );
	_layoutGrid1.SetRowHeightInterval( 2, 55, 40 );
	//
	_layoutGrid1.SetCell( 3, 0, &_lable3 );
	_layoutGrid1.SetCell( 3, 1, &_but4 );
	_layoutGrid1.SetCell( 4, 0, 4, 1, &_grid3 );
	_layoutGrid1.SetRowHeightInterval( 4, 90, 40 );
	//
	_layoutGrid1.SetCell( 5, 0, &_lable4 );
	_layoutGrid1.SetCell( 5, 1, &_but5 );
	_layoutGrid1.SetCell( 6, 0, 6, 1, &_grid4 );
	_layoutGrid1.SetRowHeightInterval( 6, 55, 40 );
	//
	_layoutGrid1.CalcGridPos();

	_layoutGrid2.InitPoint( GenPoint()( 0, 0 ) );
	_layoutGrid2.SetRowColumn( 10, 2, 200, 20, 35, 10 );
	//
	_layoutGrid2.SetCell( 0, 0, &_lable5 );
	_layoutGrid2.SetCell( 0, 1, &_edit1 );
	_layoutGrid2.SetCell( 1, 0, &_but6 );
	_layoutGrid2.SetCell( 1, 1, &_but7 );
	_layoutGrid2.SetCell( 2, 0, &_but8 );
	_layoutGrid2.SetCell( 2, 1, &_but9 );
	//
	_layoutGrid2.CalcGridPos();
}

void UIWSSAtt::OnMainBar( int param )
{
	_tabX1.SetCurCell( param-1 );
}

void UIWSSAtt::OnGetAllChannels( int id )
{
	if( GetCommMethod()==0 )
		GetAllChannelsPortAtten_Com();
	else
		GetAllChannelsPortAtten_Net( GetWSSIP() );
}

void UIWSSAtt::OnSetAllChannelsOnGrid( int id )
{
	string str;

	UCHAR pList[104];
	float dList[104];
	for( int i=1; i<=CHANNEL_NUM; ++i )
	{
		_grid1.GetCellString( i, 2, str );
		pList[i-1] = atoi( str.c_str() );

		_grid1.GetCellString( i, 3, str );
		dList[i-1] = (float)atof( str.c_str() );
	}
	
	if( GetCommMethod()==0 )
		SetAllChannelsPortAtten_Com( pList, dList, CHANNEL_NUM );
	else
		SetAllChannelsPortAtten_Net( GetWSSIP(), pList, dList, CHANNEL_NUM );
}

void UIWSSAtt::OnSetSingleChannel( int id )
{
	string str;
	_grid2.GetCellString( 1, 0, str );
	int c = atoi( str.c_str() );

	_grid2.GetCellString( 1, 1, str );
	int p = atoi( str.c_str() );

	_grid2.GetCellString( 1, 2, str );
	float d = (float)atof( str.c_str() );

	if( GetCommMethod()==0 )
		SetChannelPortAtten_Com( c, p, d );
	else
		SetChannelPortAtten_Net( GetWSSIP(), c, p, d );
}

void UIWSSAtt::OnSetOddEvenChannels( int id )
{
	if( GetCommMethod()==0 && IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial statu is not connected!", "Warning", 0 );
		return;
	}

	string str;
	_grid3.GetCellString( 1, 1, str );
	int oddP = atoi( str.c_str() );
	_grid3.GetCellString( 1, 2, str );
	float oddD = (float)atof( str.c_str() );

	_grid3.GetCellString( 2, 1, str );
	int evenP = atoi( str.c_str() );
	_grid3.GetCellString( 2, 2, str );
	float evenD = (float)atof( str.c_str() );

	UCHAR pList[104];
	float dList[104];
	for( int c=1; c<=CHANNEL_NUM; c+=2 )
	{
		pList[c-1] = oddP;
		dList[c-1] = oddD;
	}
	for( int c=2; c<=CHANNEL_NUM; c+=2 )
	{
		pList[c-1] = evenP;
		dList[c-1] = evenD;
	}

	if( GetCommMethod()==0 )
		SetAllChannelsPortAtten_Com( pList, dList, CHANNEL_NUM );
	else
		SetAllChannelsPortAtten_Net( GetWSSIP(), pList, dList, CHANNEL_NUM );
}

void UIWSSAtt::OnSetAllChannelsOnSingleValue( int id )
{
	if( GetCommMethod()==0 && IsSerialOpen()==false )
	{
		::MessageBoxA( NULL, "Serial status is not connected!", "Warning", 0 );
		return;
	}

	string str;
	_grid4.GetCellString( 1, 0, str );
	int p = atoi( str.c_str() );
	_grid4.GetCellString( 1, 1, str );
	float d = (float)atof( str.c_str() );

	UCHAR pList[104];
	float dList[104];
	for( int i=0; i<CHANNEL_NUM; ++i )
	{
		pList[i] = p;
		dList[i] = d;
	}
	
	if( GetCommMethod()==0 )
		SetAllChannelsPortAtten_Com( pList, dList, CHANNEL_NUM );
	else
		SetAllChannelsPortAtten_Net( GetWSSIP(), pList, dList, CHANNEL_NUM );
}

void UIWSSAtt::OnReadProfileID( int id )
{
	ReadProfileID_Com();
}

void UIWSSAtt::OnReadProfile( int id )
{
	string str;
	_edit1.GetString( str );

	ReadChannelProfile_Com( atoi( str.c_str() ) );
}

void UIWSSAtt::OnSelectProfile( int id )
{
	string str;
	_edit1.GetString( str );

	SelectChannelProfile_Com( atoi( str.c_str() ) );
}

void UIWSSAtt::OnGenerateProfile( int id )
{
	string str;
	_edit1.GetString( str );

	UCHAR portList[104];
	UCHAR attenList[104];

	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		_grid1.GetCellString( c, 2, str );
		portList[c-1] = atoi( str.c_str() );

		_grid1.GetCellString( c, 3, str );
		attenList[c-1] = (UCHAR)( atof( str.c_str() )*10 );
	}

	GenerateChannelProfile_Com( atoi( str.c_str() ), portList, attenList );
}



void UIWSSAtt::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
		OnMainBar( param );
	else if( id==1 )
		OnGetAllChannels( param );
	else if( id==2 )
		OnSetAllChannelsOnGrid( param );
	else if( id==3 )
		OnSetSingleChannel( param );
	else if( id==4 )
		OnSetOddEvenChannels( param );
	else if( id==5 )
		OnSetAllChannelsOnSingleValue( param );	
	else if( id==6 )
		OnReadProfileID( param );
	else if( id==7 )
		OnReadProfile( param );
	else if( id==8 )
		OnSelectProfile( param );
	else if( id==9 )
		OnGenerateProfile( param );
}

void UIWSSAtt::UIInitPortFreq()
{
	string str;
	int num = CHANNEL_NUM;

	_grid1.SetRowColumn( num+1, 4 );
	_grid1.SetRowFix();
	_grid1.SetColumnFix();
	_grid1.SetCellString( 0, 0, "Channel" );
	_grid1.SetCellString( 0, 1, "Freq.(GHz)" );
	_grid1.SetCellString( 0, 2, "Port" );
	_grid1.SetCellString( 0, 3, "Atten." );
	_grid1.SetColumnWidth( 1, 100 );
	_grid1.SetColumnWidth( 3, 100 );
	for( int j=0; j<num; ++j )
	{
		STRING_FORMAT( str, "%d", j+1 );
		_grid1.SetCellString( j+1, 0, str );
	}

	//
	VECTOR_STRING strList;
	for( int i=0; i<PORT_NUM; ++i )
	{
		STRING_FORMAT( str, "%d", i+1 );
		strList.push_back( str );
	}
	_grid2.SetCellComboBox( 0, 1, 1, strList );	
	_grid3.SetCellComboBox( 0, 1, 1, strList );	
	_grid3.SetCellComboBox( 0, 2, 1, strList );	
	_grid4.SetCellComboBox( 0, 1, 0, strList );	

	//
	int begFrq = GetStartChannelFreqGHz();
	for( int j=0; j<num; ++j )
	{
		STRING_FORMAT( str, "%d", begFrq+j*( FREQ_GHZ==100?100:50 ) );
		_grid1.SetCellString( j+1, 1, str );
	}
}

void UIWSSAtt::ShowMsg( const string msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void UIWSSAtt::ShowChannelAtten( UCHAR p, UCHAR d ) 
{ 
}

void UIWSSAtt::ShowAllChannelAtten( UCHAR *p, float *d ) 
{ 
	string str;
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		STRING_FORMAT( str, "%d", p[c-1] );
		_grid1.SetCellString( c, 2, str );

		STRING_FORMAT( str, "%0.2f", d[c-1] );
		_grid1.SetCellString( c, 3, str );
	}
}

void UIWSSAtt::ShowProfileID( UCHAR pID )
{
	string str;
	STRING_FORMAT( str, "%d", pID );
	_edit1.SetString( str );
}
