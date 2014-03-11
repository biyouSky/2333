
#include "stdafx.h"
#include "UITFTP.h"

UITFTP gUITFTP;


void View0( const char* msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
}

void UITFTP::ShowMsg( std::string str )
{
	View0( str.c_str() );
}


void UITFTP::OnCreate()
{
	//
	string str;

	_but1.Create( 1, this );
	_but1.SetString( "Active_IAP" );
	_but2.Create( 2, this );
	_but2.SetString( "获取BIN文件路径" );
	_but3.Create( 3, this );
	_but3.SetString( "开始TFTP写文件" );
	
	_lable1.Create( 0, this );
	_lable1.SetString( "激活设置" );
	_lable2.Create( 0, this );
	_lable2.SetString( "BIN文件路径未设定.." );
	_lable3.Create( 0, this );
	_lable3.SetString( "设备地址" );

	_edit1.Create( 0, this );
	_edit1.SetString( "192.168.88.1" );
	
	_combo1.Create( 0, this );
	_combo1.AddString( "Active" );
	_combo1.AddString( "UnActive" );

	//
	_layoutGrid1.InitPoint( GenPoint()( 100, 100 ) );
	_layoutGrid1.SetRowColumn( 10, 4 );
	for( int r=0; r<10; ++r )
		_layoutGrid1.SetRowHeightInterval( r, 50 );
	_layoutGrid1.SetColumnWidthInterval( 1, 180 );
	_layoutGrid1.SetColumnWidthInterval( 2, 180 );
	//
	_layoutGrid1.SetCell( 0, 0, &_lable1 );
	_layoutGrid1.SetCell( 0, 1, &_combo1 );
	_layoutGrid1.SetCell( 0, 2, &_but1 );
	_layoutGrid1.SetCell( 1, 0, 1, 1, &_lable2 );
	_layoutGrid1.SetCell( 1, 2, &_but2 );
	_layoutGrid1.SetCell( 2, 0, &_lable3 );
	_layoutGrid1.SetCell( 2, 1, &_edit1 );
	_layoutGrid1.SetCell( 2, 2, &_but3 );
	//
	_layoutGrid1.CalcGridPos();
}

void UITFTP::OnNotify( int id, int param )
{
	if( id==1 )
		OnActivateDownload();
	else if( id==2 )
		OnGetBinPath();
	else if( id==3 )
		OnStartTFTP();
}


void UITFTP::OnActivateDownload()
{
	ShowMsg( "Active Iap" );
	bool flag;
	int curSel = _combo1.GetSelectIndex();
	if( curSel==-1 )
	{
		MessageBoxA( 0, "Select Parameter First", "告警", 0 );
		return;
	}
	if( curSel==0 )
		flag = true;
	else if( curSel==1 )
		flag = false;
				
	string str;
	_edit1.GetString( str );

	ActiveDownload( flag, IP4_StrToULONG()( str.c_str() ) );
}

void UITFTP::OnGetBinPath()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		_lable2.SetString( str );
	}
}

void UITFTP::OnStartTFTP()
{
	StartThread( "StartTFTPThread", &UITFTP::StartTFTPThread );
}

void UITFTP::StartTFTPThread()
{
	string path;
	_lable2.GetString( path );

	string ipStr;
	_edit1.GetString( ipStr );

	StartTFTPWrite( path, IP4_StrToULONG()( ipStr.c_str() ) );
}

