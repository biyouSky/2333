/*
	Add:	2011-5-31
	Update:	2011-6-22

	基础数据分析库
*/

#pragma once
#include "..\Utility\GXUtil.h"

#define LIGHT_SPEED		299792458		// m/s


// 一阶拟合直线
// 求几点拟合直线的k b值
// 每相邻2点求一组k b值 最后平均
template <class TX, class TY>
bool FittingLine( std::vector<TX>& xList, std::vector<TY>& yList, std::vector<int>& pointIndexList, double& k, double& b )
{
	if( xList.size()!=yList.size() )
	{
		//gDebugTrace.PushMsg( "Function: FittingLine()	err: xList.size()!=yList.size()" );
		return false;
	}

	if( pointIndexList.size()<2 )
	{
		//gDebugTrace.PushMsg( "Function: FittingLine()	err: pointIndexList.size()<2" );
		return false;
	}

	k = 0;
	b = 0;
	int pairNum = pointIndexList.size()-1;
	for( UINT i=0; i<pointIndexList.size()-1; ++i )
	{
		int index = pointIndexList[i];
		int index2 = pointIndexList[i+1];

		if( xList[index]==xList[index2] )
		{
			--pairNum;
			if( pairNum==0 )
				return false;
			continue;
		}

		double kTemp = static_cast<double>( yList[index]-yList[index2] )/( xList[index]-xList[index2] );
		double bTemp = static_cast<double>( yList[index]- kTemp*xList[index] );
		k += kTemp;
		b += bTemp;
	}

	k = k/pairNum;
	b = b/pairNum;

	return true;
}

template <class T>
bool FittingLine( std::vector<T>& xList, std::vector<T>& yList, std::vector<int>& pointIndexList, double& k, double& b )
{
	return FittingLine<T, T>( xList, yList, pointIndexList, k, b );
}



// 最小2乘法拟合 计算k b 线形拟合度
// 输出 k=result[0]; b=result[1]; 拟合度=result[2];
inline void LeastSquares_FittingLine( float *X, float *Y, char num, float *result )
{
	unsigned char fiti;
	float X11, Y11, Z11, X22, Y22, Z22, aveX, aveY;
	float addX=0, addY=0, addXY=0;

	X11=num;
	Y11=0;
	Z11=0;
	X22=0;
	Y22=0;
	Z22=0;

	for( fiti=0; fiti<num; fiti++ )
	{
		Y11 += X[fiti];
		Z11 += Y[fiti];
		Y22 += X[fiti]*X[fiti];
		Z22 += X[fiti]*Y[fiti];
	}
	X22=Y11;

	result[0] = ( Z11*X22-Z22*X11 )/( Y11*X22-Y22*X11 );
	result[1] = -( Z11*Y22-Z22*Y11 )/( X11*Y22-X22*Y11 );

	aveX = Y11/num;
	aveY = Z11/num;

	for( fiti=0; fiti<num; fiti++ )
	{
		addXY += ( X[fiti]-aveX )*( Y[fiti]-aveY );
		addX += ( X[fiti]-aveX )*( X[fiti]-aveX );
		addY += ( Y[fiti]-aveY )*( Y[fiti]-aveY );
	}

	result[2] = addXY/sqrt( addX*addY );
}


// 单值大小比较
template <class T>		
struct Compare_Value// : public binary_function<T, T, bool>
{
	bool lessFlag;
	Compare_Value( bool flag=true ) { lessFlag=flag; }

	bool operator()( const T& f, const T& x ) const
	{
		return lessFlag==true ? ( f<x ):( f>x );
	}
};

// pair点y值比较
template<class TX, class TY>
struct Compare_PointY
{
	bool lessFlag;
	Compare_PointY( bool flag ) 
	{ 
		lessFlag=flag; 
	}

	typedef std::pair<TX, TY> POINT_TYPE;
	bool operator ()( POINT_TYPE& p1, POINT_TYPE& p2 )
	{
		return lessFlag==true ? ( p1.second<p2.second ):( p1.second>p2.second );
	}
};



