#include "stdafx.h"
#include "WSST3.h"
#include "WSSTX.h"

using namespace std;

PFunc_View_Type0 PCB_View_ShowMsg;
PFunc_View_Type101 PCB_View_SetIndicatorGrid;
PFunc_View_Type102 PCB_View_SetIndicatorGridCell;

WSST3* WSST3::_pSingleton = NULL;


struct CalcMaxMinAve
{
	void operator()( VarJList<double>& dataList )
	{
		if( dataList.Size()==0 ) return;

		double maxV, minV, aveV;
		if( dataList.CalcMaxMinAve( maxV, minV, aveV ) )
		{
			dataList.Push_Back( minV );
			dataList.Push_Back( maxV );
			dataList.Push_Back( aveV );
		}
		else
		{
			dataList.Push_NoValue();
			dataList.Push_NoValue();
			dataList.Push_NoValue();
		}
	}
};

struct GetShowDataMsg
{
	void operator()( VarJList<double>& dataList, ULONG index, string& showMsg )
	{
		if( dataList.Size()==0 )
			showMsg = "不计算";
		else if( dataList.Size()<=index )
			showMsg = "未知错误";
		else if( (bool)dataList[index]==false )
			showMsg = "不存在";
		else
			STRING_FORMAT( showMsg, "%0.3lf", (double)dataList[index] );
	}

	void operator()( VarJList<double>& dataList, ULONG index, string& showMsg, bool& isColor )
	{
		this->operator()( dataList, index, showMsg );
		
		if( showMsg=="不计算" || showMsg=="未知错误" || showMsg=="不存在" )
			isColor = true;
		else 
			isColor = false;
	}
};

struct SetGridCellDataMsg
{
	void operator()( int port, VarJList<double>& dataList, ULONG index,  ULONG column )
	{
		string str;
		bool isColor;

		GetShowDataMsg()( dataList, index, str, isColor );
		(*PCB_View_SetIndicatorGridCell)( port, index+1, column, str.c_str(), isColor==false?RGB(0,0,0):RGB(0,0,255) );
	}
};


WSST3* WSST3::GetSingleton()
{
	if( _pSingleton==NULL )
		_pSingleton = new WSST3();

	return _pSingleton;
}

void WSST3::Destroy()
{
	if( _pSingleton!=NULL )
		delete _pSingleton;
}


WSST3::WSST3()
{
	_parseType = 0;

	string str;
	GetExeDirectory( str );
	GetUpDirectory( str );
	str += "\\Data";

	LoadParseITUList( str+"\\Parse_ITU.xls" );
}

void WSST3::LoadParseITUList( string parseItuFile )
{
	// 载入分析用itu波长信息
	TxtOBJ txtOBJ;
	if( txtOBJ.OpenFile( parseItuFile )==true )
	{
		_parseITUGhzList50G.clear();

		vector<VECTOR_STRING > allLineWordsList;
		txtOBJ.ReadAllLineWords( allLineWordsList );
		//
		UINT allSize = allLineWordsList.size();
		for( UINT i=1; i<allSize; ++i )
		{
			VECTOR_STRING& wordList = allLineWordsList[allSize-i];

			if( atoi( wordList[0].c_str() )%50==0 )
			{
				_parseITUGhzList50G.push_back( atoi( wordList[0].c_str() ) );

				if( _parseITUGhzList50G.back()%100==0 )
					_parseITUGhzList100G.push_back( _parseITUGhzList50G.back() );
			}
		}
	}
}

int WSST3::CalcStorageIndex( string str )
{
	int index = -1;

	if( _parseType==0 )				// 对应光谱仪
	{
		VECTOR_STRING wordList;
		VECTOR_CHAR splitList;
		splitList.push_back( '-' );
		splitList.push_back( '.' );
		//
		SplitWords( str, wordList, splitList );
		if( wordList.size()!=3 )
			return -1;

		int p = atoi( string( wordList[0].begin()+1, wordList[0].end() ).c_str() );
		index = ( p-1 )*7;
		if( wordList[1]=="0" )
			index += 0;
		else if( wordList[1]=="5" )
			index += 1;
		else if( wordList[1]=="10" )
			index += 2;
		else if( wordList[1]=="15" )
			index += 3;
		else if( wordList[1]=="B" )
			index += 4;
	}
	else if( _parseType==1 )		// 测试系统
	{
		VECTOR_STRING wordList;
		VECTOR_CHAR splitList;
		splitList.push_back( '-' );
		//
		SplitWords( str, wordList, splitList );
		if( wordList.size()!=4 )
			return -1;

		int p = atoi( string( wordList[1].begin()+1, wordList[1].end() ).c_str() );
		index = ( p-1 )*7;
		if( wordList[0]=="all" )
		{
			if( wordList[2]=="0dB" )
				index += 0;
			else if( wordList[2]=="5" )
				index += 1;
			else if( wordList[2]=="10dB" )
				index += 2;
			else if( wordList[2]=="15dB" )
				index += 3;
			else if( wordList[2]=="block" )
				index += 4;
		}
		else if( wordList[0]=="odd" )
		{
			index += 5;
		}
		else if( wordList[0]=="even" )
		{
			index += 6;
		}
	}

	return index;
}

void WSST3::StartLoadFileListThread( VECTOR_STRING& filePathList )
{
	_filePathListCache = filePathList;
	StartThread( "LoadFileListThread", &WSST3::LoadFileListThread );
}

void WSST3::LoadFileListThread()
{
	_parseFileList.clear();
	_parseFileList.resize( 7*PORT_NUM );

	for( UINT i=0; i<_filePathListCache.size(); ++i )
			LoadFileX( _filePathListCache[i] );
}

