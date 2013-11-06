#ifndef __UIBASE_H__
#define __UIBASE_H__
#pragma once

namespace DuiLib {

/**
  \brief 容器样式,无任何样式
 */
#define UI_WNDSTYLE_CONTAINER  (0)

/**
  \brief UI标准框架窗体样式(标题栏、边框、图标、标题、最大、最小按钮、关闭按钮)
 */
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)

/**
  \brief UI窗体子控件样式
 */
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

/**
  \brief UI对话框窗体样式
 */
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

/**
  \brief 窗体客户区凹陷样式
 */
#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)

/**
  \brief 工具栏窗口样式+模式对话框样式
 */
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

/**
  \brief 类容器样式
 */
#define UI_CLASSSTYLE_CONTAINER  (0)

/**
  \brief 水平、垂直大小改变时绘制
 */
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)

/**
  \brief 水平、垂直大小改变时绘制、支持双击事件、保存窗体位图
 */
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

/**
  \brief 水平、垂直大小改变时绘制、支持双击事件、保存窗体位图
 */
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

/**
  \brief 达式是否为NULL或FALSE
 */
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

/**
  \brief 在Debug输出窗口打印格式化字符串
 */
#ifdef _DEBUG
#ifndef TRACE
#define TRACE __Trace
#endif
#define TRACEMSG __TraceMsg
#else
#ifndef TRACE
#define TRACE
#endif
#define TRACEMSG _T("")
#endif

void UILIB_API __Trace(LPCTSTR pstrFormat, ...);
LPCTSTR UILIB_API __TraceMsg(UINT uMsg);

/**
  \brief 用于构建资源名称或资源ID号,
         字符串类型和整型统一输出为字符串类型
 */
class STRINGorID
{
public:
    STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
    { }
    STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
    { }
    LPCTSTR m_lpstr;
};

/**
  \brief 点的封装类,支持LPARAM构建点
 */
class UILIB_API CPoint : public tagPOINT
{
public:
    CPoint();
    CPoint(const POINT& src);
    CPoint(int x, int y);
    CPoint(LPARAM lParam);
};

/**
  \brief 大小封装
 */
class UILIB_API CSize : public tagSIZE
{
public:
    CSize();
    CSize(const SIZE& src);
    CSize(const RECT rc);
    CSize(int cx, int cy);
};

/**
  \brief 区域封装
 */
class UILIB_API CRect : public tagRECT
{
public:
    CRect();
    CRect(const RECT& src);
    CRect(int iLeft, int iTop, int iRight, int iBottom);

	/**
	  \brief 获取宽度
	 */
    int GetWidth() const;

	/**
	  \brief 获取高度
	 */
    int GetHeight() const;
    
	/**
	  \brief 清空
	 */
	void Empty();

	/**
	  \brief 是否为空
	 */
    bool IsNull() const;

	/**
	  \brief 支持区域合并
	 */
    void Join(const RECT& rc);
    void ResetOffset();
    void Normalize();

	/**
	  \brief 区域偏移,cx为横向移动距离,cy为纵向移动距离
	 */
    void Offset(int cx, int cy);

	/**
	  \brief 区域膨胀,cx左右各放大的大小,cy为上下各方大的大小
	 */
    void Inflate(int cx, int cy);

	/**
	  \brief 区域收缩,cy为左右各缩小大小，cy上下各缩小大小
	 */
    void Deflate(int cx, int cy);

	/**
	  \brief 区域合并操作
	 */
    void Union(CRect& rc);
};

/**
  \brief 指针数组
 */
class UILIB_API CStdPtrArray
{
public:
    CStdPtrArray(int iPreallocSize = 0);
	CStdPtrArray(const CStdPtrArray& src);
    ~CStdPtrArray();

	/**
	  \brief 清空记录
	 */
    void Empty();

	/**
	  \brief 重新分配内存大小
	 */
    void Resize(int iSize);

	/**
	  \brief 是否为空数组
	 */
    bool IsEmpty() const;

	/**
	  \brief 查找指定元素的索引号
	 */
    int Find(LPVOID iIndex) const;

	/**
	  \brief 在指针集合中添加新数据
	 */
    bool Add(LPVOID pData);
    bool SetAt(int iIndex, LPVOID pData);
    bool InsertAt(int iIndex, LPVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID* GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPVOID* m_ppVoid;
    int m_nCount;
    int m_nAllocated;
};

/**
  \brief 数据数组
 */
class UILIB_API CStdValArray
{
public:
    CStdValArray(int iElementSize, int iPreallocSize = 0);
    ~CStdValArray();

