#include <assert.h>
#include "ChartCtrl.h"
#include "UIHelper.h"
#include "UIResource.h"


CurveData::CurveData(int capacity, int sampleNum)
	: m_capacity(capacity), m_header(0), m_tail(0), m_num(0), 
	m_max(1.0), m_min(0.0), m_color(RGB(255, 0, 0)), m_haveInitMaxAndMin(FALSE)
{
	m_sampleHelper.haveReadNum = 0;
	m_sampleHelper.sampleNum = sampleNum;
	m_sampleHelper.tmpMax = 0;
	m_sampleHelper.tmpMin = 0;
	m_sampleHelper.orderMax = 0;
	m_sampleHelper.orderMin = 0;

	m_circleBuffer = new SamplePoint[m_capacity];
	assert(m_circleBuffer);
}

CurveData::CurveData()
	: m_circleBuffer(NULL),m_capacity(-1), m_header(0), m_tail(0), m_num(0), 
	m_max(1.0), m_min(0.0), m_color(RGB(255, 0, 0)), m_haveInitMaxAndMin(FALSE)
{
	m_sampleHelper.haveReadNum = 0;
	m_sampleHelper.sampleNum = -1;
	m_sampleHelper.tmpMax = 0;
	m_sampleHelper.tmpMin = 0;
	m_sampleHelper.orderMax = 0;
	m_sampleHelper.orderMin = 0;
}

CurveData::~CurveData()
{
	if ( m_circleBuffer != NULL )
	{
		delete[] m_circleBuffer;
	}
}

void CurveData::AllocBuffer( const int capacity, const int sampleSize )
{
	if ( m_circleBuffer != NULL )
	{
		return;
	}
	m_capacity = capacity;
	m_sampleHelper.sampleNum = sampleSize;
	m_circleBuffer = new SamplePoint[m_capacity];
	assert(m_circleBuffer);
}

void CurveData::FreeBuffer()
{
	m_haveInitMaxAndMin = FALSE;
	if ( m_circleBuffer != NULL )
	{
		delete[] m_circleBuffer;
		m_capacity = -1;
		m_header = 0;
		m_tail = 0;
		m_num = 0;
	}
}

void CurveData::AppendData(const float data)
{
	if ( m_circleBuffer == NULL )
	{
		return;
	}

	// 更新抽样后曲线数据的最大值和最小值
	if ( ! m_haveInitMaxAndMin )
	{
		// 防止最大最小值叠加
		m_max = data + 1;
		m_min = data;
		m_haveInitMaxAndMin = TRUE;
	}
	else if ( data > m_max  )
	{
		m_max = data;
	}
	else if ( data < m_min )
	{
		m_min = data;
	}
	
	// 更新已读入样本的数量和当前样本的抽样结果
	m_sampleHelper.haveReadNum ++;
	if ( m_sampleHelper.haveReadNum == 1 )
	{
		m_sampleHelper.tmpMax = data;
		m_sampleHelper.tmpMin = data;
		m_sampleHelper.orderMax = m_sampleHelper.haveReadNum;
		m_sampleHelper.orderMin = m_sampleHelper.haveReadNum;
	}
	else if ( m_sampleHelper.haveReadNum > 1 && data > m_sampleHelper.tmpMax )
	{
		m_sampleHelper.orderMax = m_sampleHelper.haveReadNum;
		m_sampleHelper.tmpMax = data;
	}
	else if ( m_sampleHelper.haveReadNum > 1 && data < m_sampleHelper.tmpMin )
	{
		m_sampleHelper.orderMin = m_sampleHelper.haveReadNum;
		m_sampleHelper.tmpMin = data;
	}
	
	// 抽样完毕,则将新的抽样点添加到缓存队列,若队列已满则删除最早的数据
	if ( m_sampleHelper.haveReadNum >= m_sampleHelper.sampleNum )
	{
		if ( m_sampleHelper.orderMax < m_sampleHelper.orderMin )
		{
			m_circleBuffer[m_tail].value1 = m_sampleHelper.tmpMax;
			m_circleBuffer[m_tail].value2 = m_sampleHelper.tmpMin;
		}
		else
		{
			m_circleBuffer[m_tail].value1 = m_sampleHelper.tmpMin;
			m_circleBuffer[m_tail].value2 = m_sampleHelper.tmpMax;
		}
		m_sampleHelper.haveReadNum = 0;
		m_sampleHelper.tmpMin = 0;
		m_sampleHelper.tmpMin = 0;
		m_tail = (m_tail + 1) % m_capacity;
		
		// 队列已满
		if ( m_num >= m_capacity )
		{
			m_header = (m_header + 1) % m_capacity;
		}
		else
		{
			m_num ++;
		}
	}
}

