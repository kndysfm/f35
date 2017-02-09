#include "stdafx.h"
#include "ChartGraphicsBase.h"

#include <string>
#include <unordered_map>

USING_F35_NS;

struct ChartAxisRange
{
	FLOAT min, max, range;
};

struct ChartAxisUnit
{
	FLOAT major, minor;
};

struct ChartGraphicsBase::Impl
{
	typedef std::basic_string<_TCHAR> Tstring;
	typedef std::unordered_map<Tstring, ChartDataSeriesBase const *> ChartDataSeriesMap;
	typedef std::unordered_map<Tstring, ChartLegendBase const *> ChartLegendMap;

	ChartDataSeriesMap data_series_map;
	ChartLegendMap legend_map;

	ChartGraphicsBase *parent;

	ChartAxisRange axis_x, axis_y, axis_z, axis_w;
	ChartAxisUnit unit_x, unit_y, unit_z, unit_w;

	Tstring number_format;

	D2D1::ColorF color_bg, color_fg;

	BOOL inv_x, inv_y, inv_z, inv_w;

	FLOAT axis_pos_x, axis_pos_y;

	D2D_VECTOR_4F pt_origin;

	D2D1_SIZE_F area_size;

	Impl(ChartGraphicsBase *p): 
		parent(p), 
		number_format(_T("%.1f")),
		color_bg(D2D1::ColorF(D2D1::ColorF::White, 1.0f)),
		color_fg(D2D1::ColorF(D2D1::ColorF::Black, 1.0f))
	{
		// initialize four axes
		axis_x.min =	axis_y.min =	axis_z.min =	axis_w.min =	0.0f;
		axis_x.max =	axis_y.max =	axis_z.max =	axis_w.max =	1.0f;
		axis_x.range =	axis_y.range =	axis_z.range =	axis_w.range =	1.0f;

		unit_x.major =	unit_y.major =	unit_z.major =	unit_w.major =	1.0f;
		unit_x.minor =	unit_y.minor =	unit_z.minor =	unit_w.minor =	0.0f;
		inv_x		 =	inv_z		 =	inv_w		 =	FALSE;
		inv_y = TRUE;

		axis_pos_x = 0.0f;
		axis_pos_y = 0.0f;

		pt_origin.x = pt_origin.y = pt_origin.z = pt_origin.w = 0.0f;

		area_size = D2D1::SizeF();
	}

	~Impl(void)
	{
		parent = NULL;
	}

	BOOL add_data_series( LPCTSTR name, ChartDataSeriesBase const *series, ChartLegendBase const *legend )
	{
		if (data_series_map.count(name) > 0) return FALSE;

		data_series_map[name] = series;
		legend_map[name] = legend;
		return TRUE;
	}

	BOOL remove_data_series( LPCTSTR name)
	{
		BOOL bRet = TRUE;

		if (data_series_map.count(name) > 0)
			data_series_map.erase(name);
		else
			bRet = FALSE;

		if (legend_map.count(name) > 0)
			legend_map.erase(name);
		else
			bRet = FALSE;

		return bRet;
	}

	static FLOAT val_to_ratio_in_range(FLOAT val, const ChartAxisRange &axis, BOOL inv)
	{
		return inv? 1.0f - (val - axis.min) / axis.range: (val - axis.min) / axis.range;
	}

	static D2D1_POINT_2F pt_to_pt_in_rect(D2D1_POINT_2F pt, const D2D1_RECT_F &rect,
		const ChartAxisRange &axis_x, BOOL inv_x,
		const ChartAxisRange &axis_y, BOOL inv_y)
	{
		D2D1_POINT_2F pt_ret;

		pt_ret.x = rect.left + (rect.right - rect.left) * val_to_ratio_in_range(pt.x, axis_x, inv_x);
		pt_ret.y = rect.top + (rect.bottom - rect.top) * val_to_ratio_in_range(pt.y, axis_y, inv_y);

		return pt_ret;
	}

