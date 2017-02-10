#pragma once
#include "GraphicsBase.h"

#include "ChartDataSeriesBase.h"
#include "ChartLegendBase.h"

namespace F35_NS
{


class ChartGraphicsBase :
	public GraphicsBase
{
	struct Impl;
	Impl * pImpl;

public:
	ChartGraphicsBase(void);
	virtual ~ChartGraphicsBase(void);

	BOOL AddDataSeries(LPCTSTR name, ChartDataSeriesBase const *series, ChartLegendBase const *legend);

	ChartDataSeriesBase const * GetDataSeries(LPCTSTR name) const;

	ChartLegendBase const * GetLegend(LPCTSTR name) const;

	BOOL RemoveDataSeries(LPCTSTR name);

	void ClearAllDataSeries(void);

	void SetChartAxisX (FLOAT min, FLOAT max);
	void SetChartAxisY (FLOAT min, FLOAT max);
	void SetChartAxisZ (FLOAT min, FLOAT max);
	void SetChartAxisW (FLOAT min, FLOAT max);

	void GetChartAxisX (FLOAT *min, FLOAT *max) const;
	void GetChartAxisY (FLOAT *min, FLOAT *max) const;
	void GetChartAxisZ (FLOAT *min, FLOAT *max) const;
	void GetChartAxisW (FLOAT *min, FLOAT *max) const;

	void SetChartUnitX (FLOAT major, FLOAT minor = 0.0f);
	void SetChartUnitY (FLOAT major, FLOAT minor = 0.0f);
	void SetChartUnitZ (FLOAT major, FLOAT minor = 0.0f);
	void SetChartUnitW (FLOAT major, FLOAT minor = 0.0f);

	void GetChartUnitX (FLOAT *major, FLOAT *minor) const;
	void GetChartUnitY (FLOAT *major, FLOAT *minor) const;
	void GetChartUnitZ (FLOAT *major, FLOAT *minor) const;
	void GetChartUnitW (FLOAT *major, FLOAT *minor) const;

	void SetChartOrigin(D2D_VECTOR_4F pt);
	D2D_VECTOR_4F GetChartOrigin(void ) const;

	void SetChartNumberStringFormat(LPCTSTR fmt);

	LPCTSTR GetChartNumberStringFormat( void ) const;

	void SetForegroundColor(const D2D1::ColorF & color);

	void SetBackgroundColor(const D2D1::ColorF & color);

	D2D1::ColorF GetForegroundColor(void) const;

	D2D1::ColorF GetBackgroundColor(void) const;

	void InvertAxisX (BOOL enable);
	void InvertAxisY (BOOL enable);
	void InvertAxisZ (BOOL enable);
	void InvertAxisW (BOOL enable);
	BOOL IsInvertedAxisX (void) const;
	BOOL IsInvertedAxisY (void) const;
	BOOL IsInvertedAxisZ (void) const;
	BOOL IsInvertedAxisW (void) const;

	void SetAxisPositionX (FLOAT y_val);
	FLOAT GetAxisPositionX (void) const;

	void SetAxisPositionY (FLOAT x_val);
	FLOAT GetAxisPositionY (void) const;

	D2D1_SIZE_F GetChartAreaSize(void) const;

	void SetChartAreaSize(D2D1_SIZE_F sz);
	void SetChartAreaSize(FLOAT x, FLOAT y);

	void AdjustChartAxesAuto(void);

protected:

	HRESULT PlotChartData( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect );

	HRESULT PlotLegends( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *rect );

	HRESULT DrawHorizontalLine( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect, FLOAT y_value,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawMajorGridLinesY( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawMinorGridLinesY( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawMajorGridLabelY( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect, FLOAT label_margin, FLOAT label_width,
		IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format = _T("%.0f") );

	HRESULT DrawAxisLineY( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect, 
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawVerticalLine( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect, FLOAT x_value,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL);

	HRESULT DrawMajorGridLinesX( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawMinorGridLinesX( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect,
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );

	HRESULT DrawMajorGridLabelX( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect,
		FLOAT label_margin, FLOAT label_height,
		IDWriteTextFormat *text_format, ID2D1Brush *brush, LPCTSTR format = _T("%.0f")  );

	HRESULT DrawAxisLineX( RendererBase *renderer, ID2D1RenderTarget *target, 
		D2D1_RECT_F const *chart_rect, 
		ID2D1Brush *brush, FLOAT line_width = 1.0f, ID2D1StrokeStyle *style = NULL );


};

}
