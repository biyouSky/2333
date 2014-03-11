
#include "stdafx.h"
#include "OLMProControl.h"
using namespace std;


PFunc_SetView_Type0 SetCB_View_ShowMsg;
PFunc_SetView_Type1 SetCB_View_ShowProduct;
PFunc_SetView_Type2 SetCB_View_ShowTestOPM;
PFunc_SetView_Type3 SetCB_View_ShowOSWInfo;
PFunc_SetView_Type4 SetCB_View_ShowOPMPMList;
PFunc_SetView_Type5 SetCB_View_ShowOSWaveLenList;
PFunc_SetView_Type6 SetCB_View_ShowOSOutPutPMList;


PFunc_Model_Type01 ReadProductInfo;
PFunc_Model_Type02 SetSN;
PFunc_Model_Type03 SetIP;
PFunc_Model_Type04 SetMAC;
PFunc_Model_Type05 SetFrameSN;
PFunc_Model_Type06 ReadProductInfoSyn;
PFunc_Model_Type11 TestDS1642_E2prom;
PFunc_Model_Type12 TestDS1642_Temp;
PFunc_Model_Type13 TestBackBoardE2prom;
PFunc_Model_Type14 TestFan;
PFunc_Model_Type15 TestWatchDog;
PFunc_Model_Type16 TestLed;
PFunc_Model_Type17 TestOPM;
PFunc_Model_Type18 TestOSW;
PFunc_Model_Type19 TestI2C0_E2prom;
PFunc_Model_Type20 TestI2C0_Temp;
PFunc_Model_Type25 ActiveIP;
PFunc_Model_Type26 ActiveDownload;
PFunc_Model_Type31 GetOSWStatus;
PFunc_Model_Type32 SetOSWStatus;
PFunc_Model_Type33 SetOSWType;

PFunc_Model_Type41 GetOPSample;
PFunc_Model_Type42 GetOPMPowerMeter;
PFunc_Model_Type43 SetOPMWaveLen;
PFunc_Model_Type44 GetOPMCalbTestPM;
PFunc_Model_Type45 SendOPMCalData;
PFunc_Model_Type46 SetOPMPortNum;
// Synchronous
PFunc_Model_Type47 GetOPMSampleSyn;
PFunc_Model_Type48 GetOPMCalbTestPMSyn;
PFunc_Model_Type49 SendOPMCalDataSyn;

// OPM 新版命令 用于定标
PFunc_Model_Type51 SetOPM2Clear;
PFunc_Model_Type52 GetOPM2ADPM;
PFunc_Model_Type53 SetOPM2Dx;
// Synchronous
PFunc_Model_Type54 GetOPM2ADPMSyn;
PFunc_Model_Type55 SetOPM2DxSyn;
PFunc_Model_Type56 SetOPM2ClearSyn;

PFunc_Model_Type61 GetOSSampleOrPM;
PFunc_Model_Type62 SwitchOS;
PFunc_Model_Type63 SetOSCalData;
PFunc_Model_Type64 GetOSOutPutPM;
PFunc_Model_Type65 GetOSWaveLen;
PFunc_Model_Type66 SetOSWaveLen;
PFunc_Model_Type67 GetOSMaxMinPM;
PFunc_Model_Type68 SetOSResistance;
// Synchronous
PFunc_Model_Type69 GetOSPMSyn;
PFunc_Model_Type70 GetOSMaxMinPMSyn;
PFunc_Model_Type71 GetOSSampleSyn;
PFunc_Model_Type72 GetOSPortNumSyn;
PFunc_Model_Type73 SetOSResistanceSyn;
PFunc_Model_Type74 GetOSOutPutPMSyn;
PFunc_Model_Type75 GetOSOutPutPortPMSyn;


PFunc_Model_Type80 StartRecvResponseThread;