	ChartDataPoint convert_data_point(ChartDataPoint pt)
	{
		ChartDataPoint pt_ret;

		pt_ret.x = val_to_ratio_in_range(pt.x, axis_x, inv_x);
		pt_ret.y = val_to_ratio_in_range(pt.y, axis_y, inv_y);
		pt_ret.z = val_to_ratio_in_range(pt.z, axis_z, inv_z);
		pt_ret.w = val_to_ratio_in_range(pt.w, axis_w, inv_w);
		
		pt_ret.raw.x = pt.x;
		pt_ret.raw.y = pt.y;
		pt_ret.raw.z = pt.z;
		pt_ret.raw.w = pt.w;

		return pt_ret;
	}

	void plot_chart_data( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect )
	{
		for (ChartLegendMap::iterator itr = legend_map.begin();
			itr != legend_map.end(); itr++)
		{
			Tstring name = itr->first;
			if (data_series_map.count(name) == 1)
			{
				ChartLegendBase const * l = itr->second;
				ChartDataSeriesBase const * ds = data_series_map[name];

				//UINT cnt = ds->GetCount();
				if (ds->GetCount() > 0)
				{
					ChartDataPoint pt_prev, pt_curr, pt_next;
					l->BeginDraw(renderer, target);
					pt_curr = convert_data_point(ds->GetDataPoint(0));
					if (ds->GetCount() == 1)
					{
						l->Draw(renderer, target, rect, &pt_curr, NULL, NULL);
					}
					else
					{
						pt_next = convert_data_point(ds->GetDataPoint(1));
						l->Draw(renderer, target, rect, &pt_curr, NULL, &pt_next);
						for (UINT idx = 2; idx < ds->GetCount(); idx++)
						{
							pt_prev = pt_curr;
							pt_curr = pt_next;
							pt_next = convert_data_point(ds->GetDataPoint(idx));
							l->Draw(renderer, target, rect, &pt_curr, &pt_prev, &pt_next);
						}
						pt_prev = pt_curr;
						pt_curr = pt_next;
						l->Draw(renderer, target, rect, &pt_curr, &pt_prev, NULL);
					}
					l->EndDraw(renderer, target);
				}
			}
		}

	}

	void plot_legends( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect )
	{
		FLOAT w_rect = rect->right - rect->left;
		FLOAT h_rect = rect->bottom - rect->top;
		UINT num = legend_map.size();

		FLOAT h_line = 0.3f / (num + 1.0f);
		FLOAT y_pos = h_line * 0.5f;

		for (ChartLegendMap::iterator itr = legend_map.begin();
			itr != legend_map.end(); itr++)
		{
			Tstring name = itr->first;
			if (data_series_map.count(name) == 1)
			{
				ChartLegendBase const * l = itr->second;
				ChartDataSeriesBase const * ds = data_series_map[name];

				ChartDataPoint cdp_max = ds->GetMaxDataValues();

				UINT cnt = ds->GetCount();
				if (cnt > 0)
				{
					ChartDataPoint pt_prev, pt_curr, pt_next;
					l->BeginDraw(renderer, target);
					pt_curr.x = 0.06f; pt_next.x = 0.14f;
					pt_curr.y = pt_next.y = y_pos;
					l->Draw(renderer, target, rect, &pt_curr, NULL, &pt_next);
					l->Draw(renderer, target, rect, &pt_curr, &pt_next, NULL);

					pt_curr.x = 0.15f;
					l->Print(renderer, target, rect, &pt_curr, name.c_str());

					l->EndDraw(renderer, target);
				}
			}
			y_pos += h_line;
		}

	}

	void draw_line_h( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect, FLOAT y_value,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		D2D1_POINT_2F pt0, pt1;
		pt0 = pt_to_pt_in_rect(D2D1::Point2F(axis_x.min, y_value), *rect, axis_x, inv_x, axis_y, inv_y);
		pt1 = pt_to_pt_in_rect(D2D1::Point2F(axis_x.max, y_value), *rect, axis_x, inv_x, axis_y, inv_y);
		target->DrawLine(pt0, pt1, brush, line_width, style);
	}

	void draw_major_grid_y( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		if (unit_y.major == 0.0f || 
			rect->bottom <= rect->top ||
			unit_y.major / axis_y.range < 1.0e-3f) return;

		FLOAT val0 = pt_origin.y + unit_y.major * ceilf((axis_y.min - pt_origin.y) / unit_y.major);
		for (FLOAT val = 0.0f; val < axis_y.range; val += unit_y.major)
		{
			draw_line_h(renderer, target, rect, val0 + val, brush, line_width, style);
		}
	}

