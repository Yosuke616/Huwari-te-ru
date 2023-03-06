/**
 * @file	FlashComponent.h
 * @brief	�_��(���l)��������
 * @author	AT12A �u���Č�
 * @date	2022/01/28
 */

//===== �C���N���[�h =====
#include "FlashComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"

//===== �}�N����` =====
#define ALPHA_RATE	(0.01f)

// �R���X�g���N�^
CFlash::CFlash()
	: m_pTransform(nullptr)
	, m_pDraw2D(nullptr)
	, m_fAlpha(1.0f)
	, m_nDrawTime(0)
	, m_nNoneTime(0)
	, m_nKeepAlpha(0)
	, m_nKeepNone(0)
{

}

// �f�X�g���N�^
CFlash::~CFlash()
{

}

// ������
void CFlash::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();	// ���W
	m_pDraw2D = Parent->GetComponent<CDraw2D>();		// �`��
}

// �X�V
void CFlash::Update()
{
	// �`�悵�Ȃ�
	if (m_nNoneTime > 0)
	{
		m_nNoneTime--;							// ���Ԍ��Z
		if (m_nNoneTime == 0)					// �`�悵�Ȃ����Ԃ������Ȃ����Ƃ�
		{
			m_nNoneTime = 0;					// �`�悵�Ȃ��t���O�폜
			m_nDrawTime = m_nKeepAlpha;
			//m_nDrawTime = rand() % 600 + 600;	// �`�掞�Ԏw��
		}
		m_fAlpha -= ALPHA_RATE;
		if (m_fAlpha < 0.0f)					// ���l�␳
			m_fAlpha = 0.0f;					// ��
		m_pDraw2D->SetAlpha(m_fAlpha);			// ���l�Ȃ�
		return;									// �ȍ~�̏����������Ȃ�
	}
	
	// �`�悷��
	if (m_nDrawTime > 0)
	{
		m_nDrawTime--;							// �`�掞�Ԍ��Z
		if (m_nDrawTime == 0)					// �`�掞�Ԃ������Ȃ����Ƃ�
		{
			m_nDrawTime = 0;					// �`�掞�ԂȂ�
			m_nNoneTime = m_nKeepNone;
			//m_nNoneTime = rand() % 600 + 600;	// �`�悵�Ȃ����Ԑݒ�
		}
		m_fAlpha += ALPHA_RATE;					// ���l���Z
		if (m_fAlpha >= 1.0f)					// ���l�␳
			m_fAlpha = 1.0f;					// ��
		m_pDraw2D->SetAlpha(m_fAlpha);			// ���l�ݒ�
	}

}

// �`��
void CFlash::Draw()
{

}

// �Z�b�g�֐�
void CFlash::SetAlphaTime(int nTime)
{
	m_nDrawTime = nTime * 60;		// �`�掞�Ԑݒ�
	m_nKeepAlpha = nTime * 60;		// �`�掞�ԕۑ��p
}

// �Z�b�g�֐�
void CFlash::SetNoneTime(int nTime)
{
	m_nNoneTime = nTime * 60;		// �`�悵�Ȃ����Ԑݒ�
	m_nKeepNone = nTime * 60;		// �`�悵�Ȃ����ԕۑ��p
}