
#include "stdafx.h"
#include "WSSCal.h"

PFunc_View_Type0 PCB_View_ShowMsg					= NULL;
PFunc_View_Type101 PCB_View_ShowCollectMsg			= NULL;			
PFunc_View_Type102 PCB_View_ShowCollectGrid			= NULL;					
PFunc_View_Type103 PCB_View_ClearCollectChart		= NULL;				
PFunc_View_Type104 PCB_View_AddCollectCurve			= NULL;			
PFunc_View_Type105 PCB_View_ShowParseGrid			= NULL;			
PFunc_View_Type106 PCB_View_ClearParseChart			= NULL;									
PFunc_View_Type107 PCB_View_AddParseCurve			= NULL;	 
PFunc_View_Type108 PCB_View_SetParseGridChannel		= NULL;	


CollectDataProcess* CollectDataProcess::_pSingleton = NULL;
ParseDataProcess* ParseDataProcess::_pSingleton = NULL;


bool EqualFloat( float& f, float cv, float acc=0.01f )
{
	return abs( f-cv )<acc;
}

CollectDataProcess* CollectDataProcess::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new CollectDataProcess();

	return _pSingleton;
}

void CollectDataProcess::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}

CollectDataProcess::CollectDataProcess() 
{
	_saveLightFlag = false;
	_saveLightPort = 1;

	_laser.Open();

	_inspectOpmAddr = 0;
}

bool CollectDataProcess::SetTemPointsList( VECTOR_FLOAT& temPointsList )
{
	_tempPointList.clear();
	_tempPointList = temPointsList;
	return true;
}

void CollectDataProcess::WaitingTempStable()
{
	VECTOR_FLOAT tempData[10];

	while( 1 )
	{
_FLAG:
		int& waitTime = GetWaitTempStableTime();
		Sleep( waitTime );
		UINT boardNum = _childThreadDataList.size();
		for( UINT i=0; i<boardNum; ++i )
		{
			float tem;
			while( GetTemperatureSyn_Net( _childThreadDataList[i]._ipAddr, &tem )==false ) { Sleep( 1000 ); }
			if( tempData[i].size()!=0 && abs( tem-tempData[i][0] )>0.11 )
				tempData[i].clear();
			tempData[i].push_back( tem );

			string str;
			STRING_FORMAT( str, "��·��%s��ǰ�¶�Ϊ%0.1f", IP4_ULONGToStr()( _childThreadDataList[i]._ipAddr ), tem );
			(*PCB_View_ShowCollectMsg)( _childThreadDataList[i]._ipAddr, str.c_str() );
		}

		// 
		for( UINT i=0; i<boardNum; ++i )
		{
			if( tempData[i].size()<8 )
				goto _FLAG;
		}
		(*PCB_View_ShowMsg)( "���е�·���¶��ȶ�.." );

		// ����
		for( UINT i=0; i<boardNum; ++i )
			_childThreadDataList[i]._stableTemp = tempData[i].back();
		return;
	}
}

bool CollectDataProcess::IsTemThreadRunning()
{
	return SearchThreadPos( "CollectAllTempDataThread" )==-1 ? false:true;
}

bool CollectDataProcess::StartCollectAllTempDataThread()
{
	return StartThread( "CollectAllTempDataThread", &CollectDataProcess::CollectAllTempDataThread, NULL );
}

void CollectDataProcess::CollectAllTempDataThread( void* pData )
{
	// ����������̵߳��¶�����
	CleraAllChildThreadsTemData();

	TBMC711 temBox;
	for( UINT t=0; t<_tempPointList.size(); ++t )
	{
		// ����ѭ�����¶�
		string str;
		STRING_FORMAT( str, "%d��:������ѭ���¶ȵ�%0.1f..", t+1, _tempPointList[t] );
		(*PCB_View_ShowMsg)( str.c_str() );
		while( temBox.SetSetTem( _tempPointList[t] )==false ) { Sleep( 1000 ); }

		// �ȴ����е�·���ȶ������¶�
		WaitingTempStable();

		// ͨ�����е�·�����ͼ�lsb  ps����·�岻�ȶ� ���ò����Ƶ�����
		NotifyAllChildsWssTypeAndLSB();

		// Ԥ�����¶ȵ����ݿռ�
		for( UINT i=0; i<_childThreadDataList.size(); ++i )
		{
			// 
			int sn;
			while( GetSerialNumberSyn_Net( _childThreadDataList[i]._ipAddr, &sn )==false );

			string filePath, flieName;
			GetExeDirectory( filePath );
			STRING_FORMAT( flieName, "\\wss_0x%X_%0.1f(%0.1f).txt", sn, _tempPointList[t], _childThreadDataList[i]._stableTemp );
			filePath += flieName;
			AddChildThread( _childThreadDataList[i]._ipAddr, t, filePath );
		}

		// ��ʼ���¶ȵ�Ĳɼ�
		StartCollectThreads( t );

		// �ȴ����¶ȵ�ɼ����
		while( IsMainThreadRunning()==true )
			Sleep( 5000 );
	}
}

bool CollectDataProcess::StartCollectThreads( UINT curTempIndex )
{
	// ֹͣ���еĲɼ������߳�
	StopThread( "MainThread" );
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
	{
		string str;
		STRING_FORMAT( str, "ChildThread%d", i );
		StopThread( str.c_str() );
	}
	StopThread( "InspectLaser" );

	// �����������߳�
	StartThread( "MainThread", &CollectDataProcess::MainThread, (void*)( &_mainThreadData ) );

	// ���������߳�
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
	{
		_childThreadDataList[i]._curTempIndex = curTempIndex;

		string str;
		STRING_FORMAT( str, "ChildThread%d", i );
		StartThread( str, &CollectDataProcess::ChildThread, (void*)( &_childThreadDataList[i] ) );
	}

	// ������ع�Դ�߳�
	StartInspectLaserThread();

	return true;
}

bool CollectDataProcess::IsMainThreadRunning()
{
	return SearchThreadPos( "MainThread" )==-1 ? false:true;
}

void CollectDataProcess::SetInspectOPMAddr( ULONG addr )
{
	_inspectOpmAddr = addr;
}

bool CollectDataProcess::StartInspectLaserThread()
{
	if( _inspectOpmAddr!=0 )
		return StartThread( "InspectLaser", &CollectDataProcess::InspectLaserThread, 0 );
	else
		return false;
}

