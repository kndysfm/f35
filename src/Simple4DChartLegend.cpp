#include "stdafx.h"
#include "Simple4DChartLegend.h"

USING_F35_NS;

struct Simple4DChartLegend::Impl
{
	H::R<ID2D1SolidColorBrush> line_brush;

	static D2D1_POINT_2F convert_point_xy(D2D1_RECT_F const *rect, ChartDataPoint const *pt_ratio)
	{
		D2D1_POINT_2F pt;

		pt.x = rect->left + pt_ratio->x * (rect->right - rect->left);
		pt.y = rect->top + pt_ratio->y * (rect->bottom - rect->top);

		return pt;
	}

	static D2D1_POINT_2F convert_point_zw(D2D1_RECT_F const *rect, ChartDataPoint const *pt_ratio)
	{
		D2D1_POINT_2F pt;

		pt.x = rect->left + (pt_ratio->x + pt_ratio->z/70.0f) * (rect->right - rect->left);
		pt.y = rect->top + (pt_ratio->y + pt_ratio->w/40.0f) * (rect->bottom - rect->top);

		return pt;
	}

	Impl(): line_brush(NULL) { }

	~Impl()
	{
	}
};

Simple4DChartLegend::Simple4DChartLegend(void):
	pImpl(new Impl)
{
}


Simple4DChartLegend::~Simple4DChartLegend(void)
{
	delete pImpl;
	pImpl = NULL;
}

void F35_NS::Simple4DChartLegend::BeginDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const
{
	pImpl->line_brush = renderer->MakeBrush(GetLineColor());
}

void F35_NS::Simple4DChartLegend::Draw( RendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, ChartDataPoint const *point_previous /*= NULL*/, ChartDataPoint const *point_next /*= NULL */ ) const
{
	if (!pImpl || !pImpl->line_brush) return;

	D2D1_POINT_2F pt0 = Impl::convert_point_xy(chart_rect, point);
	D2D1_POINT_2F pt1 = Impl::convert_point_zw(chart_rect, point);
	target->DrawLine(pt0, pt1, pImpl->line_brush);
}

void F35_NS::Simple4DChartLegend::EndDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const
{
}
