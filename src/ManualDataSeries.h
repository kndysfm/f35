#pragma once
#include "F35Helper.h"
#include "chartdataseriesbase.h"

namespace F35_NS {

class ManualDataSeries :
	public ChartDataSeriesBase
{
	struct Impl;
	Impl *pImpl;

public:
	ManualDataSeries(void);
	virtual ~ManualDataSeries(void);

	void SetCount( UINT count );

	void SetDataPoint(UINT index, D2D_VECTOR_4F point);

	void SetDataPoint(UINT index, FLOAT x, FLOAT y, FLOAT z = 0.0f, FLOAT w = 0.0f);

	virtual UINT GetCount( void ) const;

	virtual D2D_VECTOR_4F GetDataPoint( UINT index ) const;

	template<typename S, typename T, typename U, typename V>
	void ApplyArray( UINT count, const S *x_array, const T *y_array,
		const U *z_array, const V *w_array)
	{
		SetCount(count);
		for (UINT idx = 0; idx < count; idx++)
		{
			SetDataPoint(idx, 
				x_array? static_cast<FLOAT>(x_array[idx]): 0.0f, 
				y_array? static_cast<FLOAT>(y_array[idx]): 0.0f,
				z_array? static_cast<FLOAT>(z_array[idx]): 0.0f,
				w_array? static_cast<FLOAT>(w_array[idx]): 0.0f);
		}
	}

	template<typename S, typename T, typename U>
	void ApplyArray( UINT count, const S *x_array, const T *y_array, const U *z_array)
	{
		SetCount(count);
		for (UINT idx = 0; idx < count; idx++)
		{
			SetDataPoint(idx, 
				x_array? static_cast<FLOAT>(x_array[idx]): 0.0f, 
				y_array? static_cast<FLOAT>(y_array[idx]): 0.0f,
				z_array? static_cast<FLOAT>(z_array[idx]): 0.0f,
				0.0f);
		}
	}

	template<typename S, typename T>
	void ApplyArray( UINT count, const S *x_array, const T *y_array)
	{
		SetCount(count);
		for (UINT idx = 0; idx < count; idx++)
		{
			SetDataPoint(idx, 
				x_array? static_cast<FLOAT>(x_array[idx]): 0.0f, 
				y_array? static_cast<FLOAT>(y_array[idx]): 0.0f,
				0.0f,
				0.0f);
		}
	}

	virtual D2D_VECTOR_4F GetMinDataValues( void ) const;

	virtual D2D_VECTOR_4F GetMaxDataValues( void ) const;

};

}