void CollectDataProcess::InspectLaserThread( void* pData )
{
	WSSProCal udpPro;

	while( 1 )
	{
		float pm;
		int count = 0;
		while( udpPro.GetChannelPortPMSyn( _inspectOpmAddr, 1, pm )==false )
		{
			++count;
			if( count>3 )
			{
				string str;
				STRING_FORMAT( str, "�����%sʧЧ..%s..", IP4_ULONGToStr()( _inspectOpmAddr ), udpPro._debugMsg.c_str() );
				(*PCB_View_ShowMsg) ( str.c_str() );
				Sleep( 1000 );
			}
		}
		
		// ͨ��
		if( pm==-70 )
		{
			for( UINT i=0; i<_childThreadDataList.size(); ++i )
				SetInsepectLaserFlag( _childThreadDataList[i]._ipAddr, false );
				//while( udpPro.NotifyLaserDying( _childThreadDataList[i]._ipAddr )==false ) Sleep( 100 );
		}

		Sleep( 1000 );
	}
}

bool CollectDataProcess::SetInsepectLaserFlag( ULONG addr, bool flag )
{
	int index = SearchChildThread( addr );
	if( index==-1 )
		return false;

	_childThreadDataList[index]._isLaserOK = flag;
	return true;
}

bool CollectDataProcess::GetInsepectLaserFlag( ULONG addr, bool& flag )
{
	int index = SearchChildThread( addr );
	if( index==-1 )
		return false;

	flag = _childThreadDataList[index]._isLaserOK;
	return true;
}

int CollectDataProcess::SearchChildThread( ULONG ipAddress )
{
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
	{
		if( _childThreadDataList[i]._ipAddr==ipAddress )
			return i;
	}

	return -1;
}

bool CollectDataProcess::AddChildThread( ULONG ipAddress )
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	int index = SearchChildThread( ipAddress );
	if( index==-1 )
		_childThreadDataList.push_back( CollectChildThreadData( ipAddress ) );

	return true;
}

bool CollectDataProcess::AddChildThread( ULONG ipAddress, VECTOR_INT& channelIndexList )
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	int index = SearchChildThread( ipAddress );
	if( index==-1 )
		_childThreadDataList.push_back( CollectChildThreadData( ipAddress, channelIndexList ) );
	else
		_childThreadDataList[index]._channelIndexList = channelIndexList;

	return true;
}

bool CollectDataProcess::AddChildThread( ULONG ipAddress, UCHAR tempPoint, string filePath )
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	int index = SearchChildThread( ipAddress );
	if( index==-1 )
	{
		_childThreadDataList.push_back( CollectChildThreadData( ipAddress ) );
		_childThreadDataList.back()._tempDataList.push_back( Temp_Data( tempPoint, filePath ) );
	}
	else
	{
		_childThreadDataList[index].AddTempData( tempPoint, filePath );
	}

	return true;
}

bool CollectDataProcess::AddChildThread( ULONG ipAddress, ULONG oswIP )
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	int index = SearchChildThread( ipAddress );
	if( index==-1 )
		return false;
	
	_childThreadDataList[index]._oswIP = oswIP;
	return true;
}

bool CollectDataProcess::DelChildThread( ULONG ipAddress )
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	int index = SearchChildThread( ipAddress );
	if( index!=-1 )
	{
		_childThreadDataList.erase( _childThreadDataList.begin()+index );
	}

	return true;	 
}

void CollectDataProcess::ClearAllThreads()
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	StopAllThreads();
	_childThreadDataList.clear();
}

void CollectDataProcess::CleraAllChildThreadsTemData()
{
	cnCriticalSectionAuto csAuto( _childThreadListCS );

	for( UINT i=0; i<_childThreadDataList.size(); ++i )
		_childThreadDataList[i].ClearTemData();
}  

bool CollectDataProcess::GetChildThreadCollectData( ULONG ipAddress, UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList )
{
	int index = SearchChildThread( ipAddress );
	if( index==-1 )
		return false;

	if( curTempIndex>=_childThreadDataList[index]._tempDataList.size() )
		return false;

	if( channel<1 || channel>CHANNEL_NUM )
		return false;

	Temp_Data& tempData = _childThreadDataList[index]._tempDataList[curTempIndex];
	if( WSS_TYPE==WSSType::TYPE_21 || WSS_TYPE==WSSType::TYPE_12 )
	{
		if( port!=1 && port!=2 )
			return false;

		Channel_Data& channelData = tempData._dataChannelList[channel-1];
		pXList = &(channelData._curveXPortList[port-1]);
		pYList = &(channelData._curveYPortList[port-1]);
	}
	else if( WSS_TYPE==WSSType::TYPE_X1 || WSS_TYPE==WSSType::TYPE_1X )
	{
		Channel_Data& channelData = tempData._dataChannelList[channel-1];
		pXList = &(channelData._curveXPortList[port-1]);
		pYList = &(channelData._curveYPortList[port-1]);
	}

	return true;
}

void CollectDataProcess::SetLaserWaveLen( int channelIndex )
{							
	while( !_laser.Open() )									// �򿪼�����	
	{
		(*PCB_View_ShowMsg)( "��������ʼ��ʧ�� ���³���" );
		Sleep( 3000 );
	}

	string& waveLen = WSS_ITULIST[channelIndex-1];			// ��õ�ǰҪ���õĲ���

	string str;
	STRING_FORMAT( str, "ͨ��%d ���ù�Դ����Ϊ%snm..", channelIndex, waveLen.c_str() );
	(*PCB_View_ShowMsg)( str.c_str() );

	while( _laser.SetWavelength( waveLen.c_str() )==false ) 
	{ 
		(*PCB_View_ShowMsg)( "���������ò��� ���³���" );
		Sleep( 3000 ); 
	}
	Sleep( 5000 );											// �ȴ��������ȶ�
}

void CollectDataProcess::PollWaveLenList( int param )
{
	VECTOR_INT& channelIndexList = _mainThreadData._channelIndexList;
	for( UINT i=0; i<channelIndexList.size(); ++i )
	{	
		// ���õ�ǰ����
		SetLaserWaveLen( channelIndexList[i] );

		// ���̻߳��ͨ������0
		_mainThreadData.SetNotifyCount( 0 );
	
		// ͨ������߳̽�����Ӧ�Ĳɼ�����
		for( UINT j=0; j<_childThreadDataList.size(); ++j )
			_childThreadDataList[j].SetNotifyInfo( channelIndexList[i], param );

		// �ȴ������߳����
		DWORD beginTick = ::GetTickCount(); 
		while( _mainThreadData._notifyCount<_childThreadDataList.size() )
		{
			Sleep( 1000 );

			// ��ʱ
			DWORD dxTick = ::GetTickCount()-beginTick;
			if( dxTick>5.5*60*1000 )
			{
				string str;
				STRING_FORMAT( str, "����5.5���ӻ�����߳�ͨ����%d//%d..�������ʧ��..", _mainThreadData._notifyCount, _childThreadDataList.size() );
				(*PCB_View_ShowMsg) ( str.c_str() );
			}
		}
	}
}

