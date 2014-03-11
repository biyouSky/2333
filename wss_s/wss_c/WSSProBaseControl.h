
#pragma once

/*-------------------------------------------view 回调注册函数-------------------------------------------*/
// view 回调接口
typedef void (*PFunc_View_Type0)( const char* msg );
typedef void (*PFunc_View_Type1)( int serialNumber );
typedef void (*PFunc_View_Type2)( int type );
typedef void (*PFunc_View_Type3)( UCHAR hardVH, UCHAR hardVL, UCHAR firmVH, UCHAR firmVL );
typedef void (*PFunc_View_Type4)( UCHAR hardDateCodeH, UCHAR hardDateCodeL );
typedef void (*PFunc_View_Type5)( UCHAR FirmwareDateCodeH, UCHAR FirmwareDateCodeL );
typedef void (*PFunc_View_Type6)( UCHAR p, UCHAR d );
typedef void (*PFunc_View_Type7)( UCHAR *p, float *d );
typedef void (*PFunc_View_Type8)( UCHAR pID );
typedef void (*PFunc_View_Type9)( UCHAR d );
typedef void (*PFunc_View_Type10)( USHORT vol );
typedef void (*PFunc_View_Type11)( float temp );
typedef void (*PFunc_View_Type12)( float temp );
typedef void (*PFunc_View_Type13)( float temp );
typedef void (*PFunc_View_Type14)( UCHAR d );
typedef void (*PFunc_View_Type15)( UCHAR d );

// view 回调注册接口
typedef void (*PFunc_SetView_Type0)( PFunc_View_Type0 func );
typedef void (*PFunc_SetView_Type1)( PFunc_View_Type1 func );
typedef void (*PFunc_SetView_Type2)( PFunc_View_Type2 func );
typedef void (*PFunc_SetView_Type3)( PFunc_View_Type3 func );
typedef void (*PFunc_SetView_Type4)( PFunc_View_Type4 func );
typedef void (*PFunc_SetView_Type5)( PFunc_View_Type5 func );
typedef void (*PFunc_SetView_Type6)( PFunc_View_Type6 func );
typedef void (*PFunc_SetView_Type7)( PFunc_View_Type7 func );
typedef void (*PFunc_SetView_Type8)( PFunc_View_Type8 func );
typedef void (*PFunc_SetView_Type9)( PFunc_View_Type9 func );
typedef void (*PFunc_SetView_Type10)( PFunc_View_Type10 func );
typedef void (*PFunc_SetView_Type11)( PFunc_View_Type11 func );
typedef void (*PFunc_SetView_Type12)( PFunc_View_Type12 func );
typedef void (*PFunc_SetView_Type13)( PFunc_View_Type13 func );
typedef void (*PFunc_SetView_Type14)( PFunc_View_Type14 func );
typedef void (*PFunc_SetView_Type15)( PFunc_View_Type15 func );


extern PFunc_SetView_Type0 SetCB_View_ShowMsg;
extern PFunc_SetView_Type1 SetCB_View_ShowSerialNumber;
extern PFunc_SetView_Type2 SetCB_View_ShowType;
extern PFunc_SetView_Type3 SetCB_View_ShowVersion;
extern PFunc_SetView_Type4 SetCB_View_ShowHardDateCode;
extern PFunc_SetView_Type5 SetCB_View_ShowFirmwareDateCode;
extern PFunc_SetView_Type6 SetCB_View_ShowChannelAtten;
extern PFunc_SetView_Type7 SetCB_View_ShowAllChannelAtten;
extern PFunc_SetView_Type8 SetCB_View_ShowProfileID;
extern PFunc_SetView_Type9 SetCB_View_ShowAlarm;
extern PFunc_SetView_Type10 SetCB_View_ShowVoltage;
extern PFunc_SetView_Type11 SetCB_View_ShowTemperature;
extern PFunc_SetView_Type12 SetCB_View_ShowLowTemperature;
extern PFunc_SetView_Type13 SetCB_View_ShowHighTemperature;
extern PFunc_SetView_Type14 SetCB_View_ShowMemWear;
extern PFunc_SetView_Type15 SetCB_View_ShowManufactureID;




/*-------------------------------------------功能函数接口-------------------------------------------*/

