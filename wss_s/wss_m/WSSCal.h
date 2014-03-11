#pragma once

#include "WSSProCal.h"

/*--------------------------------------------------view接口--------------------------------------------------*/

typedef void (*PFunc_View_Type101)( ULONG ip, const char* msg );
typedef void (*PFunc_View_Type102)( ULONG ip, int c, const char* msg );
typedef void (*PFunc_View_Type103)( ULONG ip );
typedef void (*PFunc_View_Type104)( ULONG ip, int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag ); 
typedef void (*PFunc_View_Type105)( int c, const char* msg );		
typedef void (*PFunc_View_Type106)();								
typedef void (*PFunc_View_Type107)( int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag ); 
typedef void (*PFunc_View_Type108)( bool* channelUseFlag );


extern PFunc_View_Type0 PCB_View_ShowMsg;
extern PFunc_View_Type101 PCB_View_ShowCollectMsg;				
extern PFunc_View_Type102 PCB_View_ShowCollectGrid;				
extern PFunc_View_Type103 PCB_View_ClearCollectChart;			
extern PFunc_View_Type104 PCB_View_AddCollectCurve;		
extern PFunc_View_Type105 PCB_View_ShowParseGrid;		
extern PFunc_View_Type106 PCB_View_ClearParseChart;								
extern PFunc_View_Type107 PCB_View_AddParseCurve; 
extern PFunc_View_Type108 PCB_View_SetParseGridChannel;


/*--------------------------------------------------数据结构--------------------------------------------------*/
// 单个通道的定标数据
struct Channel_Data
{
	// 原始采集port数据
	VECTOR_INT _curveXPortList[MAX_PORT_NUM];
	VECTOR_FLOAT _curveYPortList[MAX_PORT_NUM];

	// 插值处理后的定标数据
	VECTOR_INT _curveXCalcPortList[MAX_PORT_NUM];
	VECTOR_FLOAT _curveYCalcPortList[MAX_PORT_NUM];

	// 辅助信息
	int _portPeakIndexList[MAX_PORT_NUM];						// 顶点序号 保存第一步数据的顶点

	Channel_Data()
	{
		for( int i=0; i<MAX_PORT_NUM; ++i )
			_portPeakIndexList[i] = -1;
	}

	void ClearOriginal()
	{
		for( int i=0; i<MAX_PORT_NUM; ++i )
		{
			_curveXPortList[i].clear();
			_curveYPortList[i].clear();
		}
	}

	void ClearCalc()
	{
		for( int i=0; i<MAX_PORT_NUM; ++i )
		{
			_curveXCalcPortList[i].clear();
			_curveYCalcPortList[i].clear();
		}
	}

	void Clear()
	{
		ClearOriginal();
		ClearCalc();
	}
};

// 单个温度点的定标数据
struct Temp_Data
{
	UCHAR _tempPoint;											// 温度点
	std::string _filePath;										// 文件路径

	// 处理过的各通道数据
	Channel_Data _dataChannelList[MAX_CHANNEL_NUM];
	bool _channelUseFlag[MAX_CHANNEL_NUM];						// 通道存在标志 用于分析数据时的载入

	Temp_Data( UCHAR tempPoint, std::string filePath )
	{
		_tempPoint = tempPoint;
		_filePath = filePath;

		for( int i=0; i<MAX_CHANNEL_NUM; ++i )
			_channelUseFlag[i] = false;
	}

	void Clear()
	{
		for( int i=0; i<MAX_CHANNEL_NUM; ++i )
		{
			_dataChannelList[i].Clear();
			_channelUseFlag[i] = false;
		}
	}
};



/*--------------------------------------------------采集过程--------------------------------------------------*/
// 主线程数据
struct CollectMainThreadData
{
	CollectMainThreadData()  {  _notifyCount = 0;  }

	/*--------------------------------线程数据--------------------------------*/
	VECTOR_INT _channelIndexList;								// 波长列表 以及波长设置

	/*--------------------------------线程交互--------------------------------*/
	// 子线程任务完成通告
	UINT _notifyCount;											// 获得子线程通告数
	cnCriticalSection _notifyCountCS;
	//
	void SetNotifyCount( UINT count=0 )
	{
		_notifyCountCS.Lock();
		_notifyCount = count;
		_notifyCountCS.UnLock();
	}
	void AddNotifyCount()
	{
		_notifyCountCS.Lock();
		
		++_notifyCount;
		std::string str;
		STRING_FORMAT( str, "当前子线程通告数：%d", _notifyCount );
		//(*PCB_View_ShowMsg)( str.c_str() );

		_notifyCountCS.UnLock();
	}
};

