#include "stdafx.h"
#include "GraphicsRenderer.h"

#include <deque>

USING_F35_NS;

struct GraphicsRenderer::Impl: public GraphicsContainer
{
	Impl(void){}
	virtual ~Impl(void){}

	void attach(ID2D1RenderTarget * target)
	{
		IGraphics *g_self = this;
		g_self->AttachRenderTarget(target); 
	}

	void dettach() 
	{
		IGraphics *g_self = this;
		g_self->DettachRenderTarget(); 
	}
};

GraphicsRenderer::GraphicsRenderer(HWND hwnd):
	RendererBase(hwnd),
	pImpl(new Impl)
{
	this->EnableAutoErase();
}


GraphicsRenderer::~GraphicsRenderer(void)
{
	delete pImpl;
}

void F35_NS::GraphicsRenderer::InternalUpdate( void )
{
	pImpl->Update();
}

void F35_NS::GraphicsRenderer::InternalRender( ID2D1RenderTarget *target )
{
	pImpl->Render(target);
	return;
}

HRESULT F35_NS::GraphicsRenderer::InternalInit( ID2D1RenderTarget *target )
{
	pImpl->attach(target);
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