float CurveData::GetMax() const
{
	return m_max;
}

float CurveData::GetMin() const
{
	return m_min;
}

int CurveData::GetNum() const
{
	return m_num;
}

void CurveData::GetSamplePoint( const int idx, SamplePoint *point ) const
{
	if ( m_circleBuffer == NULL )
	{
		return;
	}

	int idxRead = (m_header+idx) % m_capacity;
	point->value1 = m_circleBuffer[idxRead].value1;
	point->value2 = m_circleBuffer[idxRead].value2;
}

void CurveData::Clear()
{
	m_haveInitMaxAndMin = FALSE;
	m_num = 0;
	m_header = 0;
	m_tail = 0;
}

void CurveData::SetColor(const COLORREF color)
{
	m_color = color;
}

COLORREF CurveData::GetColor() const
{
	return m_color;
}

CChartCtrl::CChartCtrl()
{
	m_curvePixelPoint.resize(0);
}

CChartCtrl::~CChartCtrl()
{

}

BOOL CChartCtrl::Init( const ChartAttr *attr )
{
	if ( attr == NULL || m_dcCurve.GetSafeHdc() != NULL 
		|| m_dcBkgnd.GetSafeHdc() != NULL )
	{
		return FALSE;
	}
	/**
	  \note 趋势图各区域  
	      (x, y)
	         | <---------------------- width --------------------------------->  |
	   ---   |-------------------------------------------------------------------|
	    |    |         |       |           edge                        |         |
	    |    |         |       |    -------------------------------    |         |
	    |    |         |       |    |      top padding            |    | right   |
	    |    | title   |yScale |    |-----------------------------|    | padding |
	  height |         |Note   |    |                             |    |         |
	    |    |         |Width  |    |                             |    |         |
	    |    |         |       |    |                             |    |         |
	    |    |         |       |    |                             |    |         |
	    |    |         |       |    |        axis                 |    |         |
	    |    |         |       |    |                             |    |         |
	    |    |         |       |    |                             |    |         |
	    |    |         |       |    |                             |    |         |
	    |    |         |       |    |-----------------------------|    |         |
	    |    |         |       |    |     bottom padding          |    |         |
		|    |         |       |    -------------------------------    |         |
	    |    |         |       |           edge                        |         |
	    |    |         |       |---------------------------------------|         |
	    |    |         |          xScaleNoteHeight                     |         |
		|    |         |                                               |         |
	   ---   |-------------------------------------------------------------------|
	*/

	// 计算趋势图的详细属性
	m_detailAttr.x = attr->x;
	m_detailAttr.y = attr->y;
	m_detailAttr.width = attr->width;
	m_detailAttr.height = attr->height;
	m_detailAttr.baseTitle = attr->baseTitle;
	m_detailAttr.xSpan = attr->xSpan;
	m_detailAttr.ySpan = attr->ySpan;
	m_detailAttr.edgeWidth = attr->edgeWidth;
	m_detailAttr.hasXScale = attr->hasXScale;
	m_detailAttr.hasYTitle = attr->hasYTitle;

	m_detailAttr.coordinateHeight = 3;
	// 计算趋势图横向各部分的详细数据,按比例计算
	m_detailAttr.titleWidth = 0;
	if ( m_detailAttr.hasYTitle )
	{
		m_detailAttr.titleWidth = attr->width / 35;
	}
	m_detailAttr.yScaleNoteWidth = attr->width / 22;
	m_detailAttr.rightPadding = attr->width / 35;
	
	// 计算坐标区域宽度并将剩余像素点附加到右边距
	int leftWidth = m_detailAttr.width 
		- m_detailAttr.titleWidth 
		- m_detailAttr.yScaleNoteWidth
		- 2 * m_detailAttr.edgeWidth 
		- m_detailAttr.rightPadding;
	m_detailAttr.axisWidth = (leftWidth / m_detailAttr.xSpan) * m_detailAttr.xSpan;
	m_detailAttr.rightPadding += (leftWidth - m_detailAttr.axisWidth);

	// 计算趋势图纵向各部分的详细数据,按比例计算
	m_detailAttr.xScaleNoteHeight = 0;
	if ( m_detailAttr.hasXScale )
	{
		m_detailAttr.xScaleNoteHeight = attr->height / 10;
	}
	m_detailAttr.axisHeightTopPadding = attr->height / 30;
	m_detailAttr.axisHeightBottomPadding = attr->height / 30;

	// 计算坐标区域高度使其除去上端和下端预留的像素点后
	// 能被y轴的刻度个数整除,并将剩余像素点附加到坐标的上端预留像素点
	int leftHeight = m_detailAttr.height 
		- 2 * m_detailAttr.edgeWidth
		- m_detailAttr.xScaleNoteHeight
		- m_detailAttr.axisHeightTopPadding
		- m_detailAttr.axisHeightBottomPadding;
	m_detailAttr.axisHeight = (leftHeight / m_detailAttr.ySpan) * m_detailAttr.ySpan;
	m_detailAttr.axisHeightTopPadding += (leftHeight - m_detailAttr.axisHeight);
	m_detailAttr.axisHeight += (m_detailAttr.axisHeightTopPadding + m_detailAttr.axisHeightBottomPadding);

	return DrawChart();
}