void CollectDataProcess::MainThread( void* pData )
{
	CollectMainThreadData& mainThreadData = *( (CollectMainThreadData*)pData );

	// ��ȡ�������߳�Ҫ��ѭ�Ĳ�����
	VECTOR_INT& channelIndexList = mainThreadData._channelIndexList;
	channelIndexList.clear();
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
		channelIndexList.insert( channelIndexList.end(), _childThreadDataList[i]._channelIndexList.begin(), _childThreadDataList[i]._channelIndexList.end() ) ;
	// Ψһ��
	sort( channelIndexList.begin(), channelIndexList.end() );
	channelIndexList.erase( unique( channelIndexList.begin(), channelIndexList.end() ), channelIndexList.end() );

	// ���������
	if( _saveLightFlag==true )
	{	// ���
		if( _saveLightPort!=-1 )
			MainSaveLightPort( _saveLightPort );
		else
			MainSaveLightAllPorts();
	}
	else
	{	// ���ݲɼ�
		if( WSS_TYPE==WSSType::TYPE_11 )
			MainCollectData11();
		else if( WSS_TYPE==WSSType::TYPE_21 || WSS_TYPE==WSSType::TYPE_X1 )
			MainCollectDataX1();
		else if( WSS_TYPE==WSSType::TYPE_12 || WSS_TYPE==WSSType::TYPE_1X )
			MainCollectData1X();
	}
}

void CollectDataProcess::ChildThread( void* pData )
{
	CollectChildThreadData& childThreadData = *( (CollectChildThreadData*)pData );

	// 
	if( _saveLightFlag==false )
	{
		// ����ϴ�����
		childThreadData._tempDataList[childThreadData._curTempIndex].Clear();	
	}

	while( 1 )
	{
		if( childThreadData._notifyFlag==false )					// δ������߳�ͨ�� �ȴ�
		{
			Sleep( 50 );
			continue;
		}

		string str;
		STRING_FORMAT( str, "���߳�%s������߳�ͨ��..", IP4_ULONGToStr()( childThreadData._ipAddr ) );
		(*PCB_View_ShowMsg) ( str.c_str() );

		childThreadData._notifyFlag=false;							// ���ͨ��
		//
		if( childThreadData.JudgeNotifyWaveLen()==true )			// ��ǰ�������б���
		{	
			// 
			string str;
			STRING_FORMAT( str, "���߳�%s��ʼ�ò������ݲɼ�", IP4_ULONGToStr()( childThreadData._ipAddr ) );
			(*PCB_View_ShowMsg) ( str.c_str() );
			
			// ���������
			if( _saveLightFlag==true )
			{
				ChildSaveLight( childThreadData );
			}
			else
			{	// ���иò����Ĳɼ�����
				if( WSS_TYPE==WSSType::TYPE_11 )
					ChildCollectData11( childThreadData );
				else if( WSS_TYPE==WSSType::TYPE_21 || WSS_TYPE==WSSType::TYPE_X1 )
					ChildCollectDataX1( childThreadData );
				else if( WSS_TYPE==WSSType::TYPE_12 || WSS_TYPE==WSSType::TYPE_1X )
					ChildCollectData1X( childThreadData );
			}
		}	
		//
		_mainThreadData.AddNotifyCount();							// ͨ�����߳���ɴ���	
	}
}

void CollectDataProcess::MainSaveLightAllPorts()
{
	(*PCB_View_ShowMsg)( "���ʼ���嵽port1�ϣ�" );

	for( int p=1; p<=PORT_NUM; ++p )
		MainSaveLightPort( p );

	(*PCB_View_ShowMsg)( "����port�����ɣ�" );
}

void CollectDataProcess::MainSaveLightPort( int port )
{
	string str;
	STRING_FORMAT( str, "�� port%d ���д��ɼ�..", port );
	(*PCB_View_ShowMsg)( str.c_str() );

	// ���� 2*1 N*1���л�ÿ���ӽ��̵�osw
	if( WSS_TYPE==WSSType::TYPE_21 || WSS_TYPE==WSSType::TYPE_X1 )
	{
		STRING_FORMAT( str, "osw�л�����port%d..", port );
		(*PCB_View_ShowMsg)( str.c_str() );
		//
		for( UINT i=0; i<_childThreadDataList.size(); ++i )
		{
			WSSOSW osw( _childThreadDataList[i]._oswIP );
			while( osw.SetSwitch( port )==false )
			{
				(*PCB_View_ShowMsg)( osw._debugMsg.c_str() );
				Sleep( 1000 );
			}
		}
		Sleep( 1000 );
		//
		STRING_FORMAT( str, "��osw�ĳ�port%d���ӵ�OPM..", port );
		::MessageBoxA( NULL, str.c_str(), "��ʾ", 0 );
	}
	else if( WSS_TYPE==WSSType::TYPE_12 || WSS_TYPE==WSSType::TYPE_1X )
	{
		// ��ʾ�ӵ���Ӧ��port
		STRING_FORMAT( str, "���ӵ�OPM�ϵ�port%d..", port );
		::MessageBoxA( NULL, str.c_str(), "��ʾ", 0 );
	}
	else if( WSS_TYPE==WSSType::TYPE_11 )
	{
	}

	// 
	PollWaveLenList( port );

	STRING_FORMAT( str, "port%d������..", port );
	(*PCB_View_ShowMsg)( str.c_str() );
}

void CollectDataProcess::ChildSaveLight( CollectChildThreadData& childThreadData )
{
	ULONG& ipAddr = childThreadData._ipAddr;
	int& c = childThreadData._notifyChannelIndex;
	int& port = childThreadData._notifyParam;

	float portPM;
	WSSProCal udpPro;
	if( WSS_TYPE==WSSType::TYPE_11 )
		while( udpPro.GetChannelPortPMSyn( ipAddr, 1, portPM )==false );
	else if( WSS_TYPE==WSSType::TYPE_21 || WSS_TYPE==WSSType::TYPE_X1 )
		while( udpPro.GetChannelPortPMSyn( ipAddr, 1, portPM )==false );
	else if( WSS_TYPE==WSSType::TYPE_12 || WSS_TYPE==WSSType::TYPE_1X )
		while( udpPro.GetChannelPortPMSyn( ipAddr, port, portPM )==false );
	childThreadData._saveLightList[c-1][port-1] = portPM;

	//
	string str;
	STRING_FORMAT( str, "%0.2f", portPM );
	(*PCB_View_ShowCollectGrid)( ipAddr, c, str.c_str() );

	// �ж��Ƿ�Ϊ���һ��ͨ�� save����ļ�
	if( c==childThreadData._channelIndexList.back() )
		SaveSaveLightData( childThreadData );
}

