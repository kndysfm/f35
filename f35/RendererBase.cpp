#include "stdafx.h"

#include "RendererBase.h"
#include <stdarg.h>
#include <wincodec.h>

USING_F35_NS;

class RendererBase::Impl
{
	H::R<ID2D1Factory> pD2dFactory;
	H::R<ID2D1HwndRenderTarget> pRenderTarget;
	H::R<IDWriteFactory> pDWriteFactory;

	HWND hWnd;
	HWND hParent;
	RendererBase *self;
	D2D1_POINT_2F currPos;

	BOOL enable_auto_erase;
	D2D1_COLOR_F color_to_erase;
	D2D1_RENDER_TARGET_PROPERTIES rtProps_shared;

public:
	Impl(RendererBase *renderer, HWND hwnd)
	{
		this->self = renderer;
		this->hWnd = hwnd;
		HWND hc = hwnd;
		HWND hp = hwnd;
		do {
			hc = hp;
			hp = ::GetParent(hc);
		}while (hp != NULL);
		this->hParent = hc;
		this->enable_auto_erase = FALSE;
		color_to_erase = D2D1::ColorF(0);
	}

	virtual ~Impl(void)
	{
	}

	ID2D1RenderTarget* GetTarget(void)
	{
		return pRenderTarget;
	}

	D2D1_POINT_2F GetCurrPosDpi(void)
	{
		return currPos;
	}

	IDWriteTextFormat * GetTextFormat( LPCTSTR fontName, FLOAT fontSize, 
		DWRITE_TEXT_ALIGNMENT textAlign, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign )
	{
		if (pDWriteFactory)
		{
			IDWriteTextFormat *pFormat = NULL;
			HRESULT hr = pDWriteFactory->CreateTextFormat(
				fontName,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				fontSize,
				L"", //locale
				&pFormat
				);
			if (SUCCEEDED(hr))
			{
				pFormat->SetTextAlignment(textAlign);
				pFormat->SetParagraphAlignment(paragraphAlign);
				return pFormat;
			}
			else if (pFormat) pFormat->Release();
		}
		return NULL;
	}

	D2D1_SIZE_F GetSize(void) const
	{
		return pRenderTarget->GetSize();
	}

	ID2D1SolidColorBrush * GetSolidBrush( const D2D1::ColorF & color )
	{
		if (pRenderTarget)
		{
			ID2D1SolidColorBrush *pBrush = NULL;
			HRESULT hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			if (SUCCEEDED(hr))
			{
				return pBrush;
			}
			else if (pBrush) pBrush->Release();
		}
		return NULL;
	}

	ID2D1StrokeStyle * GetStrokeStyle(  FLOAT dashes, UINT32 dashesCount )
	{
		if (pDWriteFactory)
		{
			ID2D1StrokeStyle *pStyle;
			D2D1_STROKE_STYLE_PROPERTIES props = D2D1::StrokeStyleProperties();
			HRESULT hr = pD2dFactory->CreateStrokeStyle(props, &dashes, dashesCount, &pStyle);
			if (SUCCEEDED(hr))
			{
				return pStyle;
			}
			else if (pStyle) pStyle->Release();
		}
		return NULL;
	}

	ID2D1Layer * GetLayer(void)
	{
		if (pRenderTarget)
		{
			ID2D1Layer *pLayer = NULL;
			D2D1_SIZE_F size = pRenderTarget->GetSize();
			HRESULT hr = pRenderTarget->CreateLayer(size, &pLayer);
			if (SUCCEEDED(hr))
			{
				return pLayer;
			}
			else if (pLayer) pLayer->Release();
		}
		return NULL;
	}

	ID2D1Bitmap * GetBitmap(void)
	{
		if (pRenderTarget)
		{
			ID2D1Bitmap *pBitmap = NULL;
			D2D1_SIZE_F sizeF = pRenderTarget->GetSize();
			D2D1_SIZE_U sizeU = D2D1::SizeU((UINT32)sizeF.width, (UINT32)sizeF.height);
			D2D1_BITMAP_PROPERTIES prop;
			pRenderTarget->GetDpi(&prop.dpiX, &prop.dpiY);
			prop.pixelFormat = pRenderTarget->GetPixelFormat();
			HRESULT hr = pRenderTarget->CreateBitmap(sizeU, prop, &pBitmap);
			if (SUCCEEDED(hr))
			{
				hr = pBitmap->CopyFromRenderTarget(NULL, pRenderTarget, NULL);
				if (SUCCEEDED(hr))
				{
					return pBitmap;
				}
			}
			
			if (pBitmap) pBitmap->Release();
		}
		return NULL;
	}

