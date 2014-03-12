/*
	Add:	2011-5-31
	Update:	2011-6-2

	波形数据分析库, 波形分析基于拐点理论
	
	BaseWaveOBJ: 基础波形数据分析
		||
		\/
	LaserWaveOBJ: 对应ITU波形数据分析

	ITUDataParseOBJ: 只通过ITU以及带宽定义分析数据　暂未实现
*/

#pragma once
#include "GXBaseMath.h"

/*
    拐点判定:
	
	波形分析: 基于拐点判定, 普遍意义上的波形周期计算, 增加峰值过滤

	光波形分析: 基于波形分析, 增加itu波长及峰值判定过滤出要分析的真实周期, 并增加带宽计算算法
*/


/*
	判断拐点, 左右进行判定
	条件:
	a. fullCheckFlag: 左右点数不够时是否继续进行判定
	b. 辅助点数helpPointNum	左右高于或低于helpPointNum个点
	c. dx 左右需相差大于dx 若dx=-1则该条件不起作用
	注意: 当条件c满足时 可直接判定该分支为true
*/
template <class T>
bool JudgeInflectionPoint( std::vector<T>& yList,	// 数据
							bool upFlag,			// 曲线上升还是下降
							int index,				// 要判定的拐点index
							bool fullCheckFlag,		// 左右点数不够时是否继续进行判定
							int helpPointNum,		// 辅助点数
							double dx,				// 辅助差值
							int beginRangeIndex,	// 判定曲线范围
							int endRangeIndex )
{
	int preIndex = index;
	int lastIndex = index;

	// 判断左边
	bool flag3L = (dx==-1) ? true:false;
	for( int i=0; i<helpPointNum; ++i )
	{
		// 条件a
		--preIndex;
		if( preIndex<beginRangeIndex )
		{
			if( fullCheckFlag==true )
				return false;
			else
				goto _FLAG1;
		}

		// 条件b
		if( upFlag ? yList[preIndex]>yList[index] : yList[preIndex]<yList[index] )
			return false;

		// 条件c
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
	// 判断右边
	bool flag3R = (dx==-1) ? true:false;
	for( int i=0; i<helpPointNum; ++i )
	{
		// 条件a
		++lastIndex;
		if( lastIndex>endRangeIndex )
		{
			if( fullCheckFlag==true )
				return false;
			else
				goto _FLAG2;
		}

		// 条件b
		if( upFlag ? ( yList[lastIndex]>yList[index] ):( yList[lastIndex]<yList[index] ) )
			return false;

		// 条件c
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


// 寻找峰值
template <class T>
int SearchPeak(	std::vector<T>& yList,				// 数据
					int startIndex,					// 开始判定的点
					bool fullCheckFlag,				// 拐点判定条件
					int helpPointNum, 
					double dx, 
					T yLimit, bool yLimitFlag,		// 顶点Limit检查: 要高于某个值才算峰
					int beginRangeIndex,			// 判定曲线范围
					int endRangeIndex )
{
	int index = startIndex;
	while( index>=beginRangeIndex && index<=endRangeIndex )
	{
		if( yLimitFlag==false || ( yLimitFlag==true && yList[index]>=yLimit ) )
		{	// 判定峰值拐点
			if( JudgeInflectionPoint( yList, true, index, fullCheckFlag, helpPointNum, dx, beginRangeIndex, endRangeIndex ) )
				return index;
		}

		++index;
	}
	
	return -1;	// 超出范围 寻找失败
}

template <class T>
int SearchPeak( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx, T yLimit=0, bool yLimitFlag=false )
{
	return SearchPeak( yList, startIndex, fullCheckFlag, helpPointNum, dx, yLimit, yLimitFlag, 0, yList.size()-1 );
}


// 寻找谷值
template <class T>
int SearchValley( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx, int beginRangeIndex, int endRangeIndex )
{
	int index = startIndex;
	while( index>=beginRangeIndex && index<=endRangeIndex )
	{
		// 判定谷值拐点
		if( JudgeInflectionPoint( yList, false, index, fullCheckFlag, helpPointNum, dx, beginRangeIndex, endRangeIndex )==true )
			return index;
		++index;
	}
	
	return -1;	// 超出范围 查找失败
}

template <class T>
int SearchValley( std::vector<T>& yList, int startIndex, bool fullCheckFlag, int helpPointNum, double dx )
{
	return SearchValley( yList, startIndex, fullCheckFlag, helpPointNum, dx, 0, yList.size() );
}



// 一个峰 一个谷构成一个周期
template <class T>
struct WaveCycleData 
{
	// 峰 谷
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

// 辅助信息类 记录拟合顶点的左右辅助拟合点
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

// 基础波形分析
/*
	1. 分析周期
	2. 拟合峰谷值
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

	// 设置曲线数据
	bool SetWave( std::vector<T>& xList, std::vector<T>& yList ) { return SetWave( xList, yList, 0, xList.size()-1 ); }
	bool SetWave( std::vector<T>& xList, std::vector<T>& yList, int beginRangeIndex, int endRangeIndex )
	{
		// 验证曲线x y点数相同
		if( xList.size()!=yList.size() )
			return false;

		// 保存曲线分析的范围
		_beginRangeIndex = beginRangeIndex;
		_endRangeIndex = endRangeIndex;

		// 保存曲线
		_pXList = &xList;
		_pYList = &yList;

		// 清除上次分析结果
		_cycleList.clear();

		return true;
	}

	// 设置相关参数
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

	// 分析峰值为主的周期信息 
	void ParsePeakWaveCycle()
	{
		_cycleList.clear();

		// 可能的组合: 峰 峰谷
		int index = _beginRangeIndex;
		while( index>=_beginRangeIndex && index<=_endRangeIndex  )
		{
			int peakIndex = SearchPeak( index );											// 寻找峰值
			if( peakIndex==-1 ) return;

			int valleyIndex = SearchValley( peakIndex+1 );									// 寻找谷底	
			_cycleList.push_back( WaveCycleData<T>( peakIndex, valleyIndex ) );				// 保存该周期信息

			if( valleyIndex==-1 ) return;
			index = valleyIndex+1;															// 下一次循环准备
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

	// 保存分析的周期信息	
	std::vector<WaveCycleData<T> > _cycleList;	

	// 曲线  默认为较稳定且连续的曲线点集合  若中间有断点，则每段在分析时通过设置区域隔开
	std::vector<T>* _pXList;
	std::vector<T>* _pYList;

	// 曲线需要分析的段区域 左右闭合[beginIndex, endIndex]
	int _beginRangeIndex;
	int _endRangeIndex;

protected:
	// 计算峰 谷
	inline int SearchPeak( int startIndex )
	{
		return ::SearchPeak( *_pYList, startIndex,  _inflectionFullCheckFlag, _inflectionHelpPointNum, _inflectionHelpDX, 
							 _peakYLimit, _peakYLimitFlag, _beginRangeIndex, _endRangeIndex );
	}
	inline int SearchValley( int startIndex )
	{
		return ::SearchValley( *_pYList, startIndex,  _inflectionFullCheckFlag, _inflectionHelpPointNum, _inflectionHelpDX, _beginRangeIndex, _endRangeIndex );
	}

	// 计算拟合顶点
	// 拟合直线
	bool FittingLine( std::vector<int>& pointIndexList, double& k, double& b ) { return ::FittingLine( *_pXList, *_pYList, pointIndexList, k, b ); }

	// 拟合顶点辅助点x值( 左右各一个 )
	bool  FittingVertexHelpPointXCoord( int cycleIndex, int vertexNO, bool leftFlag, T& fittingPointX )
	{
		// 相关检查  顶点存在的判定交给上层调用函数完成
		if( vertexNO!=1 && vertexNO!=2 )
			return false;

		// 获取数据以及周期
		std::vector<T>& xList = *_pXList;
		std::vector<T>& yList = *_pYList;
		WaveCycleData<T>& cycle = _cycleList[cycleIndex];
		//
		if( cycle._peakIndex==-1 || cycle._valleyIndex==-1 )
			return false;

		// 根据序号找到顶点
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

		// 寻找第一个超过中心波段的极限点
		int index = vertexIndex;
		while( 1 )
		{
			if( leftFlag )
				--index;
			else
				++index;
			// 超过范围
			if( index<_beginRangeIndex || index>_endRangeIndex )
				return false;
			int dx = static_cast<int>( ( cycle._valleyIndex-cycle._peakIndex )*3.5 );               // ????????经验值
			if( index<vertexIndex-dx || index>vertexIndex+dx )
				return false;

			bool flag = vertexNO==1 ? yList[index]<limitValue : yList[index]>limitValue;
			if( flag )
				break;
		}

		// 计算参与拟合的点集合
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

		// 计算拟合曲线k b值
		double k=0, b=0;
		if( FittingLine( pointIndexList, k, b )==false )
			return false; 
		if( k==0 )
			return false;
		//
		fittingPointX = static_cast<T>( ( limitValue-b )/k );
		return true;
	}

	// 计算拟合顶点的y坐标
	T FittingVertexYCoord( int cycleIndex, int vertexNO, T vertexFittingXCoord )
	{
		std::vector<T>& xList = *_pXList;
		std::vector<T>& yList = *_pYList;
		WaveCycleData<T>& cycle = _cycleList[cycleIndex];

		// 获得顶点信息
		int vertexIndex;
		if( vertexNO==1 )
			vertexIndex = cycle._peakIndex;
		else if( vertexNO==2 )
			vertexIndex = cycle._valleyIndex;

		if( vertexFittingXCoord==xList[vertexIndex] )	// 中心顶点与采集顶点重合 
			return yList[vertexIndex];

		// 计算参与拟合的点集合
		std::vector<int> pointList;
		pointList.push_back( vertexIndex );
		if( vertexFittingXCoord<xList[vertexIndex] )	// 中心顶点在采集顶点的左边
		{  
			int index = vertexIndex;
			while( vertexFittingXCoord<xList[--index] )
				pointList.push_back( index );
			int num = vertexIndex-index;
			for( int j=0; j<num; ++j )
				pointList.push_back( index-- );
		}
		else	// 中心顶点在采集顶点的右边
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

		// 拟合顶点信息
	void _ParseFittingPeakValley( T centerWaveWidth, std::vector<FittingPointData<T> > *pPeakInfoList, 
								  std::vector<FittingPointData<T> > *pValleyInfoList )
	{
		// 保证至少有一个周期
		if( _fittingCycleIndexList.size()==0 )
			return;

		// 检查
		if( centerWaveWidth<0 )
			return;
		_centerWaveWidth = centerWaveWidth;

		// 
		T leftFittingPointX, rightFittingPointX;

		// 对所有周期进行处理   可能的组合: 峰 峰谷 谷
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

	// 拐点判定
	bool _inflectionFullCheckFlag;
	int _inflectionHelpPointNum;
	double _inflectionHelpDX;

	// 峰值Limit
	T _peakYLimit;
	bool _peakYLimitFlag;

	// 曲线拟合辅助点数
	int _fittingHelpPointNum;	

	// 中心波长带宽
	T _centerWaveWidth;									

	// 要进行拟合的cycle的indexList
	std::vector<int> _fittingCycleIndexList;
};





// 激光周期信息
template <class T>
struct LaserWaveCycleData
{
	int _cycleIndex;		// 周期index
	T _ituWave;				// ITU中心波长

	// 带宽范围
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

// 激光波形分析
/*
	1. 根据ITU波长信息过滤周期
	2. 对ITU周期进行拟合
*/
template <class T>
class LaserWaveOBJ : public BaseWaveOBJ<T>
{
public: 
	std::vector<LaserWaveCycleData<T> > _filterCycleList;	// filter后的周期信息

public:
	// 构造
	LaserWaveOBJ() { _centerWaveWidth = 3; }

	// 通过ITU波长过滤要分析的周期
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

	// 对要分析的周期计算itu波长的clear pass band带宽范围
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