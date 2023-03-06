#include "EGravityComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"

#define END_MAX_VELOCITY	(1.5f)



// �R���X�g���N�^
CEGravity::CEGravity()
	: m_pTransform(nullptr)
{
}


// �f�X�g���N�^
CEGravity::~CEGravity()
{
}

// ������
void CEGravity::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CEGravity::Update()
{

	m_pTransform->Vel.y -= EGRAVITY;
	if (m_pTransform->Vel.y < -END_MAX_VELOCITY) m_pTransform->Vel.y = -END_MAX_VELOCITY;

}