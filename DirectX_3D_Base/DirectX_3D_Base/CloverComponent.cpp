/**
 * @file	CloverComponent.h
 * @brief	クローバーの処理
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 *			2021/12/22 shimizu yousuke 中身を変更
*/

//===== インクルード =====
#include "CloverComponent.h"
#include "InputManager.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "FallComponent.h"
#include "Draw2DComponent.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "StageManager.h"
#include "Sound.h"
#include "Draw3dComponent.h"
#include "GravityComponent.h"
#include "ModelManager.h"
#include "FallComponent.h"
#include "OutRangeCom.h"
#include "TagCom.h"

//***コンストラクタ
CloverComponent::CloverComponent()
	: m_bCloverflag(false), m_pTransform(nullptr),
	m_nCnt_Clover(0)
	, m_OldPos(0.0f,0.0f)
	, m_bBind(false)
	, m_nNumber(0)
{
}

// デストラクタ
CloverComponent::~CloverComponent(){
}

// 初期化
void CloverComponent::Start()
{
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CloverComponent::Update()
{
	m_OldPos = m_pCollider->GetCenterPos();

	// X移動量に慣性をかける
	m_pTransform->Vel.x += -m_pTransform->Vel.x * RATE_MOVE_CLOVER;

}

// 描画
void CloverComponent::Draw(){
}

// 衝突処理
void CloverComponent::OnCollisionEnter(Object* pObject)
{
	//	ブロック
	if (pObject->GetName() == BLOCK_NAME ||
		pObject->GetName() == ICICLE_UP_NAME ||
		pObject->GetName() == ICICLE_DOWN_NAME ||
		pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	プレイヤーの情報を取得
		auto Owner = Parent->GetComponent<CCollider>();		//	あたり判定の大きさが欲しい
		auto OwnerPos = Owner->GetCenterPos();				// 中心座標
		auto OwnerSize = Owner->GetColliderSize();			// あたり判定の大きさ
		auto OwnerOffset = Owner->GetOffSet();				// 中心とのずれ
		//	ぶつかった相手の情報の取得
		auto Other = pObject->GetComponent<CCollider>();	// あたり判定の大きさが欲しい
		auto OtherPos = Other->GetCenterPos();				// 中心座標
		auto OtherSize = Other->GetColliderSize();			// あたり判定の大きさ
		auto OtherOffset = Other->GetOffSet();				// 中心とのずれ

		//	それぞれの半分の大きさを格納
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

		//	ブロックのあたり判定ライン
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	ブロックの左端
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		　  右端
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		　  上端
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		　  下端

		/*		   __
			　	〇 |P|
				　 |_|			 __
			_________________ × |P|
		  　|←　ブロックの横幅 →|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// 左の点
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// 右の点
		{
			//	P
			//	↓
			//	□
			if (m_pTransform->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&		// 前はめり込んでいない
				OwnerPos.y - PlayerHalhSize.y < BlockUpLine)		// 今はめり込んでる
			{
				// yの速度をなくす
				m_pTransform->Vel.y = 0.0f;

				// 座標補正
				m_pTransform->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

#ifdef COLVER_FALL
				// 揺れるフラグOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);
#endif
			}
			//	□
			//	↑
			//	P
			else if (m_pTransform->Vel.y > 0 &&
				BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// 前はめり込んでいない
				OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// 今はめり込んでる
			{
				// yの速度をなくす
				m_pTransform->Vel.y = 0.0f;

				// 座標補正
				m_pTransform->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// 中心座標

		/*
				× __
				  |P|
				  |_|
		  __
		 　ブ|
		 　ロ|	〇 __
		　 ッ|	  |P|
		　 ク|	  |_|
		  __|

		*/
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// P→□
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&	// 前はめり込んでいない
				OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)		// 今はめり込んでる
			{
				//	x移動量を消す
				m_pTransform->Vel.x = 0.0f;

				// 座標補正
				m_pTransform->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
			// □←P
			else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
				OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
			{
				//	x移動量を消す
				m_pTransform->Vel.x = 0.0f;

				// 座標補正
				m_pTransform->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
		}


		return;
	}

}


// クローバー生成
Object* CloverComponent::CreateClover(XMFLOAT3 pos, int id)
{
#pragma region ---クローバー
	Object* pClover = new Object(CLOVER_NAME, UPDATE_MODEL, DRAW_MODEL);
	// components
	auto TransClover = pClover->AddComponent<CTransform>();				//　オブジェクトの座標とか
	auto DrawClover = pClover->AddComponent<CDrawModel>();					//　モデル機能
	auto ColliderClover = pClover->AddComponent<CCollider>();			//　衝突判定機能
	auto Clover = pClover->AddComponent<CloverComponent>();				//　クローバーのコンポーネント
	pClover->AddComponent<CGravity>();									//  風に乗って落とせ利用に
	auto outRange = pClover->AddComponent<COutRange>();
	auto comID = pClover->AddComponent<CTag>();
	comID->SetID(id);
	outRange->SetLimitRange(OUT_RANGE_X - 40.0f, OUT_RANGE_Y);
	// ssettings
	TransClover->SetPosition(pos.x, pos.y + CLOVER_OFFSET_Y, pos.z + CLOVER_POS_Z);				//　座標
	TransClover->SetRotate(CLOVER_ROTATE_X, CLOVER_ROTATE_Y, CLOVER_ROTATE_Z);	// 回転
	TransClover->SetScale(CLOVER_SCALE_X, CLOVER_SCALE_Y, CLOVER_SCALE_Z);		//　大きさ
	DrawClover->SetModel(ModelManager::GetInstance()->GetModel(CLOVER_MODEL_NUM));//　モデル
	ColliderClover->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);			//　当たり判定の大きさ
	// add to list
	ObjectManager::GetInstance()->AddObject(pClover);
#pragma endregion

	return pClover;
}

// 拘束されているフラグの設定	true 拘束されている
void CloverComponent::Binded(bool bBind)
{
	m_bBind = bBind;
}

// 拘束されているかの確認
bool CloverComponent::IsBinded()
{
	return m_bBind;
}


// クローバーの番号(ステージの左から0, 1, 2, 3
void CloverComponent::SetNumber(int number)
{
	m_nNumber = number;
}
// クローバーの番号を取得する
int CloverComponent::GetNumber()
{
	return m_nNumber;
}