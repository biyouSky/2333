

#include "StdAfx.h"
#include "UIT4.h"

using namespace std;


UIT4 gUIT4;

UINT gPortNum = 10;



bool ReadOPMList( ULONG devIP, float* pmList )
{
	UdpOBJ udp;

	UCHAR buf[512];
	memset( buf, 0, 512 );
	buf[0] = 0xEF;
	buf[1] = 0xEF;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x0B;
	buf[5] = 0x11;
	buf[6] = 0x00;
	buf[7] = 0x01;
	buf[8] = 0x22;
	buf[9] = 0x00;
	buf[10] = 0x01;
	buf[11] = 0x1E;
	buf[12] = 0x01;
	buf[13] = 0x00;
	buf[14] = 0x03;
	buf[15] = 0x40;
	udp.SendTo( (const char*)buf, 16, devIP );

	memset( buf, 0, 512 );
	if( udp.RecvFrom( (char*)buf, 512 )!=83 )
		return false;

	if( buf[7]!=0x01 && buf[8]!=0x22 )
		return false;

	USHORT pm = 0;
	UCHAR* pDataBuf = buf+18;
	for( UINT i=0; i<gPortNum; ++i )
	{
		memcpy( &pm, pDataBuf+2, 2 );
		pm = ntohs( pm );

		pmList[i] = (short)pm/100.f;

		pDataBuf += 4;
	}

	return true;
}

void UIT4::InspectPM()
{
	for( UINT r=1; r<=gPortNum; ++r )
	{
		for( UINT c=1; c<4; ++c )
			_grid1.SetCellString( r, c, "" );
	}

	// 第一次读取初始化
	float pmList[16];
	string str;

	while( 1 )
	{
		while( !ReadOPMList( GetOSWIP(), pmList ) ) { Sleep( 50 ); }

		vector<VECTOR_STRING> rowList;

		for( UINT i=0; i<gPortNum; ++i )
		{
			VECTOR_STRING row;

			// 绝对值
			STRING_FORMAT( str, "%0.3f", pmList[i] );
			row.push_back( str );

			// 插损值
			STRING_FORMAT( str, "%0.3f", pmList[i]-_slPM1[i] );
			row.push_back( str );

			// 一致性
			STRING_FORMAT( str, "%0.3f", pmList[i]-_slPM1[i]-_slPM2[i] );
			row.push_back( str );

			rowList.push_back( row );
		}
		_grid1.SetAreaCells( 1, 1, rowList );

		UIRefresh();
		Sleep( 20 );
	}
}

ULONG UIT4::GetOSWIP()
{
	string str;
	_edit1.GetString( str );
	return IP4_StrToULONG()( str.c_str() );
}

