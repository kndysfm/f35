#pragma once
#include "F35Lib.h"

#include "D2DRendererBase.h"

namespace F35_NS
{
class ChartLegendBase: NonCopyable
{
	struct Impl;
	Impl *pImpl;
public:
	ChartLegendBase(void);
	virtual ~ChartLegendBase(void);

	void SetLineColor(const D2D1::ColorF & color);
	D2D1::ColorF  GetLineColor (void) const; 

	void SetFillColor(const D2D1::ColorF & color);
	D2D1::ColorF  GetFillColor (void) const;

	virtual void BeginDraw(D2DRendererBase *renderer, ID2D1RenderTarget * target) const { }

	virtual void Draw(D2DRendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point,
		ChartDataPoint const *point_previous = NULL, ChartDataPoint const *point_next = NULL) const = 0;

	virtual void Print(D2DRendererBase *renderer, ID2D1RenderTarget * target, 
		D2D1_RECT_F const *chart_rect, ChartDataPoint const *point,
		LPCTSTR str_fmt, ...) const { }

	virtual void EndDraw(D2DRendererBase *renderer, ID2D1RenderTarget * target) const { }

};
}
