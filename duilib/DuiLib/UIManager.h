#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

namespace DuiLib 
{
	
class CControlUI;

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT__LAST,
};

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

typedef struct tagTFontInfo
{
    HFONT hFont;             /**< ������ */
    CStdString sFontName;    /**< �������� */
    int iSize;               /**< �ֺ� */
    bool bBold;              /**< �Ƿ�Ӵ� */
    bool bUnderline;         /**< �Ƿ����»��� */
    bool bItalic;            /**< �Ƿ�Ϊб�� */
    TEXTMETRIC tm;           /**< �������TEXTMETRIC��Ϣ */
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;    
    int nX;
    int nY;
    bool alphaChannel;
    CStdString sResType;
    DWORD dwMask;
} TImageInfo;

// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
    CStdString sType;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WPARAM wParam;
    LPARAM lParam;
} TNotifyUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

/**
  \brief �¼�֪ͨ������,��Ҫ�������£�
         1. ����Notify�麯�����������¼�֪ͨ
     
 */
class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

// MessageFilter interface
class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

/**
  \brief ������Ϣ��ͼ�λ��ƹ�������,�봰�ڰ������о�����:
         1. ���ƿؼ�
		 2. ��Ϣ����
		 3. �¼�֪ͨ
 */
class UILIB_API CPaintManagerUI
{
public:

    CPaintManagerUI();
    
	~CPaintManagerUI();

public:

	/**
	  \brief ��ͼ��������ʼ��
	         m_hWndPaint, m_hDcPaint��ֵ,��Ԥ������Ϣ�м��������)
	 */
	void Init(HWND hWnd);
   
	/**
	  \brief ��ǰ��Ҫ���½���
	 */
	void NeedUpdate();

	/**
	  \brief ָ������ʧЧ
	 */
	void Invalidate(RECT& rcItem);

	/**
	  \brief ��ȡ��ͼ�豸DC
	 */
    HDC GetPaintDC() const;

	/**
	  \brief ��ȡ��ͼ���ھ�� 
	 */
	HWND GetPaintWindow() const;

	/**
	  \brief ��ȡ��ʾ���ھ��
	 */
	HWND GetTooltipWindow() const;

	/**
	  \brief ��ȡ��ǰ����λ�� 
	 */
    POINT GetMousePos() const;

	/**
	  \brief ��ȡ�ͻ�����С
	 */
	SIZE GetClientSize() const;

