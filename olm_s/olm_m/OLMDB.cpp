#include "stdafx.h"
#include "OLMDB.h"
using namespace std;


 OLMDB* OLMDB::_pSingleton = NULL;



OLMDB* OLMDB::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new OLMDB;

	return _pSingleton;
}

void OLMDB::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}


bool OLMDB::Connect( ULONG addr )
{
	return ADODB::Connect( addr, "OLM", "biyou", "123" );
}

inline void OLMDB::InsertTablePre( string& fieldStr, string& valueStr, ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	fieldStr =  "sn, ip, mac, firm_ver, hard_ver, dateTime";

	string macStr;
	MacToStr( pMac, macStr );

	string timeStr;
	SYSTEMTIME stTime;
	GetLocalTime(  &stTime  );
	TimeToStr( stTime, timeStr );

	STRING_FORMAT( valueStr, "'0x%08X', '%s', '%s', %0.1f, %d, '%s'", 
				   sn, IP4_ULONGToStr()( ip ), macStr.c_str(), (float)firmVer/10, hardwareVer, timeStr.c_str() );
}

inline void OLMDB::UpdateTablePre( VECTOR_STRING& fieldStrV, VECTOR_STRING& valueStrV, string& whereStr, ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	fieldStrV.push_back( "ip" );
	fieldStrV.push_back( "mac" );
	fieldStrV.push_back( "firm_ver" );
	fieldStrV.push_back( "hard_ver" );

	valueStrV.push_back( IP4_ULONGToStr()( ip ) );
	// 
	string valueStr;
	MacToStr( pMac, valueStr );
	valueStrV.push_back( valueStr );
	// 
	STRING_FORMAT( valueStr, "%0.1f", (float)firmVer/10 );
	valueStrV.push_back( valueStr );
	// 
	STRING_FORMAT( valueStr, "%d", hardwareVer );
	valueStrV.push_back( valueStr );

	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );
}

bool OLMDB::InsertOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance )
{
	string fieldStr, valueStr;
	InsertTablePre( fieldStr, valueStr, sn, ip, pMac, firmVer, hardwareVer );

	fieldStr += ", pm_1, pm_2, pm_3, pm_4, pm_5, pm_6, pm_7, pm_8, pm_9, pm_10, pm_11, pm_12, pm_13, pm_14, pm_15, pm_16, k, b, IBMON, IMPDMON, resistance";
	string str;
	for( int i=0; i<16; ++i )
	{
		STRING_FORMAT( str, ",%0.2f", pm[i] );
		valueStr += str;
	}
	STRING_FORMAT( str, ",%0.2f", k );
	valueStr += str;
	STRING_FORMAT( str, ",%0.2f", b );
	valueStr += str;
	STRING_FORMAT( str, ",%0.2f", IBMON );
	valueStr += str;
	STRING_FORMAT( str, ",%0.2f", IMPDMON );
	valueStr += str;
	STRING_FORMAT( str, ",%0.2f", resistance );
	valueStr += str;

	return ADODB::Insert( "OS", fieldStr, valueStr );
}

bool OLMDB::UpdateOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance )
{
	VECTOR_STRING fieldStrV, valueStrV;
	string whereStr;

	UpdateTablePre( fieldStrV, valueStrV, whereStr, sn, ip, pMac, firmVer, hardwareVer );

	// 
	string str;
	for( int i=0; i<16; ++i )
	{
		STRING_FORMAT( str, "pm_%d", i+1 );
		fieldStrV.push_back(  str  );
		//
		STRING_FORMAT( str, "%0.2f", pm[i] );
		valueStrV.push_back( str );
	}
	fieldStrV.push_back(  "k"  );
	STRING_FORMAT( str, "%f", k );
	valueStrV.push_back( str );
	//
	fieldStrV.push_back(  "b"  );
	STRING_FORMAT( str, "%f", b );
	valueStrV.push_back( str );
	//
	fieldStrV.push_back(  "IBMON"  );
	STRING_FORMAT( str, "%f", IBMON );
	valueStrV.push_back( str );
	//
	fieldStrV.push_back(  "IMPDMON"  );
	STRING_FORMAT( str, "%f", IMPDMON );
	valueStrV.push_back( str );
	//
	fieldStrV.push_back(  "resistance"  );
	STRING_FORMAT( str, "%f", resistance );
	valueStrV.push_back( str );
	
	return ADODB::Update( "OS", fieldStrV, valueStrV, whereStr );
}

