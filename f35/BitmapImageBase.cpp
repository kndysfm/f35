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

	BOOL Resize(BitmapImageBase *that, D2D1_SIZE_U size)
	{
		HRESULT hr = S_OK;
		bitmap_size_ = size;

		H::R<ID2D1Bitmap> pBmp;
		if (pWICBitmap_)
		{
			ID2D1Bitmap *ptr = NULL;
			hr = pBitmapRenderTarget_->CreateBitmapFromWicBitmap(pWICBitmap_, &ptr);
			if (SUCCEEDED(hr)) pBmp = ptr;
			else if (ptr) ptr->Release();
		}

		pWICBitmap_ = Factory::MakeWICBitmap(size.width, size.height);

		if (pWICBitmap_)
		{
			pBitmapRenderTarget_ = Factory::MakeWicBitmapRenderTarget(pWICBitmap_);
			if (pBitmapRenderTarget_)
			{
				that->InitImage(pBitmapRenderTarget_);
				if (pBmp)
				{
					pBitmapRenderTarget_->BeginDraw();
					pBitmapRenderTarget_->DrawBitmap(pBmp);
					pBitmapRenderTarget_->EndDraw();
				}
			}
		}
		else pBitmapRenderTarget_ = nullptr;

		return pBitmapRenderTarget_? TRUE: FALSE;
	}

	BOOL Render(BitmapImageBase *that, ID2D1RenderTarget * target)
	{
		if (!pBitmapRenderTarget_) return FALSE;
		
		pBitmapRenderTarget_->BeginDraw();
		that->RenderImage(pBitmapRenderTarget_);
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
	pImpl->Resize(this, size);
}

D2D1_SIZE_U F35_NS::BitmapImageBase::GetImageSize(void)
{
	return pImpl->bitmap_size_;
}

void F35_NS::BitmapImageBase::SetImageClipRect(D2D1_RECT_U rect)
{
	pImpl->shown_rect_ = rect;
}

D2D1_RECT_U F35_NS::BitmapImageBase::GetImageClipRect(void)
{
	return pImpl->shown_rect_;
}

BOOL F35_NS::BitmapImageBase::SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
{
	return pImpl->SaveFile(filename, fmt);
}

void F35_NS::BitmapImageBase::InternalInit(ID2D1RenderTarget * target)
{
	D2D1_SIZE_F size = target->GetSize();
	pImpl->shown_rect_ = D2D1::RectU(0, 0, size.width, size.height);
	pImpl->Resize(this, D2D1::SizeU(size.width, size.height));
}

void F35_NS::BitmapImageBase::InternalUpdate(void)
{
	this->UpdateImage();
}

BOOL F35_NS::BitmapImageBase::InternalRender(ID2D1RenderTarget * target)
{
	return pImpl->Render(this, target);
}

void F35_NS::BitmapImageBase::InternalDestroy(void) 
{
	this->DestroyImage();
}
