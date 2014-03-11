// wss_shell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "locker_shell.h"

void UILocker::AddUIElements()
{
	RECT rc;
	::GetClientRect( _hWnd, &rc );

	string path;
	GetExeDirectory( path );
	GetUpDirectory( path );

	_startScreen.Create( 0, &_uiTopContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_startScreen.LoadPlugINs( path+"\\PlugINs" );
	_startScreen.SetBKImage( IDB_IMAGE, 6, 30 );
}

void UILocker::ShowPlugINMsg( const std::string msg )
{
	_startScreen.SetMainBarTipMsg( msg );
}
