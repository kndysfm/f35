#include "stdafx.h"
#include "TestGraphics.h"

USING_F35_NS;

struct TestGraphics::Impl
{
	H::R<IDWriteTextFormat> textf;
	H::R<ID2D1SolidColorBrush> brush;

	Impl(void)
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

BOOL F35_NS::TestGraphics::InternalRender( RendererBase * renderer, ID2D1RenderTarget * target )
{
	if (pImpl->textf && pImpl->brush)
	{
		D2D1_POINT_2F pos = GetPosition();
		D2D1_RECT_F rect = D2D1::RectF(0,0,80,20);
		H::WriteText(target, pImpl->textf, rect, pImpl->brush, _T("(%.0f,%.0f)"), pos.x, pos.y);
	}

	return TRUE;
}

void F35_NS::TestGraphics::InternalInit( RendererBase * renderer )
{
	GraphicsBase::InternalInit(renderer);
	pImpl->textf = Factory::MakeTextFormat(_T("MS UI Gothic"), 12.0f);
	pImpl->brush = renderer->MakeBrush(D2D1::ColorF(D2D1::ColorF::White, 0.3F));
}

void F35_NS::TestGraphics::InternalUpdate( RendererBase * renderer )
{
	GraphicsBase::InternalUpdate(renderer);

}

void F35_NS::TestGraphics::InternalDestroy( RendererBase * renderer )
{
	GraphicsBase::InternalDestroy(renderer);
}
