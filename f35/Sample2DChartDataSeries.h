#pragma once
#include "F35Helper.h"
#include "chartdataseriesbase.h"

namespace F35_NS {

class Sample2DChartDataSeries :
	public ChartDataSeriesBase
{
	struct Impl;
	Impl *pIml;

public:
	Sample2DChartDataSeries(void);
	virtual ~Sample2DChartDataSeries(void);

	void Update(void);

	virtual UINT GetCount( void ) const;

	virtual D2D_VECTOR_4F GetDataPoint( UINT index ) const;

	virtual D2D_VECTOR_4F GetMinDataValues( void ) const;

	virtual D2D_VECTOR_4F GetMaxDataValues( void ) const;

};

}