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
    HFONT hFont;             /**< 字体句柄 */
    CStdString sFontName;    /**< 字体名称 */
    int iSize;               /**< 字号 */
    bool bBold;              /**< 是否加粗 */
    bool bUnderline;         /**< 是否有下划线 */
    bool bItalic;            /**< 是否为斜体 */
    TEXTMETRIC tm;           /**< 该字体的TEXTMETRIC信息 */
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
  \brief 事件通知抽象类,主要作用如下：
         1. 重载Notify虚函数数，处理事件通知
     
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
  \brief 窗口消息及图形绘制管理器类,与窗口绑定作用列举如下:
         1. 绘制控件
		 2. 消息管理
		 3. 事件通知
 */
class UILIB_API CPaintManagerUI
{
public:

    CPaintManagerUI();
    
	~CPaintManagerUI();

public:

	/**
	  \brief 绘图管理器初始化
	         m_hWndPaint, m_hDcPaint赋值,在预处理消息中加入管理器)
	 */
	void Init(HWND hWnd);
   
	/**
	  \brief 当前需要更新界面
	 */
	void NeedUpdate();

	/**
	  \brief 指定区域失效
	 */
	void Invalidate(RECT& rcItem);

	/**
	  \brief 获取绘图设备DC
	 */
    HDC GetPaintDC() const;

	/**
	  \brief 获取绘图窗口句柄 
	 */
	HWND GetPaintWindow() const;

	/**
	  \brief 获取提示窗口句柄
	 */
	HWND GetTooltipWindow() const;

	/**
	  \brief 获取当前鼠标的位置 
	 */
    POINT GetMousePos() const;

	/**
	  \brief 获取客户区大小
	 */
	SIZE GetClientSize() const;

