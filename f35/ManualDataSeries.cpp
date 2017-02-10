#include "stdafx.h"
#include "ManualDataSeries.h"

#include <vector>

USING_F35_NS;

struct ManualDataSeries::Impl
{
	std::vector<D2D_VECTOR_4F> points;

	D2D_VECTOR_4F pt_min, pt_max;

	bool needs_update;

	void update(void)
	{
		if (needs_update)
		{
			UINT len = points.size();

			if (len > 0)
			{
				pt_min = pt_max = points[0];
				for (UINT idx = 1; idx < len; idx++)
				{
					D2D_VECTOR_4F pt = points[idx];

					if (pt_min.x > pt.x) pt_min.x = pt.x;
					if (pt_min.y > pt.y) pt_min.y = pt.y;
					if (pt_min.z > pt.z) pt_min.z = pt.z;
					if (pt_min.w > pt.w) pt_min.w = pt.w;

					if (pt_max.x < pt.x) pt_max.x = pt.x;
					if (pt_max.y < pt.y) pt_max.y = pt.y;
					if (pt_max.z < pt.z) pt_max.z = pt.z;
					if (pt_max.w < pt.w) pt_max.w = pt.w;
				}
			}

			needs_update = false;
		}
	}
};

ManualDataSeries::ManualDataSeries(void):
	pImpl(new Impl)
{
	pImpl->needs_update = true;
}


ManualDataSeries::~ManualDataSeries(void)
{
	delete pImpl;
	pImpl = NULL;
}

UINT F35_NS::ManualDataSeries::GetCount( void ) const
{
	return pImpl->points.size();
}

D2D_VECTOR_4F F35_NS::ManualDataSeries::GetDataPoint( UINT index ) const
{
	return pImpl->points[index];
}

void F35_NS::ManualDataSeries::SetCount( UINT count )
{
	pImpl->points.resize(count);
	pImpl->needs_update = true;
}

void F35_NS::ManualDataSeries::SetDataPoint( UINT index, D2D_VECTOR_4F point )
{
	pImpl->points[index] = point;
	pImpl->needs_update = true;
}

void F35_NS::ManualDataSeries::SetDataPoint( UINT index, FLOAT x, FLOAT y, FLOAT z /*= 0.0f*/, FLOAT w /*= 0.0f*/ )
{
	D2D_VECTOR_4F pt;
	pt.x = x, pt.y = y, pt.z = z, pt.w = w;
	pImpl->points[index] = pt;
	pImpl->needs_update = true;
}

D2D_VECTOR_4F F35_NS::ManualDataSeries::GetMinDataValues( void ) const
{
	pImpl->update();
	return pImpl->pt_min;
}

D2D_VECTOR_4F F35_NS::ManualDataSeries::GetMaxDataValues( void ) const
{
	pImpl->update();
	return pImpl->pt_max;
}
