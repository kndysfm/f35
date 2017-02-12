#include "stdafx.h"
#include "GeometryBase.h"

USING_F35_NS;

struct GeometryBase::Impl
{
	H::R< ID2D1SolidColorBrush> line_brush;
	H::R< ID2D1SolidColorBrush> fill_brush;

	FLOAT line_width;

	D2D1::ColorF line_color, fill_color;

	ID2D1StrokeStyle *style;

	Impl(void): 
		line_brush(NULL), 
		fill_brush(NULL), 
		line_width(1.0f),
		line_color(D2D1::ColorF(D2D1::ColorF::Black, 0.0f)),
		fill_color(D2D1::ColorF(D2D1::ColorF::Black, 0.0f)),
		style(NULL)
	{ }

	~Impl(void)
	{
	}
};

GeometryBase::GeometryBase(void):
	pImpl(new Impl)
{
}


GeometryBase::~GeometryBase(void)
{
	delete pImpl;
}

void F35_NS::GeometryBase::SetLineWidth( FLOAT w )
{
	pImpl->line_width = w;
}

FLOAT F35_NS::GeometryBase::GetLineWidth( void )
{
	return pImpl->line_width;
}

void F35_NS::GeometryBase::SetLineStyle( ID2D1StrokeStyle *style )
{
	pImpl->style = style;
}

ID2D1StrokeStyle * F35_NS::GeometryBase::GetLineStyle( void )
{
	return pImpl->style; 
}

void F35_NS::GeometryBase::SetLineColor( const D2D1::ColorF & c )
{
	pImpl->line_color = c;
	if (pImpl->line_brush) (&pImpl->line_brush)->SetColor(c);
}

D2D1_COLOR_F F35_NS::GeometryBase::GetLineColor( void )
{
	return (&pImpl->line_brush)->GetColor();
}

void F35_NS::GeometryBase::SetFillColor( const D2D1::ColorF & c )
{
	pImpl->fill_color = c;
	if (pImpl->fill_brush) (&pImpl->fill_brush)->SetColor(c);
}

D2D1_COLOR_F F35_NS::GeometryBase::GetFillColor( void )
{
	return (&pImpl->fill_brush)->GetColor();
}

void F35_NS::GeometryBase::InternalInit( RendererBase * renderer )
{
	pImpl->line_brush = renderer->MakeBrush(pImpl->line_color);
	pImpl->fill_brush = renderer->MakeBrush(pImpl->fill_color);
}

void F35_NS::GeometryBase::InternalDestroy( RendererBase * renderer )
{
	pImpl->line_brush = NULL;
	pImpl->fill_brush = NULL;
}

ID2D1Brush * F35_NS::GeometryBase::GetLineBrush( void )
{
	return pImpl->line_brush;
}

ID2D1Brush * F35_NS::GeometryBase::GetFillBrush( void )
{
	return pImpl->fill_brush;
}
