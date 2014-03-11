
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

	// ��ʼ�����ʼ�
	if( !_refPM.Init( 9 ) )
	{
		PCB_View_ShowMsg( "���չ��ʼƳ�ʼ��ʧ�ܣ��޷�����..." );
		return;
	}

	// ���osͨ����
	PCB_View_ShowMsg( "���osͨ����..." );
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
		STRING_FORMAT( str, "osͨ����Ϊ%d����Ϊ2��8..", portNum ); 
		PCB_View_ShowMsg( str.c_str() );
		return;
	}

	// ����2����
	float sampleList[2];
	float referPMList[2];
	for(int i=0; i<2; ++i)
	{
		STRING_FORMAT( str, "���ڹ��ʵ�%0.2f..", pmList[i] );
		PCB_View_ShowMsg( str.c_str() );
		if( AutoAdjustPM( pmList[i] )==false )
			return;

		// ��ȡ��ǰ����ֵ��lsbֵ
		referPMList[i] = _refPM.ReadPower();
		USHORT sample;
		while( !GetOSSampleSyn( sample, SELDEV_IP ) ) { Sleep( 100 ); }
		sampleList[i] = sample;
		STRING_FORMAT( str, "����ֵ: %0.2f   lsbֵ: %0.2f ..", referPMList[i], sampleList[i] );
		PCB_View_ShowMsg( str.c_str() );
	}

	// dbm = 10lgLSB - b
	float b[2];
	for( int i=0; i<2; ++i )
		b[i] = 10*log10( sampleList[i] )-referPMList[i];

	float bAve = ( accumulate( b, b+2, 0.f ) )/2;
	PCB_View_ShowKB( 1, bAve );

	// �������⹦��	�� ���浱ǰ��ѹֵ
	AutoAdjustPM( pmList[1] );
	SetOSResistance( 0x44, SELDEV_IP );
	PCB_View_ShowMsg( "k b�Զ��������.." );
}

bool OSLogic::AutoAdjustPM( float pm )
{
	while(1)
	{
		// ��ȡ��ǰ����
		float referPM = _refPM.ReadPower();
		if( abs( referPM-pm )<0.1 )
			return true;

		// ����
		/*
			����ֵ = 25+(RES+1)/1024*25000����λŷķ��
			�������ֵ = SAMPLE2*K1����λ΢������K1���ڽ����ֹ����ã�Ĭ������Ϊ0.06103515625��
			��������ֵ = SAMPLE0*K2����λ��������K2���ڽ����ֹ����ã�Ĭ������Ϊ0.29296875��
		*/
		USHORT IBMON, IMPDMON, resistanceLSB;
		while( SetOSResistanceSyn( referPM>pm ? 1:2, IBMON, IMPDMON, resistanceLSB, SELDEV_IP )==false );

		if( PCB_View_ShowResistance( IBMON, IMPDMON, resistanceLSB )==false )
		{
			::MessageBoxA( 0,  "�����澯ֵ!" ,  "�澯" , 0 );
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

	// ��ʼ�����ʼ�
	if( _refPM.Init( 9 )==false )
	{
		PCB_View_ShowMsg( "���չ��ʼƳ�ʼ��ʧ�ܣ��޷�����..." );
		return;
	}

	// ���osͨ����
	PCB_View_ShowMsg( "���osͨ����..." );
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
		PCB_View_ShowMsg( "osͨ������Ϊ2��8..." );
		return;
	}

	// ����2����
	float referPMList[3];
	float readPMList[3];
	for(int i=0; i<3; ++i)
	{
		STRING_FORMAT( str, "���ڹ��ʵ�%0.2f.." , pmList[i] );
		PCB_View_ShowMsg( str.c_str() );

		if( AutoAdjustPM( pmList[i] )==false )
			return;

		// ��ȡ��ǰ����ֵ��lsbֵ
		while( GetOSOutPutPortPMSyn( 1, readPMList[i], SELDEV_IP )==false );
		referPMList[i] = _refPM.ReadPower();
	}

	// ��ʾ��view
	AutoAdjustPM( pmList[0] );
	SetOSResistance( 0x44, SELDEV_IP );

	// ��ȡ��ǰ����
	GetOSOutPutPM(  SELDEV_IP  );

	//
	PCB_View_ShowVerifyKB( readPMList, referPMList );
	PCB_View_ShowMsg( "����������..." );
}