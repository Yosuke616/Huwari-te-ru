#include "TexScrollComponent.h"
#include "Draw2dComponent.h"

#include "Camera.h"

#define RATE_CHASE_SCROLL	(0.1f)

// �ÓI�����o�ϐ�
bool CTexScroll::m_bScroll = true;

// �R���X�g���N�^
CTexScroll::CTexScroll()
	: m_pDraw2D(nullptr)
	, m_pPosX(nullptr), m_pPosY(nullptr)
{
	m_Scroll = XMFLOAT2(0.0f, 0.0f);
	m_UVscroll = XMFLOAT2(0.0f, 0.0f);
	m_ScrollValue = XMFLOAT2(0.0f, 0.0f);
}

// �f�X�g���N�^
CTexScroll::~CTexScroll(){
}

// ������
void CTexScroll::Start()
{
	//	�`��N���X�̃|�C���^�̎擾
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}
// �X�V
void CTexScroll::Update()
{
	// �X�N���[�����Ȃ��ꍇ
	if (!m_bScroll)
	{
		// ���݂�UV�ړ��ʂ��Z�b�g���ďI���
		m_pDraw2D->SetScroll(m_UVscroll);
		return;
	}

	// X�����̃X�N���[������ݒ肳��Ă�����
	if (m_pPosX)
	{
		// ���݂̃X�N���[���ʂƖړI�̃X�N���[���ʂ̍��������߂�
		float differenceX = (*m_pPosX * m_ScrollValue.x) - m_Scroll.x;

		// ����̃X�N���[�� = ���܂ł̃X�N���[���� + ���� * ����
		m_Scroll.x = m_Scroll.x + differenceX * RATE_CHASE_SCROLL;

		// �����_�������o��
		m_UVscroll.x = m_Scroll.x - (int)m_Scroll.x;
	}
	// Y�����̃X�N���[������ݒ肳��Ă�����
	if (m_pPosY)
	{
		//// ���݂̃X�N���[���ʂƖړI�̃X�N���[���ʂ̍��������߂�
		//float differenceY = (*m_pPosY * m_ScrollValue.y) - m_Scroll.y;
		//
		//// ����̃X�N���[�� = ���܂ł̃X�N���[���� + ���� * ����
		//m_Scroll.y = m_Scroll.y + differenceY * RATE_CHASE_SCROLL;
		//
		//// �����_�������o��
		//m_UVscroll.y = m_Scroll.y - (int)m_Scroll.y;

		m_UVscroll.y = *m_pPosY;
	}

	// �e�N�X�`����UV�ړ��ʂ��Z�b�g
	m_pDraw2D->SetScroll(m_UVscroll);
}

// �`��
void CTexScroll::Draw()
{
}

void CTexScroll::SetAxizX(float* px)
{
	m_pPosX = px;
}
void CTexScroll::SetAxizY(float* py)
{
	m_pPosY = py;
}

void CTexScroll::SetScrollValue(float fx)
{
	m_ScrollValue.x = fx;
}

void CTexScroll::SetScrollValueY(float fy)
{
	m_ScrollValue.y = fy;
}

void  CTexScroll::Scroll(bool bScroll)
{
	m_bScroll = bScroll;
}