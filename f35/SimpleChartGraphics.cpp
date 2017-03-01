#include "stdafx.h"
#include "SimpleChartGraphics.h"


USING_F35_NS;


struct SimpleChartGraphics::Impl
{
	RendererBase *renderer;

	H::R<ID2D1SolidColorBrush> axis_line_brush;
	H::R<ID2D1SolidColorBrush> major_grid_brush;
	H::R<ID2D1SolidColorBrush> minor_grid_brush;
	H::R<ID2D1SolidColorBrush> text_brush;
	H::R<IDWriteTextFormat> x_label_textf;
	H::R<IDWriteTextFormat> y_label_textf;
	H::R<IDWriteTextFormat> mess_textf;

	LPCTSTR mess;

	Impl(RendererBase *r): 
		renderer(r), 
		axis_line_brush( r->MakeBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.00f))),
		major_grid_brush(r->MakeBrush(D2D1::ColorF(D2D1::ColorF::Black, 0.75f))),
		minor_grid_brush(r->MakeBrush(D2D1::ColorF(D2D1::ColorF::Black, 0.50f))),
		text_brush(r->MakeBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.00f))),
		y_label_textf(r->MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR)),
		x_label_textf(r->MakeTextFormat(_T("MS Gothic"), 8.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR)),
		mess_textf(r->MakeTextFormat(_T("MS Gothic"), 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER)),
		mess(NULL)
	{
	}

	~Impl(void)
	{
		renderer = NULL;
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

void F35_NS::SimpleChartGraphics::InternalInit( RendererBase * renderer)
{
	if (!pImpl)
	{
		pImpl = new Impl(renderer);
	}
}

void F35_NS::SimpleChartGraphics::InternalUpdate( RendererBase * )
{
	// do nothing	
}

BOOL F35_NS::SimpleChartGraphics::InternalRender( RendererBase * renderer, ID2D1RenderTarget * target )
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

	DrawMinorGridLinesX(renderer, target, &rect, pImpl->minor_grid_brush);
	DrawMajorGridLinesX(renderer, target, &rect, pImpl->major_grid_brush);
	DrawAxisLineX(renderer, target, &rect, pImpl->axis_line_brush, 1.5f);
	DrawMajorGridLabelX(renderer, target, &rect, 2.0f, 20.0f,
		pImpl->x_label_textf, pImpl->major_grid_brush,
		GetChartNumberStringFormat());

	DrawMinorGridLinesY(renderer, target, &rect, pImpl->minor_grid_brush);
	DrawMajorGridLinesY(renderer, target, &rect, pImpl->major_grid_brush);
	DrawAxisLineY(renderer, target, &rect, pImpl->axis_line_brush, 1.5f);
	DrawMajorGridLabelY(renderer, target, &rect, -2.0f, 30.0f,
		pImpl->y_label_textf, pImpl->major_grid_brush, 
		GetChartNumberStringFormat());

	PlotChartData(renderer, target, &rect);

	PlotLegends(renderer, target, &rect);

	if (pImpl->mess != NULL)
	{
		pImpl->text_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Silver));
		H::WriteText(target, pImpl->mess_textf, rect,  pImpl->text_brush, pImpl->mess);
	}

	return TRUE;
}

void F35_NS::SimpleChartGraphics::InternalDestroy( RendererBase * )
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
