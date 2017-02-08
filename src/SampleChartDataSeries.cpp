#include "stdafx.h"
#include "SampleChartDataSeries.h"

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>



USING_F35_NS;

struct SampleChartDataSeries::Impl
{
	typedef std::vector<ChartDataPoint> ChartDataPointVector;

	ChartDataPointVector data_points;

	ChartDataPoint frequencies;
	ChartDataPoint phases;

	ChartDataPoint pt_min, pt_max;

	FLOAT rand_f(FLOAT min, FLOAT max)
	{
		return min + (max - min) * rand() / RAND_MAX;
	}

	ChartDataPoint rand_pt (FLOAT min, FLOAT max)
	{
		ChartDataPoint pt;
		pt.x = rand_f(min, max);
		pt.y = rand_f(min, max);
		pt.z = rand_f(min, max);
		pt.w = rand_f(min, max);
		return pt;
	}

	void Init(UINT len)
	{
		data_points.resize(len);
		frequencies = rand_pt(0.01f, 0.1f);
		phases = rand_pt(0, 2.0f*M_PI);

		Update();
	}

	void Update(void)
	{
		phases.x += 2.0f * M_PI * frequencies.x;
		phases.y += 2.0f * M_PI * frequencies.y;
		phases.z += 2.0f * M_PI * frequencies.z;
		phases.w += 2.0f * M_PI * frequencies.w;

		UINT len = data_points.size();
		for (UINT idx = 0; idx < len; idx++)
		{
			FLOAT rad = 2.0f * M_PI * idx / len;
			ChartDataPoint &pt = data_points[idx];

			pt.x = 35.0f * (sinf(rad + phases.x) + 1.0f);
			pt.y = 20.0f * (sinf(rad + phases.y) + 1.0f);
			pt.z = sinf(rad + phases.z);
			pt.w = sinf(rad + phases.w);

			if (idx == 0)
			{
				pt_min = pt_max = pt;
			}
			else
			{
				if (pt_min.x > pt.x) pt_min.x = pt.x;
				if (pt_min.y > pt.y) pt_min.y = pt.y;

				if (pt_max.x < pt.x) pt_max.x = pt.x;
				if (pt_max.y < pt.y) pt_max.y = pt.y;
			}
		}
		pt_min.z = pt_min.w = -1.0f;
		pt_max.z = pt_max.w = +1.0f;
	}
};

SampleChartDataSeries::SampleChartDataSeries(void):
	pImpl(new Impl)
{
	pImpl->Init(100);
}

SampleChartDataSeries::~SampleChartDataSeries(void)
{
}

UINT F35_NS::SampleChartDataSeries::GetCount( void ) const
{
	return pImpl->data_points.size();
}

F35_NS::ChartDataPoint F35_NS::SampleChartDataSeries::GetDataPoint( UINT index ) const
{
	return pImpl->data_points[index];
}

void F35_NS::SampleChartDataSeries::SetLength( UINT length )
{
	pImpl->Init(length);
}

void F35_NS::SampleChartDataSeries::Update( void )
{
	pImpl->Update();
}

F35_NS::ChartDataPoint F35_NS::SampleChartDataSeries::GetMinDataValues( void ) const
{
	throw std::exception("The method or operation is not implemented.");
}

F35_NS::ChartDataPoint F35_NS::SampleChartDataSeries::GetMaxDataValues( void ) const
{
	throw std::exception("The method or operation is not implemented.");
}