// 拟合曲线  y值按相应条件差等分后 计算相应的x值
template<class TX, class TY>
bool SortFitting_Curve( std::vector<TX>& curveX, std::vector<TY>& curveY,					// 原曲线
						std::vector<TX>& newCurveX, std::vector<TY>& newCurveY,				// 插值拟合后的新曲线
						TY peakValue, TY stepValue, TY widthValue, bool upFlag )			// 相关插值拟合参数
{
	struct FilterRepeat
	{
		void operator()( std::vector<TX>& newCurveX, TX no_value )
		{
			int newCurveSize = (int)newCurveX.size();
			for( int i=0; i<newCurveSize-1; ++i )
			{
				int start1=0, end1=0; // 计算范围
				int start2=0, end2=0; // 重复范围	

				if( newCurveX[i]==no_value || newCurveX[i+1]==no_value )
					continue;

				if( newCurveX[i]!=newCurveX[i+1] )
					continue;

				// 找到重复范围
				start2 = i;
				end2 = i+1;
				int index = i+1;
				while( newCurveX[start2]==newCurveX[index] )
				{
					end2 = index;
					++index;
					if( index>=newCurveSize )
						break;
				}

				// 计算计算范围
				start1 = start2; 
				index = start1-1;
				while( index>=0 )
				{
					if( newCurveX[index]!=no_value )
					{
						start1 = index;
						break;
					}
					--index;
				}
				//
				end1 = end2;
				index = end1+1;
				while( index<newCurveSize )
				{
					if( newCurveX[index]!=no_value )
					{
						end1 = index;
						break;
					}
					++index;
				}
				//
				if( end1==end2 )
					continue;

				TX dx = ( newCurveX[end1]-newCurveX[start1] )/( end1-start1 );
				for( int j=0; j<end2-start2+1; ++j )
					newCurveX[start2+j] = newCurveX[start1]+(start2+j-start1)*dx;
			}
		}
	};

	typedef std::pair<TX, TY> POINT_TYPE;
	typedef std::vector<POINT_TYPE > CURVE_TYPE;

	// 有一定的危险性			
	TX no_value = -9999;

	// 构造原始曲线以方便按upFlag进行排序 排序后x可能无序 会将畸变点的误差引入
	CURVE_TYPE curve;
	for( UINT i=0; i<curveX.size(); ++i )
		curve.push_back( POINT_TYPE( curveX[i], curveY[i] ) );
	// 
	sort( curve.begin(), curve.end(), Compare_PointY<TX, TY>( upFlag ) );
	
	int calcTimes = (int)(widthValue/stepValue) + 1;				// 计算循环次数 初始化新曲线
	TY startValue = upFlag ? ( peakValue-widthValue ):peakValue;	// 计算起始值

	// 初步插值
	for( int i=0; i<calcTimes; ++i )
	{
		TY curValue = upFlag ? ( startValue+i*stepValue ):( startValue-i*stepValue );

		newCurveY.push_back( curValue );
		newCurveX.push_back( no_value );

		// 计算范围  [, ]
		TY rangeValue1 = 0;
		TY rangeValue2 = 0;
		TY helpRange = stepValue;
		int tryCount = 0;
_FLAG1:
		rangeValue1 = upFlag ? (curValue-helpRange):(curValue+helpRange);
		rangeValue2 = upFlag ? (curValue+helpRange):(curValue-helpRange);

		std::vector<TX> rangeXList;
		int pointIndex = 0;
		while( Compare_Value<TY>( upFlag )( curve[pointIndex].second, rangeValue1 ) )
		{	// 进入range
			++pointIndex;

			if( (UINT)pointIndex>=curve.size() )
				goto _FLAG3;
		}	
		while( Compare_Value<TY>( upFlag )( curve[pointIndex].second, rangeValue2 ) )
		{	// 出range
			rangeXList.push_back( curveX[pointIndex] );
			++pointIndex;

			if( (UINT)pointIndex>=curve.size() )
				goto _FLAG2;
		}

_FLAG2:
		if( rangeXList.size()!=0 )
		{
			newCurveX[i] = accumulate( rangeXList.begin(), rangeXList.end(), 0 )/rangeXList.size();
		}
		else
		{
			if( ++tryCount<4 )
			{
				helpRange += stepValue;
				goto _FLAG1;
			}
		}
	}

_FLAG3:
	// 检查一遍 不存在的值根据附近的点进行kb拟合
	int newCurveSize = (int)newCurveX.size();
	for( int i=0; i<newCurveSize; ++i )
	{
		if( newCurveX[i]==no_value )
		{
			// 计算左右环绕的10个点
			int tempList[10];
			memset( tempList, -1, sizeof(int)*10 );
			//
			for( int j=i-5; j<i; ++j )
			{
				if( j>=0 && newCurveX[j]!=no_value )
					tempList[j-i+5] = j;
			}
			for( int j=i+1; j<=i+5; ++j )
			{
				if( j<newCurveSize && newCurveX[j]!=no_value )
					tempList[j-i+4] = j;
			}

			// 尝试前后插值
			std::vector<int> pointIndexList;
			if( tempList[4]!=-1 && tempList[5]!=-1 )
			{
				pointIndexList.push_back( tempList[4] );
				pointIndexList.push_back( tempList[5] );
			}
			else if( tempList[3]!=-1 && tempList[6]!=-1 )
			{
				pointIndexList.push_back( tempList[3] );
				pointIndexList.push_back( tempList[6] );
			}	
			// 尝试其他
			if( pointIndexList.size()==0 )
			{
				for( int j=0; j<10; ++j )
				{
					if( tempList[j]!=-1 )
						pointIndexList.push_back( tempList[j] );
				}
			}

			if( pointIndexList.size()<2 )
			{
				//gDebugTrace.PushMsg( "Function: SortFittingCurve()	err: around points num <2" );
				return false;
			}

			double k, b;
			if( FittingLine( newCurveX, newCurveY, pointIndexList, k, b )==false )
				return false;
			newCurveX[i] = static_cast<TX>( ( newCurveY[i]-b )/k );
		}
	}

	// 检查插值相同的值 重新计算
	FilterRepeat()( newCurveX, no_value );

	return true;
}






// ripple: 一定波长范围内的最大值最小值之差
template<class TX, class TY>
bool CalcRipple( std::vector<TX>& curveX, std::vector<TY>& curveY, TX point, TX dx, TY& ripple )
{
	// 计算范围
	TX leftX, rightX;
	leftX = point-dx;
	rightX = point+dx;

	// 检查范围是否存在
	if( curveX.front()>leftX ) return false;
	if( curveX.back()<rightX ) return false;
	// 计算范围index
	int startIndex = 0;
	while( curveX[startIndex]<leftX ) ++startIndex;
	--startIndex;
	int endIndex = startIndex;
	while( curveX[endIndex]<rightX ) ++endIndex;

	// 
	TY min = *min_element( curveY.begin()+startIndex, curveY.begin()+endIndex );
	TY max = *max_element( curveY.begin()+startIndex, curveY.begin()+endIndex );

	ripple = max-min;
	return true;
}
