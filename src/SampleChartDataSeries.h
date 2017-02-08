#pragma once
#include "F35Lib.h"
#include "chartdataseriesbase.h"

namespace F35_NS {

class SampleChartDataSeries :
	public ChartDataSeriesBase
{
	struct Impl;
	Impl *pImpl;
public:
	SampleChartDataSeries(void);
	virtual ~SampleChartDataSeries(void);

	void SetLength( UINT length );

	void Update(void);

	virtual UINT GetCount( void ) const;

	virtual ChartDataPoint GetDataPoint( UINT index ) const;

	virtual ChartDataPoint GetMinDataValues( void ) const;

	virtual ChartDataPoint GetMaxDataValues( void ) const;

};

}