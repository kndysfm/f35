#include "stdafx.h"
#include "GraphicsContainer.h"

#include <deque>

USING_F35_NS;
struct GraphicsContainer::Impl
{
	std::deque<GraphicsBase *> graphics;

};

GraphicsContainer::GraphicsContainer(void):
	pImpl(new Impl)
{
}


GraphicsContainer::~GraphicsContainer(void)
{
	delete pImpl;
}

BOOL F35_NS::GraphicsContainer::AddGraphic( GraphicsBase *graphic, LONG insert_index /*= LONG_MAX */ )
{
	GraphicsContainer *self = this;
	GraphicsBase * child = graphic;
	LONG idx = insert_index;

	if (idx >= pImpl->graphics.size())
	{
		if (child->AddContainer(self))
			pImpl->graphics.push_back(child);
		else 
			return FALSE;
	}
	else if (idx == 0)
	{
		if (child->AddContainer(self))
			pImpl->graphics.push_front(child);
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

		pImpl->graphics.insert(pImpl->graphics.begin() + idx, child);
	}

	if (child->GetRenderer() != NULL) child->DettachRenderer();
	child->AttachRenderer(self->GetRenderer());

	return TRUE;
}

LONG F35_NS::GraphicsContainer::GetIndexOfGraphic(GraphicsBase *g )
{
	LONG idx = 0;
	for (; idx < pImpl->graphics.size(); idx++)
	{
		if (g == pImpl->graphics[idx])
			return idx;
	}
	return LONG_MIN;
}

BOOL F35_NS::GraphicsContainer::RemoveGraphic( GraphicsBase *g )
{
	GraphicsContainer* self = this;

	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		if ((*itr) == g)
		{
			pImpl->graphics.erase(itr);
			g->RemoveContainer(self);
			return TRUE;
		}
	}
	return FALSE;
}

void F35_NS::GraphicsContainer::UpdateGraphic( D2DRendererBase * renderer )
{
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->UpdateGraphic(renderer);
	}
}

void F35_NS::GraphicsContainer::InitGraphic( D2DRendererBase * renderer )
{
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->InitGraphic(renderer);
	}
}

void F35_NS::GraphicsContainer::DestroyGraphic( D2DRendererBase * renderer )
{
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->DestroyGraphic(renderer);
	}
}

BOOL F35_NS::GraphicsContainer::RenderGraphic( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs )
{
	BOOL ret = FALSE;
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		D2D1_POINT_2F pt = pt_abs;
		D2D1_POINT_2F pos = (*itr)->GetPosition();
		pt.x += pos.x;
		pt.y += pos.y;
		ret = (*itr)->RenderGraphic(renderer, target, pt) || ret;
	}
	return ret;
}

BOOL F35_NS::GraphicsContainer::AttachRenderer( D2DRendererBase * renderer )
{
	BOOL ret = FALSE;
	GraphicsBase::AttachRenderer(renderer);
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		GraphicsBase *g = *itr;
		if (g->GetRenderer() != NULL) g->DettachRenderer();
		ret = g->AttachRenderer(renderer) || ret;
	}
	return ret;
}

void F35_NS::GraphicsContainer::DettachRenderer( void )
{
	for (std::deque<GraphicsBase *>::iterator itr = pImpl->graphics.begin();
		itr != pImpl->graphics.end(); itr++)
	{
		(*itr)->DettachRenderer();
	}
	GraphicsBase::DettachRenderer();
}