typedef bool (*PFunc_Model_Type91)( DWORD dwPort, DWORD dwBaudRate );
typedef bool (*PFunc_Model_Type92)( DWORD baudRate );
typedef bool (*PFunc_Model_Type93)();
typedef void (*PFunc_Model_Type81)( UINT channel, int coord, USHORT vol );
typedef bool (*PFunc_Model_Type82)( UINT channel, int coord, USHORT vol );
typedef void (*PFunc_Model_Type01)();												// 获得序号
typedef void (*PFunc_Model_Type02)();												// 获得类型
typedef void (*PFunc_Model_Type03)();												// 获得版本号
typedef void (*PFunc_Model_Type04)( int baudRate );									// 获得串口速率
typedef void (*PFunc_Model_Type05)();												// 获得硬件版本号
typedef void (*PFunc_Model_Type06)();												// 获得固件版本号
typedef void (*PFunc_Model_Type07)();												// 获得制造ID
typedef bool (*PFunc_Model_Type08)( int c, USHORT& v );	
typedef void (*PFunc_Model_Type11)();												// 获取告警
typedef void (*PFunc_Model_Type12)();												// 获取电压
typedef void (*PFunc_Model_Type13)();												// 获取温度
typedef void (*PFunc_Model_Type14)( float temperature );							// 设置低温度告警门限
typedef void (*PFunc_Model_Type15)();												// 获取低温度告警门限
typedef void (*PFunc_Model_Type16)( float temperature );							// 设置高温度告警门限
typedef void (*PFunc_Model_Type17)();												// 获取高温度告警门限
typedef bool (*PFunc_Model_Type18)( float& temp );												
typedef void (*PFunc_Model_Type21)( int c, UCHAR p, float d );						// 设置通道port衰减
typedef void (*PFunc_Model_Type22)( int c );										// 获取通道衰减
typedef void (*PFunc_Model_Type23)( UCHAR *p, float *d, int n );					// 设置所有通道port衰减
typedef void (*PFunc_Model_Type24)();												// 获取所有通道衰减
typedef void (*PFunc_Model_Type31)( UCHAR pID, UCHAR portList[], UCHAR attenList[] );// profile
typedef void (*PFunc_Model_Type32)( UCHAR pID );							
typedef void (*PFunc_Model_Type33)( UCHAR pID );
typedef void (*PFunc_Model_Type34)();
typedef void (*PFunc_Model_Type41)();												// 开始固件加载
typedef void (*PFunc_Model_Type42)();												// 加载新固件
typedef void (*PFunc_Model_Type43)();												// 切换新固件
// 内部命令
typedef void (*PFunc_Model_Type51)( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
typedef void (*PFunc_Model_Type52)( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
typedef void (*PFunc_Model_Type53)( USHORT volList[104] );
typedef void (*PFunc_Model_Type54)( UCHAR port, UCHAR f, USHORT volList[104] );

typedef void (*PFunc_Model_Type101)( ULONG ip );									// 获得序号
typedef void (*PFunc_Model_Type102)( ULONG ip );									// 获得类型
typedef void (*PFunc_Model_Type103)( ULONG ip );									// 获得版本号
typedef void (*PFunc_Model_Type104)( ULONG ip, int baudRate );						// 获得串口速率
typedef void (*PFunc_Model_Type105)( ULONG ip );									// 获得硬件版本号
typedef void (*PFunc_Model_Type106)( ULONG ip );									// 获得固件版本号
typedef void (*PFunc_Model_Type107)( ULONG ip );									// 获得制造ID
typedef bool (*PFunc_Model_Type108)( ULONG ip, int* sn );										
typedef bool (*PFunc_Model_Type181)( ULONG ip, UINT channel, int coord, USHORT vol );// 设置通道 轴(x:0 y:1) 电压
typedef void (*PFunc_Model_Type111)( ULONG ip );									// 获取告警
typedef void (*PFunc_Model_Type112)( ULONG ip );									// 获取电压
typedef void (*PFunc_Model_Type113)( ULONG ip );									// 获取温度
typedef bool (*PFunc_Model_Type114)( ULONG ip, float* temp );	
typedef void (*PFunc_Model_Type115)( ULONG ip, float temperature );					// 设置低温度告警门限
typedef void (*PFunc_Model_Type116)( ULONG ip );									// 获取低温度告警门限
typedef void (*PFunc_Model_Type117)( ULONG ip, float temperature );					// 设置高温度告警门限
typedef void (*PFunc_Model_Type118)( ULONG ip );									// 获取高温度告警门限
typedef void (*PFunc_Model_Type121)( ULONG ip, int c, UCHAR p, float d );			// 设置通道port衰减
typedef void (*PFunc_Model_Type122)( ULONG ip, int c );								// 获取通道衰减
typedef void (*PFunc_Model_Type123)( ULONG ip, UCHAR *p, float *d, int n );			// 设置所有通道port衰减
typedef void (*PFunc_Model_Type124)( ULONG ip );									// 获取所有通道衰减
// 内部命令						
typedef void (*PFunc_Model_Type151)( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
typedef void (*PFunc_Model_Type152)( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
typedef void (*PFunc_Model_Type153)( ULONG ip, USHORT volList[104] );
typedef void (*PFunc_Model_Type154)( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );

typedef bool (*PFunc_Model_Type00)();												// 固件	







extern PFunc_Model_Type91 OpenSerial;
extern PFunc_Model_Type92 SetSerialRate;
extern PFunc_Model_Type93 IsSerialOpen;
extern PFunc_Model_Type81 SetDACVoltage_Com;
extern PFunc_Model_Type82 SetDACVoltageSyn_Com;
extern PFunc_Model_Type01 GetSerialNumber_Com;											// 获得序号
extern PFunc_Model_Type02 GetType_Com;													// 获得类型
extern PFunc_Model_Type03 GetVersion_Com;												// 获得版本号
extern PFunc_Model_Type04 SetRS232BaudRate_Com;											// 获得串口速率
extern PFunc_Model_Type05 GetHardwareDateCode_Com;										// 获得硬件版本号
extern PFunc_Model_Type06 GetFirmwareDateCode_Com;										// 获得固件版本号
extern PFunc_Model_Type07 GetManufactureID_Com;											// 获得制造ID
extern PFunc_Model_Type08 GetAD5535Syn_Com;	
extern PFunc_Model_Type11 GetAlarm_Com;													// 获取告警
extern PFunc_Model_Type12 GetVoltage_Com;												// 获取电压
extern PFunc_Model_Type13 GetTemperature_Com;											// 获取温度
extern PFunc_Model_Type18 GetTemperatureSyn_Com;
extern PFunc_Model_Type14 SetLowTemperatureAlarmThreshold_Com;							// 设置低温度告警门限
extern PFunc_Model_Type15 GetLowTemperatureAlarmThreshold_Com;							// 获取低温度告警门限
extern PFunc_Model_Type16 SetHighTemperatureAlarmThreshold_Com;							// 设置高温度告警门限
extern PFunc_Model_Type17 GetHighTemperatureAlarmThreshold_Com;							// 获取高温度告警门限
extern PFunc_Model_Type21 SetChannelPortAtten_Com;										// 设置通道port衰减
extern PFunc_Model_Type22 GetChannelPortAtten_Com;										// 获取通道衰减
extern PFunc_Model_Type23 SetAllChannelsPortAtten_Com;									// 设置所有通道port衰减
extern PFunc_Model_Type24 GetAllChannelsPortAtten_Com;									// 获取所有通道衰减
extern PFunc_Model_Type31 GenerateChannelProfile_Com;
extern PFunc_Model_Type32 ReadChannelProfile_Com;							
extern PFunc_Model_Type33 SelectChannelProfile_Com;
extern PFunc_Model_Type34 ReadProfileID_Com;
extern PFunc_Model_Type41 StartFirmwareLoading_Com;										// 开始固件加载
extern PFunc_Model_Type42 LoadNewFirmware_Com;											// 加载新固件
extern PFunc_Model_Type43 SwitchtoNewFirmware_Com;										// 切换新固件
extern PFunc_Model_Type51 SetModuleInfo1_Com;
extern PFunc_Model_Type52 SetModuleInfo2_Com;
extern PFunc_Model_Type53 SetModuleInfo3_Com;
extern PFunc_Model_Type54 SetModuleInfo4_Com;


extern PFunc_Model_Type101 GetSerialNumber_Net;											// 获得序号
extern PFunc_Model_Type102 GetType_Net;													// 获得类型
extern PFunc_Model_Type103 GetVersion_Net;												// 获得版本号
extern PFunc_Model_Type104 SetRS232BaudRate_Net;										// 获得串口速率
extern PFunc_Model_Type105 GetHardwareDateCode_Net;										// 获得硬件版本号
extern PFunc_Model_Type106 GetFirmwareDateCode_Net;										// 获得固件版本号
extern PFunc_Model_Type107 GetManufactureID_Net;										// 获得制造ID
extern PFunc_Model_Type108 GetSerialNumberSyn_Net;		
extern PFunc_Model_Type181 SetDACVoltageSyn_Net;										// 设置通道 轴(x:0 y:1) 电压
extern PFunc_Model_Type111 GetAlarm_Net;												// 获取告警
extern PFunc_Model_Type112 GetVoltage_Net;												// 获取电压
extern PFunc_Model_Type113 GetTemperature_Net;											// 获取温度
extern PFunc_Model_Type114 GetTemperatureSyn_Net;								
extern PFunc_Model_Type115 SetLowTemperatureAlarmThreshold_Net;							// 设置低温度告警门限
extern PFunc_Model_Type116 GetLowTemperatureAlarmThreshold_Net;							// 获取低温度告警门限
extern PFunc_Model_Type117 SetHighTemperatureAlarmThreshold_Net;						// 设置高温度告警门限
extern PFunc_Model_Type118 GetHighTemperatureAlarmThreshold_Net;						// 获取高温度告警门限
extern PFunc_Model_Type121 SetChannelPortAtten_Net;										// 设置通道port衰减
extern PFunc_Model_Type122 GetChannelPortAtten_Net;										// 获取通道衰减
extern PFunc_Model_Type123 SetAllChannelsPortAtten_Net;									// 设置所有通道port衰减
extern PFunc_Model_Type124 GetAllChannelsPortAtten_Net;									// 获取所有通道衰减
extern PFunc_Model_Type151 SetModuleInfo1_Net;
extern PFunc_Model_Type152 SetModuleInfo2_Net;
extern PFunc_Model_Type153 SetModuleInfo3_Net;
extern PFunc_Model_Type154 SetModuleInfo4_Net;

extern PFunc_Model_Type00 StartLoadNewFirmwareProcess;





bool LoadWSSProBaseDLL();