#include "stdafx.h"
#include "Geometry.h"

USING_F35_NS;

struct Geometry::Impl
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

Geometry::Geometry(void):
	pImpl(new Impl)
{
}


Geometry::~Geometry(void)
{
	delete pImpl;
}

void F35_NS::Geometry::SetLineWidth( FLOAT w )
{
	pImpl->line_width = w;
}

FLOAT F35_NS::Geometry::GetLineWidth( void )
{
	return pImpl->line_width;
}

void F35_NS::Geometry::SetLineStyle( ID2D1StrokeStyle *style )
{
	pImpl->style = style;
}

ID2D1StrokeStyle * F35_NS::Geometry::GetLineStyle( void )
{
	return pImpl->style; 
}

void F35_NS::Geometry::SetLineColor( const D2D1::ColorF & c )
{
	pImpl->line_color = c;
	if (pImpl->line_brush) (&pImpl->line_brush)->SetColor(c);
}

D2D1_COLOR_F F35_NS::Geometry::GetLineColor( void )
{
	return (&pImpl->line_brush)->GetColor();
}

void F35_NS::Geometry::SetFillColor( const D2D1::ColorF & c )
{
	pImpl->fill_color = c;
	if (pImpl->fill_brush) (&pImpl->fill_brush)->SetColor(c);
}

D2D1_COLOR_F F35_NS::Geometry::GetFillColor( void )
{
	return (&pImpl->fill_brush)->GetColor();
}

void F35_NS::Geometry::InternalInit( D2DRendererBase * renderer )
{
	pImpl->line_brush = renderer->MakeBrush(pImpl->line_color);
	pImpl->fill_brush = renderer->MakeBrush(pImpl->fill_color);
}

void F35_NS::Geometry::InternalDestroy( D2DRendererBase * renderer )
{
}

ID2D1Brush * F35_NS::Geometry::GetLineBrush( void )
{
	return pImpl->line_brush;
}

ID2D1Brush * F35_NS::Geometry::GetFillBrush( void )
{
	return pImpl->fill_brush;
}
