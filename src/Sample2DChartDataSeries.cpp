#include "stdafx.h"
#include "Sample2DChartDataSeries.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

USING_F35_NS;

struct Sample2DChartDataSeries::Impl
{
	static const UINT WIDTH = 70;
	static const UINT HEIGHT = 40;
	
	static FLOAT rand_f(FLOAT min, FLOAT max)
	{
		return min + (max - min) * rand() / RAND_MAX;
	}

	static ChartDataPoint rand_pt (FLOAT min, FLOAT max)
	{
		ChartDataPoint pt;
		pt.x = rand_f(min, max);
		pt.y = rand_f(min, max);
		pt.z = rand_f(min, max);
		pt.w = rand_f(min, max);
		return pt;
	}

	ChartDataPoint rad_freq;
	ChartDataPoint phase;
};

Sample2DChartDataSeries::Sample2DChartDataSeries(void):
	pIml(new Impl)
{
	pIml->rad_freq = Impl::rand_pt(0.001f, 0.01f);
	pIml->phase = Impl::rand_pt(0.0f, 2.0f * M_PI);
}


Sample2DChartDataSeries::~Sample2DChartDataSeries(void)
{
	delete pIml;
	pIml = NULL;
}

UINT F35_NS::Sample2DChartDataSeries::GetCount( void ) const
{
	return Impl::WIDTH*Impl::HEIGHT;
}

F35_NS::ChartDataPoint F35_NS::Sample2DChartDataSeries::GetDataPoint( UINT index ) const
{
	ChartDataPoint pt;
	pt.x = (index % Impl::WIDTH) + sinf(pIml->phase.x);
	pt.y = (index / Impl::WIDTH) + sinf(pIml->phase.y);
	FLOAT rad = 2.0f * M_PI * pt.x * pt.y / (FLOAT) (Impl::WIDTH*Impl::HEIGHT);
	pt.z = sinf(pIml->phase.z + rad);
	pt.w = sinf(pIml->phase.w + rad);
	return pt;
}

void F35_NS::Sample2DChartDataSeries::Update( void )
{
	pIml->phase.x += 2.0f * M_PI * (pIml->phase.x < M_PI? pIml->rad_freq.x: (pIml->rad_freq.x - 1.0f));
	pIml->phase.y += 2.0f * M_PI * (pIml->phase.y < M_PI? pIml->rad_freq.y: (pIml->rad_freq.y - 1.0f));
	pIml->phase.z += 2.0f * M_PI * (pIml->phase.z < M_PI? pIml->rad_freq.z: (pIml->rad_freq.z - 1.0f));
	pIml->phase.w += 2.0f * M_PI * (pIml->phase.w < M_PI? pIml->rad_freq.w: (pIml->rad_freq.w - 1.0f));

}

F35_NS::ChartDataPoint F35_NS::Sample2DChartDataSeries::GetMinDataValues( void ) const
{
	ChartDataPoint pt;
	pt.x = 0;
	pt.y = 0;
	pt.z = -1.0f;
	pt.w = -1.0f;
	return pt;
}

F35_NS::ChartDataPoint F35_NS::Sample2DChartDataSeries::GetMaxDataValues( void ) const
{
	ChartDataPoint pt;
	pt.x = Impl::WIDTH-1;
	pt.y = Impl::HEIGHT-1;
	pt.z = +1.0f;
	pt.w = +1.0f;
	return pt;
}
