/*
	Add:	2011-5-31
	Update:	2011-6-2

	�������ݷ�����, ���η������ڹյ�����
	
	BaseWaveOBJ: �����������ݷ���
		||
		\/
	LaserWaveOBJ: ��ӦITU�������ݷ���

	ITUDataParseOBJ: ֻͨ��ITU�Լ�������������ݡ���δʵ��
*/

#pragma once
#include "GXBaseMath.h"

/*
    �յ��ж�:
	
	���η���: ���ڹյ��ж�, �ձ������ϵĲ������ڼ���, ���ӷ�ֵ����

	�Ⲩ�η���: ���ڲ��η���, ����itu��������ֵ�ж����˳�Ҫ��������ʵ����, �����Ӵ�������㷨
*/


/*
	�жϹյ�, ���ҽ����ж�
	����:
	a. fullCheckFlag: ���ҵ�������ʱ�Ƿ���������ж�
	b. ��������helpPointNum	���Ҹ��ڻ����helpPointNum����
	c. dx ������������dx ��dx=-1���������������
	ע��: ������c����ʱ ��ֱ���ж��÷�֧Ϊtrue
*/
template <class T>
bool JudgeInflectionPoint( std::vector<T>& yList,	// ����
							bool upFlag,			// �������������½�
							int index,				// Ҫ�ж��Ĺյ�index
							bool fullCheckFlag,		// ���ҵ�������ʱ�Ƿ���������ж�
							int helpPointNum,		// ��������
							double dx,				// ������ֵ
							int beginRangeIndex,	// �ж����߷�Χ
							int endRangeIndex )
{
	int preIndex = index;
	int lastIndex = index;

	// �ж����
	bool flag3L = (dx==-1) ? true:false;
	for( int i=0; i<helpPointNum; ++i )
	{
		// ����a
		--preIndex;
		if( preIndex<beginRangeIndex )
		{
			if( fullCheckFlag==true )
				return false;
			else
				goto _FLAG1;
		}

		// ����b
		if( upFlag ? yList[preIndex]>yList[index] : yList[preIndex]<yList[index] )
			return false;

		// ����c
		if( dx>0 && flag3L==false )
		{
			T dxTemp = abs( yList[index]-yList[preIndex] );
			if( dxTemp>=dx )
			{
				flag3L = true;
				break;
			}
		}
	}
	if( flag3L==false )
		return false;

_FLAG1:
	// �ж��ұ�
	bool flag3R = (dx==-1) ? true:false;
	for( int i=0; i<helpPointNum; ++i )
	{
		// ����a
		++lastIndex;
		if( lastIndex>endRangeIndex )
		{
			if( fullCheckFlag==true )
				return false;
			else
				goto _FLAG2;
		}

		// ����b
		if( upFlag ? ( yList[lastIndex]>yList[index] ):( yList[lastIndex]<yList[index] ) )
			return false;

		// ����c
		if( dx>0 && flag3R==false )
		{
			T dxTemp = abs( yList[index]-yList[lastIndex] );
			if( dxTemp>=dx )
			{
				flag3R = true;
				break;
			}
		}
	}
	if( flag3R==false )
		return false;

_FLAG2:
	return true;
}

template <class T>
bool JudgeInflectionPoint( std::vector<T>& yList, bool upFlag, int index, bool fullCheckFlag, int helpPointNum, double dx=-1)
{
	return JudgeInflectionPoint( yList, upFlag, index, fullCheckFlag, helpPointNum, dx, 0, yList.size() );
}


