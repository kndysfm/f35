#pragma once
#include "F35Helper.h"
#include "chartlegendbase.h"

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

	virtual void BeginDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const;

	virtual void Draw( RendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, 
		ChartDataPoint const *point_previous = NULL, ChartDataPoint const *point_next = NULL ) const;

	virtual void Print(RendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point,
		LPCTSTR str_fmt, ...) const;

	virtual void EndDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const;

};

}