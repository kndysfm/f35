#include "stdafx.h"
#include "ChartLegendBase.h"

USING_F35_NS;

struct ChartLegendBase::Impl
{
	D2D1::ColorF line_color, fill_color;

	Impl (void):
		line_color(D2D1::ColorF(D2D1::ColorF::Black, 1.0f)),
		fill_color(D2D1::ColorF(D2D1::ColorF::Black, 1.0f))
	{

	}

	~Impl(void) { }
};

F35_NS::ChartLegendBase::ChartLegendBase( void ):
	pImpl(new Impl)
{

}

F35_NS::ChartLegendBase::~ChartLegendBase( void )
{
	delete pImpl;
	pImpl = NULL;
}

void F35_NS::ChartLegendBase::SetLineColor( const D2D1::ColorF & color )
{
	pImpl->line_color = color;
}

D2D1::ColorF F35_NS::ChartLegendBase::GetLineColor( void ) const
{
	return pImpl->line_color;
}

void F35_NS::ChartLegendBase::SetFillColor( const D2D1::ColorF & color )
{
	pImpl->fill_color = color;
}

D2D1::ColorF F35_NS::ChartLegendBase::GetFillColor( void ) const
{
	return pImpl->fill_color;
}
