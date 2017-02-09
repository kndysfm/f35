#include "stdafx.h"

#include "F35Helper.h"

#include <stdarg.h>

D2D1_RECT_F F35_NS::H::MakeRectRatios(D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	D2D1_RECT_F new_rect;

	new_rect.left = left * src_rect.right + (1.0f - left) * src_rect.left;
	new_rect.top = top * src_rect.bottom + (1.0f - top) * src_rect.top;
	new_rect.right = right * src_rect.right + (1.0f - right) * src_rect.left;
	new_rect.bottom = bottom * src_rect.bottom + (1.0f - bottom) * src_rect.top;

	return new_rect;
}

D2D1_RECT_F F35_NS::H::MakeRectRatios(D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect)
{
	return MakeRectRatios(src_rect, ratio_rect.left, ratio_rect.top, ratio_rect.right, ratio_rect.bottom);
}

D2D1_POINT_2F F35_NS::H::MakePointRatio(D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio)
{
	D2D1_POINT_2F new_pt;

	new_pt.x = ratio * pt1.x, +(1.0f - ratio) * pt0.x;
	new_pt.y = ratio * pt1.y, +(1.0f - ratio) * pt0.y;

	return new_pt;
}

void F35_NS::H::PaintRectangle(ID2D1RenderTarget *target, const D2D1_RECT_F &rect, ID2D1Brush *fill_brush, ID2D1Brush *line_brush, FLOAT line_width)
{
	target->FillRectangle(rect, fill_brush);
	target->DrawRectangle(rect, line_brush, line_width);
}

void F35_NS::H::WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect, ID2D1Brush *brush, LPCTSTR str_fmt, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	target->DrawText(str, _tcslen(str), format, rect, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	delete[] str;

	va_end(arg_ptr);
}

void F35_NS::H::WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect,
	DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align,
	ID2D1Brush *brush, LPCTSTR str_fmt, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	DWRITE_TEXT_ALIGNMENT last_text_align = format->GetTextAlignment();
	DWRITE_PARAGRAPH_ALIGNMENT last_paragraph_align = format->GetParagraphAlignment();
	format->SetTextAlignment(text_align);
	format->SetParagraphAlignment(paragraph_align);
	target->DrawText(str, _tcslen(str), format, rect, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	format->SetTextAlignment(last_text_align);
	format->SetParagraphAlignment(last_paragraph_align);

	delete[] str;

	va_end(arg_ptr);
}
