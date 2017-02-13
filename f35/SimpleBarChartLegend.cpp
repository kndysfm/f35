#include "stdafx.h"
#include "SimpleBarChartLegend.h"

USING_F35_NS;

struct SimpleBarChartLegend::Impl
{
	H::R< ID2D1SolidColorBrush > brush;

	FLOAT bar_width_;

	D2D1_RECT_F convert_point(D2D1_RECT_F const *rect, D2D_VECTOR_4F const *pt_ratio, FLOAT w_ratio)
	{
		D2D1_RECT_F bar;
		FLOAT w_rect = (rect->right - rect->left);
		FLOAT h_rect = (rect->bottom - rect->top);
		FLOAT w_bar = w_rect * w_ratio;

		bar.left = rect->left + pt_ratio->x * w_rect - w_bar * 0.5f;
		bar.right = bar.left + w_bar;

		bar.bottom = rect->bottom;
		bar.top = rect->bottom - pt_ratio->y * h_rect;

		return bar;
	}

	Impl(): brush(NULL), bar_width_(1.0f) { }
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

void SimpleBarChartLegend::SetBarWidth(FLOAT w)
{
	pImpl->bar_width_ = w;
}

FLOAT SimpleBarChartLegend::GetBarWidth(void)
{
	return pImpl->bar_width_;
}


void F35_NS::SimpleBarChartLegend::BeginDraw( ) const
{
	pImpl->brush = GetCurrentRenderer()->MakeBrush(GetFillColor());
}

void F35_NS::SimpleBarChartLegend::Draw( D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point, D2D_VECTOR_4F const *point_previous /*= NULL*/, D2D_VECTOR_4F const *point_next /*= NULL */ ) const
{
	if (!pImpl || !pImpl->brush) return;

	D2D1_RECT_F bar = pImpl->convert_point(GetCurrentChartRect(), point, pImpl->bar_width_ * GetCurrentRatioPlotToValue()->x);
	GetCurrentTarget()->FillRectangle(bar, pImpl->brush);
}

void F35_NS::SimpleBarChartLegend::EndDraw( ) const
{
}
