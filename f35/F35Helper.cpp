#include "stdafx.h"

#include "F35Helper.h"

#include <stdarg.h>

D2D1_RECT_F F35_NS::H::MakeRectRatios(D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	D2D1_RECT_F new_rect;

	new_rect.left = left * src_rect.right + (1.0f - left) * src_rect.left;
	new_rect.top = top * src_rect.bottom + (1.0f - top) * src_rect.top;
	new_rect.right = right * src_rect.right + (1.0f - right) * src_rect.left;
	new_rect.bottom = bottom * src_rect.bottom + (1.0f - bottom) * src_rect.top;

	return new_rect;
}

D2D1_RECT_F F35_NS::H::MakeRectRatios(D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect)
{
	return MakeRectRatios(src_rect, ratio_rect.left, ratio_rect.top, ratio_rect.right, ratio_rect.bottom);
}

D2D1_POINT_2F F35_NS::H::MakePointRatio(D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio)
{
	D2D1_POINT_2F new_pt;

	new_pt.x = ratio * pt1.x, +(1.0f - ratio) * pt0.x;
	new_pt.y = ratio * pt1.y, +(1.0f - ratio) * pt0.y;

	return new_pt;
}

void F35_NS::H::PaintRectangle(ID2D1RenderTarget *target, const D2D1_RECT_F &rect, ID2D1Brush *fill_brush, ID2D1Brush *line_brush, FLOAT line_width)
{
	target->FillRectangle(rect, fill_brush);
	target->DrawRectangle(rect, line_brush, line_width);
}

void F35_NS::H::WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect, ID2D1Brush *brush, LPCTSTR str_fmt, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	target->DrawText(str, _tcslen(str), format, rect, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	delete[] str;

	va_end(arg_ptr);
}

void F35_NS::H::WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect,
	DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align,
	ID2D1Brush *brush, LPCTSTR str_fmt, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	DWRITE_TEXT_ALIGNMENT last_text_align = format->GetTextAlignment();
	DWRITE_PARAGRAPH_ALIGNMENT last_paragraph_align = format->GetParagraphAlignment();
	format->SetTextAlignment(text_align);
	format->SetParagraphAlignment(paragraph_align);
	target->DrawText(str, _tcslen(str), format, rect, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	format->SetTextAlignment(last_text_align);
	format->SetParagraphAlignment(last_paragraph_align);

	delete[] str;

	va_end(arg_ptr);
}

F35_NS::H::R<ID2D1SolidColorBrush> F35_NS::H::MakeSolidColorBrush(ID2D1RenderTarget * target, const D2D1_COLOR_F & color)
{
	ID2D1SolidColorBrush *ptr = nullptr;
	HRESULT hr = target->CreateSolidColorBrush(color, &ptr);
	if (SUCCEEDED(hr)) return ptr;
	else if (ptr) ptr->Release();
	return nullptr;
}

F35_NS::H::R<ID2D1Layer> F35_NS::H::MakeLayer(ID2D1RenderTarget * target)
{
	ID2D1Layer *ptr = NULL;
	D2D1_SIZE_F size = target->GetSize();
	HRESULT hr = target->CreateLayer(size, &ptr);
	if (SUCCEEDED(hr)) return ptr;
	else if (ptr) ptr->Release();
	return nullptr;
}

USING_F35_NS;

struct Factory::Impl
{
	H::R<ID2D1Factory> pD2dFactory;
	H::R<IDWriteFactory> pDWriteFactory;
	H::R<IWICImagingFactory> pWICFactory;
	D2D1_RENDER_TARGET_PROPERTIES rtProps_shared;

