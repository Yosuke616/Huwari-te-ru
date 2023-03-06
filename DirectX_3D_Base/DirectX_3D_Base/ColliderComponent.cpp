//===== インクルード =====
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "BillboardComponent.h"
#include "Draw2dComponent.h"

//===== 静的メンバ変数 =====
std::list<Object*> CCollider::m_CollidrList;	//	あたり判定リスト

// コンストラクタ
CCollider::CCollider()
	: m_pTransform(nullptr)
{
	m_eUpdateOrder = COM_UPDATE_COLLIDER;	//	コンポーネントリストの更新順序

	m_CenterOffset = XMFLOAT2(0.0f, 0.0f);
	m_CollisionSize = XMFLOAT3(0.0f, 0.0f,0.0f);
}

// デストラクタ
CCollider::~CCollider()
{
}

// 生成時
void CCollider::Start()
{
	//	コンポーネントの取得
	m_pTransform = Parent->GetComponent<CTransform>();	//	オブジェクト座標の取得

	//	あたり判定リストに登録
	AddColliderList(Parent);

#ifdef _DEBUG
	//	デバッグ用コンポーネントの追加
	if (Parent->GetComponent<CDraw2D>() == nullptr)
	{
		auto pDebug = Parent->AddComponent<CDrawMesh>();
		//	設定
		pDebug->SetOffset(m_CenterOffset);
		pDebug->SetSize(m_CollisionSize.x, m_CollisionSize.y);

		//	初期化
		pDebug->Start();
	}
#endif // _DEBUG

}

// 更新
void CCollider::Update()
{
	//	衝突判定リストへの追加ループ
	for (auto&& OtherObject : m_CollidrList)
	{
		//	どちらかの削除フラグがtrueだった場合 continue
		if (Parent->GetDeleteFlag() == true) continue;
		if (OtherObject->GetDeleteFlag() == true) continue;

		// コンポーネントが機能停止の時 continue
		auto OtherCollider = OtherObject->GetComponent<CCollider>();
		if (this->m_bUpdateFlag == false) continue;
		if (OtherCollider->m_bUpdateFlag == false) continue;

		//	あたり判定を取る対象がこのオブジェクトだった場合 continue
		//if (OtherObject->GetName() == Parent->GetName()) continue;
		if (OtherObject == Parent) continue;
	
		//	あたり判定を取る対象の情報を取得
		auto Other = OtherObject->GetComponent<CTransform>();			//	あたり判定を取る対象の座標系

		// 2回目の衝突処理を避ける
		bool bOnceHit = false;

		for (auto OnceCollision : this->m_IntersectObjects)
		{
			// 自分が当たったことがあるオブジェクトのリスト == 現在のあたり判定を取る対象
			if (OnceCollision == OtherObject)
			{
				bOnceHit = true;
				break;
			}
		}
		if (bOnceHit) continue;	// 衝突したことがある場合 continue
		
	
		

		//	x,y平面のあたり判定
		if (this->CollisionRectToRectXY(
			m_pTransform,										//	自分の座標
			Other,												//	相手の座標
			XMFLOAT2(m_CollisionSize.x, m_CollisionSize.y),		//	自分の大きさ
			XMFLOAT2(OtherCollider->m_CollisionSize.x,			//	相手の大きさ
				OtherCollider->m_CollisionSize.y),
			m_CenterOffset,										//	自分のオフセット(描画の中心とあたり判定の中心の差)
			OtherCollider->m_CenterOffset))						//	相手のオフセット
		{
			// 相手に当たったことを知らせる
			OtherCollider->m_IntersectObjects.push_back(Parent);
		}
	}
}

// 大きさの設定
void CCollider::SetCollisionSize(float fWidth, float fHeight , float fz)
{
	m_CollisionSize.x = fWidth;
	m_CollisionSize.y = fHeight;
	m_CollisionSize.z = fz;
}

//	オフセット
void CCollider::SetOffset(float fx, float fy)
{
	m_CenterOffset.x = fx;
	m_CenterOffset.y = fy;
}
XMFLOAT2 CCollider::GetOffSet()
{
	return m_CenterOffset;
}

// サイズ取得
XMFLOAT3 CCollider::GetColliderSize()
{
	return m_CollisionSize;
}
XMFLOAT2 CCollider::GetCenterPos()
{
	return XMFLOAT2(m_pTransform->Pos.x + m_CenterOffset.x, m_pTransform->Pos.y + m_CenterOffset.y);
}

// 矩形と矩形
bool CCollider::CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
	XMFLOAT2 size1, XMFLOAT2 size2, XMFLOAT2 Velocity1, XMFLOAT2 Velocity2)
{
	XMFLOAT2 halfSize1 = XMFLOAT2(size1.x / 2.0f, size1.y / 2.0f);
	XMFLOAT2 halfSize2 = XMFLOAT2(size2.x / 2.0f, size2.y / 2.0f);


	if (centerPos2.x - halfSize2.x + Velocity2.x < centerPos1.x + halfSize1.x + Velocity1.x &&		// 2の左端 < 1の右端
		centerPos1.x - halfSize1.x + Velocity1.x < centerPos2.x + halfSize2.x + Velocity2.x)		// 1の左端 < 2の右端
	{
		if (centerPos2.y - halfSize2.y + Velocity2.y < centerPos1.y + halfSize1.y + Velocity1.y  &&	// 2の上端 < 1の下端
			centerPos1.y - halfSize1.y + Velocity1.y < centerPos2.y + halfSize2.y + Velocity2.y)	// 1の上端 < 2の下端
		{
			return true;
		}
	}

	return false;
}

