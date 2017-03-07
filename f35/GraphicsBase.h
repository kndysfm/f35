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

		virtual BOOL AttachRenderTarget(ID2D1RenderTarget * target) = 0;

		virtual void DettachRenderTarget(void) = 0;

		virtual ID2D1RenderTarget *GetRenderTarget(void) const = 0;

		virtual void Init(ID2D1RenderTarget * target) = 0;

		virtual void Update(void) = 0;

		virtual BOOL Render(ID2D1RenderTarget * target) = 0;

		virtual void Destroy(void) = 0;

	};

	class GraphicsBase : H::NonCopyable, protected IGraphics
	{

	private:
		struct Impl;
		Impl *pImpl;

		virtual BOOL RegisterContainer(IGraphicsContainer const *parent) sealed;

		virtual void DeregisterContainer(IGraphicsContainer const *parent) sealed;

		virtual IGraphicsContainer const *GetContainer(void) const sealed;

		virtual ID2D1RenderTarget *GetRenderTarget(void) const sealed;

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

		virtual void Init(ID2D1RenderTarget * target) sealed;

		virtual void Update(void) sealed;

		virtual BOOL Render(ID2D1RenderTarget * target) sealed;

		virtual void Destroy(void) sealed;

	protected:

		virtual BOOL AttachRenderTarget(ID2D1RenderTarget * target);

		virtual void DettachRenderTarget(void);

		virtual void InternalInit(ID2D1RenderTarget * target) { }

		virtual void InternalUpdate( void) { };

		virtual BOOL InternalRender(ID2D1RenderTarget * target) = 0;

		virtual void InternalDestroy( void ) { }

	};

}