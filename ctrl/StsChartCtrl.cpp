#include <assert.h>
#include "StsChartCtrl.h"
#include "UIHelper.h"
#include "UIResource.h"

StsCurveData::StsCurveData()
	: m_capacity(-1), m_num(0), m_nextSampleIdx(0),
	m_max(1.0), m_min(0.0), m_color(RGB(255, 0, 0))
{

}

StsCurveData::StsCurveData( int capacity )
	: m_capacity(capacity), m_num(0), m_nextSampleIdx(0),
	m_max(1.0), m_min(0.0), m_color(RGB(255, 0, 0))
{
	m_dataBuffer = new float[m_capacity];
	assert(m_dataBuffer);
}

StsCurveData::~StsCurveData()
{
	if ( m_dataBuffer != NULL )
	{
		delete[] m_dataBuffer;
		m_dataBuffer = NULL;
	}
}

void StsCurveData::AllocBuffer( const int capacity )
{
	if ( m_dataBuffer != NULL )
	{
		return;
	}
	m_capacity = capacity;
	m_dataBuffer = new float[m_capacity];
	assert(m_dataBuffer);
}

void StsCurveData::FreeBuffer()
{
	if ( m_dataBuffer != NULL )
	{
		delete[] m_dataBuffer;
		m_capacity = -1;
		m_num = 0;
		m_nextSampleIdx = 0;
	}
}

void StsCurveData::AppendData(const float data)
{
	if ( m_dataBuffer == NULL )
	{
		return;
	}

	if ( m_num < m_capacity )
	{
		m_dataBuffer[m_num] = data;
		m_num = m_num + 1;
	}
	// ʣ����ֵ�ĸ����������Ҫ��
	else
	{
		SampleData();
		m_dataBuffer[m_num - 1] = data;
	}
}

void StsCurveData::SampleData()
{
	if ( m_num - m_nextSampleIdx < 3 )
	{
		m_nextSampleIdx = 0;
	}
	// �������ݳ���
	int idxTmpMax;
	int idxTmpMin;
	float tmpMax = m_dataBuffer[m_nextSampleIdx];
	float tmpMin = m_dataBuffer[m_nextSampleIdx];
	for ( int i = 1; i < 3; i++ )
	{
		if ( tmpMax < m_dataBuffer[m_nextSampleIdx+i] )
		{
			tmpMax = m_dataBuffer[m_nextSampleIdx+i];
			idxTmpMax = m_nextSampleIdx + i;
		}
		if ( tmpMin < m_dataBuffer[m_nextSampleIdx+i] )
		{
			tmpMin = m_dataBuffer[m_nextSampleIdx+i];
			idxTmpMin = m_nextSampleIdx+i;
		}
	}
	m_dataBuffer[m_nextSampleIdx] = tmpMin;
	m_dataBuffer[m_nextSampleIdx + 1] = tmpMax;
	if ( idxTmpMax < idxTmpMin )
	{
		m_dataBuffer[m_nextSampleIdx] = tmpMax;
		m_dataBuffer[m_nextSampleIdx + 1] = tmpMin;
	}
	
	if ( m_num - m_nextSampleIdx > 3 )
	{
		memcpy_s( &m_dataBuffer[m_nextSampleIdx+2], 
			(m_num-m_nextSampleIdx-2) *sizeof(float), 
			&m_dataBuffer[m_nextSampleIdx+3], 
			(m_num-m_nextSampleIdx-3) *sizeof(float));
	}
	m_nextSampleIdx = m_nextSampleIdx + 3;
}

float StsCurveData::GetMax() const
{
	return m_max;
}

float StsCurveData::GetMin() const
{
	return m_min;
}

int StsCurveData::GetNum() const
{
	return m_num;
}

float StsCurveData::GetData(const int idx) const
{
	if ( m_dataBuffer == NULL || idx >= m_num )
	{
		return 0;
	}
	return m_dataBuffer[idx];
}

void StsCurveData::Clear()
{
	m_num = 0;
	m_nextSampleIdx = 0;
}

void StsCurveData::SetColor(const COLORREF color)
{
	m_color = color;
}

COLORREF StsCurveData::GetColor() const
{
	return m_color;
}

CStsChartCtrl::CStsChartCtrl()
{
	m_curvePixelPoint.resize(0);
}

CStsChartCtrl::~CStsChartCtrl()
{

}

