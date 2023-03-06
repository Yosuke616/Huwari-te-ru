#include "PostCollisionComponent.h"
#include "ObjectInfo.h"
#include "InformationComponent.h"
#include "IcicleComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "InformationComponent.h"
#include "DeleteTimerCom.h"
#include "Draw3dComponent.h"
#include "Sound.h"
#include "CloverComponent.h"
#include "EffectManager.h"


// コンストラクタ
CPostCollision::CPostCollision()
	: m_pTransform(nullptr), m_OldPos(0.0f,0.0f)
{
}

// デストラクタ
CPostCollision::~CPostCollision(){
}

// 初期化
void CPostCollision::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

void CPostCollision::Update()
{
	m_OldPos.x = m_pTransform->Pos.x;
	m_OldPos.y = m_pTransform->Pos.y;
}

// 衝突処理
void CPostCollision::OnCollisionEnter(Object* pObject)
{
	// つららとブロックのあたり判定
	if (Parent->GetName() == ICICLE_DOWN_NAME)
	{
		// ブロックとのあたり判定
		if (pObject->GetName() == BLOCK_NAME)
		{
			auto InfoCom = Parent->GetComponent<CInformation>();
			if (InfoCom)
			{
				auto pIcicle = InfoCom->GetObjetInfo()->GetComponent<CIcicle>();
				if (pIcicle)
				{
					pIcicle->Stop();
				}
			}
		}
	}
}
