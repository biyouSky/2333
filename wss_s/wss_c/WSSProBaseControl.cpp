#include "stdafx.h"
#include "WSSProBaseControl.h"


using namespace std;


PFunc_SetView_Type0 SetCB_View_ShowMsg;
PFunc_SetView_Type1 SetCB_View_ShowSerialNumber;
PFunc_SetView_Type2 SetCB_View_ShowType;
PFunc_SetView_Type3 SetCB_View_ShowVersion;
PFunc_SetView_Type4 SetCB_View_ShowHardDateCode;
PFunc_SetView_Type5 SetCB_View_ShowFirmwareDateCode;
PFunc_SetView_Type6 SetCB_View_ShowChannelAtten;
PFunc_SetView_Type7 SetCB_View_ShowAllChannelAtten;
PFunc_SetView_Type8 SetCB_View_ShowProfileID;
PFunc_SetView_Type9 SetCB_View_ShowAlarm;
PFunc_SetView_Type10 SetCB_View_ShowVoltage;
PFunc_SetView_Type11 SetCB_View_ShowTemperature;
PFunc_SetView_Type12 SetCB_View_ShowLowTemperature;
PFunc_SetView_Type13 SetCB_View_ShowHighTemperature;
PFunc_SetView_Type14 SetCB_View_ShowMemWear;
PFunc_SetView_Type15 SetCB_View_ShowManufactureID;



PFunc_Model_Type91 OpenSerial;
PFunc_Model_Type92 SetSerialRate;
PFunc_Model_Type93 IsSerialOpen;
PFunc_Model_Type81 SetDACVoltage_Com;
PFunc_Model_Type82 SetDACVoltageSyn_Com;
PFunc_Model_Type01 GetSerialNumber_Com;												// 获得序号
PFunc_Model_Type02 GetType_Com;														// 获得类型
PFunc_Model_Type03 GetVersion_Com;													// 获得版本号
PFunc_Model_Type04 SetRS232BaudRate_Com;											// 获得串口速率
PFunc_Model_Type05 GetHardwareDateCode_Com;											// 获得硬件版本号
PFunc_Model_Type06 GetFirmwareDateCode_Com;											// 获得固件版本号
PFunc_Model_Type07 GetManufactureID_Com;											// 获得制造ID
PFunc_Model_Type08 GetAD5535Syn_Com;	
PFunc_Model_Type11 GetAlarm_Com;													// 获取告警
PFunc_Model_Type12 GetVoltage_Com;													// 获取电压
PFunc_Model_Type13 GetTemperature_Com;												// 获取温度
PFunc_Model_Type18 GetTemperatureSyn_Com;
PFunc_Model_Type14 SetLowTemperatureAlarmThreshold_Com;								// 设置低温度告警门限
PFunc_Model_Type15 GetLowTemperatureAlarmThreshold_Com;								// 获取低温度告警门限
PFunc_Model_Type16 SetHighTemperatureAlarmThreshold_Com;							// 设置高温度告警门限
PFunc_Model_Type17 GetHighTemperatureAlarmThreshold_Com;							// 获取高温度告警门限
PFunc_Model_Type21 SetChannelPortAtten_Com;											// 设置通道port衰减
PFunc_Model_Type22 GetChannelPortAtten_Com;											// 获取通道衰减
PFunc_Model_Type23 SetAllChannelsPortAtten_Com;										// 设置所有通道port衰减
PFunc_Model_Type24 GetAllChannelsPortAtten_Com;										// 获取所有通道衰减
PFunc_Model_Type31 GenerateChannelProfile_Com;
PFunc_Model_Type32 ReadChannelProfile_Com;							
PFunc_Model_Type33 SelectChannelProfile_Com;
PFunc_Model_Type34 ReadProfileID_Com;
PFunc_Model_Type41 StartFirmwareLoading_Com;										// 开始固件加载
PFunc_Model_Type42 LoadNewFirmware_Com;												// 加载新固件
PFunc_Model_Type43 SwitchtoNewFirmware_Com;											// 切换新固件
PFunc_Model_Type51 SetModuleInfo1_Com;
PFunc_Model_Type52 SetModuleInfo2_Com;
PFunc_Model_Type53 SetModuleInfo3_Com;
PFunc_Model_Type54 SetModuleInfo4_Com;