BOOL CStsChartCtrl::Init( const StsChartAttr *attr )
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
	    |    |---------|-------|---------------------------------------|---------|
	    |    |                    xScaleNoteHeight                               |
		|    |                                                                   |
	   ---   |-------------------------------------------------------------------|
	*/

	// ��������ͼ����ϸ����
	m_detailAttr.x = attr->x;
	m_detailAttr.y = attr->y;
	m_detailAttr.width = attr->width;
	m_detailAttr.height = attr->height;
	m_detailAttr.baseTitle = attr->baseTitle;
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
	m_detailAttr.rightPadding = attr->width / 10;
	
	// �������������Ȳ���ʣ�����ص㸽�ӵ��ұ߾�
	int leftWidth = m_detailAttr.width 
		- m_detailAttr.titleWidth 
		- m_detailAttr.yScaleNoteWidth
		- 2 * m_detailAttr.edgeWidth 
		- m_detailAttr.rightPadding;
	// ������Ŀ��Ϊ2��������
	m_detailAttr.axisWidth = (leftWidth / 2) * 2;
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
BOOL CStsChartCtrl::DrawChart()
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

void CStsChartCtrl::DrawXScale(CDC &dc)
{	
	if ( ! m_detailAttr.hasXScale )
	{
		return;
	}

	CPoint ptCoordinateBegin;
	CPoint ptCoordinateEnd;
	// ��ʼʱ���
	ptCoordinateBegin.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	ptCoordinateBegin.y = m_detailAttr.axisHeight;
	ptCoordinateEnd.x = ptCoordinateBegin.x;
	ptCoordinateEnd.y = ptCoordinateBegin.y + m_detailAttr.coordinateHeight;
	CUIResHelper::DrawSolidLine2DC(dc, ptCoordinateBegin, ptCoordinateEnd, CHART_EDGE_COLOR, 1);

	//  ����ʱ���
	ptCoordinateBegin.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	ptCoordinateBegin.y = m_detailAttr.axisHeight;
	ptCoordinateEnd.x = ptCoordinateBegin.x;
	ptCoordinateEnd.y = ptCoordinateBegin.y + m_detailAttr.coordinateHeight;
	CUIResHelper::DrawSolidLine2DC(dc, ptCoordinateBegin, ptCoordinateEnd, CHART_EDGE_COLOR, 1);
}

void CStsChartCtrl::DrawXScaleValue(CDC &dc)
{
	if ( ! m_detailAttr.hasXScale )
	{
		return;
	}

	CPoint ptScaleValue;
	CString strScale;
	CUIRect rectScale;

	// ��ʼʱ��
	ptScaleValue.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	ptScaleValue.y = m_detailAttr.axisHeight + m_detailAttr.coordinateHeight;
	rectScale.SetValue(ptScaleValue.x - 35, ptScaleValue.y, 120, m_detailAttr.xScaleNoteHeight);
	strScale.Format(L"%04d/%02d/%02d %02d:%02d", 2014, 12, 4, 12, 24);
	CUIResHelper::DrawText2DC(dc, strScale, rectScale, DT_LEFT|DT_TOP, TXT_SIZE_XS);

	// ����ʱ��
	ptScaleValue.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth + m_detailAttr.axisWidth;
	ptScaleValue.y = m_detailAttr.axisHeight + m_detailAttr.coordinateHeight;
	rectScale.SetValue( ptScaleValue.x - 55, ptScaleValue.y, 120, m_detailAttr.xScaleNoteHeight);
	strScale.Format(L"%04d/%02d/%02d %02d:%02d", 2014, 12, 4, 15, 24);
	CUIResHelper::DrawText2DC(dc, strScale, rectScale, DT_LEFT|DT_TOP, TXT_SIZE_XS);
}

void CStsChartCtrl::DrawYScale(CDC &dc)
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

void CStsChartCtrl::DrawYScaleValue(CDC &dc)
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

void CStsChartCtrl::UpdateYAxisAttr(const float yMax, const float yMin)
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

void CStsChartCtrl::DrawYTitle(CDC &dc)
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

BOOL CStsChartCtrl::Uninit()
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

