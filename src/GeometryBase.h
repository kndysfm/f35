#pragma once
#include "GraphicsBase.h"

namespace F35_NS
{
	class GeometryBase :
		public GraphicsBase
	{
	private:
		struct Impl;
		Impl *pImpl;
	public:
		GeometryBase(void);
		virtual ~GeometryBase(void);

		void SetLineWidth(FLOAT w);

		FLOAT GetLineWidth(void);

		void SetLineStyle(ID2D1StrokeStyle *style);

		ID2D1StrokeStyle * GetLineStyle(void);

		void SetLineColor(const D2D1::ColorF &c);

		D2D1_COLOR_F GetLineColor(void);

		void SetFillColor(const D2D1::ColorF & c);

		D2D1_COLOR_F GetFillColor(void);

	protected:

		virtual void InternalInit( RendererBase * renderer );

		virtual void InternalDestroy( RendererBase * renderer );

		ID2D1Brush *GetLineBrush(void);

		ID2D1Brush *GetFillBrush(void);

	};
};


