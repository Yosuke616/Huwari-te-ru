#include "PreviewComponent.h"
#include "Draw2dComponent.h"

// �R���X�g���N�^
CPreview::CPreview()
	: m_bCollision(false), m_bOldCollision(false)
{
}

// �f�X�g���N�^
CPreview::~CPreview(){
}

// ������
void CPreview::Start(){
}

// �X�V
void CPreview::Update()
{
	// �����Փ˂��Ă�����
	if (m_bCollision)
	{
		// �O�t���[���ɓ������Ă��Ȃ�������
		if (m_bOldCollision == false)
		{

			//�v���r���[�Đ�
			Parent->GetComponent<CDrawEffect2D>()->SetPlayONOFF(true);

		}
	}
	else
	{
		// �O�t���[���ɓ������Ă�����
		if (m_bOldCollision == true)
		{
			//�v���r���[�̃A�j���[�V�������O�ɃZ�b�g
			Parent->GetComponent<CDrawEffect2D>()->SetAnimNum(0);
			//�v���r���[��~
			Parent->GetComponent<CDrawEffect2D>()->SetPlayONOFF(false);
		}


	}

	// �ޔ�
	m_bOldCollision = m_bCollision;

	// ���t���[���t���O��OFF
	m_bCollision = false;
}

// �`��
void CPreview::Draw(){
}

// �Փˏ���
void CPreview::OnCollisionEnter(Object* pObject)
{
	// �J�[�\��
	if (pObject->GetName() == CURSOR_NAME)
	{
		m_bCollision = true;
	}
}