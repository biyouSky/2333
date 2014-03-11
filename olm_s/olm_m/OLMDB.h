
#pragma once

inline void MacToStr( const UCHAR mac[6], std::string& macStr )
{
	STRING_FORMAT( macStr, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
}

inline void TimeToStr( const SYSTEMTIME stTime, std::string& timeStr, int flag=0 )
{
	if( flag==0 )
		STRING_FORMAT( timeStr, "%d-%d-%d %d:%d:%d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond );
	else
		STRING_FORMAT( timeStr, "%d:%d:%d", stTime.wHour, stTime.wMinute, stTime.wSecond );
}

class OLMDB : public ADODB
{
public:
	static OLMDB* GetSingleton();
	static void Destroy();

	bool Connect( ULONG addr = 0xAC100210 );

	bool InsertOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
	bool UpdateOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
	bool SelectOS( ULONG sn, float pm[16], float& k, float& b, float& IBMON, float& IMPDMON, float& resistance );

	bool IsOPMSNExist( ULONG sn );
	bool InsertOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
	bool UpdateOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
	bool UpdateOPMChannelData( ULONG sn, const char* waveLen, int channel, std::string& contentStr ); 
	bool SelectOPMChannelData( ULONG sn, const char* waveLen, int channel, std::string& contentStr );

	//bool InsertOSW( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float loss[32] );
	//bool UpdateOSWIP( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
	//bool UpdateOSWLoss( ULONG sn, float loss[32] );
	//bool SelectOSW( ULONG sn, float loss[32] );

private:
	inline void InsertTablePre( std::string& fieldStr, std::string& valueStr, ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
	inline void UpdateTablePre( VECTOR_STRING& fieldStrV, VECTOR_STRING& valueStrV, std::string& whereStr, ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );

	static OLMDB* _pSingleton;
};

extern "C" __declspec(dllexport) bool IsConnectOLMDB();
extern "C" __declspec(dllexport) bool ConnectOLMDB( ULONG addr );	

extern "C" __declspec(dllexport) bool InsertDBOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
extern "C" __declspec(dllexport) bool UpdateDBOS( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
extern "C" __declspec(dllexport) bool SelectDBOS( ULONG sn, float pm[16], float& k, float& b, float& IBMON, float& IMPDMON, float& resistance );

extern "C" __declspec(dllexport) bool IsDBOPMSNExist( ULONG sn );
extern "C" __declspec(dllexport) bool InsertDBOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
extern "C" __declspec(dllexport) bool UpdateDBOPM( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
extern "C" __declspec(dllexport) bool UpdateDBOPMChannelData( ULONG sn, const char* waveLen, int channel, std::string& contentStr ); 
extern "C" __declspec(dllexport) bool SelectDBOPMChannelData( ULONG sn, const char* waveLen, int channel, std::string& contentStr );



