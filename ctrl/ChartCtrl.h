#pragma once
#include <afxwin.h>
#include "MessageFilter.h"
#include "DisplayConfig.h"
#include <vector>

class ChartAttr
{
public:
	int x;              /**< 相对页面的x坐标 */
	int y;              /**< 相对页面的y坐标 */
	int width;          /**< 整个趋势图所占区域宽度,包括标签刻度等 */
	int height;         /**< 整个趋势图所占区域高度,包括标签刻度等 */
	CString baseTitle;  /**< 标题(V、A、Hz)趋势图会根据值的大小自动添加前缀 */
	int xSpan;          /**< 时间刻度个数,单位分钟 */
	int ySpan;          /**< y轴刻度个数 */
	int edgeWidth;      /**< 表示坐标轴的外边框宽度 */
	BOOL hasXScale;     /**< x轴是否拥有刻度 */
	BOOL hasYTitle;     /**< y轴是否拥有单位标签 */

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
	int titleWidth;              /**< 标题区域的宽度 */
	int xScaleNoteHeight;        /**< x轴刻度信息宽度 */
	int yScaleNoteWidth;         /**< y轴刻度信息高度 */
	int axisWidth;               /**< 坐标轴宽度 */
	int axisHeight;              /**< 坐标轴高度 */
	int axisHeightTopPadding;    /**< y轴上端预留像素 */
	int axisHeightBottomPadding; /**< y轴下端预留像素 */
	int coordinateHeight;        /**< 坐标轴上的坐标的高度 */
	int rightPadding;            /**< 右边距 */
};

/**
  \brief 趋势图数据
 */
class CurveData
{
public:
	/**
	  \brief 曲线抽样点,曲线为每个样本抽取一个最大值和最小值以最接近的方式表示曲线趋势
     */
	class SamplePoint
	{
	public:
		float value1;
		float value2;
	};

	/**
	  \brief 抽样辅助类
	 */
	class SampleHelper
	{
	public:
		int haveReadNum;    /**< 已更新数据个数,将其与曲线的分辨率比较 */
		int sampleNum;      /**< 抽样样本大小 */
		float tmpMax;       /**< 临时最大值 */
		float tmpMin;       /**< 临时最小值 */
		int orderMax;       /**< 临时最大值读入的顺序号 */
		int orderMin;       /**< 临时最小值读入的顺序号 */
	};

	explicit CurveData();

	CurveData(int capacity, int sampleNum);

	~CurveData();

	void AllocBuffer(const int capacity, const int sampleNum);
	
	void FreeBuffer();

	/**
	  \brief 以抽样模式追加数据,当队列满时删除最早的数据
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

	SamplePoint *m_circleBuffer; /**< 曲线数据点队列缓存区别于普通像素点 */

	int m_capacity;              /**< 队列容量 */

	int m_header;                /**< 队列头指针 */
	
	int m_tail;                  /**< 队列尾指针 */

	int m_num;                   /**< 队列中数据点的个数 */

	float m_max;                 /**< 队列中的最大值 */

	float m_min;                 /**< 队列中的最小值 */

	BOOL m_haveInitMaxAndMin;      /**< 是否已初始化完毕最大最小值 */

	COLORREF m_color;              /**< 曲线颜色 */

	SampleHelper m_sampleHelper; /**< 抽样辅助类 */
};

/**
  \brief 抽样趋势图
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
	  \brief 绘制曲线
	  \param[in] curveDataArr 曲线数据缓存
	  \param[in] arrSize      绘存中无素的个数
	 */
	void DrawCurve(const CurveData *curveDataArr, const int arrSize);

	/**
	  \brief 获取曲线抽样样本大小
	 */
	int GetSampleNum();

	/**
	  \brief 获取曲线可容纳抽样点的数量
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
	  \brief 绘制趋势图背景
	 */
	BOOL DrawChart();

	/**
	  \brief 绘制x轴坐标刻度
	 */
	void DrawXScale(CDC &dc);
	
	/**
	  \brief 绘制x轴坐标刻度的值
	 */
	void DrawXScaleValue(CDC &dc);

	/**
	  \brief 绘制y轴坐标刻度
	 */
	void DrawYScale(CDC &dc);

	/**
	  \brief 更新曲线y轴属性,调整y轴的值使其每个刻度值的数据值都为整数
	         并且不超过3位
	 */
	void UpdateYAxisAttr(const float yMax, const float yMin);

	/**
	  \brief 绘制y轴坐标刻度的值
	 */
	void DrawYScaleValue(CDC &dc);

	/**
	  \brief 绘制y轴的单位标签
	 */
	void DrawYTitle(CDC &dc);

	/**
	  \brief 绘制曲线,耗费时间较长
	 */
	void PolyLineEx(CDC &dc, const CPoint *ptArr, const int arrSize, const COLORREF color);

private:
	/**
	  \brief 内存DC
	 */
	CDC m_dcCurve;

	/**
	  \brief 曲线最大值
	 */
	float m_yAxisMax;

	/**
	  \brief 曲线最小值
	 */
	float m_yAxisMin;

	/**
	  \brief 曲线标题前缀
	 */
	CString m_strTitlePrefix;

	/**
	  \brief 背景位图
	 */
	CBitmap m_bmpBkgnd;

	/**
	  \brief 曲线位图
	 */
	CBitmap m_bmpCurve;

	/**
	  \brief 背景DC
	 */
	CDC m_dcBkgnd;

	/**
	  \brief 表格属性
	 */
	ChartDetailAttr m_detailAttr;

	/**
	  \brief 绘制曲线的临时辅助空间
	 */
	std::vector<CPoint> m_curvePixelPoint;
};