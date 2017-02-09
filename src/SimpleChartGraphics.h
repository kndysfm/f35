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
	virtual void InternalInit( D2DRendererBase * );

	virtual void InternalUpdate( D2DRendererBase * );

	virtual BOOL InternalRender( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs );

	virtual void InternalDestroy( D2DRendererBase * );

};

}