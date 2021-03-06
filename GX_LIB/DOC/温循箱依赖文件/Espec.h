#if !defined(AFX_ESPEC_EXPORTS_H__2D9A780E_E209_11D7_8252_00E04C3D92DB__INCLUDED_)
#define AFX_ESPEC_EXPORTS_H__2D9A780E_E209_11D7_8252_00E04C3D92DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ESPECDLL_EXPORTS
#define ESPECDLL_API __declspec(dllexport)
#else
#define ESPECDLL_API __declspec(dllimport)
#endif


#define COMM_READ					0
#define COMM_WRITE					1

#define ERR_SUCCEED					0
#define ERR_FAIL					1000
#define ERR_INITED					1001
#define ERR_BUFFER_FULL				1002
#define ERR_OPEN_DATABASE			1003
#define ERR_OPEN_SERIAL_PORT		1004
#define ERR_INVALID_PARAM			1005
#define ERR_NO_OPENED				1006
#define ERR_INVALID_MACHID			1007

enum{
	ESPEC_POWER_OFF,
	ESPEC_POWER_ON,
	ESPEC_GATHER,
	ESPEC_MODEM,
	GET_SEG_TIMER,
	GET_CHAMBER_STATE,
	GET_SET_TEMP,
	GET_SET_HUMI,
	GET_SET_PARA,
	GET_ACT_TEMP,
	GET_ACT_HUMI,
	GET_ACT_PARA,
	GET_SWITCH,
	GET_RUN_PRO,
	GET_RUN_TIME,
	GET_RUN_SEG,
	GET_RUN_CYCLE,
	GET_RUN_CYCLE_TIME,
	GET_ERROR_NO,
	GET_HIGH_TEMP,
	GET_LOW_TEMP,
	GET_HEAT_OUT,
	GET_HUMI_OUT,
	GET_SAMPLE_POWER,
	GET_OUTSIDE_ALARM,
	GET_AIR_CONTROL_FLAG,
	GET_PRINT_SPEED,
	GET_SET_VALUE,
	GET_HUMI_CONTROL_FLAG,
	GET_CHAMBER_PROTET_TEMP,
	SET_CHAMBER_PROTET_TEMP,
	GET_CHAMBER_PROTET_HUMI,
	GET_SAMPLE_PROTET_TEMP,
	GET_SPECIAL_PARAM_FOR_QIRI,
	GET_SPECIAL_PARAM_FOR_AERO_OIL,
	GET_SPECIAL_PARAM_FOR_RAIN,
	GET_SPECIAL_PARAM_FOR_NORMAL_SUN,
	UPLOAD_PROGRAM,
	ESPEC_REAL_TIME_DATA
};

enum
{
	ESPEC_NO_MODEM,
	ESPEC_DIAL,
	ESPEC_CONNECT,
	ESPEC_UNCONNECT
};

extern "C" {
typedef int (CALLBACK MESSAGE_PROC)(
	DWORD MessageID,
	DWORD MacID ,
	long *lpParam
	);
}
typedef struct proseg_tag{
		long Hour;
		long Minuter;
		long Wait;
		long Temp;
		long Humi;
		long Param;
		long Switch;
		long TimeSwitch;
		long Reserve;
}PROSEG;

typedef struct{
	long BeginSeg;
	long EndSeg;
	long CycleTimes;
}CYCLESEG;

extern "C"
{

ESPECDLL_API DWORD ESPEC_Init(
	DWORD CommPort, 
	UINT  Baud,
	char  Parity,
	UINT  DataBits,
	UINT  StopBits,
	LPCTSTR lpPhoneNo,
	DWORD *lpDeviceSelectFlag, 
	DWORD *lpDeviceIs2000AFlag,
	MESSAGE_PROC lpMessageProc
	);
ESPECDLL_API DWORD ESPEC_RealTimeDataSave(DWORD *lpIntervalTime);
ESPECDLL_API DWORD ESPEC_SetDeviceSelect(DWORD *lpDeviceSelectFlag, DWORD *lpDeviceIs2000AFlag);
ESPECDLL_API DWORD ESPEC_SetIntervalTime(DWORD *lpIntervalTime);
ESPECDLL_API DWORD ESPEC_SetDeviceAdditionByte(DWORD *lpAdditionByte);
ESPECDLL_API void ESPEC_Close();
ESPECDLL_API DWORD ESPEC_DeviceType(DWORD *lpDeviceType);
ESPECDLL_API DWORD ESPEC_SendMessage(
	DWORD ReadOrWriteFlag,
	DWORD DeviceNo,
	DWORD BeginUnitNo,
	DWORD EntriesNum,
	DWORD CmdID,
	char *lpParam
	);

ESPECDLL_API BOOL ESPEC_CommInit();

ESPECDLL_API DWORD ESPEC_DownloadProgram(
	DWORD MacID, 
	DWORD ProNum, 
	PROSEG *lpProSeg, 
	CYCLESEG *lpCycleSeg, 
	DWORD EndOperate, 
	DWORD NextProNum
	);

ESPECDLL_API DWORD ESPEC_GetSegTimer(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetChamberState(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetChamberStartRun(DWORD MacID, DWORD StartType, DWORD StartProNum, DWORD StartSecNum, DWORD EndState);
ESPECDLL_API DWORD ESPEC_SetChamberStop(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetResetPlc(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSetTemp(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSetHumi(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSetPara(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetActTemp(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetActHumi(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetActPara(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSwitch(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetRunPro(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetRunTime(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetRunSeg(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetRunCycle(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetRunCycleTime(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetErrorNo(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetHighTemp(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetLowTemp(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetHeatOut(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetHumiOut(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetSamplePower(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetOutsideAlarm(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_GetAirControlFlag(DWORD MacID); 
ESPECDLL_API DWORD ESPEC_SetAirControlFlag(DWORD MacID, DWORD AirCtrFlag);
ESPECDLL_API DWORD ESPEC_GetPrintSpeed(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetPrintSpeed(DWORD MacID, DWORD PrintSpeed);
ESPECDLL_API DWORD ESPEC_SetPrint(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetPrintPro(DWORD MacID, DWORD ProNum);
ESPECDLL_API DWORD ESPEC_SetSetValue(DWORD MacID, long SetTemp, long SetHumi, long SetPara, DWORD HumiCtlFlag, DWORD Switch);
ESPECDLL_API DWORD ESPEC_GetSetValue(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetHumiControlFlag(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetChamberProtetTemp(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetChamberProtetHumi(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSampleProtetTemp(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetSampleProtetTemp(DWORD MacID, long HighTemp, long LowTemp);
ESPECDLL_API DWORD ESPEC_GetSpecialParamForQiRi(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetSpecialParamForAeroOil(DWORD MacID, long SetAeroOilTemp, long SetAeroOilPress);
ESPECDLL_API DWORD ESPEC_GetSpecialParamForAeroOil(DWORD MacID);
ESPECDLL_API DWORD ESPEC_GetSpecialParamForRain(DWORD MacID);
ESPECDLL_API DWORD ESPEC_ResetRecordForRain(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetSpecialParamForRain(DWORD MacID, long ControlFlag, long SetWaterLine, long SetFlux);
ESPECDLL_API DWORD ESPEC_GetSpecialParamForNormalSun(DWORD MacID);
ESPECDLL_API DWORD ESPEC_SetSpecialParamForNormalSun(DWORD MacID, long TotalSwitch, long SubSwitch, long SetRadiance1, long SetRadiance2, long SetRadiance3, long SetRadiance4);
ESPECDLL_API DWORD ESPEC_UploadProgram(DWORD MacID, long ProNum);

}
#endif