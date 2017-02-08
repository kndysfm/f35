#include "stdafx.h"
#include "TestGraphic.h"

USING_F35_NS;

struct TestGraphic::Impl
{
	IDWriteTextFormat *textf;
	ID2D1Brush *brush;

	Impl(void): textf(NULL), brush(NULL)
	{

	}

	~Impl(void)
	{
		D2DRendererBase::SafeRelease(&textf);
		D2DRendererBase::SafeRelease(&brush);
	}
};

TestGraphic::TestGraphic(void):
	pImpl(new Impl)
{
}


TestGraphic::~TestGraphic(void)
{
	delete pImpl;
}

BOOL F35_NS::TestGraphic::RenderGraphic( D2DRendererBase * renderer, ID2D1RenderTarget * target, const D2D1_POINT_2F &pt_abs )
{
	D2D1_POINT_2F pos = GetPosition();
	D2D1_RECT_F rect;
	rect.left = pt_abs.x; rect.right = rect.left + 30.0f;
	rect.top = pt_abs.y; rect.bottom = rect.top + 10.0f;
	if (pImpl->textf && pImpl->brush)
		renderer->Print(pImpl->textf, rect, pImpl->brush, _T("(%.1f,%.1f)"), pos.x, pos.y);

	return TRUE;
}

void F35_NS::TestGraphic::InitGraphic( D2DRendererBase * renderer )
{
	GraphicsBase::InitGraphic(renderer);
	pImpl->textf = renderer->MakeTextFormat(_T("MS UI Gothic"), 7.0f);
	pImpl->brush = renderer->MakeBrush(D2D1::ColorF(D2D1::ColorF::Gray));
}

void F35_NS::TestGraphic::UpdateGraphic( D2DRendererBase * renderer )
{
	GraphicsBase::UpdateGraphic(renderer);

}

void F35_NS::TestGraphic::DestroyGraphic( D2DRendererBase * renderer )
{
	D2DRendererBase::SafeRelease(&pImpl->textf);
	D2DRendererBase::SafeRelease(&pImpl->brush);
	GraphicsBase::DestroyGraphic(renderer);
}
