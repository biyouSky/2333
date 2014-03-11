#include "stdafx.h"
#include "UIConfigBase.h"

using namespace std;

UIWSSConfig gUIConfig;	

void UIWSSConfig::OnCreate()
{
	string str;

	_configBase.CreateDirect( this, GenRect()( 50, 50, 350, 500 ) );	

#if !_UICULT_
	// 控制方式
	_lable1.Create( 0, &_uiContainer );
	_lable1.SetString( "控制方式:" );
	_checkBut1.Create( 1, &_uiContainer );
	_checkBut1.SetString( "串口" );
	_checkBut1.SetCheck( true );
	_checkBut2.Create( 1, &_uiContainer );
	_checkBut2.SetString( "网口" );
	UISetCheckButtonMutex( &_checkBut1, &_checkBut2 );
	if( GetCommMethod() == WSSComm::COM )
		_checkBut1.SetCheck( true );
	else
		_checkBut2.SetCheck( true );

	// 控制盘ip
	_but1.Create( 2, &_uiContainer );
	_but1.SetString( "控制IP:" );		 
	_edit1.Create( 0, &_uiContainer );
	_edit1.SetString( IP4_ULONGToStr()( GetWSSIP() ) );

	// 设置布局信息
	_layoutGrid2.InitPoint( GenPoint()( 400, 50 ) );
	_layoutGrid2.SetRowColumn( 2, 3 );
	_layoutGrid2.SetCell( 0, 0, &_lable1 );
	_layoutGrid2.SetCell( 0, 1, &_checkBut1 );
	_layoutGrid2.SetCell( 0, 2, &_checkBut2 );
	_layoutGrid2.SetCell( 1, 0, &_but1 );
	_layoutGrid2.SetCell( 1, 1, 1, 2, &_edit1 );
	_layoutGrid2.CalcGridPos();

	// 初始化各种配置信息  因为设置的信息并未及时生效
	UIInitPortFreq();
#endif
}

void UIWSSConfig::OnSetCommunicationMethod( int param )
{
	bool f;
	_checkBut1.GetCheck( f );
	GetCommMethod() = f ? WSSComm::COM : WSSComm::NET;
}

void UIWSSConfig::OnSetWSSIP( int param )
{
	string str;
	_edit1.GetString( str );

	GetWSSIP() = IP4_StrToULONG()( str.c_str() );
}

void UIWSSConfig::OnNotify( int id, int param ) 
{
	if( id==1 )
		OnSetCommunicationMethod( param );
	else if( id==2 )
		OnSetWSSIP( param );
}

void UIWSSConfig::UIInitPortFreq()
{
	_configBase.UIInitPortFreq();
}