void CollectDataProcess::SaveSaveLightData( CollectChildThreadData& childThreadData )
{
	TxtOBJ txt;
	string str;
	STRING_FORMAT( str, "WSS�������_%s.txt", IP4_ULONGToStr()( childThreadData._ipAddr ) );
	txt.CreateFile( str );
		
	// ��ͷ��һ�� 
	str = "ͨ��	";
	for( int p=1; p<=PORT_NUM; ++p )
	{
		string str2;
		STRING_FORMAT( str2, "p%d	", p );
		str += str2;
	}
	str += "\r\n";
	txt.WriteToEnd( str );

	// ������
	for( int c=1; c<=CHANNEL_NUM; ++c )
	{
		STRING_FORMAT( str, "%d	", c ); 

		for( int p=1; p<=PORT_NUM; ++p )
		{
			string str2;
			STRING_FORMAT( str2, "%0.2f	", childThreadData._saveLightList[c-1][p-1] );
			str += str2;
		}

		str += "\r\n";
		txt.WriteToEnd( str );
	}
}

void CollectDataProcess::LoadSaveLightData()
{
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
	{
		CollectChildThreadData& childThreadData = _childThreadDataList[i];

		string str;
		STRING_FORMAT( str, "WSS�������_%s.txt", IP4_ULONGToStr()( childThreadData._ipAddr ) );
		TxtOBJ txt;
		if( txt.OpenFile( str )==false )
			continue;

		vector<vector<string> > allLineWordsList;
		txt.ReadAllLineWords( allLineWordsList );

		for( UINT i=1; i<allLineWordsList.size(); ++i )
		{
			vector<string>& line = allLineWordsList[i];
			if( line.size()==1+PORT_NUM )
			{
				int c = atoi( line[0].c_str() );

				for( int p=1; p<=PORT_NUM; ++p )
					childThreadData._saveLightList[c-1][p-1] = (float)atof( line[p].c_str() );
			}
		}
	}
}

void CollectDataProcess::CollecteData_Core( ULONG ip, int c, int p, VECTOR_INT& curveX, VECTOR_FLOAT& curveY )
{
	WSSProCal udpPro;

_FLAG1:
	SetInsepectLaserFlag( ip, true );
	curveX.clear();
	curveY.clear();
	while( udpPro.StartCollectDataSyn( ip, c, p )==false ) 
	{ 
		( *PCB_View_ShowCollectMsg )( ip, "����ͨ�濪ʼ.." );
		Sleep( 100 ); 
	}

	string str;
	int frameLSB = LSB*250;
	int frameCount = LSB_RANGE/frameLSB+( LSB_RANGE%frameLSB==0 ? 0:1 );

	for( int j=0; j<frameCount; ++j )
	{
		int rt;
_FLAG2:
		string rtMsg;
		while( udpPro.GetCollectDataSyn( ip, c, p, j, frameCount, rt, rtMsg, curveX, curveY )==false ) { Sleep( 100 ); }

		if( rt==0 )			// δ��� ���»������
		{	
			( *PCB_View_ShowCollectMsg )( ip, rtMsg.c_str() );
			Sleep( 400 );
			goto _FLAG2;
		}
		else if( rt==2 )	// ���ش��� 
		{	
			( *PCB_View_ShowCollectMsg )( ip, rtMsg.c_str() );
			Sleep( 800 );
			goto _FLAG1;	
		}
		else if( rt==1 )	// ��������
		{
			( *PCB_View_ShowCollectMsg )( ip, rtMsg.c_str() );
		}

		bool flag;
		GetInsepectLaserFlag( ip, flag );
		if( !flag )
		{
			( *PCB_View_ShowCollectMsg )( ip, "������ʧЧ�����¿�ʼ.." );
			Sleep( 800 );
			goto _FLAG1;	
		}
	}

	int lsbNum = LSB_RANGE/LSB + ( LSB_RANGE%LSB==0 ? 0:1 );		// ������ʵ��ѹ����
	curveX.erase( curveX.begin()+lsbNum, curveX.end() );
	curveY.erase( curveY.begin()+lsbNum, curveY.end() );
}

void CollectDataProcess::NotifyAllChildsWssTypeAndLSB()
{
	string str;
	STRING_FORMAT( str, "ͨ�����еĵ�·��wss����Ϊ %d*%d..", INPORT_NUM, OUTPORT_NUM );
	(*PCB_View_ShowMsg)( str.c_str() );

	WSSProCal proUDP;
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
		while( proUDP.NotifyWssType( _childThreadDataList[i]._ipAddr, INPORT_NUM, OUTPORT_NUM )==false );

	STRING_FORMAT( str, "ͨ�����еĵ�·��lsbΪ %d..", LSB );
	(*PCB_View_ShowMsg)( str.c_str() );

	for( UINT i=0; i<_childThreadDataList.size(); ++i )
		while( proUDP.SetLSB( _childThreadDataList[i]._ipAddr, LSB )==false );
}

void CollectDataProcess::MainCollectData11()
{
	// ����ͨ�����ݲɼ�
	(*PCB_View_ShowMsg)( "��ѡͨ�����вɼ�.." );
	PollWaveLenList( 1 );

	// �����ݵ��ı���
	SaveFileCollectX();
	(*PCB_View_ShowMsg)( "���ݲɼ����.." );
}

void CollectDataProcess::MainCollectDataX1()
{
	for( int p=1; p<=INPORT_NUM; ++p )
	{
		string str;
		STRING_FORMAT( str, "�л���port%d..", p );
		(*PCB_View_ShowMsg)( str.c_str() );

		// �л�ÿ���ӽ��̶�Ӧ��osw
		for( UINT i=0; i<_childThreadDataList.size(); ++i )
		{
			WSSOSW osw( _childThreadDataList[i]._oswIP );
			while( osw.SetSwitch( p )==false )
			{
				(*PCB_View_ShowMsg)( osw._debugMsg.c_str() );
				Sleep( 1000 );
			}
		}
		Sleep( 3000 );

		STRING_FORMAT( str, "��ѡͨ������port%d�ɼ�..", p );
		(*PCB_View_ShowMsg)( str.c_str() );
		PollWaveLenList( p );
	}

	// �����ݵ��ı���
	SaveFileCollectX();
	(*PCB_View_ShowMsg)( "���ݲɼ����.." );
}