	void draw_major_label_y( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect, 
		FLOAT margin, FLOAT width,
		IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format = _T("%.0f"))
	{
		if (unit_y.major == 0.0f || 
			rect->bottom <= rect->top ||
			unit_y.major / axis_y.range < 1.0e-3f) return;

		FLOAT label_pos_x = 
			rect->left + (rect->right - rect->left) * (inv_x? 1.0f - axis_pos_x: axis_pos_x);
		D2D1_RECT_F rect_label;
		if (margin <= 0)
		{
			rect_label.right = label_pos_x + margin;
			rect_label.left = rect_label.right - width;
		} else {
			rect_label.left = label_pos_x + margin;
			rect_label.right = rect_label.left + width;
		}

		FLOAT y_val = pt_origin.y + unit_y.major * ceilf((axis_y.min - pt_origin.y) / unit_y.major);
		FLOAT h = rect->bottom - rect->top;
		FLOAT dy = h * unit_y.major / axis_y.range;
		for (; y_val < axis_y.max; y_val += unit_y.major)
		{
			rect_label.top = rect->top + h * val_to_ratio_in_range(y_val, axis_y, inv_y);
			rect_label.bottom = rect_label.top + dy;
			H::WriteText(target, text_format, rect_label, brush, format, y_val);
		}

	}

	void draw_minor_grid_y( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		if (unit_y.major == 0.0f || 
			unit_y.minor == 0.0f || 
			rect->bottom <= rect->top ||
			unit_y.minor / axis_y.range < 0.5e-3f) return;

		FLOAT val0 = pt_origin.y + unit_y.minor * ceilf((axis_y.min - pt_origin.y) / unit_y.minor);
		for (FLOAT val = 0.0f; val < axis_y.range; val += unit_y.minor)
		{
			draw_line_h(renderer, target, rect, val0 + val, brush, line_width, style);
		}
	}

	void draw_line_v( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect, FLOAT x_value,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		D2D1_POINT_2F pt0, pt1;
		pt0 = pt_to_pt_in_rect(D2D1::Point2F(x_value, axis_y.min), *rect, axis_x, inv_x, axis_y, inv_y);
		pt1 = pt_to_pt_in_rect(D2D1::Point2F(x_value, axis_y.max), *rect, axis_x, inv_x, axis_y, inv_y);
		target->DrawLine(pt0, pt1, brush, line_width, style);
	}

	void draw_major_grid_x( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		if (unit_x.major == 0.0f || rect->right <= rect->left ||
			unit_x.major / axis_x.range < 1.0e-3f) return;

		FLOAT val0 = pt_origin.x + unit_x.major * ceilf((axis_x.min - pt_origin.x) / unit_x.major);
		for (FLOAT val = 0.0f; val < axis_x.range; val += unit_x.major)
		{
			draw_line_v(renderer, target, rect, val0 + val, brush, line_width, style);
		}
	}

	void draw_major_label_x( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect, 
		FLOAT margin, FLOAT height,
		IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format = _T("%.0f"))
	{
		if (unit_x.major == 0.0f || rect->right <= rect->left ||
			unit_x.major / axis_x.range < 1.0e-3f) return;

		FLOAT label_pos_y = 
			rect->top + (rect->bottom - rect->top) * (inv_y? 1.0f - axis_pos_y: axis_pos_y);
		D2D1_RECT_F rect_label;
		if (margin < 0)
		{
			rect_label.bottom = label_pos_y + margin;
			rect_label.top = rect_label.bottom - height;
		}
		else
		{
			rect_label.top = label_pos_y + margin;
			rect_label.bottom = rect_label.top + height;
		}

		FLOAT x_val = pt_origin.x + unit_x.major * ceilf((axis_x.min - pt_origin.x) / unit_x.major);
		FLOAT w = rect->right - rect->left;
		FLOAT dx = w * unit_x.major / axis_x.range;
		for (; x_val < axis_x.max; x_val += unit_x.major)
		{
			rect_label.left = rect->left + w * val_to_ratio_in_range(x_val, axis_x, inv_x);
			rect_label.right = rect_label.left + dx;
			H::WriteText(target, text_format, rect_label, brush, format, x_val);
		}
	}