	HRESULT Init(void)
	{
		HRESULT hr = S_OK;

		::CoInitialize(nullptr);

		if (!pD2dFactory)
		{
			ID2D1Factory *ptr = NULL;
			hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &ptr);
			pD2dFactory = ptr;

			rtProps_shared = D2D1::RenderTargetProperties();
			rtProps_shared.type = D2D1_RENDER_TARGET_TYPE_SOFTWARE;
			rtProps_shared.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
		}
		if (!pDWriteFactory && SUCCEEDED(hr))
		{
			IDWriteFactory *ptr = NULL;
			// Create a DirectWrite factory.
			hr = ::DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(pDWriteFactory),
				reinterpret_cast<IUnknown **>(&ptr)
			);
			pDWriteFactory = ptr;
		}
		if (!pWICFactory && SUCCEEDED(hr))
		{
			IWICImagingFactory *ptr = NULL;
			hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)&ptr
			);
			pWICFactory = ptr;
		}

		return hr;
	}

	void Finalize(void)
	{
		pD2dFactory = NULL;
		pDWriteFactory = NULL;
		pWICFactory = NULL;
	}

	IDWriteTextFormat * GetTextFormat(LPCTSTR fontName, FLOAT fontSize,
		DWRITE_TEXT_ALIGNMENT textAlign, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign)
	{
		if (pDWriteFactory)
		{
			IDWriteTextFormat *ptr = NULL;
			HRESULT hr = pDWriteFactory->CreateTextFormat(
				fontName,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				fontSize,
				L"", //locale
				&ptr
			);
			if (SUCCEEDED(hr))
			{
				ptr->SetTextAlignment(textAlign);
				ptr->SetParagraphAlignment(paragraphAlign);
				return ptr;
			}
			else if (ptr) ptr->Release();
		}
		return NULL;
	}


	ID2D1StrokeStyle * GetStrokeStyle(FLOAT const *dashes, UINT32 dashesCount)
	{
		if (pDWriteFactory)
		{
			ID2D1StrokeStyle *ptr = NULL;
			D2D1_STROKE_STYLE_PROPERTIES props = D2D1::StrokeStyleProperties();
			HRESULT hr = pD2dFactory->CreateStrokeStyle(props, dashes, dashesCount, &ptr);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}


	IWICBitmap *GetWICBitmap(UINT width_px, UINT height_px)
	{
		if (pWICFactory)
		{
			IWICBitmap *ptr = NULL;
			HRESULT hr = pWICFactory->CreateBitmap(
				width_px, height_px,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapCacheOnLoad,
				&ptr
			);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}

	ID2D1RenderTarget *GetWICBitmapRenderTarget(IWICBitmap *pBmp)
	{
		if (pD2dFactory)
		{
			ID2D1RenderTarget *ptr = NULL;
			HRESULT hr = pD2dFactory->CreateWicBitmapRenderTarget(pBmp, rtProps_shared, &ptr);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}

	ID2D1HwndRenderTarget * GetHwndRenderTarget(HWND hwnd, D2D1_SIZE_U size)
	{
		if (pD2dFactory)
		{
			ID2D1HwndRenderTarget *ptr;
			HRESULT hr = pD2dFactory->CreateHwndRenderTarget(
				rtProps_shared,//D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, size),
				&ptr
			);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}


	ID2D1PathGeometry * GetPathGeometry(void)
	{
		if (pD2dFactory)
		{
			ID2D1PathGeometry *ptr;
			HRESULT hr = pD2dFactory->CreatePathGeometry(&ptr);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}

	IWICStream * MakeWicStream(void)
	{
		if (pWICFactory)
		{
			IWICStream *ptr;
			HRESULT hr = pWICFactory->CreateStream(&ptr);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}

	IWICBitmapEncoder * MakeWicEncoder(ImageFileFormat fmt)
	{
		if (pWICFactory)
		{
			GUID guid = GUID_ContainerFormatPng;
			switch (fmt)
			{
			case fxxxv::Factory::IFF_PNG:
				guid = GUID_ContainerFormatPng;
				break;
			case fxxxv::Factory::IFF_BMP:
				guid = GUID_ContainerFormatBmp;
				break;
			case fxxxv::Factory::IFF_JPEG:
				guid = GUID_ContainerFormatJpeg;
				break;
			case fxxxv::Factory::IFF_GIF:
				guid = GUID_ContainerFormatGif;
				break;
			}
			IWICBitmapEncoder *ptr = NULL;
			HRESULT hr = pWICFactory->CreateEncoder(guid, NULL, &ptr);
			if (SUCCEEDED(hr)) return ptr;
			else if (ptr) ptr->Release();
		}
		return NULL;
	}
};

std::unique_ptr<Factory::Impl> Factory::pImpl(new Factory::Impl);

HRESULT Factory::Init(void) { return pImpl->Init(); }

void Factory::Finalize(void) { return pImpl->Finalize(); }

H::R<IDWriteTextFormat> Factory::MakeTextFormat(LPCTSTR fontName, FLOAT fontSize, DWRITE_TEXT_ALIGNMENT textAlign /*= DWRITE_TEXT_ALIGNMENT_CENTER*/, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign /*= DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/)
{
	return pImpl->GetTextFormat(fontName, fontSize, textAlign, paragraphAlign);
}

H::R<ID2D1StrokeStyle> Factory::MakeStrokeStyle(FLOAT const *dashes, UINT32 dashesCount)
{
	return pImpl->GetStrokeStyle(dashes, dashesCount);
}

H::R<IWICBitmap> Factory::MakeWICBitmap(UINT width_px, UINT height_px)
{
	return pImpl->GetWICBitmap(width_px, height_px);
}

H::R<ID2D1RenderTarget> Factory::MakeWicBitmapRenderTarget(IWICBitmap *pBmp)
{
	return pImpl->GetWICBitmapRenderTarget(pBmp);
}

H::R<ID2D1HwndRenderTarget> Factory::MakeHwndRenderTarget(HWND hwnd, D2D1_SIZE_U size)
{
	return pImpl->GetHwndRenderTarget(hwnd, size);
}

H::R<ID2D1PathGeometry> Factory::MakePathGeometry(void)
{
	return pImpl->GetPathGeometry();
}

H::R<IWICStream> Factory::MakeWicStream(void)
{
	return pImpl->MakeWicStream();
}

H::R<IWICBitmapEncoder> Factory::MakeWicEncoder(ImageFileFormat fmt)
{
	return pImpl->MakeWicEncoder(fmt);
}