	/**
	  \brief 获取和设置窗体初始化的大小
	 */
	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);

	/**
	  \brief 获取和设置窗体边框区域大小
	 */
	RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);

	/**
	  \brief 获取和设置标题区域位置
	 */
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);

	/**
	  \brief 获取和设置窗体四角的圆角弧度
	 */
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);

	/**
	  \brief 设置和获取窗体可调整到的最小大小
	 */
	SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    
	/**
	  \brief 获取和设置窗体可调整到的最大大小
	 */
	SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
    
	/**
	  \brief 设置窗体的透明度(0完全透明-255完全不透明)
	 */
	void SetTransparent(int nOpacity);

	/**
	  \brief 设置绘图是否支持透明处理
	 */
    void SetBackgroundTransparent(bool bTrans);
    
	/**
	  \brief 是否显示更新区域
	 */
	bool IsShowUpdateRect() const;
    
	/**
	  \brief 设置是否显示更新
	 */
	void SetShowUpdateRect(bool show);

	/**
	  \brief 获取和设置当前管理的实例句柄
	 */
    static HINSTANCE GetInstance();
	static void SetInstance(HINSTANCE hInst);

	/**
	  \brief 获取当前运行的实例的路径
	 */
    static CStdString GetInstancePath();

	/**
	  \brief 获取和设置当前的工作路径
	 */
    static CStdString GetCurrentPath();
	static void SetCurrentPath(LPCTSTR pStrPath);
    
	/**
	  \brief 获取资源DLL的实例句柄
	 */
	static HINSTANCE GetResourceDll();
	static void SetResourceDll(HINSTANCE hInst);
    
	/**
	  \brief 获取资源路径(以'\'结尾)
	 */
	static const CStdString& GetResourcePath();
	static void SetResourcePath(LPCTSTR pStrPath);
    
	/**
	  \brief 获取Zip资源的路径
	 */
	static const CStdString& GetResourceZip();
	static void SetResourceZip(LPCTSTR pStrZip);

    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
    static void ReloadSkin();

	/**
	  \brief 设置使用上级资源的绘图管理器
	 */
    bool UseParentResource(CPaintManagerUI* pm);
    /**
      \brief 获取上级资源的绘图管理器
     */
	CPaintManagerUI* GetParentResource() const;

	/**
	  \brief 获取和设置禁用状态的默认颜色
	 */
    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    
	/**
	  \brief 获取和设置字体默认颜色
	 */
	DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    
	/**
	  \brief 获取和设置链接文字的默认颜色
	 */
	DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    
	/**
	  \brief 获取鼠标悬停与超链接上的默认字体的颜色
	 */
	DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    
	/**
	  \brief 获取和设置选中状态的默认背景颜色
	 */
	DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    
	/**
	  \brief 获取默认使用的字体信息
	 */
	TFontInfo* GetDefaultFontInfo();
    
	/**
	  \brief 设置默认使用的字体信息
	 */
	void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief 获取用户自定义字体的数量 
	 */
	DWORD GetCustomFontCount() const;
    
	/**
	  \brief 向字体数组列表中追加字体资源
	 */
	HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/**
	  \brief 向字体数组列表中插入字体资源
	 */
	HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief 获取数组中指定下标的字体对象句柄
	 */
	HFONT GetFont(int index);

	/**
	  \brief 从字体数组中获取指定配置的字体对象句柄
	 */
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief 查看字体数组集合中是否存在字体对象
	 */
	bool FindFont(HFONT hFont);

	/**
	  \brief 查看字体数组集合中是否存在指定配置的字体对象
	 */
    bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief 获取字体对象的索引号
	 */
	int GetFontIndex(HFONT hFont);

	/**
	  \brief 获取指定配置的字体对象的索引号
	 */
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    
	/**
	  \brief 从字体数组中删除字体对象
	 */
	bool RemoveFont(HFONT hFont);

	/**
	  \brief 删除指定索引号的字体对象
	 */
    bool RemoveFontAt(int index);

	/**
	  \brief 清空字体数组列表
	 */
    void RemoveAllFonts();

	/**
	  \brief 获取字体对象信息
	 */
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);

	/**
	  \brief 查看图像信息
	 */
    const TImageInfo* GetImage(LPCTSTR bitmap);
    const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	/**
	  \brief 添加图像
	 */
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	/**
	  \brief 添加图象
	 */
	const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    
	/**
	  \brief 根据图像名称移除图像
	 */
	bool RemoveImage(LPCTSTR bitmap);
    
	/**
	  \brief 移除全部图像
	 */
	void RemoveAllImages();
    void ReloadAllImages();

	/**
	  \brief 添加控件的默认配置信息
	 */
    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    
	/**
	  \brief 移除控制的默认配置
	 */
	bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    
	/**
	  \brief 获取默认配置信息列表
	 */
	const CStdStringPtrMap& GetDefaultAttribultes() const;
    void RemoveAllDefaultAttributeList();

	/**
	  \brief 将对话框附加到当前管理器中
	 */
    bool AttachDialog(CControlUI* pControl);
    
	/**
	  \brief 控件初始化
	 */
	bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    
	/**
	  \brief 控件回收
	 */
	void ReapObjects(CControlUI* pControl);

	/**
	  \brief 添加控件到指定的选项组
	 */
    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    
	/**
	  \brief 查询指定选项组中的全部选项
	 */
	CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    
	/**
	  \brief 从指定控件中移除指定选项组名称的选项组
	 */
	void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    
	/**
	  \brief 清空全部选项组列表
	 */
	void RemoveAllOptionGroups();

	/**
	  \brief 获取焦点状态的控件
	 */
    CControlUI* GetFocus() const;

	/**
	  \brief 设置控件为获取焦点状态
	 */
    void SetFocus(CControlUI* pControl);
    
	/**
	  \brief 设置控制为需要绘制焦点
	 */
	void SetFocusNeeded(CControlUI* pControl);

	/**
	  \brief 设置下一个获得Tab键会获得焦点的控件,Tab是否继续往下走
	 */
    bool SetNextTabControl(bool bForward = true);

	/**
	  \brief 为指定控件及其子控件设置定时器
	 */
    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    
	/**
	  \brief 移除指定控件上的指定编号的定时器
	 */
	bool KillTimer(CControlUI* pControl, UINT nTimerID);

	/**
	  \brief 移除指定控件的定时器
	 */
	void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

	/**
	  \brief 设置窗体接受鼠标事件
	 */
    void SetCapture();

	/**
	  \brief 释放窗体捕获鼠标事件
	 */
    void ReleaseCapture();

	/**
	  \brief 判断窗体是否接受鼠标事件
	 */
    bool IsCaptured();

	/**
	  \brief 添加控件到通知集合中
	 */
    bool AddNotifier(INotifyUI* pControl);

	/**
	  \brief 将控件从通知集合中移除
	 */
    bool RemoveNotifier(INotifyUI* pControl);   

	/**
	  \brief 发送同步/异步通知
	 */
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);

	/**
	  \brief 构造同步/异步通知并发送
	 */
    void SendNotify(CControlUI* pControl, 
		LPCTSTR pstrMessage, 
		WPARAM wParam = 0, 
		LPARAM lParam = 0, 
		bool bAsync = false);

	/**
	  \brief 向预处理消息过滤器链中添加消息过滤器
	 */
    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    
	/**
	  \brief 从预处理消息过滤器链中移除指定的消息过滤器
	 */
	bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief 向消息过滤器链中添加消息过滤器
	 */
    bool AddMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief 从消息过滤器链中移除消息过滤器
	 */
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

	/**
	  \brief 获取发送需要绘件的控件的数量
	 */
    int GetPostPaintCount() const;

	/**
	  \brief 向绘制请求集合中添加要绘制的控件
	 */
    bool AddPostPaint(CControlUI* pControl);

	/**
	  \brief 从绘制请求集合中移除指定控件
	 */
    bool RemovePostPaint(CControlUI* pControl);

	/**
	  \brief 将绘制请求控件插入到绘制请求集合的指定位置
	 */
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

	/**
	  \brief 向延迟清理集合中添加需要延迟清理的对象
	 */
    void AddDelayedCleanup(CControlUI* pControl);

	/**
	  \brief 获取根结点控件
	 */
    CControlUI* GetRoot() const;

	/**
	  \brief 从根节点查找指定点所在的控件
	 */
    CControlUI* FindControl(POINT pt) const;

	/**
	  \brief 从根节点开始查找指定名称的控件
	 */
    CControlUI* FindControl(LPCTSTR pstrName) const;
	
	/**
	  \brief 从指定节点开始查找指定点所在的控件
	 */
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    

	/**
	  \brief 从指定节点开始查找指定名称的控件
	 */
	CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
	CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
	CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();

	/**
	  \brief 消息循环，非游戏框架消息泵，无法利用无消息的空闲时间
	 */
    static void MessageLoop();

	/**
	  \brief 消息翻译，在Win32原有的消息转换基础上，将需要自己处理的消息转发
	         给消息预处理器
	 */
    static bool TranslateMessage(const LPMSG pMsg);

	/**
	  \brief 消息处理
	 */
    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
    
	/**
	  \brief 消息预处理器
	         1. 消息预处理过滤(消息预处理过滤器集合对消息进行过滤处理)
			 2. 检查是否按下Tab键，设置下一个获得焦点的控件
			 3. 处理Alt+Shortcut Key按下后的控件获取焦点和激活的设置
			 4. 检查是否有系统消息，有则发送获取焦点的控件的事件
	 */
	bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

