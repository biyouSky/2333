
#pragma once


/*-------------------------------------------view回调接口-------------------------------------------*/

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




/*-------------------------------------------公共基础协议-------------------------------------------*/
/*
		最开始为串口命令  为调试方便转成串口转网口命令
		本部分为公共基础协议
*/
// wss发送命令
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

// wss接收命令
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
	/*------------------------------------- 命令生成 -------------------------------------*/
	// 模块信息
	WSSCommand GetSerialNumber();											// 获得序号
	WSSCommand GetType();													// 获得类型
	WSSCommand GetVersion();												// 获得版本号
	WSSCommand SetRS232BaudRate( int baudRate );							// 设置串口速率
	WSSCommand GetHardwareDateCode();										// 获得硬件版本号
	WSSCommand GetFirmwareDateCode();										// 获得固件版本号
	WSSCommand GetManufactureID();											// 获得制造ID

	WSSCommand GetAD5535( int c );											// 获得AD5535值

	// 通道端口衰减
	WSSCommand SetChannelPortAtten( int c, UCHAR p, float d );				// 设置通道port衰减
	WSSCommand GetChannelPortAtten( int c );								// 获取通道衰减
	WSSCommand SetAllChannelsPortAtten( UCHAR *p, float *d, int n );		// 设置所有通道port衰减
	WSSCommand GetAllChannelsPortAtten();									// 获取所有通道衰减

	// Module Operation Status
	WSSCommand GetAlarm();													// 获取告警
	WSSCommand GetVoltage();												// 获取电压
	WSSCommand GetTemperature();											// 获取温度

	// 告警信息
	WSSCommand SetLowTemperatureAlarmThreshold( float temperature );		// 设置低温度告警门限
	WSSCommand GetLowTemperatureAlarmThreshold();							// 获取低温度告警门限
	WSSCommand SetHighTemperatureAlarmThreshold( float temperature );		// 设置高温度告警门限
	WSSCommand GetHighTemperatureAlarmThreshold();							// 获取高温度告警门限

	// 固件升级 该系列命令实际不能转成网口的
	WSSCommand StartFirmwareLoading();										// 开始固件加载
	WSSCommand LoadNewFirmware();											// 加载新固件
	WSSCommand SwitchtoNewFirmware();										// 切换新固件

	// Set DAC Voltage
	WSSCommand SetDACVoltage( UINT channel, int coord, USHORT vol );		// 设置通道 轴(x:0 y:1) 电压

	// 配置文件
	WSSCommand GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
	WSSCommand ReadChannelProfile( UCHAR pID );							
	WSSCommand SelectChannelProfile( UCHAR pID );
	WSSCommand ReadProfileID();

	// 内部命令
	WSSCommand SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	WSSCommand SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	WSSCommand SetModuleInfo3( USHORT volList[104] );
	WSSCommand SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] );

	/*------------------------------------- 命令解析及显示处理 -------------------------------------*/
	void HandleParseRes( std::string& strRes, UCHAR op, bool& rt, std::string& strMsg, std::vector<Var>& _resultList );
	void HandleShowResult( UCHAR op, bool rt, std::string strMsg, std::vector<Var>& _resultList );
};




/*-------------------------------------------串口协议-------------------------------------------*/
/*
		直接和wss通信, 采用串口异步方式, 因串口通信特殊性以及协议的定义, 同步机制缺乏保护
		命令之间可能产生的数据冲突问题建议交由上层来完成 同一时间只允许一条命令进行操作
*/
class WSSProSeria : public SerialASYN
{
	struct SerialSynData
	{	// 协议原因 不能对操作OP做区分
		bool _useFlag;													// 同步数据使用标志
		bool _stopWaitFlag;												// 同步数据等待标志
		std::vector<Var> _resultList;									// 返回的结果列表

		SerialSynData() : _useFlag( false ), _stopWaitFlag( false ) {}
	};
	SerialSynData _synData;												// 同步数据

	UCHAR _code;														// 标识码	
	WSSBasePro _wssBasePro;												// 基础协议

protected:
	bool ParaseRes( std::string& res );									// 处理命令回复

public:
	WSSProSeria();

	// command
	// 模块信息
	void GetSerialNumber();												// 获得序号
	void GetType();														// 获得类型
	void GetVersion();													// 获得版本号
	void SetRS232BaudRate( int baudRate );								// 设置串口速率
	void GetHardwareDateCode();											// 获得硬件版本号
	void GetFirmwareDateCode();											// 获得固件版本号
	void GetManufactureID();											// 获得制造ID

