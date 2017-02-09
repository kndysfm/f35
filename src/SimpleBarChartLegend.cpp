#include "stdafx.h"
#include "SimpleBarChartLegend.h"

USING_F35_NS;

struct SimpleBarChartLegend::Impl
{
	H::R< ID2D1SolidColorBrush > brush;

	static D2D1_RECT_F convert_point(D2D1_RECT_F const *rect, D2D_VECTOR_4F const *pt_ratio, FLOAT bar_width)
	{
		D2D1_RECT_F bar;

		bar.left = rect->left + pt_ratio->x * (rect->right - rect->left) - bar_width * 0.5f;
		bar.right = bar.left + bar_width;

		bar.bottom = rect->bottom;
		bar.top = rect->bottom - pt_ratio->y * (rect->bottom - rect->top);

		return bar;
	}

	Impl(): brush(NULL) { }
	~Impl()
	{
	}
};

SimpleBarChartLegend::SimpleBarChartLegend(void):
	pImpl(new Impl)
{
}


SimpleBarChartLegend::~SimpleBarChartLegend(void)
{
	delete pImpl;
	pImpl = NULL;
}

void F35_NS::SimpleBarChartLegend::BeginDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const
{
	pImpl->brush = renderer->MakeBrush(GetFillColor());
}

void F35_NS::SimpleBarChartLegend::Draw( RendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const *chart_rect, D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point, D2D_VECTOR_4F const *point_previous /*= NULL*/, D2D_VECTOR_4F const *point_next /*= NULL */ ) const
{
	if (!pImpl || !pImpl->brush) return;

	D2D1_RECT_F bar = Impl::convert_point(chart_rect, point, 4.0f);
	target->FillRectangle(bar, pImpl->brush);
}

void F35_NS::SimpleBarChartLegend::EndDraw( RendererBase *renderer, ID2D1RenderTarget * target ) const
{
}
