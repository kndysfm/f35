#pragma once
#include "chartgraphicsbase.h"


namespace F35_NS
{

class SimpleChartGraphics :
	public ChartGraphicsBase
{
	struct Impl;
	Impl * pImpl;

public:
	SimpleChartGraphics(void);
	virtual ~SimpleChartGraphics(void);

	void ShowMessage(LPCTSTR mess);

protected:
	virtual void InternalInit( RendererBase * );

	virtual void InternalUpdate( RendererBase * );

	virtual BOOL InternalRender( RendererBase * renderer, ID2D1RenderTarget * target);

	virtual void InternalDestroy( RendererBase * );

};

}