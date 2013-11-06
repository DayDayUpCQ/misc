#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#pragma once

namespace DuiLib {

/**
  \brief 区域引擎,
         设置在dc上绘制的区域,防止子控件绘制到父控件外侧,float属性控件除外
 */
class UILIB_API CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

	/**
	  \brief 产生一个剪切区域, hDC窗口句柄,rc区域,clip[out]剪切区
	 */
    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    
	/**
	  \brief 产生一个圆角剪切区域,hDC窗口句柄, rc区域, width 横向圆角弧度, height纵向圆角弧度, clip[out]剪切区
	 */
	static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    
	/**
	  \brief 使用整个活跃区域作为剪切区, hDC窗口句柄, clip剪切区
	 */
	static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    
	/**
	  \brief 使用当前控件有效区域作为剪切区, hDC窗口句柄, clip剪切区
	 */
	static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/**
  \brief 渲染引擎
 */
class UILIB_API CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
    
	/**
	  \brief 加载图像
	         1. 读取文件,资源和zip包中图像文个数据到内存,通过HIWORD(bitmap.m_lpstr)区别从资源中或文件中加载,
			    MAKEINTRESOURCE  The return value is the specified value in the low-order word and zero in the high-order word
			 2. 创建DIB
			 3. 通过stbi_load_from_memory加载数据文件并转化后复制到DIB
			 4. 返回TImageInfo结构
	  \param[in] bitmap 图像
	  \param[in] type   类型
	  \param[in] mask   隐蔽色
	 */
	static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);

	/**
	  \brief 绘制图像
	  \param[in] hDC          设备句柄
	  \param[in] hBitmap      位图句柄
	  \param[in] rc           对象区域
	  \param[in] rcPaint      绘制区域
	  \param[in] rcBmpPart    位图区域
	  \param[in] rcConrners   拉伸区域
	  \param[in] alphaChannel alpha通道
	  \param[in] uFade        透明度
	  \param[in] hole         居中
	  \param[in] xtiled       横向平铺
	  \param[in] ytiled       纵向平铺 
	 */
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    
	/**
	  \brief 按标识字符串绘制图像
	  \param[in] hDC       设备句柄
      \param[in] pManager  绘制管理者
      \param[in] rc        对象区域
      \param[in] rcPaint   绘制区域
      \param[in] pStrImage 标识字符串
	  \param[in] pStrMoify 标识字符串
	 */
	static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

	/**
	  \brief 绘制矩形,如果不需要alpah混合则SetBkColor() ExtTextOut(),
	         否则创建一个像素大小为32位DIB
	  \param[in] hDC   设备句柄
      \param[in] rc    对象区域
      \param[in] color 颜色(alpha, r, g, b)
	 */
    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);

	/**
	  \brief 绘制渐变
	  \param[in] hDC       设备句柄
	  \param[in] rc        对象区域
      \param[in] dwFirst   颜色1
	  \param[in] dwSecond  颜色2
	  \param[in] bVertical 渐变方向 true 垂直 flase 水平
	  \param[in] nSteps    步长[使用GradientFill() API时无效]
	 */
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

	/**
	  \brief 绘制直线
	         以下函数中的颜色参数alpha值无效
	 */
    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    
	/**
	  \brief 绘制矩形
	 */
	static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    
	/**
	  \brief 绘制圆角矩形
	 */
	static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
    
	/**
	  \brief 绘制文字
	 */
	static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    
	/**
	  \brief 绘制"mini-html"标识字符串
	 */
	static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, RECT* pLinks, CStdString* sLinks, int& nLinkRects, UINT uStyle);
    
	/**
	  \brief 产生一个BMP位图
	  \param[in] pManager 绘制管理者
      \param[in] pControl 控件句柄
      \param[in] rc       对象区域
	 */
	static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