// 绘制表格
BOOL CChartCtrl::DrawChart()
{
	// 调整窗体大小,并绘制基础的坐标轴和网格
	MoveWindow( m_detailAttr.x, m_detailAttr.y, m_detailAttr.width, m_detailAttr.height);
	CClientDC dc(this);
	m_dcBkgnd.CreateCompatibleDC(&dc);
	m_dcCurve.CreateCompatibleDC(&dc);
	m_bmpBkgnd.CreateCompatibleBitmap(&dc, m_detailAttr.width, m_detailAttr.height);
	m_bmpCurve.CreateCompatibleBitmap(&dc, m_detailAttr.width, m_detailAttr.height);
	CBitmap *oldBmp = m_dcBkgnd.SelectObject(&m_bmpBkgnd);
	oldBmp->DeleteObject();
	oldBmp = m_dcCurve.SelectObject(&m_bmpCurve);
	oldBmp->DeleteObject();

	// 画背景
	CBrush brush(CHART_BKGND_COLOR);
	CUIRect rectBkgnd(0, 0, m_detailAttr.width, m_detailAttr.height);
	m_dcBkgnd.FillRect(&rectBkgnd, &brush);
	CBrush *oldBrush = m_dcBkgnd.SelectObject(&brush);
	oldBrush->DeleteObject();

	// 画坐标轴
	CUIRect rectAxis( m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.edgeWidth/2, 
		m_detailAttr.edgeWidth/2, 
		m_detailAttr.axisWidth + m_detailAttr.edgeWidth, 
		m_detailAttr.axisHeight + m_detailAttr.edgeWidth);
	CPen pen(PS_SOLID, m_detailAttr.edgeWidth, CHART_EDGE_COLOR);
	CPen *oldPen = dc.SelectObject(&pen);
	oldPen->DeleteObject();
	m_dcBkgnd.Rectangle(rectAxis);

	// 垂直网格线,由于初始化属性时已对值进行调整,因此坐标轴的宽度一定能被x轴刻度的个数整除
	int xScaleWidth = m_detailAttr.axisWidth / m_detailAttr.xSpan;
	//垂直网格线在x轴上的第一个点
	CPoint xFirstPtOfVerGrid;
	xFirstPtOfVerGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	xFirstPtOfVerGrid.y = m_detailAttr.axisHeight + m_detailAttr.edgeWidth;
	for ( int i = 1; i < m_detailAttr.xSpan; i ++ )
	{
		CPoint ptVerGirdBegin(xFirstPtOfVerGrid.x - i*xScaleWidth, m_detailAttr.edgeWidth);
		CPoint ptVerGirdEnd(xFirstPtOfVerGrid.x - i*xScaleWidth, xFirstPtOfVerGrid.y);
		CUIResHelper::DrawDotLine2DC(m_dcBkgnd, ptVerGirdBegin, ptVerGirdEnd, CHART_GRID_COLOR, 1);
	}

	// 水平网格线,由于初始化属性时已对值进行调整
	// 因此坐标轴的高度除去上下端预留的空间后一定能被y轴刻度的个数整除
	int yScaleHeight = 
		(m_detailAttr.axisHeight - m_detailAttr.axisHeightTopPadding - m_detailAttr.axisHeightBottomPadding) / m_detailAttr.ySpan;
	// 水平网格线在y轴上的第一个点
	CPoint yFirstPtOfHorGrid;
	yFirstPtOfHorGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	yFirstPtOfHorGrid.y = m_detailAttr.edgeWidth + m_detailAttr.axisHeight -  m_detailAttr.axisHeightBottomPadding;

	for ( int i = 0; i <= m_detailAttr.ySpan; i ++ )
	{
		CPoint ptHorGridBegin(yFirstPtOfHorGrid.x, yFirstPtOfHorGrid.y - i * yScaleHeight);
		CPoint ptHorGridEnd(yFirstPtOfHorGrid.x + m_detailAttr.axisWidth + m_detailAttr.edgeWidth, yFirstPtOfHorGrid.y - i * yScaleHeight);
		CUIResHelper::DrawDotLine2DC(m_dcBkgnd, ptHorGridBegin, ptHorGridEnd, CHART_GRID_COLOR, 1);
	}

	// x轴的刻度一直不变
	DrawXScale(m_dcBkgnd);
	DrawXScaleValue(m_dcBkgnd);
	DrawYScale(m_dcBkgnd);
	UpdateYAxisAttr(1.0, 0);
	m_dcCurve.BitBlt(0,0, m_detailAttr.width, m_detailAttr.height, &m_dcBkgnd, 0, 0, SRCCOPY);
	DrawYScaleValue(m_dcCurve);
	DrawYTitle(m_dcCurve);
	return TRUE;
}

