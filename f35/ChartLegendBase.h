#pragma once
#include "F35Helper.h"

#include "RendererBase.h"
#include "ChartDataSeriesBase.h"

namespace F35_NS
{
	class IChartLegend
	{
	public:
		virtual void Setup(RendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const &chart_rect, D2D_VECTOR_4F const &ratio_plot_to_value) const = 0;

		virtual void BeginDraw(void) const = 0;

		virtual void Draw(D2D_VECTOR_4F const *point_value, D2D_VECTOR_4F const *point_draw,
			D2D_VECTOR_4F const *point_previous = NULL, D2D_VECTOR_4F const *point_next = NULL) const = 0;

		virtual void Print(D2D_VECTOR_4F const *point, LPCTSTR str_fmt, ...) const = 0;

		virtual void EndDraw(void) const = 0;

	};

	class ChartLegendBase : public IChartLegend, H::NonCopyable
	{
		struct Impl;
		Impl *pImpl;
	public:
		ChartLegendBase(void);
		virtual ~ChartLegendBase(void);

		void SetLineColor(const D2D1::ColorF & color);
		D2D1::ColorF  GetLineColor(void) const;

		void SetFillColor(const D2D1::ColorF & color);
		D2D1::ColorF  GetFillColor(void) const;

	protected:
		virtual void Setup(RendererBase *renderer, ID2D1RenderTarget * target, D2D1_RECT_F const &chart_rect, D2D_VECTOR_4F const &ratio_plot_to_value) const sealed;

		RendererBase *GetCurrentRenderer() const;

		ID2D1RenderTarget * GetCurrentTarget() const;

		D2D1_RECT_F const * GetCurrentChartRect() const;

		D2D_VECTOR_4F const * GetCurrentRatioPlotToValue() const;

		virtual void BeginDraw(void) const = 0;

		virtual void Draw(D2D_VECTOR_4F const *point_value, D2D_VECTOR_4F const *point_draw,
			D2D_VECTOR_4F const *point_previous = NULL, D2D_VECTOR_4F const *point_next = NULL) const = 0;

		virtual void Print(D2D_VECTOR_4F const *point, LPCTSTR str_fmt, ...) const = 0;

		virtual void EndDraw(void) const = 0;


	};
}
