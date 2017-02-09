#pragma once

#include "GraphicsBase.h"

namespace F35_NS
{
	struct IGraphicsContainer
	{
		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX) = 0;

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics) = 0;

		virtual BOOL RemoveGraphics(GraphicsBase *graphics) = 0;
	};

	class GraphicsContainer :public GraphicsBase, public IGraphicsContainer
	{
	private:
		struct Impl;
		Impl *pImpl;

	public:
		GraphicsContainer(void);
		virtual ~GraphicsContainer(void);

		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX);

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics);

		virtual BOOL RemoveGraphics(GraphicsBase *graphics);

		virtual BOOL AttachRenderer(RendererBase * renderer);

		virtual void DettachRenderer(void);

	protected:

		virtual void InternalInit(RendererBase * renderer);

		virtual void InternalUpdate(RendererBase * renderer);

		virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target);

		virtual void InternalDestroy(RendererBase * renderer);

	};

}
