
#include "stdafx.h"
#include "OPMLogic.h"
using namespace std;

PFunc_View_Type0 PCB_View_ShowMsg			= NULL;

OPMLogic* OPMLogic::_pSingleton				= NULL;

PFunc_View_Type101 PCB_View_ShowVerifyResult;
PFunc_View_Type102 PCB_View_ShowDXPM;


double _Round( double d, int decimals )
{
	bool positiveFlag;
	if( d>0 )
	{
		positiveFlag = true;
	}
	else if( d<0 )
	{
		d = -d;
		positiveFlag = false;
	}
	else
	{
		return 0;
	}

	// �ֽ��������ֺ�С������  �ֽ����������ֵ����Χ
	double integer = ( double )( int )d;
	double decimal = d - integer;

	// �������ı���
	double multiple = ( double )pow( 10.0, decimals );

	// decimalReserveΪ����λ����С��ֵ
	double decimalReserve =  ( ( double )( int )( decimal*multiple ) )/multiple;

	// decimalNoReserve����λ����С��ֵ
	double decimalNoReserve = decimal-decimalReserve;

	// ��С����Чֵ  eg��1��0.1�� 0.01
	double minEffectiveValue = ( double )pow( 10.0, -decimals );
	// �Ƚ�ֵ
	double compairValue = minEffectiveValue/2;

	// �ȽϹ���
	double dRt;
	double dx = decimalNoReserve-compairValue;
	if( dx<0 )
	{
		dRt = integer+decimalReserve;
	}
	else if( dx>0 )
	{
		dRt = integer+decimalReserve+minEffectiveValue;
	}
	else
	{	// ȡż
		if( ( int )( decimalReserve*multiple )%2==0 )
			dRt = integer+decimalReserve;
		else
			dRt = integer+decimalReserve+minEffectiveValue;
	}

	if( positiveFlag )
		return dRt;
	else
		return -dRt;
}


OPMLogic* OPMLogic::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new OPMLogic;

	return _pSingleton;
}

void OPMLogic::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}

OPMLogic::OPMLogic()
{
	_gearList[0] = -5;
	_gearList[1] = -20;
	_gearList[2] = -35;
	_gearList[3] = -50;
}

bool OPMLogic::OpenVOAComm( DWORD port )
{
	return _voa.Open( port, 9600 );
}

bool OPMLogic::TestDB()
{
	// ������ݿ��Ƿ���������
	if( IsConnectOLMDB()==false )
	{
		PCB_View_ShowMsg( "�������ݿ�ʧ��.." );
		return false;
	}

	// ��⵱ǰsn���Ƿ������ݿ��� �����������
	if( IsDBOPMSNExist( _configInfor._sn )==false )
	{
		PCB_View_ShowMsg( "���ݿ��в����ڵ�ǰsn��..׼������.." );
		if( !InsertDBOPM( _devInfor._singleDiscSN, _devInfor._ipAddr, (UCHAR*)_devInfor._mac, _devInfor._softVer, _devInfor._hardwareVer) )
		{
			PCB_View_ShowMsg( "��ǰsn��д�����ݿ�ʧ��.." );
			return false;
		}
	}

	return true;
}

bool OPMLogic::StartDataCollectThreadThread()
{
	StopThread();

	return StartThread( &OPMLogic::DataCollectThread );
}

