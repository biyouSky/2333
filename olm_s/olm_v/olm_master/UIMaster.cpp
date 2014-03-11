#pragma once

#include "stdafx.h"
#include "UIMaster.h"


UIMaster gUIMaster;


void UIMaster::ShowSelDev()
{
	DeviceInfo devInfo;
	if( ReadProductInfoSyn( SELDEV_IP, devInfo ) )
	{
		float temp;
		temp = (char)( ( devInfo._temprature2&0xFF00 )>>8 );
		temp += (float)( ( devInfo._temprature2&0x00F0 )>>4 ) / 16;
		string str;
		STRING_FORMAT( str, "%.2f ¡æ", temp );

		_edit2.SetString( str );
	}
}

void UIMaster::OnCreate()
{
	_but1.Create( 1, this );
	_but1.SetString( "Set Frame SN" );
	//
	_edit1.Create( 0, this );

	_lable1.Create( 0, this );
	_lable1.SetString( "ÎÂ¶È2£º" );
	_edit2.Create( 0, this );

	//
	_layoutGrid1.InitPoint( GenPoint()( 100, 100 ) );
	_layoutGrid1.SetRowColumn( 10, 4 );
	_layoutGrid1.SetRowHeightInterval( 0, 50 );
	_layoutGrid1.SetRowHeightInterval( 1, 50 );
	_layoutGrid1.SetColumnWidthInterval( 0, 180 );
	_layoutGrid1.SetColumnWidthInterval( 1, 180 );
	//
	_layoutGrid1.SetCell( 0, 0, &_but1 );
	_layoutGrid1.SetCell( 0, 1, &_edit1 );
	_layoutGrid1.SetCell( 1, 0, &_lable1 );
	_layoutGrid1.SetCell( 1, 1, &_edit2 );
	//
	_layoutGrid1.CalcGridPos();
}

void UIMaster::OnNotify( int id, int param )
{
	if( id==1 )
		OnSetFrameSN();
}

void UIMaster::OnSetFrameSN()
{
	string str;
	_edit1.GetString( str );
	ULONG frameSN = strtol( str.c_str(), 0, 16 );

	STRING_FORMAT( str, "Set Frame SN:0x%x..", frameSN );
	UIWinShell::_pWinShell->ShowPlugINMsg( str );
	
	SetFrameSN( frameSN, SELDEV_IP ); 
}
