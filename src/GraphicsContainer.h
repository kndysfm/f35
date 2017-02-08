#pragma once

#include "GraphicsBase.h"

namespace F35_NS
{
struct IGraphicsContainer
{
	virtual BOOL AddGraphic( GraphicsBase *graphic, LONG insert_index = LONG_MAX ) = 0;

	virtual LONG GetIndexOfGraphic( GraphicsBase *graphic ) = 0;

	virtual BOOL RemoveGraphic( GraphicsBase *graphic ) = 0;
};

class GraphicsContainer :public GraphicsBase, public IGraphicsContainer
{
private:
	struct Impl;
	Impl *pImpl;

public:
	GraphicsContainer(void);
	virtual ~GraphicsContainer(void);

	virtual BOOL AddGraphic( GraphicsBase *graphic, LONG insert_index = LONG_MAX );

	virtual LONG GetIndexOfGraphic( GraphicsBase *graphic );

	virtual BOOL RemoveGraphic( GraphicsBase *graphic );

	virtual void InitGraphic( D2DRendererBase * renderer );

	virtual void UpdateGraphic( D2DRendererBase * renderer );

	virtual BOOL RenderGraphic( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs );

	virtual void DestroyGraphic( D2DRendererBase * renderer );

	virtual BOOL AttachRenderer( D2DRendererBase * renderer );

	virtual void DettachRenderer( void );

	};

}