	ID2D1PathGeometry * GetPathGeometry (void)
	{
		if (pD2dFactory)
		{
			ID2D1PathGeometry *pPath;
			HRESULT hr = pD2dFactory->CreatePathGeometry(&pPath);
			if (SUCCEEDED(hr))
			{
				return pPath;
			}
			else if (pPath) pPath->Release();
		}
		return NULL;
	}

	HRESULT ResetRenderer(void)
	{
		HRESULT hr = S_OK;
		pRenderTarget = NULL;
		if (pD2dFactory)
		{
			RECT rc;
			GetClientRect(this->hWnd, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
			rtProps_shared = D2D1::RenderTargetProperties();
			rtProps_shared.type = D2D1_RENDER_TARGET_TYPE_SOFTWARE;
			rtProps_shared.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
			ID2D1HwndRenderTarget *pRT;
			hr = pD2dFactory->CreateHwndRenderTarget(
				rtProps_shared,//D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(this->hWnd, size),
				&pRT
			);
			pRenderTarget = pRT;
		}
		return hr;
	}

	HRESULT Init(void)
	{
		HRESULT hr = S_OK;

		if (!pD2dFactory)
		{
			ID2D1Factory *pFactory;
			hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory);
			pD2dFactory = pFactory;
		}
		if (!pDWriteFactory && SUCCEEDED(hr)/* && pD2dFactory*/)
		{
			IDWriteFactory *pFactory;
			// Create a DirectWrite factory.
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(pDWriteFactory),
				reinterpret_cast<IUnknown **>(&pFactory)
				);
			pDWriteFactory = pFactory;
		}
		if (SUCCEEDED(hr)) hr = ResetRenderer();

