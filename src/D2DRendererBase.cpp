#include "stdafx.h"

#include "D2DRendererBase.h"
#include <stdarg.h>

USING_F35_NS;

class D2DRendererBase::Impl
{
	ID2D1Factory* pD2dFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	IDWriteFactory* pDWriteFactory;

	HWND hWnd;
	HWND hParent;
	D2DRendererBase *self;
	D2D1_POINT_2F currPos;

public:
	Impl(D2DRendererBase *renderer, HWND hwnd)
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

		this->pD2dFactory = NULL;
		this->pRenderTarget = NULL;
		this->pDWriteFactory = NULL;
	}

	virtual ~Impl(void)
	{
		Destroy();
	}

	ID2D1RenderTarget* GetTarget(void)
	{
		return pRenderTarget;
	}

	D2D1_POINT_2F GetCurrPosDpi(void)
	{
		return currPos;
	}

	void Print(IDWriteTextFormat *format, const D2D1_RECT_F &rect, BOOL clip,
		ID2D1Brush *brush, LPCTSTR str)
	{
		pRenderTarget->DrawText(str, _tcslen(str), format, rect, brush, 
			clip? D2D1_DRAW_TEXT_OPTIONS_CLIP: D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	}

	void Print(IDWriteTextFormat *format, const D2D1_RECT_F &rect,
		DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align, BOOL clip,
		ID2D1Brush *brush, LPCTSTR str)
	{
		DWRITE_TEXT_ALIGNMENT last_text_align = format->GetTextAlignment();
		DWRITE_PARAGRAPH_ALIGNMENT last_paragraph_align = format->GetParagraphAlignment();
		format->SetTextAlignment(text_align);
		format->SetParagraphAlignment(paragraph_align);
		pRenderTarget->DrawText(str, _tcslen(str), format, rect, brush,
			clip? D2D1_DRAW_TEXT_OPTIONS_CLIP: D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
		format->SetTextAlignment(last_text_align);
		format->SetParagraphAlignment(last_paragraph_align);
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
			else
			{
				SafeRelease(&pFormat);
			}
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
			else
			{
				SafeRelease(&pBrush);
			}
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
			else
			{
				SafeRelease(&pStyle);
			}
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
			else
			{
				SafeRelease(&pPath);
			}
		}
		return NULL;
	}

	HRESULT Init(void)
	{
		HRESULT hr = S_OK;

		if (!pD2dFactory)
		{
			hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2dFactory);
		}
		if (!pRenderTarget && SUCCEEDED(hr) && pD2dFactory)
		{
			RECT rc;
			GetClientRect(this->hWnd, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
			hr = pD2dFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(this->hWnd, size),
				&pRenderTarget
				);
		}
		if (!pDWriteFactory && SUCCEEDED(hr) && pD2dFactory)
		{
			// Create a DirectWrite factory.
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(pDWriteFactory),
				reinterpret_cast<IUnknown **>(&pDWriteFactory)
				);
		}

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
		SafeRelease(&pD2dFactory);
		SafeRelease(&pRenderTarget);
		SafeRelease(&pDWriteFactory);
	}
};

D2DRendererBase::D2DRendererBase(HWND hwnd):
	pImpl(new Impl(this, hwnd))
{
}


D2DRendererBase::~D2DRendererBase(void)
{
	Destroy();
	delete pImpl;
}

HRESULT D2DRendererBase::Init( void )
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

void D2DRendererBase::Update( void )
{
	if (!TryLock()) return;

	// Update Base Class
	pImpl->Update();
	// Update Extended Class
	InternalUpdate();

	Unlock();
}

HRESULT D2DRendererBase::Render( void )
{
	if (!TryLock()) return S_FALSE;

	// Prepare to Render
	HRESULT hr = pImpl->BeginRender();
	// Render by Extended class
	if (SUCCEEDED(hr))
	{
		ID2D1RenderTarget* target = pImpl->GetTarget();
		if (target) hr = InternalRender(target);
	}
	// Prepare to Render
	hr = pImpl->EndRender(hr);

	Unlock();
	return hr;
}

void D2DRendererBase::Destroy( void )
{
	Lock();
	// Destroy Extended class
	InternalDestroy();
	// Destroy Base Class
	pImpl->Destroy();
	
	Unlock();
}