private:
	/**
	  \brief 将所有的控件添加到m_mNameHash哈希表中
	 */
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 计算控件数量
	 */
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 根据点是否在区域中查询控件
	 */
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 通过Tab信息查询控件
	 */
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 从快照中查询控件
	 */
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 查找需要更新控件
	 */
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
    
	/**
	  \brief 通过名称比较查询控件
	 */
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:

	/**
	  \brief 要CPaintManagerUI进行Direct绘图操作的窗体句柄
	 */
    HWND m_hWndPaint;

	/**
	  \brief 提示窗口句柄
	 */
	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;

	/**
	  \brief 直接绘制到窗体的DC(为窗体的整个区域包括费客户区)
	 */
	HDC m_hDcPaint;

	/**
	  \brief 内存绘图区绘图DC
	 */
    HDC m_hDcOffscreen;

	/**
	  \brief 背景绘制(支持AlphaBackground时使用)
	 */
    HDC m_hDcBackground;

	/**
	  \brief m_hDcPaint的后能作图画布
	 */
    HBITMAP m_hbmpOffscreen;

	/**
	  \brief 背景图片bmp
	 */
    HBITMAP m_hbmpBackground;

	/**
	  \brief 是否显示更新区域
	 */
    bool m_bShowUpdateRect;
    
	/**
	  \brief xml根节点解析成的对象,通常为各种Window
	 */
    CControlUI* m_pRoot;

	/**
	  \brief 处于获得焦点状态的控件
	 */
    CControlUI* m_pFocus;

	/**
	  \brief 处理鼠标悬停状态的控件
	 */
    CControlUI* m_pEventHover;

	/**
	  \brief 被鼠标点击的控件
	 */
    CControlUI* m_pEventClick;

	/**
	  \brief 接收键盘输入的控件
	 */
    CControlUI* m_pEventKey;
    
	/**
	  \brief 鼠标最新位置
	 */
    POINT m_ptLastMousePos;

	/**
	  \brief 设置窗体可调整到的最小大小
	 */
    SIZE m_szMinWindow;

	/**
	  \brief 设置窗体可调整到的最大大小
	 */
    SIZE m_szMaxWindow;

	/**
	  \brief 窗体初始化时的大小
	 */
    SIZE m_szInitWindowSize;

	/**
	  \brief 窗体外边框区域的大小
	 */
    RECT m_rcSizeBox;

	/**
	  \brief 窗体四角的圆角弧度
	 */
    SIZE m_szRoundCorner;

	/**
	  \brief 窗体标题栏区域大小
	 */
	RECT m_rcCaption;

	/**
	  \brief 当前定时器ID
	 */
    UINT m_uTimerID;

	/**
	  \brief 是否是首个布局
	 */
    bool m_bFirstLayout;

	/**
	  \brief 是否需要更新界面
	 */
    bool m_bUpdateNeeded;

	/**
	  \brief 是否需要焦点
	 */
    bool m_bFocusNeeded;

	/**
	  \brief 是否需要开双缓存绘图
	 */
    bool m_bOffscreenPaint;

	/**
	  \brief 窗体背景是否支持Alpah通道(如png图片的半透明效果)
	 */
    bool m_bAlphaBackground;

	/**
	  \brief 是否支持鼠标追踪
	 */
    bool m_bMouseTracking;

	/**
	  \brief 是否支持鼠标捕获
	 */
    bool m_bMouseCapture;

	/**
	  \brief 能够接收通知的对象集合
	 */
    CStdPtrArray m_aNotifiers;

	/**
	  \brief 定时器集合
	 */
    CStdPtrArray m_aTimers;

	/**
	  \brief 预处理消息过滤器集合
	 */
    CStdPtrArray m_aPreMessageFilters;

	/**
	  \brief 消息过滤器集合
	 */
    CStdPtrArray m_aMessageFilters;

	/**
	  \brief 发送绘制请求的控件集合
	 */
    CStdPtrArray m_aPostPaintControls;

	/**
	  \brief 延迟清理的对象集合
	 */
    CStdPtrArray m_aDelayedCleanup;

	/**
	  \brief 异步通知消息集合
	 */
    CStdPtrArray m_aAsyncNotify;

    CStdPtrArray m_aFoundControls;
    
	/**
	  \brief 名称HashMap
	 */
	CStdStringPtrMap m_mNameHash;

	/**
	  \brief 选项组Map
	 */
    CStdStringPtrMap m_mOptionGroup;
    
	/**
	  \brief 上级(父类)资源的PaintManagerUI绘图管理器
	 */
    CPaintManagerUI* m_pParentResourcePM;

	/**
	  \brief 默认失效状态颜色
	 */
    DWORD m_dwDefaultDisabledColor;

	/**
	  \brief 默认字体颜色
	 */
    DWORD m_dwDefaultFontColor;

	/**
	  \brief 默认链接字体颜色
	 */
    DWORD m_dwDefaultLinkFontColor;

	/**
	  \brief 默认超链接鼠标悬停状态的字体颜色
	 */
    DWORD m_dwDefaultLinkHoverFontColor;

	/**
	  \brief 默认选中状态背景色
	 */
    DWORD m_dwDefaultSelectedBkColor;

	/**
	  \brief 默认字体信息
	 */
    TFontInfo m_DefaultFontInfo;

	/**
	  \brief 自定义字体资源集合
	 */
    CStdPtrArray m_aCustomFonts;

	/**
	  \brief 图片资源HashMap
	 */
    CStdStringPtrMap m_mImageHash;

	/**
	  \brief 默认属性HashMap
	 */
    CStdStringPtrMap m_DefaultAttrHash;
    
	/**
	  \brief 当前管理的Instance实例
	 */
    static HINSTANCE m_hInstance;

	/**
	  \brief 当前管理的资源DLL Instance句柄
	 */
    static HINSTANCE m_hResourceInstance;

	/**
	  \brief 当前使用的资源路径
	 */
    static CStdString m_pStrResourcePath;

	/**
	  \brief 当前使用的资源压缩包文件名称
	 */
    static CStdString m_pStrResourceZip;
    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
};

} // namespace DuiLib

#endif // __UIMANAGER_H__
