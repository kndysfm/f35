#pragma once
#include "F35Helper.h"

#include "RendererBase.h"
#include "ChartDataSeriesBase.h"

namespace F35_NS
{
	class ChartLegendBase : H::NonCopyable
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

		virtual void BeginDraw(RendererBase *renderer, ID2D1RenderTarget * target) const = 0;

		virtual void Draw(RendererBase *renderer, ID2D1RenderTarget * target,
			D2D1_RECT_F const *chart_rect, D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point,
			D2D_VECTOR_4F const *point_previous = NULL, D2D_VECTOR_4F const *point_next = NULL) const = 0;

		virtual void Print(RendererBase *renderer, ID2D1RenderTarget * target,
			D2D1_RECT_F const *chart_rect, D2D_VECTOR_4F const *point,
			LPCTSTR str_fmt, ...) const = 0;

		virtual void EndDraw(RendererBase *renderer, ID2D1RenderTarget * target) const = 0;

	};
}
