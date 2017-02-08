#pragma once
#include "F35Lib.h"
#include "chartlegendbase.h"
#include "ChartRendererBase.h"

namespace F35_NS {

class SimpleLineChartLegend :
	public ChartLegendBase
{
	struct Impl;
	Impl *pImpl;

public:
	SimpleLineChartLegend(void);
	virtual ~SimpleLineChartLegend(void);

	void ShowMarker(BOOL show);

	virtual void BeginDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const;

	virtual void Draw( D2DRendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, 
		ChartDataPoint const *point_previous = NULL, ChartDataPoint const *point_next = NULL ) const;

	virtual void Print(D2DRendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point,
		LPCTSTR str_fmt, ...) const;

	virtual void EndDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const;

};

}