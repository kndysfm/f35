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

		void SetImageClipRect(D2D1_RECT_U rect);
		
		BOOL SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt = Factory::IFF_PNG);

	private:
		virtual void InternalInit(RendererBase * renderer) sealed;

		virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target) sealed;

	protected:
		virtual BOOL InternalRenderImage(ID2D1RenderTarget * target) = 0;

	};
}

