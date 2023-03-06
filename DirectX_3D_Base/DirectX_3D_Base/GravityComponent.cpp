#include "GravityComponent.h"
#include "TransformComponent.h"

#define MAX_VELOCITY	(1.5f)

// �R���X�g���N�^
CGravity::CGravity()
	: m_pTransform(nullptr)
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}


// �f�X�g���N�^
CGravity::~CGravity()
{
}

// ������
void CGravity::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CGravity::Update()
{
	m_pTransform->Vel.y -= GRAVITY;
	if (m_pTransform->Vel.y < -MAX_VELOCITY) m_pTransform->Vel.y = -MAX_VELOCITY;

}