	void draw_minor_grid_x( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL)
	{
		if (unit_x.major == 0.0f || unit_x.minor == 0.0f || rect->right <= rect->left ||
			unit_x.minor / axis_x.range < 0.5e-3f) return;

		FLOAT val0 = pt_origin.x + unit_x.minor * ceilf((axis_x.min - pt_origin.x) / unit_x.minor);
		for (FLOAT val = 0.0f; val < axis_x.range; val += unit_x.minor)
		{
			draw_line_v(renderer, target, rect, val0 + val, brush, line_width, style);
		}
	}
};


ChartGraphicsBase::ChartGraphicsBase(void):
	pImpl(new Impl(this))
{
}


ChartGraphicsBase::~ChartGraphicsBase(void)
{
	if (pImpl)
	{
		delete pImpl;
		pImpl = NULL;
	}
}

BOOL F35_NS::ChartGraphicsBase::AddDataSeries( LPCTSTR name, ChartDataSeriesBase const *series, ChartLegendBase const *legend )
{
	if (pImpl)
		return pImpl->add_data_series(name, series, legend);
	else
		return FALSE;
}

ChartDataSeriesBase const * F35_NS::ChartGraphicsBase::GetDataSeries( LPCTSTR name ) const
{
	if (pImpl && pImpl->data_series_map.count(name) > 0)
		return pImpl->data_series_map[name];
	else
		return NULL;
}

ChartLegendBase const * F35_NS::ChartGraphicsBase::GetLegend( LPCTSTR name ) const
{
	if (pImpl && pImpl->legend_map.count(name) > 0)
		return pImpl->legend_map[name];
	else
		return NULL;

}

BOOL F35_NS::ChartGraphicsBase::RemoveDataSeries( LPCTSTR name )
{
	if (pImpl)
		return pImpl->remove_data_series(name);
	else
		return FALSE;
}

void F35_NS::ChartGraphicsBase::ClearAllDataSeries( void )
{
	if (pImpl)
	{
		pImpl->data_series_map.clear();
		pImpl->legend_map.clear();
	}
}

void F35_NS::ChartGraphicsBase::SetChartAxisX (FLOAT min, FLOAT max)
{
	if (pImpl && min < max)
	{
		pImpl->axis_x.min = min;
		pImpl->axis_x.max = max;
		pImpl->axis_x.range = max - min;
	}
}
void F35_NS::ChartGraphicsBase::SetChartAxisY (FLOAT min, FLOAT max)
{
	if (pImpl && min < max)
	{
		pImpl->axis_y.min = min;
		pImpl->axis_y.max = max;
		pImpl->axis_y.range = max - min;
	}
}
void F35_NS::ChartGraphicsBase::SetChartAxisZ (FLOAT min, FLOAT max)
{
	if (pImpl && min < max)
	{
		pImpl->axis_z.min = min;
		pImpl->axis_z.max = max;
		pImpl->axis_z.range = max - min;
	}
}
void F35_NS::ChartGraphicsBase::SetChartAxisW (FLOAT min, FLOAT max)
{
	if (pImpl && min < max)
	{
		pImpl->axis_w.min = min;
		pImpl->axis_w.max = max;
		pImpl->axis_w.range = max - min;
	}
}

void F35_NS::ChartGraphicsBase::GetChartAxisX (FLOAT *min, FLOAT *max) const
{
	if (pImpl)
	{
		*min = pImpl->axis_x.min;
		*max = pImpl->axis_x.max;
	}
}
void F35_NS::ChartGraphicsBase::GetChartAxisY (FLOAT *min, FLOAT *max) const
{
	if (pImpl)
	{
		*min = pImpl->axis_y.min;
		*max = pImpl->axis_y.max;
	}
}
void F35_NS::ChartGraphicsBase::GetChartAxisZ (FLOAT *min, FLOAT *max) const
{
	if (pImpl)
	{
		*min = pImpl->axis_z.min;
		*max = pImpl->axis_z.max;
	}
}
void F35_NS::ChartGraphicsBase::GetChartAxisW (FLOAT *min, FLOAT *max) const
{
	if (pImpl)
	{
		*min = pImpl->axis_w.min;
		*max = pImpl->axis_w.max;
	}
}

