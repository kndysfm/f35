#include "stdafx.h"
#include "BitmapImageBase.h"
#include <wincodec.h>


class F35_NS::BitmapImageBase::Impl
{
	H::R<IWICBitmap> pWICBitmap_;
	H::R<ID2D1Bitmap> pShownBitmap_;
	H::R<ID2D1RenderTarget> pBitmapRenderTarget_;
	D2D1_SIZE_U bitmap_size_;
	D2D1_RECT_U clipping_rect_;

public:
	D2D1_SIZE_U GetBitmapSize(void) { return bitmap_size_; }

	void SetBitmapSize(BitmapImageBase *that, D2D1_SIZE_U size)
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
	}

	D2D1_RECT_U GetClippingRect(void) { return clipping_rect_; }

	ID2D1RenderTarget const *GetTarget(void) { return pBitmapRenderTarget_; }

	void SetClippingRect(BitmapImageBase *that, D2D1_RECT_U rect)
	{
		clipping_rect_ = rect;

		if (!pBitmapRenderTarget_) return;

		D2D1_BITMAP_PROPERTIES prop;
		pBitmapRenderTarget_->GetDpi(&prop.dpiX, &prop.dpiY);
		prop.pixelFormat = pBitmapRenderTarget_->GetPixelFormat();
		ID2D1Bitmap *ptr = NULL;
		HRESULT hr = pBitmapRenderTarget_->CreateBitmap(
			D2D1::SizeU(clipping_rect_.right - clipping_rect_.left, clipping_rect_.bottom - clipping_rect_.top),
			//D2D1::SizeU(size_f.width, size_f.height),
			prop, &ptr);
		if (SUCCEEDED(hr)) pShownBitmap_ = ptr;
		else if (ptr) ptr->Release();
	}


	BOOL Render(BitmapImageBase *that, ID2D1RenderTarget * target)
	{
		if (!pBitmapRenderTarget_) return FALSE;
		
		pBitmapRenderTarget_->BeginDraw();
		that->RenderImage(pBitmapRenderTarget_);
		HRESULT hr = pBitmapRenderTarget_->EndDraw();

		if (SUCCEEDED(hr) && pShownBitmap_)
		{
			D2D1_POINT_2U pt = D2D1::Point2U();
			hr = pShownBitmap_->CopyFromRenderTarget(&pt,  pBitmapRenderTarget_, &clipping_rect_);
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
	pImpl->SetBitmapSize(this, size);
}

D2D1_SIZE_U F35_NS::BitmapImageBase::GetImageSize(void)
{
	return pImpl->GetBitmapSize();
}

void F35_NS::BitmapImageBase::SetImageClipRect(D2D1_RECT_U rect)
{
	pImpl->SetClippingRect(this, rect);
}

D2D1_RECT_U F35_NS::BitmapImageBase::GetImageClipRect(void)
{
	return pImpl->GetClippingRect();
}

BOOL F35_NS::BitmapImageBase::SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt)
{
	return pImpl->SaveFile(filename, fmt);
}

void F35_NS::BitmapImageBase::InternalInit(ID2D1RenderTarget * target)
{
	D2D1_SIZE_F size = target->GetSize();
	if (pImpl->GetTarget() == NULL) pImpl->SetBitmapSize(this, D2D1::SizeU(size.width, size.height));
	pImpl->SetClippingRect(this, D2D1::RectU(0, 0, size.width, size.height));
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
