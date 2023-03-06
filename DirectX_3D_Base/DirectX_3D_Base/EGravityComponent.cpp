#include "EGravityComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"

#define END_MAX_VELOCITY	(1.5f)



// コンストラクタ
CEGravity::CEGravity()
	: m_pTransform(nullptr)
{
}


// デストラクタ
CEGravity::~CEGravity()
{
}

// 初期化
void CEGravity::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CEGravity::Update()
{

	m_pTransform->Vel.y -= EGRAVITY;
	if (m_pTransform->Vel.y < -END_MAX_VELOCITY) m_pTransform->Vel.y = -END_MAX_VELOCITY;

}