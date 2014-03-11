#pragma once

#include "WSSProCal.h"

/*--------------------------------------------------view�ӿ�--------------------------------------------------*/

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


/*--------------------------------------------------���ݽṹ--------------------------------------------------*/
// ����ͨ���Ķ�������
struct Channel_Data
{
	// ԭʼ�ɼ�port����
	VECTOR_INT _curveXPortList[MAX_PORT_NUM];
	VECTOR_FLOAT _curveYPortList[MAX_PORT_NUM];

	// ��ֵ�����Ķ�������
	VECTOR_INT _curveXCalcPortList[MAX_PORT_NUM];
	VECTOR_FLOAT _curveYCalcPortList[MAX_PORT_NUM];

	// ������Ϣ
	int _portPeakIndexList[MAX_PORT_NUM];						// ������� �����һ�����ݵĶ���

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

// �����¶ȵ�Ķ�������
struct Temp_Data
{
	UCHAR _tempPoint;											// �¶ȵ�
	std::string _filePath;										// �ļ�·��

	// ������ĸ�ͨ������
	Channel_Data _dataChannelList[MAX_CHANNEL_NUM];
	bool _channelUseFlag[MAX_CHANNEL_NUM];						// ͨ�����ڱ�־ ���ڷ�������ʱ������

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



/*--------------------------------------------------�ɼ�����--------------------------------------------------*/
// ���߳�����
struct CollectMainThreadData
{
	CollectMainThreadData()  {  _notifyCount = 0;  }

	/*--------------------------------�߳�����--------------------------------*/
	VECTOR_INT _channelIndexList;								// �����б� �Լ���������

	/*--------------------------------�߳̽���--------------------------------*/
	// ���߳��������ͨ��
	UINT _notifyCount;											// ������߳�ͨ����
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
		STRING_FORMAT( str, "��ǰ���߳�ͨ������%d", _notifyCount );
		//(*PCB_View_ShowMsg)( str.c_str() );

		_notifyCountCS.UnLock();
	}
};

// ���߳�����
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

	/*--------------------------------�߳�����--------------------------------*/
	ULONG _ipAddr;												// ��·��IP��ַ
	ULONG _oswIP;												// �⿪�ص�ַ
	float _saveLightList[MAX_CHANNEL_NUM][MAX_PORT_NUM];		// ���ֵ
	float _stableTemp;											// �ȶ�����¶�

	VECTOR_INT _channelIndexList;								// ������Ϣ
	
	// ����ɼ�����
	std::vector<Temp_Data> _tempDataList;						// ���¶ȵ��µĲɼ�����
	int _curTempIndex;											// ��ǰ�¶�����index
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

	/*--------------------------------�߳̽���--------------------------------*/
	// ���߳�ͨ���������Ϣ
	bool _notifyFlag;
	int _notifyChannelIndex;
	int _notifyParam;
	//
	bool JudgeNotifyWaveLen() 
	{ 
		return find( _channelIndexList.begin(), _channelIndexList.end(), _notifyChannelIndex )!=_channelIndexList.end() ? true:false; 
	}
	void SetNotifyInfo( int channelIndex, int param )
	{	// �������Ż����ƻ�˳��??
		_notifyChannelIndex = channelIndex;
		_notifyParam = param;
		_notifyFlag = true;
	}

	/*--------------------------------������Ч��--------------------------------*/
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

/*--------------------------------��ѭ������--------------------------------*/
public:
	bool SetTemPointsList( VECTOR_FLOAT& temPointsList );

protected:
	void WaitingTempStable();									// �ȴ����е�·���¶��ȶ�
	bool IsTemThreadRunning();									// �ж���ѭ�߳��Ƿ���������

/*--------------------------------����������--------------------------------*/
protected:
	Laser8163B _laser;											// ������
	//Laser8164B _laser;										// ������

