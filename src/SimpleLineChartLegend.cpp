#include "stdafx.h"
#include "SimpleLineChartLegend.h"
#include <stdarg.h>

USING_F35_NS;

struct SimpleLineChartLegend::Impl
{
	ID2D1Brush *line_brush;
	IDWriteTextFormat *textf;
	BOOL marker;
	
	static D2D1_POINT_2F convert_point(D2D1_RECT_F const *rect, ChartDataPoint const *pt_ratio)
	{
		D2D1_POINT_2F pt;

		pt.x = rect->left + pt_ratio->x * (rect->right - rect->left);
		pt.y = rect->top + pt_ratio->y * (rect->bottom - rect->top);

		return pt;
	}

	Impl(): line_brush(NULL), marker(FALSE), textf(NULL) { }
	~Impl()
	{
		if (line_brush)
			D2DRendererBase::SafeRelease(&line_brush);
		if (textf)
			D2DRendererBase::SafeRelease(&textf);
	}
};

SimpleLineChartLegend::SimpleLineChartLegend(void):
	pImpl(new Impl)
{
}


SimpleLineChartLegend::~SimpleLineChartLegend(void)
{
	delete pImpl;
	pImpl = NULL;
}

void F35_NS::SimpleLineChartLegend::ShowMarker( BOOL show )
{
	if (pImpl) pImpl->marker = show;
}

void F35_NS::SimpleLineChartLegend::BeginDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const
{
	if (pImpl->line_brush)
		D2DRendererBase::SafeRelease(&(pImpl->line_brush));
	pImpl->line_brush = renderer->MakeBrush(GetLineColor());

	if (pImpl->textf)
		D2DRendererBase::SafeRelease(&pImpl->textf);
	pImpl->textf = renderer->MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void F35_NS::SimpleLineChartLegend::Draw( D2DRendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, ChartDataPoint const *point_previous /*= NULL*/, ChartDataPoint const *point_next /*= NULL */ ) const
{
	if (!pImpl || pImpl->line_brush == NULL) return;

	D2D1_POINT_2F pt1 = Impl::convert_point(chart_rect, point);
	if (point_previous)
	{
		D2D1_POINT_2F pt0 = Impl::convert_point(chart_rect, point_previous);
		target->DrawLine(pt0, pt1, pImpl->line_brush);
	}

	if(pImpl->marker)
	{
		D2D1_ELLIPSE e;
		e.point = pt1;
		e.radiusX = e.radiusY = 2.0f;
		target->FillEllipse(e, pImpl->line_brush);
	}
}


void F35_NS::SimpleLineChartLegend::Print( D2DRendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const *chart_rect, ChartDataPoint const *point, LPCTSTR str_fmt, ... ) const
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	D2D1_RECT_F rect = *chart_rect;
	rect.left += point->x * (chart_rect->right - chart_rect->left);
	rect.top += point->y * (chart_rect->bottom - chart_rect->top);

	renderer->Print(pImpl->textf, rect, pImpl->line_brush, str);

	delete[] str;

	va_end(arg_ptr);
}


void F35_NS::SimpleLineChartLegend::EndDraw( D2DRendererBase *renderer, ID2D1RenderTarget * target ) const
{
	if (pImpl->line_brush)
		D2DRendererBase::SafeRelease(&(pImpl->line_brush));
	if (pImpl->textf)
		D2DRendererBase::SafeRelease(&pImpl->textf);
}
