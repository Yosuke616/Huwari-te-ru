#include "OutRangeCom.h"
#include "TransformComponent.h"
#include "Camera.h"


// ���S���狗��


// �R���X�g���N�^
COutRange::COutRange()
	: m_pTransform(nullptr)
{
	m_LimitRange.x = OUT_RANGE_X;
	m_LimitRange.y = OUT_RANGE_Y;
}

// �f�X�g���N�^
COutRange::~COutRange(){
}

// ������
void COutRange::Start()
{
	// �I�u�W�F�N�g���W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
}
// �X�V
void COutRange::Update()
{
	// �J��������ɂ���
	// ����������L�������ꂽ�I�u�W�F�N�g�̋@�\���~������

	// ��ʂ̒��S���W
	XMFLOAT3 Center = CCamera::Get()->GetPos();

	// ��苗������Ă�����X�V���~�߂�
	if (Center.x + m_LimitRange.x < m_pTransform->Pos.x ||	
		Center.x - m_LimitRange.x > m_pTransform->Pos.x)	
		//Center.y + m_LimitRange.y < m_pTransform->Pos.y ||
		//Center.y - m_LimitRange.y > m_pTransform->Pos.y)	
	{
		// �@�\��~
		Parent->StopUpdate();
		Parent->StopDraw();

		// ���̃R���|�[�l���g�����X�V����
		this->m_bUpdateFlag = true;
	}
	// ���L�����ȓ��ɂ���ꍇ�A�X�V������
	else
	{
		// �X�V���ĊJ
		Parent->Use();
	}
}
// �`��
void COutRange::Draw()
{

}
// �Փˏ���
void COutRange::OnCollisionEnter(Object* pObject)
{

}


// ���̃R���|�[�l���g�ȊO�̃I�u�W�F�N�g�̋@�\���~������
void COutRange::NoFunction()
{
	// �@�\��~
	Parent->NoUse();

	// ���̃R���|�[�l���g�����X�V����
	this->m_bUpdateFlag = true;
}

void COutRange::SetLimitRange(float x, float y)
{
	m_LimitRange.x = x;
	m_LimitRange.y = y;
}