void CollectDataProcess::MainCollectData1X()
{
	// ����ͨ�����ݲɼ�
	(*PCB_View_ShowMsg)( "��ѡͨ�����вɼ�.." );
	PollWaveLenList( 0 );

	// �����ݵ��ı���
	SaveFileCollectX();
	(*PCB_View_ShowMsg)( "���ݲɼ����.." );
}

void CollectDataProcess::ChildCollectData11( CollectChildThreadData& childThreadData )
{
	ULONG& ipAddr = childThreadData._ipAddr;
	int& c = childThreadData._notifyChannelIndex;
	Temp_Data& tempData = childThreadData._tempDataList[childThreadData._curTempIndex];
	Channel_Data& channelData = tempData._dataChannelList[c-1];

	VECTOR_INT& curveX = channelData._curveXPortList[0];
	VECTOR_FLOAT& curveY = channelData._curveYPortList[0];
	//
	CollecteData_Core( ipAddr, c, 1, curveX, curveY );
	(*PCB_View_ShowCollectGrid)( ipAddr, c, "port1�������.." );
	// �������
	for( UINT j=0; j<curveY.size(); ++j )
		curveY[j] -=  childThreadData._saveLightList[c-1][0];

	// ��ͼ
	string str;
	STRING_FORMAT( str, "channel%d_port1", c );
	(*PCB_View_ClearCollectChart)( ipAddr );
	(*PCB_View_AddCollectCurve)( ipAddr, &curveX[0], &curveY[0], curveX.size(), str.c_str(), RGB(0,0,255), true );
}

void CollectDataProcess::ChildCollectDataX1( CollectChildThreadData& childThreadData )
{
	ULONG& ipAddr = childThreadData._ipAddr;
	int& c = childThreadData._notifyChannelIndex;
	Temp_Data& tempData = childThreadData._tempDataList[childThreadData._curTempIndex];
	Channel_Data& channelData = tempData._dataChannelList[c-1];

	string str;
	int port = childThreadData._notifyParam;
	VECTOR_INT& curveX = channelData._curveXPortList[port-1];
	VECTOR_FLOAT& curveY = channelData._curveYPortList[port-1];
	
	// �ɼ�
	CollecteData_Core( ipAddr, c, port, curveX, curveY );
	STRING_FORMAT( str, "port%d�������..", port );
	(*PCB_View_ShowCollectGrid)( ipAddr, c, str.c_str() );
	// �������
	for( UINT j=0; j<curveY.size(); ++j )
		curveY[j] -=  childThreadData._saveLightList[c-1][port-1];
	
	// ��ͼ
	(*PCB_View_ClearCollectChart)( ipAddr );
	for( int p=1; p<=port; ++p )
	{
		VECTOR_INT& curveXP = channelData._curveXPortList[p-1];
		VECTOR_FLOAT& curveYP = channelData._curveYPortList[p-1];

		STRING_FORMAT( str, "c%d_p%d", c, p );
		(*PCB_View_AddCollectCurve)( ipAddr, &curveXP[0], &curveYP[0], curveXP.size(), str.c_str(), p%2==0?RGB(255,0,0):RGB(0,0,255), true );
	}
}

void CollectDataProcess::ChildCollectData1X( CollectChildThreadData& childThreadData )
{
	ULONG& ipAddr = childThreadData._ipAddr;
	int& c = childThreadData._notifyChannelIndex;
	Temp_Data& tempData = childThreadData._tempDataList[childThreadData._curTempIndex];
	Channel_Data& channelData = tempData._dataChannelList[c-1];

	string str;
	(*PCB_View_ClearCollectChart)( ipAddr );

	for( int p=1; p<=OUTPORT_NUM; ++p )
	{
		VECTOR_INT& curveX = channelData._curveXPortList[p-1];
		VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];

		// port x�ɼ�
		CollecteData_Core( ipAddr, c, p, curveX, curveY );
		STRING_FORMAT( str, "port%d�������..", p );
		(*PCB_View_ShowCollectGrid)( ipAddr, c, str.c_str() );
		// �������
		for( UINT j=0; j<curveY.size(); ++j )
			curveY[j] -=  childThreadData._saveLightList[c-1][p-1];

		// ��ͼ
		STRING_FORMAT( str, "c%d_p%d", c, p );
		(*PCB_View_AddCollectCurve)( ipAddr, &curveX[0], &curveY[0], curveX.size(), str.c_str(), p%2==0?RGB(255,0,0):RGB(0,0,255), true );
	}
}

void CollectDataProcess::SaveFileCollectX()
{
	for( UINT i=0; i<_childThreadDataList.size(); ++i )
	{
		CollectChildThreadData& childThreadData = _childThreadDataList[i];
		Temp_Data& tempData = childThreadData._tempDataList[childThreadData._curTempIndex];
		VECTOR_INT& channelIndexList = childThreadData._channelIndexList;

		// �����ı�
		TxtOBJ txt;
		txt.CreateFile( tempData._filePath );

		string str;
		STRING_FORMAT( str, "�¶ȵ�%d; ����ͨ������..\r\n", tempData._tempPoint );
		txt.WriteToEnd( str );

		for( UINT j=0; j<channelIndexList.size(); ++j )
		{
			int& c = channelIndexList[j];
			Channel_Data& channelData = tempData._dataChannelList[c-1];

			STRING_FORMAT( str, "ͨ�� %d �ɼ�����: \r\n", c );
			txt.WriteToEnd( str );

			// ������
			str = "vol";
			for( int p=1; p<=PORT_NUM; ++p )
			{
				string tempStr;
				STRING_FORMAT( tempStr, "	port%d", p );
				str += tempStr;
			}
			str += "\r\n";
			txt.WriteToEnd( str );

			// ����
			VECTOR_INT& curveX = channelData._curveXPortList[0];
			for( UINT i=0; i<curveX.size(); ++i )
			{
				STRING_FORMAT( str, "%d", curveX[i] );

				for( int p=1; p<=PORT_NUM; ++p )
				{
					VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];
				
					string tempStr;
					STRING_FORMAT( tempStr, "	%0.2f", curveY[i] );
					str += tempStr;
				}

				str += "\r\n";
				txt.WriteToEnd( str );
			}
		}
	}
}

