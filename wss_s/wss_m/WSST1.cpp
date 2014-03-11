
#include "stdafx.h"
#include "WSST1.h"
#include "WSSTX.h"

PFunc_View_Type0 PCB_View_ShowMsg;
PFunc_View_Type101 PCB_View_ShowYLCurve;
PFunc_View_Type102 PCB_View_ShowYLGrid;


WSST1* WSST1::_pSingleton = NULL;


WSST1* WSST1::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new WSST1();

	return _pSingleton;
}

void WSST1::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}


WSST1::WSST1()
{
	_isStopYLFlag = false;
	_osa.pCB_ShowMsg = PCB_View_ShowMsg;

	_ylOffset = 0;


	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	str += "\\Data\\ITUGHZ.xls";

	LoadITUGhzList( str.c_str() );
}

WSST1::~WSST1()
{
	DisconnectDevices();
}


void WSST1::ConnectDevices( VECTOR_STRING& msgList )
{
	// ���ӹ�����
	bool rt = _osa.ConnnectTCPIP( _osaIP, _osaPort );
	msgList.push_back( rt?"���������ӳɹ�..":"����������ʧ��.." );

	// �⿪������Ϊcomģʽ
	rt = _osw.Open( _oswCom );
	if( rt )
		_osw.ComModeCall();
	msgList.push_back( rt?"�⿪�����ӳɹ�..":"�⿪������ʧ��.." );
}

void WSST1::DisconnectDevices()
{
	_osa.DisConnect();
	_osw.Close();
}

// portΪҪ����port ��Ϊ-1�������port
void WSST1::StartSaveLightProcess( int port )
{
	// ���Ҫ����port
	_slPortList.clear();
	if( port==-1 )
	{
		for( int p=1; p<=PORT_NUM; ++p )
			_slPortList.push_back( p );
	}
	else
	{
		_slPortList.push_back( port );
	}

	StartSaveLightThread();
}

void WSST1::StartSaveLightThread()
{
	if( _slPortList.size()==0 )
		return;
	
	if( StartThread( &WSST1::SaveLightThread ) )
		PCB_View_ShowMsg( "�����̿�ʼ.." );
	else
		PCB_View_ShowMsg( "������ʧ��.." );
}

void WSST1::SaveLightThread()
{
	string str;
	for( UINT i=0; i<_slPortList.size(); ++i )
	{
		if( WSS_TYPE<20 )
			STRING_FORMAT( str, "�뽫�⿪��port%d�ӵ���Դ", _slPortList[i] );
		else
			STRING_FORMAT( str, "�뽫�⿪��port%d�ӵ�������", _slPortList[i] );
		PCB_View_ShowMsg( str.c_str() );

		STRING_FORMAT( str, "�л��⿪�ص�port%d", _slPortList[i] );
		PCB_View_ShowMsg( str.c_str() );
		_osw.SetChannel( _slPortList[i] );

		PCB_View_ShowMsg( "osa��ȡ����.." );
		_osa.GetTraceA( str );

		PCB_View_ShowMsg( "�������洢�������.." );
		SaveSaveLight( str, _slPortList[i] );
	}

	PCB_View_ShowMsg( "���������.." );
}

void WSST1::SaveSaveLight( string& str, int port )
{
	string fileName;
	STRING_FORMAT( fileName, "T1SL_%d.txt", port );

	SaveSLData( fileName, str );
}

void WSST1::LoadSaveLight( int port )
{
	string fileName;
	STRING_FORMAT( fileName, "T1SL_%d.txt", port );

	LoadSLData( fileName, _slDataList[port-1] );
}

void WSST1::StopYellowLightProcess()
{
	_isStopYLFlag = true;
}

void WSST1::StartYellowLightProcess()
{
	StartYellowLightThread();
}

void WSST1::StartYellowLightThread()
{
	if( StartThread( &WSST1::YellowLightThread ) )
		PCB_View_ShowMsg( "��ʼ��ػ���.." );
	else
		PCB_View_ShowMsg( "��ػ���ʧ��.." );
}

