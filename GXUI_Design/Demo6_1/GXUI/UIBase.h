
#pragma once

#include "UIUtil.h"


extern cnCriticalSection gDXCS;

/*-------------------------------------------------------DirectUI ��Ϣѭ������-------------------------------------------------------*/
// ��Ϣ����ʽ
class UIWinBase;
struct UIMessage
{
	UIMessage();
	UIMessage( UIWinBase* pWin, UINT message, WPARAM wParam, LPARAM lParam );

	UIWinBase* _pWin;
	UINT _msg;
	WPARAM _wParam;
	LPARAM _lParam;
};



// DirectUI ��Ϣѭ���������  �����¼�ȫ��������Ϣѭ���� �����л��ƴ���  ����ʵ���˲�������
class UIMessageLoopManage : public _MultiThreadManage<UIMessageLoopManage>
{
public:
	static UIMessageLoopManage* GetSingleton();
	static void Destroy();
	
	void RunMessageLoop();
	friend void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

private:
	UIMessageLoopManage();
	~UIMessageLoopManage();

	void RunMessageLoopThread( void* pData );

	void PushMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
	bool PopMessage( UIMessage& msg ); 

	bool HandleMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

	static UIMessageLoopManage* _pSingleton;

	std::deque<UIMessage> _uiMsgList;		// UI��Ϣ����
	cnCriticalSection _uiMsgListCS;
};
void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
void UIRefresh( bool immediately=true );
bool UIMessageFilter1( UINT message );		// ����UI��Ϣѭ������







/*-------------------------------------------------------UIWinBase-------------------------------------------------------*/
/*
	���������� 
	������Ϣ��
	1 λ����Ϣ
	2 �����Ϣ
	3 ������Ϣ
	4 ����

	�������ܣ�
	1 ��������
	2 �������ɡ�����
	3 ��Ϣ��Ĭ�ϴ���
	ע���������б������ޱ������б����������������͵���Ϣѭ���У��ޱ�����ֱ�Ӳ�����
		�����ޱ�����ֱ�����ú������󶼽�����Direct����SetXXDirect()��
*/
class UIContainer;
class UIWinBase
{
	friend UIContainer;

public:
	// ���� ����
	UIWinBase();
	virtual ~UIWinBase();

	// ��������  
	bool CreateDirect( UIContainer* pUIContainer, RECT& relativeRect, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false );
	bool CreateDirect( UIWinBase* pParent, RECT& relativeRect=_NULLRECT_, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false );
	// ���ٴ���
	bool DestroyDirect();

	// ��Ϣ����
	virtual bool HandleMessage( UINT message, WPARAM wParam, LPARAM lParam );

	// ���� ֱ���ޱ���
	

	// ��ȡ �ޱ���
	RECT GetClientRect();
	RECT GetRelativeRect();

	//
	virtual void Draw();						// ����

	UIContainer* _pUIContainer;					// ��������ָ��

protected:
	// Ĭ����Ϣ����
	bool DefHandleMessage( UINT message, WPARAM wParam, LPARAM lParam );

	// λ����Ϣ
	RECT _clientRC;								// ��ʼΪ0�Ĵ�С����
	POINT _relativePoint;						// ��Ը����ڵ���ʼλ�õ�
	POINT _abusolutePoint;						// ������㴰�ڵ���ʼλ�õ�	

	// �����Ϣ
	int _layoutLever;							// ��� �������ֵ 1-0.2 2-0.19 3-0.18
	float _z;									// ���ֵ 

	// ����
	bool _isShow;								// ��ʾ����
	bool _isRecvTabKey;							// ����tab��
	bool _isFocus;								// ���ڻ�ý��㼤����Ϣ
	int _layoutMode;							// ����ģʽ
	bool _isTransmissionMsg;					// ͸���Ӵ��ڴ��ݵ���Ϣ

	UIContainer* _pParentUIContainer;			// ������ָ��
	UIWinBase *_pRealParent;					// popup���ʹ�������
};






/*-------------------------------------------------------UIContainer-------------------------------------------------------*/
/*
	���֣���������
	�����������࣬�ṩ���Ӵ��ڵĹ����ַ����ݲ�������Ϣ��
	�÷�������ͨ�� ���� �ķ�ʽ���ڸ��������Ϣ�����а����������������Ϣ����
	���ܣ�
		1 �Ӵ��ڹ������봰�ڵĴ��������١����ּ��㡢���ƹ���ȣ�
		2 ��Ϣ���´��ݴ����������յ���Ϣ����ͨ���󶨵Ĵ����������ݵ��Ӵ��ڽ���Ԥ����
		 ��ͨ���Ӵ��ڰ󶨵Ĵ��������������´��ݣ��γ�һ���ݹ���̣������Ӵ���δ�ܴ��������ɸ�����������д���

	ע�⣺��������������ò��������ޱ����ģ�ֻ������Ϣѭ�������е��ò���ȷ����ȫ������Ϊ������ֻ��GXUI����ڲ�ʹ�ã�
		  ���ڿ���ڲ������˺����������Direct����
*/
class UIContainer
{
	// ����Ԫ��
	struct UIElement
	{
		UIWinBase* _pWin;						// �Ӵ���ָ��
		UILayoutCalc _layoutInfor;				// ����ģʽ
		bool _isOnHeap;							// �����ڴ��Ƿ�λ�ڶ���

		UIElement( UIWinBase* p, bool isOnHeap ) { _pWin=p; _isOnHeap=isOnHeap; }
		bool operator==( UIWinBase* p ) { return _pWin==p; }
	};
	typedef std::vector<UIElement> UIElementListType;

public:
	UIContainer();
	~UIContainer();

	// �븸���ڰ�
	void BindWin( UIWinBase *pBindWindow );
	void BindWin( HWND hWnd );

	// ����Ӵ���  ɾ���Ӵ���  ������δ�������ݱ���
	void AddChild( UIWinBase* pWin, bool isOnHeap );	
	void DelChild( UIWinBase* pWin );
 
	// ��ϢԤǰ���������WM_PAINT��������Ϣ
	bool HandleMessagePre( UINT message, WPARAM wParam, LPARAM lParam );

	// ��ȡ
	POINT GetBindWinAbusolutePoint();
	int GetBindWinLayoutLever();
	
	// ����
	void SetNewFocusOnChild( UIWinBase* pNew );

	// ����
	void Draw();

private:	
	UIWinBase* GetFocusOnChild();				// ����focusΪon���Ӵ���

	bool _isBindDUI;							// ��ǰ󶨵ĸ�����ʱDUI��hwnd
	UIWinBase* _pBindDUIWin;					// ��󶨵�DUI������
	HWND _bindWindowHwnd;						// ��󶨵�hwnd������

	POINT _preMousePt;							// ��¼�����ǰһ��λ��

	UIElementListType _winList;					// �Ӵ������
};

// �ṩ����Container֧��
template<class T>
class UIContainerHelp
{
public:
	UIContainerHelp()
	{
		T* pT = static_cast<T*>( this );
		pT->_pUIContainer = &_uiContainer;
		_uiContainer.BindWin( pT ); 
	}

protected:
	UIContainer _uiContainer;
};