	/**
	  \brief ��ȡ�����ô����ʼ���Ĵ�С
	 */
	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);

	/**
	  \brief ��ȡ�����ô���߿������С
	 */
	RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);

	/**
	  \brief ��ȡ�����ñ�������λ��
	 */
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);

	/**
	  \brief ��ȡ�����ô����Ľǵ�Բ�ǻ���
	 */
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);

	/**
	  \brief ���úͻ�ȡ����ɵ���������С��С
	 */
	SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    
	/**
	  \brief ��ȡ�����ô���ɵ�����������С
	 */
	SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
    
	/**
	  \brief ���ô����͸����(0��ȫ͸��-255��ȫ��͸��)
	 */
	void SetTransparent(int nOpacity);

	/**
	  \brief ���û�ͼ�Ƿ�֧��͸������
	 */
    void SetBackgroundTransparent(bool bTrans);
    
	/**
	  \brief �Ƿ���ʾ��������
	 */
	bool IsShowUpdateRect() const;
    
	/**
	  \brief �����Ƿ���ʾ����
	 */
	void SetShowUpdateRect(bool show);

	/**
	  \brief ��ȡ�����õ�ǰ�����ʵ�����
	 */
    static HINSTANCE GetInstance();
	static void SetInstance(HINSTANCE hInst);

	/**
	  \brief ��ȡ��ǰ���е�ʵ����·��
	 */
    static CStdString GetInstancePath();

	/**
	  \brief ��ȡ�����õ�ǰ�Ĺ���·��
	 */
    static CStdString GetCurrentPath();
	static void SetCurrentPath(LPCTSTR pStrPath);
    
	/**
	  \brief ��ȡ��ԴDLL��ʵ�����
	 */
	static HINSTANCE GetResourceDll();
	static void SetResourceDll(HINSTANCE hInst);
    
	/**
	  \brief ��ȡ��Դ·��(��'\'��β)
	 */
	static const CStdString& GetResourcePath();
	static void SetResourcePath(LPCTSTR pStrPath);
    
	/**
	  \brief ��ȡZip��Դ��·��
	 */
	static const CStdString& GetResourceZip();
	static void SetResourceZip(LPCTSTR pStrZip);

    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
    static void ReloadSkin();

	/**
	  \brief ����ʹ���ϼ���Դ�Ļ�ͼ������
	 */
    bool UseParentResource(CPaintManagerUI* pm);
    /**
      \brief ��ȡ�ϼ���Դ�Ļ�ͼ������
     */
	CPaintManagerUI* GetParentResource() const;

	/**
	  \brief ��ȡ�����ý���״̬��Ĭ����ɫ
	 */
    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    
	/**
	  \brief ��ȡ����������Ĭ����ɫ
	 */
	DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    
	/**
	  \brief ��ȡ�������������ֵ�Ĭ����ɫ
	 */
	DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    
	/**
	  \brief ��ȡ�����ͣ�볬�����ϵ�Ĭ���������ɫ
	 */
	DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    
	/**
	  \brief ��ȡ������ѡ��״̬��Ĭ�ϱ�����ɫ
	 */
	DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    
	/**
	  \brief ��ȡĬ��ʹ�õ�������Ϣ
	 */
	TFontInfo* GetDefaultFontInfo();
    
	/**
	  \brief ����Ĭ��ʹ�õ�������Ϣ
	 */
	void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief ��ȡ�û��Զ������������ 
	 */
	DWORD GetCustomFontCount() const;
    
	/**
	  \brief �����������б���׷��������Դ
	 */
	HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/**
	  \brief �����������б��в���������Դ
	 */
	HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief ��ȡ������ָ���±�����������
	 */
	HFONT GetFont(int index);

	/**
	  \brief �����������л�ȡָ�����õ����������
	 */
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief �鿴�������鼯�����Ƿ�����������
	 */
	bool FindFont(HFONT hFont);

	/**
	  \brief �鿴�������鼯�����Ƿ����ָ�����õ��������
	 */
    bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief ��ȡ��������������
	 */
	int GetFontIndex(HFONT hFont);

	/**
	  \brief ��ȡָ�����õ���������������
	 */
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief ������������ɾ���������
	 */
	bool RemoveFont(HFONT hFont);

	/**
	  \brief ɾ��ָ�������ŵ��������
	 */
    bool RemoveFontAt(int index);

	/**
	  \brief ������������б�
	 */
    void RemoveAllFonts();

	/**
	  \brief ��ȡ���������Ϣ
	 */
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);

	/**
	  \brief �鿴ͼ����Ϣ
	 */
    const TImageInfo* GetImage(LPCTSTR bitmap);
    const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	/**
	  \brief ���ͼ��
	 */
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	/**
	  \brief ���ͼ��
	 */
	const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    
	/**
	  \brief ����ͼ�������Ƴ�ͼ��
	 */
	bool RemoveImage(LPCTSTR bitmap);
    
	/**
	  \brief �Ƴ�ȫ��ͼ��
	 */
	void RemoveAllImages();
    void ReloadAllImages();

	/**
	  \brief ��ӿؼ���Ĭ��������Ϣ
	 */
    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    
	/**
	  \brief �Ƴ����Ƶ�Ĭ������
	 */
	bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    
	/**
	  \brief ��ȡĬ��������Ϣ�б�
	 */
	const CStdStringPtrMap& GetDefaultAttribultes() const;
    void RemoveAllDefaultAttributeList();

	/**
	  \brief ���Ի��򸽼ӵ���ǰ��������
	 */
    bool AttachDialog(CControlUI* pControl);
    
	/**
	  \brief �ؼ���ʼ��
	 */
	bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    
	/**
	  \brief �ؼ�����
	 */
	void ReapObjects(CControlUI* pControl);

	/**
	  \brief ��ӿؼ���ָ����ѡ����
	 */
    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    
	/**
	  \brief ��ѯָ��ѡ�����е�ȫ��ѡ��
	 */
	CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    
	/**
	  \brief ��ָ���ؼ����Ƴ�ָ��ѡ�������Ƶ�ѡ����
	 */
	void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    
	/**
	  \brief ���ȫ��ѡ�����б�
	 */
	void RemoveAllOptionGroups();

	/**
	  \brief ��ȡ����״̬�Ŀؼ�
	 */
    CControlUI* GetFocus() const;

	/**
	  \brief ���ÿؼ�Ϊ��ȡ����״̬
	 */
    void SetFocus(CControlUI* pControl);
    
	/**
	  \brief ���ÿ���Ϊ��Ҫ���ƽ���
	 */
	void SetFocusNeeded(CControlUI* pControl);

	/**
	  \brief ������һ�����Tab�����ý���Ŀؼ�,Tab�Ƿ����������
	 */
    bool SetNextTabControl(bool bForward = true);

	/**
	  \brief Ϊָ���ؼ������ӿؼ����ö�ʱ��
	 */
    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    
	/**
	  \brief �Ƴ�ָ���ؼ��ϵ�ָ����ŵĶ�ʱ��
	 */
	bool KillTimer(CControlUI* pControl, UINT nTimerID);

	/**
	  \brief �Ƴ�ָ���ؼ��Ķ�ʱ��
	 */
	void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

	/**
	  \brief ���ô����������¼�
	 */
    void SetCapture();

	/**
	  \brief �ͷŴ��岶������¼�
	 */
    void ReleaseCapture();

	/**
	  \brief �жϴ����Ƿ��������¼�
	 */
    bool IsCaptured();

	/**
	  \brief ��ӿؼ���֪ͨ������
	 */
    bool AddNotifier(INotifyUI* pControl);

	/**
	  \brief ���ؼ���֪ͨ�������Ƴ�
	 */
    bool RemoveNotifier(INotifyUI* pControl);   

	/**
	  \brief ����ͬ��/�첽֪ͨ
	 */
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);

	/**
	  \brief ����ͬ��/�첽֪ͨ������
	 */
    void SendNotify(CControlUI* pControl, 
		LPCTSTR pstrMessage, 
		WPARAM wParam = 0, 
		LPARAM lParam = 0, 
		bool bAsync = false);

	/**
	  \brief ��Ԥ������Ϣ���������������Ϣ������
	 */
    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    
	/**
	  \brief ��Ԥ������Ϣ�����������Ƴ�ָ������Ϣ������
	 */
	bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief ����Ϣ���������������Ϣ������
	 */
    bool AddMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief ����Ϣ�����������Ƴ���Ϣ������
	 */
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief ��ȡ������Ҫ����Ŀؼ�������
	 */
    int GetPostPaintCount() const;

	/**
	  \brief ��������󼯺������Ҫ���ƵĿؼ�
	 */
    bool AddPostPaint(CControlUI* pControl);

	/**
	  \brief �ӻ������󼯺����Ƴ�ָ���ؼ�
	 */
    bool RemovePostPaint(CControlUI* pControl);

	/**
	  \brief ����������ؼ����뵽�������󼯺ϵ�ָ��λ��
	 */
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

	/**
	  \brief ���ӳ��������������Ҫ�ӳ�����Ķ���
	 */
    void AddDelayedCleanup(CControlUI* pControl);

	/**
	  \brief ��ȡ�����ؼ�
	 */
    CControlUI* GetRoot() const;

	/**
	  \brief �Ӹ��ڵ����ָ�������ڵĿؼ�
	 */
    CControlUI* FindControl(POINT pt) const;

	/**
	  \brief �Ӹ��ڵ㿪ʼ����ָ�����ƵĿؼ�
	 */
    CControlUI* FindControl(LPCTSTR pstrName) const;
	
	/**
	  \brief ��ָ���ڵ㿪ʼ����ָ�������ڵĿؼ�
	 */
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    

	/**
	  \brief ��ָ���ڵ㿪ʼ����ָ�����ƵĿؼ�
	 */
	CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
	CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
	CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();

	/**
	  \brief ��Ϣѭ��������Ϸ�����Ϣ�ã��޷���������Ϣ�Ŀ���ʱ��
	 */
    static void MessageLoop();

	/**
	  \brief ��Ϣ���룬��Win32ԭ�е���Ϣת�������ϣ�����Ҫ�Լ��������Ϣת��
	         ����ϢԤ������
	 */
    static bool TranslateMessage(const LPMSG pMsg);

	/**
	  \brief ��Ϣ����
	 */
    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
    
	/**
	  \brief ��ϢԤ������
	         1. ��ϢԤ�������(��ϢԤ������������϶���Ϣ���й��˴���)
			 2. ����Ƿ���Tab����������һ����ý���Ŀؼ�
			 3. ����Alt+Shortcut Key���º�Ŀؼ���ȡ����ͼ��������
			 4. ����Ƿ���ϵͳ��Ϣ�������ͻ�ȡ����Ŀؼ����¼�
	 */
	bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