void CChartCtrl::DrawXScale(CDC &dc)
{	
	if ( ! m_detailAttr.hasXScale )
	{
		return;
	}
	//垂直网格线在x轴上的第一个点
	CPoint xFirstPtOfVerGrid;
	xFirstPtOfVerGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	xFirstPtOfVerGrid.y = m_detailAttr.axisHeight;
	int xScaleWidth = m_detailAttr.axisWidth / m_detailAttr.xSpan;
	
	CPoint ptCoordinateBegin;
	CPoint ptCoordinateEnd;
	CString strScale;
	for ( int i = 0; i <= m_detailAttr.xSpan; i ++ )
	{
		ptCoordinateBegin.x = xFirstPtOfVerGrid.x - i*xScaleWidth;
		ptCoordinateBegin.y = xFirstPtOfVerGrid.y;
		ptCoordinateEnd.x = xFirstPtOfVerGrid.x - i*xScaleWidth;
		ptCoordinateEnd.y = xFirstPtOfVerGrid.y + m_detailAttr.coordinateHeight;
		CUIResHelper::DrawSolidLine2DC(dc, ptCoordinateBegin, ptCoordinateEnd, CHART_EDGE_COLOR, 1);
	}
}

void CChartCtrl::DrawXScaleValue(CDC &dc)
{
	if ( ! m_detailAttr.hasXScale )
	{
		return;
	}
	//垂直网格线在x轴上的第一个点
	CPoint xFirstPtOfVerGrid;
	xFirstPtOfVerGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	xFirstPtOfVerGrid.y = m_detailAttr.axisHeight;
	int xScaleWidth = m_detailAttr.axisWidth / m_detailAttr.xSpan;

	CPoint ptScaleValue;
	CString strScale;
	for ( int i = 0; i <= m_detailAttr.xSpan; i ++ )
	{
		ptScaleValue.x = xFirstPtOfVerGrid.x - i*xScaleWidth;
		ptScaleValue.y = xFirstPtOfVerGrid.y + m_detailAttr.coordinateHeight;
		CUIRect rectScale( ptScaleValue.x - 15, ptScaleValue.y, 40, m_detailAttr.xScaleNoteHeight);
		strScale.Format(L"%d min", i);
		CUIResHelper::DrawText2DC(dc, strScale, rectScale, DT_LEFT|DT_TOP, TXT_SIZE_XS);
	}
}

