#include "stdafx.h"
#include "GraphicsRenderer.h"

#include <deque>

USING_F35_NS;

struct GraphicsRenderer::Impl: public GraphicsContainer
{
	Impl(void){}
	virtual ~Impl(void){}

	void attach(RendererBase *r) 
	{
		IGraphics *g_self = this;
		g_self->AttachRenderer(r); 
	}

	void dettach() 
	{
		IGraphics *g_self = this;
		g_self->DettachRenderer(); 
	}
};

GraphicsRenderer::GraphicsRenderer(HWND hwnd):
	RendererBase(hwnd),
	pImpl(new Impl)
{
}


GraphicsRenderer::~GraphicsRenderer(void)
{
	delete pImpl;
}

void F35_NS::GraphicsRenderer::InternalUpdate( void )
{
	pImpl->Update(this);
}

void F35_NS::GraphicsRenderer::InternalRender( ID2D1RenderTarget *target )
{
	pImpl->Render(this, target);
	return;
}

HRESULT F35_NS::GraphicsRenderer::InternalInit( ID2D1RenderTarget *target )
{
	pImpl->attach(this);
	return S_OK;
}

void F35_NS::GraphicsRenderer::InternalDestroy( void )
{
	pImpl->dettach();
}

BOOL F35_NS::GraphicsRenderer::AddGraphics(GraphicsBase *graphics, LONG insert_index /*= LONG_MAX */ )
{
	return pImpl->AddGraphics(graphics, insert_index);
}

LONG F35_NS::GraphicsRenderer::GetIndexOfGraphics(GraphicsBase *graphics )
{
	return pImpl->GetIndexOfGraphics(graphics);
}

BOOL F35_NS::GraphicsRenderer::RemoveGraphics(GraphicsBase *graphics )
{
	return pImpl->RemoveGraphics(graphics);
}

