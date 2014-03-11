
#pragma once

/*-------------------------------------------view 回调注册函数-------------------------------------------*/
// view 回调接口
typedef void (*PFunc_View_Type0)( const char* msg );
typedef void (*PFunc_View_Type1)( void* pDev ); 
typedef void (*PFunc_View_Type2)( USHORT* pList, int num ); 
typedef void (*PFunc_View_Type3)( UCHAR oswType, USHORT temp, USHORT inportNum, USHORT outportNum, USHORT inportNo, USHORT outportNo ); 
typedef void (*PFunc_View_Type4)( UCHAR pmType, UCHAR beginPort, UCHAR endPort, ULONG* pmList );
typedef void (*PFunc_View_Type5)( UCHAR portNum, std::string* waveLenList );
typedef void (*PFunc_View_Type6)( UCHAR portNum, float* pmList );

// view 回调注册接口
typedef void (*PFunc_SetView_Type0)( PFunc_View_Type0 func );
typedef void (*PFunc_SetView_Type1)( PFunc_View_Type1 func );
typedef void (*PFunc_SetView_Type2)( PFunc_View_Type2 func );
typedef void (*PFunc_SetView_Type3)( PFunc_View_Type3 func );
typedef void (*PFunc_SetView_Type4)( PFunc_View_Type4 func );
typedef void (*PFunc_SetView_Type5)( PFunc_View_Type5 func );
typedef void (*PFunc_SetView_Type6)( PFunc_View_Type6 func );


extern PFunc_SetView_Type0 SetCB_View_ShowMsg;
extern PFunc_SetView_Type1 SetCB_View_ShowProduct;
extern PFunc_SetView_Type2 SetCB_View_ShowTestOPM;
extern PFunc_SetView_Type3 SetCB_View_ShowOSWInfo;
extern PFunc_SetView_Type4 SetCB_View_ShowOPMPMList;
extern PFunc_SetView_Type5 SetCB_View_ShowOSWaveLenList;
extern PFunc_SetView_Type6 SetCB_View_ShowOSOutPutPMList;



// 设备信息
// 可能出现的自动字节对齐 除了预编译宏外还用重载的=来保证  尽量避免memcpy
#pragma pack( 1 )
struct DeviceInfo
{
	DeviceInfo() { memset( this, 0, sizeof( DeviceInfo ) ); }
	~DeviceInfo() {}

	void assignValue( UCHAR *pBuf )
	{
		int offset = 0;

		_dataVer = *pBuf;
		offset += 1;

		memcpy( &_corNO, pBuf+offset, 2 );
		_corNO = ntohs( _corNO );
		offset += 2;

		memcpy( &_proNO, pBuf+offset, 2 );
		_proNO = ntohs( _proNO );
		offset += 2;
	
		memcpy( &_softVer, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_hardwareVer, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_mainType, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_subType, pBuf+offset, 2 );
		_subType = ntohs( _subType );
		offset += 2;

		memcpy( &_frameSN, pBuf+offset, 4 );
		_frameSN = ntohl( _frameSN );
		offset += 4;

		memcpy( &_slot, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_slotWidth, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_singleDiscSN, pBuf+offset, 4 );
		_singleDiscSN = ntohl( _singleDiscSN );
		offset += 4;

		memcpy( &_mac, pBuf+offset, 6 );
		offset += 6;

		memcpy( &_ipAddr, pBuf+offset, 4 );
		_ipAddr = ntohl( _ipAddr );
		offset += 4;
	
		memcpy( &_mask, pBuf+offset, 4 );
		_mask = ntohl( _mask );
		offset += 4;
	
		memcpy( &_gateAddr, pBuf+offset, 4 );
		_gateAddr = ntohl( _gateAddr );
		offset += 4;
	
		offset += 10;

		memcpy( &_udpMinPollingInterval, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_temprature, pBuf+offset, 2 );
		_temprature = ntohs( _temprature );
		offset += 2;

		memcpy( &_temprature2, pBuf+offset, 2 );
		_temprature2 = ntohs( _temprature2 );
		offset += 2;
	}
	struct CompareSlot //: public binary_function<DeviceInfo, UCHAR, bool>
	{
		bool operator()( DeviceInfo& d, UCHAR curSlot ) { return ( d._slot==curSlot ); }
	};
	struct CompareIP //: public binary_function<DeviceInfo, ULONG, bool>
	{
		bool operator()( DeviceInfo& d, ULONG curIP ) { return ( d._ipAddr==curIP ); }
	};

	UCHAR _dataVer;
	USHORT _corNO;
	USHORT _proNO;
	UCHAR _softVer;
	UCHAR _hardwareVer;
	UCHAR _mainType;
	USHORT _subType;
	ULONG _frameSN;
	UCHAR _slot;
	UCHAR _slotWidth;
	ULONG _singleDiscSN;
	UCHAR _mac[6];
	ULONG _ipAddr;
	ULONG _mask;
	ULONG _gateAddr;
	UCHAR _resver1[10];
	UCHAR _udpMinPollingInterval;
	USHORT _temprature;
	USHORT _temprature2;								// 主控有效
	UCHAR _resver2[19];
	UCHAR _extraBuf[32];								// 自定义存放额外的信息
};


