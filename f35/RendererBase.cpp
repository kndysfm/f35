#include "stdafx.h"

#include "RendererBase.h"
#include <stdarg.h>


USING_F35_NS;

class RendererBase::Impl
{
	H::R<ID2D1HwndRenderTarget> pRenderTarget;

	HWND hWnd;
	HWND hParent;
	RendererBase *self;
	D2D1_POINT_2F currPos;

	BOOL enable_auto_erase;
	D2D1_COLOR_F color_to_erase;

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

	D2D1_POINT_2F GetCurrPosDpi(void) const
	{
		return currPos;
	}

	POINT GetScreenOrigin(void) const
	{
		POINT pt = { 0, 0 };
		::ClientToScreen(hWnd, &pt);
		return pt;
	}

	D2D1_SIZE_F GetSize(void) const
	{
		return pRenderTarget->GetSize();
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

	BOOL ResetRenderer(RendererBase *that)
	{
		RECT rc;
		::GetClientRect(this->hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		that->InternalDestroy();
		pRenderTarget = Factory::MakeHwndRenderTarget(this->hWnd, size);
		if (pRenderTarget)
		{
			that->InternalInit(pRenderTarget);
		}

		return pRenderTarget ? S_OK : S_FALSE;
	}

	HRESULT Init(RendererBase *that)
	{
		HRESULT hr = Factory::Init();

		if (SUCCEEDED(hr)) hr = ResetRenderer(that);

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
		pRenderTarget = NULL;
	}

	void EnableAutoErase(D2D1_COLOR_F c) { enable_auto_erase = TRUE;  color_to_erase = c; }
	void DisableAutoErase() { enable_auto_erase = FALSE; }

	HRESULT SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
	{
		if (!pRenderTarget) return S_FALSE;

		HRESULT hr = S_OK;

		H::R<IWICBitmap> pWICBitmap;
		H::R<ID2D1Bitmap> pSrcBitmap;
		H::R<ID2D1RenderTarget> pBitmapRenderTarget;
		H::R<IWICBitmapEncoder> pEncoder;
		H::R<IWICBitmapFrameEncode> pFrameEncode;
		H::R<IWICStream> pStream;

		//
		// Create IWICBitmap and RT
		//

		UINT sc_bitmapWidth = pRenderTarget->GetSize().width;
		UINT sc_bitmapHeight = pRenderTarget->GetSize().height;

		if (SUCCEEDED(hr))
		{
			pWICBitmap = Factory::MakeWICBitmap(sc_bitmapWidth, sc_bitmapHeight);
		}

		if (SUCCEEDED(hr) && pWICBitmap)
		{
			pBitmapRenderTarget = Factory::MakeWicBitmapRenderTarget(pWICBitmap);
		}

		if (SUCCEEDED(hr) && pBitmapRenderTarget)
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
			pStream = Factory::MakeWicStream();
		}

		if (SUCCEEDED(hr) && pStream)
		{
			hr = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
		}
		if (SUCCEEDED(hr))
		{
			pEncoder = Factory::MakeWicEncoder(fmt);
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

	void Resize(RendererBase *that, BOOL scaling)
	{
		D2D1_SIZE_F size_last = pRenderTarget->GetSize();
		D2D1_RECT_U rect_src = D2D1::RectU(0, 0, (UINT32)size_last.width, (UINT32)size_last.height);
		H::R<ID2D1Bitmap> bmp_last = GetBitmap();
		HRESULT hr = ResetRenderer(that);
		if (SUCCEEDED(hr) && !enable_auto_erase)
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

	void Validate(void)
	{
		::ValidateRect(this->hWnd, NULL);
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

	hr = pImpl->Init(this);

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

D2D1_POINT_2F RendererBase::GetCurrentCursorPosDpi( void )
{
	return pImpl->GetCurrPosDpi();
}

void RendererBase::Invalidate( void )
{
	pImpl->Invalidate();
}

void RendererBase::Validate(void)
{
	pImpl->Validate();
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

D2D1_RECT_F F35_NS::RendererBase::GetScreenRect(void)
{
	D2D1_RECT_F rect;
	D2D1_SIZE_F size = pImpl->GetSize();
	POINT origin = pImpl->GetScreenOrigin();
	rect.left = origin.x;
	rect.top = origin.y;
	rect.right = origin.x + size.width;
	rect.bottom = origin.y + size.height;
	return rect;
}

void F35_NS::RendererBase::EnableAutoErase(D2D1_COLOR_F color_to_erase)
{
	pImpl->EnableAutoErase(color_to_erase);
}

void F35_NS::RendererBase::DisableAutoErase(void)
{
	pImpl->DisableAutoErase();
}

HRESULT F35_NS::RendererBase::SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
{
	return pImpl->SaveImageFile(filename, fmt);
}

void F35_NS::RendererBase::Resize(BOOL scaling)
{
	return pImpl->Resize(this, scaling);
}

