
#include "stdafx.h"
#include "OLMConfig.h"
using namespace std;


OLMConfig* OLMConfig::_pSingleton = NULL;


OLMConfig* OLMConfig::GetSingleton()
{
	#define CheckNULL( obj )  if( obj==NULL ) return NULL;

	if( _pSingleton==NULL )  
		_pSingleton = new OLMConfig;			// new第一个对象

	return _pSingleton; 
}

void OLMConfig::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

OLMConfig::OLMConfig()
{
	_selDevIP = 0;
}


__declspec(dllexport) ULONG& GetSelDevIP()
{
	return OLMConfig::GetSingleton()->_selDevIP;
}

__declspec(dllexport) string& GetSelDevType()
{
	return OLMConfig::GetSingleton()->_devType;
}