void CChartCtrl::DrawYScale(CDC &dc)
{
	int yScaleHeight = (m_detailAttr.axisHeight 
		- m_detailAttr.axisHeightTopPadding 
		- m_detailAttr.axisHeightBottomPadding) / m_detailAttr.ySpan;
	CPoint firstPtOfYScale;
	firstPtOfYScale.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	firstPtOfYScale.y = m_detailAttr.edgeWidth + m_detailAttr.axisHeightTopPadding;
	CPoint ptCoordinateBegin;
	CPoint ptCoordinateEnd;
	for ( int i = 0; i <= m_detailAttr.ySpan; i ++ )
	{
		ptCoordinateBegin.x = firstPtOfYScale.x;
		ptCoordinateBegin.y = firstPtOfYScale.y + i * yScaleHeight;
		ptCoordinateEnd.x = ptCoordinateBegin.x  - m_detailAttr.coordinateHeight;
		ptCoordinateEnd.y = ptCoordinateBegin.y;
		CUIResHelper::DrawSolidLine2DC(dc, ptCoordinateBegin, ptCoordinateEnd, CHART_EDGE_COLOR, 1);
	}
}

void CChartCtrl::DrawYScaleValue(CDC &dc)
{
	// 调整坐标的最大最小值,使y轴刻度值长度始终为3位
	float maxValue = m_yAxisMax;
	float oneScaleValue = (m_yAxisMax - m_yAxisMin) / m_detailAttr.ySpan;
	if ( fabs(m_yAxisMax)/1000 >= 1.0 || fabs(m_yAxisMin)/1000 >= 1.0 )
	{
		maxValue = maxValue / 1000;
		oneScaleValue = oneScaleValue / 1000;
	}

	// 绘制y轴刻度
	CString strScale;
	int yScaleHeight = (m_detailAttr.axisHeight 
		- m_detailAttr.axisHeightTopPadding 
		- m_detailAttr.axisHeightBottomPadding) / m_detailAttr.ySpan;
	
	// 第一个刻度的位置
	CPoint firstPtOfYScale;
	firstPtOfYScale.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth - m_detailAttr.coordinateHeight;
	firstPtOfYScale.y = m_detailAttr.edgeWidth + m_detailAttr.axisHeightTopPadding;
	CPoint ptScaleValue;
	for ( int i = 0; i <= m_detailAttr.ySpan; i ++ )
	{
		ptScaleValue.x = firstPtOfYScale.x;
		ptScaleValue.y = firstPtOfYScale.y + i * yScaleHeight;
		if ( i % 2 == 0 )
		{
			CUIRect rectScale( ptScaleValue.x - m_detailAttr.yScaleNoteWidth + m_detailAttr.coordinateHeight,
				ptScaleValue.y - 5, 
				m_detailAttr.yScaleNoteWidth - m_detailAttr.coordinateHeight, 10);
			float scaleValue = maxValue - i*oneScaleValue;
			if ( fabs(scaleValue / 100) >= 1 )
			{
				strScale.Format(L"%3d", (int)scaleValue);
			}
			else
			{
				strScale.Format(L"%3.1f", scaleValue);
			}
			CUIResHelper::DrawText2DC(dc, strScale, rectScale, DT_RIGHT|DT_VCENTER, TXT_SIZE_XS);
		}
	}
}

