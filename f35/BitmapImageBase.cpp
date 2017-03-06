#include "stdafx.h"
#include "BitmapImageBase.h"
#include <wincodec.h>


struct F35_NS::BitmapImageBase::Impl
{
	H::R<IWICBitmap> pWICBitmap_;
	H::R<ID2D1Bitmap> pShownBitmap_;
	H::R<ID2D1RenderTarget> pBitmapRenderTarget_;
	D2D1_SIZE_U bitmap_size_;
	D2D1_RECT_U shown_rect_;

	BOOL Resize(D2D1_SIZE_U size)
	{
		HRESULT hr = S_OK;
		bitmap_size_ = size;;
		pWICBitmap_ = Factory::MakeWICBitmap(size.width, size.height);
		pBitmapRenderTarget_ = pWICBitmap_? Factory::MakeWicBitmapRenderTarget(pWICBitmap_): NULL;

		return pBitmapRenderTarget_? TRUE: FALSE;
	}

	BOOL Render(BitmapImageBase *that, RendererBase * renderer, ID2D1RenderTarget * target)
	{
		if (!pBitmapRenderTarget_) return FALSE;
		
		pBitmapRenderTarget_->BeginDraw();
		that->InternalRenderImage(pBitmapRenderTarget_);
		HRESULT hr = pBitmapRenderTarget_->EndDraw();

		if (!SUCCEEDED(hr)) return FALSE;

		// clip bitmap for shown rectangle
		D2D1_BITMAP_PROPERTIES prop;
		pBitmapRenderTarget_->GetDpi(&prop.dpiX, &prop.dpiY);
		prop.pixelFormat = pBitmapRenderTarget_->GetPixelFormat();
		//D2D1_SIZE_F size_f = target->GetSize();
		ID2D1Bitmap *ptr = NULL;
		hr = pBitmapRenderTarget_->CreateBitmap(
			D2D1::SizeU(shown_rect_.right - shown_rect_.left, shown_rect_.bottom - shown_rect_.top),
			//D2D1::SizeU(size_f.width, size_f.height),
			prop, &ptr);
		pShownBitmap_ = ptr;
		if (SUCCEEDED(hr))
		{
			D2D1_POINT_2U pt = D2D1::Point2U();
			hr = pShownBitmap_->CopyFromRenderTarget(&pt,  pBitmapRenderTarget_, &shown_rect_);
			if (SUCCEEDED(hr))
			{
				target->DrawBitmap(pShownBitmap_);
				return TRUE;
			}
		}

		return FALSE;
	}

	HRESULT SaveFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
	{
		HRESULT hr = S_OK;
		H::R<IWICBitmapEncoder> pEncoder;
		H::R<IWICBitmapFrameEncode> pFrameEncode;
		H::R<IWICStream> pStream;

		if (SUCCEEDED(hr)) pStream = Factory::MakeWicStream();
		if (SUCCEEDED(hr) && pStream) hr = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
		if (SUCCEEDED(hr)) pEncoder = Factory::MakeWicEncoder(fmt);
		if (SUCCEEDED(hr) && pEncoder) hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
		if (SUCCEEDED(hr))
		{
			IWICBitmapFrameEncode *ptr = NULL;
			hr = pEncoder->CreateNewFrame(&ptr, NULL);
			pFrameEncode = ptr;
		}
		if (SUCCEEDED(hr)) hr = pFrameEncode->Initialize(NULL);
		if (SUCCEEDED(hr)) hr = pFrameEncode->SetSize(bitmap_size_.width, bitmap_size_.height);
		if (SUCCEEDED(hr))
		{
			WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
			hr = pFrameEncode->SetPixelFormat(&format);
		}
		if (SUCCEEDED(hr)) hr = pFrameEncode->WriteSource(pWICBitmap_, NULL);
		if (SUCCEEDED(hr)) hr = pFrameEncode->Commit();
		if (SUCCEEDED(hr)) hr = pEncoder->Commit();

		return hr;
	}

};

F35_NS::BitmapImageBase::BitmapImageBase() : pImpl(new Impl)
{
}

F35_NS::BitmapImageBase::BitmapImageBase(BitmapImageBase &&x) : pImpl(x.pImpl) { x.pImpl = nullptr; }

F35_NS::BitmapImageBase & F35_NS::BitmapImageBase::operator=(BitmapImageBase &&x)
{
	pImpl = x.pImpl;
	x.pImpl = nullptr;
	return *this;
}

F35_NS::BitmapImageBase::~BitmapImageBase()
{
	if (pImpl)
	{
		delete pImpl;
		pImpl = nullptr;
	}
}

void F35_NS::BitmapImageBase::SetImageSize(D2D1_SIZE_U size)
{
	pImpl->Resize(size);
}

void F35_NS::BitmapImageBase::SetImageClipRect(D2D1_RECT_U rect)
{
	pImpl->shown_rect_ = rect;
}

BOOL F35_NS::BitmapImageBase::SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
{
	return pImpl->SaveFile(filename, fmt);
}

void F35_NS::BitmapImageBase::InternalInit(RendererBase * renderer)
{
	D2D1_SIZE_F size = renderer->GetSize();
	pImpl->Resize(D2D1::SizeU(size.width, size.height));
	pImpl->shown_rect_ = D2D1::RectU(0, 0, size.width, size.height);
}

BOOL F35_NS::BitmapImageBase::InternalRender(RendererBase * renderer, ID2D1RenderTarget * target)
{
	return pImpl->Render(this, renderer, target);
}