PFunc_Model_Type101 GetSerialNumber_Net;											// 获得序号
PFunc_Model_Type102 GetType_Net;													// 获得类型
PFunc_Model_Type103 GetVersion_Net;													// 获得版本号
PFunc_Model_Type104 SetRS232BaudRate_Net;											// 获得串口速率
PFunc_Model_Type105 GetHardwareDateCode_Net;										// 获得硬件版本号
PFunc_Model_Type106 GetFirmwareDateCode_Net;										// 获得固件版本号
PFunc_Model_Type107 GetManufactureID_Net;											// 获得制造ID
PFunc_Model_Type108 GetSerialNumberSyn_Net;		
PFunc_Model_Type181 SetDACVoltageSyn_Net;											// 设置通道 轴(x:0 y:1) 电压
PFunc_Model_Type111 GetAlarm_Net;													// 获取告警
PFunc_Model_Type112 GetVoltage_Net;													// 获取电压
PFunc_Model_Type113 GetTemperature_Net;												// 获取温度
PFunc_Model_Type114 GetTemperatureSyn_Net;								
PFunc_Model_Type115 SetLowTemperatureAlarmThreshold_Net;							// 设置低温度告警门限
PFunc_Model_Type116 GetLowTemperatureAlarmThreshold_Net;							// 获取低温度告警门限
PFunc_Model_Type117 SetHighTemperatureAlarmThreshold_Net;							// 设置高温度告警门限
PFunc_Model_Type118 GetHighTemperatureAlarmThreshold_Net;							// 获取高温度告警门限
PFunc_Model_Type121 SetChannelPortAtten_Net;										// 设置通道port衰减
PFunc_Model_Type122 GetChannelPortAtten_Net;										// 获取通道衰减
PFunc_Model_Type123 SetAllChannelsPortAtten_Net;									// 设置所有通道port衰减
PFunc_Model_Type124 GetAllChannelsPortAtten_Net;									// 获取所有通道衰减
PFunc_Model_Type151 SetModuleInfo1_Net;
PFunc_Model_Type152 SetModuleInfo2_Net;
PFunc_Model_Type153 SetModuleInfo3_Net;
PFunc_Model_Type154 SetModuleInfo4_Net;

PFunc_Model_Type00 StartLoadNewFirmwareProcess;






