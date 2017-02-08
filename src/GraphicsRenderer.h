#pragma once
#include "d2drendererbase.h"
#include "GraphicsContainer.h"

namespace F35_NS
{
	
	class GraphicsRenderer : 
		public D2DRendererBase, 
		public IGraphicsContainer
{
private:
	struct Impl;
	Impl *pImpl;

public:
	GraphicsRenderer(HWND hwnd);
	virtual ~GraphicsRenderer(void);

	virtual BOOL AddGraphic( GraphicsBase *graphic, LONG insert_index = LONG_MAX );

	virtual LONG GetIndexOfGraphic( GraphicsBase *graphic );

	virtual BOOL RemoveGraphic( GraphicsBase *graphic );

protected:

	virtual HRESULT InternalInit( ID2D1RenderTarget *target );

	virtual void InternalUpdate( void );

	virtual HRESULT InternalRender( ID2D1RenderTarget *target );

	virtual void InternalDestroy( void );

	};

}