void WSST3::LoadFileX( string filePath )
{
	string str;
	STRING_FORMAT( str, "读取文件 %s..", filePath.c_str() );	
	(*PCB_View_ShowMsg)( str.c_str() );

	// 打开数据文件
	TxtOBJ txtOBJ;
	if( !txtOBJ.OpenFile( filePath ) ) 
	{
		STRING_FORMAT( str, "读取文件%s失败..", filePath.c_str() );	
		(*PCB_View_ShowMsg)( str.c_str() );
		return;
	}

	// 通过文件名分析文件标识
	string fileName;
	GetFileName( filePath, fileName );
	int index = CalcStorageIndex( fileName );
	if( index==-1 )
	{
		(*PCB_View_ShowMsg)( "文件名不对" );
		return;
	}

	// 根据文件标识找到特定的存储位置
	ParseFile& parseFileData = _parseFileList[index];
	parseFileData._filePath = fileName;

	// 反向存储
	if( _parseType==0 )		// 光谱仪
	{
		// 读取所有数据
		vector<VECTOR_STRING > allLineWordsList;
		txtOBJ.ReadAllLineWords( allLineWordsList );

		int p = index/7+1;

		for( UINT i=allLineWordsList.size()-1; i>30; --i )
		{
			VECTOR_STRING& lineWords = allLineWordsList[i];
			if( lineWords.size()!=2 ) continue;
			if( atof( lineWords[0].c_str() )<1500 ) continue;

			parseFileData._xList.push_back( LIGHT_SPEED/atof( lineWords[0].c_str() ) );
			parseFileData._yILPortList[p-1].push_back( atof( lineWords[1].c_str() ) );
		}
	}
	else					// 测试系统
	{
		// 读取所有数据  
		vector<VECTOR_STRING > allLineWordsList;
		txtOBJ.ReadAllLineWords( allLineWordsList );

		for( UINT i=allLineWordsList.size()-1; i>10; --i )
		{
			VECTOR_STRING& lineWords = allLineWordsList[i];
			if( lineWords.size()!=9 ) continue;
			if( atof( lineWords[0].c_str() )<1500 ) continue;

			parseFileData._xList.push_back( LIGHT_SPEED/atof( lineWords[0].c_str() ) );

			parseFileData._yILPortList[0].push_back( atof( lineWords[3].c_str() ) );
			parseFileData._yILPortList[1].push_back( atof( lineWords[7].c_str() ) );

			parseFileData._yPDLPortList[0].push_back( atof( lineWords[4].c_str() ) );
			parseFileData._yPDLPortList[1].push_back( atof( lineWords[8].c_str() ) );
		}
	}
}

void WSST3::StartSetParseITUListThread( VECTOR_INT parseITUList )
{
	_parseITUListCache = parseITUList;
	StartThread( "SetParseITUListThread", &WSST3::SetParseITUListThread );
}

void WSST3::SetParseITUListThread()
{
	_parseITUList = _parseITUListCache;
}

void WSST3::StartParseFileListThread()
{
	// 
	if( FREQ_GHZ==100 )
	{
		ITU_DXRANGE = 20;
		CHANNEL_INTERVAL = 100;
	}
	else if( FREQ_GHZ==50 )
	{
		ITU_DXRANGE = 10;
		CHANNEL_INTERVAL = 50;
	}

	StartThread( "ParseFileListThread", &WSST3::ParseFileListThread );
}

void WSST3::ParseFileListThread()
{
	(*PCB_View_ShowMsg)( "开始分析指标.." );

	ParseFileListX();

	(*PCB_View_ShowMsg)( "指标分析完成.." );
}

void WSST3::ParseFileListX()
{
	// 计算周期信息
	for( int i=0; i<7*PORT_NUM; ++i )
		_parseFileList[i].CalcCycleList( _parseITUList, ITU_DXRANGE );

	if( _parseType==0 )			// 分析光谱仪数据指标
		ParseFileListXType0();
	else if( _parseType==1 )
		ParseFileListXType1();
}