		return hr;
	}

	void Update( void )
	{
		static USHORT lastVkeyStates[256] = {0};

		HWND focusWnd = ::GetActiveWindow();
		do {
			HWND h = ::GetParent(focusWnd);
			if (h == NULL) break;
			else focusWnd = h;
		}while(1);

		if (focusWnd == hParent)
		{
			for (INT vk = 0; vk < 0x100; vk++)
			{
				USHORT state = ::GetAsyncKeyState(vk) & 0x8000;
				if (lastVkeyStates[vk] != state)
				{
					if (state) self->OnKeyDown(vk);
					else self->OnKeyUp(vk);

					lastVkeyStates[vk] = state;
				}
			}
		}

		POINT pt;
		if (pRenderTarget && ::GetCursorPos(&pt) && ::ScreenToClient(hWnd, &pt))
		{
			FLOAT dpiX, dpiY;
			pRenderTarget->GetDpi(&dpiX, &dpiY);
			currPos.x = (FLOAT)pt.x*96.0f/dpiX;
			currPos.y = (FLOAT)pt.y*96.0f/dpiY;
		}

	}

	HRESULT BeginRender(void)
	{
		if (!pRenderTarget) return S_FALSE;

		pRenderTarget->BeginDraw();
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		if (enable_auto_erase) pRenderTarget->Clear(color_to_erase);

		return S_OK;
	}

	HRESULT EndRender(HRESULT hr)
	{
		if (SUCCEEDED(hr))
		{
			if (pRenderTarget) hr = pRenderTarget->EndDraw();
		}
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			Destroy();
		}
		return hr;
	}

	void Invalidate(void)
	{
		::InvalidateRect(this->hWnd, NULL, FALSE);
	}

	void Destroy(void)
	{
		pD2dFactory = NULL;
		pRenderTarget = NULL;
		pDWriteFactory = NULL;
	}

	void EnableAutoErase(D2D1_COLOR_F c) { enable_auto_erase = TRUE;  color_to_erase = c; }
	void DisableAutoErase() { enable_auto_erase = FALSE; }

	HRESULT SaveImageFile(LPCTSTR filename, ImageFileFormat fmt)
	{
		if (!pD2dFactory || !pRenderTarget) return S_FALSE;

		HRESULT hr = S_OK;

		H::R<IWICImagingFactory> pWICFactory;
		H::R<IWICBitmap> pWICBitmap;
		H::R<ID2D1Bitmap>pSrcBitmap;
		H::R<ID2D1RenderTarget> pBitmapRenderTarget;
		H::R<IWICBitmapEncoder> pEncoder;
		H::R<IWICBitmapFrameEncode> pFrameEncode;
		H::R<IWICStream> pStream;

		if (SUCCEEDED(hr))
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

		//
		// Create IWICBitmap and RT
		//

		UINT sc_bitmapWidth = pRenderTarget->GetSize().width;
		UINT sc_bitmapHeight = pRenderTarget->GetSize().height;

		if (SUCCEEDED(hr))
		{
			IWICBitmap *ptr = NULL;
			hr = pWICFactory->CreateBitmap(
				sc_bitmapWidth,
				sc_bitmapHeight,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapCacheOnLoad,
				&ptr
			);
			pWICBitmap = ptr;
		}

		if (SUCCEEDED(hr))
		{
			ID2D1RenderTarget *ptr = NULL;
			hr = pD2dFactory->CreateWicBitmapRenderTarget(
				pWICBitmap,
				rtProps_shared,
				&ptr
			);
			pBitmapRenderTarget = ptr;
		}

		if (SUCCEEDED(hr))
		{
			//
			// Render into the bitmap
			//
			pBitmapRenderTarget->BeginDraw();
			pBitmapRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			pSrcBitmap = GetBitmap();
			if (pSrcBitmap) pBitmapRenderTarget->DrawBitmap(pSrcBitmap);
			hr = pBitmapRenderTarget->EndDraw();
		}
		if (SUCCEEDED(hr))
		{
			IWICStream * ptr = NULL;
			hr = pWICFactory->CreateStream(&ptr);
			pStream = ptr;
		}

		if (SUCCEEDED(hr))
		{
			hr = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
		}
		if (SUCCEEDED(hr))
		{
			GUID guid = GUID_ContainerFormatPng;
			switch (fmt)
			{
			case fxxxv::RendererBase::IFF_PNG:
				guid = GUID_ContainerFormatPng;
				break;
			case fxxxv::RendererBase::IFF_BMP:
				guid = GUID_ContainerFormatBmp;
				break;
			case fxxxv::RendererBase::IFF_JPEG:
				guid = GUID_ContainerFormatJpeg;
				break;
			case fxxxv::RendererBase::IFF_GIF:
				guid = GUID_ContainerFormatGif;
				break;
			}
			IWICBitmapEncoder *ptr = NULL;
			hr = pWICFactory->CreateEncoder(guid, NULL, &ptr);
			pEncoder = ptr;
		}
		if (SUCCEEDED(hr))
		{
			hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
		}
		if (SUCCEEDED(hr))
		{
			IWICBitmapFrameEncode *ptr = NULL;
			hr = pEncoder->CreateNewFrame(&ptr, NULL);
			pFrameEncode = ptr;
		}
		if (SUCCEEDED(hr))
		{
			hr = pFrameEncode->Initialize(NULL);
		}
		if (SUCCEEDED(hr))
		{
			hr = pFrameEncode->SetSize(sc_bitmapWidth, sc_bitmapHeight);
		}
		if (SUCCEEDED(hr))
		{
			WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
			hr = pFrameEncode->SetPixelFormat(&format);
		}
		if (SUCCEEDED(hr))
		{
			hr = pFrameEncode->WriteSource(pWICBitmap, NULL);
		}
		if (SUCCEEDED(hr))
		{
			hr = pFrameEncode->Commit();
		}
		if (SUCCEEDED(hr))
		{
			hr = pEncoder->Commit();
		}

		return hr;
	}

	void Resize(BOOL scaling)
	{
		D2D1_SIZE_F size_last = pRenderTarget->GetSize();
		D2D1_RECT_U rect_src = D2D1::RectU(0, 0, (UINT32)size_last.width, (UINT32)size_last.height);
		H::R<ID2D1Bitmap> bmp_last = GetBitmap();
		HRESULT hr = ResetRenderer();
		if (SUCCEEDED(hr))
		{
			pRenderTarget->BeginDraw();
			if (scaling)
			{
				D2D1_SIZE_U size_px = bmp_last->GetPixelSize();
				D2D1_RECT_F rect_px = D2D1::RectF(0, 0, size_px.width, size_px.height);
				D2D1_SIZE_F size = pRenderTarget->GetSize();
				D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width, size.height);
				pRenderTarget->DrawBitmap(bmp_last, rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect_px);
			}
			else
			{
				pRenderTarget->DrawBitmap(bmp_last);
			}
			pRenderTarget->EndDraw();
		}
	}
};

