
#include "stdafx.h"
#include "OSLogic.h"


PFunc_View_Type0 PCB_View_ShowMsg				= NULL;
PFunc_View_Type101 PCB_View_ShowKB				= NULL;
PFunc_View_Type102 PCB_View_ShowResistance		= NULL;
PFunc_View_Type103 PCB_View_ShowVerifyKB		= NULL;

OSLogic* OSLogic::_pSingleton = NULL;


OSLogic* OSLogic::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new OSLogic;

	return _pSingleton;
}

void OSLogic::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}

OSLogic::OSLogic()
{
	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	str += "\\Config\\os_k.txt";

	TxtOBJ txt;
	if( txt.OpenFile( str.c_str() ) )
	{
		vector<vector<string> > allLineWordList;
		txt.ReadAllLineWords( allLineWordList );

		vector<string>& line1 = allLineWordList[1];
		k1[0] = line1[0];
		k1[1] = line1[1];
		
		vector<string>& line2 = allLineWordList[2];
		k2[0] = line2[0];
		k2[1] = line2[1];
	}
}

void OSLogic::StartCalcOSKBThread()
{
	StartThread( &OSLogic::CalcOSKBThread );
}

void OSLogic::CalcOSKBThread()
{
	string str;

	// 初始化功率计
	if( !_refPM.Init( 9 ) )
	{
		PCB_View_ShowMsg( "参照功率计初始化失败，无法定标..." );
		return;
	}

	// 获得os通道数
	PCB_View_ShowMsg( "获得os通道数..." );
	UCHAR portNum;
	while( !GetOSPortNumSyn( portNum, SELDEV_IP ) ) { Sleep( 100 ); }

	float pmList[2];
	if( portNum==2 )
	{
		pmList[0] = -2;
		pmList[1] = -1;
	}
	else if( portNum==8 )
	{
		pmList[0] = -8;
		pmList[1] = -7;
	}
	else
	{
		STRING_FORMAT( str, "os通道数为%d，不为2或8..", portNum ); 
		PCB_View_ShowMsg( str.c_str() );
		return;
	}

	// 调节2个点
	float sampleList[2];
	float referPMList[2];
	for(int i=0; i<2; ++i)
	{
		STRING_FORMAT( str, "调节功率到%0.2f..", pmList[i] );
		PCB_View_ShowMsg( str.c_str() );
		if( AutoAdjustPM( pmList[i] )==false )
			return;

		// 读取当前功率值与lsb值
		referPMList[i] = _refPM.ReadPower();
		USHORT sample;
		while( !GetOSSampleSyn( sample, SELDEV_IP ) ) { Sleep( 100 ); }
		sampleList[i] = sample;
		STRING_FORMAT( str, "功率值: %0.2f   lsb值: %0.2f ..", referPMList[i], sampleList[i] );
		PCB_View_ShowMsg( str.c_str() );
	}

	// dbm = 10lgLSB - b
	float b[2];
	for( int i=0; i<2; ++i )
		b[i] = 10*log10( sampleList[i] )-referPMList[i];

	float bAve = ( accumulate( b, b+2, 0.f ) )/2;
	PCB_View_ShowKB( 1, bAve );

	// 调整出光功率	并 保存当前电压值
	AutoAdjustPM( pmList[1] );
	SetOSResistance( 0x44, SELDEV_IP );
	PCB_View_ShowMsg( "k b自动计算完成.." );
}

bool OSLogic::AutoAdjustPM( float pm )
{
	while(1)
	{
		// 读取当前功率
		float referPM = _refPM.ReadPower();
		if( abs( referPM-pm )<0.1 )
			return true;

		// 调节
		/*
			电阻值 = 25+(RES+1)/1024*25000，单位欧姆。
			背光电流值 = SAMPLE2*K1，单位微安。（K1可在界面手工设置，默认设置为0.06103515625）
			驱动电流值 = SAMPLE0*K2，单位毫安。（K2可在界面手工设置，默认设置为0.29296875）
		*/
		USHORT IBMON, IMPDMON, resistanceLSB;
		while( SetOSResistanceSyn( referPM>pm ? 1:2, IBMON, IMPDMON, resistanceLSB, SELDEV_IP )==false );

		if( PCB_View_ShowResistance( IBMON, IMPDMON, resistanceLSB )==false )
		{
			::MessageBoxA( 0,  "超过告警值!" ,  "告警" , 0 );
			return false;
		}
	}
}

void OSLogic::StartVerifyOSKBThread()
{
	StartThread( &OSLogic::VerifyOSKBThread );
}

void OSLogic::VerifyOSKBThread()
{
	string str;

	// 初始化功率计
	if( _refPM.Init( 9 )==false )
	{
		PCB_View_ShowMsg( "参照功率计初始化失败，无法定标..." );
		return;
	}

	// 获得os通道数
	PCB_View_ShowMsg( "获得os通道数..." );
	UCHAR portNum;
	while( GetOSPortNumSyn( portNum, SELDEV_IP )==false );

	float pmList[3];
	if( portNum==2 )
	{
		pmList[2] = -2;
		pmList[1] = -1.5;
		pmList[0] = -1;
	}
	else if( portNum==8 )
	{
		pmList[2] = -8;
		pmList[1] = -7.5;
		pmList[0] = -7;
	}
	else
	{
		PCB_View_ShowMsg( "os通道数不为2或8..." );
		return;
	}

	// 调节2个点
	float referPMList[3];
	float readPMList[3];
	for(int i=0; i<3; ++i)
	{
		STRING_FORMAT( str, "调节功率到%0.2f.." , pmList[i] );
		PCB_View_ShowMsg( str.c_str() );

		if( AutoAdjustPM( pmList[i] )==false )
			return;

		// 读取当前功率值与lsb值
		while( GetOSOutPutPortPMSyn( 1, readPMList[i], SELDEV_IP )==false );
		referPMList[i] = _refPM.ReadPower();
	}

	// 显示到view
	AutoAdjustPM( pmList[0] );
	SetOSResistance( 0x44, SELDEV_IP );

	// 读取当前功率
	GetOSOutPutPM(  SELDEV_IP  );

	//
	PCB_View_ShowVerifyKB( readPMList, referPMList );
	PCB_View_ShowMsg( "检验过程完成..." );
}