/**
  \file StsChartCtrl.h

  \brief 统计趋势图控件
 */
#pragma once
#include <afxwin.h>
#include "MessageFilter.h"
#include "DisplayConfig.h"
#include <vector>

/**
  \brief 统计趋势图数据,当效率较低时可采用链式缓存改写该部分
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
	  \brief 以抽样模式追加数据
	         与实时数据趋势图不同该趋势图每次抽样都以样本数量为3的取值进行抽样
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
	  \brief 以3个样本值为样本组进行抽样
	 */
	void SampleData();

	float *m_dataBuffer; /**< 数据缓存 */ 
	
	int m_capacity;       /**< 缓存容量 */
	
	int m_num;            /**< 缓存中数据的个数 */
	
	float m_max;          /**< 缓存中数据的最大值 */

	float m_min;          /**< 缓存中数据的最小值 */

	int m_nextSampleIdx;  /**< 下一次抽样样本的起始位置 */

	COLORREF m_color;     /**< 曲线颜色 */
};

class StsChartAttr
{
public:
	int x;              /**< 相对页面的x坐标 */
	int y;              /**< 相对页面的y坐标 */
	int width;          /**< 整个趋势图所占区域宽度,包括标签刻度等 */
	int height;         /**< 整个趋势图所占区域高度,包括标签刻度等 */
	CString baseTitle;  /**< 标题(V、A、Hz)趋势图会根据值的大小自动添加前缀 */
	int ySpan;          /**< y轴刻度个数 */
	int edgeWidth;      /**< 表示坐标轴的外边框宽度 */
	BOOL hasXScale;     /**< 是否拥有x轴刻度,x轴仅有起始和结束两个时间刻度 */
	BOOL hasYTitle;     /**< 是否拥有y轴单位标签 */

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
  \brief 抽样趋势图
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
	  \brief 绘制曲线
	  \param[in] StsCurveDataArr 曲线数据缓存
	  \param[in] arrSize         绘存中无素的个数
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
	void UpdateCurve(int type, const StsCurveData *StsCurveDataArr, const int arrSize, const CString &strTitle);
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
	StsChartDetailAttr m_detailAttr;

	/**
	  \brief 绘制曲线的临时辅助空间
	 */
	std::vector<CPoint> m_curvePixelPoint;
};