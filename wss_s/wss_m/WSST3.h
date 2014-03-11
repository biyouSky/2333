#pragma once

#include "WSSDevice.h"

typedef void (*PFunc_View_Type101)( int port, int row, int column );
typedef void (*PFunc_View_Type102)( int port, int row, int column, const char* v, COLORREF color );

extern PFunc_View_Type0 PCB_View_ShowMsg;
extern PFunc_View_Type101 PCB_View_SetIndicatorGrid;
extern PFunc_View_Type102 PCB_View_SetIndicatorGridCell;



#define CHANNEL_INTERVAL	WSST3::GetSingleton()->_channelInterval
#define ITU_DXRANGE			WSST3::GetSingleton()->_ituDXRange




class WSST3 : public _QueueThreadManage<WSST3>
{
	struct ParseFile
	{
		// �����б�index��Χ����
		void CalcCycleList( VECTOR_INT& ituGhzList, int ituDXRange )
		{
			_cycleList.clear();
	
			if( _xList.size()==0 )
			{
				std::pair<int, int> range;
				range.first = -1;
				range.second = -1;
				for( UINT i=0; i<ituGhzList.size(); ++i )
					_cycleList.push_back( CycleInfor( range, ituGhzList[i] ) );
				return;
			}

			for( UINT i=0; i<ituGhzList.size(); ++i )
			{
				std::pair<int, int> range;

				// ����ͨ������
				int startGhz = ituGhzList[i]-ituDXRange;
				int endGhz = ituGhzList[i]+ituDXRange;

				// ��֤ͨ�����������
				if( startGhz<_xList.front() || endGhz>_xList.back() )
				{
					range.first = -1;
					range.second = -1;
					_cycleList.push_back( CycleInfor( range, ituGhzList[i] ) );
					continue;
				}

				// �ҵ�ͨ�������index
				int j=0;
				while( _xList[j++]<startGhz );
				range.first = j;
				while( _xList[j++]<endGhz );
				range.second = j-1;
				_cycleList.push_back( CycleInfor( range, ituGhzList[i] ) );
			}
		}

		// �����½�xdb��Ĵ���
		void CalcBindWidth( int port, float downDB, bool minFlag, VarJList<double>& bwList, VarJList<double>* pCWList=NULL )
		{
			struct CalcFittingDownX
			{
				bool operator()( VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, int maxIndex, int ituGhz, float downDB, float range, int dFlag, double& fittingX )
				{
					// ������Ч��Χ
					double xBegin = ituGhz-range;
					double xEnd = ituGhz+range;

					double yDown = yList[maxIndex]-downDB;

					int index = maxIndex;
					while( yList[index]>yDown )
					{
						if( dFlag==0 ) 
						{
							--index;
							if( index<0 )
								return false;
							if( xList[index]<xBegin )
								return false;
						}
						else if( dFlag==1 ) 
						{
							++index;
							if( index>(int)xList.size()-1 )
								return false;
							if( xList[index]>xEnd )
								return false;
						}
					}

					// ����2�����k b
					int index2;
					if( dFlag==0 ) 
						index2 = index+1;
					else if( dFlag==1 ) 
						index2 = index-1;

					double k = ( yList[index]-yList[index2] ) / ( xList[index]-xList[index2] );
					double b = yList[index]-k*xList[index];

					// ����yDown������ϵ�xֵ
					fittingX = ( yDown-b ) / k;
					return true;
				}
			};

			for( UINT i=0; i<_cycleList.size(); ++i )
			{
				int startIndex = _cycleList[i]._range.first;
				int endIndex = _cycleList[i]._range.second;

				if( startIndex==-1 || endIndex==-1 )
				{
					bwList.Push_NoValue();
					continue;
				}

				VECTOR_DOUBLE& xList = _xList;
				VECTOR_DOUBLE& yList = _yILPortList[port-1];
				// �����ֵ
				int maxIndex = max_element( yList.begin()+startIndex, yList.begin()+endIndex )-yList.begin();
				double ILMax = yList[maxIndex]; 
				// ������ҵ�
				double leftDownX;
				if( CalcFittingDownX()( xList, yList, maxIndex, _cycleList[i]._ituGhz, downDB, CHANNEL_INTERVAL/2+10.f, 0, leftDownX )==false )
				{
					bwList.Push_NoValue();
					if( pCWList!=NULL ) pCWList->Push_NoValue();
					continue;
				}
				double rightDownX;
				if( CalcFittingDownX()( xList, yList, maxIndex, _cycleList[i]._ituGhz, downDB, CHANNEL_INTERVAL/2+10.f, 1, rightDownX )==false )
				{
					bwList.Push_NoValue();
					if( pCWList!=NULL ) pCWList->Push_NoValue();
					continue;
				}

				//
				double bw;
				if( minFlag==true )
					bw = 2*min( rightDownX-_cycleList[i]._ituGhz, _cycleList[i]._ituGhz-leftDownX );
				else
					bw = 2*max( rightDownX-_cycleList[i]._ituGhz, _cycleList[i]._ituGhz-leftDownX );

				bwList.Push_Back( bw );
				if( pCWList!=NULL ) pCWList->Push_Back( ( leftDownX+rightDownX )/2 );
			}
		}

