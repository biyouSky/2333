#include "stdafx.h"
#include "OLMDBControl.h"
using namespace std;



PFunc_Model_Type200 ConnectOLMDB;
PFunc_Model_Type201 IsConnectOLMDB;

PFunc_Model_Type211 InsertDBOS;
PFunc_Model_Type212 UpdateDBOS;
PFunc_Model_Type213 SelectDBOS;


PFunc_Model_Type221 IsDBOPMSNExist;
PFunc_Model_Type222	InsertDBOPM;
PFunc_Model_Type223 UpdateDBOPM;
PFunc_Model_Type224 UpdateDBOPMChannelData;
PFunc_Model_Type225 SelectDBOPMChannelData;



bool LoadOLMDBDLL()
{
	#define CheckNULL( obj )  if( obj==NULL ) return false;

	HMODULE handle = ::LoadLibraryA( "olm_db.dll" );
	CheckNULL( handle )

	// 功能接口获得
	ConnectOLMDB				= ( PFunc_Model_Type200 )GetProcAddress( handle, "ConnectOLMDB");
	IsConnectOLMDB				= ( PFunc_Model_Type201 )GetProcAddress( handle, "IsConnectOLMDB");

	InsertDBOS					= ( PFunc_Model_Type211 )GetProcAddress( handle, "InsertDBOS");
	UpdateDBOS					= ( PFunc_Model_Type212 )GetProcAddress( handle, "UpdateDBOS");
	SelectDBOS					= ( PFunc_Model_Type213 )GetProcAddress( handle, "SelectDBOS");

	IsDBOPMSNExist			= ( PFunc_Model_Type221 )GetProcAddress( handle, "IsDBOPMSNExist");
	InsertDBOPM					= ( PFunc_Model_Type222 )GetProcAddress( handle, "InsertDBOPM");
	UpdateDBOPM					= ( PFunc_Model_Type223 )GetProcAddress( handle, "UpdateDBOPM");
	UpdateDBOPMChannelData		= ( PFunc_Model_Type224 )GetProcAddress( handle, "UpdateDBOPMChannelData");
	SelectDBOPMChannelData		= ( PFunc_Model_Type225 )GetProcAddress( handle, "SelectDBOPMChannelData");

	if( !IsConnectOLMDB() )
	{
		string str;
		GetExeDirectory( str );
		GetUpDirectory( str );
		TxtOBJ txt;
		if( txt.OpenFile( str+"\\Config\\db_ip.txt" ) )
		{
			vector<vector<string> > allLineWordList;
			txt.ReadAllLineWords( allLineWordList );

			vector<string>& line = allLineWordList[0];
			str = line[1];
		}
		ConnectOLMDB( IP4_StrToULONG()( str.c_str() ) );
	}

	return true;
}