bool CollectDataProcess::StartSaveLightProcess( int port )
{
	if( IsMainThreadRunning()==true )
	{
		::MessageBoxA( 0, "����������ִ��..��ȴ�..", "�澯", 0 );
		return false;
	}

	_saveLightFlag = true;
	_saveLightPort = port;

	return StartCollectThreads();
}

bool CollectDataProcess::StartCollectAllTemPointsProcess()
{
	if( IsTemThreadRunning()==true )
	{
		::MessageBoxA( 0, "����������ִ��..��ȴ�..", "�澯", 0 );
		return false;
	}

	_saveLightFlag = false;
	LoadSaveLightData();			// ����������

	return StartCollectAllTempDataThread();
}






ParseDataProcess* ParseDataProcess::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new ParseDataProcess();

	return _pSingleton;
}

void ParseDataProcess::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}

ParseDataProcess::ParseDataProcess() 
{
}

void ParseDataProcess::LoadCollectData( ULONG ipAddr, VECTOR_STRING fileList, UCHAR tempList[6] )
{
	_ipAddr = ipAddr;

	_tempDataList.clear();
	for( UINT i=0; i<fileList.size(); ++i )
		_tempDataList.push_back( Temp_Data( tempList[i], fileList[i] ) );

	StartThread( "LoadThread", &ParseDataProcess::LoadCollectDataThread );
}

void ParseDataProcess::LoadCollectDataThread()
{
	LoadCollectDataX();	
}

void ParseDataProcess::LoadCollectDataX()
{
	string str;
	for( UINT k=0; k<_tempDataList.size(); ++k )
	{
		Temp_Data& tempData = _tempDataList[k];

		string str;
		STRING_FORMAT( str, "��ȡ�ļ� %s..", tempData._filePath.c_str() );	
		(*PCB_View_ShowMsg)( str.c_str() );
		
		// ��ʼ��ͨ����־
		bool* channelUseFlag = tempData._channelUseFlag;
		for( int i=0; i<CHANNEL_NUM; ++i )
			channelUseFlag[i] = false;

		// ��ȡ�ļ�
		vector<vector<string> > allLineWordsList;
		{
			TxtOBJ txt;
			txt.OpenFile( tempData._filePath );
			txt.ReadAllLineWords( allLineWordsList );
		}
		int lsbNum = LSB_RANGE/LSB + ( LSB_RANGE%LSB==0 ? 0:1 );		// �����ѹ����
		int channelNum = allLineWordsList.size()/lsbNum;				// ����ͨ������

		int lineIndex = 1;												// �ӹ���1��
		for( int c=0; c<channelNum; ++c )
		{
			// ��ȡ��ǰͨ��index
			int curChannel = atoi( allLineWordsList[lineIndex++][1].c_str() );
			channelUseFlag[curChannel-1] = true;
			Channel_Data& channelData = tempData._dataChannelList[curChannel-1];
			channelData.ClearCalc();

			// ����ÿ��port����
			(*PCB_View_ClearParseChart)();
			++lineIndex;												// �ӹ������� vol p1 ���� p9

			// ��������
			for( int i=0; i<lsbNum; ++i )
			{
				vector<string>& line = allLineWordsList[lineIndex++];

				for( int p=1; p<=PORT_NUM; ++p )
				{				
					VECTOR_INT& curveX = channelData._curveXPortList[p-1];
					VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];

					curveX.push_back( atoi( line[0].c_str() ) );
					curveY.push_back( (float)atof( line[p].c_str() ) );
				}
			}
			
			// ������
			for( int p=1; p<=PORT_NUM; ++p )
			{				
				VECTOR_INT& curveX = channelData._curveXPortList[p-1];
				VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];

				STRING_FORMAT( str, "c%d_p%d", curChannel, p );	
				(*PCB_View_AddParseCurve)( &curveX[0], &curveY[0], curveX.size(), str.c_str(), p%2==0?RGB(0,255,0):RGB(255,0,0), true );
			}
			(*PCB_View_ShowParseGrid)( curChannel, "��������ɹ�.." );
		}

		(*PCB_View_SetParseGridChannel)( channelUseFlag );
	}

	(*PCB_View_ShowMsg)( "�����ļ�����ɹ�.." );
}

void ParseDataProcess::SetParseChannels( UINT curTempIndex, bool* pChannelUseFlag, int num )
{
	_curTempIndex = curTempIndex;
	_channelUseFlagCache.clear();
	for( int i=0; i<num; ++i )
		_channelUseFlagCache.push_back( pChannelUseFlag[i] );
	StartThread( "SetParseChannelsThread", &ParseDataProcess::SetParseChannelsThread );
}

void ParseDataProcess::SetParseChannelsThread()
{
	if( _curTempIndex!=-1 )
	{
		Temp_Data& tempData = _tempDataList[_curTempIndex];
		for( UINT i=0; i<_channelUseFlagCache.size(); ++i )
			tempData._channelUseFlag[i] = _channelUseFlagCache[i];
	}
	else
	{
		for( UINT j=0; j<_tempDataList.size(); ++j )
		{
			Temp_Data& tempData = _tempDataList[j];
			for( UINT i=0; i<_channelUseFlagCache.size(); ++i )
				tempData._channelUseFlag[i] = _channelUseFlagCache[i];
		}
	}

	(*PCB_View_ShowMsg)( "Ҫ�����Ĳ����б��������.." );
}

void ParseDataProcess::ParseCollectData()
{
	// ��ʼ��txt����
	TxtOBJ txt;
	txt.CreateFile( "wss�������ݷ���.txt" );

	StartThread( "ParseThread", &ParseDataProcess::ParseCollectDataThread );
}

void ParseDataProcess::ParseCollectDataThread()
{
	ParseCollectDataX();
}

void ParseDataProcess::ParseCollectDataX()
{
	TxtOBJ txt;
	txt.OpenFile( "wss�������ݷ���.txt" );

	// �����׼��ֵ
	(*PCB_View_ShowMsg)( "������ͷ�ֵ.." );
	SearchAllPortPeakX();
	string str;
	STRING_FORMAT( str, "��Ѱ��ͷ�ֵlowestPeak: %f; \r\n", _lowestPeak );	
	txt.WriteToEnd( str );

	for( UINT k=0; k<_tempDataList.size(); ++k )
	{
		Temp_Data& tempData = _tempDataList[k];

		STRING_FORMAT( str, "�����ļ� %s..\r\n", _tempDataList[k]._filePath.c_str() );
		(*PCB_View_ShowMsg)( str.c_str() );
		txt.WriteToEnd( str );
		
		_curTempIndex = k;
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			if( tempData._channelUseFlag[c-1]==false )
				continue;

			string str;
			STRING_FORMAT( str, "����ͨ��%d����..", c );
			txt.WriteToEnd( str );
			(*PCB_View_ShowMsg)( str.c_str() );

			ParseCollectChannelDataX( c, txt );
		}
	}

	(*PCB_View_ShowMsg)( "�����ļ��������.." );
}