bool LoadWSSProBaseDLL()
{
	#define CheckNULL( obj )  if( obj==NULL ) return false;

	HMODULE handle = ::LoadLibraryA( "wss_proBase.dll" );
	CheckNULL( handle )

	// 功能接口获得
	OpenSerial									= ( PFunc_Model_Type91 )GetProcAddress( handle, "OpenSerial");
	SetSerialRate								= ( PFunc_Model_Type92 )GetProcAddress( handle, "SetSerialRate");
	IsSerialOpen								= ( PFunc_Model_Type93 )GetProcAddress( handle, "IsSerialOpen");
	SetDACVoltage_Com							= ( PFunc_Model_Type81 )GetProcAddress( handle, "SetDACVoltage_Com");
	SetDACVoltageSyn_Com						= ( PFunc_Model_Type82 )GetProcAddress( handle, "SetDACVoltageSyn_Com");
	GetSerialNumber_Com							= ( PFunc_Model_Type01 )GetProcAddress( handle, "GetSerialNumber_Com");
	GetType_Com									= ( PFunc_Model_Type02 )GetProcAddress( handle, "GetType_Com");
	GetVersion_Com								= ( PFunc_Model_Type03 )GetProcAddress( handle, "GetVersion_Com");
	SetRS232BaudRate_Com						= ( PFunc_Model_Type04 )GetProcAddress( handle, "SetRS232BaudRate_Com");
	GetHardwareDateCode_Com						= ( PFunc_Model_Type05 )GetProcAddress( handle, "GetHardwareDateCode_Com");
	GetFirmwareDateCode_Com						= ( PFunc_Model_Type06 )GetProcAddress( handle, "GetFirmwareDateCode_Com");
	GetManufactureID_Com						= ( PFunc_Model_Type07 )GetProcAddress( handle, "GetManufactureID_Com");
	GetAD5535Syn_Com							= ( PFunc_Model_Type08 )GetProcAddress( handle, "GetAD5535Syn_Com");
	GetAlarm_Com								= ( PFunc_Model_Type11 )GetProcAddress( handle, "GetAlarm_Com");
	GetVoltage_Com								= ( PFunc_Model_Type12 )GetProcAddress( handle, "GetVoltage_Com");
	GetTemperature_Com							= ( PFunc_Model_Type13 )GetProcAddress( handle, "GetTemperature_Com");
	GetTemperatureSyn_Com						= ( PFunc_Model_Type18 )GetProcAddress( handle, "GetTemperatureSyn_Com");
	SetLowTemperatureAlarmThreshold_Com			= ( PFunc_Model_Type14 )GetProcAddress( handle, "SetLowTemperatureAlarmThreshold_Com");
	GetLowTemperatureAlarmThreshold_Com			= ( PFunc_Model_Type15 )GetProcAddress( handle, "GetLowTemperatureAlarmThreshold_Com");
	SetHighTemperatureAlarmThreshold_Com		= ( PFunc_Model_Type16 )GetProcAddress( handle, "SetHighTemperatureAlarmThreshold_Com");
	GetHighTemperatureAlarmThreshold_Com		= ( PFunc_Model_Type17 )GetProcAddress( handle, "GetHighTemperatureAlarmThreshold_Com");
	SetChannelPortAtten_Com						= ( PFunc_Model_Type21 )GetProcAddress( handle, "SetChannelPortAtten_Com");
	GetChannelPortAtten_Com						= ( PFunc_Model_Type22 )GetProcAddress( handle, "GetChannelPortAtten_Com");
	SetAllChannelsPortAtten_Com					= ( PFunc_Model_Type23 )GetProcAddress( handle, "SetAllChannelsPortAtten_Com");
	GetAllChannelsPortAtten_Com					= ( PFunc_Model_Type24 )GetProcAddress( handle, "GetAllChannelsPortAtten_Com");
	GenerateChannelProfile_Com					= ( PFunc_Model_Type31 )GetProcAddress( handle, "GenerateChannelProfile_Com");
	ReadChannelProfile_Com						= ( PFunc_Model_Type32 )GetProcAddress( handle, "ReadChannelProfile_Com");	
	SelectChannelProfile_Com					= ( PFunc_Model_Type33 )GetProcAddress( handle, "SelectChannelProfile_Com");
	ReadProfileID_Com							= ( PFunc_Model_Type34 )GetProcAddress( handle, "ReadProfileID_Com");
	StartFirmwareLoading_Com					= ( PFunc_Model_Type41 )GetProcAddress( handle, "StartFirmwareLoading_Com");
	LoadNewFirmware_Com							= ( PFunc_Model_Type42 )GetProcAddress( handle, "LoadNewFirmware_Com");
	SwitchtoNewFirmware_Com						= ( PFunc_Model_Type43 )GetProcAddress( handle, "SwitchtoNewFirmware_Com");
	SetModuleInfo1_Com							= ( PFunc_Model_Type51 )GetProcAddress( handle, "SetModuleInfo1_Com");
	SetModuleInfo2_Com							= ( PFunc_Model_Type52 )GetProcAddress( handle, "SetModuleInfo2_Com");
	SetModuleInfo3_Com							= ( PFunc_Model_Type53 )GetProcAddress( handle, "SetModuleInfo3_Com");
	SetModuleInfo4_Com							= ( PFunc_Model_Type54 )GetProcAddress( handle, "SetModuleInfo4_Com");


	GetSerialNumber_Net							= ( PFunc_Model_Type101 )GetProcAddress( handle, "GetSerialNumber_Net");
	GetType_Net									= ( PFunc_Model_Type102 )GetProcAddress( handle, "GetType_Net");
	GetVersion_Net								= ( PFunc_Model_Type103 )GetProcAddress( handle, "GetVersion_Net");
	SetRS232BaudRate_Net						= ( PFunc_Model_Type104 )GetProcAddress( handle, "SetRS232BaudRate_Net");
	GetHardwareDateCode_Net						= ( PFunc_Model_Type105 )GetProcAddress( handle, "GetHardwareDateCode_Net");
	GetFirmwareDateCode_Net						= ( PFunc_Model_Type106 )GetProcAddress( handle, "GetFirmwareDateCode_Net");
	GetManufactureID_Net						= ( PFunc_Model_Type107 )GetProcAddress( handle, "GetManufactureID_Net");
	GetSerialNumberSyn_Net						= ( PFunc_Model_Type108 )GetProcAddress( handle, "GetSerialNumberSyn_Net");		
	SetDACVoltageSyn_Net						= ( PFunc_Model_Type181 )GetProcAddress( handle, "SetDACVoltageSyn_Net");
	GetAlarm_Net								= ( PFunc_Model_Type111 )GetProcAddress( handle, "GetAlarm_Net");
	GetVoltage_Net								= ( PFunc_Model_Type112 )GetProcAddress( handle, "GetVoltage_Net");
	GetTemperature_Net							= ( PFunc_Model_Type113 )GetProcAddress( handle, "GetTemperature_Net");
	GetTemperatureSyn_Net						= ( PFunc_Model_Type114 )GetProcAddress( handle, "GetTemperatureSyn_Net");							
	SetLowTemperatureAlarmThreshold_Net			= ( PFunc_Model_Type115 )GetProcAddress( handle, "SetLowTemperatureAlarmThreshold_Net");
	GetLowTemperatureAlarmThreshold_Net			= ( PFunc_Model_Type116 )GetProcAddress( handle, "GetLowTemperatureAlarmThreshold_Net");
	SetHighTemperatureAlarmThreshold_Net		= ( PFunc_Model_Type117 )GetProcAddress( handle, "SetHighTemperatureAlarmThreshold_Net");
	GetHighTemperatureAlarmThreshold_Net		= ( PFunc_Model_Type118 )GetProcAddress( handle, "GetHighTemperatureAlarmThreshold_Net");
	SetChannelPortAtten_Net						= ( PFunc_Model_Type121 )GetProcAddress( handle, "SetChannelPortAtten_Net");
	GetChannelPortAtten_Net						= ( PFunc_Model_Type122 )GetProcAddress( handle, "GetChannelPortAtten_Net");
	SetAllChannelsPortAtten_Net					= ( PFunc_Model_Type123 )GetProcAddress( handle, "SetAllChannelsPortAtten_Net");
	GetAllChannelsPortAtten_Net					= ( PFunc_Model_Type124 )GetProcAddress( handle, "GetAllChannelsPortAtten_Net");
	SetModuleInfo1_Net							= ( PFunc_Model_Type151 )GetProcAddress( handle, "SetModuleInfo1_Net");
	SetModuleInfo2_Net							= ( PFunc_Model_Type152 )GetProcAddress( handle, "SetModuleInfo2_Net");
	SetModuleInfo3_Net							= ( PFunc_Model_Type153 )GetProcAddress( handle, "SetModuleInfo3_Net");
	SetModuleInfo4_Net							= ( PFunc_Model_Type154 )GetProcAddress( handle, "SetModuleInfo4_Net");

	StartLoadNewFirmwareProcess					= ( PFunc_Model_Type00 )GetProcAddress( handle, "StartLoadNewFirmwareProcess");


	// view回调注册函数
	SetCB_View_ShowMsg							= ( PFunc_SetView_Type0 )GetProcAddress( handle, "SetCB_View_ShowMsg");
	SetCB_View_ShowSerialNumber					= ( PFunc_SetView_Type1 )GetProcAddress( handle, "SetCB_View_ShowSerialNumber");
	SetCB_View_ShowType							= ( PFunc_SetView_Type2 )GetProcAddress( handle, "SetCB_View_ShowType");
	SetCB_View_ShowVersion						= ( PFunc_SetView_Type3 )GetProcAddress( handle, "SetCB_View_ShowVersion");
	SetCB_View_ShowHardDateCode					= ( PFunc_SetView_Type4 )GetProcAddress( handle, "SetCB_View_ShowHardDateCode");
	SetCB_View_ShowFirmwareDateCode				= ( PFunc_SetView_Type5 )GetProcAddress( handle, "SetCB_View_ShowFirmwareDateCode");
	SetCB_View_ShowChannelAtten					= ( PFunc_SetView_Type6 )GetProcAddress( handle, "SetCB_View_ShowChannelAtten");
	SetCB_View_ShowAllChannelAtten				= ( PFunc_SetView_Type7 )GetProcAddress( handle, "SetCB_View_ShowAllChannelAtten");
	SetCB_View_ShowProfileID					= ( PFunc_SetView_Type8 )GetProcAddress( handle, "SetCB_View_ShowProfileID");
	SetCB_View_ShowAlarm						= ( PFunc_SetView_Type9 )GetProcAddress( handle, "SetCB_View_ShowAlarm");
	SetCB_View_ShowVoltage						= ( PFunc_SetView_Type10 )GetProcAddress( handle, "SetCB_View_ShowVoltage");
	SetCB_View_ShowTemperature					= ( PFunc_SetView_Type11 )GetProcAddress( handle, "SetCB_View_ShowTemperature");
	SetCB_View_ShowLowTemperature				= ( PFunc_SetView_Type12 )GetProcAddress( handle, "SetCB_View_ShowLowTemperature");
	SetCB_View_ShowHighTemperature				= ( PFunc_SetView_Type13 )GetProcAddress( handle, "SetCB_View_ShowHighTemperature");
	SetCB_View_ShowMemWear						= ( PFunc_SetView_Type14 )GetProcAddress( handle, "SetCB_View_ShowMemWear");
	SetCB_View_ShowManufactureID				= ( PFunc_SetView_Type15 )GetProcAddress( handle, "SetCB_View_ShowManufactureID");

	return true;
}	