void F35_NS::ChartGraphicsBase::SetChartUnitX (FLOAT major, FLOAT minor)
{
	if (pImpl && major >= minor)
	{
		pImpl->unit_x.major = major;
		pImpl->unit_x.minor = minor;
	}
}
void F35_NS::ChartGraphicsBase::SetChartUnitY (FLOAT major, FLOAT minor)
{
	if (pImpl && major >= minor)
	{
		pImpl->unit_y.major = major;
		pImpl->unit_y.minor = minor;
	}
}
void F35_NS::ChartGraphicsBase::SetChartUnitZ (FLOAT major, FLOAT minor)
{
	if (pImpl && major >= minor)
	{
		pImpl->unit_z.major = major;
		pImpl->unit_z.minor = minor;
	}
}
void F35_NS::ChartGraphicsBase::SetChartUnitW (FLOAT major, FLOAT minor)
{
	if (pImpl && major >= minor)
	{
		pImpl->unit_w.major = major;
		pImpl->unit_w.minor = minor;
	}
}

void F35_NS::ChartGraphicsBase::GetChartUnitX (FLOAT *major, FLOAT *minor) const
{
	if (pImpl)
	{
		*major = pImpl->unit_w.major;
		*minor = pImpl->unit_w.minor;
	}
}
void F35_NS::ChartGraphicsBase::GetChartUnitY (FLOAT *major, FLOAT *minor) const
{
	if (pImpl)
	{
		*major = pImpl->unit_y.major;
		*minor = pImpl->unit_y.minor;
	}
}
void F35_NS::ChartGraphicsBase::GetChartUnitZ (FLOAT *major, FLOAT *minor) const
{
	if (pImpl)
	{
		*major = pImpl->unit_z.major;
		*minor = pImpl->unit_z.minor;
	}
}
void F35_NS::ChartGraphicsBase::GetChartUnitW (FLOAT *major, FLOAT *minor) const
{
	if (pImpl)
	{
		*major = pImpl->unit_w.major;
		*minor = pImpl->unit_w.minor;
	}
}