void ParseDataProcess::SearchAllPortPeakX()
{
	string str;
	TxtOBJ txt;
	txt.OpenFile( "wss�������ݷ���.txt" );
	txt.WriteToEnd( "��ֵ����..\r\n" );

	// ��ÿ���¶ȶ�Ӧ���ļ������д���
	vector<float> portPeakList;
	for( UINT k=0; k<_tempDataList.size(); ++k )
	{
		Temp_Data& tempData = _tempDataList[k];

		STRING_FORMAT( str, "�¶ȵ�%d:\r\n", tempData._tempPoint );
		txt.WriteToEnd( str );

		for( int c=1; c<=CHANNEL_NUM; ++c )
		{	// ��ÿ��ͨ�������д���
			// ��������ͨ�� �ӹ�
			if( tempData._channelUseFlag[c-1]==false )
				continue;

			STRING_FORMAT( str, "ͨ��%d:	", c );
			txt.WriteToEnd( str );

			// ��ǰͨ������
			Channel_Data& channelData = tempData._dataChannelList[c-1];

			for( int p=1; p<=PORT_NUM; ++p )
			{
				VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];
				channelData._portPeakIndexList[p-1] = SearchPeak<float>( curveY, 0, false, 4000/LSB, -1, -30.f, true );
				portPeakList.push_back( curveY[ channelData._portPeakIndexList[p-1] ] );

				STRING_FORMAT( str, "%0.2f	", portPeakList.back() );
				txt.WriteToEnd( str );
			}
			txt.WriteToEnd( "\r\n" );
		}
	}

	// �����¶ȵ���Ѱ����С�ķ�ֵ��Ϊ����ͨ���ı�׼��ֵ
	_lowestPeak = *min_element( portPeakList.begin(), portPeakList.end() );
}

void ParseDataProcess::ParseCollectChannelDataX( int c, TxtOBJ& txt )
{
	string str;
	float down25dbLimit = _lowestPeak-26;
	Channel_Data& channelData = _tempDataList[_curTempIndex]._dataChannelList[c-1];

	bool portFlag[MAX_PORT_NUM];
	for( int p=0; p<MAX_PORT_NUM; ++p )
		portFlag[p] = true;

	for( int p=1; p<=PORT_NUM; ++p )
	{
		bool& isOddRight = IsOddRight();
		bool rightFlag = (p%2==1) ? isOddRight:!isOddRight;

		VECTOR_INT& curveX = channelData._curveXPortList[p-1];
		VECTOR_FLOAT& curveY = channelData._curveYPortList[p-1];

		// �ӷ�ֵ������25db��
		int peakIndex = channelData._portPeakIndexList[p-1];
		int downIndex = peakIndex;
		if( rightFlag )
			while( curveY[++downIndex]>down25dbLimit );
		else
			while( curveY[--downIndex]>down25dbLimit );

		// �������
		vector<int>& newCurveX = channelData._curveXCalcPortList[p-1];
		vector<float>& newCurveY = channelData._curveYCalcPortList[p-1];
		vector<int> tempCurveX;
		vector<float> tempCurveY;
		//
		if( rightFlag )
		{
			tempCurveX.assign( curveX.begin()+peakIndex, curveX.begin()+downIndex );
			tempCurveY.assign( curveY.begin()+peakIndex, curveY.begin()+downIndex );
		}
		else
		{
			tempCurveX.assign( curveX.begin()+downIndex, curveX.begin()+peakIndex );
			tempCurveY.assign( curveY.begin()+downIndex, curveY.begin()+peakIndex );
		}
		if( SortFitting_Curve<int, float>( tempCurveX, tempCurveY, newCurveX, newCurveY, _lowestPeak, 0.1f, 25.f, !rightFlag )==false )
			portFlag[p-1] = false;
	}

	string str1, str2;
	for( int p=1; p<=PORT_NUM; ++p )
	{
		STRING_FORMAT( str, "%d,", p );
		if( portFlag[p-1]==false )
			str1 += str;
		else
			str2 += str;
	}
	if( str1.size()>0 )
	{
		str1 = string( str1.begin(), str1.end()-1 );
		str1 += "ʧ��;";
	}
	if( str2.size()>0 )
	{
		str2 = string( str2.begin(), str2.end()-1 );
		str2 += "�ɹ�;";
	}
	//
	str = str1+str2;
	(*PCB_View_ShowParseGrid)( c, str.c_str() );
	txt.WriteToEnd( str+"\r\n" );

	(*PCB_View_ClearParseChart)();
	for( int p=1; p<=PORT_NUM; ++p )
	{
		vector<int>& newCurveX = channelData._curveXCalcPortList[p-1];
		vector<float>& newCurveY = channelData._curveYCalcPortList[p-1];

		STRING_FORMAT( str, "c%d_p%d", c, p );
		(*PCB_View_AddParseCurve)( &newCurveX[0], &newCurveY[0], newCurveX.size(), str.c_str(), (p%2==1) ? RGB(0,0,0):RGB(255,0,0), true );
	}
}

void ParseDataProcess::SendParseData( VECTOR_INT& yVolList )
{
	//
	_yVolList = yVolList;

	StartThread( "SendThread", &ParseDataProcess::SendParseDataThread );
}

void ParseDataProcess::SendParseDataThread()
{
	SendParseDataX();
}

void ParseDataProcess::SendParseDataX()
{
	for( UINT k=0; k<_tempDataList.size(); ++k )
	{
		_curTempIndex = k;
		Temp_Data& tempData = _tempDataList[k];

		string str;
		STRING_FORMAT( str, "�������� ��Ӧ�ļ�%s..", _tempDataList[k]._filePath.c_str() );
		(*PCB_View_ShowMsg)( str.c_str() );
		
		for( int c=1; c<=CHANNEL_NUM; ++c )
		{
			if( tempData._channelUseFlag[c-1]==false )
				continue;

			STRING_FORMAT( str, "����ͨ��%d����", c );
			(*PCB_View_ShowMsg)( str.c_str() );

			if( PORT_NUM==1 )
				SendParseChannelData1( c );
			else if( PORT_NUM==2 )
				SendParseChannelData2( c );
			else if( PORT_NUM>2 )
				SendParseChannelDataX( c );
		}
	}

	(*PCB_View_ShowMsg)( "���ݷ������.." );
}

