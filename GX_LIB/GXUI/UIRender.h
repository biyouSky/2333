
#pragma once

#include "UIUtil.h"

/*-------------------------------------------------------DirectX��ʼ��-------------------------------------------------------*/
class UIDXInit
{
public:
	static UIDXInit* GetSingleton();
	static void Destroy();
	static void Assign( UIDXInit* pPara );

	bool InitD3D(	HWND bindHWnd,							// [in]
					ULONG width, ULONG height,				// [in] Backbuffer dimensions. �������С ����봰�ڴ�С����һ��
					bool windowed=true,						// [in] Windowed ( true )or full screen ( false ).
					D3DDEVTYPE deviceType=D3DDEVTYPE_HAL );	// [in] HAL or REF

	bool ResetD3D( ULONG width, ULONG height );

	HWND GetBindHWnd();		

	ULONG GetPresentWidth();
	ULONG GetPresentHeight();		

	IDirect3DDevice9* _pIDevice;							// d9 �豸�ӿ�
	D3DPRESENT_PARAMETERS _presentParam;					// �����������

private:
	// ���� ����
	UIDXInit();
	~UIDXInit();

	static UIDXInit* _pSingleton;
};






/*-------------------------------------------------------UI���Ʋ�-------------------------------------------------------*/
class UIRender
{
public:
	static UIRender* GetSingleton();
	static void Destroy();
	static void Assign( UIRender* pPara );

	void Render();																// �����й�
	bool AutoAdjustSizeChange( ULONG width, ULONG height );						// ����Ӧ���ڴ�С�仯	

	void SaveUIImage( std::string imagePath );

private:
	UIRender();
	~UIRender();

	void InitRenderEffect();

	static UIRender* _pSingleton;
};






/*-------------------------------------------------------��������-------------------------------------------------------*/

// ��������
/*
	�ӻ��������Ķ������������������֣�
	1. ��ȫDrawAuto()��������л���
	2. ����ȫDrawAuto()��ֻ�̳��˶��������ж�������������д���
*/
struct UIAnimateBase
{
	friend class UIAnimateManage;

	virtual void InitAnimate() {}					// ��ʼ��
	virtual void UninitAnimate() {}					// 

	virtual bool IsRunAnimate() = 0;				// �����Ƿ�����
	virtual bool IsDrawFrame() = 0;					// ���㱾֡���� �Ƿ����
	virtual void DrawAuto() {}						// ��֡�������� ����Ϊ�� 
};

// ��������
class UIAnimateManage
{
public:
	static UIAnimateManage* GetSingleton();
	static void Destroy();
	static void Assign( UIAnimateManage* pPara );

	void AddAnimate( UIAnimateBase* pAnimateObj );
	bool HandleTimer();
	void DrawAutoList();

protected:
	UIAnimateManage();
	~UIAnimateManage();

	std::list<UIAnimateBase* > _animateList;		// �����б� ������������Զ�ɾ��
	std::vector<UIAnimateBase* > _animateListTemp;	

	static UIAnimateManage* _pSingleton;
};
void UIRegisterAnimate( UIAnimateBase* pAnimateObj );


// �����֧��
class UICaret : public UIAnimateBase
{
public:
	static UICaret* GetSingleton();
	static void Destroy();
	static void Assign( UICaret* pPara );

	void SetCaret( float z, float width, float height, D3DCOLOR rgb );
	void HideCaret();
	void SetPos( int x, int y, bool IsShowImmd );

private:
	UICaret();

	bool IsRunAnimate();
	bool IsDrawFrame();
	void DrawAuto();

	static UICaret* _pSingleton;

	// �������
	float _x, _y, _width, _height, _z;			// ����_x, _yΪcaret���ĵ�һ�������
	D3DCOLOR _rgb;

	// �������
	bool _isAnimateOn;							// ������־
	bool _isCaretOn;							// ��ʾ��־
	bool _isDraw;								// ���Ʊ�־

	DWORD _oldTick;
};
//
void UIShowCaret( float z, int width, int height, D3DCOLOR rgb );
void UIHideCaret();
void UISetCaretPos( int x, int y, bool IsShowImmd=true );


// ����������Ч
class UIAnimateEffect : public UIAnimateBase
{
	/*--------------------------------- �������� ---------------------------------*/
public:
	UIAnimateEffect();

	void PlayAnimate( int mode, int count=5 );

	enum 
	{	
		HIT_DRUM = 100,			 // 100���������Զ���
		SET_HOT,
		SET_COOL
	};

protected:
	bool IsRunAnimate();
	bool IsDrawFrame();

	enum 
	{	// ��������֡��
		HIT_DRUM_FRAME = 10,
		SET_HOT_FRAME = 9,
		SET_COOL_FRAME = 9
	};

	int _animateMode;
	int _maxFrame;
	int _frameCount;

	/*--------------------------------- ������Ч ---------------------------------*/
public:
	void SetImageIDAndDLL( int id, std::string path="GUIResource.dll", D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void SetImagePath( std::string path, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void SetHitPower( float v );

	void DrawHitDrumAnimate( int centerX, int centerY, float _z );
	void DrawHitDrumAnimate( RECT rc, float _z );
	void DrawFrameHitDrumAnimate( RECT rc, int corner, float _z );

protected:
	int _loadImageWay;							// 1: _imageID    2: _imagePath
	int _imageID;
	std::string _dllPath;						
	std::string _imagePath;
	D3DCOLOR _colorKey;
	float _hitPower;							// ͼƬ�Ŵ�ϵ��

	
	/*--------------------------------- hot��Ч ---------------------------------*/
public:
	void DrawHotAnimate( RECT rc, float _z );


	/*--------------------------------- cool��Ч ---------------------------------*/
public:
	void DrawCoolAnimate( RECT rc, float _z );
};