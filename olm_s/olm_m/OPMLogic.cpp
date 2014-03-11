
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

	// 分解整数部分和小数部分  分解后处理扩大数值处理范围
	double integer = ( double )( int )d;
	double decimal = d - integer;

	// 求出扩大的倍数
	double multiple = ( double )pow( 10.0, decimals );

	// decimalReserve为保留位数内小数值
	double decimalReserve =  ( ( double )( int )( decimal*multiple ) )/multiple;

	// decimalNoReserve保留位数外小数值
	double decimalNoReserve = decimal-decimalReserve;

	// 最小的有效值  eg：1，0.1， 0.01
	double minEffectiveValue = ( double )pow( 10.0, -decimals );
	// 比较值
	double compairValue = minEffectiveValue/2;

	// 比较过程
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
	{	// 取偶
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
	// 检测数据库是否能连接上
	if( IsConnectOLMDB()==false )
	{
		PCB_View_ShowMsg( "连接数据库失败.." );
		return false;
	}

	// 检测当前sn项是否在数据库中 不存在则插入
	if( IsDBOPMSNExist( _configInfor._sn )==false )
	{
		PCB_View_ShowMsg( "数据库中不存在当前sn项..准备存入.." );
		if( !InsertDBOPM( _devInfor._singleDiscSN, _devInfor._ipAddr, (UCHAR*)_devInfor._mac, _devInfor._softVer, _devInfor._hardwareVer) )
		{
			PCB_View_ShowMsg( "当前sn项写入数据库失败.." );
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

	/*--------------------------------------------------检查准备工作---------------------------------------------------*/
	// 检查串口
	if( _voa.IsOpen()==false )
	{
		PCB_View_ShowMsg( "串口未打开..停止任务.." );
		return;
	}
		
	//// 测试数据库
	if( TestDB()==false )								
		return;

	// 检查提示
	::MessageBoxA( NULL, "请检查填写的通道与插入的通道是否一致!!", "提示", 0 );

	// 初始化参照功率计
	PCB_View_ShowMsg( "初始化参照功率计.." );
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "参照功率计初始化失败，无法定标.." );
		return;
	}


	/*--------------------------------------------------数据采集---------------------------------------------------*/
	// 选中通道检查
	if( _configInfor._channelList.size()==0 )
		return;

	// 清理原有数据
	for(int i=0; i<16; ++i)									
		_channelDXList[i].clear();

	if( _configInfor._channelList[0]==1 )
	{
		// 清空原档位值
		float dx = 0;
		for( int i=0; i<5; ++i )
			while( SetOPM2DxSyn( 1, 1, i, &dx, SELDEV_IP )==false ) { Sleep( 100 ); }

		// 对第1通道进行4档位定标  衰减到各个档位并记录dx值
		for( int i=0; i<4; ++i )
		{
			// 调整到该档位
			bool rt = AutoAdjustPM( _gearList[i] );
			if( rt==false )
				return;

			float referPM = _refPM.ReadPower();
			// 保存数据
			{
				UINT channel = _configInfor._channelList[0];				// 当前通道
				float fixReferPM = referPM-_configInfor._dxPM[channel-1];	// 修正后的参照值

				// 读取采样值
				float pm;
				while( !GetOPM2ADPMSyn( channel, channel, &pm, SELDEV_IP ) ) { Sleep( 100 ); }

				float dx = fixReferPM-pm;
				_channelDXList[channel-1].push_back( dx );

				STRING_FORMAT( str, "通道%d 第%d档, 参考值:%0.01f,采样值:%0.01f,差值:%0.01f", channel, i+1, fixReferPM, pm, dx );
				PCB_View_ShowMsg( str.c_str() );

				// 发送
				while( !SetOPM2DxSyn( channel, channel, i, &dx, SELDEV_IP ) );
			}
		}
	}

	// 对其他通道计算偏移值
	{
		bool rt = AutoAdjustPM( -30 );
		if( rt==false )
			return;
		//
		float referPM = _refPM.ReadPower();
		
		//
		for( UINT i=0; i<_configInfor._channelList.size(); ++i )
		{
			UINT channel = _configInfor._channelList[i];				// 当前通道
			if( channel==1 )
				continue;

			// 清空
			float dx = 0;
			while( SetOPM2DxSyn(channel, channel, 6, &dx, SELDEV_IP )==false );
			Sleep( 2000 );

			//
			float fixReferPM = referPM-_configInfor._dxPM[channel-1];	// 修正后的参照值
			//
			float pm;
			while( GetOPM2ADPMSyn(channel, channel, &pm, SELDEV_IP )==false );
			//
			dx = fixReferPM-pm;
			_channelOffsetList[channel-1] = dx;

			STRING_FORMAT( str, "通道%d, 参考值:%0.01f,采样值:%0.01f,差值:%0.01f", channel, fixReferPM, pm, dx );
			PCB_View_ShowMsg( str.c_str() );

			// 发送
			while( SetOPM2DxSyn(channel, channel, 6, &dx, SELDEV_IP )==false );
		}
	}
	
	//
	STRING_FORMAT( str, "定标数据采集 发送完成.." );
	PCB_View_ShowMsg( str.c_str() );


	/*--------------------------------------------------验证 写入数据库---------------------------------------------------*/
	for( UINT j=0; j<_configInfor._channelList.size(); ++j )
	{
		UCHAR channel = _configInfor._channelList[j];

		AutoAdjustPM( 0 );
		VerifyCollectData( channel );		// 抽样验证

		//写入数据库
		//组织数据
		STRING_FORMAT( str, "通道%d插值数据写入数据库..", channel);
		PCB_View_ShowMsg( str.c_str() );

		string contentStr;
		if( channel==1 )
		{
			STRING_FORMAT( str, "'%d通道:\r\n", channel);
			contentStr += str;
			//
			for(int k=0; k<4; ++k)
			{	
				STRING_FORMAT( str, "%d档: %0.2f\r\n", k+1, _channelDXList[channel-1][k]);
				contentStr += str;
			}
			//
			contentStr += '\'';
		}
		else
		{
			STRING_FORMAT( str, "'%d通道:\r\n", channel);
			contentStr += str;

			STRING_FORMAT( str, "偏移: %0.2f\r\n", _channelOffsetList[channel-1]);
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
	if( IsConnectOLMDB()==false )						// 测试数据库连接
	{
		PCB_View_ShowMsg( "连接数据失败.." );
		return;
	}
	if( LoadDataFromDB( _configInfor._sn )==false )		// 从数据库读出定标数据
		return;

	_refPM.Init( _configInfor._pmGPIBAddr );			// 初始化参照功率计
	AutoAdjustPM( 0 );									// 自动调整VOA 使得初始功率为起始功率
	
	for( UINT j=0; j<_configInfor._channelList.size(); ++j )
		VerifyCollectData( _configInfor._channelList[j] );
}

bool OPMLogic::VerifyCollectData( UCHAR channel )
{
	string str;
	STRING_FORMAT( str, "验证通道%d采集的数据..", channel );
	PCB_View_ShowMsg( str.c_str() );

	// 抽样点验证 0到-65
	int count = static_cast<int>( 65/_configInfor._verifyStep )+1;
	STRING_FORMAT( str, "通道%d 从0到-65 开始抽样检测，检测%d次..", channel, count );
	PCB_View_ShowMsg( str.c_str() );
	//
	vector<pair<float, float> > exceptionPointList;
	float referPM;	
	float pmValue;
	for(int i=0; i<count; ++i)
	{
		// 设置衰减0 并延时稳定
		_voa.SetVOAAbsoluteAtten( _autoAdjustAtten+i*_configInfor._verifyStep );
		if( i==0 )
			Sleep( 10000 );
		else
			Sleep( 3000 );
	
		// 读取参照功率值 和 测试功率值
		referPM = _refPM.ReadPower();						// 读取参照功率计值
		float fixReferPM = referPM-_configInfor._dxPM[channel-1];		// 修正后的参照值
		// 读取OPM功率计值
		while( !GetOPM2ADPMSyn( channel, channel, &pmValue, SELDEV_IP ) ) { Sleep( 100 ); }
		STRING_FORMAT( str, "通道%d %d抽样检查	参考标准功率%0.2lf	读取功率%0.2f", channel, i+1, fixReferPM, pmValue );
		PCB_View_ShowMsg( str.c_str() );
		
		// 异常误差值
		float dx = 0;
		if( fixReferPM>-50 )
			dx = 0.2f;
		else
			dx = 0.4f;

		// 超过范围的不判定
		if( fixReferPM<-65 )
			continue;

		// 判断异常点并保存
		if( abs( fixReferPM-pmValue )>dx )
			exceptionPointList.push_back( pair<float, float>( fixReferPM, pmValue ) );
	}

	if( exceptionPointList.size()>0 )
	{
		STRING_FORMAT( str, "通道%d自动抽样点检查失败..", channel );
		PCB_View_ShowMsg( str.c_str() );
		PCB_View_ShowVerifyResult( exceptionPointList, channel );
		return false;
	}
	else
	{
		STRING_FORMAT( str, "通道%d自动抽样点检查成功..", channel );
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
	
	// 从数据库读出数据并写入文本
	TxtOBJ txt;
	txt.CreateFile( "OPM2定标数据.txt" );
	//
	for(int i=0; i<16; ++i)
	{
		string contentStr;
		if( !SelectDBOPMChannelData( _configInfor._sn, _configInfor._waveLen.c_str(), i+1, contentStr ) )
		{
			::MessageBoxA( NULL, "从数据库opm表中读取数据失败..", "告警", 0 );
			return false;
		}
		txt.WriteToEnd( contentStr.c_str() );
	}

	// 解析文本
	vector<VECTOR_STRING > allLineWordList;
	txt.ReadAllLineWords( allLineWordList );
	int channel = -1;

	for( UINT i=0; i<allLineWordList.size(); ++i )
	{
		// 获得本行的words
		VECTOR_STRING& words = allLineWordList[i];

		// 判断是否为为通道号标示
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

	PCB_View_ShowMsg( "数据载入成功.." );
	return true;
}

bool OPMLogic::StartMonitorPMThread()
{
	StopThread();

	return StartThread( &OPMLogic::MonitorPMThread );
}

void OPMLogic::MonitorPMThread()
{
	//初始化参照功率计
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "初始化参照功率计失败..停止任务.." );
		return;
	}

	while(1)
	{
		Sleep(500);
		float referPM = _refPM.ReadPower();

		string str;
		STRING_FORMAT( str, "标准PM功率：%0.3lf", referPM );

		PCB_View_ShowMsg( str.c_str() );
	}
}

bool OPMLogic::StartAutoCalcDXThread()
{
	return StartThread( &OPMLogic::AutoCalcDXThread );
}

void OPMLogic::AutoCalcDXThread()
{
	// 初始化参照功率计
	if( _refPM.Init( _configInfor._pmGPIBAddr )==false )
	{
		PCB_View_ShowMsg( "初始化参照功率计失败..停止任务.." );
		return;
	}
	
	string str;
	float referPM = _refPM.ReadPower();
	STRING_FORMAT( str, "读取标准功率值%0.2f..", referPM );
	PCB_View_ShowMsg( str.c_str() );

	for( UINT i=0; i<_configInfor._channelList.size(); ++i )
		_configInfor._dxPM[_configInfor._channelList[i]] = referPM - _configInfor._dxPM[_configInfor._channelList[i]];
	
	PCB_View_ShowDXPM();
	PCB_View_ShowMsg( "自动计算差值完毕.." );
}

bool OPMLogic::AutoAdjustPM( float dstPM )
{
	//初始化VOA
	_voa._waveLen = _configInfor._waveLen;
	PCB_View_ShowMsg( "清空VOA的衰减值.." );
	_voa.ClearVOA();
	Sleep( 3000 );

	//将激光器的起始值设置为衰减范围的起始值
	float referPM;
	float dx;
	_autoAdjustAtten=0;
	string str;
	for(int i=0; i<3; ++i)
	{
		STRING_FORMAT( str, "激光器功率第%d次自动调整计算..", i+1 );
		PCB_View_ShowMsg( str.c_str() );

		referPM = _refPM.ReadPower();
		if( ( referPM<dstPM ) && ( i==0 ) )
		{
			STRING_FORMAT( str, "激光器最大功率为%f，低于衰减范围起始值，无法定标..", referPM );
			PCB_View_ShowMsg( str.c_str() );
			Sleep( 3000 );
			return false;
		}
		
		dx = (float)referPM-dstPM;
		dx = (float)_Round(dx, 1);
		_autoAdjustAtten += dx;
		STRING_FORMAT( str, "激光器功率为%f，起始值为%f, 调整%f..", referPM, _configInfor._attenBegin, dx );
		PCB_View_ShowMsg( str.c_str() );
		_voa.SetVOARelativeAtten(dx);
		Sleep( 3000 );
	}
	PCB_View_ShowMsg( "激光器功率调整完毕.." );
	//_autoAdjustAtten = _voa.GetVOA(waveFlag);
	STRING_FORMAT( str, "最终的调整值为%0.2f..", _autoAdjustAtten );
	PCB_View_ShowMsg( str.c_str() );
	Sleep( 1000 );

	return true;
}