// 子线程数据
struct CollectChildThreadData
{
	void Init()
	{
		_curTempIndex = -1;
		_notifyFlag = false;
		_notifyParam = -1;

		_ipAddr = 0;
		_oswIP = 0;
		_stableTemp = 0;

		for( int c=0; c<MAX_CHANNEL_NUM; ++c )
		{
			for( int p=0; p<MAX_PORT_NUM; ++p )
				_saveLightList[c][p] = 0;
		}

		_curTempIndex = 0;

		_notifyFlag = false;
		_notifyChannelIndex = 0;
	    _notifyParam = 0;
	}
	CollectChildThreadData( ULONG ipAddr )
	{
		Init();
		_ipAddr = ipAddr;
	}
	CollectChildThreadData( ULONG ipAddr, VECTOR_INT& _channelIndexList )
	{
		Init();
		_ipAddr = ipAddr;
		_channelIndexList = _channelIndexList;
	}

	/*--------------------------------线程数据--------------------------------*/
	ULONG _ipAddr;												// 电路板IP地址
	ULONG _oswIP;												// 光开关地址
	float _saveLightList[MAX_CHANNEL_NUM][MAX_PORT_NUM];		// 存光值
	float _stableTemp;											// 稳定后的温度

	VECTOR_INT _channelIndexList;								// 波长信息
	
	// 定标采集数据
	std::vector<Temp_Data> _tempDataList;						// 多温度点下的采集数据
	int _curTempIndex;											// 当前温度数据index
	//
	int SearchTempData( UCHAR tempPoint )
	{
		for( UINT i=0; i<_tempDataList.size(); ++i )
		{
			if( _tempDataList[i]._tempPoint==tempPoint )
				return i;
		}
		return -1;
	}
	void AddTempData( UCHAR tempPoint, std::string& filePath )
	{
		int index = SearchTempData( tempPoint );
		if( index==-1 )
			_tempDataList.push_back( Temp_Data( tempPoint, filePath ) );
		else
			_tempDataList[index]._filePath = filePath;
	}
	void ClearTemData()
	{
		_tempDataList.clear();
		_curTempIndex = -1;
	}

	/*--------------------------------线程交互--------------------------------*/
	// 主线程通告的任务信息
	bool _notifyFlag;
	int _notifyChannelIndex;
	int _notifyParam;
	//
	bool JudgeNotifyWaveLen() 
	{ 
		return find( _channelIndexList.begin(), _channelIndexList.end(), _notifyChannelIndex )!=_channelIndexList.end() ? true:false; 
	}
	void SetNotifyInfo( int channelIndex, int param )
	{	// 编译器优化会破坏顺序??
		_notifyChannelIndex = channelIndex;
		_notifyParam = param;
		_notifyFlag = true;
	}

	/*--------------------------------数据有效性--------------------------------*/
	bool _isLaserOK;
};

class CollectDataProcess : public _MultiThreadManage<CollectDataProcess>
{
public:
	static CollectDataProcess* GetSingleton();
	static void Destroy();

private:
	CollectDataProcess();

	static CollectDataProcess* _pSingleton;

/*--------------------------------温循箱设置--------------------------------*/
public:
	bool SetTemPointsList( VECTOR_FLOAT& temPointsList );

protected:
	void WaitingTempStable();									// 等待所有电路板温度稳定
	bool IsTemThreadRunning();									// 判定温循线程是否正在运行

/*--------------------------------激光器控制--------------------------------*/
protected:
	Laser8163B _laser;											// 激光器
	//Laser8164B _laser;										// 激光器

	void SetLaserWaveLen( int channelIndex );					// 设置激光器波长
	void PollWaveLenList( int param );							// 轮询波长并通告子线程并等待所有子线程完成 param: 代表port信息 或者 step信息

/*--------------------------------主线程 子线程--------------------------------*/
public:
	bool AddChildThread( ULONG ipAddress );
	bool AddChildThread( ULONG ipAddress, VECTOR_INT& channelIndexList );
	bool AddChildThread( ULONG ipAddress, UCHAR tempPoint, std::string filePath );
	bool AddChildThread( ULONG ipAddress, ULONG oswIP );
	bool DelChildThread( ULONG ipAddress );						// 添加 删除子线程
	void ClearAllThreads();										// 停止并清除所有子线程信息
	void CleraAllChildThreadsTemData();							// 清除所有子线程的温度数据
	bool GetChildThreadCollectData( ULONG ipAddress, UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList ); // 获取采集数据

protected:
	CollectMainThreadData _mainThreadData;						// 主线程数据
	std::vector<CollectChildThreadData> _childThreadDataList;	// 子线程数据列表
	cnCriticalSection _childThreadListCS;						// 子线程信号量

