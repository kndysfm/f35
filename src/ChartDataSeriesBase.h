#pragma once
#include "F35Lib.h"

#include <d2d1.h>

namespace F35_NS
{

class ChartDataSeriesBase: NonCopyable
{
public:
	ChartDataSeriesBase(void) { }
	virtual ~ChartDataSeriesBase(void) { }

	virtual UINT GetCount(void) const = 0;

	virtual ChartDataPoint GetDataPoint(UINT index) const = 0;

	virtual ChartDataPoint GetMinDataValues(void) const = 0;

	virtual ChartDataPoint GetMaxDataValues(void) const = 0;
};
}
