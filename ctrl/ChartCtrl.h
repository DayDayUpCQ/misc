#pragma once
#include <afxwin.h>
#include "MessageFilter.h"
#include "DisplayConfig.h"
#include <vector>

class ChartAttr
{
public:
	int x;              /**< ���ҳ���x���� */
	int y;              /**< ���ҳ���y���� */
	int width;          /**< ��������ͼ��ռ������,������ǩ�̶ȵ� */
	int height;         /**< ��������ͼ��ռ����߶�,������ǩ�̶ȵ� */
	CString baseTitle;  /**< ����(V��A��Hz)����ͼ�����ֵ�Ĵ�С�Զ����ǰ׺ */
	int xSpan;          /**< ʱ��̶ȸ���,��λ���� */
	int ySpan;          /**< y��̶ȸ��� */
	int edgeWidth;      /**< ��ʾ���������߿��� */
	BOOL hasXScale;     /**< x���Ƿ�ӵ�п̶� */
	BOOL hasYTitle;     /**< y���Ƿ�ӵ�е�λ��ǩ */

	ChartAttr()
	{
		x = CHART_POS_X;
		y = CHART_POS_Y;
		baseTitle = "V";
		width = CHART_WIDTH;
		height = CHART_HEIGHT;
		xSpan = CHART_X_SPAN;
		ySpan = CHART_Y_SPAN;
		edgeWidth = CHART_EDGE_WIDTH;
		hasXScale = TRUE;
		hasYTitle = TRUE;
	}
};

class ChartDetailAttr : public ChartAttr
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
  \brief ����ͼ����
 */
class CurveData
{
public:
	/**
	  \brief ���߳�����,����Ϊÿ��������ȡһ�����ֵ����Сֵ����ӽ��ķ�ʽ��ʾ��������
     */
	class SamplePoint
	{
	public:
		float value1;
		float value2;
	};

	/**
	  \brief ����������
	 */
	class SampleHelper
	{
	public:
		int haveReadNum;    /**< �Ѹ������ݸ���,���������ߵķֱ��ʱȽ� */
		int sampleNum;      /**< ����������С */
		float tmpMax;       /**< ��ʱ���ֵ */
		float tmpMin;       /**< ��ʱ��Сֵ */
		int orderMax;       /**< ��ʱ���ֵ�����˳��� */
		int orderMin;       /**< ��ʱ��Сֵ�����˳��� */
	};

	explicit CurveData();

	CurveData(int capacity, int sampleNum);

	~CurveData();

	void AllocBuffer(const int capacity, const int sampleNum);
	
	void FreeBuffer();

	/**
	  \brief �Գ���ģʽ׷������,��������ʱɾ�����������
	 */
	void AppendData(const float data);

	float GetMax() const;

	float GetMin() const;

	int GetNum() const;

	void GetSamplePoint(const int idx, SamplePoint *point) const;

	void Clear();

	void SetColor(const COLORREF color);

	COLORREF GetColor() const;

private:

	SamplePoint *m_circleBuffer; /**< �������ݵ���л�����������ͨ���ص� */

	int m_capacity;              /**< �������� */

	int m_header;                /**< ����ͷָ�� */
	
	int m_tail;                  /**< ����βָ�� */

	int m_num;                   /**< ���������ݵ�ĸ��� */

	float m_max;                 /**< �����е����ֵ */

	float m_min;                 /**< �����е���Сֵ */

	BOOL m_haveInitMaxAndMin;      /**< �Ƿ��ѳ�ʼ����������Сֵ */

	COLORREF m_color;              /**< ������ɫ */

	SampleHelper m_sampleHelper; /**< ���������� */
};

/**
  \brief ��������ͼ
 */
class CChartCtrl : public CStatic, public MessageFilter
{
public:

	DECLARE_DYNAMIC(CChartCtrl)
	
	CChartCtrl();

	virtual ~CChartCtrl();

	BOOL Init(const ChartAttr *attr);

	BOOL Uninit();

	/**
	  \brief ��������
	  \param[in] curveDataArr �������ݻ���
	  \param[in] arrSize      ��������صĸ���
	 */
	void DrawCurve(const CurveData *curveDataArr, const int arrSize);

	/**
	  \brief ��ȡ���߳���������С
	 */
	int GetSampleNum();

	/**
	  \brief ��ȡ���߿����ɳ����������
	 */
	int GetSamplePointCapacity();

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
	ChartDetailAttr m_detailAttr;

	/**
	  \brief �������ߵ���ʱ�����ռ�
	 */
	std::vector<CPoint> m_curvePixelPoint;
};