void ParseDataProcess::SendParseChannelData1( int c )
{
	Channel_Data& channelData = _tempDataList[_curTempIndex]._dataChannelList[c-1];

	// ����block�� 
	//int block = SearchBlock1( channelData._curveXPortList[0], channelData._curveYPortList[0] );
	int block = 0;
	//if( c%2==0 )  // żͨ��
	//	block = 8500;
	//else
	//	block = 7000;
	////
	//if( c==3 )
	//	block = 5600;
	//if( c==9 )
	//	block = 6000;

	WSSProCal proUDP;

	VECTOR_INT curveX = channelData._curveXCalcPortList[0];
	if( curveX.size()!=250 ) return;

	while( proUDP.SendCalChannelPortDataSyn( _ipAddr, _tempDataList[_curTempIndex]._tempPoint, c, 1, curveX, 0, block )==false );
	(*PCB_View_ShowParseGrid)( c, "���ݷ��ͳɹ�.." );
}

void ParseDataProcess::SendParseChannelData2( int c )
{
	Channel_Data& channelData = _tempDataList[_curTempIndex]._dataChannelList[c-1];

	// ����block�� 
	int block = SearchBlock2( channelData._curveXPortList[0], channelData._curveYPortList[0], 
							  channelData._curveXPortList[1], channelData._curveYPortList[1] );

	WSSProCal proUDP;
	{	// port1
		VECTOR_INT curveX = channelData._curveXCalcPortList[0];
		if( curveX.size()!=250 )
			return;

		while( proUDP.SendCalChannelPortDataSyn( _ipAddr, _tempDataList[_curTempIndex]._tempPoint, c, 1, curveX, 0, block )==false );
	}

	{	// port2
		VECTOR_INT curveX = channelData._curveXCalcPortList[1];
		if( curveX.size()!=250 )
			return;
		reverse( curveX.begin(), curveX.end() );

		while( proUDP.SendCalChannelPortDataSyn( _ipAddr, _tempDataList[_curTempIndex]._tempPoint, c, 2, curveX, 0, block )==false );
	}

	(*PCB_View_ShowParseGrid)( c, "���ݷ��ͳɹ�.." );
}

void ParseDataProcess::SendParseChannelDataX( int c )
{
	Channel_Data& channelData = _tempDataList[_curTempIndex]._dataChannelList[c-1];

	// ����block�� 
	//int block = SearchBlockX( channelData._curveXPortList[PORT_NUM-1], channelData._curveYPortList[PORT_NUM-1] );
	int block = 0;

	WSSProCal proUDP;
	for( int p=1; p<=PORT_NUM; ++p )
	{
		VECTOR_INT curveX = channelData._curveXCalcPortList[p-1];
		if( curveX.size()!=250 )
			return;

		bool& isOddRight = IsOddRight();
		bool rightFlag = (p%2==1) ? isOddRight:!isOddRight;
		if( !rightFlag )
			reverse( curveX.begin(), curveX.end() );

		while( proUDP.SendCalChannelPortDataSyn( _ipAddr, _tempDataList[_curTempIndex]._tempPoint, c, p, curveX, _yVolList[c-1], block )==false );
	}

	(*PCB_View_ShowParseGrid)( c, "���ݷ��ͳɹ�.." );
}

int ParseDataProcess::SearchBlock1( VECTOR_INT& curveX1, VECTOR_FLOAT& curveY1 )
{
	int peakIndex = SearchPeak<float>( curveY1, 0, false, 1000, -1, -30.f, true );

	float yLimit = curveY1[peakIndex]-45;

	while( curveY1[++peakIndex]>yLimit );
	return curveX1[peakIndex];
}

int ParseDataProcess::SearchBlock2( VECTOR_INT& curveX1, VECTOR_FLOAT& curveY1, VECTOR_INT& curveX2, VECTOR_FLOAT& curveY2 )
{
	int peakIndex1 = SearchPeak<float>( curveY1, 0, false, 1000, -1, -30.f, true );
	int peakIndex2 = SearchPeak<float>( curveY2, 0, false, 1000, -1, -30.f, true );

	VECTOR_FLOAT tempList;
	for( int i=peakIndex1; i<=peakIndex2; ++i )
		tempList.push_back( curveY1[i]>curveY2[i] ? curveY1[i]:curveY2[i] );

	int minIndex = min_element( tempList.begin(), tempList.end() )-tempList.begin();
	minIndex += peakIndex1;

	return curveX1[minIndex];
}   
	
int ParseDataProcess::SearchBlockX( VECTOR_INT& curveX, VECTOR_FLOAT& curveY )
{
	bool& isOddRight = IsOddRight();
	bool rightFlag = (PORT_NUM%2==1) ? isOddRight:!isOddRight;

	int peakIndex = SearchPeak<float>( curveY, 0, false, 1000, -1, -30.f, true );
	float yLimit = curveY[peakIndex]-45;

	if( rightFlag )
		while( curveY[++peakIndex]>yLimit );
	else
		while( curveY[--peakIndex]>yLimit );

	return curveX[peakIndex];
}

bool ParseDataProcess::GetLoadData( UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList )
{
	if( curTempIndex>=_tempDataList.size() )
		return false;

	if( channel<1 || channel>CHANNEL_NUM )
		return false;

	Temp_Data& tempData = _tempDataList[curTempIndex];
	Channel_Data& channelData = tempData._dataChannelList[channel-1];
	pXList = &(channelData._curveXPortList[port-1]);
	pYList = &(channelData._curveYPortList[port-1]);

	return true;
}

bool ParseDataProcess::GetParseData( UINT curTempIndex, int channel, int port, VECTOR_INT*& pXList, VECTOR_FLOAT*& pYList )
{
	if( curTempIndex>=_tempDataList.size() )
		return false;

	if( channel<1 || channel>CHANNEL_NUM )
		return false;

	Temp_Data& tempData = _tempDataList[curTempIndex];
	Channel_Data& channelData = tempData._dataChannelList[channel-1];
	pXList = &(channelData._curveXCalcPortList[port-1]);
	pYList = &(channelData._curveYCalcPortList[port-1]);

	return true;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         