	bool GetAD5535Syn( int c, USHORT& v );	

	// 通道端口衰减
	void SetChannelPortAtten( int c, UCHAR p, float d );				// 设置通道port衰减
	void GetChannelPortAtten( int c );									// 获取通道衰减
	void SetAllChannelsPortAtten( UCHAR *p, float *d, int n );			// 设置所有通道port衰减
	void GetAllChannelsPortAtten();										// 获取所有通道衰减

	// Module Operation Status
	void GetAlarm();													// 获取告警
	void GetVoltage();													// 获取电压
	void GetTemperature();												// 获取温度
	//
	bool GetTemperatureSyn( float& temp );		

	// 告警信息
	void SetLowTemperatureAlarmThreshold( float temperature );			// 设置低温度告警门限
	void GetLowTemperatureAlarmThreshold();								// 获取低温度告警门限
	void SetHighTemperatureAlarmThreshold( float temperature );			// 设置高温度告警门限
	void GetHighTemperatureAlarmThreshold();							// 获取高温度告警门限

	// 固件升级
	void StartFirmwareLoading();										// 开始固件加载
	void LoadNewFirmware();												// 加载新固件
	void SwitchtoNewFirmware();											// 切换新固件
	// 
	bool LoadNewFirmwareSyn();

	// 配置文件
	void GenerateChannelProfile( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
	void ReadChannelProfile( UCHAR pID );							
	void SelectChannelProfile( UCHAR pID );
	void ReadProfileID();

	// Set DAC Voltage
	void SetDACVoltage( UINT channel, int coord, USHORT vol );			// 设置通道 轴(x:0 y:1) 电压
	// 
	bool SetDACVoltageSyn( UINT channel, int coord, USHORT vol );

	// 内部命令
	void SetModuleInfo1( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	void SetModuleInfo2( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	void SetModuleInfo3( USHORT volList[104] );
	void SetModuleInfo4( UCHAR port, UCHAR f, USHORT volList[104] );
};


/*-------------------------------------------udp协议-------------------------------------------*/
/*
		直接和定标电路板通信
		采用udp同步方式, 用于定标, 实现简单 吞吐率差
		其中在多线程定标中需定义多个对象 保证同时接收
		同一对象在多个线程中同时进行接收虽然能进行 但似乎出现互斥现象 原因未深入分析？？
		
		因为协议定义缺陷 收到的回复包无op字符 不能进行区分
		只能通道对命令之间进行隔离（发送+接收整个过程）来保证解析的正确性 用信号量来完成
*/
class WSSProUDP
{	
	UdpOBJ _udpOBJ;

public:
	std::string _debugMsg;
	WSSProUDP();

	/*------------------------------------- 串口转网口命令 -------------------------------------*/
protected:
	WSSBasePro _wssBasePro;														// 基础协议
	cnCriticalSection _cs;														// 串口转网口命令之间隔离性互斥

	bool Func1( UCHAR op, std::vector<Var>& _resultList );						// 接收 解析通用处理		
	
public:
	bool GetSerialNumber( ULONG ip );											// 获得序号
	bool GetType( ULONG ip );													// 获得类型
	bool GetVersion( ULONG ip );												// 获得版本号
	bool SetRS232BaudRate( ULONG ip, int baudRate );							// 设置串口速率
	bool GetHardwareDateCode( ULONG ip );										// 获得硬件版本号
	bool GetFirmwareDateCode( ULONG ip );										// 获得固件版本号
	bool GetManufactureID( ULONG ip );											// 获得制造ID
	//
	bool GetSerialNumberSyn( ULONG ip, int& sn );

	// 通道端口衰减
	bool SetChannelPortAtten( ULONG ip, int c, UCHAR p, float d );				// 设置通道port衰减
	bool GetChannelPortAtten( ULONG ip, int c );								// 获取通道衰减
	bool SetAllChannelsPortAtten( ULONG ip, UCHAR *p, float *d, int n );		// 设置所有通道port衰减
	bool GetAllChannelsPortAtten( ULONG ip );									// 获取所有通道衰减

	// Module Operation Status
	bool GetAlarm( ULONG ip );													// 获取告警
	bool GetVoltage( ULONG ip );												// 获取电压
	bool GetTemperature( ULONG ip );											// 获取温度
	//
	bool GetTemperatureSyn( ULONG ip, float& temp );

	// 告警信息
	bool SetLowTemperatureAlarmThreshold( ULONG ip, float temperature );		// 设置低温度告警门限
	bool GetLowTemperatureAlarmThreshold( ULONG ip );							// 获取低温度告警门限
	bool SetHighTemperatureAlarmThreshold( ULONG ip, float temperature );		// 设置高温度告警门限
	bool GetHighTemperatureAlarmThreshold( ULONG ip );							// 获取高温度告警门限

	// Set DAC Voltage
	bool SetDACVoltage( ULONG ip, UINT channel, int coord, USHORT vol );		// 设置通道 轴(x:0 y:1) 电压
	
	// 内部命令
	bool SetModuleInfo1( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
	bool SetModuleInfo2( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
	bool SetModuleInfo3( ULONG ip, USHORT volList[104] );
	bool SetModuleInfo4( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );
};


/*-------------------------------------------协议扩展-------------------------------------------*/
/*
		
*/
class WSSProEX : public _QueueThreadManage<WSSProEX>
{
	void LoadNewFirmwareThread();

public:
	// 固件升级
	bool StartLoadNewFirmwareThread();			
};


/*-------------------------------------------协议对象-------------------------------------------*/
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



/*-------------------------------------------功能接口-------------------------------------------*/

/*------------------------------------- 串口协议 -------------------------------------*/
// 串口控制
extern "C" __declspec(dllexport) bool OpenSerial( DWORD dwPort, DWORD dwBaudRate=115200 );
extern "C" __declspec(dllexport) bool SetSerialRate( DWORD baudRate );
extern "C" __declspec(dllexport) bool IsSerialOpen();

extern "C" __declspec(dllexport) void GetSerialNumber_Com();												// 获得序号
extern "C" __declspec(dllexport) void GetType_Com();														// 获得类型
extern "C" __declspec(dllexport) void GetVersion_Com();														// 获得版本号
extern "C" __declspec(dllexport) void SetRS232BaudRate_Com( int baudRate );									// 获得串口速率
extern "C" __declspec(dllexport) void GetHardwareDateCode_Com();											// 获得硬件版本号
extern "C" __declspec(dllexport) void GetFirmwareDateCode_Com();											// 获得固件版本号
extern "C" __declspec(dllexport) void GetManufactureID_Com();												// 获得制造ID

extern "C" __declspec(dllexport) bool GetAD5535Syn_Com( int c, USHORT& v );	

extern "C" __declspec(dllexport) void SetDACVoltage_Com( UINT channel, int coord, USHORT vol );
extern "C" __declspec(dllexport) bool SetDACVoltageSyn_Com( UINT channel, int coord, USHORT vol );

// 通道端口衰减
extern "C" __declspec(dllexport) void SetChannelPortAtten_Com( int c, UCHAR p, float d );					// 设置通道port衰减
extern "C" __declspec(dllexport) void GetChannelPortAtten_Com( int c );										// 获取通道衰减
extern "C" __declspec(dllexport) void SetAllChannelsPortAtten_Com( UCHAR *p, float *d, int n );				// 设置所有通道port衰减
extern "C" __declspec(dllexport) void GetAllChannelsPortAtten_Com();										// 获取所有通道衰减

// Module Operation Status
extern "C" __declspec(dllexport) void GetAlarm_Com();														// 获取告警
extern "C" __declspec(dllexport) void GetVoltage_Com();														// 获取电压
extern "C" __declspec(dllexport) void GetTemperature_Com();
//
extern "C" __declspec(dllexport) bool GetTemperatureSyn_Com( float& temp );	

// 告警信息
extern "C" __declspec(dllexport) void SetLowTemperatureAlarmThreshold_Com( float temperature );				// 设置低温度告警门限
extern "C" __declspec(dllexport) void GetLowTemperatureAlarmThreshold_Com();								// 获取低温度告警门限
extern "C" __declspec(dllexport) void SetHighTemperatureAlarmThreshold_Com( float temperature );			// 设置高温度告警门限
extern "C" __declspec(dllexport) void GetHighTemperatureAlarmThreshold_Com();								// 获取高温度告警门限

// 固件升级
extern "C" __declspec(dllexport) void StartFirmwareLoading_Com();											// 开始固件加载
extern "C" __declspec(dllexport) void LoadNewFirmware_Com();												// 加载新固件
extern "C" __declspec(dllexport) void SwitchtoNewFirmware_Com();											// 切换新固件

// 配置文件
extern "C" __declspec(dllexport) void GenerateChannelProfile_Com( UCHAR pID, UCHAR portList[], UCHAR attenList[] );
extern "C" __declspec(dllexport) void ReadChannelProfile_Com( UCHAR pID );							
extern "C" __declspec(dllexport) void SelectChannelProfile_Com( UCHAR pID );
extern "C" __declspec(dllexport) void ReadProfileID_Com();

// 内部命令
extern "C" __declspec(dllexport) void SetModuleInfo1_Com( ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
extern "C" __declspec(dllexport) void SetModuleInfo2_Com( UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
extern "C" __declspec(dllexport) void SetModuleInfo3_Com( USHORT volList[104] );
extern "C" __declspec(dllexport) void SetModuleInfo4_Com( UCHAR port, UCHAR f, USHORT volList[104] );




/*------------------------------------- 串口转网口协议 -------------------------------------*/
extern "C" __declspec(dllexport) void GetSerialNumber_Net( ULONG ip );										// 获得序号
extern "C" __declspec(dllexport) void GetType_Net( ULONG ip );												// 获得类型
extern "C" __declspec(dllexport) void GetVersion_Net( ULONG ip );											// 获得版本号
extern "C" __declspec(dllexport) void SetRS232BaudRate_Net( ULONG ip, int baudRate );						// 获得串口速率
extern "C" __declspec(dllexport) void GetHardwareDateCode_Net( ULONG ip );									// 获得硬件版本号
extern "C" __declspec(dllexport) void GetFirmwareDateCode_Net( ULONG ip );									// 获得固件版本号
extern "C" __declspec(dllexport) void GetManufactureID_Net( ULONG ip );										// 获得制造ID
//
extern "C" __declspec(dllexport) bool GetSerialNumberSyn_Net( ULONG ip, int* sn );										

// 通道端口衰减
extern "C" __declspec(dllexport) void SetChannelPortAtten_Net( ULONG ip, int c, UCHAR p, float d );			// 设置通道port衰减
extern "C" __declspec(dllexport) void GetChannelPortAtten_Net( ULONG ip, int c );							// 获取通道衰减
extern "C" __declspec(dllexport) void SetAllChannelsPortAtten_Net( ULONG ip, UCHAR *p, float *d, int n );	// 设置所有通道port衰减
extern "C" __declspec(dllexport) void GetAllChannelsPortAtten_Net( ULONG ip );								// 获取所有通道衰减

// Module Operation Status
extern "C" __declspec(dllexport) void GetAlarm_Net( ULONG ip );												// 获取告警
extern "C" __declspec(dllexport) void GetVoltage_Net( ULONG ip );											// 获取电压
extern "C" __declspec(dllexport) void GetTemperature_Net( ULONG ip );										// 获取温度
//
extern "C" __declspec(dllexport) bool GetTemperatureSyn_Net( ULONG ip, float* temp );								

// 告警信息
extern "C" __declspec(dllexport) void SetLowTemperatureAlarmThreshold_Net( ULONG ip, float temperature );	// 设置低温度告警门限
extern "C" __declspec(dllexport) void GetLowTemperatureAlarmThreshold_Net( ULONG ip );						// 获取低温度告警门限
extern "C" __declspec(dllexport) void SetHighTemperatureAlarmThreshold_Net( ULONG ip, float temperature );	// 设置高温度告警门限
extern "C" __declspec(dllexport) void GetHighTemperatureAlarmThreshold_Net( ULONG ip );						// 获取高温度告警门限

// Set DAC Voltage
extern "C" __declspec(dllexport) bool SetDACVoltageSyn_Net( ULONG ip, UINT channel, int coord, USHORT vol );// 设置通道 轴(x:0 y:1) 电压

// 内部命令
extern "C" __declspec(dllexport) void SetModuleInfo1_Net( ULONG ip, ULONG sn, UCHAR type, UCHAR hv1, UCHAR hv2, UCHAR hd1, UCHAR hd2 );
extern "C" __declspec(dllexport) void SetModuleInfo2_Net( ULONG ip, UCHAR flag1, UCHAR flag2, UCHAR tempNum, UCHAR lowTemp, UCHAR highTemp );
extern "C" __declspec(dllexport) void SetModuleInfo3_Net( ULONG ip, USHORT volList[104] );
extern "C" __declspec(dllexport) void SetModuleInfo4_Net( ULONG ip, UCHAR port, UCHAR f, USHORT volList[104] );

// 协议扩展
extern "C" __declspec(dllexport) bool StartLoadNewFirmwareProcess();