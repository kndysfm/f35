#pragma once
#include "GraphicsBase.h"

namespace F35_NS
{

	class TestGraphics :
		public GraphicsBase
	{
		struct Impl;
		Impl * pImpl;
	public:
		TestGraphics(void);
		virtual ~TestGraphics(void);

		virtual BOOL InternalRender( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs );

		virtual void InternalInit( D2DRendererBase * renderer );

		virtual void InternalUpdate( D2DRendererBase * renderer );

		virtual void InternalDestroy( D2DRendererBase * renderer );

	};
}

