#include "FollowCompomemt.h"
#include "TransformComponent.h"

//*** �R���X�g���N�^
CFollow::CFollow()
	: m_pTransform(nullptr), m_pFollowObject(nullptr)
{
}

//*** �f�X�g���N�^
CFollow::~CFollow(){
}

//*** �J�n
void CFollow::Start()
{
	// �I�u�W�F�N�g�̍��W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
}

//*** �X�V
void CFollow::Update()
{
	// ���Ă����Ώۂ����Ȃ��Ȃ����ꍇ����
	if (m_pFollowObject->GetDeleteFlag())
	{
		Parent->Delete();
		return;
	}

	// �Ώۂ̃I�u�W�F�N�g�̑��x��������
	if (m_pFollowObject)
	{
		auto pFollowTransform = m_pFollowObject->GetComponent<CTransform>();
		if (pFollowTransform)
		{
			m_pTransform->Pos.x += pFollowTransform->Vel.x;
			m_pTransform->Pos.y += pFollowTransform->Vel.y;
			m_pTransform->Pos.z += pFollowTransform->Vel.z;
		}
	}
}

//*** �`��
void CFollow::Draw(){
}

//*** �Փˏ���
void CFollow::OnCollisionEnter(Object* pObject){
}

//*** ���̃I�u�W�F�N�g�����Ă����Ώۂ̍��W�̐ݒ�
void CFollow::SetParent(Object* pParent)
{
	m_pFollowObject = pParent;
}