#pragma once
#include "GraphicsBase.h"


namespace F35_NS
{
	class BitmapImageBase :
		public GraphicsBase
	{
	private:
		struct Impl;
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
		virtual void InternalInit(RendererBase * renderer) sealed;

		virtual void InternalUpdate(RendererBase * renderer) sealed;

		virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target) sealed;

		virtual void InternalDestroy(RendererBase * renderer) sealed;

	protected:
		virtual void InternalUpdateImage(void) = 0;

		virtual BOOL InternalRenderImage(ID2D1RenderTarget * target) = 0;

	};
}

