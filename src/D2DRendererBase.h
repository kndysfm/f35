#pragma once
#include "F35Helper.h"

#include <dwrite.h>

namespace F35_NS
{

class D2DRendererBase: Lockable, NonCopyable
{

private:
	class Impl;
	Impl *pImpl;

public:

	/*! \fn D2DRendererBase::PaintRectangle
	 *  \brief 直線で長方形を描いて中を塗りつぶす
	 *  \param const D2D1_RECT_F & rect
	 *  \param ID2D1Brush * fill_brush
	 *  \param ID2D1Brush * line_brush
	 *  \param FLOAT line_width
	 *  \return void
	 */
	void PaintRectangle(const D2D1_RECT_F &rect, ID2D1Brush *fill_brush, ID2D1Brush *line_brush, FLOAT line_width = 1.0f);

	/*! \fn D2DRendererBase::Print
	 *  \brief 文字列描画の簡素化
	 *  \param IDWriteTextFormat * format
	 *  \param const D2D1_RECT_F & rect
	 *  \param ID2D1Brush * brush
	 *  \param LPCTSTR str_fmt
	 *  \param ...
	 *  \return void
	 */
	void Print(IDWriteTextFormat *format, const D2D1_RECT_F &rect, ID2D1Brush *brush, LPCTSTR str_fmt, ...);

	/*! \fn D2DRendererBase::Print
	 *  \brief 
	 *  \param IDWriteTextFormat * format
	 *  \param const D2D1_RECT_F & rect
	 *  \param DWRITE_TEXT_ALIGNMENT text_align
	 *  \param DWRITE_PARAGRAPH_ALIGNMENT paragraph_align
	 *  \param ID2D1Brush * brush
	 *  \param LPCTSTR str_fmt
	 *  \param ...
	 *  \return void
	 */
	void Print(IDWriteTextFormat *format, const D2D1_RECT_F &rect, 
		DWRITE_TEXT_ALIGNMENT text_align, DWRITE_PARAGRAPH_ALIGNMENT paragraph_align,
		ID2D1Brush *brush, LPCTSTR str_fmt, ...);

	/*! \fn D2DRendererBase::MakeTextFormat
	 *  \brief TextFormatオブジェクトの生成
	 *  \param LPCTSTR fontName
	 *  \param FLOAT fontSize
	 *  \param DWRITE_TEXT_ALIGNMENT textAlign
	 *  \param DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign
	 *  \return IDWriteTextFormat *
	 */
	ResourceHolder<IDWriteTextFormat> MakeTextFormat(LPCTSTR fontName, FLOAT fontSize,
		DWRITE_TEXT_ALIGNMENT textAlign = DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	/*! \fn F35LIB_NAMESPACE::D2DRendererBase::MakeStrokeStyle
	 *  \brief 
	 *  \param FLOAT dashes
	 *  \param UINT32 dashesCount
	 *  \return ID2D1StrokeStyle *
	 */
	ResourceHolder<ID2D1StrokeStyle> MakeStrokeStyle(  FLOAT dashes, UINT32 dashesCount );
	
	/*! \fn D2DRendererBase::MakeBrush
	 *  \brief ブラシオブジェクトの生成
	 *  \param D2D1::ColorF color
	 *  \return ID2D1Brush *
	 */
	ResourceHolder<ID2D1SolidColorBrush> MakeBrush(const D2D1::ColorF &color);

	
	/*! \fn F35LIB_NAMESPACE::D2DRendererBase::MakePathGeometry
	 *  \brief 
	 *  \param void
	 *  \return ID2D1PathGeometry *
	 */
	ResourceHolder<ID2D1PathGeometry> MakePathGeometry(void);

	/*! \fn D2DRendererBase::GetCurrentCursorPosDpi
	 *  \brief 現在のカーソル位置を取得
	 *  \param void
	 *  \return D2D1_POINT_2F
	 */
	D2D1_POINT_2F GetCurrentCursorPosDpi(void);

	/*! \fn F35LIB_NAMESPACE::D2DRendererBase::GetSize
	 *  \brief 
	 *  \param void
	 *  \return D2D1_SIZE_F
	 */
	D2D1_SIZE_F GetSize(void) const;


	/*! \fn F35LIB_NAMESPACE::D2DRendererBase::GetRectangle
	 *  \brief 
	 *  \param void
	 *  \return D2D1_RECT_F
	 */
	D2D1_RECT_F GetRectangle(void) const;


protected:

	/*! \fn D2DRendererBase::InternalInit
	 *  \brief 継承クラスにおける初期化処理
	 *  \param ID2D1RenderTarget * target
	 *  \return HRESULT
	 */
	virtual HRESULT InternalInit(ID2D1RenderTarget *target) { return S_OK; }

	/*! \fn D2DRendererBase::InternalUpdate
	 *  \brief 継承クラスにおけるデータ更新
	 *  \param void
	 *  \return void
	 */
	virtual void InternalUpdate(void) { return; }

	/*! \fn D2DRendererBase::InternalRender
	 *  \brief 継承クラスにおける画面描画処理
	 *  \param ID2D1RenderTarget * target
	 *  \return HRESULT
	 */
	virtual HRESULT InternalRender(ID2D1RenderTarget *target) { return S_OK; }

	/*! \fn D2DRendererBase::InternalDestroy
	 *  \brief 継承クラスにおけるリソース破棄処理
	 *  \param void
	 *  \return void
	 */
	virtual void InternalDestroy(void) { return; }

	/*! \fn D2DRendererBase::OnKeyDown
	 *  \brief VirtualKey状態に変化があったら呼ばれる
	 *  \param INT vk_code
	 *  \return void
	 */
	virtual void OnKeyDown(INT vk_code) { }

	/*! \fn D2DRendererBase::OnKeyUp
	 *  \brief VirtualKey状態に変化があったら呼ばれる
	 *  \param INT vk_code
	 *  \return void
	 */
	virtual void OnKeyUp(INT vk_code) { }

public:
	/*! \fn D2DRendererBase::D2DRendererBase
	 *  \brief コンストラクタ
	 *  \param HWND hwnd
	 *  \return 
	 */
	D2DRendererBase(HWND hwnd);

	/*! \fn D2DRendererBase::~D2DRendererBase
	 *  \brief デストラクタ
	 *  \param void
	 *  \return 
	 */
	virtual ~D2DRendererBase(void);

	/*! \fn D2DRendererBase::Init
	 *  \brief 初期化処理
	 *  \param void
	 *  \return HRESULT
	 */
	HRESULT Init(void);

	/*! \fn D2DRendererBase::Update
	 *  \brief 画面描画前のデータ更新
	 *  \param void
	 *  \return void
	 */
	void Update(void);

	/*! \fn D2DRendererBase::Render
	 *  \brief 画面描画処理
	 *  \param void
	 *  \return HRESULT
	 */
	HRESULT Render(void);

	void Invalidate(void);

	/*! \fn D2DRendererBase::Destroy
	 *  \brief リソースの廃棄
	 *  \param void
	 *  \return void
	 */
	void Destroy(void);
};

}

