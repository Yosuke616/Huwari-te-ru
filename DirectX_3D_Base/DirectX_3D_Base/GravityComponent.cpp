#include "GravityComponent.h"
#include "TransformComponent.h"

#define MAX_VELOCITY	(1.5f)

// コンストラクタ
CGravity::CGravity()
	: m_pTransform(nullptr)
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}


// デストラクタ
CGravity::~CGravity()
{
}

// 初期化
void CGravity::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CGravity::Update()
{
	m_pTransform->Vel.y -= GRAVITY;
	if (m_pTransform->Vel.y < -MAX_VELOCITY) m_pTransform->Vel.y = -MAX_VELOCITY;

}