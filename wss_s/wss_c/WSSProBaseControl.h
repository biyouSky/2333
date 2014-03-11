
#pragma once

/*-------------------------------------------view �ص�ע�ắ��-------------------------------------------*/
// view �ص��ӿ�
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

// view �ص�ע��ӿ�
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




/*-------------------------------------------���ܺ����ӿ�-------------------------------------------*/

typedef bool (*PFunc_Model_Type91)( DWORD dwPort, DWORD dwBaudRate );
typedef bool (*PFunc_Model_Type92)( DWORD baudRate );
typedef bool (*PFunc_Model_Type93)();
typedef void (*PFunc_Model_Type81)( UINT channel, int coord, USHORT vol );
typedef bool (*PFunc_Model_Type82)( UINT channel, int coord, USHORT vol );
typedef void (*PFunc_Model_Type01)();												// ������
typedef void (*PFunc_Model_Type02)();												// �������
typedef void (*PFunc_Model_Type03)();												// ��ð汾��
typedef void (*PFunc_Model_Type04)( int baudRate );									// ��ô�������
typedef void (*PFunc_Model_Type05)();												// ���Ӳ���汾��
typedef void (*PFunc_Model_Type06)();												// ��ù̼��汾��
typedef void (*PFunc_Model_Type07)();												// �������ID
typedef bool (*PFunc_Model_Type08)( int c, USHORT& v );	
typedef void (*PFunc_Model_Type11)();												// ��ȡ�澯
typedef void (*PFunc_Model_Type12)();												// ��ȡ��ѹ
typedef void (*PFunc_Model_Type13)();												// ��ȡ�¶�
typedef void (*PFunc_Model_Type14)( float temperature );							// ���õ��¶ȸ澯����
typedef void (*PFunc_Model_Type15)();												// ��ȡ���¶ȸ澯����
typedef void (*PFunc_Model_Type16)( float temperature );							// ���ø��¶ȸ澯����
typedef void (*PFunc_Model_Type17)();												// ��ȡ���¶ȸ澯����
typedef bool (*PFunc_Model_Type18)( float& temp );												
typedef void (*PFunc_Model_Type21)( int c, UCHAR p, float d );						// ����ͨ��port˥��
typedef void (*PFunc_Model_Type22)( int c );										// ��ȡͨ��˥��
typedef void (*PFunc_Model_Type23)( UCHAR *p, float *d, int n );					// ��������ͨ��port˥��
typedef void (*PFunc_Model_Type24)();												// ��ȡ����ͨ��˥��
typedef void (*PFunc_Model_Type31)( UCHAR pID, UCHAR portList[], UCHAR attenList[] );// profile
typedef void (*PFunc_Model_Type32)( UCHAR pID );							
typedef void (*PFunc_Model_Type33)( UCHAR pID );
typedef void (*PFunc_Model_Type34)();
typedef void (*PFunc_Model_Type41)();												// ��ʼ�̼�����
typedef void (*PFunc_Model_Type42)();												// �����¹̼�
typedef void (*PFunc_Model_Type43)();												// �л��¹̼�
// �ڲ�����
typedef void (*PFunc_Model_Type51)( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
typedef void (*PFunc_Model_Type52)( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
typedef void (*PFunc_Model_Type53)( USHORT volList[104] );
typedef void (*PFunc_Model_Type54)( UCHAR port, UCHAR f, USHORT volList[104] );

typedef void (*PFunc_Model_Type101)( ULONG ip );									// ������
typedef void (*PFunc_Model_Type102)( ULONG ip );									// �������
typedef void (*PFunc_Model_Type103)( ULONG ip );									// ��ð汾��
typedef void (*PFunc_Model_Type104)( ULONG ip, int baudRate );						// ��ô�������
typedef void (*PFunc_Model_Type105)( ULONG ip );									// ���Ӳ���汾��
typedef void (*PFunc_Model_Type106)( ULONG ip );									// ��ù̼��汾��
typedef void (*PFunc_Model_Type107)( ULONG ip );									// �������ID
typedef bool (*PFunc_Model_Type108)( ULONG ip, int* sn );										
typedef bool (*PFunc_Model_Type181)( ULONG ip, UINT channel, int coord, USHORT vol );// ����ͨ�� ��(x:0 y:1) ��ѹ
typedef void (*PFunc_Model_Type111)( ULONG ip );									// ��ȡ�澯
typedef void (*PFunc_Model_Type112)( ULONG ip );									// ��ȡ��ѹ
typedef void (*PFunc_Model_Type113)( ULONG ip );									// ��ȡ�¶�
typedef bool (*PFunc_Model_Type114)( ULONG ip, float* temp );	
typedef void (*PFunc_Model_Type115)( ULONG ip, float temperature );					// ���õ��¶ȸ澯����
typedef void (*PFunc_Model_Type116)( ULONG ip );									// ��ȡ���¶ȸ澯����
typedef void (*PFunc_Model_Type117)( ULONG ip, float temperature );					// ���ø��¶ȸ澯����
typedef void (*PFunc_Model_Type118)( ULONG ip );									// ��ȡ���¶ȸ澯����
typedef void (*PFunc_Model_Type121)( ULONG ip, int c, UCHAR p, float d );			// ����ͨ��port˥��
typedef void (*PFunc_Model_Type122)( ULONG ip, int c );								// ��ȡͨ��˥��
typedef void (*PFunc_Model_Type123)( ULONG ip, UCHAR *p, float *d, int n );			// ��������ͨ��port˥��
typedef void (*PFunc_Model_Type124)( ULONG ip );									// ��ȡ����ͨ��˥��
// �ڲ�����						
typedef void (*PFunc_Model_Type151)( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
typedef void (*PFunc_Model_Type152)( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
typedef void (*PFunc_Model_Type153)( ULONG ip, USHORT volList[104] );
typedef void (*PFunc_Model_Type154)( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );

typedef bool (*PFunc_Model_Type00)();												// �̼�	







extern PFunc_Model_Type91 OpenSerial;
extern PFunc_Model_Type92 SetSerialRate;
extern PFunc_Model_Type93 IsSerialOpen;
extern PFunc_Model_Type81 SetDACVoltage_Com;
extern PFunc_Model_Type82 SetDACVoltageSyn_Com;
extern PFunc_Model_Type01 GetSerialNumber_Com;											// ������
extern PFunc_Model_Type02 GetType_Com;													// �������
extern PFunc_Model_Type03 GetVersion_Com;												// ��ð汾��
extern PFunc_Model_Type04 SetRS232BaudRate_Com;											// ��ô�������
extern PFunc_Model_Type05 GetHardwareDateCode_Com;										// ���Ӳ���汾��
extern PFunc_Model_Type06 GetFirmwareDateCode_Com;										// ��ù̼��汾��
extern PFunc_Model_Type07 GetManufactureID_Com;											// �������ID
extern PFunc_Model_Type08 GetAD5535Syn_Com;	
extern PFunc_Model_Type11 GetAlarm_Com;													// ��ȡ�澯
extern PFunc_Model_Type12 GetVoltage_Com;												// ��ȡ��ѹ
extern PFunc_Model_Type13 GetTemperature_Com;											// ��ȡ�¶�
extern PFunc_Model_Type18 GetTemperatureSyn_Com;
extern PFunc_Model_Type14 SetLowTemperatureAlarmThreshold_Com;							// ���õ��¶ȸ澯����
extern PFunc_Model_Type15 GetLowTemperatureAlarmThreshold_Com;							// ��ȡ���¶ȸ澯����
extern PFunc_Model_Type16 SetHighTemperatureAlarmThreshold_Com;							// ���ø��¶ȸ澯����
extern PFunc_Model_Type17 GetHighTemperatureAlarmThreshold_Com;							// ��ȡ���¶ȸ澯����
extern PFunc_Model_Type21 SetChannelPortAtten_Com;										// ����ͨ��port˥��
extern PFunc_Model_Type22 GetChannelPortAtten_Com;										// ��ȡͨ��˥��
extern PFunc_Model_Type23 SetAllChannelsPortAtten_Com;									// ��������ͨ��port˥��
extern PFunc_Model_Type24 GetAllChannelsPortAtten_Com;									// ��ȡ����ͨ��˥��
extern PFunc_Model_Type31 GenerateChannelProfile_Com;
extern PFunc_Model_Type32 ReadChannelProfile_Com;							
extern PFunc_Model_Type33 SelectChannelProfile_Com;
extern PFunc_Model_Type34 ReadProfileID_Com;
extern PFunc_Model_Type41 StartFirmwareLoading_Com;										// ��ʼ�̼�����
extern PFunc_Model_Type42 LoadNewFirmware_Com;											// �����¹̼�
extern PFunc_Model_Type43 SwitchtoNewFirmware_Com;										// �л��¹̼�
extern PFunc_Model_Type51 SetModuleInfo1_Com;
extern PFunc_Model_Type52 SetModuleInfo2_Com;
extern PFunc_Model_Type53 SetModuleInfo3_Com;
extern PFunc_Model_Type54 SetModuleInfo4_Com;


extern PFunc_Model_Type101 GetSerialNumber_Net;											// ������
extern PFunc_Model_Type102 GetType_Net;													// �������
extern PFunc_Model_Type103 GetVersion_Net;												// ��ð汾��
extern PFunc_Model_Type104 SetRS232BaudRate_Net;										// ��ô�������
extern PFunc_Model_Type105 GetHardwareDateCode_Net;										// ���Ӳ���汾��
extern PFunc_Model_Type106 GetFirmwareDateCode_Net;										// ��ù̼��汾��
extern PFunc_Model_Type107 GetManufactureID_Net;										// �������ID
extern PFunc_Model_Type108 GetSerialNumberSyn_Net;		
extern PFunc_Model_Type181 SetDACVoltageSyn_Net;										// ����ͨ�� ��(x:0 y:1) ��ѹ
extern PFunc_Model_Type111 GetAlarm_Net;												// ��ȡ�澯
extern PFunc_Model_Type112 GetVoltage_Net;												// ��ȡ��ѹ
extern PFunc_Model_Type113 GetTemperature_Net;											// ��ȡ�¶�
extern PFunc_Model_Type114 GetTemperatureSyn_Net;								
extern PFunc_Model_Type115 SetLowTemperatureAlarmThreshold_Net;							// ���õ��¶ȸ澯����
extern PFunc_Model_Type116 GetLowTemperatureAlarmThreshold_Net;							// ��ȡ���¶ȸ澯����
extern PFunc_Model_Type117 SetHighTemperatureAlarmThreshold_Net;						// ���ø��¶ȸ澯����
extern PFunc_Model_Type118 GetHighTemperatureAlarmThreshold_Net;						// ��ȡ���¶ȸ澯����
extern PFunc_Model_Type121 SetChannelPortAtten_Net;										// ����ͨ��port˥��
extern PFunc_Model_Type122 GetChannelPortAtten_Net;										// ��ȡͨ��˥��
extern PFunc_Model_Type123 SetAllChannelsPortAtten_Net;									// ��������ͨ��port˥��
extern PFunc_Model_Type124 GetAllChannelsPortAtten_Net;									// ��ȡ����ͨ��˥��
extern PFunc_Model_Type151 SetModuleInfo1_Net;
extern PFunc_Model_Type152 SetModuleInfo2_Net;
extern PFunc_Model_Type153 SetModuleInfo3_Net;
extern PFunc_Model_Type154 SetModuleInfo4_Net;

extern PFunc_Model_Type00 StartLoadNewFirmwareProcess;





bool LoadWSSProBaseDLL();