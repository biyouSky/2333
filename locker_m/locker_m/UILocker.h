#pragma once


class UILocker : public UIWindow<UILocker>
{
	friend UIWindow;

public:
	void OnLoadReport();
	void OnLoadReport2();
	void OnSaveReport();
	void OnSelAllConditions();
	void OnSearchOnConditions();
	void OnSaveChanges();
	void OnSearchEmpty();
	void OnSearchDamage();
	void OnSearchKeyLost();
	void OnChooseDirectory();
	void OnDefaultDirectory();
	void OnAutoCopyAndLoadReports();
	void OnGird302( int param );

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void ShowSearchResult( std::vector<VECTOR_STRING>& tempList, std::string msg="查询结果如下: " );
	void ShowLoadReport( vector<VECTOR_STRING>& tempList, std::string path );
	void ShowLoadReportGrid( vector<VECTOR_STRING>& tempList );
	void ShowDirectoryAndFiles( std::string& path, VECTOR_STRING& fileList );

	void PreLoadReport( std::string& filePath );

	UITab _tabX0;
	
	// 1
	UICanvas _canvas101;
	UICheckButton _checkBut101;
	UIButton _but101;
	UIButton _but102;
	UIButton _but103;
	UIButton _but104;
	UIButton _but105;
	UILable _lable101;
	UILable _lable102;
	UILable _lable103;
	UILable _lable104;
	UILable _lable105;
	UILable _lable106;
	UILable _lable107;
	UIEdit _edit101;
	UIGrid _grid101;
	// 
	UILayoutGrid _layoutGrid101;

	// 2
	UICanvas _canvas201;
	UIGrid _grid201;
	UILable _lable201;	
	
	// 3
	UITab _tabY301;
	//
	UICanvas _canvas301;
	UILable _lable301;
	UILable _lable302;
	UIButton _but301;
	UIButton _but302;
	UIButton _but303;
	UIButton _but304;
	UIGrid _grid301;
	//
	UICanvas _canvas302;
	UILable _lable303;
	UIButton _but305;
	UIGrid _grid302;
	//
	UILayoutGrid _layoutGrid301;

	// 可能修改的行
	VECTOR_UINT _changeIndexList;
};
extern UILocker gUILocker;




struct LItem
{
	std::string  _NO;
	std::string  _lockerNO;
	std::string  _lockerFloor;
	std::string  _workFloor;
	std::string  _sapNO;

	std::string _name;
	std::string _gender;
	std::string _department;
	
	std::string _isDamaged;
	std::string _isKeyLost;
	std::string _isKeyExist;

	std::string _keyNum;

	LItem( VECTOR_STRING& line );
	operator VECTOR_STRING();
};

class LLogic
{
public:
	bool LoadXLSReport( std::vector<VECTOR_STRING>& tempList );
	void SearchOnConditions( std::vector<std::pair<std::string, std::string> >& conditions, std::vector<VECTOR_STRING>& tempList );
	bool SaveChanges( VECTOR_UINT changeIndexList, std::vector<VECTOR_STRING>& tempList, std::vector<VECTOR_STRING>& reLoadList, std::string& str );
	void SearchEmpty( std::vector<VECTOR_STRING>& tempList );
	void SearchDamage( std::vector<VECTOR_STRING>& tempList );
	void SearchKeyLost( std::vector<VECTOR_STRING>& tempList );

	// 自动拷贝中间日期文件与当天文件
	void AutoCopyReports( std::string& srcFile, std::string& copyMsg );

	std::string GetDefaultReportDirectory();
	void GetSearchResult( std::vector<VECTOR_STRING>& tempList );

	std::string _LoadFilePath;
	std::vector<LItem> _LoadItemList;

private:
	VECTOR_UINT SearchOnCondition( VECTOR_UINT& indexList, std::string& key, std::string& value );

	VECTOR_UINT _searchIndexList;				// 查询的缓存index
};
extern LLogic gLLogic;