// Ѱ�ҷ�ֵ
template <class T>
int SearchPeak(	std::vector<T>& yList,				// ����
					int startIndex,					// ��ʼ�ж��ĵ�
					bool fullCheckFlag,				// �յ��ж�����
					int helpPointNum, 
					double dx, 
					T yLimit, bool yLimitFlag,		// ����Limit���: Ҫ����ĳ��ֵ�����
					int beginRangeIndex,			// �ж����߷�Χ
					int endRangeIndex )
{
	int index = startIndex;
	while( index>=beginRangeIndex && index<=endRangeIndex )
	{
		if( yLimitFlag==false || ( yLimitFlag==true && yList[index]>=yLimit ) )
		{	// �ж���ֵ�յ�
			if( JudgeInflectionPoint( yList, true, index, fullCheckFlag, helpPointNum, dx, beginRangeIndex, endRangeIndex ) )
				return index;
		}

		++index;
	}
	
	return -1;	// ������Χ Ѱ��ʧ��
}

template <class T>
int SearchPeak( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx, T yLimit=0, bool yLimitFlag=false )
{
	return SearchPeak( yList, startIndex, fullCheckFlag, helpPointNum, dx, yLimit, yLimitFlag, 0, yList.size()-1 );
}


// Ѱ�ҹ�ֵ
template <class T>
int SearchValley( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx, int beginRangeIndex, int endRangeIndex )
{
	int index = startIndex;
	while( index>=beginRangeIndex && index<=endRangeIndex )
	{
		// �ж���ֵ�յ�
		if( JudgeInflectionPoint( yList, false, index, fullCheckFlag, helpPointNum, dx, beginRangeIndex, endRangeIndex )==true )
			return index;
		++index;
	}
	
	return -1;	// ������Χ ����ʧ��
}

template <class T>
int SearchValley( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx )
{
	return SearchValley( yList, startIndex, fullCheckFlag, helpPointNum, dx, 0, yList.size() );
}



// һ���� һ���ȹ���һ������
template <class T>
struct WaveCycleData 
{
	// �� ��
	int _peakIndex;
	int _valleyIndex;

	WaveCycleData( int peakIndex_, int valleyIndex_ )
	{
		_peakIndex = peakIndex_;
		_valleyIndex = valleyIndex_;
	}

	void Assign( int peakIndex_, int valleyIndex_ )
	{
		_peakIndex = peakIndex_;
		_valleyIndex = valleyIndex_;
	}
};

// ������Ϣ�� ��¼��϶�������Ҹ�����ϵ�
template <class T>
struct FittingPointData
{
	bool _fittingFlag;
	T _xFitting;
	T _yFitting;
	
	bool _leftFlag;
	T _leftV;
	bool _rightFlag;
	T _rightV;

	FittingPointData()
	{
		_fittingFlag = false;
		_leftFlag = false;
		_rightFlag = false;

		_xFitting = 0;
		_yFitting = 0;
		_leftV = 0;
		_rightV = 0;
	}

	void PushLeftPoint( T& v )	{ _leftFlag=true; _leftV=v;	}
	void PushRightPoint( T& v )	{ _rightFlag=true; _rightV=v; }
	bool CheckBothExist()		{ return ( _leftFlag==true )&&( _rightFlag=true ); }
};

// �������η���
/*
	1. ��������
	2. ��Ϸ��ֵ
*/
template <class T>
class BaseWaveOBJ
{
public:
	BaseWaveOBJ()
	{
		_inflectionFullCheckFlag = true;
		_inflectionHelpDX = -1;
		_inflectionHelpPointNum = 5;
		_fittingHelpPointNum = 2;
		_peakYLimitFlag = false;
	}

	// ������������
	bool SetWave( std::vector<T>& xList, std::vector<T>& yList ) { return SetWave( xList, yList, 0, xList.size()-1 ); }
	bool SetWave( std::vector<T>& xList, std::vector<T>& yList, int beginRangeIndex, int endRangeIndex )
	{
		// ��֤����x y������ͬ
		if( xList.size()!=yList.size() )
			return false;

		// �������߷����ķ�Χ
		_beginRangeIndex = beginRangeIndex;
		_endRangeIndex = endRangeIndex;

		// ��������
		_pXList = &xList;
		_pYList = &yList;

		// ����ϴη������
		_cycleList.clear();

		return true;
	}

