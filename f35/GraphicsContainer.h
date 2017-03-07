#pragma once

#include "GraphicsBase.h"

namespace F35_NS
{
	class IGraphicsContainer
	{
	public:

		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX) = 0;

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics) = 0;

		virtual BOOL RemoveGraphics(GraphicsBase *graphics) = 0;
	};

	class GraphicsContainer :public GraphicsBase, public IGraphicsContainer
	{
		friend class IGraphicsContainer;

	private:
		struct Impl;
		Impl *pImpl;

		virtual BOOL AttachRenderTarget(ID2D1RenderTarget * target) sealed;

		virtual void DettachRenderTarget(void) sealed;

	public:
		GraphicsContainer(void);
		virtual ~GraphicsContainer(void);

		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX) sealed;

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics) sealed;

		virtual BOOL RemoveGraphics(GraphicsBase *graphics) sealed;

	protected:

		virtual void InternalInit(ID2D1RenderTarget * target);

		virtual void InternalUpdate(void);

		virtual BOOL InternalRender(ID2D1RenderTarget * target);

		virtual void InternalDestroy(void);

	};

}
