//===== �C���N���[�h =====
#include "InformationComponent.h"
#include "Object.h"


// �R���X�g���N�^
CInformation::CInformation()
	: m_pObject(nullptr)
{
}

// �f�X�g���N�^
CInformation::~CInformation(){
}

// �X�V
void CInformation::Update(){
}

// �I�u�W�F�N�g����ݒ�
void CInformation::SetObjectInfo(Object* pObject)
{
	m_pObject = pObject;
}

// �I�u�W�F�N�g���̎擾
Object* CInformation::GetObjetInfo()
{
	return m_pObject;
}