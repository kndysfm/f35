#include "stdafx.h"
#include "GraphicsBase.h"


USING_F35_NS;

struct GraphicsBase::Impl
{
	D2D1_POINT_2F position;
	D2D1_SIZE_F size;
	GraphicsContainer * container;
	D2DRendererBase * renderer;
		
	Impl (void) : 
		container(NULL),
		renderer(NULL)
	{
		position.x = position.y = 0.0f;
		size.width = size.height = 100.0f;
	}

	virtual ~Impl() { }
};

GraphicsBase::GraphicsBase(void): pImpl(new Impl)
{
}


GraphicsBase::~GraphicsBase(void)
{
	delete pImpl;
}

D2D1_POINT_2F F35_NS::GraphicsBase::GetPosition( void )
{
	return pImpl->position;
}

void F35_NS::GraphicsBase::SetPosition( D2D1_POINT_2F pt )
{
	pImpl->position = pt;
}

void F35_NS::GraphicsBase::SetPosition( FLOAT x, FLOAT y )
{
	pImpl->position = D2D1::Point2F(x, y);
}

BOOL F35_NS::GraphicsBase::AddContainer( GraphicsContainer *parent )
{
	if (pImpl->container != NULL) return FALSE;
	pImpl->container = parent;
}

void F35_NS::GraphicsBase::RemoveContainer( GraphicsContainer *parent )
{
	if (pImpl->container == parent ) pImpl->container = NULL;
}

GraphicsContainer * F35_NS::GraphicsBase::GetParent( void ) const
{
	return pImpl->container;
}

BOOL F35_NS::GraphicsBase::AttachRenderer( D2DRendererBase * renderer )
{
	if (pImpl->renderer != NULL) return FALSE;

	pImpl->renderer = renderer;
	if (renderer != NULL)
		this->InitGraphic(renderer);
	return TRUE;
}

void F35_NS::GraphicsBase::DettachRenderer( void )
{
	if (pImpl->renderer != NULL)
		this->DestroyGraphic(pImpl->renderer);
	pImpl->renderer = NULL;
}

D2D1_SIZE_F F35_NS::GraphicsBase::GetSize( void )
{
	return pImpl->size;
}

void F35_NS::GraphicsBase::SetSize( D2D1_SIZE_F sz )
{
	pImpl->size = sz;
}

void F35_NS::GraphicsBase::SetSize( FLOAT width, FLOAT height )
{
	pImpl->size = D2D1::SizeF(width, height);
}

D2DRendererBase * F35_NS::GraphicsBase::GetRenderer( void ) const
{
	return pImpl->renderer;
}


