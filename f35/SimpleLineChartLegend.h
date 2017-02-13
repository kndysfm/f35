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

protected:
	virtual void BeginDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const;

	virtual void Draw( RendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point,
		D2D_VECTOR_4F const *point_previous = NULL, D2D_VECTOR_4F const *point_next = NULL ) const;

	virtual void Print(RendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, D2D_VECTOR_4F const *point,
		LPCTSTR str_fmt, ...) const;

	virtual void EndDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const;

};

}