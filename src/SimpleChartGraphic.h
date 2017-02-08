#pragma once
#include "chartgraphicbase.h"


namespace F35_NS
{

class SimpleChartGraphic :
	public ChartGraphicBase
{
	struct Impl;
	Impl * pImpl;

public:
	SimpleChartGraphic(void);
	virtual ~SimpleChartGraphic(void);

	void ShowMessage(LPCTSTR mess);

	virtual void InitGraphic( D2DRendererBase * );

	virtual void UpdateGraphic( D2DRendererBase * );

	virtual BOOL RenderGraphic( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs );

	virtual void DestroyGraphic( D2DRendererBase * );

};

}