void CChartCtrl::UpdateYAxisAttr(const float yMax, const float yMin)
{
	// 补余为整
	m_yAxisMax = yMax;
	if ( fabs(yMax - (int)yMax) > 0 )
	{
		m_yAxisMax = (float)(int)(yMax + 0.999999);
	}
	m_yAxisMin = yMin;
	if ( fabs(yMin - (int)yMin) > 0 )
	{
		m_yAxisMin  = (float)(int)yMin;
		if ( yMin < 0 )
		{
			m_yAxisMin  = (float)(int)(yMin - 0.9999999);
		}
	}
	// 调整坐标的最大最小值,使y轴刻度值长度始终为3位
	float maxValue = m_yAxisMax;
	float oneScaleValue = (m_yAxisMax - m_yAxisMin) / m_detailAttr.ySpan;
	if ( fabs(m_yAxisMax)/1000 >= 1.0 || fabs(m_yAxisMin)/1000 >= 1.0 )
	{
		maxValue = maxValue / 1000;
		oneScaleValue = oneScaleValue / 1000;
		m_strTitlePrefix = "k";
	}
	// 若要绘制的曲线不超过坐标区域,则需再进行一次补余为整操作
	// 因为当除去1000的时候可能会出现1.001kV的情况
	int idxScale = 0;
	while ( idxScale <= m_detailAttr.ySpan )
	{
		float scaleValue = maxValue - idxScale * oneScaleValue;
		if ( idxScale % 2 == 0 
			&& fabs(scaleValue - (int)scaleValue) > 0 
			&& fabs(scaleValue / 100) >= 1 )
		{
			if ( scaleValue > 0 )
			{
				scaleValue = (float)(int)scaleValue;
			}
			else
			{
				scaleValue = (float)(int)(scaleValue - 0.9999999);
			}
			// 第0个必然为整数,否则该处会出现除数为0的错误
			oneScaleValue = (maxValue - scaleValue) / idxScale;
			idxScale = 0;
			continue;
		}
		idxScale = idxScale + 1;
	}

	// 更新曲线最小值,当前缀为k时需乘上1000对取值进行还原
	m_yAxisMin = m_yAxisMax - oneScaleValue * m_detailAttr.ySpan;
	if ( fabs(m_yAxisMax/1000) >= 1 )
	{
		m_yAxisMin = m_yAxisMax - oneScaleValue * 1000 * m_detailAttr.ySpan;
	}
}

void CChartCtrl::DrawYTitle(CDC &dc)
{
	if ( ! m_detailAttr.hasYTitle )
	{
		return;
	}
	// 画标题,当长度大于1时则倾斜90度显示
	int escapement = 0;
	CString strTitle = m_strTitlePrefix+m_detailAttr.baseTitle;
	if ( strTitle.GetLength() > 1 )
	{
		escapement = 900;
	}

	CUIResHelper::DrawText2DC(dc, strTitle, 
		CUIRect(0, 0, m_detailAttr.titleWidth, m_detailAttr.axisHeight), 
		DT_CENTER | DT_VCENTER, TXT_SIZE_XS, 0, escapement);
}

