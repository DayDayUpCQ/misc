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

	// ���³������������ݵ����ֵ����Сֵ
	if ( ! m_haveInitMaxAndMin )
	{
		// ��ֹ�����Сֵ����
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
	
	// �����Ѷ��������������͵�ǰ�����ĳ������
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
	
	// �������,���µĳ�������ӵ��������,������������ɾ�����������
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
		
		// ��������
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
	  \note ����ͼ������  
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

	// ��������ͼ����ϸ����
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
	// ��������ͼ��������ֵ���ϸ����,����������
	m_detailAttr.titleWidth = 0;
	if ( m_detailAttr.hasYTitle )
	{
		m_detailAttr.titleWidth = attr->width / 35;
	}
	m_detailAttr.yScaleNoteWidth = attr->width / 22;
	m_detailAttr.rightPadding = attr->width / 35;
	
	// �������������Ȳ���ʣ�����ص㸽�ӵ��ұ߾�
	int leftWidth = m_detailAttr.width 
		- m_detailAttr.titleWidth 
		- m_detailAttr.yScaleNoteWidth
		- 2 * m_detailAttr.edgeWidth 
		- m_detailAttr.rightPadding;
	m_detailAttr.axisWidth = (leftWidth / m_detailAttr.xSpan) * m_detailAttr.xSpan;
	m_detailAttr.rightPadding += (leftWidth - m_detailAttr.axisWidth);

	// ��������ͼ��������ֵ���ϸ����,����������
	m_detailAttr.xScaleNoteHeight = 0;
	if ( m_detailAttr.hasXScale )
	{
		m_detailAttr.xScaleNoteHeight = attr->height / 10;
	}
	m_detailAttr.axisHeightTopPadding = attr->height / 30;
	m_detailAttr.axisHeightBottomPadding = attr->height / 30;

	// ������������߶�ʹ���ȥ�϶˺��¶�Ԥ�������ص��
	// �ܱ�y��Ŀ̶ȸ�������,����ʣ�����ص㸽�ӵ�������϶�Ԥ�����ص�
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

// ���Ʊ��
BOOL CChartCtrl::DrawChart()
{
	// ���������С,�����ƻ����������������
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

	// ������
	CBrush brush(CHART_BKGND_COLOR);
	CUIRect rectBkgnd(0, 0, m_detailAttr.width, m_detailAttr.height);
	m_dcBkgnd.FillRect(&rectBkgnd, &brush);
	CBrush *oldBrush = m_dcBkgnd.SelectObject(&brush);
	oldBrush->DeleteObject();

	// ��������
	CUIRect rectAxis( m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.edgeWidth/2, 
		m_detailAttr.edgeWidth/2, 
		m_detailAttr.axisWidth + m_detailAttr.edgeWidth, 
		m_detailAttr.axisHeight + m_detailAttr.edgeWidth);
	CPen pen(PS_SOLID, m_detailAttr.edgeWidth, CHART_EDGE_COLOR);
	CPen *oldPen = dc.SelectObject(&pen);
	oldPen->DeleteObject();
	m_dcBkgnd.Rectangle(rectAxis);

	// ��ֱ������,���ڳ�ʼ������ʱ�Ѷ�ֵ���е���,���������Ŀ��һ���ܱ�x��̶ȵĸ�������
	int xScaleWidth = m_detailAttr.axisWidth / m_detailAttr.xSpan;
	//��ֱ��������x���ϵĵ�һ����
	CPoint xFirstPtOfVerGrid;
	xFirstPtOfVerGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	xFirstPtOfVerGrid.y = m_detailAttr.axisHeight + m_detailAttr.edgeWidth;
	for ( int i = 1; i < m_detailAttr.xSpan; i ++ )
	{
		CPoint ptVerGirdBegin(xFirstPtOfVerGrid.x - i*xScaleWidth, m_detailAttr.edgeWidth);
		CPoint ptVerGirdEnd(xFirstPtOfVerGrid.x - i*xScaleWidth, xFirstPtOfVerGrid.y);
		CUIResHelper::DrawDotLine2DC(m_dcBkgnd, ptVerGirdBegin, ptVerGirdEnd, CHART_GRID_COLOR, 1);
	}

	// ˮƽ������,���ڳ�ʼ������ʱ�Ѷ�ֵ���е���
	// ���������ĸ߶ȳ�ȥ���¶�Ԥ���Ŀռ��һ���ܱ�y��̶ȵĸ�������
	int yScaleHeight = 
		(m_detailAttr.axisHeight - m_detailAttr.axisHeightTopPadding - m_detailAttr.axisHeightBottomPadding) / m_detailAttr.ySpan;
	// ˮƽ��������y���ϵĵ�һ����
	CPoint yFirstPtOfHorGrid;
	yFirstPtOfHorGrid.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	yFirstPtOfHorGrid.y = m_detailAttr.edgeWidth + m_detailAttr.axisHeight -  m_detailAttr.axisHeightBottomPadding;

	for ( int i = 0; i <= m_detailAttr.ySpan; i ++ )
	{
		CPoint ptHorGridBegin(yFirstPtOfHorGrid.x, yFirstPtOfHorGrid.y - i * yScaleHeight);
		CPoint ptHorGridEnd(yFirstPtOfHorGrid.x + m_detailAttr.axisWidth + m_detailAttr.edgeWidth, yFirstPtOfHorGrid.y - i * yScaleHeight);
		CUIResHelper::DrawDotLine2DC(m_dcBkgnd, ptHorGridBegin, ptHorGridEnd, CHART_GRID_COLOR, 1);
	}

	// x��Ŀ̶�һֱ����
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
	//��ֱ��������x���ϵĵ�һ����
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
	//��ֱ��������x���ϵĵ�һ����
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
	// ��������������Сֵ,ʹy��̶�ֵ����ʼ��Ϊ3λ
	float maxValue = m_yAxisMax;
	float oneScaleValue = (m_yAxisMax - m_yAxisMin) / m_detailAttr.ySpan;
	if ( fabs(m_yAxisMax)/1000 >= 1.0 || fabs(m_yAxisMin)/1000 >= 1.0 )
	{
		maxValue = maxValue / 1000;
		oneScaleValue = oneScaleValue / 1000;
	}

	// ����y��̶�
	CString strScale;
	int yScaleHeight = (m_detailAttr.axisHeight 
		- m_detailAttr.axisHeightTopPadding 
		- m_detailAttr.axisHeightBottomPadding) / m_detailAttr.ySpan;
	
	// ��һ���̶ȵ�λ��
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
	// ����Ϊ��
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
	// ��������������Сֵ,ʹy��̶�ֵ����ʼ��Ϊ3λ
	float maxValue = m_yAxisMax;
	float oneScaleValue = (m_yAxisMax - m_yAxisMin) / m_detailAttr.ySpan;
	if ( fabs(m_yAxisMax)/1000 >= 1.0 || fabs(m_yAxisMin)/1000 >= 1.0 )
	{
		maxValue = maxValue / 1000;
		oneScaleValue = oneScaleValue / 1000;
		m_strTitlePrefix = "k";
	}
	// ��Ҫ���Ƶ����߲�������������,�����ٽ���һ�β���Ϊ������
	// ��Ϊ����ȥ1000��ʱ����ܻ����1.001kV�����
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
			// ��0����ȻΪ����,����ô�����ֳ���Ϊ0�Ĵ���
			oneScaleValue = (maxValue - scaleValue) / idxScale;
			idxScale = 0;
			continue;
		}
		idxScale = idxScale + 1;
	}

	// ����������Сֵ,��ǰ׺Ϊkʱ�����1000��ȡֵ���л�ԭ
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
	// ������,�����ȴ���1ʱ����б90����ʾ
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
	  \note ������ÿ�������ص��ʾһ��������(ÿ����������һ�����ֵ����Сֵ,�÷����ɽϺ���������ͼ������)
	        ÿ���̶ȱ�ʾʱ��Ϊ1����,ÿ����������ռ�ĺ�����Ϊ:
			(m_detailAttr.xSpan * 60 * 1000) / samplePointCapacity
			DSPÿ200ms����һ��ֵ,������һ�������������DSP����ֵ����������Ϊ:
			((m_detailAttr.xSpan * 60 * 1000) / samplePointCapacity) / 200 
			== (m_detailAttr.xSpan*300) / samplePointCapacity 
			�ô���: (m_detailAttr.axisWidth / m_detailAttr.xSpan����ȥ������������������
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

// ��������
void CChartCtrl::DrawCurve(const CurveData *curveDataArr, const int arrSize)
{
	if ( m_dcCurve.GetSafeHdc() == NULL || m_dcBkgnd.GetSafeHdc() == NULL )
	{
		return;
	}

	// ����ԭ������
	m_dcCurve.BitBlt(0,0, m_detailAttr.width, m_detailAttr.height, &m_dcBkgnd, 0, 0, SRCCOPY);

	// y������ֵ����Сֵ
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

	// ������ʼ���ص�����
	CPoint ptPixelCurveBegin;
	ptPixelCurveBegin.x = m_detailAttr.titleWidth 
		+ m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	ptPixelCurveBegin.y = m_detailAttr.edgeWidth 
		+ m_detailAttr.axisHeight 
		- m_detailAttr.axisHeightBottomPadding;

	// ����y����ÿ�����ص��ʾ��ֵ�Ĵ�С
	float yValueOnePixel = (m_yAxisMax - m_yAxisMin) 
		/ ( m_detailAttr.axisHeight - m_detailAttr.axisHeightBottomPadding 
		- m_detailAttr.axisHeightTopPadding );

	// ��������
	CPen pen;
	for ( int idxCurve = 0; idxCurve < arrSize; idxCurve++ )
	{
		m_curvePixelPoint.resize(0);
		for ( int idxSamplePt = 0; idxSamplePt < curveDataArr[idxCurve].GetNum(); idxSamplePt++ )
		{
			// ������ת��Ϊ��������ص�
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
	// ����С����
	POINT point[3]   = { {ptArr[0].x, ptArr[0].y}, {ptArr[0].x+10, ptArr[0].y-5}, {ptArr[0].x+10, ptArr[0].y+5} };
	CBrush brush;
	brush.CreateSolidBrush(color);
	CBrush* oldBrush = dc.SelectObject(&brush);
	dc.Polygon(point, 3);
	dc.SelectObject(oldBrush);
	brush.DeleteObject();
	
	// ��������
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
