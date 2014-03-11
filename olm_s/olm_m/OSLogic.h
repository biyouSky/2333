
#pragma once

typedef void(*PFunc_View_Type101)( float k, float b ); 
typedef bool(*PFunc_View_Type102)( ULONG IBMON, ULONG IMPDMON, ULONG resistance );
typedef void(*PFunc_View_Type103)( float* readPM, float* referPM ); 

extern PFunc_View_Type0 PCB_View_ShowMsg;
extern PFunc_View_Type101 PCB_View_ShowKB;
extern PFunc_View_Type102 PCB_View_ShowResistance;
extern PFunc_View_Type103 PCB_View_ShowVerifyKB;


class OSLogic :  public _SingleThreadManage<OSLogic>
{
public:
	static OSLogic* GetSingleton();
	static void Destroy();

	void StartCalcOSKBThread();
	void StartVerifyOSKBThread();

	std::string k1[2];
	std::string k2[2];

private:
	OSLogic();
	void CalcOSKBThread();
	bool AutoAdjustPM( float pm );

	void VerifyOSKBThread();

	static OSLogic* _pSingleton;
	OPMML9001A _refPM;
};