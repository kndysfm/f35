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

		virtual BOOL InternalRender( RendererBase * renderer, ID2D1RenderTarget * target);

		virtual void InternalInit( RendererBase * renderer );

		virtual void InternalUpdate( RendererBase * renderer );

		virtual void InternalDestroy( RendererBase * renderer );

	};
}

