#include "StdAfx.h"
#include "WSSTX.h"


PFunc_View_Type1000 PCB_View_ShowWSSMsg = 0;	



void ParseOSATrace( string& str, VECTOR_FLOAT& vList )
{
	VECTOR_STRING wordList;
	vector<char> splitList;
	splitList.push_back( ',' );
	splitList.push_back( 0x0D );
	SplitWords( str, wordList, splitList );
	//
	for( UINT i=0; i<wordList.size(); ++i )
		vList.push_back( (float)atof( wordList[i].c_str() ) );
}


void SaveSLData( string& trace, string& fileName )
{
	VECTOR_FLOAT vList;
	ParseOSATrace( trace, vList );

	TxtOBJ txt;
	txt.CreateFile( fileName );
	string str;
	for( UINT i=0; i<vList.size(); ++i )
	{
		STRING_FORMAT( str, "%d	%f\r\n", i+1, vList[i] );
		txt.WriteToEnd( str );
	}
}

void LoadSLData( string& fileName, VECTOR_FLOAT& vList )
{
	TxtOBJ txt;

	if( txt.OpenFile( fileName ) )
	{
		vector<VECTOR_STRING > allLineWordsList;
		txt.ReadAllLineWords( allLineWordsList );

		vList.clear();

		for( UINT i=0; i<allLineWordsList.size(); ++i )
		{
			if( allLineWordsList[i].size()==2 )
				vList.push_back( (float)atof( allLineWordsList[i][1].c_str() ) );
		}
	}
}


void WaitingTempStable_Net( VECTOR_INT ipList, VECTOR_FLOAT& stableTempList )
{
	VECTOR_FLOAT tempData[10];

	while( 1 )
	{
_FLAG:
		Sleep( GetWaitTempStableTime() );

		UINT wssNum = ipList.size();
		for( UINT i=0; i<wssNum; ++i )
		{
			float temp;
			while( GetTemperatureSyn_Net( ipList[i], &temp )==false ) { Sleep( 1000 ); }
			if( tempData[i].size()!=0 && abs( temp-tempData[i][0] )>0.11 )
				tempData[i].clear();
			tempData[i].push_back( temp );

			string str;
			STRING_FORMAT( str, "当前温度为%0.1f", temp );
			(*PCB_View_ShowWSSMsg)( ipList[i], str.c_str() );
		}

		// 
		for( UINT i=0; i<wssNum; ++i )
		{
			if( tempData[i].size()<8 )
				goto _FLAG;
		}
		(*PCB_View_ShowMsg)( "所有电路板温度稳定.." );

		// 返回
		for( UINT i=0; i<wssNum; ++i )
			stableTempList[i] = tempData[i].back();
		break;
	}
}

void WaitingTempStable_Com( VECTOR_INT comList, VECTOR_FLOAT& stableTempList )
{
	VECTOR_FLOAT tempData[10];

	while( 1 )
	{
_FLAG:
		Sleep( GetWaitTempStableTime() );

		UINT wssNum = comList.size();
		for( UINT i=0; i<wssNum; ++i )
		{
			float temp;
			while( OpenSerial( comList[i], 115200 )==false ) { Sleep( 1000 ); }
			while( GetTemperatureSyn_Com( temp )==false ) { Sleep( 1000 ); }
			if( tempData[i].size()!=0 && abs( temp-tempData[i][0] )>0.11 )
				tempData[i].clear();
			tempData[i].push_back( temp );

			string str;
			STRING_FORMAT( str, "当前温度为%0.1f", temp );
			(*PCB_View_ShowWSSMsg)( comList[i], str.c_str() );
		}

		// 
		for( UINT i=0; i<wssNum; ++i )
		{
			if( tempData[i].size()<8 )
				goto _FLAG;
		}
		(*PCB_View_ShowMsg)( "所有电路板温度稳定.." );

		// 返回
		for( UINT i=0; i<wssNum; ++i )
			stableTempList[i] = tempData[i].back();
		break;
	}
}