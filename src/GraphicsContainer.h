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

		virtual BOOL AttachRenderer(RendererBase * renderer) sealed;

		virtual void DettachRenderer(void) sealed;

	public:
		GraphicsContainer(void);
		virtual ~GraphicsContainer(void);

		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX) sealed;

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics) sealed;

		virtual BOOL RemoveGraphics(GraphicsBase *graphics) sealed;

	protected:

		virtual void InternalInit(RendererBase * renderer);

		virtual void InternalUpdate(RendererBase * renderer);

		virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target);

		virtual void InternalDestroy(RendererBase * renderer);

	};

}
