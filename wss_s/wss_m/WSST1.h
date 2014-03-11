
#pragma once

#include "WSSTX.h"

typedef void (*PFunc_View_Type101)( VECTOR_FLOAT& xList, VECTOR_FLOAT& yList, VECTOR_FLOAT& xYList, VECTOR_FLOAT& yYList );
typedef void (*PFunc_View_Type102)();

extern PFunc_View_Type0 PCB_View_ShowMsg;
extern PFunc_View_Type101 PCB_View_ShowYLCurve;
extern PFunc_View_Type102 PCB_View_ShowYLGrid;


class WSST1 : public _SingleThreadManage<WSST1>
{	   
	/*--------------------------------公共--------------------------------*/
public:
	static WSST1* GetSingleton();
	static void Destroy();
	WSST1();
	~WSST1();

	void ConnectDevices( VECTOR_STRING& msgList );
	void DisconnectDevices();
	void StartSaveLightProcess( int port=-1 );
	void LoadSaveLight( int port );

	OSW32X _osw;

	ULONG _osaIP;						// osa地址
	ULONG _osaPort;		
	ULONG _oswCom;
	ULONG _oswChannel;

private:
	void StartSaveLightThread();
	void SaveLightThread();
	void SaveSaveLight( std::string& str, int port );
	

	static WSST1* _pSingleton;

	OSA _osa;

	VECTOR_INT _slPortList;				// 需要存光的port
	VECTOR_FLOAT _slDataList[9];		// 存光值


	/*--------------------------------黄线功能--------------------------------*/
public:
	void StartYellowLightProcess();
	void StopYellowLightProcess();

	int _ylOffset;						// 黄线对比起始index
	int _ylDBFlag;						// 计算黄线用的中心波长  1:0.5db  2:3db
	float _rippleDX;					// 用来计算ripple的波段范围 默认为10
	float _lowPeakLimit;

private:
	void StartYellowLightThread();
	void YellowLightThread();
	void CalcYLIndicator( VECTOR_FLOAT& xList, VECTOR_FLOAT& yList );

	bool _isStopYLFlag;


	/*--------------------------------自动加电压功能--------------------------------*/
public:
	bool GetOSATrace( VECTOR_FLOAT& xNMList, VECTOR_FLOAT& vList );
};



