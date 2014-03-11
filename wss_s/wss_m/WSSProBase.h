
#pragma once


/*-------------------------------------------view�ص��ӿ�-------------------------------------------*/

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
extern "C" __declspec(dllexport) void SetCB_View_ShowMsg( PFunc_View_Type0 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowSerialNumber( PFunc_View_Type1 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowType( PFunc_View_Type2 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowVersion( PFunc_View_Type3 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowHardDateCode( PFunc_View_Type4 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowFirmwareDateCode( PFunc_View_Type5 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowChannelAtten( PFunc_View_Type6 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowAllChannelAtten( PFunc_View_Type7 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowProfileID( PFunc_View_Type8 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowAlarm( PFunc_View_Type9 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowVoltage( PFunc_View_Type10 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowTemperature( PFunc_View_Type11 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowLowTemperature( PFunc_View_Type12 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowHighTemperature( PFunc_View_Type13 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowMemWear( PFunc_View_Type14 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowManufactureID( PFunc_View_Type15 func );




/*-------------------------------------------��������Э��-------------------------------------------*/
/*
		�ʼΪ��������  Ϊ���Է���ת�ɴ���ת��������
		������Ϊ��������Э��
*/
// wss��������
struct WSSCommand
{
	UCHAR _code;
	UCHAR _dataLen;
	UCHAR _dataBuf[1024];

	WSSCommand() { memset( this, 0, sizeof( WSSCommand ) ); }
	void CalcCheckSum()
	{
		UCHAR checkSum = 0;
		checkSum += _code;
		checkSum += _dataLen;
		for( int i = 0; i<_dataLen; ++i )
			checkSum += _dataBuf[i];

		_dataBuf[_dataLen] = 0-checkSum;
	}
};

// wss��������
struct WSSResponse
{
	UCHAR status;
	UCHAR _dataLen;
	UCHAR _dataBuf[1024];

	WSSResponse() { memset( this, 0, sizeof( WSSResponse ) ); }
	bool CalcCheckSum()
	{
		UCHAR checkSum = 0;
		checkSum += status;
		checkSum += _dataLen;
		for( int i = 0; i<_dataLen; ++i )
			checkSum += _dataBuf[i];
		checkSum = 0-checkSum;

		return  _dataBuf[_dataLen]==checkSum ? true:false;
	}
};

struct WSSBasePro
{
	/*------------------------------------- �������� -------------------------------------*/
	// ģ����Ϣ
	WSSCommand GetSerialNumber();											// ������
	WSSCommand GetType();													// �������
	WSSCommand GetVersion();												// ��ð汾��
	WSSCommand SetRS232BaudRate( int baudRate );							// ���ô�������
	WSSCommand GetHardwareDateCode();										// ���Ӳ���汾��
	WSSCommand GetFirmwareDateCode();										// ��ù̼��汾��
	WSSCommand GetManufactureID();											// �������ID

	WSSCommand GetAD5535( int c );											// ���AD5535ֵ

	// ͨ���˿�˥��
	WSSCommand SetChannelPortAtten( int c, UCHAR p, float d );				// ����ͨ��port˥��
	WSSCommand GetChannelPortAtten( int c );								// ��ȡͨ��˥��
	WSSCommand SetAllChannelsPortAtten( UCHAR *p, float *d, int n );		// ��������ͨ��port˥��
	WSSCommand GetAllChannelsPortAtten();									// ��ȡ����ͨ��˥��

	// Module Operation Status
	WSSCommand GetAlarm();													// ��ȡ�澯
	WSSCommand GetVoltage();												// ��ȡ��ѹ
	WSSCommand GetTemperature();											// ��ȡ�¶�

	// �澯��Ϣ
	WSSCommand SetLowTemperatureAlarmThreshold( float temperature );		// ���õ��¶ȸ澯����
	WSSCommand GetLowTemperatureAlarmThreshold();							// ��ȡ���¶ȸ澯����
	WSSCommand SetHighTemperatureAlarmThreshold( float temperature );		// ���ø��¶ȸ澯����
	WSSCommand GetHighTemperatureAlarmThreshold();							// ��ȡ���¶ȸ澯����

	// �̼����� ��ϵ������ʵ�ʲ���ת�����ڵ�
	WSSCommand StartFirmwareLoading();										// ��ʼ�̼�����
	WSSCommand LoadNewFirmware();											// �����¹̼�
	WSSCommand SwitchtoNewFirmware();										// �л��¹̼�

	// Set DAC Voltage
	WSSCommand SetDACVoltage( UINT channel, int coord, USHORT vol );		// ����ͨ�� ��(x:0 y:1) ��ѹ

	// �����ļ�
	WSSCommand GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
	WSSCommand ReadChannelProfile( UCHAR pID );							
	WSSCommand SelectChannelProfile( UCHAR pID );
	WSSCommand ReadProfileID();

	// �ڲ�����
	WSSCommand SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	WSSCommand SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	WSSCommand SetModuleInfo3( USHORT volList[104] );
	WSSCommand SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] );

	/*------------------------------------- �����������ʾ���� -------------------------------------*/
	void HandleParseRes( std::string& strRes, UCHAR op, bool& rt, std::string& strMsg, std::vector<Var>& _resultList );
	void HandleShowResult( UCHAR op, bool rt, std::string strMsg, std::vector<Var>& _resultList );
};




/*-------------------------------------------����Э��-------------------------------------------*/
/*
		ֱ�Ӻ�wssͨ��, ���ô����첽��ʽ, �򴮿�ͨ���������Լ�Э��Ķ���, ͬ������ȱ������
		����֮����ܲ��������ݳ�ͻ���⽨�齻���ϲ������ ͬһʱ��ֻ����һ��������в���
*/
class WSSProSeria : public SerialASYN
{
	struct SerialSynData
	{	// Э��ԭ�� ���ܶԲ���OP������
		bool _useFlag;													// ͬ������ʹ�ñ�־
		bool _stopWaitFlag;												// ͬ�����ݵȴ���־
		std::vector<Var> _resultList;									// ���صĽ���б�

		SerialSynData() : _useFlag( false ), _stopWaitFlag( false ) {}
	};
	SerialSynData _synData;												// ͬ������

	UCHAR _code;														// ��ʶ��	
	WSSBasePro _wssBasePro;												// ����Э��

protected:
	bool ParaseRes( std::string& res );									// ��������ظ�

public:
	WSSProSeria();

	// command
	// ģ����Ϣ
	void GetSerialNumber();												// ������
	void GetType();														// �������
	void GetVersion();													// ��ð汾��
	void SetRS232BaudRate( int baudRate );								// ���ô�������
	void GetHardwareDateCode();											// ���Ӳ���汾��
	void GetFirmwareDateCode();											// ��ù̼��汾��
	void GetManufactureID();											// �������ID

	bool GetAD5535Syn( int c, USHORT& v );	

	// ͨ���˿�˥��
	void SetChannelPortAtten( int c, UCHAR p, float d );				// ����ͨ��port˥��
	void GetChannelPortAtten( int c );									// ��ȡͨ��˥��
	void SetAllChannelsPortAtten( UCHAR *p, float *d, int n );			// ��������ͨ��port˥��
	void GetAllChannelsPortAtten();										// ��ȡ����ͨ��˥��

	// Module Operation Status
	void GetAlarm();													// ��ȡ�澯
	void GetVoltage();													// ��ȡ��ѹ
	void GetTemperature();												// ��ȡ�¶�
	//
	bool GetTemperatureSyn( float& temp );		

	// �澯��Ϣ
	void SetLowTemperatureAlarmThreshold( float temperature );			// ���õ��¶ȸ澯����
	void GetLowTemperatureAlarmThreshold();								// ��ȡ���¶ȸ澯����
	void SetHighTemperatureAlarmThreshold( float temperature );			// ���ø��¶ȸ澯����
	void GetHighTemperatureAlarmThreshold();							// ��ȡ���¶ȸ澯����

	// �̼�����
	void StartFirmwareLoading();										// ��ʼ�̼�����
	void LoadNewFirmware();												// �����¹̼�
	void SwitchtoNewFirmware();											// �л��¹̼�
	// 
	bool LoadNewFirmwareSyn();

	// �����ļ�
	void GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
	void ReadChannelProfile( UCHAR pID );							
	void SelectChannelProfile( UCHAR pID );
	void ReadProfileID();

	// Set DAC Voltage
	void SetDACVoltage( UINT channel, int coord, USHORT vol );			// ����ͨ�� ��(x:0 y:1) ��ѹ
	// 
	bool SetDACVoltageSyn( UINT channel, int coord, USHORT vol );

	// �ڲ�����
	void SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	void SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	void SetModuleInfo3( USHORT volList[104] );
	void SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] );
};


/*-------------------------------------------udpЭ��-------------------------------------------*/
/*
		ֱ�ӺͶ����·��ͨ��
		����udpͬ����ʽ, ���ڶ���, ʵ�ּ� �����ʲ�
		�����ڶ��̶߳������趨�������� ��֤ͬʱ����
		ͬһ�����ڶ���߳���ͬʱ���н�����Ȼ�ܽ��� ���ƺ����ֻ������� ԭ��δ�����������
		
		��ΪЭ�鶨��ȱ�� �յ��Ļظ�����op�ַ� ���ܽ�������
		ֻ��ͨ��������֮����и��루����+�����������̣�����֤��������ȷ�� ���ź��������
*/
class WSSProUDP
{	
	UdpOBJ _udpOBJ;

public:
	std::string _debugMsg;
	WSSProUDP();

	/*------------------------------------- ����ת�������� -------------------------------------*/
protected:
	WSSBasePro _wssBasePro;														// ����Э��
	cnCriticalSection _cs;														// ����ת��������֮������Ի���

	bool Func1( UCHAR op, std::vector<Var>& _resultList );						// ���� ����ͨ�ô���		
	
public:
	bool GetSerialNumber( ULONG ip );											// ������
	bool GetType( ULONG ip );													// �������
	bool GetVersion( ULONG ip );												// ��ð汾��
	bool SetRS232BaudRate( ULONG ip, int baudRate );							// ���ô�������
	bool GetHardwareDateCode( ULONG ip );										// ���Ӳ���汾��
	bool GetFirmwareDateCode( ULONG ip );										// ��ù̼��汾��
	bool GetManufactureID( ULONG ip );											// �������ID
	//
	bool GetSerialNumberSyn( ULONG ip, int& sn );

	// ͨ���˿�˥��
	bool SetChannelPortAtten( ULONG ip, int c, UCHAR p, float d );				// ����ͨ��port˥��
	bool GetChannelPortAtten( ULONG ip, int c );								// ��ȡͨ��˥��
	bool SetAllChannelsPortAtten( ULONG ip, UCHAR *p, float *d, int n );		// ��������ͨ��port˥��
	bool GetAllChannelsPortAtten( ULONG ip );									// ��ȡ����ͨ��˥��

	// Module Operation Status
	bool GetAlarm( ULONG ip );													// ��ȡ�澯
	bool GetVoltage( ULONG ip );												// ��ȡ��ѹ
	bool GetTemperature( ULONG ip );											// ��ȡ�¶�
	//
	bool GetTemperatureSyn( ULONG ip, float& temp );

	// �澯��Ϣ
	bool SetLowTemperatureAlarmThreshold( ULONG ip, float temperature );		// ���õ��¶ȸ澯����
	bool GetLowTemperatureAlarmThreshold( ULONG ip );							// ��ȡ���¶ȸ澯����
	bool SetHighTemperatureAlarmThreshold( ULONG ip, float temperature );		// ���ø��¶ȸ澯����
	bool GetHighTemperatureAlarmThreshold( ULONG ip );							// ��ȡ���¶ȸ澯����

	// Set DAC Voltage
	bool SetDACVoltage( ULONG ip, UINT channel, int coord, USHORT vol );		// ����ͨ�� ��(x:0 y:1) ��ѹ
	
	// �ڲ�����
	bool SetModuleInfo1( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	bool SetModuleInfo2( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	bool SetModuleInfo3( ULONG ip, USHORT volList[104] );
	bool SetModuleInfo4( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );
};


/*-------------------------------------------Э����չ-------------------------------------------*/
/*
		
*/
class WSSProEX : public _QueueThreadManage<WSSProEX>
{
	void LoadNewFirmwareThread();

public:
	// �̼�����
	bool StartLoadNewFirmwareThread();			
};


/*-------------------------------------------Э�����-------------------------------------------*/
class WSSProBase
{
public:
	static WSSProBase* GetSingleton();
	static void Destroy();

	WSSProSeria _wssProSeria;
	WSSProUDP _wssProUDP;
	WSSProEX _wssProEX;

private:
	WSSProBase();

	static WSSProBase* _pSingleton;
};



/*-------------------------------------------���ܽӿ�-------------------------------------------*/

/*------------------------------------- ����Э�� -------------------------------------*/
// ���ڿ���
extern "C" __declspec(dllexport) bool OpenSerial( DWORD dwPort, DWORD dwBaudRate=115200 );
extern "C" __declspec(dllexport) bool SetSerialRate( DWORD baudRate );
extern "C" __declspec(dllexport) bool IsSerialOpen();

extern "C" __declspec(dllexport) void GetSerialNumber_Com();												// ������
extern "C" __declspec(dllexport) void GetType_Com();														// �������
extern "C" __declspec(dllexport) void GetVersion_Com();														// ��ð汾��
extern "C" __declspec(dllexport) void SetRS232BaudRate_Com( int baudRate );									// ��ô�������
extern "C" __declspec(dllexport) void GetHardwareDateCode_Com();											// ���Ӳ���汾��
extern "C" __declspec(dllexport) void GetFirmwareDateCode_Com();											// ��ù̼��汾��
extern "C" __declspec(dllexport) void GetManufactureID_Com();												// �������ID

extern "C" __declspec(dllexport) bool GetAD5535Syn_Com( int c, USHORT& v );	

extern "C" __declspec(dllexport) void SetDACVoltage_Com( UINT channel, int coord, USHORT vol );
extern "C" __declspec(dllexport) bool SetDACVoltageSyn_Com( UINT channel, int coord, USHORT vol );

// ͨ���˿�˥��
extern "C" __declspec(dllexport) void SetChannelPortAtten_Com( int c, UCHAR p, float d );					// ����ͨ��port˥��
extern "C" __declspec(dllexport) void GetChannelPortAtten_Com( int c );										// ��ȡͨ��˥��
extern "C" __declspec(dllexport) void SetAllChannelsPortAtten_Com( UCHAR *p, float *d, int n );				// ��������ͨ��port˥��
extern "C" __declspec(dllexport) void GetAllChannelsPortAtten_Com();										// ��ȡ����ͨ��˥��

// Module Operation Status
extern "C" __declspec(dllexport) void GetAlarm_Com();														// ��ȡ�澯
extern "C" __declspec(dllexport) void GetVoltage_Com();														// ��ȡ��ѹ
extern "C" __declspec(dllexport) void GetTemperature_Com();
//
extern "C" __declspec(dllexport) bool GetTemperatureSyn_Com( float& temp );	

// �澯��Ϣ
extern "C" __declspec(dllexport) void SetLowTemperatureAlarmThreshold_Com( float temperature );				// ���õ��¶ȸ澯����
extern "C" __declspec(dllexport) void GetLowTemperatureAlarmThreshold_Com();								// ��ȡ���¶ȸ澯����
extern "C" __declspec(dllexport) void SetHighTemperatureAlarmThreshold_Com( float temperature );			// ���ø��¶ȸ澯����
extern "C" __declspec(dllexport) void GetHighTemperatureAlarmThreshold_Com();								// ��ȡ���¶ȸ澯����

// �̼�����
extern "C" __declspec(dllexport) void StartFirmwareLoading_Com();											// ��ʼ�̼�����
extern "C" __declspec(dllexport) void LoadNewFirmware_Com();												// �����¹̼�
extern "C" __declspec(dllexport) void SwitchtoNewFirmware_Com();											// �л��¹̼�

// �����ļ�
extern "C" __declspec(dllexport) void GenerateChannelProfile_Com( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
extern "C" __declspec(dllexport) void ReadChannelProfile_Com( UCHAR pID );							
extern "C" __declspec(dllexport) void SelectChannelProfile_Com( UCHAR pID );
extern "C" __declspec(dllexport) void ReadProfileID_Com();

// �ڲ�����
extern "C" __declspec(dllexport) void SetModuleInfo1_Com( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
extern "C" __declspec(dllexport) void SetModuleInfo2_Com( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
extern "C" __declspec(dllexport) void SetModuleInfo3_Com( USHORT volList[104] );
extern "C" __declspec(dllexport) void SetModuleInfo4_Com( UCHAR port, UCHAR f, USHORT volList[104] );




/*------------------------------------- ����ת����Э�� -------------------------------------*/
extern "C" __declspec(dllexport) void GetSerialNumber_Net( ULONG ip );										// ������
extern "C" __declspec(dllexport) void GetType_Net( ULONG ip );												// �������
extern "C" __declspec(dllexport) void GetVersion_Net( ULONG ip );											// ��ð汾��
extern "C" __declspec(dllexport) void SetRS232BaudRate_Net( ULONG ip, int baudRate );						// ��ô�������
extern "C" __declspec(dllexport) void GetHardwareDateCode_Net( ULONG ip );									// ���Ӳ���汾��
extern "C" __declspec(dllexport) void GetFirmwareDateCode_Net( ULONG ip );									// ��ù̼��汾��
extern "C" __declspec(dllexport) void GetManufactureID_Net( ULONG ip );										// �������ID
//
extern "C" __declspec(dllexport) bool GetSerialNumberSyn_Net( ULONG ip, int* sn );										

// ͨ���˿�˥��
extern "C" __declspec(dllexport) void SetChannelPortAtten_Net( ULONG ip, int c, UCHAR p, float d );			// ����ͨ��port˥��
extern "C" __declspec(dllexport) void GetChannelPortAtten_Net( ULONG ip, int c );							// ��ȡͨ��˥��
extern "C" __declspec(dllexport) void SetAllChannelsPortAtten_Net( ULONG ip, UCHAR *p, float *d, int n );	// ��������ͨ��port˥��
extern "C" __declspec(dllexport) void GetAllChannelsPortAtten_Net( ULONG ip );								// ��ȡ����ͨ��˥��

// Module Operation Status
extern "C" __declspec(dllexport) void GetAlarm_Net( ULONG ip );												// ��ȡ�澯
extern "C" __declspec(dllexport) void GetVoltage_Net( ULONG ip );											// ��ȡ��ѹ
extern "C" __declspec(dllexport) void GetTemperature_Net( ULONG ip );										// ��ȡ�¶�
//
extern "C" __declspec(dllexport) bool GetTemperatureSyn_Net( ULONG ip, float* temp );								

// �澯��Ϣ
extern "C" __declspec(dllexport) void SetLowTemperatureAlarmThreshold_Net( ULONG ip, float temperature );	// ���õ��¶ȸ澯����
extern "C" __declspec(dllexport) void GetLowTemperatureAlarmThreshold_Net( ULONG ip );						// ��ȡ���¶ȸ澯����
extern "C" __declspec(dllexport) void SetHighTemperatureAlarmThreshold_Net( ULONG ip, float temperature );	// ���ø��¶ȸ澯����
extern "C" __declspec(dllexport) void GetHighTemperatureAlarmThreshold_Net( ULONG ip );						// ��ȡ���¶ȸ澯����

// Set DAC Voltage
extern "C" __declspec(dllexport) bool SetDACVoltageSyn_Net( ULONG ip, UINT channel, int coord, USHORT vol );// ����ͨ�� ��(x:0 y:1) ��ѹ

// �ڲ�����
extern "C" __declspec(dllexport) void SetModuleInfo1_Net( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
extern "C" __declspec(dllexport) void SetModuleInfo2_Net( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
extern "C" __declspec(dllexport) void SetModuleInfo3_Net( ULONG ip, USHORT volList[104] );
extern "C" __declspec(dllexport) void SetModuleInfo4_Net( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );

// Э����չ
extern "C" __declspec(dllexport) bool StartLoadNewFirmwareProcess();