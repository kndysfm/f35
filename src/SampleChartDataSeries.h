#pragma once
#include "F35Helper.h"
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

	virtual D2D_VECTOR_4F GetDataPoint( UINT index ) const;

	virtual D2D_VECTOR_4F GetMinDataValues( void ) const;

	virtual D2D_VECTOR_4F GetMaxDataValues( void ) const;

};

}