HRESULT F35_NS::ChartGraphicsBase::PlotChartData( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *chart_rect )
{
	if (! pImpl) return S_FALSE;
	pImpl->plot_chart_data(renderer, target, chart_rect);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::PlotLegends( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *rect )
{
	if (! pImpl) return S_FALSE;
	pImpl->plot_legends(renderer, target, rect);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawHorizontalLine( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, FLOAT y_value, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_line_h(renderer, target, chart_rect, y_value, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMajorGridLinesY( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_major_grid_y(renderer, target, chart_rect, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMinorGridLinesY( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_minor_grid_y(renderer, target, chart_rect, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMajorGridLabelY( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, 
	FLOAT label_margin, FLOAT label_width, 
	IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format /*= _T("%.0f") */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_major_label_y(renderer, target, chart_rect, label_margin, label_width, text_format, brush, format);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawVerticalLine( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, FLOAT x_value, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL*/ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_line_v(renderer, target, chart_rect, x_value, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMajorGridLinesX( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_major_grid_x(renderer, target, chart_rect, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMinorGridLinesX( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_minor_grid_x(renderer, target, chart_rect, brush, line_width, style);
	return S_OK;
}

HRESULT F35_NS::ChartGraphicsBase::DrawMajorGridLabelX( 
	D2DRendererBase *renderer, ID2D1RenderTarget *target, 
	D2D1_RECT_F const *chart_rect,
	FLOAT label_margin, FLOAT label_height, 
	IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format /*= _T("%.0f") */ )
{
	if (! pImpl) return S_FALSE;
	pImpl->draw_major_label_x(renderer, target, chart_rect, label_margin, label_height, text_format, brush, format);
	return S_OK;
}

void F35_NS::ChartGraphicsBase::SetChartNumberStringFormat( LPCTSTR fmt )
{
	if (pImpl) pImpl->number_format = fmt;
}

LPCTSTR F35_NS::ChartGraphicsBase::GetChartNumberStringFormat( void ) const
{
	return pImpl? pImpl->number_format.c_str(): (LPCTSTR) 0;
}

void F35_NS::ChartGraphicsBase::SetForegroundColor( const D2D1::ColorF & color )
{
	if (pImpl) pImpl->color_fg = color;
}

void F35_NS::ChartGraphicsBase::SetBackgroundColor( const D2D1::ColorF & color )
{
	if (pImpl) pImpl->color_bg = color;
}

D2D1::ColorF F35_NS::ChartGraphicsBase::GetForegroundColor( void ) const
{
	return 	pImpl? pImpl->color_fg : D2D1::ColorF(0);
}

D2D1::ColorF F35_NS::ChartGraphicsBase::GetBackgroundColor( void ) const
{
	return 	pImpl? pImpl->color_bg : D2D1::ColorF(0) ;
}

void F35_NS::ChartGraphicsBase::InvertAxisX( BOOL enable )
{
	if (pImpl) pImpl->inv_x = enable;
}

void F35_NS::ChartGraphicsBase::InvertAxisY( BOOL enable )
{
	if (pImpl) pImpl->inv_y = enable;
}

void F35_NS::ChartGraphicsBase::InvertAxisZ( BOOL enable )
{
	if (pImpl) pImpl->inv_z = enable;
}

void F35_NS::ChartGraphicsBase::InvertAxisW( BOOL enable )
{
	if (pImpl) pImpl->inv_w = enable;
}

void F35_NS::ChartGraphicsBase::SetAxisPositionX( FLOAT y_val )
{
	if (pImpl) pImpl->axis_pos_x = (y_val - pImpl->axis_y.min) / pImpl->axis_y.range ;
}

FLOAT F35_NS::ChartGraphicsBase::GetAxisPositionX( void )
{
	return pImpl->axis_pos_x * pImpl->axis_y.range + pImpl->axis_y.min;
}

void F35_NS::ChartGraphicsBase::SetAxisPositionY( FLOAT x_val )
{
	if (pImpl) pImpl->axis_pos_y = (x_val - pImpl->axis_x.min) / pImpl->axis_x.range ;
}

FLOAT F35_NS::ChartGraphicsBase::GetAxisPositionY( void )
{
	return pImpl->axis_pos_y * pImpl->axis_x.range + pImpl->axis_x.min;
}

HRESULT F35_NS::ChartGraphicsBase::DrawAxisLineY( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (pImpl == NULL) return S_FALSE;

	FLOAT pos_x = pImpl->axis_x.min + pImpl->axis_pos_y * pImpl->axis_x.range;
	return DrawVerticalLine(renderer, target, chart_rect, pos_x, brush, line_width, style);
}

HRESULT F35_NS::ChartGraphicsBase::DrawAxisLineX( D2DRendererBase *renderer, ID2D1RenderTarget *target, D2D1_RECT_F const *chart_rect, ID2D1Brush *brush, FLOAT line_width /*= 1.0f*/, ID2D1StrokeStyle *style /*= NULL */ )
{
	if (pImpl == NULL) return S_FALSE;

	FLOAT pos_y = pImpl->axis_y.min + pImpl->axis_pos_x * pImpl->axis_y.range;
	return DrawHorizontalLine(renderer, target, chart_rect, pos_y, brush, line_width, style);
}

BOOL F35_NS::ChartGraphicsBase::IsInvertedAxisX( void )
{
	return pImpl->inv_x;
}

BOOL F35_NS::ChartGraphicsBase::IsInvertedAxisY( void )
{
	return pImpl->inv_y;
}

BOOL F35_NS::ChartGraphicsBase::IsInvertedAxisZ( void )
{
	return pImpl->inv_z;
}

BOOL F35_NS::ChartGraphicsBase::IsInvertedAxisW( void )
{
	return pImpl->inv_w;
}

void F35_NS::ChartGraphicsBase::SetChartOrigin(D2D_VECTOR_4F pt )
{
	pImpl->pt_origin = pt;
}

D2D_VECTOR_4F F35_NS::ChartGraphicsBase::GetChartOrigin( void )
{
	return pImpl->pt_origin;
}

D2D1_SIZE_F F35_NS::ChartGraphicsBase::GetChartAreaSize(void)
{
	return pImpl->area_size;
}

void F35_NS::ChartGraphicsBase::SetChartAreaSize(D2D1_SIZE_F sz)
{
	pImpl->area_size = sz;
}

void F35_NS::ChartGraphicsBase::SetChartAreaSize(FLOAT x, FLOAT y)
{
	pImpl->area_size = D2D1::SizeF(x, y);
}