void UIT4::OnCreate()
{
	string str;

	_grid1.Create( 0, this, GenRect()( 20, 20, 700, 550 ), UILayoutCalc::SIZE_Y );
	_grid1.SetRowColumn( 17, 8 );
	_grid1.SetRowFix();
	_grid1.SetColumnFix();
	_grid1.SetCellString( 0, 0, "port" );
	_grid1.SetCellString( 0, 1, "绝对值" );
	_grid1.SetCellString( 0, 2, "插损值" );
	_grid1.SetCellString( 0, 3, "一致性" );
	_grid1.SetCellString( 0, 4, "存光1" );
	_grid1.SetCellString( 0, 5, "存光2" );
	_grid1.SetCellString( 0, 6, "button" );
	_grid1.SetCellString( 0, 7, "button" );
	for( int i=1; i<17; ++i )
	{
		STRING_FORMAT( str, "%d", i ); 
		_grid1.SetCellString( i, 0, str.c_str() );
		_grid1.SetRowHeight( i, 30 );

		_grid1.SetCellButton( 100+i, i, 6, "存光1" );
		_grid1.SetCellButton( 200+i, i, 7, "存光2" );
	}
	_grid1.SetRowHeight( 0, 32 );
	_grid1.SetColumnWidth( 0, 40 );
	_grid1.SetColumnWidth( 1, 90 );
	_grid1.SetColumnWidth( 2, 90 );
	_grid1.SetColumnWidth( 3, 90 );
	_grid1.SetColumnWidth( 4, 90 );
	_grid1.SetColumnWidth( 5, 90 );
	_grid1.SetColumnWidth( 6, 90 );
	_grid1.SetColumnWidth( 7, 90 );

	_but1.Create( 1, this );
	_but1.SetString( "开始监控" );
	_but2.Create( 2, this );
	_but2.SetString( "停止监控" );
	_but3.Create( 3, this );
	_but3.SetString( "存光1" );
	_but4.Create( 4, this );
	_but4.SetString( "存光2" );

	_but5.Create( 5, this );
	_but5.SetString( "调试" );
	_but6.Create( 6, this );
	_but6.SetString( "固化" );
	_but7.Create( 7, this );
	_but7.SetString( "老化" );
	_but8.Create( 8, this );
	_but8.SetString( "温循后" );

	_but9.Create( 9, this );
	_but9.SetString( "常温1" );
	_but10.Create( 10, this );
	_but10.SetString( "常温2" );
	_but11.Create( 11, this );
	_but11.SetString( "高温" );
	_but12.Create( 12, this );
	_but12.SetString( "低温" );
	_but13.Create( 13, this );
	_but13.SetString( "存储存光" );
	_but14.Create( 14, this );
	_but14.SetString( "加载存光" );

	_lable1.Create( 0, this );
	_lable1.SetString( "光开关IP地址:" );
	_edit1.Create( 0, this );

	_layoutGrid1.InitPoint( GenPoint()( 710, 20 ) );
	_layoutGrid1.SetRowColumn( 14, 2 );
	_layoutGrid1.SetCell( 0, 0, 0, 1, &_lable1 );
	_layoutGrid1.SetCell( 1, 0, 1, 1, &_edit1 );
	_layoutGrid1.SetCell( 2, 0, &_but1 );
	_layoutGrid1.SetCell( 2, 1, &_but2 );
	_layoutGrid1.SetCell( 3, 0, &_but3 );
	_layoutGrid1.SetCell( 3, 1, &_but4 );
	_layoutGrid1.SetCell( 4, 0, &_but5 );
	_layoutGrid1.SetCell( 4, 1, &_but6 );
	_layoutGrid1.SetCell( 5, 0, &_but7 );
	_layoutGrid1.SetCell( 5, 1, &_but8 );
	_layoutGrid1.SetCell( 6, 0, &_but9 );
	_layoutGrid1.SetCell( 6, 1, &_but10 );
	_layoutGrid1.SetCell( 7, 0, &_but11 );
	_layoutGrid1.SetCell( 7, 1, &_but12 );
	_layoutGrid1.SetCell( 9, 0, &_but13 );
	_layoutGrid1.SetCell( 9, 1, &_but14 );
	_layoutGrid1.CalcGridPos();

	// 操作模板
	//string path;
	//GetExeDirectory( path );
	////STRING_FORMAT( path, "%s\\阵列准直器测试数据模版.xls", path );
	//STRING_FORMAT( path, "D:\\WorkSpace\\wss_s\\wss_shell\\Bin\\阵列准直器测试数据模版.xls" );
}

void UIT4::OnSaveLight1All()
{
	float slPM[16];
	string str;
	if( !ReadOPMList( GetOSWIP(), slPM ) )
	{
		::MessageBoxA( NULL, "功率读取失败", "warning", 0 );
	}
	else
	{
		for( UINT i=0; i<gPortNum; ++i )
		{
			STRING_FORMAT( str, "%0.3f", slPM[i] );
			_grid1.SetCellString( i+1, 4, str );
			_slPM1[i] = slPM[i];
		}
	}
}

void UIT4::OnSaveLight2All()
{
	float slPM[16];
	string str;
	if( !ReadOPMList( GetOSWIP(), slPM ) )
	{
		::MessageBoxA( NULL, "功率读取失败", "warning", 0 );
	}
	else
	{
		for( UINT i=0; i<gPortNum; ++i )
		{
			string str;
			_grid1.GetCellString( i+1, 2, str );
			_grid1.SetCellString( i+1, 5, str );

			_slPM2[i] = (float)atof( str.c_str() );
		}
	}
}

void UIT4::OnSaveLight1( UINT index )
{
	string str;
	if( index>=1 && index<=gPortNum )
	{
		float slPM[16];
		if( !ReadOPMList( GetOSWIP(), slPM ) )
		{
			::MessageBoxA( NULL, "功率读取失败", "warning", 0 );
		}
		else
		{
			STRING_FORMAT( str, "%0.3f", slPM[index-1] );
			_grid1.SetCellString( index, 4, str );
			_slPM1[index-1] = slPM[index-1];
		}
	}
}

