#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"

class C360SafeFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	C360SafeFrameWnd() 
	{

	};

	LPCTSTR GetWindowClassName() const 
	{ 
		return _T("UIMainFrame"); 
	};

	UINT GetClassStyle() const { return CS_DBLCLKS; };
	
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() 
	{
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	}

	void OnPrepare() 
	{
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) 
		{
			OnPrepare();
		}
		else if( msg.sType == _T("click") ) 
		{
			if( msg.pSender == m_pCloseBtn ) 
			{
				PostQuitMessage(0);
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) 
			{ 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
				return; 
			}
			else if( msg.pSender == m_pMaxBtn ) 
			{ 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
				return; 
			}
			else if( msg.pSender == m_pRestoreBtn ) 
			{ 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
				return; 
			}
		}
		else if(msg.sType==_T("setfocus"))
		{
			CStdString name = msg.pSender->GetName();
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
			if(name==_T("examine"))
			{
				 pControl->SelectItem(0);
			}
			else if(name==_T("trojan"))
			{
				 pControl->SelectItem(1);
			}
			else if(name==_T("plugins"))
			{
				pControl->SelectItem(2);
			}
			else if(name==_T("vulnerability"))
			{
				pControl->SelectItem(3);
			}
			else if(name==_T("rubbish"))
			{
				pControl->SelectItem(4);
			}
			else if(name==_T("cleanup"))
			{
				pControl->SelectItem(5);
			}
			else if(name==_T("fix"))
			{
				pControl->SelectItem(6);
			}
			else if(name==_T("tool"))
			{
				pControl->SelectItem(7);
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		Init();
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left 
			&& pt.x < rcClient.right - rcCaption.right 
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
		{
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl 
					&& _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 
					&& _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 
					&& _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				{
					return HTCAPTION;
				}
		}
		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-rcWork.left, -rcWork.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) 
		{
			// 创建对话框
		case WM_CREATE:        
			lRes = OnCreate(uMsg, wParam, lParam, bHandled);
			break;

			// 关闭对话框
		case WM_CLOSE:         
			lRes = OnClose(uMsg, wParam, lParam, bHandled); 
			break;

			// 删除对话框
		case WM_DESTROY:       
			lRes = OnDestroy(uMsg, wParam, lParam, bHandled); 
			break;
			// 当改变窗口大小、移动窗口到屏幕边界时都会产生一个WM_NCPAINT消息
			// 要求NC区域重绘,但是当右键点击任务栏时，会弹出菜单，同时窗口的标题栏会变色，此时不会产生
			// 一个WM_NCACTIVE消息，系统会去重绘
			// 所以做界面皮肤时，处理WM_NCPAINT的同时还要处理WM_INACTIVE，否则，右键任务栏时，系统会
			// 画出默认的边框
		case WM_NCACTIVATE:    
			lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); 
			break;

			// 当窗口位置移动，Windows将会给窗口发送一个WM_NCCALCSIZE消息
			// 通知窗口的位置或大小改变了，应用程序应当指定新的非客户区和客户区
			// message:WM_NCCALCSIZE
            // wParam: TRUE
            // lParam: 一个指向三个矩形的指针（NCCALCSIZE_PARAMS *）。下面是NCCALCSIZE_PARAMS结构：
		    // typedef struct tagNCCALCSIZE_PARAMS {
   			//      RECT rgrc[3];
			//       PWINDOWPOS lppos;
		    // } NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;
		case WM_NCCALCSIZE:    
			lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); 
			break;
			
			// 当窗体框架(非客户区)要被重画时，WM_NCPAINT消息产生
		case WM_NCPAINT:       
			lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); 
			break;
			
			// 当鼠标移动或鼠标键按下时发出的消息
			// WM_NCHITTEST的消息会根据鼠标当前的坐标来判断鼠标命中窗口的哪个部位，
			// 消息咱应函数的返回值指出了部位，例如它可能会返回HTCAPTION，或者HTCLIENT
		case WM_NCHITTEST:     
			lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
			
			// 当窗口大小发生改变时，系统窗口发送WM_SIZE消息，WPARAM指出窗口新的状态,
			// LPARAM给出窗口新的大小,窗口的状态有三种SIZE_MINMIZE, SIZE_MAXIMIZE,SIZE_RESTORED
		case WM_SIZE:          
			lRes = OnSize(uMsg, wParam, lParam, bHandled); 
			break;

			// 无论什么时候以何种方式改变窗口的尺寸或大小，是拖拽到窗口边缘也好，或是在代码中调用改变窗口
			// 尺寸的函数也好，总之不管用什么方法,Windows都会首先发送WM_GETMINMAXINFO
			// 消息，该消息的意思是，如果改变窗口的尺寸或大小，就必须附上详细的MINMAXINFO结构体信息
			// 可解决应用程序无法全屏的错误
		case WM_GETMINMAXINFO: 
			lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); 
			break;
			// 当用户从窗口菜单选择一个命名或当用户选择最大化按钮，最小化按钮，复原按钮或关闭按钮时
			// 一个窗口将会接收到WM_SYSCOMMAND消息
		case WM_SYSCOMMAND:    
			lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); 
			break;

		default:
			bHandled = FALSE;
		}
		if( bHandled ) 
		{
			return lRes;
		}
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) 
		{
			return lRes;
		}
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	//...
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));

	C360SafeFrameWnd* pFrame = new C360SafeFrameWnd();
	if( pFrame == NULL ) return 0;
	// 创建对话框
	pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	// 将对话框移至屏幕中间
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	// 进行消息循环
	CPaintManagerUI::MessageLoop();
	return 0;
}