void OPMLogic::DataCollectThread()
{
	string str;

	/*--------------------------------------------------���׼������---------------------------------------------------*/
	// ��鴮��
	if( _voa.IsOpen()==false )
	{
		PCB_View_ShowMsg( "����δ��..ֹͣ����.." );
		return;
	}
		
	//// �������ݿ�
	if( TestDB()==false )								
		return;

	// �����ʾ
	::MessageBoxA( NULL, "������д��ͨ��������ͨ���Ƿ�һ��!!", "��ʾ", 0 );

	// ��ʼ�����չ��ʼ�
	PCB_View_ShowMsg( "��ʼ�����չ��ʼ�.." );
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "���չ��ʼƳ�ʼ��ʧ�ܣ��޷�����.." );
		return;
	}


	/*--------------------------------------------------���ݲɼ�---------------------------------------------------*/
	// ѡ��ͨ�����
	if( _configInfor._channelList.size()==0 )
		return;

	// ����ԭ������
	for(int i=0; i<16; ++i)									
		_channelDXList[i].clear();

	if( _configInfor._channelList[0]==1 )
	{
		// ���ԭ��λֵ
		float dx = 0;
		for( int i=0; i<5; ++i )
			while( SetOPM2DxSyn( 1, 1, i, &dx, SELDEV_IP )==false ) { Sleep( 100 ); }

		// �Ե�1ͨ������4��λ����  ˥����������λ����¼dxֵ
		for( int i=0; i<4; ++i )
		{
			// �������õ�λ
			bool rt = AutoAdjustPM( _gearList[i] );
			if( rt==false )
				return;

			float referPM = _refPM.ReadPower();
			// ��������
			{
				UINT channel = _configInfor._channelList[0];				// ��ǰͨ��
				float fixReferPM = referPM-_configInfor._dxPM[channel-1];	// ������Ĳ���ֵ

				// ��ȡ����ֵ
				float pm;
				while( !GetOPM2ADPMSyn( channel, channel, &pm, SELDEV_IP ) ) { Sleep( 100 ); }

				float dx = fixReferPM-pm;
				_channelDXList[channel-1].push_back( dx );

				STRING_FORMAT( str, "ͨ��%d ��%d��, �ο�ֵ:%0.01f,����ֵ:%0.01f,��ֵ:%0.01f", channel, i+1, fixReferPM, pm, dx );
				PCB_View_ShowMsg( str.c_str() );

				// ����
				while( !SetOPM2DxSyn( channel, channel, i, &dx, SELDEV_IP ) );
			}
		}
	}

	// ������ͨ������ƫ��ֵ
	{
		bool rt = AutoAdjustPM( -30 );
		if( rt==false )
			return;
		//
		float referPM = _refPM.ReadPower();
		
		//
		for( UINT i=0; i<_configInfor._channelList.size(); ++i )
		{
			UINT channel = _configInfor._channelList[i];				// ��ǰͨ��
			if( channel==1 )
				continue;

			// ���
			float dx = 0;
			while( SetOPM2DxSyn(channel, channel, 6, &dx, SELDEV_IP )==false );
			Sleep( 2000 );

			//
			float fixReferPM = referPM-_configInfor._dxPM[channel-1];	// ������Ĳ���ֵ
			//
			float pm;
			while( GetOPM2ADPMSyn(channel, channel, &pm, SELDEV_IP )==false );
			//
			dx = fixReferPM-pm;
			_channelOffsetList[channel-1] = dx;

			STRING_FORMAT( str, "ͨ��%d, �ο�ֵ:%0.01f,����ֵ:%0.01f,��ֵ:%0.01f", channel, fixReferPM, pm, dx );
			PCB_View_ShowMsg( str.c_str() );

			// ����
			while( SetOPM2DxSyn(channel, channel, 6, &dx, SELDEV_IP )==false );
		}
	}
	
	//
	STRING_FORMAT( str, "�������ݲɼ� �������.." );
	PCB_View_ShowMsg( str.c_str() );


	/*--------------------------------------------------��֤ д�����ݿ�---------------------------------------------------*/
	for( UINT j=0; j<_configInfor._channelList.size(); ++j )
	{
		UCHAR channel = _configInfor._channelList[j];

		AutoAdjustPM( 0 );
		VerifyCollectData( channel );		// ������֤

		//д�����ݿ�
		//��֯����
		STRING_FORMAT( str, "ͨ��%d��ֵ����д�����ݿ�..", channel);
		PCB_View_ShowMsg( str.c_str() );

		string contentStr;
		if( channel==1 )
		{
			STRING_FORMAT( str, "'%dͨ��:\r\n", channel);
			contentStr += str;
			//
			for(int k=0; k<4; ++k)
			{	
				STRING_FORMAT( str, "%d��: %0.2f\r\n", k+1, _channelDXList[channel-1][k]);
				contentStr += str;
			}
			//
			contentStr += '\'';
		}
		else
		{
			STRING_FORMAT( str, "'%dͨ��:\r\n", channel);
			contentStr += str;

			STRING_FORMAT( str, "ƫ��: %0.2f\r\n", _channelOffsetList[channel-1]);
			contentStr += str;
			//
			contentStr += '\'';
		}
		
		UpdateDBOPMChannelData( _configInfor._sn, _configInfor._waveLen.c_str(), channel, contentStr );
	}
}