	// ������ز���
	bool SetBeginEndRange( int beginRangeIndex, int endRangeIndex )
	{
		if( beginRangeIndex<0 || endRangeIndex>(int)( *_pXList ).size() )
			return false;

		_beginRangeIndex = beginRangeIndex;
		_endRangeIndex = endRangeIndex;

		return true;
	}
	//
	void SetInflectionFullCheckFlag( bool flag ) { _inflectionFullCheckFlag = flag; }
	void SetInflectionHelpPoint( int num ) { _inflectionHelpPointNum = num; }
	void SetInflectionHelpDX( double dx ) { _inflectionHelpDX=dx; }
	//
	void SetPeakYLimit( T yLimit ) { _peakYLimit=yLimit; _peakYLimitFlag=true; }
	//
	void SetFittingHelpPoint( int num ) { _fittingHelpPointNum = num; }

	// ������ֵΪ����������Ϣ 
	void ParsePeakWaveCycle()
	{
		_cycleList.clear();

		// ���ܵ����: �� ���
		int index = _beginRangeIndex;
		while( index>=_beginRangeIndex && index<=_endRangeIndex  )
		{
			int peakIndex = SearchPeak( index );											// Ѱ�ҷ�ֵ
			if( peakIndex==-1 ) return;

			int valleyIndex = SearchValley( peakIndex+1 );									// Ѱ�ҹȵ�	
			_cycleList.push_back( WaveCycleData<T>( peakIndex, valleyIndex ) );				// �����������Ϣ

			if( valleyIndex==-1 ) return;
			index = valleyIndex+1;															// ��һ��ѭ��׼��
		}	
	}

	void ParseFittingPeakValley( T centerWaveWidth, std::vector<FittingPointData<T> > *pPeakInfoList=NULL, 
								 std::vector<FittingPointData<T> > *pValleyInfoList=NULL )
	{
		_fittingCycleIndexList.clear();
		for( UINT i=0; i<_cycleList.size(); ++i )
			_fittingCycleIndexList.push_back( i );

		_ParseFittingPeakValley( centerWaveWidth, pPeakInfoList, pValleyInfoList );
	}

	// ���������������Ϣ	
	std::vector<WaveCycleData<T> > _cycleList;	

	// ����  Ĭ��Ϊ���ȶ������������ߵ㼯��  ���м��жϵ㣬��ÿ���ڷ���ʱͨ�������������
	std::vector<T>* _pXList;
	std::vector<T>* _pYList;

	// ������Ҫ�����Ķ����� ���ұպ�[beginIndex, endIndex]
	int _beginRangeIndex;
	int _endRangeIndex;

protected:
	// ����� ��
	inline int SearchPeak( int startIndex )
	{
		return ::SearchPeak( *_pYList, startIndex,  _inflectionFullCheckFlag, _inflectionHelpPointNum, _inflectionHelpDX, 
							 _peakYLimit, _peakYLimitFlag, _beginRangeIndex, _endRangeIndex );
	}
	inline int SearchValley( int startIndex )
	{
		return ::SearchValley( *_pYList, startIndex,  _inflectionFullCheckFlag, _inflectionHelpPointNum, _inflectionHelpDX, _beginRangeIndex, _endRangeIndex );
	}

	// ������϶���
	// ���ֱ��
	bool FittingLine( std::vector<int>& pointIndexList, double& k, double& b ) { return ::FittingLine( *_pXList, *_pYList, pointIndexList, k, b ); }

