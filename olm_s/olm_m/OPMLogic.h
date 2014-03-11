
#pragma once


typedef void(*PFunc_View_Type101)( std::vector<std::pair<float, float> >& exceptionPointList, UCHAR channel ); 
typedef void(*PFunc_View_Type102)();


extern PFunc_View_Type101 PCB_View_ShowVerifyResult;
extern PFunc_View_Type102 PCB_View_ShowDXPM;


struct OPMConfig
{
	int _serialPortNum;			// ���ں�
	int _pmGPIBAddr;			// ���չ��ʼ�GPIB��ַ

	std::string _waveLen;		// ������1310 1550 1625

	float _attenBegin;
	float _attenEnd;

	float _verifyStep;			// ���鲽��
	float _rang1VerifyAcc;		// 0��-50��֤����
	float _rang2VerifyAcc;		// -50��-65��֤����

	ULONG _sn;					// �豸sn��

	VECTOR_UCHAR _channelList;
	float _dxPM[16];
};


class OPMLogic : public _SingleThreadManage<OPMLogic>
{
public:
	static OPMLogic* GetSingleton();
	static void Destroy();

	// �����߳�
	bool StartDataCollectThreadThread();
	bool StartVerifyChannelThread();
	bool StartLoadDataThread();
	bool StartMonitorPMThread();
	bool StartAutoCalcDXThread();
	
	// ���ܺ���
	bool OpenVOAComm( DWORD port );				// ��voa����
	bool AutoAdjustPM( float dstPM );			// �Զ�������ʼ����ֵ

	void VerifyChannelThread();					// ��֤ͨ��
	bool VerifyCollectData( UCHAR channel );	// ��ͨ��������֤

	bool TestDB();								// db�������
	void LoadDataThread();						// �����ݿ���������
	bool LoadDataFromDB( ULONG sn );				// �����ݿ��ж�ȡ����
	
	void MonitorPMThread();							// ��ز���pm
	void AutoCalcDXThread();					// ��ز���pm

	OPMConfig _configInfor;						// ������Ϣ

	VECTOR_FLOAT _channelDXList[16];
	float _channelOffsetList[16];

protected:
	OPMLogic();
	void DataCollectThread();
	
	static OPMLogic* _pSingleton;

	VOAXX _voa;								// ����豸
	OPMML9001A _refPM;

	DeviceInfo _devInfor;						// �豸��Ϣ
	
	//�����ͨ���������
	float _autoAdjustAtten;						// ��ʼ�Զ��������� -> voa�ĳ�ʼ˥��

	bool _preFlag;
	float _gearList[4];							// 4�����ֵ
};

extern OPMLogic gOPMLogic;


extern PFunc_View_Type0 PCB_View_ShowMsg;