#include "stdafx.h"
#include "SimpleLineChartLegend.h"
#include <stdarg.h>

USING_F35_NS;

struct SimpleLineChartLegend::Impl
{
	H::R<ID2D1SolidColorBrush> line_brush;
	H::R<IDWriteTextFormat> textf;
	BOOL marker;
	
	static D2D1_POINT_2F convert_point(D2D1_RECT_F const *rect, D2D_VECTOR_4F const *pt_ratio)
	{
		D2D1_POINT_2F pt;

		pt.x = rect->left + pt_ratio->x * (rect->right - rect->left);
		pt.y = rect->top + pt_ratio->y * (rect->bottom - rect->top);

		return pt;
	}

	Impl(): line_brush(NULL), marker(FALSE), textf(NULL) { }
	~Impl()
	{
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

void F35_NS::SimpleLineChartLegend::BeginDraw( ) const
{
	pImpl->line_brush = GetCurrentRenderer()->MakeBrush(GetLineColor());

	pImpl->textf = GetCurrentRenderer()->MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void F35_NS::SimpleLineChartLegend::Draw( D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point, D2D_VECTOR_4F const *point_previous /*= NULL*/, D2D_VECTOR_4F const *point_next /*= NULL */ ) const
{
	if (!pImpl || !pImpl->line_brush) return;

	D2D1_POINT_2F pt1 = Impl::convert_point(GetCurrentChartRect(), point);
	if (point_previous)
	{
		D2D1_POINT_2F pt0 = Impl::convert_point(GetCurrentChartRect(), point_previous);
		GetCurrentTarget()->DrawLine(pt0, pt1, pImpl->line_brush);
	}

	if(pImpl->marker)
	{
		D2D1_ELLIPSE e;
		e.point = pt1;
		e.radiusX = e.radiusY = 2.0f;
		GetCurrentTarget()->FillEllipse(e, pImpl->line_brush);
	}
}


void F35_NS::SimpleLineChartLegend::Print( D2D_VECTOR_4F const *point, LPCTSTR str_fmt, ... ) const
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	D2D1_RECT_F rect = *GetCurrentChartRect();
	rect.left += point->x * (GetCurrentChartRect()->right - GetCurrentChartRect()->left);
	rect.top += point->y * (GetCurrentChartRect()->bottom - GetCurrentChartRect()->top);

	H::WriteText(GetCurrentTarget(), pImpl->textf, rect, pImpl->line_brush, str);

	delete[] str;

	va_end(arg_ptr);
}


void F35_NS::SimpleLineChartLegend::EndDraw( ) const
{
}
