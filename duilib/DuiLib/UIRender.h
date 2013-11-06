#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#pragma once

namespace DuiLib {

/**
  \brief ��������,
         ������dc�ϻ��Ƶ�����,��ֹ�ӿؼ����Ƶ����ؼ����,float���Կؼ�����
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
	  \brief ����һ����������, hDC���ھ��,rc����,clip[out]������
	 */
    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    
	/**
	  \brief ����һ��Բ�Ǽ�������,hDC���ھ��, rc����, width ����Բ�ǻ���, height����Բ�ǻ���, clip[out]������
	 */
	static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    
	/**
	  \brief ʹ��������Ծ������Ϊ������, hDC���ھ��, clip������
	 */
	static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    
	/**
	  \brief ʹ�õ�ǰ�ؼ���Ч������Ϊ������, hDC���ھ��, clip������
	 */
	static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/**
  \brief ��Ⱦ����
 */
class UILIB_API CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
    
	/**
	  \brief ����ͼ��
	         1. ��ȡ�ļ�,��Դ��zip����ͼ���ĸ����ݵ��ڴ�,ͨ��HIWORD(bitmap.m_lpstr)�������Դ�л��ļ��м���,
			    MAKEINTRESOURCE  The return value is the specified value in the low-order word and zero in the high-order word
			 2. ����DIB
			 3. ͨ��stbi_load_from_memory���������ļ���ת�����Ƶ�DIB
			 4. ����TImageInfo�ṹ
	  \param[in] bitmap ͼ��
	  \param[in] type   ����
	  \param[in] mask   ����ɫ
	 */
	static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);

	/**
	  \brief ����ͼ��
	  \param[in] hDC          �豸���
	  \param[in] hBitmap      λͼ���
	  \param[in] rc           ��������
	  \param[in] rcPaint      ��������
	  \param[in] rcBmpPart    λͼ����
	  \param[in] rcConrners   ��������
	  \param[in] alphaChannel alphaͨ��
	  \param[in] uFade        ͸����
	  \param[in] hole         ����
	  \param[in] xtiled       ����ƽ��
	  \param[in] ytiled       ����ƽ�� 
	 */
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    
	/**
	  \brief ����ʶ�ַ�������ͼ��
	  \param[in] hDC       �豸���
      \param[in] pManager  ���ƹ�����
      \param[in] rc        ��������
      \param[in] rcPaint   ��������
      \param[in] pStrImage ��ʶ�ַ���
	  \param[in] pStrMoify ��ʶ�ַ���
	 */
	static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

	/**
	  \brief ���ƾ���,�������Ҫalpah�����SetBkColor() ExtTextOut(),
	         ���򴴽�һ�����ش�СΪ32λDIB
	  \param[in] hDC   �豸���
      \param[in] rc    ��������
      \param[in] color ��ɫ(alpha, r, g, b)
	 */
    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);

	/**
	  \brief ���ƽ���
	  \param[in] hDC       �豸���
	  \param[in] rc        ��������
      \param[in] dwFirst   ��ɫ1
	  \param[in] dwSecond  ��ɫ2
	  \param[in] bVertical ���䷽�� true ��ֱ flase ˮƽ
	  \param[in] nSteps    ����[ʹ��GradientFill() APIʱ��Ч]
	 */
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

	/**
	  \brief ����ֱ��
	         ���º����е���ɫ����alphaֵ��Ч
	 */
    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    
	/**
	  \brief ���ƾ���
	 */
	static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    
	/**
	  \brief ����Բ�Ǿ���
	 */
	static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
    
	/**
	  \brief ��������
	 */
	static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    
	/**
	  \brief ����"mini-html"��ʶ�ַ���
	 */
	static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, RECT* pLinks, CStdString* sLinks, int& nLinkRects, UINT uStyle);
    
	/**
	  \brief ����һ��BMPλͼ
	  \param[in] pManager ���ƹ�����
      \param[in] pControl �ؼ����
      \param[in] rc       ��������
	 */
	static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