void CStsChartCtrl::DrawStsCurve(const StsCurveData *stsCurveDataArr, const int arrSize)
{
	if ( m_dcCurve.GetSafeHdc() == NULL || m_dcBkgnd.GetSafeHdc() == NULL )
	{
		return;
	}

	// ����ԭ������
	m_dcCurve.BitBlt(0,0, m_detailAttr.width, m_detailAttr.height, &m_dcBkgnd, 0, 0, SRCCOPY);

	// y������ֵ����Сֵ
	float yMax = stsCurveDataArr[0].GetMax();
	float yMin = stsCurveDataArr[0].GetMin();
	int maxNum = stsCurveDataArr[0].GetNum();
	for ( int i = 1; i < arrSize; i++ )
	{
		if ( yMax < stsCurveDataArr[i].GetMax() )
		{
			yMax = stsCurveDataArr[i].GetMax();
		}
		if ( yMin > stsCurveDataArr[i].GetMin() )
		{
			yMin = stsCurveDataArr[i].GetMin();
		}

		if ( maxNum < stsCurveDataArr[i].GetNum() )
		{
			maxNum = stsCurveDataArr[i].GetNum();
		}
	}
	UpdateYAxisAttr(yMax,yMin);
	DrawYScaleValue(m_dcCurve);
	DrawYTitle(m_dcCurve);

	// ����y����ÿ�����ص��ʾ��ֵ�Ĵ�С
	float yValueOnePixel = (m_yAxisMax - m_yAxisMin) 
		/ ( m_detailAttr.axisHeight - m_detailAttr.axisHeightBottomPadding 
		- m_detailAttr.axisHeightTopPadding );

	// ÿ��ֵ��x����ռ�����ص�,������ÿ��ֵ��ռ�����ص�
	int xPixelOneValue = m_detailAttr.axisWidth / maxNum;
	if ( xPixelOneValue == 0 )
	{
		xPixelOneValue = 1;
		maxNum = m_detailAttr.axisWidth;
	}

	// ������ʼ���ص�����
	CPoint ptPixelCurveBegin;
	ptPixelCurveBegin.x = m_detailAttr.titleWidth + m_detailAttr.yScaleNoteWidth;
	ptPixelCurveBegin.y = m_detailAttr.edgeWidth + m_detailAttr.axisHeight - m_detailAttr.axisHeightBottomPadding;
	// ����ͳ������
	CPen pen;
	for ( int idxCurve = 0; idxCurve < arrSize; idxCurve++ )
	{
		m_curvePixelPoint.resize(0);
		int dataNum = maxNum < stsCurveDataArr[idxCurve].GetNum() ? maxNum : stsCurveDataArr[idxCurve].GetNum();
		for ( int idxData = 0; idxData < dataNum; idxData++ )
		{
			// ������ת��Ϊ��������ص�
			CPoint ptPixel;
			ptPixel.x = ptPixelCurveBegin.x + idxData * xPixelOneValue;
			ptPixel.y = ptPixelCurveBegin.y - (int)((stsCurveDataArr[idxCurve].GetData(idxData) - m_yAxisMin) / yValueOnePixel);
			m_curvePixelPoint.push_back(ptPixel);
		}
		if ( m_curvePixelPoint.size() > 0 )
		{
			PolyLineEx(m_dcCurve, &m_curvePixelPoint[0], m_curvePixelPoint.size(), stsCurveDataArr[idxCurve].GetColor());
		}
	}
	Invalidate();
}

void CStsChartCtrl::PolyLineEx(CDC &dc, const CPoint *ptArr, const int arrSize, const COLORREF color)
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

void CStsChartCtrl::OnShow()
{
	CStatic::ShowWindow(SW_SHOWNORMAL);
}

void CStsChartCtrl::OnHide()
{
	CStatic::ShowWindow(SW_HIDE);
}

IMPLEMENT_DYNAMIC(CStsChartCtrl, CStatic)

BEGIN_MESSAGE_MAP(CStsChartCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CStsChartCtrl::OnPaint()
{
	CPaintDC dc(this);
	if ( m_dcCurve.GetSafeHdc() == NULL )
	{
		return;
	}
	dc.BitBlt(0, 0, m_detailAttr.width, m_detailAttr.height, &m_dcCurve, 0, 0, SRCCOPY);
}

BOOL CStsChartCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	CDC::DeleteTempMap();
	return TRUE;
}

void CStsChartCtrl::SetBaseTitle( const CString &strTitle )
{
	m_detailAttr.baseTitle = strTitle;
}