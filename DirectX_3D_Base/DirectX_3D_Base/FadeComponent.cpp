/**
 * @file	FadeComponent.cpp
 * @brief	�G�߂̃��S���o������ɏ��X�ɏ����Ă���
 * @author	�u���Č�
 * @date	2021/12/22
 */

//===== �C���N���[�h =====
#include "FadeComponent.h"
#include "Draw2dComponent.h"

// ===== �}�N����` =====
#define START_FADE_LOGO		(180)			// �t�F�[�h�J�n����

#define ALPHA_ADDITION		(0.01f)			// ���Z(���l)
#define ALPHA_SUBTRACTION	(0.01f)			// ���Z(���l)

#define UV_ADDITION			(0.01f)			// ���Z(UV���W)
#define UV_SUBTRACTION		(0.01f)			// ���Z(UV���W)

#define SCROLL_ADDITION		(0.01f)			// ���Z(�X�N���[�����W)

// �R���X�g���N�^
CFade::CFade()
	: m_pDraw2D(nullptr)
	, m_fTime(0.0f)
	, m_fAlpha(0.0f)
	, m_vPos(XMFLOAT2(0.0f, 1.0f))
	, m_vScrPos(XMFLOAT2(0.0f, 0.0f))
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}

// �f�X�g���N�^
CFade::~CFade()
{

}

// ������
void CFade::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// �X�V
void CFade::Update()
{
	m_fTime++;									// �o�ߎ��ԍX�V
	if (m_fTime < START_FADE_LOGO)				// �G�߂̃��S��`��
	{
		m_fAlpha += ALPHA_ADDITION;				// ���l���Z
		m_vPos.x += UV_ADDITION;				// UV���W(x)�����Z
		if (m_fAlpha >= 1.0f)					// ���l�␳
			m_fAlpha = 1.0f;
		if (m_vPos.x >= 1.0f)					// UV���W�␳
			m_vPos.x = 1.0f;
	}
	if (m_fTime >= START_FADE_LOGO)				// �G�߂̃��S�������Ă���
	{
		m_fAlpha -= ALPHA_SUBTRACTION;			// ���l���炷
		m_vPos.x -= UV_SUBTRACTION;				// UV���W(x)�����Z
		m_vScrPos.x += SCROLL_ADDITION;			// �X�N���[�����W(x)�����Z
		if (m_vPos.x <= 0.0f)					// UV���W�␳
			m_vPos.x = 0.0f;
		if (m_vScrPos.x >= 1.0f)				// �X�N���[�����W�␳
			m_vScrPos.x = 1.0f;
		if (m_fAlpha <= 0.0f)					// ���l��0.0f�𒴂�����
			Parent->Delete();					// �R���|�[�l���g���폜����
	}
	
	m_pDraw2D->SetUVsize(m_vPos.x, m_vPos.y);	// UV���W�ݒ�
	m_pDraw2D->SetScroll(m_vScrPos);			// �X�N���[���ݒ�
	m_pDraw2D->SetAlpha(m_fAlpha);				// ���l�ݒ�
}

// �`��
void CFade::Draw()
{

}