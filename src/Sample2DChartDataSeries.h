#pragma once
#include "F35Lib.h"
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

	virtual ChartDataPoint GetDataPoint( UINT index ) const;

	virtual ChartDataPoint GetMinDataValues( void ) const;

	virtual ChartDataPoint GetMaxDataValues( void ) const;

};

}