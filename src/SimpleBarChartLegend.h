#pragma once
#include "F35Lib.h"
#include "chartlegendbase.h"

namespace F35_NS{

class SimpleBarChartLegend :
	public ChartLegendBase
{
	struct Impl;
	Impl *pImpl;

public:
	SimpleBarChartLegend(void);
	virtual ~SimpleBarChartLegend(void);

	virtual void BeginDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const;

	virtual void Draw( D2DRendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, ChartDataPoint const *point_previous = NULL, ChartDataPoint const *point_next = NULL ) const;

	virtual void EndDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const;

};

}