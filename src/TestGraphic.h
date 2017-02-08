#pragma once
#include "GraphicsBase.h"

namespace F35_NS
{

	class TestGraphic :
		public GraphicsBase
	{
		struct Impl;
		Impl * pImpl;
	public:
		TestGraphic(void);
		virtual ~TestGraphic(void);

		virtual BOOL RenderGraphic( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs );

		virtual void InitGraphic( D2DRendererBase * renderer );

		virtual void UpdateGraphic( D2DRendererBase * renderer );

		virtual void DestroyGraphic( D2DRendererBase * renderer );

	};
}