    void Empty();
    bool IsEmpty() const;
    bool Add(LPCVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPBYTE m_pVoid;
    int m_iElementSize;
    int m_nCount;
    int m_nAllocated;
};

/**
  \brief 字符串数组
 */
class UILIB_API CStdString
{
public:
    enum { MAX_LOCAL_STRING_LEN = 63 };

    CStdString();
    CStdString(const TCHAR ch);
    CStdString(const CStdString& src);
    CStdString(LPCTSTR lpsz, int nLen = -1);
    ~CStdString();

    void Empty();
    int GetLength() const;
    bool IsEmpty() const;
    TCHAR GetAt(int nIndex) const;
    void Append(LPCTSTR pstr);
    void Assign(LPCTSTR pstr, int nLength = -1);
    LPCTSTR GetData();

    void SetAt(int nIndex, TCHAR ch);
    operator LPCTSTR() const;

    TCHAR operator[] (int nIndex) const;
    const CStdString& operator=(const CStdString& src);
    const CStdString& operator=(const TCHAR ch);
    const CStdString& operator=(LPCTSTR pstr);
#ifndef _UNICODE
    const CStdString& CStdString::operator=(LPCWSTR lpwStr);
#endif
    CStdString operator+(const CStdString& src);
    CStdString operator+(LPCTSTR pstr);
    const CStdString& operator+=(const CStdString& src);
    const CStdString& operator+=(LPCTSTR pstr);
    const CStdString& operator+=(const TCHAR ch);

    bool operator == (LPCTSTR str) const;
    bool operator != (LPCTSTR str) const;
    bool operator <= (LPCTSTR str) const;
    bool operator <  (LPCTSTR str) const;
    bool operator >= (LPCTSTR str) const;
    bool operator >  (LPCTSTR str) const;

    int Compare(LPCTSTR pstr) const;
    int CompareNoCase(LPCTSTR pstr) const;

    void MakeUpper();
    void MakeLower();

    CStdString Left(int nLength) const;
    CStdString Mid(int iPos, int nLength = -1) const;
    CStdString Right(int nLength) const;

    int Find(TCHAR ch, int iPos = 0) const;
    int Find(LPCTSTR pstr, int iPos = 0) const;
    int ReverseFind(TCHAR ch) const;
    int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

    int __cdecl Format(LPCTSTR pstrFormat, ...);
    int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

protected:
    LPTSTR m_pstr;
    TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};


/////////////////////////////////////////////////////////////////////////////////////
//

struct TITEM
{
    CStdString Key;
    LPVOID Data;
    struct TITEM* pNext;
};

/**
  \brief 字符串指针映射数组
 */
class UILIB_API CStdStringPtrMap
{
public:
    CStdStringPtrMap(int nSize = 83);
    ~CStdStringPtrMap();

    void Resize(int nSize = 83);
    LPVOID Find(LPCTSTR key) const;
    bool Insert(LPCTSTR key, LPVOID pData);
    LPVOID Set(LPCTSTR key, LPVOID pData);
    bool Remove(LPCTSTR key);
    int GetSize() const;
    LPCTSTR GetAt(int iIndex) const;
    LPCTSTR operator[] (int nIndex) const;

protected:
    TITEM** m_aT;
    int m_nBuckets;
};

/**
  \brief 窗口对象管理父类,主要作用列举如下:
      1. 创建窗口
	  2. 窗口消息过程处理
	  3. 提供窗口子类化与超类化接口
 */
class UILIB_API CWindowWnd
{
public:
    CWindowWnd();

    HWND GetHWND() const;
    operator HWND() const;

    bool RegisterWindowClass();
    bool RegisterSuperclass();

    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
    HWND Subclass(HWND hWnd);
    void Unsubclass();
    void ShowWindow(bool bShow = true, bool bTakeFocus = true);
    bool ShowModal();
    void Close();
    void CenterWindow();
    void SetIcon(UINT nRes);

    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    void ResizeClient(int cx = -1, int cy = -1);

protected:
    virtual LPCTSTR GetWindowClassName() const = 0;
    virtual LPCTSTR GetSuperClassName() const;
    virtual UINT GetClassStyle() const;

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnFinalMessage(HWND hWnd);

    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;
    WNDPROC m_OldWndProc;
    bool m_bSubclassed;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CWaitCursor
{
public:
    CWaitCursor();
    ~CWaitCursor();

protected:
    HCURSOR m_hOrigCursor;
};

} // namespace DuiLib

#endif // __UIBASE_H__
