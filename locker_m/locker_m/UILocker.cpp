#include "stdafx.h"
#include "UILocker.h"
using namespace std;

UILocker gUILocker;
LLogic gLLogic;

void ShowMsg( string msg )
{
	UIWinShell::_pWinShell->ShowPlugINMsg( msg );
	UIRefresh();
}



void UILocker::OnCreate()
{
	_tabX0.Create( 0, &_uiContainer, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX0.HideTab();
	_tabX0.SetCellNum( 3 );
	_tabX0.SetCell( 0, "0", &_canvas101 );
	_tabX0.SetCell( 1, "1", &_canvas201 );
	_tabX0.SetCell( 2, "2", &_tabY301 );

	{
		_canvas101.CreateDirect( &_tabX0, GenRect()( 0, 0, 1000, 1000 ) );

		_lable104.Create( 0, &_canvas101 );
		_lable104.SetString( "数据初始化相关: " );
		_lable104.SetColor( _BLUE_ );
		_checkBut101.Create( 0, &_canvas101 );
		_checkBut101.SetString( "是否将导入报表拷贝为当天报表??" );

		_lable101.Create( 0, &_canvas101 );
		_lable101.SetString( "直接选报表文件载入: " );
		_lable101.SetColor( _BLUE_ );
		_but101.Create( 1, &_canvas101 );
		_but101.SetString( "导入报表" );

		_lable102.Create( 0, &_canvas101 );
		_lable102.SetString( "报表目录相关: " );
		_lable102.SetColor( _BLUE_ );
		_edit101.Create( 0, &_canvas101 );
		_but102.Create( 11, &_canvas101 );
		_but102.SetString( "重选目录" );
		_but103.Create( 12, &_canvas101 );
		_but103.SetString( "默认目录" );
		_grid101.Create( 0, &_canvas101 );
		_grid101.SetRowColumn( 1, 1 );
		_grid101.SetColumnWidth( 0, 200 );
		_grid101.SetRowFix();
		_grid101.SetCellString( 0, 0, "文件名列表" );
		_but104.Create( 13, &_canvas101 );
		_but104.SetString( "导入选中报表" );

		_lable103.Create( 0, &_canvas101 );
		_lable103.SetString( "自动生成缺失日期报表功能(只限默认目录下): " );
		_lable103.SetColor( _BLUE_ );
		_lable105.Create( 0, &_canvas101 );
		_lable105.SetString( "规则:寻找最近日期报表作为源文件进行拷贝." );
		_lable106.Create( 0, &_canvas101 );
		_lable107.Create( 0, &_canvas101 );
		_but105.Create( 14, &_canvas101 );
		_but105.SetString( "自动生成报表并载入" );

		_layoutGrid101.InitPoint( GenPoint()( 50, 20 ) );
		_layoutGrid101.SetRowColumn( 14, 10 );
		_layoutGrid101.SetRowHeightInterval( 1, 40, 50 );
		_layoutGrid101.SetRowHeightInterval( 4, 40, 10 );
		_layoutGrid101.SetColumnWidthInterval( 3, 100, 50 );
		//
		_layoutGrid101.SetCell( 0, 0, 0, 1, &_lable104 );
		_layoutGrid101.SetCell( 1, 0, 1, 2, &_checkBut101 );
		//
		_layoutGrid101.SetCell( 0, 4, 0, 5, &_lable101 );
		_layoutGrid101.SetCell( 1, 4, 1, 5, &_but101 );
		//
		_layoutGrid101.SetCell( 2, 0, 2, 1, &_lable102 );
		_layoutGrid101.SetCell( 3, 0, 3, 3, &_edit101 );
		_layoutGrid101.SetCell( 4, 0, 10, 1, &_grid101 );
		_layoutGrid101.SetCell( 4, 2, &_but102 );
		_layoutGrid101.SetCell( 4, 3, &_but103 );
		_layoutGrid101.SetCell( 6, 2, 7, 3, &_but104 );
		//
		_layoutGrid101.SetCell( 2, 4, 2, 8, &_lable103 );
		_layoutGrid101.SetCell( 3, 4, 3, 8, &_lable105 );
		_layoutGrid101.SetCell( 4, 4, 4, 5, &_but105 );
		_layoutGrid101.SetCell( 6, 4, 6, 8, &_lable106 );
		_layoutGrid101.SetCell( 7, 4, 7, 8, &_lable107 );
		//
		_layoutGrid101.CalcGridPos();
	}

	{
		_canvas201.CreateDirect( &_tabX0, GenRect()( 0, 0, 1000, 1000 ) );
		
		_grid201.Create( 0, &_canvas201, GenRect()( 20, 65, 1040, 980 ), UILayoutCalc::SIZE_Y );
		_grid201.SetRowColumn( 1, 12 );								// 2014-2-19
		_grid201.SetRowFix();
		//_grid201.SetColumnFix();
		_grid201.SetCellString( 0, 0, "序号" );
		_grid201.SetColumnWidth( 0, 75 );
		_grid201.SetCellString( 0, 1, "衣柜楼层" );
		_grid201.SetColumnWidth( 1, 75 );
		_grid201.SetCellString( 0, 2, "更衣柜号码" );
		_grid201.SetColumnWidth( 2, 90 );
		_grid201.SetCellString( 0, 3, "姓名" );
		_grid201.SetColumnWidth( 3, 75 );
		_grid201.SetCellString( 0, 4, "性别" );
		_grid201.SetColumnWidth( 4, 75 );
		_grid201.SetCellString( 0, 5, "SAP号" );
		_grid201.SetColumnWidth( 5, 75 );
		_grid201.SetCellString( 0, 6, "工位楼层" );
		_grid201.SetColumnWidth( 6, 75 );
		_grid201.SetCellString( 0, 7, "所属室别" );
		_grid201.SetColumnWidth( 7, 130 );
		_grid201.SetCellString( 0, 8, "衣柜损坏" );
		_grid201.SetColumnWidth( 8, 75 );
		_grid201.SetCellString( 0, 9, "钥匙缺失" );
		_grid201.SetColumnWidth( 9, 75 );
		_grid201.SetCellString( 0, 10, "已配钥匙" );
		_grid201.SetColumnWidth( 10, 75 );
		_grid201.SetCellString( 0, 11, "发放钥匙" );				// 2014-2-19
		_grid201.SetColumnWidth( 11, 75 );

		//
		/*_but102.Create( 2, &_canvas201, GenRect()( 500, 945, 600, 985 ), UILayoutCalc::MOVE_Y );
		_but102.SetString( "导出报表" );*/

		_lable201.Create( 0, &_canvas201, GenRect()( 20, 15, 900, 50 ), UILayoutCalc::MOVE_Y );
	}

	{
		_tabY301.Create( 0, &_tabX0, _NULLRECT_, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		_tabY301.SetY();
		_tabY301.SetCellNum( 2 );
		_tabY301.SetCell( 0, "条件", &_canvas301 );
		_tabY301.SetCell( 1, "结果", &_canvas302 );

		_canvas301.CreateDirect( &_tabY301, GenRect()( 0, 0, 1000, 1000 ) );
		//
		_lable301.Create( 0, &_canvas301 );
		_lable301.SetString( "相关查询条件设置(全不选表示查所有条目): " );
		//
		_grid301.Create( 0, &_canvas301 );
		_grid301.SetRowColumn( 5, 3 );
		_grid301.SetRowFix();
		_grid301.SetCellCheckButton( 5, 0, 0, "全选" );
		_grid301.SetColumnWidth( 0, 80 );
		_grid301.SetCellString( 0, 1, "条件" );
		_grid301.SetColumnWidth( 1, 110 );
		_grid301.SetCellString( 0, 2, "值" );
		_grid301.SetColumnWidth( 2, 110 );
		_grid301.SetCellCheckButton( 0, 1, 0, "" );
		_grid301.SetCellCheckButton( 0, 2, 0, "" );
		_grid301.SetCellCheckButton( 0, 3, 0, "" );
		_grid301.SetCellCheckButton( 0, 4, 0, "" );
		_grid301.SetCellString( 1, 1, "衣柜楼层" );
		_grid301.SetCellString( 2, 1, "更衣室号码" );
		_grid301.SetCellString( 3, 1, "姓名" );
		_grid301.SetCellString( 4, 1, "SAP号" );
		for( UINT i=0; i<5; ++i )
			_grid301.SetRowHeight( i, 35 );
		//
		_lable302.Create( 0, &_canvas301 );
		_lable302.SetString( "报表统计功能: " );
		//
		_but301.Create( 6, &_canvas301 );
		_but301.SetString( "查   询" );
		_but302.Create( 8, &_canvas301 );
		_but302.SetString( "空余衣柜" );
		_but303.Create( 9, &_canvas301 );
		_but303.SetString( "损坏衣柜" );
		_but304.Create( 10, &_canvas301 );
		_but304.SetString( "缺钥匙衣柜" );

		_canvas302.CreateDirect( &_tabY301, GenRect()( 0, 0, 1000, 1000 ) );
		//
		_lable303.Create( 0, &_canvas302, GenRect()( 20, 15, 300, 50 ) );
		_lable303.SetString( "查询结果如下:" );
		//
		_but305.Create( 7, &_canvas302, GenRect()( 350, 15, 480, 50 ) );
		_but305.SetString( "保存修改" );
		//
		_grid302.Create( 20, &_canvas302, GenRect()( 20, 65, 1020, 980 ), UILayoutCalc::SIZE_Y );
		_grid302.SetRowColumn( 1, 12 );								// 2014-2-19
		_grid302.SetRowFix();
		_grid302.SetCellString( 0, 0, "序号" );
		_grid302.SetColumnWidth( 0, 75 );
		_grid302.SetCellString( 0, 1, "衣柜楼层" );
		_grid302.SetColumnWidth( 1, 75 );
		_grid302.SetCellString( 0, 2, "更衣柜号码" );
		_grid302.SetColumnWidth( 2, 90 );
		_grid302.SetCellString( 0, 3, "姓名" );
		_grid302.SetColumnWidth( 3, 75 );
		_grid302.SetCellString( 0, 4, "性别" );
		_grid302.SetColumnWidth( 4, 75 );
		_grid302.SetCellString( 0, 5, "SAP号" );
		_grid302.SetColumnWidth( 5, 75 );
		_grid302.SetCellString( 0, 6, "工位楼层" );
		_grid302.SetColumnWidth( 6, 75 );
		_grid302.SetCellString( 0, 7, "所属室别" );
		_grid302.SetColumnWidth( 7, 130 );
		_grid302.SetCellString( 0, 8, "衣柜损坏" );
		_grid302.SetColumnWidth( 8, 75 );
		_grid302.SetCellString( 0, 9, "钥匙缺失" );
		_grid302.SetColumnWidth( 9, 75 );
		_grid302.SetCellString( 0, 10, "已配钥匙" );
		_grid302.SetColumnWidth( 10, 75 );
		_grid302.SetCellString( 0, 11, "发放钥匙" );				// 2014-2-19
		_grid302.SetColumnWidth( 11, 75 );

		_layoutGrid301.InitPoint( GenPoint()( 100, 50 ) );
		_layoutGrid301.SetRowColumn( 14, 10 );
		_layoutGrid301.SetCell( 0, 0, 0, 3, &_lable301 );
		_layoutGrid301.SetCell( 1, 0, 4, 2, &_grid301 );
		_layoutGrid301.SetCell( 5, 0, 6, 1, &_but301 );
		//
		_layoutGrid301.SetCell( 0, 4, 0, 5, &_lable302 );
		_layoutGrid301.SetCell( 1, 4, 2, 5, &_but302 );
		_layoutGrid301.SetCell( 3, 4, 4, 5, &_but303 );
		_layoutGrid301.SetCell( 5, 4, 6, 5, &_but304 );
		_layoutGrid301.CalcGridPos();
	}

	UIPostMessage( this, WM_NOTIFY, 12, 0 );
	UIRefresh();
}

void UILocker::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
	{
		_tabX0.SetCurCell( param-1 );
	}
	else if( id==1 )
	{
		OnLoadReport();
	}
	else if( id==2 )
	{
		OnSaveReport();
	}
	else if( id==5 )
	{
		OnSelAllConditions();
	}
	else if( id==6 )
	{
		OnSearchOnConditions();
	}
	else if( id==7 )
	{
		OnSaveChanges();
	}
	else if( id==8 )
	{
		OnSearchEmpty();
	}
	else if( id==9 )
	{
		OnSearchDamage();
	}
	else if( id==10 )
	{
		OnSearchKeyLost();
	}
	else if( id==11 )
	{
		OnChooseDirectory();
	}
	else if( id==12 )
	{
		OnDefaultDirectory();
	}
	else if( id==13 )
	{
		OnLoadReport2();
	}
	else if( id==14 )
	{
		OnAutoCopyAndLoadReports();
	}
	else if( id==20 )
	{
		OnGird302( param );
	}
}

void UILocker::OnGird302( int param )
{
	NM_GRID* pNM = (NM_GRID*)param;

	if( pNM->_code==4 )
	{
		if( pNM->_column>=3 )
		{
			_changeIndexList.push_back( pNM->_row-1 );
		}
	}
}

void UILocker::OnLoadReport()
{
	// 获得导入文件路径
	string str;
	if( !OpenFolderFile( str ) ) 
		return;

	// 预处理
	PreLoadReport( str );

	// 导入数据
	vector<VECTOR_STRING> tempList;
	gLLogic.LoadXLSReport( tempList );

	ShowLoadReport( tempList, gLLogic._LoadFilePath );
}

void UILocker::OnLoadReport2()
{
	// 获得选中文件
	UINT row, column;
	if( !_grid101.GetSelectCell( row, column ) )
	{
		ShowMsg( "表格未选中文件!!" );
		return;
	}

	// 获得选中的文件路径
	string path, str;
	_edit101.GetString( path );
	_grid101.GetCellString( row, column, str );
	path += str;

	// 预处理
	PreLoadReport( path );

	// 导入数据
	vector<VECTOR_STRING> tempList;
	gLLogic.LoadXLSReport( tempList );

	ShowLoadReport( tempList, gLLogic._LoadFilePath );
}

void UILocker::OnSaveReport()
{

}

void UILocker::OnSelAllConditions()
{
	bool flag;
	_grid301.GetCellCheckState( 0, 0, flag );

	_grid301.SetCellCheckState( 1, 0, flag );
	_grid301.SetCellCheckState( 2, 0, flag );
	_grid301.SetCellCheckState( 3, 0, flag );
	_grid301.SetCellCheckState( 4, 0, flag );
}

void UILocker::OnSearchOnConditions()
{
	// 获得当前查询条件
	vector<pair<string, string> > conditions;
	for( UINT i=0; i<4; ++i )
	{
		bool flag;
		_grid301.GetCellCheckState( i+1, 0, flag );
		if( flag )
		{
			string strKey, strValue;
			_grid301.GetCellString( i+1, 1, strKey );
			_grid301.GetCellString( i+1, 2, strValue );
			conditions.push_back( make_pair<string, string>( strKey, strValue ) );
		}
	}

	// 交由逻辑进行查询
	vector<VECTOR_STRING> tempList;
	gLLogic.SearchOnConditions( conditions, tempList );

	// 显示查询到的结果
	ShowSearchResult( tempList );
}

void UILocker::OnSaveChanges()
{
	// 可能修改过的行
	sort( _changeIndexList.begin(), _changeIndexList.end() );
	_changeIndexList.erase( unique( _changeIndexList.begin(), _changeIndexList.end() ), _changeIndexList.end() );

	// 获得修改后的数据
	string str;
	vector<VECTOR_STRING> tempList;
	//
	int row, column;
	_grid302.GetRowColumn( row, column );
	for( int i=1; i<row; ++i )
	{
		VECTOR_STRING line;

		for( int j=3; j<12; ++j )								// 2014-2-19
		{
			_grid302.GetCellString( i, j, str );
			line.push_back( str );
		}

		tempList.push_back( line );
	}

	// 交由逻辑进行处理  修改文本
	vector<VECTOR_STRING> reLoadList;
	string msg;
	if( gLLogic.SaveChanges( _changeIndexList, tempList, reLoadList, msg ) )
	{	// 刷新载入表格
		ShowLoadReportGrid( reLoadList );
	}

	// 刷新修改表格
	gLLogic.GetSearchResult( tempList );
	_lable303.GetString( str );
	ShowSearchResult( tempList, str );

	// 显示逻辑处理结果
	ShowMsg( msg );

	// 
	_changeIndexList.clear();
}

void UILocker::OnSearchEmpty()
{
	vector<VECTOR_STRING> tempList;
	gLLogic.SearchEmpty( tempList );

	ShowSearchResult( tempList, "空余衣柜查询结果如下: " );
}

void UILocker::OnSearchDamage()
{
	vector<VECTOR_STRING> tempList;
	gLLogic.SearchDamage( tempList );

	ShowSearchResult( tempList, "损坏衣柜查询结果如下: " );
}

void UILocker::OnSearchKeyLost()
{
	vector<VECTOR_STRING> tempList;
	gLLogic.SearchKeyLost( tempList );

	ShowSearchResult( tempList, "缺钥匙衣柜查询结果如下: " );
}

void UILocker::OnChooseDirectory()
{
	string folderPath;
	VECTOR_STRING fileList;
	if( ReadFolderAllFiles1( "xlsx", folderPath, fileList ) )
		ShowDirectoryAndFiles( folderPath, fileList );
}

void UILocker::OnDefaultDirectory()
{
	string folderPath = gLLogic.GetDefaultReportDirectory();
	VECTOR_STRING fileList;
	if( ReadFolderAllFiles2( "xlsx", folderPath, fileList ) )
		ShowDirectoryAndFiles( folderPath, fileList );
}

void UILocker::OnAutoCopyAndLoadReports()
{
	string srcFile, copyMsg, msg;
	gLLogic.AutoCopyReports( srcFile, copyMsg );
	_lable106.SetString( srcFile );
	_lable107.SetString( copyMsg );
	
	// 导入数据
	vector<VECTOR_STRING> tempList;
	gLLogic.LoadXLSReport( tempList );

	ShowLoadReport( tempList, gLLogic._LoadFilePath ); 
}

void UILocker::PreLoadReport( std::string& filePath )
{
	// 是否保存为今天数据
	bool flag;
	_checkBut101.GetCheck( flag );

	// 判断是否将导入报表拷贝为当天报表
	if( flag )
	{
		string str;
		TimeFunc curTime;
		STRING_FORMAT( str, "L%04d_%02d_%02d.xlsx", curTime.GetYear(), curTime.GetMonth(), curTime.GetMonthDay() );

		string path = gLLogic.GetDefaultReportDirectory();
		path += "\\";
		path += str;
		
		CopyFileX( filePath, path );
		//CopyFile( filePath.c_str(), path.c_str(), true );

		gLLogic._LoadFilePath = path;
	}
	else
	{
		gLLogic._LoadFilePath = filePath;
	}

	_checkBut101.SetCheck( false );
}

void UILocker::ShowSearchResult( vector<VECTOR_STRING>& tempList, std::string msg )
{
	// 选中行信息清空
	_changeIndexList.clear();

	// _grid302表格显示
	_grid302.ClearUnfixRows();
	_grid302.AddRows( tempList, UI2DFont::FONT_CENTER );
	_tabY301.SetCurCell( 1 );
	for( UINT i=0; i<tempList.size(); ++i )
		_grid302.SetRowHeight( i+1, 28 );
	//
	VECTOR_STRING strList;
	strList.push_back( "男" );
	strList.push_back( "女" );
	strList.push_back( "" );
	for( UINT i=0; i<tempList.size(); ++i )
		_grid302.SetCellComboBox( 0, i+1, 4, strList, false );
	//
	strList.clear();
	strList.push_back( "信用卡业务室" );
	strList.push_back( "综合业务室" );
	strList.push_back( "品质管理室" );
	strList.push_back( "培训及知识管理室" );
	strList.push_back( "人力资源室" );
	strList.push_back( "综合及财务室" );
	strList.push_back( "" );
	for( UINT i=0; i<tempList.size(); ++i )
		_grid302.SetCellComboBox( 0, i+1, 7, strList, false );
	//
	strList.clear();
	strList.push_back( "是" );
	strList.push_back( "否" );
	for( UINT i=0; i<tempList.size(); ++i )
	{
		_grid302.SetCellComboBox( 0, i+1, 8, strList, false );
		_grid302.SetCellComboBox( 0, i+1, 9, strList, false );
		_grid302.SetCellComboBox( 0, i+1, 10, strList, false );
	}
	//
	for( UINT i=0; i<tempList.size(); ++i )
	{
		_grid302.SetCellFix( i+1, 0 );
		_grid302.SetCellFix( i+1, 1 );
		_grid302.SetCellFix( i+1, 2 );
	}

	_lable303.SetString( msg );

	// 
	string str;
	STRING_FORMAT( str, "共查询到%d条记录..", tempList.size() );
	ShowMsg( str );
}

void UILocker::ShowLoadReport( vector<VECTOR_STRING>& tempList, string path )
{
	_tabX0.SetCurCell( 1 );

	// 清除
	//_tabY301.SetCurCell( 0 );
	_lable303.SetString( "查询结果如下: " );
	_grid302.ClearUnfixRows();

	// 显示载入的文件
	string str;
	STRING_FORMAT( str, "载入报表: %s !!", path.c_str() );
	_lable201.SetString( str );

	// 显示数据
	_grid201.ClearUnfixRows();
	_grid201.AddRows( tempList, UI2DFont::FONT_CENTER );
	for( UINT i=0; i<tempList.size(); ++i )
		_grid201.SetRowHeight( i+1, 25 );
	_grid201.SetAllCellsFix();

	// 显示文件信息
	STRING_FORMAT( str, "共载入%d条记录..", tempList.size() );
	ShowMsg( str );
}

void UILocker::ShowLoadReportGrid( vector<VECTOR_STRING>& tempList )
{
	_grid201.ClearUnfixRows();
	_grid201.AddRows( tempList, UI2DFont::FONT_CENTER );
	for( UINT i=0; i<tempList.size(); ++i )
		_grid201.SetRowHeight( i+1, 25 );
	_grid201.SetAllCellsFix();
}

void UILocker::ShowDirectoryAndFiles( std::string& path, VECTOR_STRING& fileList )
{
	_edit101.SetString( path+"\\" );
	_grid101.ClearUnfixRows();

	for( UINT i=0; i<fileList.size(); ++i )
	{
		VECTOR_STRING row;
		row.push_back( fileList[i] );
		_grid101.AddRow( row, UI2DFont::FONT_CENTER );
		_grid101.SetCellFix( i+1, 0 );
		_grid101.SetRowHeight( i+1, 30 );
	}

	string str;
	STRING_FORMAT( str, "目录下存在 %d 个报表..", fileList.size() );
	ShowMsg( str );
}

LItem::LItem( VECTOR_STRING& line )
{
	_NO = line[0];
	_lockerFloor = line[1];
	_lockerNO = line[2];
	_name = line[3];
	_gender = line[4];
	_sapNO = line[5];
	_workFloor = line[6];
	_department = line[7];
	_isDamaged = line[8];
	_isKeyLost = line[9];
	_isKeyExist = line[10];
	_keyNum = line[11];					// 2014-2-19
}

LItem::operator VECTOR_STRING() 
{
	VECTOR_STRING line;
	line.resize( 12 );

	line[0] = _NO;
	line[1] = _lockerFloor;
	line[2] = _lockerNO;
	line[3] = _name;
	line[4] = _gender;
	line[5] = _sapNO;
	line[6] = _workFloor;
	line[7] = _department;
	line[8] = _isDamaged;
	line[9] = _isKeyLost;
	line[10] = _isKeyExist;
	line[11] = _keyNum;					// 2014-2-19

	return line;
}

void LLogic::AutoCopyReports( std::string& srcFile, std::string& copyMsg )
{
	// 获得报表文件夹路径
	string path, str;
	path = GetDefaultReportDirectory();
	path += "\\";

	// 获得所有的文件名
	string suffixLine = "xlsx";
	VECTOR_STRING fileListTemp;
	ReadFolderAllFiles2( suffixLine, path, fileListTemp );

	// 过滤文件名
	VECTOR_STRING fileList;
	for( UINT i=0; i<fileListTemp.size(); ++i )
	{
		if( fileListTemp[i].size()!=16 )
			continue;

		if( fileListTemp[i][0]!='L' || fileListTemp[i][5]!='_' || fileListTemp[i][8]!='_' )
			continue;

		fileList.push_back( fileListTemp[i] );
	}
	if( fileList.size()==0 )
	{
		copyMsg = "过往日期报表不存在..";
		return;
	}

	// 获得最近日期的文件作为拷贝源
	int selIndex = 0;
	for( UINT i=1; i<fileList.size(); ++i )
	{
		// 获得当前标记的最近日期
		int selYear = atoi( string( fileList[selIndex].begin()+1, fileList[selIndex].begin()+5 ).c_str() );
		int selMonth = atoi( string( fileList[selIndex].begin()+6, fileList[selIndex].begin()+8 ).c_str() );
		int selMonthDay = atoi( string( fileList[selIndex].begin()+9, fileList[selIndex].begin()+11 ).c_str() );

		// 获得要比较的日期
		int cmpYear = atoi( string( fileList[i].begin()+1, fileList[i].begin()+5 ).c_str() );
		int cmpMonth = atoi( string( fileList[i].begin()+6, fileList[i].begin()+8 ).c_str() );
		int cmpMonthDay = atoi( string( fileList[i].begin()+9, fileList[i].begin()+11 ).c_str() );

		if( selYear<cmpYear )
		{
			selIndex = i;
			continue;
		}
		else if( selYear==cmpYear )
		{
			if( selMonth<cmpMonth )
			{
				selIndex = i;
				continue;
			}
			else if( selMonth==cmpMonth )
			{
				if( selMonthDay<cmpMonthDay )
				{
					selIndex = i;
					continue;
				}
			}
		}
	}
	//
	STRING_FORMAT( srcFile, "最近日期报表: %s", fileList[selIndex].c_str() );

	// 获得选定文件的日期
	int selYear = atoi( string( fileList[selIndex].begin()+1, fileList[selIndex].begin()+5 ).c_str() );
	int selMonth = atoi( string( fileList[selIndex].begin()+6, fileList[selIndex].begin()+8 ).c_str() );
	int selMonthDay = atoi( string( fileList[selIndex].begin()+9, fileList[selIndex].begin()+11 ).c_str() );
	
	// 获得当前日期
	TimeFunc curTime;
	int curYear = curTime.GetYear();
	int curMonth = curTime.GetMonth();
	int curMonthDay = curTime.GetMonthDay();
	
	// 设定当天日期文件为当前选定文件路径
	STRING_FORMAT( str, "L%04d_%02d_%02d.xlsx", curYear, curMonth, curMonthDay );
	_LoadFilePath = path+str;

	// 比较最近日期是否等于当天日期
	if( selYear==curYear && selMonth==curMonth && selMonthDay==curMonthDay )
	{
		copyMsg = "当天日期报表已经存在..";
		return;
	}

	// 获得中间间隔日期
	TimeFunc timeFunc;
	timeFunc.SetDate( selYear, selMonth, selMonthDay );
	VECTOR_STRING timeSpan;
	timeFunc.GetTimeSpan( timeSpan );
	// 自动拷贝中间日期文件
	for( UINT i=0; i<timeSpan.size(); ++i )
	{
		str = timeSpan[i];
		str[4] = '_';
		str[7] = '_';
		TimeFunc curTime;
		STRING_FORMAT( str, "L%s.xlsx", str.c_str() );
		
		CopyFileX( path+fileList[selIndex], path+str );
	}

	// 自动拷贝当天日期文件
	CopyFileX( path+fileList[selIndex], _LoadFilePath );

	//
	STRING_FORMAT( copyMsg, "总共拷贝%d个文件..", timeSpan.size()+1 );
}

bool LLogic::LoadXLSReport( vector<VECTOR_STRING>& tempList )
{
	// 清理工作
	_LoadItemList.clear();

	// 首先载入数据
	if( XLSOpenFile( _LoadFilePath.c_str() ) )
	{
		if( XLSOpenSheet( "Sheet1" ) )
		{
			vector<VECTOR_STRING> allLineWordsList;
			XLSGetAllCellsString( &allLineWordsList );

			if( allLineWordsList.size()>1 )
			{
				for( UINT i=1; i<allLineWordsList.size(); ++i )
				{
					VECTOR_STRING& line = allLineWordsList[i];

					// 2014-2-19
					// 读取兼容以前的格式
					if( line.size()==11 )
						line.push_back( "" );

					if( line.size()==12 )
						tempList.push_back( line );
				}
			}
		}
	}
	XLSClose();

	// 数据导入本地内存
	for( UINT i=0; i<tempList.size(); ++i )
		_LoadItemList.push_back( LItem( tempList[i] ) );

	return tempList.size()>1 ? true:false;
}

void LLogic::SearchOnConditions( vector<pair<string, string> >& conditions, vector<VECTOR_STRING>& tempList )
{
	// 准备未进行条件过滤的所有条目index
	_searchIndexList.clear();
	for( UINT i=0; i<_LoadItemList.size(); ++i )
		_searchIndexList.push_back( i );

	// 按条件进行过滤
	for( UINT i=0; i<conditions.size(); ++i )
		_searchIndexList = SearchOnCondition( _searchIndexList, conditions[i].first, conditions[i].second );

	// 根据indexList获得具体的条目
	tempList.clear();
	for( UINT i=0; i<_searchIndexList.size(); ++i )
		tempList.push_back( _LoadItemList[_searchIndexList[i]] );
}

vector<UINT> LLogic::SearchOnCondition( VECTOR_UINT& indexList, std::string& key, std::string& value )
{
	vector<UINT> tempIndexList;
	for( UINT i=0; i<indexList.size(); ++i )
	{
		if( key=="衣柜楼层" )
		{
			if( _LoadItemList[indexList[i]]._lockerFloor==value )
				tempIndexList.push_back( indexList[i] );
		}
		else if( key=="更衣室号码" )
		{
			if( _LoadItemList[indexList[i]]._lockerNO==value )
				tempIndexList.push_back( indexList[i] );
		}
		else if( key=="姓名" )
		{
			if( _LoadItemList[indexList[i]]._name==value )
				tempIndexList.push_back( indexList[i] );
		}
		else if( key=="SAP号" )
		{
			if( _LoadItemList[indexList[i]]._sapNO==value )
				tempIndexList.push_back( indexList[i] );
		}
	}

	return tempIndexList;
}

bool LLogic::SaveChanges( VECTOR_UINT changeIndexList, vector<VECTOR_STRING>& tempList, vector<VECTOR_STRING>& reLoadList, string& str )
{
	// 获得已有的sap号
	VECTOR_STRING sapList;
	for( UINT i=0; i<_LoadItemList.size(); ++i )
		sapList.push_back( _LoadItemList[i]._sapNO );
	
	// 判定sap号是否重复
	for( UINT i=0; i<changeIndexList.size(); ++i )
	{
		string& checkSap = tempList[changeIndexList[i]][2];

		// 空sap号不进行重复判定
		if( checkSap=="" )
			continue;

		// 进行判定
		VECTOR_STRING tempSapList = sapList;
		UINT& index = _searchIndexList[changeIndexList[i]];
		tempSapList[index] = checkSap;
		if( count( tempSapList.begin(), tempSapList.end(), checkSap )>1 )
		{
			STRING_FORMAT( str, "SAP号%s重复, 录入失败！", checkSap.c_str() ); 
			return false;
		}
	}

	// 只修改文本 不修改内存
	str = "文件打开失败！";
	if( XLSOpenFile( _LoadFilePath.c_str() ) )
	{
		if( XLSOpenSheet( "Sheet1" ) )
		{
			int saveCount = 0;
			for( UINT i=0; i<changeIndexList.size(); ++i )
			{
				bool flag = false;
				VECTOR_STRING& line = tempList[changeIndexList[i]];

				// 逐字判断修改
				UINT& index = _searchIndexList[changeIndexList[i]];
				if( line[0]!=_LoadItemList[index]._name )
				{
					flag = true;
					XLSSetCellString( index+2, 4, line[0].c_str() );
				}

				if( line[1]!=_LoadItemList[index]._gender )
				{
					flag = true;
					XLSSetCellString( index+2, 5, line[1].c_str() );
				}

				if( line[2]!=_LoadItemList[index]._sapNO )
				{
					flag = true;
					XLSSetCellString( index+2, 6, line[2].c_str() );
				}

				if( line[3]!=_LoadItemList[index]._workFloor )
				{
					flag = true;
					XLSSetCellString( index+2, 7, line[3].c_str() );
				}

				if( line[4]!=_LoadItemList[index]._department )
				{
					flag = true;
					XLSSetCellString( index+2, 8, line[4].c_str() );
				}

				if( line[5]!=_LoadItemList[index]._isDamaged )
				{
					flag = true;
					XLSSetCellString( index+2, 9, line[5].c_str() );
				}

				if( line[6]!=_LoadItemList[index]._isKeyLost )
				{
					flag = true;
					XLSSetCellString( index+2, 10, line[6].c_str() );
				}

				if( line[7]!=_LoadItemList[index]._isKeyExist )
				{
					flag = true;
					XLSSetCellString( index+2, 11, line[7].c_str() );
				}

				// 2014-2-19
				// 发放钥匙数修改
				if( line[8]!=_LoadItemList[index]._keyNum )
				{
					flag = true;
					XLSSetCellString( index+2, 12, line[8].c_str() );
				}
		
				if( flag ) 
				{
					++saveCount;
				}
			}

			STRING_FORMAT( str, "修改数为%d条, 保存成功！..", saveCount );
		}
	}
	XLSClose();

	// 重新读取文本
	LoadXLSReport( reLoadList );
	return true;
}

void LLogic::GetSearchResult( std::vector<VECTOR_STRING>& tempList )
{
	// 根据indexList获得具体的条目
	tempList.clear();
	for( UINT i=0; i<_searchIndexList.size(); ++i )
		tempList.push_back( _LoadItemList[_searchIndexList[i]] );
}

void LLogic::SearchEmpty( std::vector<VECTOR_STRING>& tempList )
{
	// 准备未进行条件过滤的所有条目index
	_searchIndexList.clear();
	for( UINT i=0; i<_LoadItemList.size(); ++i )
	{
		if( _LoadItemList[i]._department == "" && _LoadItemList[i]._name=="" &&
			_LoadItemList[i]._gender == "" && _LoadItemList[i]._sapNO=="" && _LoadItemList[i]._workFloor=="" )
			_searchIndexList.push_back( i );
	}

	// 根据indexList获得具体的条目
	tempList.clear();
	for( UINT i=0; i<_searchIndexList.size(); ++i )
		tempList.push_back( _LoadItemList[_searchIndexList[i]] );
}

void LLogic::SearchDamage( std::vector<VECTOR_STRING>& tempList )
{
	// 准备未进行条件过滤的所有条目index
	_searchIndexList.clear();
	for( UINT i=0; i<_LoadItemList.size(); ++i )
	{
		if( _LoadItemList[i]._isDamaged == "是" )
			_searchIndexList.push_back( i );
	}

	// 根据indexList获得具体的条目
	tempList.clear();
	for( UINT i=0; i<_searchIndexList.size(); ++i )
		tempList.push_back( _LoadItemList[_searchIndexList[i]] );
}

void LLogic::SearchKeyLost( std::vector<VECTOR_STRING>& tempList )
{
	// 准备未进行条件过滤的所有条目index
	_searchIndexList.clear();
	for( UINT i=0; i<_LoadItemList.size(); ++i )
	{
		if( _LoadItemList[i]._isKeyLost == "是" )
			_searchIndexList.push_back( i );
	}

	// 根据indexList获得具体的条目
	tempList.clear();
	for( UINT i=0; i<_searchIndexList.size(); ++i )
		tempList.push_back( _LoadItemList[_searchIndexList[i]] );
}

std::string LLogic::GetDefaultReportDirectory()
{
	string path;
	GetExeDirectory( path );
	GetUpDirectory( path );
	path += "\\Reports";
	return path;
}