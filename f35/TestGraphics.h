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

		virtual BOOL InternalRender(ID2D1RenderTarget * target);

		virtual void InternalInit(ID2D1RenderTarget * target);

		virtual void InternalUpdate( void);

		virtual void InternalDestroy( void);

	};
}