/*-------------------------------------------功能函数接口-------------------------------------------*/

typedef void (*PFunc_Model_Type01)( ULONG scanIP );																		
typedef void (*PFunc_Model_Type02)( UCHAR slot, ULONG sn, ULONG devIP );															
typedef void (*PFunc_Model_Type03)( UCHAR slot, ULONG ipAddr, ULONG gateIP, ULONG netMask, ULONG devIP );						
typedef void (*PFunc_Model_Type04)( UCHAR slot, UCHAR* pMAC, ULONG devIP );														
typedef void (*PFunc_Model_Type05)( ULONG RackSN, ULONG devIP );																
// Synchronous
typedef bool (*PFunc_Model_Type06)( ULONG devIP, DeviceInfo& devInfo );

// 硬件测试协议
// Asynchronous
typedef void (*PFunc_Model_Type11)( ULONG devIP );
typedef void (*PFunc_Model_Type12)( ULONG devIP );
typedef void (*PFunc_Model_Type13)( ULONG devIP );
typedef void (*PFunc_Model_Type14)( ULONG devIP );
typedef void (*PFunc_Model_Type15)( ULONG devIP );
typedef void (*PFunc_Model_Type16)( ULONG devIP );
typedef void (*PFunc_Model_Type17)( USHORT para, ULONG devIP );
typedef void (*PFunc_Model_Type18)( int sel, int outPort, ULONG devIP );
typedef void (*PFunc_Model_Type19)( ULONG devIP );
typedef void (*PFunc_Model_Type20)( ULONG devIP );

// 固件升级协议
// Asynchronous
typedef void (*PFunc_Model_Type25)( bool activeIP, ULONG devIP );
typedef void (*PFunc_Model_Type26)( bool activeDown, ULONG devIP );

// OSW
// Asynchronous
typedef void (*PFunc_Model_Type31)( UCHAR mainType, USHORT subType, ULONG devIP );
typedef void (*PFunc_Model_Type32)( UCHAR oswType, UCHAR mainType, USHORT subType, USHORT inPortNO, USHORT outPortNO, ULONG devIP );
typedef void (*PFunc_Model_Type33)( UCHAR oswType, ULONG devIP );

// OPM 旧版设计通信协议 新版尽量兼容
// Asynchronous
typedef void (*PFunc_Model_Type41)( UCHAR mainType, USHORT subType, UCHAR channel, ULONG devIP );
typedef void (*PFunc_Model_Type42)( UCHAR mainType, USHORT subType, ULONG devIP );
typedef void (*PFunc_Model_Type43)( UCHAR waveFlag[32], ULONG devIP );
typedef void (*PFunc_Model_Type44)( UCHAR mainType, USHORT subType, UCHAR port, ULONG devIP );
typedef void (*PFunc_Model_Type45)( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );
typedef void (*PFunc_Model_Type46)( UCHAR portNum, ULONG devIP );
// Synchronous
typedef bool (*PFunc_Model_Type47)( UCHAR mainType, USHORT subType, UCHAR channel, USHORT &sampleValue, ULONG devIP );
typedef bool (*PFunc_Model_Type48)( UCHAR mainType, USHORT subType, UCHAR port, float& calbTestPM, ULONG devIP );
typedef bool (*PFunc_Model_Type49)( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );

