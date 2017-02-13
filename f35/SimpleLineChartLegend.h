#pragma once
#include "F35Helper.h"
#include "chartlegendbase.h"

namespace F35_NS {

class SimpleLineChartLegend :
	public ChartLegendBase
{
	struct Impl;
	Impl *pImpl;

public:
	SimpleLineChartLegend(void);
	virtual ~SimpleLineChartLegend(void);

	void ShowMarker(BOOL show);

protected:
	virtual void BeginDraw() const;

	virtual void Draw(D2D_VECTOR_4F const *value, D2D_VECTOR_4F const *point, D2D_VECTOR_4F const *point_previous = NULL, D2D_VECTOR_4F const *point_next = NULL) const;

	virtual void Print(D2D_VECTOR_4F const *point, LPCTSTR str_fmt, ...) const;

	virtual void EndDraw() const;

};

}