	// 采集主线程 子线程主体
	void MainThread( void* pData );
	void ChildThread( void* pData );

	bool StartCollectThreads( UINT curTempIndex=0 );			// 开始所有数据采集工作线程
	int SearchChildThread( ULONG ipAddress );					// 寻找特定子线程数据
	bool IsMainThreadRunning();									// 判定主线程是否正在运行

/*--------------------------------激光器防死机制--------------------------------*/
public:
	void SetInspectOPMAddr( ULONG addr );						// 设置监测盘地址

protected:
	ULONG _inspectOpmAddr;

	void InspectLaserThread( void* pData );
	bool StartInspectLaserThread();								// 开始监测光源是否死掉

	bool SetInsepectLaserFlag( ULONG addr, bool flag );
	bool GetInsepectLaserFlag( ULONG addr, bool& flag );

/*--------------------------------存光相关--------------------------------*/
public:
	bool StartSaveLightProcess( int port );						// 存光过程

protected:
	bool _saveLightFlag;										// 存光标志
	int _saveLightPort;											// 要存光的通道

	// 存储 载入存光数据
	void SaveSaveLightData( CollectChildThreadData& childThreadData );
	void LoadSaveLightData();									

	void MainSaveLightAllPorts();								// n*1存光 port自动切换
	void MainSaveLightPort( int port );							// 特定port存光
	void ChildSaveLight( CollectChildThreadData& childThreadData );

/*--------------------------------数据采集--------------------------------*/	
public:
	bool StartCollectAllTemPointsProcess();						// 所有温度点采集

	// 核心采集
	void CollecteData_Core( ULONG ip, int c, int p, VECTOR_INT& curveX, VECTOR_FLOAT& curveY );

protected:
	bool StartCollectAllTempDataThread();						// 开启所有温度点数据采集
	void CollectAllTempDataThread( void* pData );		

	// 参数通告
	void NotifyAllChildsWssTypeAndLSB();

	void MainCollectData11();
	void MainCollectDataX1();
	void MainCollectData1X();
	void ChildCollectData11( CollectChildThreadData& childThreadData );
	void ChildCollectData1X( CollectChildThreadData& childThreadData );
	void ChildCollectDataX1( CollectChildThreadData& childThreadData );
	void SaveFileCollectX();

	VECTOR_FLOAT _tempPointList;									// 温循箱 温度点
};


/*--------------------------------------------------分析过程--------------------------------------------------*/
class ParseDataProcess : public _QueueThreadManage<ParseDataProcess>
{
public:
	static ParseDataProcess* GetSingleton();
	static void Destroy();

	void LoadCollectData( ULONG ipAddr,	VECTOR_STRING fileList, UCHAR tempList[6] );
	void SetParseChannels( UINT curTempIndex, bool* pChannelUseFlag, int num );
	void ParseCollectData();
	void SendParseData( VECTOR_INT& yVolList );

	bool GetLoadData( UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList );
	bool GetParseData( UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList );

private:
	ParseDataProcess();

	void LoadCollectDataThread();
	void SetParseChannelsThread();
	void ParseCollectDataThread();
	void SendParseDataThread();

	void LoadCollectDataX();
	void ParseCollectDataX();
	void SearchAllPortPeakX();
	void ParseCollectChannelDataX( int c, TxtOBJ& txt );

	void SendParseDataX();
	void SendParseChannelData1( int c );
	void SendParseChannelData2( int c );
	void SendParseChannelDataX( int c );
	int SearchBlock1( VECTOR_INT& curveX1, VECTOR_FLOAT& curveY1 );
	int SearchBlock2( VECTOR_INT& curveX1, VECTOR_FLOAT& curveY1, VECTOR_INT& curveX2, VECTOR_FLOAT& curveY2 );
	int SearchBlockX( VECTOR_INT& curveX, VECTOR_FLOAT& curveY );


	static ParseDataProcess* _pSingleton;

	ULONG _ipAddr;												// 电路板IP地址

	std::vector<Temp_Data> _tempDataList;						// 多温度点下的采集数据
	int _curTempIndex;											// 当前温度数据index
	std::vector<bool> _channelUseFlagCache;						// 临时通道处理缓存

	float _portPeak[MAX_PORT_NUM];								// 各port峰值
	float _lowestPeak;											// 最低峰

	VECTOR_INT _yVolList;
};
