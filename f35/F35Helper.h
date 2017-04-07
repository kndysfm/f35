#pragma once

#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <memory>

#define F35_NS			fxxxv
#define USING_F35_NS	using namespace F35_NS

namespace F35_NS
{
	namespace H
	{
		class NonCopyable
		{
		protected:
			NonCopyable() {}
			virtual ~NonCopyable() {}
		private:
			NonCopyable(const NonCopyable &) = delete;
			NonCopyable(NonCopyable &&) = delete;
			NonCopyable & operator= (const NonCopyable &) = delete;
			NonCopyable & operator= (NonCopyable &&) = delete;
		};

		class Lockable
		{
		private:
			CRITICAL_SECTION cs;
		protected:
			Lockable() { ::InitializeCriticalSection(&cs); }
			virtual ~Lockable()
			{
				::EnterCriticalSection(&cs);
				::DeleteCriticalSection(&cs);
			}
			void Lock(void) { ::EnterCriticalSection(&cs); }
			void Unlock(void) { ::LeaveCriticalSection(&cs); }
			BOOL TryLock(void) { return ::TryEnterCriticalSection(&cs); }
		};

		//! Resource Holder class for Direct2D objects
		template<class Interface>
		class R
		{
		private:
			Interface *pResource;
			explicit R(const R &) = delete;
			explicit R(Interface const *p) = delete;
			R & operator= (const R &) = delete;

			static void safeRelease(Interface **ppInterfaceToRelease)
			{
				if (*ppInterfaceToRelease != NULL)
				{
					(*ppInterfaceToRelease)->Release();
					(*ppInterfaceToRelease) = NULL;
				}
			}

		public:
			R() : R(nullptr) { }
			explicit R(R && rh) : R(rh.pResource) { rh.pResource = NULL; }
			R(nullptr_t ptr) : pResource(NULL) { }
			R(Interface *p) : pResource(p) { }

			R & operator= (R && rh)
			{
				safeRelease(&this->pResource);
				this->pResource = rh.pResource; // move value
				rh.pResource = NULL; // prevent release
				return *this;
			}

			R & operator= (Interface*p)
			{
				safeRelease(&this->pResource);
				this->pResource = p;
				return *this;
			}

			operator bool() { return pResource != NULL; }

			operator BOOL() { return pResource != NULL; }

			Interface * const * operator& () const { return &pResource; }

			Interface * operator->() { return pResource; }

			Interface const * operator->() const { return pResource; }

			operator Interface *() { return pResource; }

			operator const Interface *() const { return pResource; }

			virtual ~R() { safeRelease(&pResource); }
		};

		D2D1_RECT_F MakeRectRatios(D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

		D2D1_RECT_F MakeRectRatios(D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect);

		D2D1_POINT_2F MakePointRatio(D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio);

		void PaintRectangle(ID2D1RenderTarget *target, const D2D1_RECT_F &rect, ID2D1Brush *fill_brush, ID2D1Brush *line_brush, FLOAT line_width = 1.0f);

		void WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect, ID2D1Brush *brush, LPCTSTR str_fmt, ...);

		void WriteText(ID2D1RenderTarget *target, IDWriteTextFormat *format, const D2D1_RECT_F &rect,
			DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align,
			ID2D1Brush *brush, LPCTSTR str_fmt, ...);

		H::R<ID2D1SolidColorBrush> MakeSolidColorBrush(ID2D1RenderTarget *target, const D2D1_COLOR_F &color);

		H::R<ID2D1Layer> MakeLayer(ID2D1RenderTarget *target);

	}

	class Factory
	{
	private:
		struct Impl;
		static std::unique_ptr<Impl> pImpl;

		Factory() = delete;
		Factory(Factory const &) = delete;
		Factory(Factory &&) = delete;
		Factory & operator=(Factory const &) = delete;
		Factory & operator=(Factory &&) = delete;

	public:

		static HRESULT Init(void);

		static void Finalize(void);

		static H::R<ID2D1HwndRenderTarget> MakeHwndRenderTarget(HWND hwnd, D2D1_SIZE_U size);

		/*! \fn Factory::MakeTextFormat
		*  \brief TextFormatオブジェクトの生成
		*  \param LPCTSTR fontName
		*  \param FLOAT fontSize
		*  \param DWRITE_TEXT_ALIGNMENT textAlign
		*  \param DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign
		*  \return IDWriteTextFormat *
		*/
		static H::R<IDWriteTextFormat> MakeTextFormat(LPCTSTR fontName, FLOAT fontSize,
			DWRITE_TEXT_ALIGNMENT textAlign = DWRITE_TEXT_ALIGNMENT_CENTER,
			DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		/*! \fn Factory::MakeStrokeStyle
		*  \brief
		*  \param FLOAT dashes
		*  \param UINT32 dashesCount
		*  \return ID2D1StrokeStyle *
		*/
		static H::R<ID2D1StrokeStyle> MakeStrokeStyle(FLOAT const *dashes, UINT32 dashesCount);

		/*! \fn Factory::MakePathGeometry
		*  \brief
		*  \param void
		*  \return ID2D1PathGeometry *
		*/
		static H::R<ID2D1PathGeometry> MakePathGeometry(void);

		static H::R<IWICBitmap> MakeWICBitmap(UINT width_px, UINT height_px);

		static H::R<ID2D1RenderTarget> MakeWicBitmapRenderTarget(IWICBitmap *pBmp);

		static H::R<IWICStream> MakeWicStream(void);

		enum ImageFileFormat {
			IFF_PNG, IFF_BMP, IFF_JPEG, IFF_GIF,
		};
		static H::R<IWICBitmapEncoder> MakeWicEncoder(ImageFileFormat fmt);
	};
}
