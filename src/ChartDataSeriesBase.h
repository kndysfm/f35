#pragma once
#include "F35Helper.h"

#include <d2d1.h>

namespace F35_NS
{
	struct ChartDataPoint : D2D_VECTOR_4F
	{
		D2D_VECTOR_4F raw;
	};

	class ChartDataSeriesBase: H::NonCopyable
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
