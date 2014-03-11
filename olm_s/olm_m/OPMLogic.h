
#pragma once


typedef void(*PFunc_View_Type101)( std::vector<std::pair<float, float> >& exceptionPointList, UCHAR channel ); 
typedef void(*PFunc_View_Type102)();


extern PFunc_View_Type101 PCB_View_ShowVerifyResult;
extern PFunc_View_Type102 PCB_View_ShowDXPM;


struct OPMConfig
{
	int _serialPortNum;			// 串口号
	int _pmGPIBAddr;			// 参照功率计GPIB地址

	std::string _waveLen;		// 波长：1310 1550 1625

	float _attenBegin;
	float _attenEnd;

	float _verifyStep;			// 检验步长
	float _rang1VerifyAcc;		// 0到-50验证精度
	float _rang2VerifyAcc;		// -50到-65验证精度

	ULONG _sn;					// 设备sn号

	VECTOR_UCHAR _channelList;
	float _dxPM[16];
};


class OPMLogic : public _SingleThreadManage<OPMLogic>
{
public:
	static OPMLogic* GetSingleton();
	static void Destroy();

	// 工作线程
	bool StartDataCollectThreadThread();
	bool StartVerifyChannelThread();
	bool StartLoadDataThread();
	bool StartMonitorPMThread();
	bool StartAutoCalcDXThread();
	
	// 功能函数
	bool OpenVOAComm( DWORD port );				// 打开voa串口
	bool AutoAdjustPM( float dstPM );			// 自动调整初始功率值

	void VerifyChannelThread();					// 验证通道
	bool VerifyCollectData( UCHAR channel );	// 对通道进行验证

	bool TestDB();								// db测试相关
	void LoadDataThread();						// 从数据库载入数据
	bool LoadDataFromDB( ULONG sn );				// 从数据库中读取数据
	
	void MonitorPMThread();							// 监控参照pm
	void AutoCalcDXThread();					// 监控参照pm

	OPMConfig _configInfor;						// 配置信息

	VECTOR_FLOAT _channelDXList[16];
	float _channelOffsetList[16];

protected:
	OPMLogic();
	void DataCollectThread();
	
	static OPMLogic* _pSingleton;

	VOAXX _voa;								// 相关设备
	OPMML9001A _refPM;

	DeviceInfo _devInfor;						// 设备信息
	
	//定标的通道相关数据
	float _autoAdjustAtten;						// 初始自动调整功率 -> voa的初始衰减

	bool _preFlag;
	float _gearList[4];							// 4档相对值
};

extern OPMLogic gOPMLogic;


extern PFunc_View_Type0 PCB_View_ShowMsg;