	void SetLaserWaveLen( int channelIndex );					// ���ü���������
	void PollWaveLenList( int param );							// ��ѯ������ͨ�����̲߳��ȴ��������߳���� param: ����port��Ϣ ���� step��Ϣ

/*--------------------------------���߳� ���߳�--------------------------------*/
public:
	bool AddChildThread( ULONG ipAddress );
	bool AddChildThread( ULONG ipAddress, VECTOR_INT& channelIndexList );
	bool AddChildThread( ULONG ipAddress, UCHAR tempPoint, std::string filePath );
	bool AddChildThread( ULONG ipAddress, ULONG oswIP );
	bool DelChildThread( ULONG ipAddress );						// ��� ɾ�����߳�
	void ClearAllThreads();										// ֹͣ������������߳���Ϣ
	void CleraAllChildThreadsTemData();							// ����������̵߳��¶�����
	bool GetChildThreadCollectData( ULONG ipAddress, UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList ); // ��ȡ�ɼ�����

protected:
	CollectMainThreadData _mainThreadData;						// ���߳�����
	std::vector<CollectChildThreadData> _childThreadDataList;	// ���߳������б�
	cnCriticalSection _childThreadListCS;						// ���߳��ź���

	// �ɼ����߳� ���߳�����
	void MainThread( void* pData );
	void ChildThread( void* pData );

	bool StartCollectThreads( UINT curTempIndex=0 );			// ��ʼ�������ݲɼ������߳�
	int SearchChildThread( ULONG ipAddress );					// Ѱ���ض����߳�����
	bool IsMainThreadRunning();									// �ж����߳��Ƿ���������

/*--------------------------------��������������--------------------------------*/
public:
	void SetInspectOPMAddr( ULONG addr );						// ���ü���̵�ַ

protected:
	ULONG _inspectOpmAddr;

	void InspectLaserThread( void* pData );
	bool StartInspectLaserThread();								// ��ʼ����Դ�Ƿ�����

	bool SetInsepectLaserFlag( ULONG addr, bool flag );
	bool GetInsepectLaserFlag( ULONG addr, bool& flag );

/*--------------------------------������--------------------------------*/
public:
	bool StartSaveLightProcess( int port );						// ������

protected:
	bool _saveLightFlag;										// ����־
	int _saveLightPort;											// Ҫ����ͨ��

	// �洢 ����������
	void SaveSaveLightData( CollectChildThreadData& childThreadData );
	void LoadSaveLightData();									

	void MainSaveLightAllPorts();								// n*1��� port�Զ��л�
	void MainSaveLightPort( int port );							// �ض�port���
	void ChildSaveLight( CollectChildThreadData& childThreadData );

/*--------------------------------���ݲɼ�--------------------------------*/	
public:
	bool StartCollectAllTemPointsProcess();						// �����¶ȵ�ɼ�

	// ���Ĳɼ�
	void CollecteData_Core( ULONG ip, int c, int p, VECTOR_INT& curveX, VECTOR_FLOAT& curveY );

protected:
	bool StartCollectAllTempDataThread();						// ���������¶ȵ����ݲɼ�
	void CollectAllTempDataThread( void* pData );		

	// ����ͨ��
	void NotifyAllChildsWssTypeAndLSB();

	void MainCollectData11();
	void MainCollectDataX1();
	void MainCollectData1X();
	void ChildCollectData11( CollectChildThreadData& childThreadData );
	void ChildCollectData1X( CollectChildThreadData& childThreadData );
	void ChildCollectDataX1( CollectChildThreadData& childThreadData );
	void SaveFileCollectX();

	VECTOR_FLOAT _tempPointList;									// ��ѭ�� �¶ȵ�
};


/*--------------------------------------------------��������--------------------------------------------------*/
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

	ULONG _ipAddr;												// ��·��IP��ַ

	std::vector<Temp_Data> _tempDataList;						// ���¶ȵ��µĲɼ�����
	int _curTempIndex;											// ��ǰ�¶�����index
	std::vector<bool> _channelUseFlagCache;						// ��ʱͨ��������

	float _portPeak[MAX_PORT_NUM];								// ��port��ֵ
	float _lowestPeak;											// ��ͷ�

	VECTOR_INT _yVolList;
};
