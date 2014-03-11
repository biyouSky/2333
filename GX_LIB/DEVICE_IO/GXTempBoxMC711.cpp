#include "stdafx.h"
#include "GXTempBoxMC711.h"

struct TBStatus 
{
	// ʵ���¶�
	float _actTem;
	bool _actTemFlag;

	// Ŀ���¶�
	float _setTem;
	bool _setFlag;

	// ����״̬
	bool _chamberState;
	bool _chamberFlag;
};
static TBStatus gStatusList[16];		// macID: 0-15  ��¼16�����ӵ�����

int TBMC711::_obCount = 0;

int CALLBACK TBMC711::MessageProc(DWORD msgID, DWORD macID, long *lpParam)
{
	TBStatus& status = gStatusList[macID];

	switch(msgID)
	{
	case ESPEC_POWER_OFF:	// ����
		break;
		
	case ESPEC_POWER_ON:	// �ϵ�
		break;
		
	case GET_ACT_HUMI:		// ʵ��ʪ��
		break;
		
	case GET_ACT_TEMP:		// ʵ���¶�
		status._actTem = lpParam[0]/10.f;
		status._actTemFlag = true;
		break;
	
	case GET_SET_VALUE:		// �趨ֵ���¶ȡ�ʪ�ȡ�������ʪ�ȿ��ء�����
		status._setTem = lpParam[0]/10.f;
		//status.setVal.hum = lpParam[1] /*/ 10.0*/;
		//status.setVal.para = lpParam[2];
		//status.setVal.hswi = lpParam[3] != 0;
		//status.setVal.swi = lpParam[4];
		status._setFlag = true;
		break;
	
	case GET_CHAMBER_STATE:		// ����״̬
		status._chamberState = lpParam[0]&1;
		status._chamberFlag = true;
	}

	return 0;
}

TBMC711::TBMC711( DWORD macID, DWORD com )
{
	// ��¼��ǰmachine ID
	if( macID>16 ) throw 0;
	_macID = macID;

	// ��һ���������ʼ������
	if( ++_obCount==1 )
	{
		bool rt = InitSerial( com );
		assert( rt==true );
	}

	// ����macID��Ӧ������
	if( IsRunning()==false )
		Run();
}
TBMC711::~TBMC711() 
{	
	// ���һ���������ͷŽӿ�
	if( --_obCount==0 ) 
		ESPEC_Close();
}

bool TBMC711::InitSerial( DWORD com )
{
	DWORD DeviceSelect[16];
	DWORD Flag2000A[16];
	DWORD DeviceType[16];
	DWORD AdditionByte[16];
	     
	ZeroMemory( DeviceType, sizeof( DeviceType ) );
	ZeroMemory( DeviceSelect, sizeof( DeviceSelect ) );
	ZeroMemory( Flag2000A, sizeof( Flag2000A ) );
	ZeroMemory( AdditionByte, sizeof( AdditionByte ) );
	//
	for( int i=0; i<16; ++i )
	{
		DeviceSelect[0] = 1;
		Flag2000A[0] = 0;
		DeviceType[0] = 2;
	}

	DWORD rt;
	for( int i=0; i<3; ++i )
	{
		rt = ESPEC_Init( com, 19200, 'n', 8, 1, _T(""), /*"#2",*/ DeviceSelect, Flag2000A, &TBMC711::MessageProc );
		if( rt==ERR_SUCCEED )
			break;
		Sleep( 1000 );
	}
	ESPEC_SetDeviceAdditionByte ( AdditionByte );
	ESPEC_DeviceType ( DeviceType );
	    
	return rt!=ERR_SUCCEED ? false:true;
}

bool TBMC711::Stop() { return !IsRunning() ? true:!ESPEC_SetChamberStop( _macID ); }

bool TBMC711::Run() { return IsRunning() ? true:!ESPEC_SetChamberStartRun( _macID, 1, 0, 0, 1 ); }

bool TBMC711::IsRunning()
{
	TBStatus& status = gStatusList[_macID];

	BEGIN_WAIT()
	//
	status._chamberFlag = false;
	if( ESPEC_GetChamberState( _macID ) )
		return false;
	//
	END_WAIT_FLAG( 2000, status._chamberFlag );

	return status._chamberFlag & status._chamberState;
}

// ����Ŀ���¶�
bool TBMC711::SetSetTem( float tem ) { return !ESPEC_SetSetValue( _macID, static_cast<long>(tem*10), 0, 3230, 0, 1 ); }

bool TBMC711::GetSetTem( float& tem )
{
	TBStatus& status = gStatusList[_macID];

	BEGIN_WAIT()
	//
	status._setFlag = false;
	if( ESPEC_GetSetValue( _macID ) )
		return false;
	//
	END_WAIT_FLAG( 2000, status._setFlag );

	tem = status._setTem;
	return status._setFlag;
}

// ��ȡ��ǰ�¶�
bool TBMC711::GetActTem( float& tem )
{
	TBStatus& status = gStatusList[_macID];

	BEGIN_WAIT()
	//
	status._actTemFlag = false;
	if( ESPEC_GetActTemp( _macID ) )
		return false;
	//
	END_WAIT_FLAG( 2000, status._actTemFlag );

	tem = status._actTem;
	return status._actTemFlag;
}