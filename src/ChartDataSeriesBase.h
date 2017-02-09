#pragma once
#include "F35Helper.h"

#include <d2d1.h>

namespace F35_NS
{
	class ChartDataSeriesBase: H::NonCopyable
	{
	public:
		ChartDataSeriesBase(void) { }
		virtual ~ChartDataSeriesBase(void) { }

		virtual UINT GetCount(void) const = 0;

		virtual D2D_VECTOR_4F GetDataPoint(UINT index) const = 0;

		virtual D2D_VECTOR_4F GetMinDataValues(void) const = 0;

		virtual D2D_VECTOR_4F GetMaxDataValues(void) const = 0;
	};
}