	// ��϶��㸨����xֵ( ���Ҹ�һ�� )
	bool  FittingVertexHelpPointXCoord( int cycleIndex, int vertexNO, bool leftFlag, T& fittingPointX )
	{
		// ��ؼ��  ������ڵ��ж������ϲ���ú������
		if( vertexNO!=1 && vertexNO!=2 )
			return false;

		// ��ȡ�����Լ�����
		std::vector<T>& xList = *_pXList;
		std::vector<T>& yList = *_pYList;
		WaveCycleData<T>& cycle = _cycleList[cycleIndex];
		//
		if( cycle._peakIndex==-1 || cycle._valleyIndex==-1 )
			return false;

		// ��������ҵ�����
		int vertexIndex;
		T limitValue;
		if( vertexNO==1 )
		{
			vertexIndex = cycle._peakIndex;
			limitValue = yList[vertexIndex] - _centerWaveWidth;
			if( limitValue<yList[cycle._valleyIndex] )
				return false;
		}
		else if( vertexNO==2 )
		{
			vertexIndex = cycle._valleyIndex;
			limitValue = yList[vertexIndex] + _centerWaveWidth;
			if( limitValue>yList[cycle._peakIndex] )
				return false;
		}

		// Ѱ�ҵ�һ���������Ĳ��εļ��޵�
		int index = vertexIndex;
		while( 1 )
		{
			if( leftFlag )
				--index;
			else
				++index;
			// ������Χ
			if( index<_beginRangeIndex || index>_endRangeIndex )
				return false;
			int dx = static_cast<int>( ( cycle._valleyIndex-cycle._peakIndex )*3.5 );               // ????????����ֵ
			if( index<vertexIndex-dx || index>vertexIndex+dx )
				return false;

			bool flag = vertexNO==1 ? yList[index]<limitValue : yList[index]>limitValue;
			if( flag )
				break;
		}

		// ���������ϵĵ㼯��
		if( leftFlag )
			index -= _fittingHelpPointNum/2-1;
		else 
			index += _fittingHelpPointNum/2-1;
		// 
		std::vector<int> pointIndexList;
		for( int j=0; j<_fittingHelpPointNum; ++j )
		{
			if( index>=_beginRangeIndex && index<=_endRangeIndex )
				pointIndexList.push_back( index );

			if( leftFlag )
				++index;
			else
				--index;
		}

		// �����������k bֵ
		double k=0, b=0;
		if( FittingLine( pointIndexList, k, b )==false )
			return false; 
		if( k==0 )
			return false;
		//
		fittingPointX = static_cast<T>( ( limitValue-b )/k );
		return true;
	}

	// ������϶����y����
	T FittingVertexYCoord( int cycleIndex, int vertexNO, T vertexFittingXCoord )
	{
		std::vector<T>& xList = *_pXList;
		std::vector<T>& yList = *_pYList;
		WaveCycleData<T>& cycle = _cycleList[cycleIndex];

		// ��ö�����Ϣ
		int vertexIndex;
		if( vertexNO==1 )
			vertexIndex = cycle._peakIndex;
		else if( vertexNO==2 )
			vertexIndex = cycle._valleyIndex;

		if( vertexFittingXCoord==xList[vertexIndex] )	// ���Ķ�����ɼ������غ� 
			return yList[vertexIndex];

		// ���������ϵĵ㼯��
		std::vector<int> pointList;
		pointList.push_back( vertexIndex );
		if( vertexFittingXCoord<xList[vertexIndex] )	// ���Ķ����ڲɼ���������
		{  
			int index = vertexIndex;
			while( vertexFittingXCoord<xList[--index] )
				pointList.push_back( index );
			int num = vertexIndex-index;
			for( int j=0; j<num; ++j )
				pointList.push_back( index-- );
		}
		else	// ���Ķ����ڲɼ�������ұ�
		{	
			int index = vertexIndex;
			while( vertexFittingXCoord>xList[++index] )
				pointList.push_back( index );
			int num = index-vertexIndex;
			for( int j=0; j<num; ++j )
				pointList.push_back( index++ );
		}
		// 
		double k, b;
		FittingLine( pointList, k, b );
		return static_cast<T>( k*vertexFittingXCoord+b );
	}

