#include "stdafx.h"
#include "GraphicsContainer.h"

#include <deque>

USING_F35_NS;

namespace
{
	typedef std::deque<IGraphics *> GraphicsQue;
}

struct GraphicsContainer::Impl
{
	GraphicsQue graphics;
};

GraphicsContainer::GraphicsContainer(void):
	pImpl(new Impl)
{
}


GraphicsContainer::~GraphicsContainer(void)
{
	delete pImpl;
}

BOOL F35_NS::GraphicsContainer::AddGraphics(GraphicsBase *graphics, LONG insert_index /*= LONG_MAX */ )
{
	IGraphics *g_self = this;
	IGraphics * g_new = graphics;
	LONG idx = insert_index;

	if (idx >= pImpl->graphics.size())
	{
		if (g_new->RegisterContainer(this))
			pImpl->graphics.push_back(g_new);
		else 
			return FALSE;
	}
	else if (idx == 0)
	{
		if (g_new->RegisterContainer(this))
			pImpl->graphics.push_front(g_new);
		else 
			return FALSE;
	}
	else 
	{
		if (idx < 0)
		{
			idx += pImpl->graphics.size();
			if (idx < 0) 
				return FALSE;
		}

		pImpl->graphics.insert(pImpl->graphics.begin() + idx, g_new);
	}

	if (g_new->GetRenderer() != NULL) g_new->DettachRenderer();
	g_new->AttachRenderer(g_self->GetRenderer());

	return TRUE;
}

LONG F35_NS::GraphicsContainer::GetIndexOfGraphics(GraphicsBase *graphics)
{
	IGraphics *p = graphics;
	LONG idx = 0;
	for (; idx < pImpl->graphics.size(); idx++)
	{
		if (p == pImpl->graphics[idx])
			return idx;
	}
	return LONG_MIN;
}

BOOL F35_NS::GraphicsContainer::RemoveGraphics(GraphicsBase *graphics)
{
	IGraphics *p = graphics;
	GraphicsContainer* self = this;

	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		if ((*itr) == p)
		{
			pImpl->graphics.erase(itr);
			p->DeregisterContainer(self);
			return TRUE;
		}
	}
	return FALSE;
}

void F35_NS::GraphicsContainer::InternalUpdate( RendererBase * renderer )
{
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->Update(renderer);
	}
}

void F35_NS::GraphicsContainer::InternalInit( RendererBase * renderer )
{
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->Init(renderer);
	}
}

void F35_NS::GraphicsContainer::InternalDestroy( RendererBase * renderer )
{
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->Destroy(renderer);
	}
}

BOOL F35_NS::GraphicsContainer::InternalRender( RendererBase * renderer, ID2D1RenderTarget * target)
{
	BOOL ret = FALSE;
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		ret = (*itr)->Render(renderer, target) || ret;
	}
	return ret;
}

BOOL F35_NS::GraphicsContainer::AttachRenderer( RendererBase * renderer )
{
	BOOL ret = FALSE;
	GraphicsBase::AttachRenderer(renderer);
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		IGraphics *g = *itr;
		if (g->GetRenderer() != NULL) g->DettachRenderer();
		ret = g->AttachRenderer(renderer) || ret;
	}
	return ret;
}

void F35_NS::GraphicsContainer::DettachRenderer( void )
{
	for (GraphicsQue::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->DettachRenderer();
	}
	GraphicsBase::DettachRenderer();
}
