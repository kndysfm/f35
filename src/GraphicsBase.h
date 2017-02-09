#pragma once
#include "F35Helper.h"

#include "D2DRendererBase.h"

namespace F35_NS
{

class GraphicsContainer;

class GraphicsBase : H::NonCopyable
{
private:
	struct Impl;
	Impl *pImpl;

public:
	GraphicsBase(void);
	virtual ~GraphicsBase(void);

	D2D1_POINT_2F GetPosition (void);

	void SetPosition (D2D1_POINT_2F pt);
	void SetPosition (FLOAT x, FLOAT y);

	D2D1_SIZE_F GetSize (void);

	void SetSize (D2D1_SIZE_F sz);
	void SetSize (FLOAT width, FLOAT height);
	
	GraphicsContainer *GetParent (void) const;

	D2DRendererBase *GetRenderer (void) const;

	BOOL AddContainer(GraphicsContainer *parent);

	void RemoveContainer(GraphicsContainer *parent);

	virtual BOOL AttachRenderer(D2DRendererBase * renderer);

	virtual void DettachRenderer(void);

	void Init(D2DRendererBase * renderer);

	void Update(D2DRendererBase * renderer);

	BOOL Render(D2DRendererBase * renderer, ID2D1RenderTarget * target);

	void Destroy(D2DRendererBase * renderer);

protected:

	virtual void InternalInit( D2DRendererBase * renderer ) { }

	virtual void InternalUpdate( D2DRendererBase * renderer) { };

	virtual BOOL InternalRender(D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs) = 0;

	virtual void InternalDestroy( D2DRendererBase * renderer ) { }

};

}