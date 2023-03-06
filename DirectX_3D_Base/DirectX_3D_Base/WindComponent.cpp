#include "WindComponent.h"
#include "TransformComponent.h"
#include "ObjectInfo.h"
#include "FallComponent.h"
#include "AnimationComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "ObjectInfo.h"
#include "IcicleComponent.h"
#include "Draw3dComponent.h"

// コンストラクタ
CWind::CWind()
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}
// デストラクタ
CWind::~CWind()
{
}
// 初期化
void CWind::Start()
{
}
// 更新
void CWind::Update()
{
}
// 描画
void CWind::Draw()
{
}
// 衝突処理
void CWind::OnCollisionEnter(Object* pObject)
{
	//	プレイヤーだった場合
	if (pObject->GetName() == PLAYER_NAME)
	{
		//	プレイヤーに速度を加える
		auto Player = pObject->GetComponent<CTransform>();
		Player->Vel.x += WIND_POWER_X * m_Vector.x;
		Player->Vel.y += WIND_POWER_Y * m_Vector.y;

		//	左右に揺らさない
		auto Leaf = pObject->GetComponent<CFall>();
		if (Leaf) Leaf->SetActive(false);

		//	アニメーション = 風に吹かれている
		pObject->GetComponent<CPlayer>()->SetAnimState(PLAYER_CARRIED);


		//	向き
		auto player = pObject->GetComponent<CPlayer>();
		if (player)
		{
			if(this->m_Vector.x < 0)
			player->SetDestRot(45.0f);
			else
			{
				player->SetDestRot(-45.0f);
			}
		}
	}

	//2021/12/22 Shimizu Yosuke-------------------------------------
	//	クローバー
	if (pObject->GetName() == CLOVER_NAME) {
		//	風に速度を加える
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WINDENDING_POWER_X * m_Vector.x;
		Wind->Vel.y += WINDENDING_POWER_Y * m_Vector.y;

		if (Wind->Vel.x >  MAX_CLOVER_VELOCITY) Wind->Vel.x =  MAX_CLOVER_VELOCITY;
		if (Wind->Vel.x < -MAX_CLOVER_VELOCITY) Wind->Vel.x = -MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y >  MAX_CLOVER_VELOCITY) Wind->Vel.y =  MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y < -MAX_CLOVER_VELOCITY) Wind->Vel.y = -MAX_CLOVER_VELOCITY;
	}
	else if (pObject->GetName() == ENDING_CLOVER_NAME)
	{
		//	風に速度を加える
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WIND_POWER_X * m_Vector.x;
		Wind->Vel.y += WIND_POWER_Y * m_Vector.y;		//かける数は変えて下さい

		if (Wind->Vel.x > MAX_CLOVER_VELOCITY) Wind->Vel.x = MAX_CLOVER_VELOCITY;
		if (Wind->Vel.x < -MAX_CLOVER_VELOCITY) Wind->Vel.x = -MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y > MAX_CLOVER_VELOCITY) Wind->Vel.y = MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y < -MAX_CLOVER_VELOCITY) Wind->Vel.y = -MAX_CLOVER_VELOCITY;
	}

	//---------------------------------------------------------------
	//	つららを落とす
	if (pObject->GetName() == ICICLE_UP_NAME)
	{
		pObject->GetComponent<CIcicle>()->IcicleDamage();
	}

#ifdef BLOCK_MOVE
	else if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	風に速度を加える
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WIND_POWER_X * m_Vector.x;
		if (Wind->Vel.x > MAX_ICE_VELOCITY)
		{
			Wind->Vel.x = MAX_ICE_VELOCITY;
		}
		else if (Wind->Vel.x < -MAX_ICE_VELOCITY)
		{
			Wind->Vel.x = -MAX_ICE_VELOCITY;
		}
	}
#endif // BLOCK_MOVE

}

// 風の向き(x成分,y成分)の設定
void CWind::SetWindVector(XMFLOAT2 vector)
{
	m_Vector = vector;
}

// 風の向きの取得
XMFLOAT2 CWind::GetAngle()
{
	return m_Vector;
}