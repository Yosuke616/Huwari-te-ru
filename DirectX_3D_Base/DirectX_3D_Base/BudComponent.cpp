#include "BudComponent.h"
#include "Draw3dComponent.h"

// �R���X�g���N�^
CBud::CBud()
	: m_bCollision(false), m_bOldCollision(false), m_bOpen(false)
{
}

// �f�X�g���N�^
CBud::~CBud(){
}

// ������
void CBud::Start(){
}

// �X�V
void CBud::Update()
{
	// �����Փ˂��Ă�����
	if (m_bCollision)
	{
		// �O�t���[���ɓ������Ă��Ȃ�������
		if (m_bOldCollision == false)
		{
			// �A�j���[�V�����Đ�
			Parent->GetComponent<CDrawModel>()->Play(0);

			// �J���܂���
			m_bOpen = true;
		}
	}
	else
	{
		// �O�t���[���ɓ������Ă�����
		if (m_bOldCollision == true)
		{
			// �A�j���[�V�����Đ�
			Parent->GetComponent<CDrawModel>()->Play(1);
		}
		// �J���Ă��܂���
		m_bOpen = false;
	}

	// �ޔ�
	m_bOldCollision = m_bCollision;

	// ���t���[���t���O��OFF
	m_bCollision = false;
}

// �`��
void CBud::Draw(){
}

// �Փˏ���
void CBud::OnCollisionEnter(Object* pObject)
{
	// �v���C���[
	if (pObject->GetName() == PLAYER_NAME)
	{
		// �ڂ݂��J��
		m_bCollision = true;
	}
}