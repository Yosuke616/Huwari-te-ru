#include "IceBlockCom.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "EffectManager.h"
#include "DeleteTimerCom.h"
#include "Draw3dComponent.h"
#include "Sound.h"
#include "InformationComponent.h"
#include "CloverComponent.h"
#include "IcicleComponent.h"


CIceBlock::CIceBlock()
	: m_nDirect(DIR_NONE)
	, m_pTransform(nullptr)
	, m_OldPos(0.0f, 0.0f)
	, m_nCloverNumber(100)
{
}

CIceBlock::~CIceBlock(){
}
void CIceBlock::Start()
{
	// オブジェクト座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
}
void CIceBlock::Update()
{
	m_OldPos.x = m_pTransform->Pos.x;
	m_OldPos.y = m_pTransform->Pos.y;

	if (m_pTransform->Vel.x > 0) m_nDirect = DIR_RIGHT;
	else if (m_pTransform->Vel.x < 0) m_nDirect = DIR_LEFT;
}
void CIceBlock::Draw(){
}
void CIceBlock::OnCollisionEnter(Object* pObject)
{
	// 氷とブロック
	if (pObject->GetName() == BLOCK_NAME)
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

	// 氷と氷
	if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	プレイヤーの情報を取得
		auto Owner = Parent->GetComponent<CCollider>();		// あたり判定の大きさが欲しい
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
		//if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
		//	(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// 左の点
		//	(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// 右の点
		//{
		//	//	P
		//	//	↓
		//	//	□
		//	if (m_pTransform->Vel.y < 0 &&
		//		BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&		// 前はめり込んでいない
		//		OwnerPos.y - PlayerHalhSize.y < BlockUpLine)		// 今はめり込んでる
		//	{
		//		// yの速度をなくす
		//		m_pTransform->Vel.y = 0.0f;
		//
		//		// 座標補正
		//		m_pTransform->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;
		//	}
		//}
		//
		//OwnerPos = Owner->GetCenterPos();				// 中心座標

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
		//if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
		//	(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
		//	(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		//{
			// おはじき
			m_pTransform->Vel.x = pObject->GetComponent<CTransform>()->Vel.x;	// 速度を衝突したオブジェクトに移す
			pObject->GetComponent<CTransform>()->Vel.x = 0.0f;				// 移した側の速度を消す

			// 座標補正
			// 氷ブロックの速度で決める
			if (OwnerPos.x > OtherPos.x)		// 氷が→向き
			{
				m_pTransform->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
			else if (OwnerPos.x < OtherPos.x)	// 氷が←向き
			{
				m_pTransform->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
		//}

		return;
	}

	// 氷とつらら
	if (pObject->GetName() == ICICLE_DOWN_NAME)
	{
		if (pObject->GetComponent<CInformation>()->GetObjetInfo()->GetComponent<CIcicle>()->IsMoving() == true)
		{
			// つらら上部分削除
			pObject->GetComponent<CInformation>()->GetObjetInfo()->Delete();

			// つらら下部分削除
			pObject->Delete();

			//se
			CSound::Play(SE_ICICLE_DESTROY);	// ←これを追加
			CSound::Stop(SE_ICICLE_FALL);		// ←これを追加

			// エフェクト開始
			EffectManager::GetInstance()->Play(ICICLE_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(3.0f, 3.0f, 3.0f));

			// 氷のアニメーション再生
			Parent->GetComponent<CDrawModel>()->Play(0);

			// 数フレーム後に削除する
			Parent->GetComponent<CTimer>()->SetTime(60);

			// あたり判定と座標の更新を止める
			Parent->StopUpdate();
			Parent->GetComponent<CTimer>()->m_bUpdateFlag = true;
			Parent->GetComponent<CDrawModel>()->m_bUpdateFlag = true;

			// 機能のタイプがFUNC_CREATE_CLOVERだった場合クローバーをつくる
			if (Parent->GetComponent<CTimer>()->GetFunktion() == FUNC_CREATE_CLOVER)
			{
				// 座標
				XMFLOAT3 create_pos = Parent->GetComponent<CTransform>()->Pos;
				create_pos.y += 8.0f;

				// クローバーをつくる
				Object* pClover;
				pClover = CloverComponent::CreateClover(create_pos, -1);

				// クローバーに番号を割り振る（氷ブロックに設定されている番号を与える
				pClover->GetComponent<CloverComponent>()->SetNumber(this->GetNumber());
			}
		}
		else
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
}

// すすんでいる方向の取得
int CIceBlock::GetDirect()
{
	return m_nDirect;
}


void CIceBlock::SetNumber(int num)
{
	m_nCloverNumber = num;
}
int CIceBlock::GetNumber()
{
	return m_nCloverNumber;
}