BOOL CChartCtrl::Uninit()
{
	if ( m_dcCurve.GetSafeHdc() != NULL)
	{
		m_dcCurve.DeleteDC();
		m_dcBkgnd.DeleteDC();
		m_bmpBkgnd.DeleteObject();
		m_bmpCurve.DeleteObject();
	}
	return TRUE;
}

int CChartCtrl::GetSampleNum()
{
	/**
	  \note 横轴中每两个像素点表示一个抽样点(每个抽样点有一个最大值和最小值,该方法可较好体现趋势图的趋势)
	        每个刻度表示时间为1分钟,每个抽样点所占的毫秒数为:
			(m_detailAttr.xSpan * 60 * 1000) / samplePointCapacity
			DSP每200ms返回一个值,则生成一个抽样点所需的DSP返回值的样本数量为:
			((m_detailAttr.xSpan * 60 * 1000) / samplePointCapacity) / 200 
			== (m_detailAttr.xSpan*300) / samplePointCapacity 
			该处的: (m_detailAttr.axisWidth / m_detailAttr.xSpan用于去除非整除的余数部分
	 */
	if ( m_dcCurve.GetSafeHdc() == NULL 
		|| m_dcBkgnd.GetSafeHdc() == NULL )
	{
		return -1;
	}
	int samplePointCapacity = ((m_detailAttr.axisWidth / m_detailAttr.xSpan) * m_detailAttr.xSpan) / 2;
	if ( samplePointCapacity == 0 )
	{
		return -1;
	}
	return (m_detailAttr.xSpan*300) / samplePointCapacity;
}

int CChartCtrl::GetSamplePointCapacity()
{
	if ( m_dcCurve.GetSafeHdc() == NULL 
		|| m_dcBkgnd.GetSafeHdc() == NULL )
	{
		return -1;
	}
	return ((m_detailAttr.axisWidth / m_detailAttr.xSpan) * m_detailAttr.xSpan) / 2;
}

// 绘制曲线
void CChartCtrl::DrawCurve(const CurveData *curveDataArr, const int arrSize)
{
	if ( m_dcCurve.GetSafeHdc() == NULL || m_dcBkgnd.GetSafeHdc() == NULL )
	{
		return;
	}

	// 擦除原有曲线
	m_dcCurve.BitBlt(0,0, m_detailAttr.width, m_detailAttr.height, &m_dcBkgnd, 0, 0, SRCCOPY);

	// y轴的最大值和最小值
	float yMax = curveDataArr[0].GetMax();
	float yMin = curveDataArr[0].GetMin();
	for ( int i = 1; i < arrSize; i++ )
	{
		if ( yMax < curveDataArr[i].GetMax() )
		{
			yMax = curveDataArr[i].GetMax();
		}
		if ( yMin > curveDataArr[i].GetMin() )
		{
			yMin = curveDataArr[i].GetMin();
		}
	}
	UpdateYAxisAttr(yMax,yMin);
	DrawYScaleValue(m_dcCurve);
	DrawYTitle(m_dcCurve);

	// 曲线起始像素点坐标
	CPoint ptPixelCurveBegin;
	ptPixelCurveBegin.x = m_detailAttr.titleWidth 
		+ m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	ptPixelCurveBegin.y = m_detailAttr.edgeWidth 
		+ m_detailAttr.axisHeight 
		- m_detailAttr.axisHeightBottomPadding;

	// 计算y轴上每个像素点表示的值的大小
	float yValueOnePixel = (m_yAxisMax - m_yAxisMin) 
		/ ( m_detailAttr.axisHeight - m_detailAttr.axisHeightBottomPadding 
		- m_detailAttr.axisHeightTopPadding );

	// 绘制曲线
	CPen pen;
	for ( int idxCurve = 0; idxCurve < arrSize; idxCurve++ )
	{
		m_curvePixelPoint.resize(0);
		for ( int idxSamplePt = 0; idxSamplePt < curveDataArr[idxCurve].GetNum(); idxSamplePt++ )
		{
			// 抽样点转换为界面的像素点
			CPoint ptPixel;
			CurveData::SamplePoint samplePoint;
			curveDataArr[idxCurve].GetSamplePoint(idxSamplePt, &samplePoint);
			ptPixel.x = ptPixelCurveBegin.x - (2*idxSamplePt);
			ptPixel.y = ptPixelCurveBegin.y - (int)((samplePoint.value1 - m_yAxisMin) / yValueOnePixel);
			m_curvePixelPoint.push_back(ptPixel);
			ptPixel.x = ptPixelCurveBegin.x - (2*idxSamplePt)-1;
			ptPixel.y = ptPixelCurveBegin.y - (int)((samplePoint.value2 - m_yAxisMin) / yValueOnePixel);
			m_curvePixelPoint.push_back(ptPixel);
		}
		if ( m_curvePixelPoint.size() > 0 )
		{
			PolyLineEx(m_dcCurve, &m_curvePixelPoint[0], m_curvePixelPoint.size(), curveDataArr[idxCurve].GetColor());
		}
	}
	Invalidate();
}