bool LoadOLMProDLL()
{
	#define CheckNULL( obj )  if( obj==NULL ) return false;

	HMODULE handle = ::LoadLibraryA( "olm_pro.dll" );
	CheckNULL( handle )


	// 功能接口获得
	ReadProductInfo							= ( PFunc_Model_Type01 )GetProcAddress( handle, "ReadProductInfo");
	SetSN									= ( PFunc_Model_Type02 )GetProcAddress( handle, "SetSN");
	SetIP									= ( PFunc_Model_Type03 )GetProcAddress( handle, "SetIP");
	SetMAC									= ( PFunc_Model_Type04 )GetProcAddress( handle, "SetMAC");
	SetFrameSN								= ( PFunc_Model_Type05 )GetProcAddress( handle, "SetFrameSN");
	ReadProductInfoSyn						= ( PFunc_Model_Type06 )GetProcAddress( handle, "ReadProductInfoSyn");
	TestDS1642_E2prom						= ( PFunc_Model_Type11 )GetProcAddress( handle, "TestDS1642_E2prom");
	TestDS1642_Temp							= ( PFunc_Model_Type12 )GetProcAddress( handle, "TestDS1642_Temp");
	TestBackBoardE2prom						= ( PFunc_Model_Type13 )GetProcAddress( handle, "TestBackBoardE2prom");
	TestFan									= ( PFunc_Model_Type14 )GetProcAddress( handle, "TestFan");
	TestWatchDog							= ( PFunc_Model_Type15 )GetProcAddress( handle, "TestWatchDog");
	TestLed									= ( PFunc_Model_Type16 )GetProcAddress( handle, "TestLed");
	TestOPM									= ( PFunc_Model_Type17 )GetProcAddress( handle, "TestOPM");
	TestOSW									= ( PFunc_Model_Type18 )GetProcAddress( handle, "TestOSW");
	TestI2C0_E2prom							= ( PFunc_Model_Type19 )GetProcAddress( handle, "TestI2C0_E2prom");
	TestI2C0_Temp							= ( PFunc_Model_Type20 )GetProcAddress( handle, "TestI2C0_Temp");
	ActiveIP								= ( PFunc_Model_Type25 )GetProcAddress( handle, "ActiveIP");
	ActiveDownload							= ( PFunc_Model_Type26 )GetProcAddress( handle, "ActiveDownload");
	GetOSWStatus							= ( PFunc_Model_Type31 )GetProcAddress( handle, "GetOSWStatus");
	SetOSWStatus							= ( PFunc_Model_Type32 )GetProcAddress( handle, "SetOSWStatus");	
	SetOSWType								= ( PFunc_Model_Type33 )GetProcAddress( handle, "SetOSWType");

	GetOPSample								= ( PFunc_Model_Type41 )GetProcAddress( handle, "GetOPSample");
	GetOPMPowerMeter						= ( PFunc_Model_Type42 )GetProcAddress( handle, "GetOPMPowerMeter");
	SetOPMWaveLen							= ( PFunc_Model_Type43 )GetProcAddress( handle, "SetOPMWaveLen");
	GetOPMCalbTestPM						= ( PFunc_Model_Type44 )GetProcAddress( handle, "GetOPMCalbTestPM");
	SendOPMCalData							= ( PFunc_Model_Type45 )GetProcAddress( handle, "SendOPMCalData");
	SetOPMPortNum							= ( PFunc_Model_Type46 )GetProcAddress( handle, "SetOPMPortNum");
	GetOPMSampleSyn							= ( PFunc_Model_Type47 )GetProcAddress( handle, "GetOPMSampleSyn");
	GetOPMCalbTestPMSyn						= ( PFunc_Model_Type48 )GetProcAddress( handle, "GetOPMCalbTestPMSyn");
	SendOPMCalDataSyn						= ( PFunc_Model_Type49 )GetProcAddress( handle, "SendOPMCalDataSyn");
	SetOPM2Clear							= ( PFunc_Model_Type51 )GetProcAddress( handle, "SetOPM2Clear");
	GetOPM2ADPM								= ( PFunc_Model_Type52 )GetProcAddress( handle, "GetOPM2ADPM");
	SetOPM2Dx								= ( PFunc_Model_Type53 )GetProcAddress( handle, "SetOPM2Dx");
	GetOPM2ADPMSyn							= ( PFunc_Model_Type54 )GetProcAddress( handle, "GetOPM2ADPMSyn");
	SetOPM2DxSyn							= ( PFunc_Model_Type55 )GetProcAddress( handle, "SetOPM2DxSyn");
	SetOPM2ClearSyn							= ( PFunc_Model_Type56 )GetProcAddress( handle, "SetOPM2ClearSyn");
	GetOSSampleOrPM							= ( PFunc_Model_Type61 )GetProcAddress( handle, "GetOSSampleOrPM");
	SwitchOS								= ( PFunc_Model_Type62 )GetProcAddress( handle, "SwitchOS");
	SetOSCalData							= ( PFunc_Model_Type63 )GetProcAddress( handle, "SetOSCalData");
	GetOSOutPutPM							= ( PFunc_Model_Type64 )GetProcAddress( handle, "GetOSOutPutPM");
	GetOSWaveLen							= ( PFunc_Model_Type65 )GetProcAddress( handle, "GetOSWaveLen");
	SetOSWaveLen							= ( PFunc_Model_Type66 )GetProcAddress( handle, "SetOSWaveLen");
	GetOSMaxMinPM							= ( PFunc_Model_Type67 )GetProcAddress( handle, "GetOSMaxMinPM");
	SetOSResistance							= ( PFunc_Model_Type68 )GetProcAddress( handle, "SetOSResistance");
	GetOSPMSyn								= ( PFunc_Model_Type69 )GetProcAddress( handle, "GetOSPMSyn");
	GetOSMaxMinPMSyn						= ( PFunc_Model_Type70 )GetProcAddress( handle, "GetOSMaxMinPMSyn");
	GetOSSampleSyn							= ( PFunc_Model_Type71 )GetProcAddress( handle, "GetOSSampleSyn");
	GetOSPortNumSyn							= ( PFunc_Model_Type72 )GetProcAddress( handle, "GetOSPortNumSyn");
	SetOSResistanceSyn						= ( PFunc_Model_Type73 )GetProcAddress( handle, "SetOSResistanceSyn");
	GetOSOutPutPMSyn						= ( PFunc_Model_Type74 )GetProcAddress( handle, "GetOSOutPutPMSyn");
	GetOSOutPutPortPMSyn					= ( PFunc_Model_Type75 )GetProcAddress( handle, "GetOSOutPutPortPMSyn");

	StartRecvResponseThread					= ( PFunc_Model_Type80 )GetProcAddress( handle, "StartRecvResponseThread");


	// view回调注册函数
	SetCB_View_ShowMsg						= ( PFunc_SetView_Type0 )GetProcAddress( handle, "SetCB_View_ShowMsg");
	SetCB_View_ShowProduct					= ( PFunc_SetView_Type1 )GetProcAddress( handle, "SetCB_View_ShowProduct");
	SetCB_View_ShowTestOPM					= ( PFunc_SetView_Type2 )GetProcAddress( handle, "SetCB_View_ShowTestOPM");
	SetCB_View_ShowOSWInfo					= ( PFunc_SetView_Type3 )GetProcAddress( handle, "SetCB_View_ShowOSWInfo");
	SetCB_View_ShowOPMPMList				= ( PFunc_SetView_Type4 )GetProcAddress( handle, "SetCB_View_ShowOPMPMList");
	SetCB_View_ShowOSWaveLenList			= ( PFunc_SetView_Type5 )GetProcAddress( handle, "SetCB_View_ShowOSWaveLenList");
	SetCB_View_ShowOSOutPutPMList			= ( PFunc_SetView_Type6 )GetProcAddress( handle, "SetCB_View_ShowOSOutPutPMList");

	return true;
}