bool OPMLogic::StartVerifyChannelThread()
{
	StopThread();

	return StartThread( &OPMLogic::VerifyChannelThread );
}

void OPMLogic::VerifyChannelThread()
{
	if( IsConnectOLMDB()==false )						// �������ݿ�����
	{
		PCB_View_ShowMsg( "��������ʧ��.." );
		return;
	}
	if( LoadDataFromDB( _configInfor._sn )==false )		// �����ݿ������������
		return;

	_refPM.Init( _configInfor._pmGPIBAddr );			// ��ʼ�����չ��ʼ�
	AutoAdjustPM( 0 );									// �Զ�����VOA ʹ�ó�ʼ����Ϊ��ʼ����
	
	for( UINT j=0; j<_configInfor._channelList.size(); ++j )
		VerifyCollectData( _configInfor._channelList[j] );
}

bool OPMLogic::VerifyCollectData( UCHAR channel )
{
	string str;
	STRING_FORMAT( str, "��֤ͨ��%d�ɼ�������..", channel );
	PCB_View_ShowMsg( str.c_str() );

	// ��������֤ 0��-65
	int count = static_cast<int>( 65/_configInfor._verifyStep )+1;
	STRING_FORMAT( str, "ͨ��%d ��0��-65 ��ʼ������⣬���%d��..", channel, count );
	PCB_View_ShowMsg( str.c_str() );
	//
	vector<pair<float, float> > exceptionPointList;
	float referPM;	
	float pmValue;
	for(int i=0; i<count; ++i)
	{
		// ����˥��0 ����ʱ�ȶ�
		_voa.SetVOAAbsoluteAtten( _autoAdjustAtten+i*_configInfor._verifyStep );
		if( i==0 )
			Sleep( 10000 );
		else
			Sleep( 3000 );
	
		// ��ȡ���չ���ֵ �� ���Թ���ֵ
		referPM = _refPM.ReadPower();						// ��ȡ���չ��ʼ�ֵ
		float fixReferPM = referPM-_configInfor._dxPM[channel-1];		// ������Ĳ���ֵ
		// ��ȡOPM���ʼ�ֵ
		while( !GetOPM2ADPMSyn( channel, channel, &pmValue, SELDEV_IP ) ) { Sleep( 100 ); }
		STRING_FORMAT( str, "ͨ��%d %d�������	�ο���׼����%0.2lf	��ȡ����%0.2f", channel, i+1, fixReferPM, pmValue );
		PCB_View_ShowMsg( str.c_str() );
		
		// �쳣���ֵ
		float dx = 0;
		if( fixReferPM>-50 )
			dx = 0.2f;
		else
			dx = 0.4f;

		// ������Χ�Ĳ��ж�
		if( fixReferPM<-65 )
			continue;

		// �ж��쳣�㲢����
		if( abs( fixReferPM-pmValue )>dx )
			exceptionPointList.push_back( pair<float, float>( fixReferPM, pmValue ) );
	}

	if( exceptionPointList.size()>0 )
	{
		STRING_FORMAT( str, "ͨ��%d�Զ���������ʧ��..", channel );
		PCB_View_ShowMsg( str.c_str() );
		PCB_View_ShowVerifyResult( exceptionPointList, channel );
		return false;
	}
	else
	{
		STRING_FORMAT( str, "ͨ��%d�Զ���������ɹ�..", channel );
		PCB_View_ShowMsg( str.c_str() );
		return true;
	}
}

bool OPMLogic::StartLoadDataThread()
{
	StopThread();

	return StartThread( &OPMLogic::LoadDataThread );
}

void OPMLogic::LoadDataThread()
{
	LoadDataFromDB( _configInfor._sn );
}

