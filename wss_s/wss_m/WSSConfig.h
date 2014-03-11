
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
/*-------------------------------------------��������-------------------------------------------*/
public:
	static WSSConfig* GetSingleton();
	static void Destroy();

	void Init( int channelNum, int inPortNum, int outPortNum, int freqGHz );

	WSSType _wssType;							// wss����
	
	int _inPortNum;								// ��˿���
	int _outPortNum;							// ���˿���
	int _freqGHz;								// Ƶ��
	int _channelNum;							// ͨ����
	int _startChannelFreq;						// 

	int _commMethod;							// ͨ�ŷ�ʽ  COM:0   NET:1
	ULONG _deviceIP;							// �豸��ַ

private:
	WSSConfig();
	void CalcWSSType();

	static WSSConfig* _pSingleton;

/*-------------------------------------------���겿��-------------------------------------------*/
public:
	void LoadCalITUList( std::string ituFile );

	std::string _ITUNMList[MAX_CHANNEL_NUM];	// �����ò�����Ϣ

	int _lsbRange;								// lsb ��Χ
	int _lsb;									// lsb һ��Ϊ4

	bool _isOddRight;							// ����1*n n*1
	int _waitTemStableTime;						// �ȴ���ѭ���ȶ���ʱ��

/*-------------------------------------------�������Բ���-------------------------------------------*/
public:
	void LoadITUGhzList( std::string ItuFile );

	VECTOR_INT _ITUGhz50G;				// ������itu ������Ϣ
	VECTOR_INT _ITUGhz100G;
};




/*-------------------------------------------���ܽӿ�-------------------------------------------*/

/*-------------------------------------------��������-------------------------------------------*/
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

/*-------------------------------------------���겿��-------------------------------------------*/
extern "C" __declspec(dllexport) void LoadCalITUList( const char* file );
//extern "C" __declspec(dllexport) const char* GetITUNM( int channelIndex );
extern "C" __declspec(dllexport) std::string* GetITUNMList();
extern "C" __declspec(dllexport) int GetLsbRange();
extern "C" __declspec(dllexport) int& GetLsb();

extern "C" __declspec(dllexport) bool& IsOddRight();

extern "C" __declspec(dllexport) int& GetWaitTempStableTime();


/*-------------------------------------------�������Բ���-------------------------------------------*/
extern "C" __declspec(dllexport) void LoadITUGhzList( const char* file );

extern "C" __declspec(dllexport) void* GetITUGhzList( int ghz );