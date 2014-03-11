// wss_shell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "wss_shell.h"

void UIWSS::AddUIElements()
{
	RECT rc;
	::GetClientRect( _hWnd, &rc );

	string path;
	GetExeDirectory( path );
	GetUpDirectory( path );

	_startScreen.Create( 0, &_uiTopContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_startScreen.LoadPlugINs( path+"\\PlugINs" );
}

void UIWSS::ShowPlugINMsg( const std::string msg )
{
	_startScreen.SetMainBarTipMsg( msg );
}