bool OPMLogic::LoadDataFromDB( ULONG sn )
{
	for(int i=0; i<16; ++i)
		_channelDXList[i].clear();
	
	// �����ݿ�������ݲ�д���ı�
	TxtOBJ txt;
	txt.CreateFile( "OPM2��������.txt" );
	//
	for(int i=0; i<16; ++i)
	{
		string contentStr;
		if( !SelectDBOPMChannelData( _configInfor._sn, _configInfor._waveLen.c_str(), i+1, contentStr ) )
		{
			::MessageBoxA( NULL, "�����ݿ�opm���ж�ȡ����ʧ��..", "�澯", 0 );
			return false;
		}
		txt.WriteToEnd( contentStr.c_str() );
	}

	// �����ı�
	vector<VECTOR_STRING > allLineWordList;
	txt.ReadAllLineWords( allLineWordList );
	int channel = -1;

	for( UINT i=0; i<allLineWordList.size(); ++i )
	{
		// ��ñ��е�words
		VECTOR_STRING& words = allLineWordList[i];

		// �ж��Ƿ�ΪΪͨ���ű�ʾ
		if( words.size()==1 )
		{
			channel = atoi( words[0].c_str() );
			continue;
		}

		if( channel==1 )
			_channelDXList[channel-1].push_back( (float)atof( words[1].c_str()) );
		else
			_channelOffsetList[channel-1] = (float)atof( words[1].c_str() );
	}

	PCB_View_ShowMsg( "��������ɹ�.." );
	return true;
}

bool OPMLogic::StartMonitorPMThread()
{
	StopThread();

	return StartThread( &OPMLogic::MonitorPMThread );
}

void OPMLogic::MonitorPMThread()
{
	//��ʼ�����չ��ʼ�
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "��ʼ�����չ��ʼ�ʧ��..ֹͣ����.." );
		return;
	}

	while(1)
	{
		Sleep(500);
		float referPM = _refPM.ReadPower();

		string str;
		STRING_FORMAT( str, "��׼PM���ʣ�%0.3lf", referPM );

		PCB_View_ShowMsg( str.c_str() );
	}
}

bool OPMLogic::StartAutoCalcDXThread()
{
	return StartThread( &OPMLogic::AutoCalcDXThread );
}

void OPMLogic::AutoCalcDXThread()
{
	// ��ʼ�����չ��ʼ�
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "��ʼ�����չ��ʼ�ʧ��..ֹͣ����.." );
		return;
	}
	
	string str;
	float referPM = _refPM.ReadPower();
	STRING_FORMAT( str, "��ȡ��׼����ֵ%0.2f..", referPM );
	PCB_View_ShowMsg( str.c_str() );

	for( UINT i=0; i<_configInfor._channelList.size(); ++i )
		_configInfor._dxPM[_configInfor._channelList[i]] = referPM - _configInfor._dxPM[_configInfor._channelList[i]];
	
	PCB_View_ShowDXPM();
	PCB_View_ShowMsg( "�Զ������ֵ���.." );
}

bool OPMLogic::AutoAdjustPM( float dstPM )
{
	//��ʼ��VOA
	_voa._waveLen = _configInfor._waveLen;
	PCB_View_ShowMsg( "���VOA��˥��ֵ.." );
	_voa.ClearVOA();
	Sleep( 3000 );

	//������������ʼֵ����Ϊ˥����Χ����ʼֵ
	float referPM;
	float dx;
	_autoAdjustAtten=0;
	string str;
	for(int i=0; i<3; ++i)
	{
		STRING_FORMAT( str, "���������ʵ�%d���Զ���������..", i+1 );
		PCB_View_ShowMsg( str.c_str() );

		referPM = _refPM.ReadPower();
		if( ( referPM<dstPM ) && ( i==0 ) )
		{
			STRING_FORMAT( str, "�����������Ϊ%f������˥����Χ��ʼֵ���޷�����..", referPM );
			PCB_View_ShowMsg( str.c_str() );
			Sleep( 3000 );
			return false;
		}
		
		dx = (float)referPM-dstPM;
		dx = (float)_Round(dx, 1);
		_autoAdjustAtten += dx;
		STRING_FORMAT( str, "����������Ϊ%f����ʼֵΪ%f, ����%f..", referPM, _configInfor._attenBegin, dx );
		PCB_View_ShowMsg( str.c_str() );
		_voa.SetVOARelativeAtten(dx);
		Sleep( 3000 );
	}
	PCB_View_ShowMsg( "���������ʵ������.." );
	//_autoAdjustAtten = _voa.GetVOA(waveFlag);
	STRING_FORMAT( str, "���յĵ���ֵΪ%0.2f..", _autoAdjustAtten );
	PCB_View_ShowMsg( str.c_str() );
	Sleep( 1000 );

	return true;
}