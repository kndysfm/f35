#include "stdafx.h"
#include "GraphicsRenderer.h"

#include <deque>

USING_F35_NS;

struct GraphicsRenderer::Impl: public GraphicsContainer
{
	Impl(void){}
	virtual ~Impl(void){}
};

GraphicsRenderer::GraphicsRenderer(HWND hwnd):
	D2DRendererBase(hwnd),
	pImpl(new Impl)
{
}


GraphicsRenderer::~GraphicsRenderer(void)
{
	delete pImpl;
}

void F35_NS::GraphicsRenderer::InternalUpdate( void )
{
	pImpl->UpdateGraphic(this);
}

HRESULT F35_NS::GraphicsRenderer::InternalRender( ID2D1RenderTarget *target )
{
	pImpl->RenderGraphic(this, target, D2D1::Point2F());
	return S_OK;
}

HRESULT F35_NS::GraphicsRenderer::InternalInit( ID2D1RenderTarget *target )
{
	pImpl->AttachRenderer(this);
	return S_OK;
}

void F35_NS::GraphicsRenderer::InternalDestroy( void )
{
	pImpl->DettachRenderer();
}

BOOL F35_NS::GraphicsRenderer::AddGraphic( GraphicsBase *graphic, LONG insert_index /*= LONG_MAX */ )
{
	return pImpl->AddGraphic(graphic, insert_index);
}

LONG F35_NS::GraphicsRenderer::GetIndexOfGraphic( GraphicsBase *graphic )
{
	return pImpl->GetIndexOfGraphic(graphic);
}

BOOL F35_NS::GraphicsRenderer::RemoveGraphic( GraphicsBase *graphic )
{
	return pImpl->RemoveGraphic(graphic);
}