		// ��϶�����Ϣ
	void _ParseFittingPeakValley( T centerWaveWidth, std::vector<FittingPointData<T> > *pPeakInfoList, 
								  std::vector<FittingPointData<T> > *pValleyInfoList )
	{
		// ��֤������һ������
		if( _fittingCycleIndexList.size()==0 )
			return;

		// ���
		if( centerWaveWidth<0 )
			return;
		_centerWaveWidth = centerWaveWidth;

		// 
		T leftFittingPointX, rightFittingPointX;

		// ���������ڽ��д���   ���ܵ����: �� ��� ��
		if( pPeakInfoList!=NULL )
			pPeakInfoList->clear();
		if( pValleyInfoList!=NULL )
			pValleyInfoList->clear();
		// 
		for( UINT i=0; i<_fittingCycleIndexList.size(); ++i )
		{
			if( _fittingCycleIndexList[i]==-1 )
			{
				if( pPeakInfoList!=NULL )
					pPeakInfoList->resize( pPeakInfoList->size()+1 );

				if( pValleyInfoList!=NULL )
					pValleyInfoList->resize( pValleyInfoList->size()+1 );

				continue;
			}

			if( pPeakInfoList!=NULL )
			{
				pPeakInfoList->resize( pPeakInfoList->size()+1 );
		
				if( FittingVertexHelpPointXCoord( _fittingCycleIndexList[i], 1, true, leftFittingPointX )==true )
				{
					pPeakInfoList->back().PushLeftPoint( leftFittingPointX );

					if( FittingVertexHelpPointXCoord( _fittingCycleIndexList[i], 1, false, rightFittingPointX )==true )
					{
						pPeakInfoList->back().PushRightPoint( rightFittingPointX );

						// 
						pPeakInfoList->back()._xFitting = ( leftFittingPointX+rightFittingPointX )/2;
						pPeakInfoList->back()._yFitting = FittingVertexYCoord( _fittingCycleIndexList[i], 1, pPeakInfoList->back()._xFitting );
						pPeakInfoList->back()._fittingFlag = true;
					}
				}
			}

			if( pValleyInfoList!=NULL )
			{
				pValleyInfoList->resize( pValleyInfoList->size()+1 );

				if( FittingVertexHelpPointXCoord( _fittingCycleIndexList[i], 2, true, leftFittingPointX )==true )
				{
					pValleyInfoList->back().PushLeftPoint( leftFittingPointX );

					if( FittingVertexHelpPointXCoord( _fittingCycleIndexList[i], 2, false, rightFittingPointX )==true )
					{
						pValleyInfoList->back().PushRightPoint( rightFittingPointX );

						// 
						pValleyInfoList->back()._xFitting = ( leftFittingPointX+rightFittingPointX )/2;
						pValleyInfoList->back()._yFitting = FittingVertexYCoord( _fittingCycleIndexList[i], 2, pValleyInfoList->back()._xFitting );
						pValleyInfoList->back()._fittingFlag = true;
					}
				}
			}
		}
	}

	// �յ��ж�
	bool _inflectionFullCheckFlag;
	int _inflectionHelpPointNum;
	double _inflectionHelpDX;

	// ��ֵLimit
	T _peakYLimit;
	bool _peakYLimitFlag;

	// ������ϸ�������
	int _fittingHelpPointNum;	

	// ���Ĳ�������
	T _centerWaveWidth;									

	// Ҫ������ϵ�cycle��indexList
	std::vector<int> _fittingCycleIndexList;
};





// ����������Ϣ
template <class T>
struct LaserWaveCycleData
{
	int _cycleIndex;		// ����index
	T _ituWave;				// ITU���Ĳ���

	// ����Χ
	int _rBeginIndex;
	int _rEndIndex;

