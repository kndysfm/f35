#pragma once
#include "F35Helper.h"

#include "RendererBase.h"

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

	D2D1_SIZE_F GetScale (void);

	void SetScale(FLOAT scale);
	void SetScale (D2D1_SIZE_F sz);
	void SetScale (FLOAT x, FLOAT y);

	FLOAT GetRotation(void);
	void SetRotation(FLOAT degrees);

	GraphicsContainer *GetParent (void) const;

	RendererBase *GetRenderer (void) const;

	BOOL AddContainer(GraphicsContainer *parent);

	void RemoveContainer(GraphicsContainer *parent);

	virtual BOOL AttachRenderer(RendererBase * renderer);

	virtual void DettachRenderer(void);

	void Init(RendererBase * renderer);

	void Update(RendererBase * renderer);

	BOOL Render(RendererBase * renderer, ID2D1RenderTarget * target);

	void Destroy(RendererBase * renderer);

protected:

	virtual void InternalInit( RendererBase * renderer ) { }

	virtual void InternalUpdate( RendererBase * renderer) { };

	virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target) = 0;

	virtual void InternalDestroy( RendererBase * renderer ) { }

};

}