private:
	/**
	  \brief �����еĿؼ���ӵ�m_mNameHash��ϣ����
	 */
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief ����ؼ�����
	 */
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief ���ݵ��Ƿ��������в�ѯ�ؼ�
	 */
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief ͨ��Tab��Ϣ��ѯ�ؼ�
	 */
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief �ӿ����в�ѯ�ؼ�
	 */
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief ������Ҫ���¿ؼ�
	 */
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief ͨ�����ƱȽϲ�ѯ�ؼ�
	 */
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:

	/**
	  \brief ҪCPaintManagerUI����Direct��ͼ�����Ĵ�����
	 */
    HWND m_hWndPaint;

	/**
	  \brief ��ʾ���ھ��
	 */
	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;

	/**
	  \brief ֱ�ӻ��Ƶ������DC(Ϊ�����������������ѿͻ���)
	 */
	HDC m_hDcPaint;

	/**
	  \brief �ڴ��ͼ����ͼDC
	 */
    HDC m_hDcOffscreen;

	/**
	  \brief ��������(֧��AlphaBackgroundʱʹ��)
	 */
    HDC m_hDcBackground;

	/**
	  \brief m_hDcPaint�ĺ�����ͼ����
	 */
    HBITMAP m_hbmpOffscreen;

	/**
	  \brief ����ͼƬbmp
	 */
    HBITMAP m_hbmpBackground;

	/**
	  \brief �Ƿ���ʾ��������
	 */
    bool m_bShowUpdateRect;
    
	/**
	  \brief xml���ڵ�����ɵĶ���,ͨ��Ϊ����Window
	 */
    CControlUI* m_pRoot;

	/**
	  \brief ���ڻ�ý���״̬�Ŀؼ�
	 */
    CControlUI* m_pFocus;

	/**
	  \brief ���������ͣ״̬�Ŀؼ�
	 */
    CControlUI* m_pEventHover;

	/**
	  \brief ��������Ŀؼ�
	 */
    CControlUI* m_pEventClick;

	/**
	  \brief ���ռ�������Ŀؼ�
	 */
    CControlUI* m_pEventKey;
    
	/**
	  \brief �������λ��
	 */
    POINT m_ptLastMousePos;

	/**
	  \brief ���ô���ɵ���������С��С
	 */
    SIZE m_szMinWindow;

	/**
	  \brief ���ô���ɵ�����������С
	 */
    SIZE m_szMaxWindow;

	/**
	  \brief �����ʼ��ʱ�Ĵ�С
	 */
    SIZE m_szInitWindowSize;

	/**
	  \brief ������߿�����Ĵ�С
	 */
    RECT m_rcSizeBox;

	/**
	  \brief �����Ľǵ�Բ�ǻ���
	 */
    SIZE m_szRoundCorner;

	/**
	  \brief ��������������С
	 */
	RECT m_rcCaption;

	/**
	  \brief ��ǰ��ʱ��ID
	 */
    UINT m_uTimerID;

	/**
	  \brief �Ƿ����׸�����
	 */
    bool m_bFirstLayout;

	/**
	  \brief �Ƿ���Ҫ���½���
	 */
    bool m_bUpdateNeeded;

	/**
	  \brief �Ƿ���Ҫ����
	 */
    bool m_bFocusNeeded;

	/**
	  \brief �Ƿ���Ҫ��˫�����ͼ
	 */
    bool m_bOffscreenPaint;

	/**
	  \brief ���屳���Ƿ�֧��Alpahͨ��(��pngͼƬ�İ�͸��Ч��)
	 */
    bool m_bAlphaBackground;

	/**
	  \brief �Ƿ�֧�����׷��
	 */
    bool m_bMouseTracking;

	/**
	  \brief �Ƿ�֧����겶��
	 */
    bool m_bMouseCapture;

	/**
	  \brief �ܹ�����֪ͨ�Ķ��󼯺�
	 */
    CStdPtrArray m_aNotifiers;

	/**
	  \brief ��ʱ������
	 */
    CStdPtrArray m_aTimers;

	/**
	  \brief Ԥ������Ϣ����������
	 */
    CStdPtrArray m_aPreMessageFilters;

	/**
	  \brief ��Ϣ����������
	 */
    CStdPtrArray m_aMessageFilters;

	/**
	  \brief ���ͻ�������Ŀؼ�����
	 */
    CStdPtrArray m_aPostPaintControls;

	/**
	  \brief �ӳ�����Ķ��󼯺�
	 */
    CStdPtrArray m_aDelayedCleanup;

	/**
	  \brief �첽֪ͨ��Ϣ����
	 */
    CStdPtrArray m_aAsyncNotify;

    CStdPtrArray m_aFoundControls;
    
	/**
	  \brief ����HashMap
	 */
	CStdStringPtrMap m_mNameHash;

	/**
	  \brief ѡ����Map
	 */
    CStdStringPtrMap m_mOptionGroup;
    
	/**
	  \brief �ϼ�(����)��Դ��PaintManagerUI��ͼ������
	 */
    CPaintManagerUI* m_pParentResourcePM;

	/**
	  \brief Ĭ��ʧЧ״̬��ɫ
	 */
    DWORD m_dwDefaultDisabledColor;

	/**
	  \brief Ĭ��������ɫ
	 */
    DWORD m_dwDefaultFontColor;

	/**
	  \brief Ĭ������������ɫ
	 */
    DWORD m_dwDefaultLinkFontColor;

	/**
	  \brief Ĭ�ϳ����������ͣ״̬��������ɫ
	 */
    DWORD m_dwDefaultLinkHoverFontColor;

	/**
	  \brief Ĭ��ѡ��״̬����ɫ
	 */
    DWORD m_dwDefaultSelectedBkColor;

	/**
	  \brief Ĭ��������Ϣ
	 */
    TFontInfo m_DefaultFontInfo;

	/**
	  \brief �Զ���������Դ����
	 */
    CStdPtrArray m_aCustomFonts;

	/**
	  \brief ͼƬ��ԴHashMap
	 */
    CStdStringPtrMap m_mImageHash;

	/**
	  \brief Ĭ������HashMap
	 */
    CStdStringPtrMap m_DefaultAttrHash;
    
	/**
	  \brief ��ǰ�����Instanceʵ��
	 */
    static HINSTANCE m_hInstance;

	/**
	  \brief ��ǰ�������ԴDLL Instance���
	 */
    static HINSTANCE m_hResourceInstance;

	/**
	  \brief ��ǰʹ�õ���Դ·��
	 */
    static CStdString m_pStrResourcePath;

	/**
	  \brief ��ǰʹ�õ���Դѹ�����ļ�����
	 */
    static CStdString m_pStrResourceZip;
    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
};

} // namespace DuiLib

#endif // __UIMANAGER_H__
