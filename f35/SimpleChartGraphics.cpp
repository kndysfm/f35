#include "stdafx.h"
#include "SimpleChartGraphics.h"


USING_F35_NS;


struct SimpleChartGraphics::Impl
{
	ID2D1RenderTarget * target;

	H::R<ID2D1SolidColorBrush> axis_line_brush;
	H::R<ID2D1SolidColorBrush> major_grid_brush;
	H::R<ID2D1SolidColorBrush> minor_grid_brush;
	H::R<ID2D1SolidColorBrush> text_brush;
	H::R<IDWriteTextFormat> x_label_textf;
	H::R<IDWriteTextFormat> y_label_textf;
	H::R<IDWriteTextFormat> mess_textf;

	LPCTSTR mess;

	Impl(ID2D1RenderTarget * t):
		target(t), 
		axis_line_brush( H::MakeSolidColorBrush(t, D2D1::ColorF(D2D1::ColorF::Black, 1.00f))),
		major_grid_brush(H::MakeSolidColorBrush(t, D2D1::ColorF(D2D1::ColorF::Black, 0.75f))),
		minor_grid_brush(H::MakeSolidColorBrush(t, D2D1::ColorF(D2D1::ColorF::Black, 0.50f))),
		text_brush(H::MakeSolidColorBrush(t, D2D1::ColorF(D2D1::ColorF::Black, 1.00f))),
		y_label_textf(Factory::MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR)),
		x_label_textf(Factory::MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR)),
		mess_textf(Factory::MakeTextFormat(_T("MS Gothic"), 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER)),
		mess(NULL)
	{
	}

	~Impl(void)
	{
		target = NULL;
	}
};

SimpleChartGraphics::SimpleChartGraphics(void):
	pImpl(NULL)
{
}


SimpleChartGraphics::~SimpleChartGraphics(void)
{
	if (pImpl)
	{
		delete pImpl;
		pImpl = NULL;
	}
}

void F35_NS::SimpleChartGraphics::InternalInit(ID2D1RenderTarget * target)
{
	if (!pImpl)
	{
		pImpl = new Impl(target);
	}
}

void F35_NS::SimpleChartGraphics::InternalUpdate( void)
{
	// do nothing	
}

BOOL F35_NS::SimpleChartGraphics::InternalRender( ID2D1RenderTarget * target )
{
	if (!pImpl) return FALSE;

	D2D1_SIZE_F size = GetChartAreaSize();
	D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width, size.height);

	D2D1::ColorF color_fg = GetForegroundColor();
	pImpl->axis_line_brush->SetColor(color_fg);
	color_fg.a = 0.75f;
	pImpl->major_grid_brush->SetColor(color_fg);
	color_fg.a = 0.25f;
	pImpl->minor_grid_brush->SetColor(color_fg);

	rect.left += 30.0f;
	rect.right -= 5.0f;
	rect.top += 5.0f; 
	rect.bottom -= 20.0f; 

	target->DrawRectangle(rect, pImpl->axis_line_brush, 0.5f);

	DrawMinorGridLinesX(target, &rect, pImpl->minor_grid_brush);
	DrawMajorGridLinesX(target, &rect, pImpl->major_grid_brush);
	DrawAxisLineX(target, &rect, pImpl->axis_line_brush, 1.5f);
	DrawMajorGridLabelX(target, &rect, 2.0f, 20.0f,
		pImpl->x_label_textf, pImpl->major_grid_brush,
		GetChartNumberStringFormat());

	DrawMinorGridLinesY(target, &rect, pImpl->minor_grid_brush);
	DrawMajorGridLinesY(target, &rect, pImpl->major_grid_brush);
	DrawAxisLineY(target, &rect, pImpl->axis_line_brush, 1.5f);
	DrawMajorGridLabelY(target, &rect, -2.0f, 30.0f,
		pImpl->y_label_textf, pImpl->major_grid_brush, 
		GetChartNumberStringFormat());

	PlotChartData(target, &rect);

	PlotLegends(target, &rect);

	if (pImpl->mess != NULL)
	{
		pImpl->text_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Silver));
		H::WriteText(target, pImpl->mess_textf, rect,  pImpl->text_brush, pImpl->mess);
	}

	return TRUE;
}

void F35_NS::SimpleChartGraphics::InternalDestroy( void )
{
	if (!pImpl) return;

	delete pImpl;
	pImpl = NULL;
}

void F35_NS::SimpleChartGraphics::ShowMessage( LPCTSTR mess )
{
	if (!pImpl) return;

	pImpl->mess = mess;
}
