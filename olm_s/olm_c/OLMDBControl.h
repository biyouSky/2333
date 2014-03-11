
#pragma once



typedef bool (*PFunc_Model_Type200)( ULONG addr );	
typedef bool (*PFunc_Model_Type201)();

typedef bool (*PFunc_Model_Type211)( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
typedef bool (*PFunc_Model_Type212)( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer, float pm[16], float k, float b, float IBMON, float IMPDMON, float resistance );
typedef bool (*PFunc_Model_Type213)( ULONG sn, float pm[16], float& k, float& b, float& IBMON, float& IMPDMON, float& resistance );

typedef bool (*PFunc_Model_Type221)( ULONG sn );
typedef bool (*PFunc_Model_Type222)( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
typedef bool (*PFunc_Model_Type223)( ULONG sn, ULONG ip, UCHAR* pMac, char firmVer, char hardwareVer );
typedef bool (*PFunc_Model_Type224)( ULONG sn, const char* waveLen, int channel, std::string contentStr ); 
typedef bool (*PFunc_Model_Type225)( ULONG sn, const char* waveLen, int channel, std::string& contentStr );





extern PFunc_Model_Type200 ConnectOLMDB;
extern PFunc_Model_Type201 IsConnectOLMDB;
extern PFunc_Model_Type211 InsertDBOS;
extern PFunc_Model_Type212 UpdateDBOS;
extern PFunc_Model_Type213 SelectDBOS;

extern PFunc_Model_Type221 IsDBOPMSNExist;
extern PFunc_Model_Type222 InsertDBOPM;
extern PFunc_Model_Type223 UpdateDBOPM;
extern PFunc_Model_Type224 UpdateDBOPMChannelData;
extern PFunc_Model_Type225 SelectDBOPMChannelData;

bool LoadOLMDBDLL();