void WSST1::YellowLightThread()
{
	string str;

	// ����������
	PCB_View_ShowMsg( "������.." );
	// ����osw�����ڵ����  
	LoadSaveLight( _oswChannel );

	_isStopYLFlag = false;

	while( 1 )
	{	
_FLAG:
		if( _isStopYLFlag ) 
			break;

		VECTOR_FLOAT xNMList;
		VECTOR_FLOAT vList;
		if( GetOSATrace( xNMList, vList )==false )
			goto _FLAG;

		// ����ת��ΪghzΪ��λ
		VECTOR_FLOAT xList, yList;
		for( auto i=xNMList.rbegin(); i!=xNMList.rend(); ++i )
			xList.push_back( LIGHT_SPEED/(*i) );

		for( auto i=vList.rbegin(); i!=vList.rend(); ++i )
			yList.push_back( *i );

		// ����ָ��
		CalcYLIndicator( xList, yList );

		// ��ָ����ʾ��view
		PCB_View_ShowYLGrid();
	}
}

void WSST1::CalcYLIndicator( VECTOR_FLOAT& xList, VECTOR_FLOAT& yList )
{
	struct AnalyzeData
	{
		string operator()( VarJList<float>& vlist, int index )
		{
			string str;
			if( vlist[index] )
			{
				STRING_FORMAT( str, "%0.2f	", vlist[index]._v );
				return str;
			}
			else
			{
				return "������	";
			}
		}
	};

	// ������ڵ�������Ϣ
	PCB_View_ShowMsg( "�����ֵ������Ϣ.." );
	BaseWaveOBJ<float> waveOBJ;
	waveOBJ.SetWave( xList, yList );
	waveOBJ.SetInflectionHelpPoint( 35 );
	waveOBJ.SetInflectionHelpDX( 2 );
	waveOBJ.SetPeakYLimit( _lowPeakLimit );
	waveOBJ.ParsePeakWaveCycle();

	// ��ι����ӷ壿������������
	//

	// ����Ƶ�� 
	vector<FittingPointData<float> > peak0P5InforList, peak3InforList, peak20InforList;
	waveOBJ.ParseFittingPeakValley( 0.5, &peak0P5InforList );
	waveOBJ.ParseFittingPeakValley( 3, &peak3InforList );
	waveOBJ.ParseFittingPeakValley( 20, &peak20InforList );

	// ����0.5db����Ϸ�ֵ������м����ȱʧ������
	// ����ITUƵ�ʼ����Ӧ��������Ϣindex  ����м�ȱʧ��Ϊ-1
	// ����ԭʼ�����waveOBJ._cycleList�Ѿ����˵��ӷ�
	VECTOR_INT cycList;
	if( waveOBJ._cycleList.size()>1 )
	{
		cycList.push_back( 0 );			// ��¼��һ������

		for( UINT i=1; i<waveOBJ._cycleList.size(); ++i )
		{
			// �����м����ȱʧ������
			float dx = xList[waveOBJ._cycleList[i]._peakIndex] - xList[waveOBJ._cycleList[i-1]._peakIndex];
			float temp = dx/FREQ_GHZ;
			int missNum = (int)temp;
			if( temp-missNum > 0.8 )
				++missNum;
			--missNum;

			//
			for( int j=0; j<missNum; ++j )
				cycList.push_back( -1 );

			cycList.push_back( i );
		}
	}

	

	// ���㲢���ƻ���  ���ߵĺ�������ghzΪ��׼
	VECTOR_FLOAT xYellowList, yYellowList;
	VECTOR_INT& ituGHZList = *(VECTOR_INT*)GetITUGhzList( FREQ_GHZ );
	vector<FittingPointData<float> >& peakInforList = _ylDBFlag==1 ? peak0P5InforList:peak3InforList;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		if( cycList[i]==-1 ) continue;

		xYellowList.push_back( (float)ituGHZList[i+_ylOffset] );
		yYellowList.push_back( peakInforList[cycList[i]]._xFitting-ituGHZList[i+_ylOffset] ); 
	}
	PCB_View_ShowMsg( "����trace������.." );
	PCB_View_ShowYLCurve( xList, yList, xYellowList, yYellowList );

	// ����Ƶ����ITU����
	VarJList<float> center0p5DXList, center3DXList;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		// �����ڵ����ڲ�����
		if( cycList[i]==-1 ) continue; 

		if( peak0P5InforList[cycList[i]]._fittingFlag )
			center0p5DXList.Push_Back( peak0P5InforList[cycList[i]]._xFitting-ituGHZList[i+_ylOffset] );
		else 
			center0p5DXList.Push_NoValue();

		if( peak3InforList[cycList[i]]._fittingFlag )
			center3DXList.Push_Back( peak3InforList[cycList[i]]._xFitting-ituGHZList[i+_ylOffset] );
		else 
			center3DXList.Push_NoValue();
	}

	// ����
	VarJList<float> bw0p5List, bw3List, bw20List;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		if( cycList[i]==-1 ) continue;		// �����ڵ����ڲ�����

		if( peak0P5InforList[cycList[i]]._leftFlag && peak0P5InforList[cycList[i]]._rightFlag )
			bw0p5List.Push_Back( peak0P5InforList[cycList[i]]._rightV-peak0P5InforList[cycList[i]]._leftV );
		else 
			bw0p5List.Push_NoValue();

		if( peak3InforList[cycList[i]]._leftFlag && peak3InforList[cycList[i]]._rightFlag )
			bw3List.Push_Back( peak3InforList[cycList[i]]._rightV-peak3InforList[cycList[i]]._leftV );
		else 
			bw3List.Push_NoValue();

		if( peak20InforList[cycList[i]]._leftFlag && peak20InforList[cycList[i]]._rightFlag )
			bw20List.Push_Back( peak20InforList[cycList[i]]._rightV-peak20InforList[cycList[i]]._leftV );
		else 
			bw20List.Push_NoValue();
	}

	// rippele
	VarJList<float> ripple0p5List, ripple3List;	
	for( UINT i=0; i<cycList.size(); ++i )
	{
		// �����ڵ����ڲ�����
		if( cycList[i]==-1 ) continue; 

		if( peak0P5InforList[cycList[i]]._fittingFlag )
		{
			float ripple;
			if( CalcRipple<float, float>( xList, yList, peak0P5InforList[cycList[i]]._xFitting, _rippleDX, ripple ) )
				ripple0p5List.Push_Back( ripple );
			else
				ripple0p5List.Push_NoValue();
		}
		else 
		{
			ripple0p5List.Push_NoValue();
		}

		if( peak3InforList[cycList[i]]._fittingFlag )
		{
			float ripple;
			if( CalcRipple<float, float>( xList, yList, peak3InforList[cycList[i]]._xFitting, _rippleDX, ripple ) )
				ripple3List.Push_Back( ripple );
			else
				ripple3List.Push_NoValue();
		}
		else 
		{
			ripple3List.Push_NoValue();
		}
	}

	// ��ָ�걣�浽�ļ�
	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	TxtOBJ txt;
	txt.CreateFile( str+"\\cacheIndicator.xls" );
	
	// д��ͷ
	txt.WriteToEnd( "ITU	����	0.5BW	0.5ripple	0.5IL	0.5CFreq	0.5DX	3BW	3ripple	3IL	3CFreq	3DX	20BW\r\n" );
	// д����� ��С ƽ��
	float min[6], max[6], ave[6];
	bw0p5List.CalcMaxMinAve( min[0], max[0], ave[0] );
	ripple0p5List.CalcMaxMinAve( min[1], max[1], ave[1] );
	VarJList<float> il0p5;
	for( UINT i=0; i<peak0P5InforList.size(); ++i )
	{
		if( peak0P5InforList[i]._fittingFlag )
			il0p5.Push_Back( peak0P5InforList[i]._yFitting );
		else
			il0p5.Push_NoValue();
	}
	il0p5.CalcMaxMinAve( min[2], max[2], ave[2] );
	bw3List.CalcMaxMinAve( min[3], max[3], ave[3] );
	ripple3List.CalcMaxMinAve( min[4], max[4], ave[4] );
	VarJList<float> il3;
	for( UINT i=0; i<peak0P5InforList.size(); ++i )
	{
		if( peak0P5InforList[i]._fittingFlag )
			il3.Push_Back( peak0P5InforList[i]._yFitting );
		else
			il3.Push_NoValue();
	}
	il3.CalcMaxMinAve( min[5], max[5], ave[5] );
	STRING_FORMAT( str, "min	N	%0.2f	%0.2f	%0.2f	N	N	%0.2f	%0.2f	%0.2f	N	N	N\r\n", min[0], min[1], min[2], min[3], min[4], min[5] );
	txt.WriteToEnd( str );
	STRING_FORMAT( str, "max	N	%0.2f	%0.2f	%0.2f	N	N	%0.2f	%0.2f	%0.2f	N	N	N\r\n", max[0], max[1], max[2], max[3], max[4], max[5] );
	txt.WriteToEnd( str );
	STRING_FORMAT( str, "ave	N	%0.2f	%0.2f	%0.2f	N	N	%0.2f	%0.2f	%0.2f	N	N	N\r\n", ave[0], ave[1], ave[2], ave[3], ave[4], ave[5] );
	txt.WriteToEnd( str );
	//
	for( UINT i=0; i<cycList.size(); ++i )
	{
		// ITU
		string tem;
		STRING_FORMAT( tem, "%d	", ituGHZList[i+_ylOffset] );
		str = tem;

		// ���ڴ����ж�
		if( cycList[i]==-1 )
		{
			STRING_FORMAT( str, "%dȱʧ	\r\n", i+1 );
			txt.WriteToEnd( str );
			continue;
		}

		// �������
		STRING_FORMAT( tem, "%d	", i+1 );
		str += tem;	

		// 0.5db
		str += AnalyzeData()( bw0p5List, cycList[i] );
		str += AnalyzeData()( ripple0p5List, cycList[i] );

		if( peak0P5InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak0P5InforList[cycList[i]]._yFitting );
			str += tem;
		}
		else
		{
			str += "������	";
		}

		if( peak0P5InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak0P5InforList[cycList[i]]._xFitting );
			str += tem;
		}
		else
		{
			str += "������	";
		}

		str += AnalyzeData()( center0p5DXList, cycList[i] );

		// 3db
		str += AnalyzeData()( bw3List, cycList[i] );
		str += AnalyzeData()( ripple3List, cycList[i] );

		if( peak3InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak3InforList[cycList[i]]._yFitting );
			str += tem;
		}
		else
		{
			str += "������	";
		}

		if( peak3InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak3InforList[cycList[i]]._xFitting );
			str += tem;
		}
		else
		{
			str += "������	";
		}

		str += AnalyzeData()( center3DXList, cycList[i] );

		// 20db
		str += AnalyzeData()( bw20List, cycList[i] );

		str += "\r\n";
		txt.WriteToEnd( str );
	}
}

bool WSST1::GetOSATrace( VECTOR_FLOAT& xNMList, VECTOR_FLOAT& vList )
{
	string str;
	xNMList.clear();
	vList.clear();

	PCB_View_ShowMsg( "OSA��ȡtrace����.." );
	_osa.GetTraceA( str );

	PCB_View_ShowMsg( "����trace����ȥ���.." );
	ParseOSATrace( str, vList );
	//
	if( _slDataList[_oswChannel-1].size()==vList.size() )
	{
		for( UINT i=0; i<vList.size(); ++i )
			vList[i] -= _slDataList[_oswChannel-1][i];
	}
	else
	{
		STRING_FORMAT( str, "���ݲ�����trace=%d sl=%d", vList.size(), _slDataList[_oswChannel-1].size() );
		PCB_View_ShowMsg( str.c_str() );

		return false;
	}

	// ����x������
	float dx = ( 1570.f-1525.f )/( vList.size()-1 );
	for( UINT i=0; i<vList.size(); ++i )
		xNMList.push_back( 1525.f+dx*i );

	return true;
}










