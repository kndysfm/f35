#pragma once
#include "F35Helper.h"

namespace F35_NS
{

class RendererBase: H::Lockable, H::NonCopyable
{

private:
	class Impl;
	Impl *pImpl;

public:
	
	/*! \fn D2DRendererBase::GetCurrentCursorPosDpi
	 *  \brief ���݂̃J�[�\���ʒu���擾
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


	D2D1_RECT_F GetScreenRect(void);


protected:

	/*! \fn D2DRendererBase::InternalInit
	 *  \brief �p���N���X�ɂ����鏉��������
	 *  \param ID2D1RenderTarget * target
	 *  \return HRESULT
	 */
	virtual HRESULT InternalInit(ID2D1RenderTarget *target) { return S_OK; }

	/*! \fn D2DRendererBase::InternalUpdate
	 *  \brief �p���N���X�ɂ�����f�[�^�X�V
	 *  \param void
	 *  \return void
	 */
	virtual void InternalUpdate(void) { return; }

	/*! \fn D2DRendererBase::InternalRender
	 *  \brief �p���N���X�ɂ������ʕ`�揈��
	 *  \param ID2D1RenderTarget * target
	 *  \return HRESULT
	 */
	virtual void InternalRender(ID2D1RenderTarget *target) { return; }

	/*! \fn D2DRendererBase::InternalDestroy
	 *  \brief �p���N���X�ɂ����郊�\�[�X�j������
	 *  \param void
	 *  \return void
	 */
	virtual void InternalDestroy(void) { return; }

	/*! \fn D2DRendererBase::OnKeyDown
	 *  \brief VirtualKey��Ԃɕω�����������Ă΂��
	 *  \param INT vk_code
	 *  \return void
	 */
	virtual void OnKeyDown(INT vk_code) { }

	/*! \fn D2DRendererBase::OnKeyUp
	 *  \brief VirtualKey��Ԃɕω�����������Ă΂��
	 *  \param INT vk_code
	 *  \return void
	 */
	virtual void OnKeyUp(INT vk_code) { }

public:
	/*! \fn D2DRendererBase::D2DRendererBase
	 *  \brief �R���X�g���N�^
	 *  \param HWND hwnd
	 *  \return 
	 */
	RendererBase(HWND hwnd);

	/*! \fn D2DRendererBase::~D2DRendererBase
	 *  \brief �f�X�g���N�^
	 *  \param void
	 *  \return 
	 */
	virtual ~RendererBase(void);

	/*! \fn D2DRendererBase::Init
	 *  \brief ����������
	 *  \param void
	 *  \return HRESULT
	 */
	HRESULT Init(void);

	/*! \fn D2DRendererBase::Update
	 *  \brief ��ʕ`��O�̃f�[�^�X�V
	 *  \param void
	 *  \return void
	 */
	void Update(void);

	/*! \fn D2DRendererBase::Render
	 *  \brief ��ʕ`�揈��
	 *  \param void
	 *  \return HRESULT
	 */
	HRESULT Render(void);

	void Invalidate(void);

	void Validate(void);

	/*! \fn D2DRendererBase::Destroy
	 *  \brief ���\�[�X�̔p��
	 *  \param void
	 *  \return void
	 */
	void Destroy(void);

	void EnableAutoErase(D2D1_COLOR_F color_to_erase = D2D1::ColorF(0));

	void DisableAutoErase(void);

	HRESULT SaveImageFile(LPCTSTR filename, Factory::ImageFileFormat fmt = Factory::IFF_PNG);

	void Resize(BOOL scaling = TRUE);
};

}

