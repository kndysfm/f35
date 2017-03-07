#include "stdafx.h"
#include "ChartLegendBase.h"

USING_F35_NS;

struct ChartLegendBase::Impl
{
	D2D1::ColorF line_color_, fill_color_;

	ID2D1RenderTarget * target_;
	D2D1_RECT_F chart_rect_;
	D2D_VECTOR_4F ratio_plot_to_value_;

	Impl (void):
		line_color_(D2D1::ColorF(D2D1::ColorF::Black, 1.0f)),
		fill_color_(D2D1::ColorF(D2D1::ColorF::Black, 1.0f)),
		target_(nullptr),
		chart_rect_(D2D1::RectF()), ratio_plot_to_value_({ 0.0f })
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
	pImpl->line_color_ = color;
}

D2D1::ColorF F35_NS::ChartLegendBase::GetLineColor( void ) const
{
	return pImpl->line_color_;
}

void F35_NS::ChartLegendBase::SetFillColor( const D2D1::ColorF & color )
{
	pImpl->fill_color_ = color;
}

D2D1::ColorF F35_NS::ChartLegendBase::GetFillColor( void ) const
{
	return pImpl->fill_color_;
}

void F35_NS::ChartLegendBase::Setup(ID2D1RenderTarget * target, D2D1_RECT_F const &chart_rect, D2D_VECTOR_4F const &ratio_plot_to_value) const
{
	pImpl->target_ = target;
	pImpl->chart_rect_ = chart_rect;
	pImpl->ratio_plot_to_value_ = ratio_plot_to_value;
}

ID2D1RenderTarget * F35_NS::ChartLegendBase::GetCurrentTarget() const
{
	return pImpl->target_;
}

D2D1_RECT_F const * F35_NS::ChartLegendBase::GetCurrentChartRect() const
{
	return &pImpl->chart_rect_;
}

D2D_VECTOR_4F const * F35_NS::ChartLegendBase::GetCurrentRatioPlotToValue() const
{
	return &pImpl->ratio_plot_to_value_;
}