bool OLMDB::SelectOS( ULONG sn, float pm[16], float& k, float& b, float& IBMON, float& IMPDMON, float& resistance )
{
	string whereStr;
	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );

	VECTOR_STRING fieldStrV;
	string str;
	for( int i=0; i<16; ++i )
	{
		STRING_FORMAT( str, "pm_%d", i+1 );
		fieldStrV.push_back(  str  );
	}
	fieldStrV.push_back(  "k"  );
	fieldStrV.push_back(  "b"  );
	fieldStrV.push_back(  "IBMON"  );
	fieldStrV.push_back(  "IMPDMON"  );
	fieldStrV.push_back(  "resistance"  );
	// 
	vector<vector<_variant_t> > record;
	if(  ADODB::Select( "OS", fieldStrV, whereStr, record )==false  )
		return false;

	if(  record.size()!=1  )
		return false;
	
	for( int i=0; i<16; ++i )
		pm[i] = (float)record[0][i];

	k = (float)record[0][16];
	b = (float)record[0][17];
	IBMON = (float)record[0][18];
	IMPDMON = (float)record[0][19];
	resistance = (float)record[0][20];

	return true;
}


bool OLMDB::IsOPMSNExist( ULONG sn )
{
	string fieldStr = "sn";

	string whereStr;
	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );

	vector<vector<_variant_t> > record;
	if(  ADODB::Select( "OPM", fieldStr, whereStr, record )==false  )
		return false;

	if(  record.size()!=1  )
		return false;

	return true;
}

bool OLMDB::InsertOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	string fieldStr, valueStr;
	InsertTablePre( fieldStr, valueStr, sn, ip, pMac, firmVer, hardwareVer );

	return ADODB::Insert( "OPM", fieldStr, valueStr );
}

bool OLMDB::UpdateOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	VECTOR_STRING fieldStrV, valueStrV;
	string whereStr;

	UpdateTablePre( fieldStrV, valueStrV, whereStr, sn, ip, pMac, firmVer, hardwareVer );
	
	return ADODB::Update( "OPM", fieldStrV, valueStrV, whereStr );
}

//waveLenFlag 0:1310 1:1550 2:1625
bool OLMDB::UpdateOPMChannelData( ULONG sn, const char* waveLen, int channel, string& contentStr )
{
	string fieldStr;
	STRING_FORMAT( fieldStr, "c%d_", channel );
	fieldStr += waveLen;

	string whereStr;
	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );

	return ADODB::Update( "OPM", fieldStr, contentStr.c_str(), whereStr );
}

bool OLMDB::SelectOPMChannelData( ULONG sn, const char* waveLen, int channel, string& contentStr )
{
	string fieldStr;
	STRING_FORMAT( fieldStr, "c%d_", channel );
	fieldStr += waveLen;

	string whereStr;
	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );

	vector<vector<_variant_t> > record;
	if(  ADODB::Select( "OPM", fieldStr, whereStr, record )==false  )
		return false;

	if(  record.size()!=1  )
		return false;

	_variant_t var = record[0][0];
	if(  var.vt!=VT_NULL  )
		contentStr = (LPCSTR)_bstr_t( var );
	
	return true;
}

#if 0

bool OLMDB::InsertOSW( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float loss[32] )
{
	string fieldStr, valueStr;
	InsertTablePre( fieldStr, valueStr, sn, ip, pMac, firmVer, hardwareVer );

	fieldStr += ", loss_1, loss_2, loss_3, loss_4, loss_5, loss_6, loss_7, loss_8";
	fieldStr += ", loss_9, loss_10, loss_11, loss_12, loss_13, loss_14, loss_15, loss_16";
	fieldStr += ", loss_17, loss_18, loss_19, loss_20, loss_21, loss_22, loss_23, loss_24";
	fieldStr += ", loss_25, loss_26, loss_27, loss_28, loss_29, loss_30, loss_31, loss_32";
	string str;
	for( int i=0; i<32; ++i )
	{
		STRING_FORMAT( str, ",%0.2f", loss[i] );
		valueStr += str;
	}

	return ADODB::Insert( "OSW", fieldStr, valueStr );	
}