void CChartCtrl::PolyLineEx(CDC &dc, const CPoint *ptArr, const int arrSize, const COLORREF color)
{
	// 绘制小三角
	POINT point[3]   = { {ptArr[0].x, ptArr[0].y}, {ptArr[0].x+10, ptArr[0].y-5}, {ptArr[0].x+10, ptArr[0].y+5} };
	CBrush brush;
	brush.CreateSolidBrush(color);
	CBrush* oldBrush = dc.SelectObject(&brush);
	dc.Polygon(point, 3);
	dc.SelectObject(oldBrush);
	brush.DeleteObject();
	
	// 绘制曲线
	CPen pen(PS_SOLID, 1, color);
	CPen *oldPen = m_dcCurve.SelectObject(&pen);
	dc.MoveTo(point[0]);
	dc.LineTo(point[1]);
	dc.LineTo(point[2]);
	dc.LineTo(point[0]);
#if 1
	dc.Polyline(ptArr, arrSize);
#else
	CPoint ptBegin;
	CPoint ptEnd;
	for ( int i = 0; i < arrSize - 1; i++ )
	{
		ptBegin.x = ptArr[i].x;
		ptBegin.y = ptArr[i].y;
		ptEnd.x = ptArr[i+1].x;
		ptEnd.y = ptArr[i+1].y;
		dc.MoveTo(ptBegin);
		dc.LineTo(ptEnd);
	}
#endif
	dc.SelectObject(oldPen);
	pen.DeleteObject();
}

void CChartCtrl::OnShow()
{
	CStatic::ShowWindow(SW_SHOWNORMAL);
}

void CChartCtrl::OnHide()
{
	CStatic::ShowWindow(SW_HIDE);
}

IMPLEMENT_DYNAMIC(CChartCtrl, CStatic)

BEGIN_MESSAGE_MAP(CChartCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CChartCtrl::OnPaint()
{
	CPaintDC dc(this);
	if ( m_dcCurve.GetSafeHdc() == NULL )
	{
		return;
	}
	dc.BitBlt(0, 0, m_detailAttr.width, m_detailAttr.height, &m_dcCurve, 0, 0, SRCCOPY);
}

BOOL CChartCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	CDC::DeleteTempMap();
	return TRUE;
}

void CChartCtrl::SetBaseTitle( const CString &strTitle )
{
	m_detailAttr.baseTitle = strTitle;
}
