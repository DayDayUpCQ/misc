/**
  \file StsChartCtrl.h

  \brief ͳ������ͼ�ؼ�
 */
#pragma once
#include <afxwin.h>
#include "MessageFilter.h"
#include "DisplayConfig.h"
#include <vector>

/**
  \brief ͳ������ͼ����,��Ч�ʽϵ�ʱ�ɲ�����ʽ�����д�ò���
 */
class StsCurveData
{
public:

	StsCurveData();

	StsCurveData(int capacity);

	~StsCurveData();

	void AllocBuffer(const int capacity);
	
	void FreeBuffer();

	/**
	  \brief �Գ���ģʽ׷������
	         ��ʵʱ��������ͼ��ͬ������ͼÿ�γ���������������Ϊ3��ȡֵ���г���
	 */
	void AppendData(const float data);

	float GetMax() const;

	float GetMin() const;

	int GetNum() const;

	float GetData(const int idx) const;

	void Clear();

	void SetColor(const COLORREF color);

	COLORREF GetColor() const;

private:

	/**
	  \brief ��3������ֵΪ��������г���
	 */
	void SampleData();

	float *m_dataBuffer; /**< ���ݻ��� */ 
	
	int m_capacity;       /**< �������� */
	
	int m_num;            /**< ���������ݵĸ��� */
	
	float m_max;          /**< ���������ݵ����ֵ */

	float m_min;          /**< ���������ݵ���Сֵ */

	int m_nextSampleIdx;  /**< ��һ�γ�����������ʼλ�� */

	COLORREF m_color;     /**< ������ɫ */
};

class StsChartAttr
{
public:
	int x;              /**< ���ҳ���x���� */
	int y;              /**< ���ҳ���y���� */
	int width;          /**< ��������ͼ��ռ������,������ǩ�̶ȵ� */
	int height;         /**< ��������ͼ��ռ����߶�,������ǩ�̶ȵ� */
	CString baseTitle;  /**< ����(V��A��Hz)����ͼ�����ֵ�Ĵ�С�Զ����ǰ׺ */
	int ySpan;          /**< y��̶ȸ��� */
	int edgeWidth;      /**< ��ʾ���������߿��� */
	BOOL hasXScale;     /**< �Ƿ�ӵ��x��̶�,x�������ʼ�ͽ�������ʱ��̶� */
	BOOL hasYTitle;     /**< �Ƿ�ӵ��y�ᵥλ��ǩ */

	StsChartAttr()
	{
		x = STSCHART_POS_X;
		y = STSCHART_POS_Y;
		baseTitle = "V";
		width = STSCHART_WIDTH;
		height = STSCHART_HEIGHT;
		ySpan = STSCHART_Y_SPAN;
		edgeWidth = STSCHART_EDGE_WIDTH;
		hasXScale = TRUE;
		hasYTitle = TRUE;
	}
};

class StsChartDetailAttr : public StsChartAttr
{
public:
	int titleWidth;              /**< ��������Ŀ�� */
	int xScaleNoteHeight;        /**< x��̶���Ϣ��� */
	int yScaleNoteWidth;         /**< y��̶���Ϣ�߶� */
	int axisWidth;               /**< �������� */
	int axisHeight;              /**< ������߶� */
	int axisHeightTopPadding;    /**< y���϶�Ԥ������ */
	int axisHeightBottomPadding; /**< y���¶�Ԥ������ */
	int coordinateHeight;        /**< �������ϵ�����ĸ߶� */
	int rightPadding;            /**< �ұ߾� */
};

/**
  \brief ��������ͼ
 */
class CStsChartCtrl : public CStatic, public MessageFilter
{
public:

	DECLARE_DYNAMIC(CStsChartCtrl)
	
	CStsChartCtrl();

	virtual ~CStsChartCtrl();

	BOOL Init(const StsChartAttr *attr);

	BOOL Uninit();

	/**
	  \brief ��������
	  \param[in] StsCurveDataArr �������ݻ���
	  \param[in] arrSize         ��������صĸ���
	 */
	void DrawStsCurve(const StsCurveData *StsCurveDataArr, const int arrSize);

	void SetBaseTitle(const CString &strTitle);

	virtual void OnShow();

	virtual void OnHide();

	afx_msg void OnPaint();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:

	DECLARE_MESSAGE_MAP()
	
	/**
	  \brief ��������ͼ����
	 */
	BOOL DrawChart();

	/**
	  \brief ����x������̶�
	 */
	void DrawXScale(CDC &dc);
	
	/**
	  \brief ����x������̶ȵ�ֵ
	 */
	void DrawXScaleValue(CDC &dc);

	/**
	  \brief ����y������̶�
	 */
	void DrawYScale(CDC &dc);

	/**
	  \brief ��������y������,����y���ֵʹ��ÿ���̶�ֵ������ֵ��Ϊ����
	         ���Ҳ�����3λ
	 */
	void UpdateYAxisAttr(const float yMax, const float yMin);

	/**
	  \brief ����y������̶ȵ�ֵ
	 */
	void DrawYScaleValue(CDC &dc);

	/**
	  \brief ����y��ĵ�λ��ǩ
	 */
	void DrawYTitle(CDC &dc);

	/**
	  \brief ��������,�ķ�ʱ��ϳ�
	 */
	void PolyLineEx(CDC &dc, const CPoint *ptArr, const int arrSize, const COLORREF color);
	void UpdateCurve(int type, const StsCurveData *StsCurveDataArr, const int arrSize, const CString &strTitle);
private:
	/**
	  \brief �ڴ�DC
	 */
	CDC m_dcCurve;

	/**
	  \brief �������ֵ
	 */
	float m_yAxisMax;

	/**
	  \brief ������Сֵ
	 */
	float m_yAxisMin;

	/**
	  \brief ���߱���ǰ׺
	 */
	CString m_strTitlePrefix;

	/**
	  \brief ����λͼ
	 */
	CBitmap m_bmpBkgnd;

	/**
	  \brief ����λͼ
	 */
	CBitmap m_bmpCurve;

	/**
	  \brief ����DC
	 */
	CDC m_dcBkgnd;

	/**
	  \brief �������
	 */
	StsChartDetailAttr m_detailAttr;

	/**
	  \brief �������ߵ���ʱ�����ռ�
	 */
	std::vector<CPoint> m_curvePixelPoint;
};