
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
	// 连接光谱仪
	bool rt = _osa.ConnnectTCPIP( _osaIP, _osaPort );
	msgList.push_back( rt?"光谱仪连接成功..":"光谱仪连接失败.." );

	// 光开关设置为com模式
	rt = _osw.Open( _oswCom );
	if( rt )
		_osw.ComModeCall();
	msgList.push_back( rt?"光开关连接成功..":"光开关连接失败.." );
}

void WSST1::DisconnectDevices()
{
	_osa.DisConnect();
	_osw.Close();
}

// port为要存光的port 若为-1则存所有port
void WSST1::StartSaveLightProcess( int port )
{
	// 获得要存光的port
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
		PCB_View_ShowMsg( "存光过程开始.." );
	else
		PCB_View_ShowMsg( "存光过程失败.." );
}

void WSST1::SaveLightThread()
{
	string str;
	for( UINT i=0; i<_slPortList.size(); ++i )
	{
		if( WSS_TYPE<20 )
			STRING_FORMAT( str, "请将光开关port%d接到光源", _slPortList[i] );
		else
			STRING_FORMAT( str, "请将光开关port%d接到光谱仪", _slPortList[i] );
		PCB_View_ShowMsg( str.c_str() );

		STRING_FORMAT( str, "切换光开关到port%d", _slPortList[i] );
		PCB_View_ShowMsg( str.c_str() );
		_osw.SetChannel( _slPortList[i] );

		PCB_View_ShowMsg( "osa获取数据.." );
		_osa.GetTraceA( str );

		PCB_View_ShowMsg( "解析并存储存光数据.." );
		SaveSaveLight( str, _slPortList[i] );
	}

	PCB_View_ShowMsg( "存光过程完成.." );
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
		PCB_View_ShowMsg( "开始监控黄线.." );
	else
		PCB_View_ShowMsg( "监控黄线失败.." );
}

void WSST1::YellowLightThread()
{
	string str;

	// 载入存光数据
	PCB_View_ShowMsg( "载入存光.." );
	// 兼容osw不存在的情况  
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

		// 数据转换为ghz为单位
		VECTOR_FLOAT xList, yList;
		for( auto i=xNMList.rbegin(); i!=xNMList.rend(); ++i )
			xList.push_back( LIGHT_SPEED/(*i) );

		for( auto i=vList.rbegin(); i!=vList.rend(); ++i )
			yList.push_back( *i );

		// 计算指标
		CalcYLIndicator( xList, yList );

		// 将指标显示到view
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
				return "不存在	";
			}
		}
	};

	// 计算存在的周期信息
	PCB_View_ShowMsg( "计算峰值周期信息.." );
	BaseWaveOBJ<float> waveOBJ;
	waveOBJ.SetWave( xList, yList );
	waveOBJ.SetInflectionHelpPoint( 35 );
	waveOBJ.SetInflectionHelpDX( 2 );
	waveOBJ.SetPeakYLimit( _lowPeakLimit );
	waveOBJ.ParsePeakWaveCycle();

	// 如何过滤杂峰？？？？？？？
	//

	// 中心频率 
	vector<FittingPointData<float> > peak0P5InforList, peak3InforList, peak20InforList;
	waveOBJ.ParseFittingPeakValley( 0.5, &peak0P5InforList );
	waveOBJ.ParseFittingPeakValley( 3, &peak3InforList );
	waveOBJ.ParseFittingPeakValley( 20, &peak20InforList );

	// 根据0.5db的拟合峰值来标记中间可能缺失的周期
	// 按照ITU频率间隔对应的周期信息index  如果中间缺失则为-1
	// 假设原始计算的waveOBJ._cycleList已经过滤掉杂峰
	VECTOR_INT cycList;
	if( waveOBJ._cycleList.size()>1 )
	{
		cycList.push_back( 0 );			// 记录第一个周期

		for( UINT i=1; i<waveOBJ._cycleList.size(); ++i )
		{
			// 计算中间可能缺失的周期
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

	

	// 计算并绘制黄线  黄线的横坐标以ghz为标准
	VECTOR_FLOAT xYellowList, yYellowList;
	VECTOR_INT& ituGHZList = *(VECTOR_INT*)GetITUGhzList( FREQ_GHZ );
	vector<FittingPointData<float> >& peakInforList = _ylDBFlag==1 ? peak0P5InforList:peak3InforList;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		if( cycList[i]==-1 ) continue;

		xYellowList.push_back( (float)ituGHZList[i+_ylOffset] );
		yYellowList.push_back( peakInforList[cycList[i]]._xFitting-ituGHZList[i+_ylOffset] ); 
	}
	PCB_View_ShowMsg( "绘制trace及黄线.." );
	PCB_View_ShowYLCurve( xList, yList, xYellowList, yYellowList );

	// 中心频率与ITU差异
	VarJList<float> center0p5DXList, center3DXList;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		// 不存在的周期不计算
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

	// 带宽
	VarJList<float> bw0p5List, bw3List, bw20List;
	for( UINT i=0; i<cycList.size(); ++i )
	{
		if( cycList[i]==-1 ) continue;		// 不存在的周期不计算

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
		// 不存在的周期不计算
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

	// 将指标保存到文件
	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	TxtOBJ txt;
	txt.CreateFile( str+"\\cacheIndicator.xls" );
	
	// 写入头
	txt.WriteToEnd( "ITU	周期	0.5BW	0.5ripple	0.5IL	0.5CFreq	0.5DX	3BW	3ripple	3IL	3CFreq	3DX	20BW\r\n" );
	// 写入最大 最小 平均
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

		// 周期存在判定
		if( cycList[i]==-1 )
		{
			STRING_FORMAT( str, "%d缺失	\r\n", i+1 );
			txt.WriteToEnd( str );
			continue;
		}

		// 周期序号
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
			str += "不存在	";
		}

		if( peak0P5InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak0P5InforList[cycList[i]]._xFitting );
			str += tem;
		}
		else
		{
			str += "不存在	";
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
			str += "不存在	";
		}

		if( peak3InforList[cycList[i]]._fittingFlag )
		{
			STRING_FORMAT( tem, "%0.2f	", peak3InforList[cycList[i]]._xFitting );
			str += tem;
		}
		else
		{
			str += "不存在	";
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

	PCB_View_ShowMsg( "OSA获取trace数据.." );
	_osa.GetTraceA( str );

	PCB_View_ShowMsg( "解析trace并减去存光.." );
	ParseOSATrace( str, vList );
	//
	if( _slDataList[_oswChannel-1].size()==vList.size() )
	{
		for( UINT i=0; i<vList.size(); ++i )
			vList[i] -= _slDataList[_oswChannel-1][i];
	}
	else
	{
		STRING_FORMAT( str, "数据不符合trace=%d sl=%d", vList.size(), _slDataList[_oswChannel-1].size() );
		PCB_View_ShowMsg( str.c_str() );

		return false;
	}

	// 计算x轴数据
	float dx = ( 1570.f-1525.f )/( vList.size()-1 );
	for( UINT i=0; i<vList.size(); ++i )
		xNMList.push_back( 1525.f+dx*i );

	return true;
}