bool OLMDB::UpdateOSWIP( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	VECTOR_STRING fieldStrV, valueStrV;
	string whereStr;

	UpdateTablePre( fieldStrV, valueStrV, whereStr, sn, ip, pMac, firmVer, hardwareVer );
	return ADODB::Update( "OSW", fieldStrV, valueStrV, whereStr );
}

bool OLMDB::UpdateOSWLoss( ULONG sn, float loss[32] )
{
	VECTOR_STRING fieldStrV, valueStrV;
	string whereStr;

	string str;
	for( int i=0; i<32; ++i )
	{
		STRING_FORMAT( str, "loss_%d", i+1 );
		fieldStrV.push_back(  str  );
	}

	for( int i=0; i<32; ++i )
	{
		STRING_FORMAT( str, "%0.2f", loss[i] );
		valueStrV.push_back(  str  );
	}

	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );
	return ADODB::Update( "OSW", fieldStrV, valueStrV, whereStr );
}

bool OLMDB::SelectOSW( ULONG sn, float loss[32] )
{
	string whereStr;
	STRING_FORMAT( whereStr, "sn='0x%08X'", sn );

	string str;
	VECTOR_STRING fieldStrV;
	for( int i=0; i<32; ++i )
	{
		STRING_FORMAT( str, "loss_%d", i+1 );
		fieldStrV.push_back(  str  );
	}
	// 
	vector<vector<_variant_t> > record;
	if(  ADODB::Select( "OSW", fieldStrV, whereStr, record )==false  )
		return false;

	if(  record.size()==1  )
	{
		_variant_t varName;
		for( int i=0; i<32; ++i )
			loss[i] = (float)record[0][i];
	}
	else
	{
		return false;
	}

	return true;	
}

#endif



 __declspec(dllexport) bool IsConnectOLMDB()
 {
	 return OLMDB::GetSingleton()->IsConnect();
 }

__declspec(dllexport) bool ConnectOLMDB( ULONG addr )
{
	return OLMDB::GetSingleton()->Connect( addr );
}

__declspec(dllexport) bool InsertDBOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance )
{
	return OLMDB::GetSingleton()->InsertOS( sn, ip, pMac, firmVer, hardwareVer, pm, k, b, IBMON, IMPDMON, resistance );
}

__declspec(dllexport) bool UpdateDBOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance )
{
	return OLMDB::GetSingleton()->UpdateOS( sn, ip, pMac, firmVer, hardwareVer, pm, k, b, IBMON, IMPDMON, resistance );
}

__declspec(dllexport) bool SelectDBOS( ULONG sn, float pm[16], float& k, float& b, float& IBMON, float& IMPDMON, float& resistance )
{
	return OLMDB::GetSingleton()->SelectOS( sn, pm, k, b, IBMON, IMPDMON, resistance );
}


__declspec(dllexport) bool IsDBOPMSNExist( ULONG sn )
{
	return OLMDB::GetSingleton()->IsOPMSNExist( sn );
}

__declspec(dllexport) bool InsertDBOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	return OLMDB::GetSingleton()->InsertOPM( sn, ip, pMac, firmVer, hardwareVer );
}

__declspec(dllexport) bool UpdateDBOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer )
{
	return OLMDB::GetSingleton()->UpdateOPM( sn, ip, pMac, firmVer, hardwareVer );
}

__declspec(dllexport) bool UpdateDBOPMChannelData( ULONG sn, const char* waveLen, int channel, string& contentStr )
{
	return OLMDB::GetSingleton()->UpdateOPMChannelData( sn, waveLen, channel, contentStr );
}

__declspec(dllexport) bool SelectDBOPMChannelData( ULONG sn, const char* waveLen, int channel, string& contentStr )
{
	return OLMDB::GetSingleton()->SelectOPMChannelData( sn, waveLen, channel, contentStr );
}














