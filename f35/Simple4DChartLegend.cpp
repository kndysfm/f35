#include "stdafx.h"
#include "Simple4DChartLegend.h"

USING_F35_NS;

struct Simple4DChartLegend::Impl
{
	H::R<ID2D1SolidColorBrush> line_brush;

	static D2D1_POINT_2F convert_point_xy(D2D1_RECT_F const *rect, D2D_VECTOR_4F const *pt_ratio)
	{
		D2D1_POINT_2F pt;

		pt.x = rect->left + pt_ratio->x * (rect->right - rect->left);
		pt.y = rect->top + pt_ratio->y * (rect->bottom - rect->top);

		return pt;
	}

	static D2D1_POINT_2F convert_point_zw(D2D1_RECT_F const *rect, D2D_VECTOR_4F const *pt_ratio)
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

void F35_NS::Simple4DChartLegend::BeginDraw( ) const
{
	pImpl->line_brush = GetCurrentRenderer()->MakeBrush(GetLineColor());
}

void F35_NS::Simple4DChartLegend::Draw( D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point, D2D_VECTOR_4F const *point_previous /*= NULL*/, D2D_VECTOR_4F const *point_next /*= NULL */ ) const
{
	if (!pImpl || !pImpl->line_brush) return;

	D2D1_POINT_2F pt0 = Impl::convert_point_xy(GetCurrentChartRect(), point);
	D2D1_POINT_2F pt1 = Impl::convert_point_zw(GetCurrentChartRect(), point);
	GetCurrentTarget()->DrawLine(pt0, pt1, pImpl->line_brush);
}

void F35_NS::Simple4DChartLegend::EndDraw( ) const
{
}