void UIT4::OnSaveLight2( UINT index )
{
	string str;
	if( index>=1 && index<=gPortNum )
	{
		float slPM[16];
		if( !ReadOPMList( GetOSWIP(), slPM ) )
		{
			::MessageBoxA( NULL, "功率读取失败", "warning", 0 );
		}
		else
		{
			string str;
			_grid1.GetCellString( index, 2, str );
			_grid1.SetCellString( index, 5, str );

			_slPM2[index-1] = (float)atof( str.c_str() );
		}
	}
}

void UIT4::OnSaveData1()
{
	/*string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "调试" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 5+r, 2, str.c_str() );

				_grid1.GetCellString( r, 3, str );
				XLSSetCellString( 5+r, 3, str.c_str() );
			}
		}
		XLSClose();
	}*/
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "调试" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 2, str.c_str() );

				_grid1.GetCellString( r, 3, str );
				XLSSetCellString( 2+r, 3, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData2()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "固化" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 2, str.c_str() );

				_grid1.GetCellString( r, 3, str );
				XLSSetCellString( 2+r, 3, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData3()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "老化" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 2, str.c_str() );

				_grid1.GetCellString( r, 3, str );
				XLSSetCellString( 2+r, 3, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData4()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "温循后" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 2, str.c_str() );

				_grid1.GetCellString( r, 3, str );
				XLSSetCellString( 2+r, 3, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData5()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "温循后" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 4, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData6()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "温循后" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 6, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData7()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "温循后" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 5, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveData8()
{
	string str;
	if( OpenFolderFile( str ) )
	{
		XLSOpenFile( str.c_str() );
		if( XLSOpenSheet( "温循后" ) )
		{
			for( int r=1; r<=10; ++r )
			{
				_grid1.GetCellString( r, 2, str );
				XLSSetCellString( 2+r, 7, str.c_str() );
			}
		}
		XLSClose();
	}
}

void UIT4::OnSaveSL()
{
	TxtOBJ txt;
	if( txt.CreateFile( "T4SL.xls" ) )
	{
		string str;
		for( UINT i=0; i<16; ++i )
		{
			STRING_FORMAT( str, "%d	%0.2f	%0.2f\r\n", i+1, _slPM1[i], _slPM2[i] );
			txt.WriteToEnd( str );
		}
	}
}

void UIT4::OnLoadSL()
{
	TxtOBJ txt;
	if( txt.OpenFile( "T4SL.xls" ) )
	{
		std::vector<VECTOR_STRING > allLineWordsList;
		txt.ReadAllLineWords( allLineWordsList );

		if( allLineWordsList.size()==16 )
		{
			for( UINT i=0; i<16; ++i )
			{
				VECTOR_STRING& str = allLineWordsList[i];
			
				_slPM1[i] = (float)atof( str[1].c_str() );
				_slPM2[i] = (float)atof( str[2].c_str() );
			}
		}
	}
}

void UIT4::OnNotify( int id, int param )
{
	string str;

	if( id==1 )
	{
		StartThread( &UIT4::InspectPM );
	}
	else if( id==2 )
	{

	}
	else if( id==3 )	// 存光1 所有通道
	{
		OnSaveLight1All();
	}
	else if( id==4 )	// 存光2 所有通道
	{
		OnSaveLight2All();
	}
	else if( id>100 && id<200 )
	{
		OnSaveLight1( id-100 );
	}
	else if( id>200 && id<300 )
	{
		OnSaveLight2( id-200 );
	}
	else if( id==5 )
	{
		OnSaveData1();
	}
	else if( id==6 )
	{
		OnSaveData2();
	}
	else if( id==7 )
	{
		OnSaveData3();
	}
	else if( id==8 )
	{
		OnSaveData4();
	}
	else if( id==9 )
	{
		OnSaveData5();
	}
	else if( id==10 )
	{
		OnSaveData6();
	}
	else if( id==11 )
	{
		OnSaveData7();
	}
	else if( id==12 )
	{
		OnSaveData8();
	}
	else if( id==13 )
	{
		OnSaveSL();
	}
	else if( id==14 )
	{
		OnLoadSL();
	}
}