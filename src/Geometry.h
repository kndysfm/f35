#pragma once
#include "GraphicsBase.h"

namespace F35_NS
{
	class Geometry :
		public GraphicsBase
	{
	private:
		struct Impl;
		Impl *pImpl;
	public:
		Geometry(void);
		virtual ~Geometry(void);

		void SetLineWidth(FLOAT w);

		FLOAT GetLineWidth(void);

		void SetLineStyle(ID2D1StrokeStyle *style);

		ID2D1StrokeStyle * GetLineStyle(void);

		void SetLineColor(const D2D1::ColorF &c);

		D2D1_COLOR_F GetLineColor(void);

		void SetFillColor(const D2D1::ColorF & c);

		D2D1_COLOR_F GetFillColor(void);

	protected:

		virtual void InternalInit( D2DRendererBase * renderer );

		virtual void InternalDestroy( D2DRendererBase * renderer );

		ID2D1Brush *GetLineBrush(void);

		ID2D1Brush *GetFillBrush(void);

	};
};