void D2DRendererBase::Print( IDWriteTextFormat *format, const D2D1_RECT_F &rect, 
						 ID2D1Brush *brush, LPCTSTR str_fmt, ... )
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	pImpl->Print(format, rect, TRUE, brush, str);

	delete[] str;

	va_end(arg_ptr);
}

void D2DRendererBase::Print( IDWriteTextFormat *format, const D2D1_RECT_F &rect, 
						 DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align,
						 ID2D1Brush *brush, LPCTSTR str_fmt, ... )
{
	va_list arg_ptr;
	va_start(arg_ptr, str_fmt);

	size_t len = _tcslen(str_fmt) * 8;
	_TCHAR *str = new _TCHAR[len];
	_vstprintf_s(str, len, str_fmt, arg_ptr);

	pImpl->Print(format, rect, text_align, paragraph_align, TRUE, brush, str);

	delete[] str;

	va_end(arg_ptr);
}
IDWriteTextFormat * D2DRendererBase::MakeTextFormat( LPCTSTR fontName, FLOAT fontSize, DWRITE_TEXT_ALIGNMENT textAlign /*= DWRITE_TEXT_ALIGNMENT_CENTER*/, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign /*= DWRITE_PARAGRAPH_ALIGNMENT_CENTER*/ )
{
	return pImpl->GetTextFormat(fontName, fontSize, textAlign, paragraphAlign);
}

ID2D1SolidColorBrush * D2DRendererBase::MakeBrush( const D2D1::ColorF & color )
{
	return pImpl->GetSolidBrush(color);
}

D2D1_POINT_2F D2DRendererBase::GetCurrentCursorPosDpi( void )
{
	return pImpl->GetCurrPosDpi();
}

void D2DRendererBase::PaintRectangle( const D2D1_RECT_F &rect, ID2D1Brush *fill_brush, ID2D1Brush *line_brush , FLOAT line_width)
{
	pImpl->GetTarget()->FillRectangle(rect, fill_brush);
	pImpl->GetTarget()->DrawRectangle(rect, line_brush, line_width);
}

D2D1_RECT_F D2DRendererBase::MakeRectRatios( D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom )
{
	D2D1_RECT_F new_rect;

	new_rect.left = left * src_rect.right + (1.0f - left) * src_rect.left;
	new_rect.top = top * src_rect.bottom + (1.0f - top) * src_rect.top;
	new_rect.right = right * src_rect.right + (1.0f - right) * src_rect.left;
	new_rect.bottom = bottom * src_rect.bottom + (1.0f - bottom) * src_rect.top;

	return new_rect;
}

D2D1_RECT_F D2DRendererBase::MakeRectRatios( D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect )
{
	return MakeRectRatios(src_rect, ratio_rect.left, ratio_rect.top, ratio_rect.right, ratio_rect.bottom);
}

D2D1_POINT_2F D2DRendererBase::MakePointRatio( D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio )
{
	D2D1_POINT_2F new_pt;

	new_pt.x = ratio * pt1.x, + (1.0f - ratio) * pt0.x;
	new_pt.y = ratio * pt1.y, + (1.0f - ratio) * pt0.y;

	return new_pt;
}

void D2DRendererBase::Invalidate( void )
{
	pImpl->Invalidate();
}

D2D1_SIZE_F F35_NS::D2DRendererBase::GetSize( void ) const
{
	return pImpl->GetSize();
}

D2D1_RECT_F F35_NS::D2DRendererBase::GetRectangle( void ) const
{
	D2D1_RECT_F rect;
	D2D1_SIZE_F size = pImpl->GetSize();
	rect.left = rect.top = 0.0f;
	rect.right = size.width;
	rect.bottom = size.height;
	return rect;
}

ID2D1StrokeStyle * F35_NS::D2DRendererBase::MakeStrokeStyle( FLOAT dashes, UINT32 dashesCount )
{
	return pImpl->GetStrokeStyle(dashes, dashesCount);
}

ID2D1PathGeometry * F35_NS::D2DRendererBase::MakePathGeometry( void )
{
	return pImpl->GetPathGeometry();
}