void WSST3::ParseFileListXType0()
{
	// Insertion Loss 和 Insertion Loss Ripple: 0db
	VarJList<double> insertionLoss[MAX_PORT_NUM];
	VarJList<double> insertionLossRipple[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7], p, insertionLoss[p-1], insertionLossRipple[p-1] );
	}

	// Insertion Loss Uniformity: 0db
	VarJ<double> insertionLossUniformity[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParseInsertionLossUniformity( insertionLoss[p-1], insertionLossUniformity[p-1] );
	}

	// Channel Extinction Ratio：0db, block
	VarJList<double> ChannelExtinctionRatio[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" && _parseFileList[( p-1 )*7+4]._filePath!="" )
			ParseChannelExtinctionRatio( _parseFileList[( p-1 )*7], p, _parseFileList[( p-1 )*7+4]._yILPortList[p-1], ChannelExtinctionRatio[p-1] );
	}

	// BW@0.5dB（0dB衰减即可）：a、f
	VarJList<double> bw0p5_0db[MAX_PORT_NUM];
	VarJList<double> cw0p5_0db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" )
			ParseBW_0p5( _parseFileList[( p-1 )*7], p, bw0p5_0db[p-1], &cw0p5_0db[p-1] );
	}

	// BW@1.0dB（0dB、5dB、10dB、15dB衰减时）：a、b、c、d、f、g、h、i
	VarJList<double> bw1_0db[MAX_PORT_NUM], bw1_5db[MAX_PORT_NUM], bw1_10db[MAX_PORT_NUM], bw1_15db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" )	ParseBW_1( _parseFileList[( p-1 )*7], p, bw1_0db[p-1] );
		if( _parseFileList[( p-1 )*7+1]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+1], p, bw1_5db[p-1] );
		if( _parseFileList[( p-1 )*7+2]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+2], p, bw1_10db[p-1] );
		if( _parseFileList[( p-1 )*7+3]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+3], p, bw1_15db[p-1] );
	}

	// BW@3.0dB（0dB衰减即可）：a、b、c、d、f、g、h、i
	VarJList<double> bw3_0db[MAX_PORT_NUM], bw3_5db[MAX_PORT_NUM], bw3_10db[MAX_PORT_NUM], bw3_15db[MAX_PORT_NUM];
	VarJList<double> cw3_0db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" )	ParseBW_3( _parseFileList[( p-1 )*7], p, bw3_0db[p-1], &cw3_0db[p-1] );
		if( _parseFileList[( p-1 )*7+1]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+1], p, bw3_5db[p-1] );
		if( _parseFileList[( p-1 )*7+2]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+2], p, bw3_10db[p-1] );
		if( _parseFileList[( p-1 )*7+3]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+3], p, bw3_15db[p-1] );
	}

	// 分析精度
	// 5db
	VarJList<double> accuracy5[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+1]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+1], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 5, accuracy5[p-1] );
		}
	}	
	// 10db
	VarJList<double> accuracy10[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+2]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+2], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 10, accuracy10[p-1] );
		}
	}
	// 15db
	VarJList<double> accuracy15[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+3]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+3], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 15, accuracy15[p-1] );
		}
	}

	// 计算最大 最小 平均值
	for( int p=1; p<=PORT_NUM; ++p )
	{
		CalcMaxMinAve()( insertionLoss[p-1] );
		CalcMaxMinAve()( insertionLossRipple[p-1] );
		CalcMaxMinAve()( ChannelExtinctionRatio[p-1] );
		CalcMaxMinAve()( bw0p5_0db[p-1] );
		CalcMaxMinAve()( bw1_0db[p-1] );
		CalcMaxMinAve()( bw1_5db[p-1] );
		CalcMaxMinAve()( bw1_10db[p-1] );
		CalcMaxMinAve()( bw1_15db[p-1] );	
		CalcMaxMinAve()( bw3_0db[p-1] );
		CalcMaxMinAve()( bw3_5db[p-1] );
		CalcMaxMinAve()( bw3_10db[p-1] );
		CalcMaxMinAve()( bw3_15db[p-1] );	
		CalcMaxMinAve()( accuracy5[p-1] );
		CalcMaxMinAve()( accuracy10[p-1] );
		CalcMaxMinAve()( accuracy15[p-1] );
		CalcMaxMinAve()( cw0p5_0db[p-1] );
		CalcMaxMinAve()( cw3_0db[p-1] );
	}
	
	// 设置到view上
	// 设置grid行列数
	for( int p=1; p<=PORT_NUM; ++p )
		(*PCB_View_SetIndicatorGrid)( p, _parseITUList.size()+4, 18 );
	// 设置第1行
	for( int p=1; p<=PORT_NUM; ++p )
	{
		(*PCB_View_SetIndicatorGridCell)( p, 0, 0, "ITU", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 1, "InsertionLoss", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 2, "Ripple", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 3, "ExtinctionRatio", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 4, "bw0p5_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 5, "bw1_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 6, "bw1_5db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 7, "bw1_10db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 8, "bw1_15db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 9, "bw3_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 10, "bw3_5db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 11, "bw3_10db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 12, "bw3_15db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 13, "accuracy5", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 14, "accuracy10", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 15, "accuracy15", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 16, "cw0p5_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 17, "cw3_0db", RGB(0,0,0) );
	}
	// 设置第1列
	for( UINT i=0; i<_parseITUList.size(); ++i )
	{
		string str;
		STRING_FORMAT( str, "%d", _parseITUList[i] );
		for( int p=1; p<=PORT_NUM; ++p )
			(*PCB_View_SetIndicatorGridCell)( p, i+1, 0, str.c_str(), RGB(0,0,0) );
	}
	for( int p=1; p<=PORT_NUM; ++p )
	{
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+1, 0, "max", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+2, 0, "min", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+3, 0, "ave", RGB(0,0,0) );
	}
	// 设置中间的内容
	for( UINT i=0; i<_parseITUList.size()+3; ++i )
	{	
		for( int p=1; p<=PORT_NUM; ++p )
		{
			SetGridCellDataMsg()( p, insertionLoss[p-1], i, 1 );
			SetGridCellDataMsg()( p, insertionLossRipple[p-1], i, 2 );
			SetGridCellDataMsg()( p, ChannelExtinctionRatio[p-1], i, 3 );
			SetGridCellDataMsg()( p, bw0p5_0db[p-1], i, 4 );
			SetGridCellDataMsg()( p, bw1_0db[p-1], i, 5 );
			SetGridCellDataMsg()( p, bw1_5db[p-1], i, 6 );
			SetGridCellDataMsg()( p, bw1_10db[p-1], i, 7 );
			SetGridCellDataMsg()( p, bw1_15db[p-1], i, 8 );
			SetGridCellDataMsg()( p, bw3_0db[p-1], i, 9 );
			SetGridCellDataMsg()( p, bw3_5db[p-1], i, 10 );
			SetGridCellDataMsg()( p, bw3_10db[p-1], i, 11 );
			SetGridCellDataMsg()( p, bw3_15db[p-1], i, 12 );
			SetGridCellDataMsg()( p, accuracy5[p-1], i, 13 );
			SetGridCellDataMsg()( p, accuracy10[p-1], i, 14 );
			SetGridCellDataMsg()( p, accuracy15[p-1], i, 15 );
			SetGridCellDataMsg()( p, cw0p5_0db[p-1], i, 16 );
			SetGridCellDataMsg()( p, cw3_0db[p-1], i, 17 );
		}
	}

	for( int p=1; p<=PORT_NUM; ++p )
	{
		string str;

		TxtOBJ txtObj;
		STRING_FORMAT( str, "分析数据port%d.txt", p );
		txtObj.CreateFile( str.c_str() );
			
		str = "ITU	insertionLoss	insertionLossRipple	ChannelExtinctionRatio	";
		str += "bw0p5_0db	bw1_0db	bw1_5db	bw1_10db	bw1_15db	bw3_0db	bw3_5db	bw3_10db	bw3_15db	";
		str += "accuracy5	accuracy10	accuracy15	cw0p5_0db	cw3_0db\r\n";
		txtObj.WriteToEnd( str );
		for( UINT i=0; i<_parseITUList.size()+3; ++i )
		{
			string lineStr;
			if( i<_parseITUList.size() )
			{
				STRING_FORMAT( str, "%d	", _parseITUList[i] );
				lineStr += str;
			}
			else if( i==_parseITUList.size() )
			{
				lineStr = "min	";
			}
			else if( i==_parseITUList.size()+1 )
			{
				lineStr = "max	";
			}
			else if( i==_parseITUList.size()+2 )
			{
				lineStr = "ave	";
			}

			GetShowDataMsg()( insertionLoss[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( insertionLossRipple[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( ChannelExtinctionRatio[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw0p5_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_5db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_10db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_15db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_5db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_10db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_15db[p-1], i, str );	lineStr += ( str+"	" );	
			GetShowDataMsg()( accuracy5[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( accuracy10[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( accuracy15[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( cw0p5_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( cw3_0db[p-1], i, str );	lineStr += ( str+"	" );
			lineStr += "\r\n";
			txtObj.WriteToEnd( lineStr );
		}
	}
}

void WSST3::ParseFileListXType1()
{
	// 分析指标
	// Insertion Loss 和 Insertion Loss Ripple: a、f
	VarJList<double> insertionLoss[MAX_PORT_NUM];
	VarJList<double> insertionLossRipple[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7], p, insertionLoss[p-1], insertionLossRipple[p-1] );
	}

	// Insertion Loss Uniformity: a、f
	VarJ<double> insertionLossUniformity[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParseInsertionLossUniformity( insertionLoss[p-1], insertionLossUniformity[p-1] );
	}

	// Channel Extinction Ratio：a、e、f、j
	VarJList<double> ChannelExtinctionRatio[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" && _parseFileList[( p-1 )*7+4]._filePath!="" )
			ParseChannelExtinctionRatio( _parseFileList[( p-1 )*7], p, _parseFileList[( p-1 )*7+4]._yILPortList[0], ChannelExtinctionRatio[0] );
	}

	// Directivity：a、f
	VarJList<double> directivity[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParseDirectivity( _parseFileList[( p-1 )*7], p, directivity[p-1] );
	}

	// Port-Port Isolation：a、f
	VarJList<double> PortPortIsolation[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" ) 
			ParsePortIsolation( insertionLoss[p-1], directivity[p-1], PortPortIsolation[p-1] );
	}

	// BW@0.5dB（0dB衰减即可）：a、f
	VarJList<double> bw0p5_0db[MAX_PORT_NUM];
	VarJList<double> cw0p5_0db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7]._filePath!="" )
			ParseBW_0p5( _parseFileList[p-1], p, bw0p5_0db[p-1], &cw0p5_0db[p-1] );
	}

	// BW@1.0dB（0dB、5dB、10dB、15dB衰减时）
	VarJList<double> bw1_0db[MAX_PORT_NUM], bw1_5db[MAX_PORT_NUM], bw1_10db[MAX_PORT_NUM], bw1_15db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+0]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+0], p, bw1_0db[p-1] );
		if( _parseFileList[( p-1 )*7+1]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+1], p, bw1_5db[p-1] );
		if( _parseFileList[( p-1 )*7+2]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+2], p, bw1_10db[p-1] );
		if( _parseFileList[( p-1 )*7+3]._filePath!="" ) ParseBW_1( _parseFileList[( p-1 )*7+3], p, bw1_15db[p-1] );
	}

	// BW@3.0dB（0dB衰减即可）：a、b、c、d、f、g、h、i
	VarJList<double> bw3_0db[MAX_PORT_NUM], bw3_5db[MAX_PORT_NUM], bw3_10db[MAX_PORT_NUM], bw3_15db[MAX_PORT_NUM];
	VarJList<double> cw3_0db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+0]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+0], p, bw3_0db[p-1], &cw3_0db[p-1] );
		if( _parseFileList[( p-1 )*7+1]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+1], p, bw3_5db[p-1] );
		if( _parseFileList[( p-1 )*7+2]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+2], p, bw3_10db[p-1] );
		if( _parseFileList[( p-1 )*7+3]._filePath!="" ) ParseBW_3( _parseFileList[( p-1 )*7+3], p, bw3_15db[p-1] );
	}

	// BW@20.0dB（0dB衰减即可）
	VarJList<double> bw20_0db[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+5]._filePath!="" && _parseFileList[( p-1 )*7+6]._filePath!="" ) 
			ParseBW_20( _parseFileList[( p-1 )*7+5], _parseFileList[( p-1 )*7+6], p, bw20_0db[p-1] );
	}

	// PDL（0dB、5dB、10dB、15dB衰减时）
	VarJList<double> PDL0[MAX_PORT_NUM], PDL5[MAX_PORT_NUM], PDL10[MAX_PORT_NUM], PDL15[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+0]._filePath!="" ) ParsePDL( _parseFileList[( p-1 )*7+0], p, PDL0[p-1] );
		if( _parseFileList[( p-1 )*7+1]._filePath!="" ) ParsePDL( _parseFileList[( p-1 )*7+1], p, PDL5[p-1] );
		if( _parseFileList[( p-1 )*7+2]._filePath!="" ) ParsePDL( _parseFileList[( p-1 )*7+2], p, PDL10[p-1] );
		if( _parseFileList[( p-1 )*7+3]._filePath!="" ) ParsePDL( _parseFileList[( p-1 )*7+3], p, PDL15[p-1] );
	}

	// 分析精度
	// 5db
	VarJList<double> accuracy5[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+1]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+1], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 5, accuracy5[p-1] );
		}
	}
	// 10db
	VarJList<double> accuracy10[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+2]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+2], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 10, accuracy10[p-1] );
		}
	}
	// 15db
	VarJList<double> accuracy15[MAX_PORT_NUM];
	for( int p=1; p<=PORT_NUM; ++p )
	{
		if( _parseFileList[( p-1 )*7+3]._filePath!="" )
		{
			VarJList<double> tempIL, tempRipple;
			ParseInsertionLossAndRipple( _parseFileList[( p-1 )*7+3], p, tempIL, tempRipple );
			ParseAccuracy( insertionLoss[p-1], tempIL, 15, accuracy15[p-1] );
		}
	}

	// 计算最大 最小 平均值
	for( int p=1; p<=PORT_NUM; ++p )
	{
		CalcMaxMinAve()( insertionLoss[p-1] );
		CalcMaxMinAve()( insertionLossRipple[p-1] );
		CalcMaxMinAve()( ChannelExtinctionRatio[p-1] );
		CalcMaxMinAve()( PortPortIsolation[p-1] );
		CalcMaxMinAve()( directivity[p-1] );
		CalcMaxMinAve()( bw0p5_0db[p-1] );
		CalcMaxMinAve()( bw1_0db[p-1] );
		CalcMaxMinAve()( bw1_5db[p-1] );
		CalcMaxMinAve()( bw1_10db[p-1] );
		CalcMaxMinAve()( bw1_15db[p-1] );	
		CalcMaxMinAve()( bw3_0db[p-1] );
		CalcMaxMinAve()( bw3_5db[p-1] );
		CalcMaxMinAve()( bw3_10db[p-1] );
		CalcMaxMinAve()( bw3_15db[p-1] );	
		CalcMaxMinAve()( bw20_0db[p-1] );	
		CalcMaxMinAve()( PDL0[p-1] );
		CalcMaxMinAve()( PDL5[p-1] );
		CalcMaxMinAve()( PDL10[p-1] );
		CalcMaxMinAve()( PDL15[p-1] );
		CalcMaxMinAve()( accuracy5[p-1] );
		CalcMaxMinAve()( accuracy10[p-1] );
		CalcMaxMinAve()( accuracy15[p-1] );
		CalcMaxMinAve()( cw0p5_0db[p-1] );
		CalcMaxMinAve()( cw3_0db[p-1] );
	}
	
	// 设置到view上
	// 设置grid行列数
	for( int p=1; p<=PORT_NUM; ++p )
		(*PCB_View_SetIndicatorGrid)( p, _parseITUList.size()+4, 25 );
	// 设置第1行
	for( int p=1; p<=PORT_NUM; ++p )
	{
		(*PCB_View_SetIndicatorGridCell)( p, 0, 0, "ITU", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 1, "insertionLoss", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 2, "Ripple", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 3, "ExtinctionRatio", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 4, "PortPortIsolation", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 5, "directivity", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 6, "bw0p5_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 7, "bw1_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 8, "bw1_5db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 9, "bw1_10db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 10, "bw1_15db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 11, "bw3_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 12, "bw3_5db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 13, "bw3_10db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 14, "bw3_15db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 15, "bw20_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 16, "PDL0", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 17, "PDL5", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 18, "PDL10", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 19, "PDL15", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 20, "accuracy5", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 21, "accuracy10", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 22, "accuracy15", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 23, "cw0p5_0db", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, 0, 24, "cw3_0db", RGB(0,0,0) );
	}
	// 设置第1列
	for( UINT i=0; i<_parseITUList.size(); ++i )
	{
		string str;
		STRING_FORMAT( str, "%d", _parseITUList[i] );
		for( int p=1; p<=PORT_NUM; ++p )
			(*PCB_View_SetIndicatorGridCell)( p, i+1, 0, str.c_str(), RGB(0,0,0) );
	}
	for( int p=1; p<=PORT_NUM; ++p )
	{
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+1, 0, "max", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+2, 0, "min", RGB(0,0,0) );
		(*PCB_View_SetIndicatorGridCell)( p, _parseITUList.size()+3, 0, "ave", RGB(0,0,0) );
	}
	// 设置中间的内容
	for( UINT i=0; i<_parseITUList.size()+3; ++i )
	{	
		for( int p=1; p<=PORT_NUM; ++p )
		{
			SetGridCellDataMsg()( 1, insertionLoss[p-1], i, 1 );
			SetGridCellDataMsg()( 1, insertionLossRipple[p-1], i, 2 );
			SetGridCellDataMsg()( 1, ChannelExtinctionRatio[p-1], i, 3 );
			SetGridCellDataMsg()( 1, PortPortIsolation[p-1], i, 4 );
			SetGridCellDataMsg()( 1, directivity[p-1], i, 5 );
			SetGridCellDataMsg()( 1, bw0p5_0db[p-1], i, 6 );
			SetGridCellDataMsg()( 1, bw1_0db[p-1], i, 7 );
			SetGridCellDataMsg()( 1, bw1_5db[p-1], i, 8 );
			SetGridCellDataMsg()( 1, bw1_10db[p-1], i, 9 );
			SetGridCellDataMsg()( 1, bw1_15db[p-1], i, 10 );
			SetGridCellDataMsg()( 1, bw3_0db[p-1], i, 11 );
			SetGridCellDataMsg()( 1, bw3_5db[p-1], i, 12 );
			SetGridCellDataMsg()( 1, bw3_10db[p-1], i, 13 );
			SetGridCellDataMsg()( 1, bw3_15db[p-1], i, 14 );
			SetGridCellDataMsg()( 1, bw20_0db[p-1], i, 15 );
			SetGridCellDataMsg()( 1, PDL0[p-1], i, 16 );
			SetGridCellDataMsg()( 1, PDL5[p-1], i, 17 );
			SetGridCellDataMsg()( 1, PDL10[p-1], i, 18 );
			SetGridCellDataMsg()( 1, PDL15[p-1], i, 19 );
			SetGridCellDataMsg()( 1, accuracy5[p-1], i, 20 );
			SetGridCellDataMsg()( 1, accuracy10[p-1], i, 21 );
			SetGridCellDataMsg()( 1, accuracy15[p-1], i, 22 );
			SetGridCellDataMsg()( 1, cw0p5_0db[p-1], i, 23 );
			SetGridCellDataMsg()( 1, cw3_0db[p-1], i, 24 );
		}
	}

	for( int p=1; p<=PORT_NUM; ++p )
	{
		string str;

		TxtOBJ txtObj;
		STRING_FORMAT( str, "分析数据port%d.txt", p );
		txtObj.CreateFile( str.c_str() );
			
		str = "ITU	insertionLoss	insertionLossRipple	ChannelExtinctionRatio	";
		str += "bw0p5_0db	bw1_0db	bw1_5db	bw1_10db	bw1_15db	bw3_0db	bw3_5db	bw3_10db	bw3_15db	";
		str += "accuracy5	accuracy10	accuracy15	cw0p5_0db	cw3_0db	";
		str += "PDL0	PDL5	PDL10	PDL15	PortPortIsolation	directivity	bw20_0db\r\n";
		txtObj.WriteToEnd( str );
		for( UINT i=0; i<_parseITUList.size()+3; ++i )
		{
			string lineStr;
			if( i<_parseITUList.size() )
			{
				STRING_FORMAT( str, "%d	", _parseITUList[i] );
				lineStr += str;
			}
			else if( i==_parseITUList.size() )
			{
				lineStr = "min	";
			}
			else if( i==_parseITUList.size()+1 )
			{
				lineStr = "max	";
			}
			else if( i==_parseITUList.size()+2 )
			{
				lineStr = "ave	";
			}

			GetShowDataMsg()( insertionLoss[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( insertionLossRipple[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( ChannelExtinctionRatio[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw0p5_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_5db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_10db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw1_15db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_5db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_10db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw3_15db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( accuracy5[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( accuracy10[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( accuracy15[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( cw0p5_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( cw3_0db[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( PDL0[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( PDL5[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( PDL10[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( PDL15[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( PortPortIsolation[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( directivity[p-1], i, str );	lineStr += ( str+"	" );
			GetShowDataMsg()( bw20_0db[p-1], i, str );	lineStr += ( str+"	" );
			lineStr += "\r\n";
			txtObj.WriteToEnd( lineStr );
		}
	}
}

void WSST3::ParseAccuracy( VarJList<double>& loss_0db, VarJList<double>& loss_xdb, double xdb, VarJList<double>& accuracyList )
{
	for( UINT i=0; i<loss_0db.Size(); ++i )
	{
		if( (bool)loss_0db[i]==false || (bool)loss_xdb[i]==false )
		{
			accuracyList.Push_NoValue();
			continue;
		}

		accuracyList.Push_Back( (double)loss_0db[i]-(double)loss_xdb[i]-xdb );
	}
}

void WSST3::ParsePDL( ParseFile& parseFile, int port, VarJList<double>& pdlList )
{
	for( UINT i=0; i<parseFile._cycleList.size(); ++i )
	{
		int startIndex = parseFile._cycleList[i]._range.first;
		int endIndex = parseFile._cycleList[i]._range.second;

		if( startIndex==-1 || endIndex==-1 )
		{
			pdlList.Push_NoValue();
			continue;
		}

		// 在通带区域计算指标
		pdlList.Push_Back( *max_element( parseFile._yPDLPortList[port-1].begin()+startIndex, parseFile._yPDLPortList[port-1].begin()+endIndex ) );
	}
}

void WSST3::ParseBW_0p5( ParseFile& parseFile, int port, VarJList<double>& bw, VarJList<double>* pCW )
{
	parseFile.CalcBindWidth( port, 0.5, true, bw, pCW );
}

void WSST3::ParseBW_1( ParseFile& parseFile, int port, VarJList<double>& bw )
{
	parseFile.CalcBindWidth( port, 1, true, bw );
}

void WSST3::ParseBW_3( ParseFile& parseFile, int port, VarJList<double>& bw, VarJList<double>* pCW )
{
	parseFile.CalcBindWidth( port, 3, true, bw, pCW );
}

void WSST3::ParseBW_20( ParseFile& parseFile1, ParseFile& parseFile2, int port, VarJList<double>& bw )
{
	VarJList<double> tempList1, tempList2;
	parseFile1.CalcBindWidth( port, 20, false, tempList1 );
	parseFile2.CalcBindWidth( port, 20, false, tempList2 );

	for( UINT i=0; i<tempList1.Size(); ++i )
	{
		if( (bool)tempList1[i] )
			bw.Push_Back( (double)tempList1[i] );
		else if( (bool)tempList2[i] )
			bw.Push_Back( (double)tempList2[i] );
		else
			bw.Push_NoValue();
	}
}

void WSST3::ParseInsertionLossAndRipple( ParseFile& parseFile, int port, VarJList<double>& insertionLoss, VarJList<double>& insertionLossRipple )
{
	insertionLoss.Clear();
	insertionLossRipple.Clear();
	for( UINT i=0; i<parseFile._cycleList.size(); ++i )
	{
		int startIndex = parseFile._cycleList[i]._range.first;
		int endIndex = parseFile._cycleList[i]._range.second;

		// 验证通带存在性
		if( startIndex==-1 || endIndex==-1 )
		{
			insertionLoss.Push_NoValue();
			insertionLossRipple.Push_NoValue();
			continue;
		}

		// 在通带区域计算指标
		VECTOR_DOUBLE& yList = parseFile._yILPortList[port-1];
		double ILmin = *min_element( yList.begin()+startIndex, yList.begin()+endIndex );
		double ILmax = *max_element( yList.begin()+startIndex, yList.begin()+endIndex );

		insertionLoss.Push_Back( ILmin );
		insertionLossRipple.Push_Back( ILmax-ILmin );
	}
}

void WSST3::ParseInsertionLossUniformity( VarJList<double>& insertionLoss, VarJ<double>& insertionLossUniformity )
{
	double min, max, ave;
	if( insertionLoss.CalcMaxMinAve( min, max, ave )==true )
		insertionLossUniformity = max-min;
}

void WSST3::ParseChannelExtinctionRatio( ParseFile& parseFile, int port, VECTOR_DOUBLE& blockYList, VarJList<double>& ChannelExtinctionRatio )
{
	ChannelExtinctionRatio.Clear();
	for( UINT i=0; i<parseFile._cycleList.size(); ++i )
	{
		int startIndex = parseFile._cycleList[i]._range.first;
		int endIndex = parseFile._cycleList[i]._range.second;

		// 验证通带存在性
		if( startIndex==-1 || endIndex==-1 )
		{
			ChannelExtinctionRatio.Push_NoValue();
			continue;
		}

		//
		VECTOR_DOUBLE& yList = parseFile._yILPortList[port-1];
		double IL1 = *min_element( yList.begin()+startIndex, yList.begin()+endIndex );
		double IL2 = *max_element( blockYList.begin()+startIndex, blockYList.begin()+endIndex );

		ChannelExtinctionRatio.Push_Back( IL1-IL2 );
	}
}

void WSST3::ParseDirectivity( ParseFile& parseFile, int port, VarJList<double>& directivity )
{
	directivity.Clear();
	for( UINT i=0; i<parseFile._cycleList.size(); ++i )
	{
		int startIndex = parseFile._cycleList[i]._range.first;
		int endIndex = parseFile._cycleList[i]._range.second;

		// 验证通带存在性
		if( startIndex==-1 || endIndex==-1 )
		{
			directivity.Push_NoValue();
			continue;
		}

		// 其他要计算的portList 
		VECTOR_INT portList;
		for( int i=1; i<=PORT_NUM; ++i )
			portList.push_back( i );
		portList.erase( find( portList.begin(), portList.end(), port ) );

		// 获取其他port通带内的最大值
		VECTOR_DOUBLE maxList;
		for( UINT i=0; i<portList.size(); ++i )
		{
			VECTOR_DOUBLE& y2List = parseFile._yILPortList[portList[i]-1];
			maxList.push_back( *max_element( y2List.begin()+startIndex, y2List.begin()+endIndex ) ); 
		}
		double ILMax = *max_element( maxList.begin(), maxList.end() );

		directivity.Push_Back( ILMax );
	}
}

void WSST3::ParsePortIsolation(	VarJList<double>& insertionLoss, VarJList<double>& directivity, VarJList<double>& PortIsolation )
{
	PortIsolation.Clear();
	for( UINT i=0; i<insertionLoss.Size(); ++i )
	{
		if( (bool)insertionLoss[i]==false )
		{
			PortIsolation.Push_NoValue();
			continue;
		}

		PortIsolation.Push_Back( (double)insertionLoss[i] - (double)directivity[i] );
	}
}                         
	
	
void WSST3::StartSaveLightThread()
{
	StartThread( "SaveLightThread", &WSST3::SaveLightThread );
}        
	
void WSST3::SaveLightThread()
{
	ConnectDevices();
	OSW32X& osw1 = _osw[0];
	OSW32X& osw2 = _osw[1];

	string str;
	for( int i=0; i<_wssNum; ++i )
	{
		int c1 = i+1;
		STRING_FORMAT( str, "1号光开关切换到portd%", c1 );
		PCB_View_ShowMsg( str.c_str() );
		osw1.SetChannel( c1 );
	
		int preCount = 0;
		for( int j=0; j<_portNumList[i]; ++j )
		{
			int c2 = preCount+j+1;

			STRING_FORMAT( str, "2号光开关切换到portd%", c2 );
			PCB_View_ShowMsg( str.c_str() );
			osw2.SetChannel( c2 );

			//
			STRING_FORMAT( str, "1号光开关port%d接到2号光开关portd%", c1, c2 );
			::MessageBoxA( 0, str.c_str(), "提示", 0 );

			// 采集该port存光数据
			string trace;
			_osa.GetTraceA( trace );
			//
			STRING_FORMAT( str, "T3SL_%d_%d", c1, c2 );
			SaveSLData( str, trace );
		}
		preCount += _portNumList[i];
	}

	PCB_View_ShowMsg( "存光过程完成.." );
}      
	
void WSST3::ConnectDevices()
{
	// 打开光开关
	OSW32X& osw1 = _osw[0];
	OSW32X& osw2 = _osw[1];
	osw1.Open( 4 );
	osw2.Open( 5 );

	// 连接光谱仪
	_osa.ConnnectTCPIP( _osaIP, _osaPort );
}                        
	
void WSST3::StartCollectAllTempDataThread()
{
	StartThread( "CollectAllTempDataThread", &WSST3::CollectAllTempDataThread );
}              
	
void WSST3::CollectAllTempDataThread()
{
	ConnectDevices();

	TBMC711 temBox;
	for( UINT t=0; t<_tempPointList.size(); ++t )
	{
		// 设置循环箱温度
		string str;
		STRING_FORMAT( str, "%d次:设置温循箱温度到%0.1f..", t+1, _tempPointList[t] );
		(*PCB_View_ShowMsg)( str.c_str() );
		while( temBox.SetSetTem( _tempPointList[t] )==false ) { Sleep( 1000 ); }

		// 等待所有电路板稳定到该温度
		VECTOR_INT paraList;
		VECTOR_FLOAT stableTempList; 
		for( int i=0; i<_wssNum; ++i )
			paraList.push_back( _wssCommPara[i] );
		if( _wssCommWay==1 )
			WaitingTempStable_Com( paraList, stableTempList );
		else
			WaitingTempStable_Net( paraList, stableTempList );

		// 采集该温度点数据
		CollectTempData();
	}
}              
	
bool WSST3::WavelengthCalibration()
{
	PCB_View_ShowMsg( "波长校准.." );

	if( _isType1N )		//MessageBox("请将TLS连接至OSW2的通道32，开始进行波长校准！");
		_osw[1].SetChannel( 32 );
	else				//MessageBox("请将TLS连接至OSW1的通道32，开始进行波长校准！");
		_osw[0].SetChannel( 32 );

	string res;
	string str;

	_osa.SendCmd( "INIT:IMM" );//需要扫一次
	Sleep( 5000 );
	_osa.SendCmd( "CAL:WAV:EXT:WAV 1546.922nm" );
	Sleep( 1000 );
	_osa.SendCmd( "CAL:WAV:EXT" );
	Sleep( 5000 );
	_osa.SendCmd("SENS:BAND:RES 0.02nm");
	Sleep( 1000 );
	_osa.SendCmd( "INIT:IMM" );//需要扫一次
	Sleep( 5000 );
	_osa.SendCmd( "CALC:MARK:MAX" );
	Sleep( 1000 );
	_osa.SendCmd( "CALC:MARK:X? 0" );
	_osa.RecvRes( res );
	float l = (float)atof( string( res.begin(), res.begin()+10 ).c_str() )*1000;
	 
	float RBW = 0;
	int count = 1;
	while( RBW!=0.05f )
	{
		STRING_FORMAT( str, "第%d次设置分辨率为0.05nm", count++ );
		PCB_View_ShowMsg( str.c_str() );

		_osa.SendCmd( "SENS:BAND:RES 0.05nm" );
		Sleep( 1000 );
		_osa.SendCmd( "SENS:BAND?" );
		_osa.RecvRes( res );
		float RBW = (float)atof( string( res.begin(), res.begin()+10 ).c_str() )/100;
	}

	_osa.SendCmd("INIT:IMM");//需要扫一次
	Sleep(5000);

	return abs( l-1546.922f )<0.003f ? true:false;
}         
	
	
void WSST3::CollectTempData()
{
	OSW32X& osw1 = _osw[0];
	OSW32X& osw2 = _osw[1];

	string str;
	for( UINT t=0; t<_tempPointList.size(); ++t )
	{
		for( int i=0; i<_wssNum; ++i )
		{
			// 切换光开关1
			int c1 = i+1;
			STRING_FORMAT( str, "1号光开关切换到portd%", c1 );
			PCB_View_ShowMsg( str.c_str() );
			osw1.SetChannel( c1 );
	
			// 依次切换光开关2
			int preCount = 0;
			for( int j=0; j<_portNumList[i]; ++j )
			{
				int c2 = preCount+j+1;
				
				STRING_FORMAT( str, "2号光开关切换到portd%", c2 );
				PCB_View_ShowMsg( str.c_str() );
				osw2.SetChannel( c2 );

				// 相邻port的0dB衰减、20dB衰减
				SetPortAtten( _wssCommPara[i], j, 0, _wssChannelNum[i] );
				SetPortAtten( _wssCommPara[i], j+2, 20, _wssChannelNum[i] );		

				// 0dB衰减、5dB衰减、10dB衰减、15dB衰减、25.5dB衰减（block）
				string trace;
				SetPortAtten( _wssCommPara[i], j+1, 0, _wssChannelNum[i] );
				_osa.GetTraceA( trace );

				//
				SetPortAtten( _wssCommPara[i], j+1, 5, _wssChannelNum[i] );
				_osa.GetTraceA( trace );

				//
				SetPortAtten( _wssCommPara[i], j+1, 10, _wssChannelNum[i] );
				_osa.GetTraceA( trace );

				SetPortAtten( _wssCommPara[i], j+1, 15, _wssChannelNum[i] );
				_osa.GetTraceA( trace );

				SetPortAtten( _wssCommPara[i], j+1, 25.5, _wssChannelNum[i] );
				_osa.GetTraceA( trace );
				
			}
			preCount += _portNumList[i];
		}
	}
}         
	
void WSST3::SetPortAtten( UINT para, int port, float atten, int cNum )
{
	UCHAR pList[104];
	float dList[104];
	for( int c=1; c<=104; ++c )
	{
		pList[c-1] = port;
		dList[c-1] = atten;
	}

	if( _wssCommWay==1 )	
	{
		while( OpenSerial( para, 115200 )==false ) { Sleep( 1000 ); }
		SetAllChannelsPortAtten_Com( pList, dList, cNum );
	}
	else
	{
		SetAllChannelsPortAtten_Net( para, pList, dList, cNum );
	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      