#include "stdafx.h"
#include "GraphicsBase.h"
#include "GraphicsContainer.h"

USING_F35_NS;

struct GraphicsBase::Impl
{
	D2D1_POINT_2F position;
	D2D1_SIZE_F scale;
	FLOAT rotation;
	GraphicsContainer * container;
	D2DRendererBase * renderer;
		
	Impl (void) : 
		container(NULL),
		renderer(NULL)
	{
		position = D2D1::Point2F();
		scale = D2D1::SizeF(1.0f, 1.0f);
		rotation = 0.0f;
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
	return TRUE;
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
		this->InternalInit(renderer);
	return TRUE;
}

void F35_NS::GraphicsBase::DettachRenderer( void )
{
	if (pImpl->renderer != NULL)
		this->InternalDestroy(pImpl->renderer);
	pImpl->renderer = NULL;
}

D2D1_SIZE_F F35_NS::GraphicsBase::GetScale( void )
{
	return pImpl->scale;
}

void F35_NS::GraphicsBase::SetScale(FLOAT scale)
{
	pImpl->scale = D2D1::SizeF(scale, scale);
}

void F35_NS::GraphicsBase::SetScale( D2D1_SIZE_F sz )
{
	pImpl->scale = sz;
}

void F35_NS::GraphicsBase::SetScale( FLOAT width, FLOAT height )
{
	pImpl->scale = D2D1::SizeF(width, height);
}

FLOAT F35_NS::GraphicsBase::GetRotation(void)
{
	return pImpl->rotation;
}

void F35_NS::GraphicsBase::SetRotation(FLOAT degrees)
{
	pImpl->rotation = degrees;
}

D2DRendererBase * F35_NS::GraphicsBase::GetRenderer( void ) const
{
	return pImpl->renderer;
}


void F35_NS::GraphicsBase::Init(D2DRendererBase * renderer)
{
	InternalInit(renderer);
}

void F35_NS::GraphicsBase::Update(D2DRendererBase * renderer)
{
	InternalUpdate(renderer);
}

BOOL F35_NS::GraphicsBase::Render(D2DRendererBase * renderer, ID2D1RenderTarget * target)
{
	D2D1::Matrix3x2F mat_last;
	target->GetTransform(&mat_last);
	D2D1::Matrix3x2F mat = mat_last;
	mat = mat * D2D1::Matrix3x2F::Scale(pImpl->scale);
	mat = mat * D2D1::Matrix3x2F::Rotation(pImpl->rotation);
	mat = mat * D2D1::Matrix3x2F::Translation(D2D1::SizeF(pImpl->position.x, pImpl->position.y));
	target->SetTransform(mat);
	BOOL ret = InternalRender(renderer, target);
	target->SetTransform(mat_last);

	return ret;
}

void F35_NS::GraphicsBase::Destroy(D2DRendererBase * renderer)
{
	InternalDestroy(renderer);
}

