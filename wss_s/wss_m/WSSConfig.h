
#pragma once


/*------------------------------------------------------- WSSConfig -------------------------------------------------------*/
#define MAX_CHANNEL_NUM			104
#define MAX_PORT_NUM			9

enum WSSType
{
	TYPE_11 = 11,
	TYPE_12 = 12,
	TYPE_21 = 21,
	TYPE_1X = 19,
	TYPE_X1 = 91,
	NOT_EXIST = 0
};

enum WSSComm
{
	COM = 0,
	NET
};


struct WSSConfig
{
/*-------------------------------------------公共部分-------------------------------------------*/
public:
	static WSSConfig* GetSingleton();
	static void Destroy();

	void Init( int channelNum, int inPortNum, int outPortNum, int freqGHz );

	WSSType _wssType;							// wss类型
	
	int _inPortNum;								// 入端口数
	int _outPortNum;							// 出端口数
	int _freqGHz;								// 频率
	int _channelNum;							// 通道数
	int _startChannelFreq;						// 

	int _commMethod;							// 通信方式  COM:0   NET:1
	ULONG _deviceIP;							// 设备地址

private:
	WSSConfig();
	void CalcWSSType();

	static WSSConfig* _pSingleton;

/*-------------------------------------------定标部分-------------------------------------------*/
public:
	void LoadCalITUList( std::string ituFile );

	std::string _ITUNMList[MAX_CHANNEL_NUM];	// 定标用波长信息

	int _lsbRange;								// lsb 范围
	int _lsb;									// lsb 一般为4

	bool _isOddRight;							// 用于1*n n*1
	int _waitTemStableTime;						// 等待温循箱稳定的时间

/*-------------------------------------------分析测试部分-------------------------------------------*/
public:
	void LoadITUGhzList( std::string ItuFile );

	VECTOR_INT _ITUGhz50G;				// 分析用itu 波长信息
	VECTOR_INT _ITUGhz100G;
};




/*-------------------------------------------功能接口-------------------------------------------*/

/*-------------------------------------------公共部分-------------------------------------------*/
extern "C" __declspec(dllexport) void InitWSSType( int channelNum, int inPortNum, int outPortNum, int freqGHz );

extern "C" __declspec(dllexport) int GetChannelNum();
extern "C" __declspec(dllexport) int GetInPortNum();
extern "C" __declspec(dllexport) int GetOutPortNum();
extern "C" __declspec(dllexport) int GetPortNum();
extern "C" __declspec(dllexport) int GetFreqGHz();

extern "C" __declspec(dllexport) int& GetCommMethod();
extern "C" __declspec(dllexport) ULONG& GetWSSIP();

extern "C" __declspec(dllexport) int GetWSSType();

extern "C" __declspec(dllexport) int GetStartChannelFreqGHz();

/*-------------------------------------------定标部分-------------------------------------------*/
extern "C" __declspec(dllexport) void LoadCalITUList( const char* file );
//extern "C" __declspec(dllexport) const char* GetITUNM( int channelIndex );
extern "C" __declspec(dllexport) std::string* GetITUNMList();
extern "C" __declspec(dllexport) int GetLsbRange();
extern "C" __declspec(dllexport) int& GetLsb();

extern "C" __declspec(dllexport) bool& IsOddRight();

extern "C" __declspec(dllexport) int& GetWaitTempStableTime();


/*-------------------------------------------分析测试部分-------------------------------------------*/
extern "C" __declspec(dllexport) void LoadITUGhzList( const char* file );

extern "C" __declspec(dllexport) void* GetITUGhzList( int ghz );