// 円と円
bool CCollider::CheckCollisionCircleToCircle(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
	float radius1, float radius2)
{
	if ((centerPos1.x - centerPos2.x) * (centerPos1.x - centerPos2.x)
		+ (centerPos1.y - centerPos2.y) * (centerPos1.y - centerPos2.y)	// (円1の中心座標X - 円2の中心座標X)の2乗 + (円1の中心座標Y - 円2の中心座標Y)の2乗
		<= (radius1 + radius2) * (radius1 + radius2))	// (円1の半径+円2の半径)の2乗
	{
		return true;
	}

	return false;
}

// 球と球
bool CCollider::CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// AABB同士の衝突判定
bool CCollider::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&	//	Aの左端 <= Bの右端
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&		//	Bの左端 <= Aの右端
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&		//	Aの下端 <= Bの上端
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&		//	Bの下端 <= Aの上端
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&		//	Aの手前 <= Bの奥
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);			//	Bの手前 <= Aの奥
}

//	xy平面のあたり判定
bool CCollider::CollisionRectToRectXY(CTransform* pA, CTransform* pB
	, XMFLOAT2 Asize, XMFLOAT2 Bsize
	, XMFLOAT2 Aoffset, XMFLOAT2 Boffset)
{
	//	中心座標
	//XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + pA->Vel.x + Aoffset.x, pA->Pos.y + pA->Vel.y + Aoffset.y);
	//XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + pB->Vel.x + Aoffset.x, pB->Pos.y + pB->Vel.y + Boffset.y);
	XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + Aoffset.x, pA->Pos.y + Aoffset.y);
	XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + Aoffset.x, pB->Pos.y + Boffset.y);
	XMFLOAT2 halfSizeA = XMFLOAT2(Asize.x / 2.0f, Asize.y / 2.0f);
	XMFLOAT2 halfSizeB = XMFLOAT2(Bsize.x / 2.0f, Bsize.y / 2.0f);

	if (centerPosB.x - halfSizeB.x < centerPosA.x + halfSizeA.x  &&		// Bの左端 < Aの右端
		centerPosA.x - halfSizeA.x < centerPosB.x + halfSizeB.x )		// Aの左端 < Bの右端
	{
		if (centerPosB.y - halfSizeB.y < centerPosA.y + halfSizeA.y  &&	// Bの上端 < Aの下端
			centerPosA.y - halfSizeA.y < centerPosB.y + halfSizeB.y)	// Aの上端 < Bの下端
		{
			return true;
		}
	}

	return false;
}


// あたり判定リストに追加
void CCollider::AddColliderList(Object* pObject)
{
	//	更新リストに追加
	std::list<Object*>::iterator itr = m_CollidrList.begin();
	for (auto&& object : m_CollidrList)
	{
		if (pObject->GetUpdateOrder() > object->GetUpdateOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	m_CollidrList.insert(itr, pObject);
}

//	あたり判定のリストを取得
std::list<Object*>& CCollider::GetColliderList()
{
	return m_CollidrList;
}

void CCollider::UpdateColliders()
{
	for (auto this_object : m_CollidrList)
	{
		if (this_object->GetName() != PLAYER_NAME)
		{
			// コライダー機能を持ってくる
			auto this_collider = this_object->GetComponent<CCollider>();

			// 衝突時の処理
			//	 auto 衝突した相手		  : 衝突した相手たち
			for (auto collided_object : this_collider->m_IntersectObjects)
			{
				//	このオブジェクトの衝突処理を実行
				this_object->OnCollisionEnter(collided_object);
				//	衝突した相手の衝突処理を実行
				collided_object->OnCollisionEnter(this_object);
			}
		}
		// プレイヤー
		else
		{
			// コライダー機能を持ってくる
			auto this_collider = this_object->GetComponent<CCollider>();
			
			if (this_collider->m_IntersectObjects.size() > 1)
			{
				this_collider->m_IntersectObjects.sort([=](Object* A, Object* B)
					{
						auto thisPos = this_collider->GetCenterPos();
						auto PosA = A->GetComponent<CCollider>()->GetCenterPos();
						auto PosB = B->GetComponent<CCollider>()->GetCenterPos();
						// 距離
						XMFLOAT2 DiffA(thisPos.x - PosA.x, thisPos.y - PosA.y);
						XMFLOAT2 DiffB(thisPos.x - PosB.x, thisPos.y - PosB.y);

						XMVECTOR vDiffA = DirectX::XMLoadFloat2(&DiffA);
						XMVECTOR vDiffB = DirectX::XMLoadFloat2(&DiffB);
						XMVECTOR vLengthA;
						XMVECTOR vLengthB;
						vLengthA = DirectX::XMVector2LengthEst(vDiffA);
						vLengthB = DirectX::XMVector2LengthEst(vDiffB);
						// Diff再利用
						DirectX::XMStoreFloat2(&DiffA, vLengthA);
						DirectX::XMStoreFloat2(&DiffB, vLengthB);

						return DiffA.x < DiffB.x;
					});
			}

			// 衝突時の処理
			//	 auto 衝突した相手		  : 衝突した相手たち
			for (auto collided_object : this_collider->m_IntersectObjects)
			{
				//	このオブジェクトの衝突処理を実行
				this_object->OnCollisionEnter(collided_object);
				//	衝突した相手の衝突処理を実行
				collided_object->OnCollisionEnter(this_object);
			}
		}

		// 衝突したことを知らせるのをやめる
		this_object->GetComponent<CCollider>()->m_IntersectObjects.clear();
	}
}