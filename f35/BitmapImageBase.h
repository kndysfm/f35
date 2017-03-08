#pragma once
#include "GraphicsBase.h"


namespace F35_NS
{
	class BitmapImageBase :
		public GraphicsBase
	{
	private:
		class Impl;
		Impl *pImpl;

	public:
		BitmapImageBase();
		BitmapImageBase(BitmapImageBase const &) = delete;
		BitmapImageBase(BitmapImageBase &&);
		BitmapImageBase & operator=(BitmapImageBase const &) = delete;
		BitmapImageBase & operator=(BitmapImageBase &&);

		~BitmapImageBase();

		void SetImageSize(D2D1_SIZE_U size);

		D2D1_SIZE_U GetImageSize(void);

		void SetImageClipRect(D2D1_RECT_U rect);

		D2D1_RECT_U GetImageClipRect(void);
		
		BOOL SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt = Factory::IFF_PNG);

	private:
		virtual void InternalInit(ID2D1RenderTarget * target) sealed;

		virtual void InternalUpdate(void) sealed;

		virtual BOOL InternalRender(ID2D1RenderTarget * target) sealed;

		virtual void InternalDestroy(void) sealed;

	protected:
		virtual void InitImage(ID2D1RenderTarget * target) { }

		virtual void UpdateImage(void) { }

		virtual BOOL RenderImage(ID2D1RenderTarget * target) = 0;

		virtual void DestroyImage(void) { }

	};
}