RendererBase::RendererBase(HWND hwnd):
	pImpl(new Impl(this, hwnd))
{
}


RendererBase::~RendererBase(void)
{
	Destroy();
	delete pImpl;
}

HRESULT RendererBase::Init( void )
{
	HRESULT hr;
	Lock();

	// Init Base Class
	pImpl->Init();
	// Init Extended Class
	ID2D1RenderTarget* target = pImpl->GetTarget();
	if (target) hr = InternalInit(target);
	else hr = S_FALSE;

	Unlock();
	return hr;
}

void RendererBase::Update( void )
{
	if (!TryLock()) return;

	// Update Base Class
	pImpl->Update();
	// Update Extended Class
	InternalUpdate();

	Unlock();
}

HRESULT RendererBase::Render( void )
{
	if (!TryLock()) return S_FALSE;

	// Prepare to Render
	HRESULT hr = pImpl->BeginRender();
	// Render by Extended class
	if (SUCCEEDED(hr))
	{
		ID2D1RenderTarget* target = pImpl->GetTarget();
		if (target)
		{
			InternalRender(target);
		}
	}
	// Prepare to Render
	hr = pImpl->EndRender(hr);

	Unlock();
	return hr;
}

void RendererBase::Destroy( void )
{
	Lock();
	// Destroy Extended class
	InternalDestroy();
	// Destroy Base Class
	pImpl->Destroy();
	
	Unlock();
}

H::R<IDWriteTextFormat> RendererBase::MakeTextFormat( LPCTSTR fontName, FLOAT fontSize, DWRITE_TEXT_ALIGNMENT textAlign /*= DWRITE_TEXT_ALIGNMENT_CENTER*/, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign /*= DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/ )
{
	return pImpl->GetTextFormat(fontName, fontSize, textAlign, paragraphAlign);
}

H::R<ID2D1SolidColorBrush> RendererBase::MakeBrush( const D2D1::ColorF & color )
{
	return pImpl->GetSolidBrush(color);
}

H::R<ID2D1Layer> RendererBase::MakeLayer(void)
{
	return pImpl->GetLayer();
}

H::R<ID2D1Bitmap> RendererBase::MakeBitmap(void)
{
	return pImpl->GetBitmap();
}

D2D1_POINT_2F RendererBase::GetCurrentCursorPosDpi( void )
{
	return pImpl->GetCurrPosDpi();
}

void RendererBase::Invalidate( void )
{
	pImpl->Invalidate();
}

D2D1_SIZE_F F35_NS::RendererBase::GetSize( void ) const
{
	return pImpl->GetSize();
}

D2D1_RECT_F F35_NS::RendererBase::GetRectangle( void ) const
{
	D2D1_RECT_F rect;
	D2D1_SIZE_F size = pImpl->GetSize();
	rect.left = rect.top = 0.0f;
	rect.right = size.width;
	rect.bottom = size.height;
	return rect;
}

H::R<ID2D1StrokeStyle> F35_NS::RendererBase::MakeStrokeStyle( FLOAT dashes, UINT32 dashesCount )
{
	return pImpl->GetStrokeStyle(dashes, dashesCount);
}

H::R<ID2D1PathGeometry> F35_NS::RendererBase::MakePathGeometry( void )
{
	return pImpl->GetPathGeometry();
}

void F35_NS::RendererBase::EnableAutoErase(D2D1_COLOR_F color_to_erase)
{
	pImpl->EnableAutoErase(color_to_erase);
}

void F35_NS::RendererBase::DisableAutoErase(void)
{
	pImpl->DisableAutoErase();
}

HRESULT F35_NS::RendererBase::SaveImageFile(LPCTSTR filename, ImageFileFormat fmt)
{
	return pImpl->SaveImageFile(filename, fmt);
}

void F35_NS::RendererBase::Resize(BOOL scaling)
{
	return pImpl->Resize(scaling);
}
