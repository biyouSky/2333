
#include "stdafx.h"
#include "OLMProControl.h"
using namespace std;





PFunc_Model_Type101 GetSelDevIP;
PFunc_Model_Type102 GetSelDevType;




bool LoadOLMConfigDLL()
{
	#define CheckNULL( obj )  if( obj==NULL ) return false;

	HMODULE handle = ::LoadLibraryA( "olm_config.dll" );
	CheckNULL( handle )

	// 功能接口获得
	GetSelDevIP							= ( PFunc_Model_Type101 )GetProcAddress( handle, "GetSelDevIP");
	GetSelDevType						= ( PFunc_Model_Type102 )GetProcAddress( handle, "GetSelDevType");

	return true;
}