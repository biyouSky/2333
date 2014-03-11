
#pragma once

#include "UIUtil.h"

/*-------------------------------------------------------DirectX��ʼ��-------------------------------------------------------*/
class UIDXInit
{
public:
	static UIDXInit* GetSingleton();
	static void Destroy();

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

	virtual void Init() {}							// ��ʼ��
	virtual void Uninit() {}						// 

	virtual bool IsRun() = 0;						// �����Ƿ�����
	virtual bool IsDrawFrame() = 0;					// ���㱾֡���� �Ƿ����
	virtual void DrawAuto() {}						// ��֡�������� ����Ϊ�� 
};



// ��������
class UIAnimateManage
{
public:
	static UIAnimateManage* GetSingleton();
	static void Destroy();

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