// OPM 新版命令 用于定标
typedef void (*PFunc_Model_Type51)( ULONG devIP );
typedef void (*PFunc_Model_Type52)( UCHAR startC, UCHAR endC, ULONG devIP );
typedef void (*PFunc_Model_Type53)( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
// Synchronous
typedef bool (*PFunc_Model_Type54)( UCHAR startC, UCHAR endC, float pm[32], ULONG devIP );
typedef bool (*PFunc_Model_Type55)( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
typedef bool (*PFunc_Model_Type56)( char v[32], ULONG devIP );

// 光源
// Asynchronous
typedef void (*PFunc_Model_Type61)( bool flag, ULONG devIP );
typedef void (*PFunc_Model_Type62)( bool openFlag, ULONG devIP );
typedef void (*PFunc_Model_Type63)( float k, float b, int n, float* pData, ULONG devIP );
typedef void (*PFunc_Model_Type64)( ULONG devIP );
typedef void (*PFunc_Model_Type65)( ULONG devIP );
typedef void (*PFunc_Model_Type66)( UINT portNum, std::string waveLen[16], ULONG devIP );
typedef void (*PFunc_Model_Type67)( UCHAR monitorFlag, ULONG devIP );
typedef void (*PFunc_Model_Type68)( UCHAR flag, ULONG devIP );  
// Synchronous
typedef bool (*PFunc_Model_Type69)( float& pm, ULONG devIP );
typedef bool (*PFunc_Model_Type70)( UCHAR monitorFlag1, UCHAR& monitorFlag2, float& max, float& min, ULONG devIP );
typedef bool (*PFunc_Model_Type71)( USHORT& sample, ULONG devIP );
typedef bool (*PFunc_Model_Type72)( UCHAR& portNum, ULONG devIP );
typedef bool (*PFunc_Model_Type73)( UCHAR flag, USHORT& IBMON, USHORT& IMPDMON, USHORT& resistanceLSB, ULONG devIP );  
typedef bool (*PFunc_Model_Type74)( UCHAR& portNum, float pmList[16], ULONG devIP );
typedef bool (*PFunc_Model_Type75)( UCHAR portIndex, float& pm, ULONG devIP );

typedef void (*PFunc_Model_Type80)();


extern PFunc_SetView_Type0 SetCB_View_ShowMsg;
extern PFunc_SetView_Type1 SetCB_View_ShowProduct;
extern PFunc_SetView_Type2 SetCB_View_ShowTestOPM;
extern PFunc_SetView_Type3 SetCB_View_ShowOSWInfo;
extern PFunc_SetView_Type4 SetCB_View_ShowOPMPMList;
extern PFunc_SetView_Type5 SetCB_View_ShowOSWaveLenList;
extern PFunc_SetView_Type6 SetCB_View_ShowOSOutPutPMList;


extern PFunc_Model_Type01 ReadProductInfo;
extern PFunc_Model_Type02 SetSN;
extern PFunc_Model_Type03 SetIP;
extern PFunc_Model_Type04 SetMAC;
extern PFunc_Model_Type05 SetFrameSN;
extern PFunc_Model_Type06 ReadProductInfoSyn;
extern PFunc_Model_Type11 TestDS1642_E2prom;
extern PFunc_Model_Type12 TestDS1642_Temp;
extern PFunc_Model_Type13 TestBackBoardE2prom;
extern PFunc_Model_Type14 TestFan;
extern PFunc_Model_Type15 TestWatchDog;
extern PFunc_Model_Type16 TestLed;
extern PFunc_Model_Type17 TestOPM;
extern PFunc_Model_Type18 TestOSW;
extern PFunc_Model_Type19 TestI2C0_E2prom;
extern PFunc_Model_Type20 TestI2C0_Temp;
extern PFunc_Model_Type25 ActiveIP;
extern PFunc_Model_Type26 ActiveDownload;
extern PFunc_Model_Type31 GetOSWStatus;
extern PFunc_Model_Type32 SetOSWStatus;
extern PFunc_Model_Type33 SetOSWType;

extern PFunc_Model_Type41 GetOPSample;
extern PFunc_Model_Type42 GetOPMPowerMeter;
extern PFunc_Model_Type43 SetOPMWaveLen;
extern PFunc_Model_Type44 GetOPMCalbTestPM;
extern PFunc_Model_Type45 SendOPMCalData;
extern PFunc_Model_Type46 SetOPMPortNum;
// Synchronous
extern PFunc_Model_Type47 GetOPMSampleSyn;
extern PFunc_Model_Type48 GetOPMCalbTestPMSyn;
extern PFunc_Model_Type49 SendOPMCalDataSyn;

// OPM 新版命令 用于定标
extern PFunc_Model_Type51 SetOPM2Clear;
extern PFunc_Model_Type52 GetOPM2ADPM;
extern PFunc_Model_Type53 SetOPM2Dx;
// Synchronous
extern PFunc_Model_Type54 GetOPM2ADPMSyn;
extern PFunc_Model_Type55 SetOPM2DxSyn;
extern PFunc_Model_Type56 SetOPM2ClearSyn;

extern PFunc_Model_Type61 GetOSSampleOrPM;
extern PFunc_Model_Type62 SwitchOS;
extern PFunc_Model_Type63 SetOSCalData;
extern PFunc_Model_Type64 GetOSOutPutPM;
extern PFunc_Model_Type65 GetOSWaveLen;
extern PFunc_Model_Type66 SetOSWaveLen;
extern PFunc_Model_Type67 GetOSMaxMinPM;
extern PFunc_Model_Type68 SetOSResistance;
// Synchronous
extern PFunc_Model_Type69 GetOSPMSyn;
extern PFunc_Model_Type70 GetOSMaxMinPMSyn;
extern PFunc_Model_Type71 GetOSSampleSyn;
extern PFunc_Model_Type72 GetOSPortNumSyn;
extern PFunc_Model_Type73 SetOSResistanceSyn;
extern PFunc_Model_Type74 GetOSOutPutPMSyn;
extern PFunc_Model_Type75 GetOSOutPutPortPMSyn;


extern PFunc_Model_Type80 StartRecvResponseThread;












bool LoadOLMProDLL();


