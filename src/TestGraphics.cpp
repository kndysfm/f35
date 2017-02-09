#include "stdafx.h"
#include "TestGraphics.h"

USING_F35_NS;

struct TestGraphics::Impl
{
	ResourceHolder<IDWriteTextFormat> textf;
	ResourceHolder<ID2D1SolidColorBrush> brush;

	Impl(void): textf(NULL), brush(NULL)
	{

	}

	~Impl(void)
	{
	}
};

TestGraphics::TestGraphics(void):
	pImpl(new Impl)
{
}


TestGraphics::~TestGraphics(void)
{
	delete pImpl;
}

BOOL F35_NS::TestGraphics::InternalRender( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs )
{
	D2D1_POINT_2F pos = GetPosition();
	D2D1_RECT_F rect;
	rect.left = pt_abs.x; rect.right = rect.left + 100.0f;
	rect.top = pt_abs.y; rect.bottom = rect.top + 20.0f;
	if (pImpl->textf && pImpl->brush)
	{
		renderer->Print(pImpl->textf, rect, pImpl->brush, _T("(%.0f,%.0f)"), pos.x, pos.y);
	}

	return TRUE;
}

void F35_NS::TestGraphics::InitGraphics( D2DRendererBase * renderer )
{
	GraphicsBase::InternalInit(renderer);
	pImpl->textf = renderer->MakeTextFormat(_T("MS UI Gothic"), 12.0f);
	pImpl->brush = renderer->MakeBrush(D2D1::ColorF(D2D1::ColorF::White, 0.3F));
}

void F35_NS::TestGraphics::UpdateGraphics( D2DRendererBase * renderer )
{
	GraphicsBase::InternalUpdate(renderer);

}

void F35_NS::TestGraphics::DestroyGraphics( D2DRendererBase * renderer )
{
	GraphicsBase::InternalDestroy(renderer);
}
