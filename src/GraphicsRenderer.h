#pragma once
#include "rendererbase.h"
#include "GraphicsContainer.h"

namespace F35_NS
{

	class GraphicsRenderer :
		public RendererBase,
		public IGraphicsContainer
	{
	private:
		struct Impl;
		Impl *pImpl;

	public:
		GraphicsRenderer(HWND hwnd);
		virtual ~GraphicsRenderer(void);

		virtual BOOL AddGraphics(GraphicsBase *graphics, LONG insert_index = LONG_MAX);

		virtual LONG GetIndexOfGraphics(GraphicsBase *graphics);

		virtual BOOL RemoveGraphics(GraphicsBase *graphics);

	protected:

		virtual HRESULT InternalInit(ID2D1RenderTarget *target);

		virtual void InternalUpdate(void);

		virtual HRESULT InternalRender(ID2D1RenderTarget *target);

		virtual void InternalDestroy(void);

	};

}