	LaserWaveCycleData()
	{
		_cycleIndex = -1;
		_ituWave = 0;

		_rBeginIndex = -1;
		_rEndIndex = -1;
	}
};

// ���Ⲩ�η���
/*
	1. ����ITU������Ϣ��������
	2. ��ITU���ڽ������
*/
template <class T>
class LaserWaveOBJ : public BaseWaveOBJ<T>
{
public: 
	std::vector<LaserWaveCycleData<T> > _filterCycleList;	// filter���������Ϣ

public:
	// ����
	LaserWaveOBJ() { _centerWaveWidth = 3; }

	// ͨ��ITU��������Ҫ����������
	void FilterCycleList( std::vector<T>& ITUList, T dxITU )
	{
		_filterCycleList.clear();

		std::vector<T>& xList = *_pXList;
		std::vector<T>& yList = *_pYList;

		sort( ITUList.begin(), ITUList.end() );

		for( UINT i=0; i<ITUList.size(); ++i )
		{
			LaserWaveCycleData<T> filterWaveCycle;
			filterWaveCycle._cycleIndex = -1;
			filterWaveCycle._ituWave = ITUList[i];
			// 
			_filterCycleList.push_back( filterWaveCycle );

			for( UINT j=0; j<_cycleList.size(); ++j )
			{
				WaveCycleData<T>& waveCycle = _cycleList[j];
				if( waveCycle._peakIndex==-1 )
					continue;

				T dx = abs( xList[waveCycle._peakIndex]-ITUList[i] );
				if( dx<dxITU )
				{
					_filterCycleList.back()._cycleIndex = j;
					break;
				}
			}
		}
	}

	// ��Ҫ���������ڼ���itu������clear pass band����Χ
	void ParseFilterCycleRangeIndex( T range )
	{
		for( UINT i=0; i<_filterCycleList.size(); ++i )
		{
			if( _filterCycleList[i]._cycleIndex==-1 )
				continue;

			if( _cycleList[_filterCycleList[i]._cycleIndex]._peakIndex==-1 )
				continue;

			double rBeginWaveLen = _filterCycleList[i]._ituWave-range;
			double rEndWaveLen = _filterCycleList[i]._ituWave+range;

			int& rBeginIndex = _filterCycleList[i]._rBeginIndex;
			int& rEndIndex = _filterCycleList[i]._rEndIndex;
			rBeginIndex = _cycleList[_filterCycleList[i]._cycleIndex]._peakIndex;
			rEndIndex = rBeginIndex;

			//
			if( (*_pXList)[rBeginIndex]<rBeginWaveLen )
			{
				while( rBeginIndex<(int)(*_pXList).size() && (*_pXList)[rBeginIndex]<rBeginWaveLen ) 
					++rBeginIndex;
			}
			else
			{
				while( rBeginIndex>0 && (*_pXList)[rBeginIndex]>rBeginWaveLen ) 
					--rBeginIndex;
				++rBeginIndex;
			}
			
			//
			if( (*_pXList)[rEndIndex]>rEndWaveLen )
			{
				while( rEndIndex>0 && (*_pXList)[rEndIndex]>rEndWaveLen )
					--rEndIndex;
			}
			else
			{
				while( rEndIndex<(int)(*_pXList).size() && (*_pXList)[rEndIndex]<rEndWaveLen )
					++rEndIndex;
				--rEndIndex;
			}
		}
	}

	void ParseFilterFittingPeakValley(	T centerWaveWidth, std::vector<FittingPointData<T> > *pPeakInfoList=NULL, 
										std::vector<FittingPointData<T> > *pValleyInfoList=NULL )
	{
		_fittingCycleIndexList.clear();
		for( UINT i=0; i<_filterCycleList.size(); ++i )
			_fittingCycleIndexList.push_back( _filterCycleList[i]._cycleIndex );

		_ParseFittingPeakValley( centerWaveWidth, pPeakInfoList, pValleyInfoList );
	}
};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      