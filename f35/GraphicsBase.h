#pragma once
#include "F35Helper.h"

#include "RendererBase.h"

namespace F35_NS
{
	class IGraphicsContainer;

	class IGraphics
	{
	public:

		virtual BOOL RegisterContainer(IGraphicsContainer const *parent) = 0;

		virtual void DeregisterContainer(IGraphicsContainer const *parent) = 0;

		virtual IGraphicsContainer const *GetContainer(void) const = 0;

		virtual BOOL AttachRenderer(RendererBase * renderer) = 0;

		virtual void DettachRenderer(void) = 0;

		virtual RendererBase *GetRenderer(void) const = 0;

		virtual void Init(RendererBase * renderer) = 0;

		virtual void Update(RendererBase * renderer) = 0;

		virtual BOOL Render(RendererBase * renderer, ID2D1RenderTarget * target) = 0;

		virtual void Destroy(RendererBase * renderer) = 0;

	};

	class GraphicsBase : H::NonCopyable, protected IGraphics
	{

	private:
		struct Impl;
		Impl *pImpl;

		virtual BOOL RegisterContainer(IGraphicsContainer const *parent) sealed;

		virtual void DeregisterContainer(IGraphicsContainer const *parent) sealed;

		virtual IGraphicsContainer const *GetContainer(void) const sealed;

		virtual RendererBase *GetRenderer(void) const sealed;

	public:
		GraphicsBase(void);
		virtual ~GraphicsBase(void);

		D2D1_POINT_2F GetPosition (void) const;

		void SetPosition (D2D1_POINT_2F pt);
		void SetPosition (FLOAT x, FLOAT y);

		D2D1_SIZE_F GetScale (void) const;

		void SetScale(FLOAT scale);
		void SetScale (D2D1_SIZE_F sz);
		void SetScale (FLOAT x, FLOAT y);

		FLOAT GetRotation(void) const;
		void SetRotation(FLOAT degrees);

		virtual void Init(RendererBase * renderer) sealed;

		virtual void Update(RendererBase * renderer) sealed;

		virtual BOOL Render(RendererBase * renderer, ID2D1RenderTarget * target) sealed;

		virtual void Destroy(RendererBase * renderer) sealed;

	protected:

		virtual BOOL AttachRenderer(RendererBase * renderer);

		virtual void DettachRenderer(void);

		virtual void InternalInit( RendererBase * renderer ) { }

		virtual void InternalUpdate( RendererBase * renderer) { };

		virtual BOOL InternalRender(RendererBase * renderer, ID2D1RenderTarget * target) = 0;

		virtual void InternalDestroy( RendererBase * renderer ) { }

	};

}