		std::string _filePath;									// �ļ���

		VECTOR_DOUBLE _xList;									// Ƶ��
		VECTOR_DOUBLE _yILPortList[ MAX_PORT_NUM ];				// port�Ĳ���
		VECTOR_DOUBLE _yPDLPortList[ MAX_PORT_NUM ];			// port��PDL
	
		struct CycleInfor
		{
			std::pair<int, int> _range;
			int _ituGhz;

			CycleInfor( std::pair<int, int>& range, int ituGhz )
			{
				_range = range;
				_ituGhz = ituGhz;
			}
		};
		std::vector<CycleInfor> _cycleList;						// �����б�
	};
	typedef std::vector<ParseFile> ParseFileListType;

	/*--------------------------------������Ϣ--------------------------------*/
public:
	static WSST3* GetSingleton();
	static void Destroy();
	WSST3();

	void LoadParseITUList( std::string parseItuFile );
	
	VECTOR_INT _parseITUGhzList50G;				// ������itu ������Ϣ
	VECTOR_INT _parseITUGhzList100G;

	int _parseType;								// ��������		0:������	1:����ϵͳ
	int _channelInterval;						// ͨ�����
	int _ituDXRange;							// ����Χ

private:
	static WSST3* _pSingleton;


	/*--------------------------------ָ�����--------------------------------*/
public:
	void StartLoadFileListThread( VECTOR_STRING& filePathList );			// ��������
	void StartSetParseITUListThread( VECTOR_INT parseITUList );				// ����Ҫ�����Ĳ���
	void StartParseFileListThread();										// ��������

private:
	// ����ָ��
	void ParseInsertionLossAndRipple( ParseFile& parseFile, int port, VarJList<double>& insertionLoss, VarJList<double>& insertionLossRipple );
	void ParseInsertionLossUniformity( VarJList<double>& insertionLoss, VarJ<double>& insertionLossUniformity );
	void ParseChannelExtinctionRatio( ParseFile& parseFile, int port, VECTOR_DOUBLE& blockYList, VarJList<double>& ChannelExtinctionRatio );
	void ParseDirectivity( ParseFile& parseFile, int port, VarJList<double>& directivity );
	void ParsePortIsolation( VarJList<double>& insertionLoss, VarJList<double>& directivity, VarJList<double>& PortIsolation );

	void ParseBW_0p5( ParseFile& parseFile, int port, VarJList<double>& bw, VarJList<double>* pCW=NULL );
	void ParseBW_1( ParseFile& parseFile, int port, VarJList<double>& bw );
	void ParseBW_3( ParseFile& parseFile, int port, VarJList<double>& bw, VarJList<double>* pCW=NULL );
	void ParseBW_20( ParseFile& parseFile1, ParseFile& parseFile2, int port, VarJList<double>& bw );
	void ParsePDL( ParseFile& parseFile, int port, VarJList<double>& pdlList );

	void ParseAccuracy( VarJList<double>& loss_0db, VarJList<double>& loss_xdb, double xdb, VarJList<double>& accuracyList );
	
	int CalcStorageIndex( std::string str );				// ������������ļ����ڵ�index

	void LoadFileListThread();
	void LoadFileX( std::string filePath );					// ��������

	void SetParseITUListThread();

	void ParseFileListThread();
	void ParseFileListX();
	void ParseFileListXType0();
	void ParseFileListXType1();

	ParseFileListType _parseFileList;						// �����ļ��б�
	VECTOR_INT _parseITUList;								// �����Ĳ�����

	VECTOR_STRING _filePathListCache;
	VECTOR_INT _parseITUListCache;

	/*--------------------------------�������ݲɼ�--------------------------------*/  
public:
	void StartSaveLightThread();
	void StartCollectAllTempDataThread();

private:
	void SaveLightThread();
	bool WavelengthCalibration();							// ����У׼
	void ConnectDevices();

	void CollectAllTempDataThread();
	void CollectTempData();

	void SetPortAtten( UINT para, int port, float atten, int cNum );

	int _wssNum;
	int _wssCommWay;										// 1 com  2 udp 
	UINT _wssCommPara[6];									// 1 ���� 2 ip
	int _portNumList[6];
	int _wssChannelNum[6];
	bool _isType1N;												

	VECTOR_FLOAT _tempPointList;

	OSW32X _osw[2];

	OSA _osa;
	ULONG _osaIP;
	ULONG _osaPort;
};