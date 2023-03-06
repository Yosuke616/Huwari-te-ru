#include "SpiderWebComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "imgui.h"
#include "DrawMeshComponent.h"
#include "DrawEffectComponent.h"
#include "AnimationComponent.h"
#include "Draw3dComponent.h"
#include "PlayerComponent.h"
#include "Sound.h"
#include "TextureManager.h"
#include "CloverComponent.h"

//***コンストラクタ
CSpiderWeb::CSpiderWeb()
	: m_bUpColFlg(false)
	, m_bDownColFlg(false)
	, m_pTransform(nullptr)
	, m_nWindCnt(0)
	, nCnt(0) //se
{

}
CSpiderWeb::~CSpiderWeb()
{
	// クローバーの拘束フラグを解除
	for (auto&& object : m_BindObjects)
	{
		if (object.first->GetName() == CLOVER_NAME) object.first->GetComponent<CloverComponent>()->Binded(false);
	}

	m_BindObjects.clear();
}
//***初期化
void CSpiderWeb::Start()
{
	//親オブジェクトのコンポーネント取得
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();

}

//***更新
void CSpiderWeb::Update()
{
	//上部分と下部分の両方に風が接触していたら
	if (m_bUpColFlg == true && m_bDownColFlg == true)
	{
		//クモの巣破壊時SE再生
		CSound::Play(SE_WEB_DESTROY);

		//親オブジェクトを削除
		Parent->Delete();
		return;
	}


	for (auto&& Object : m_BindObjects)
	{
		if (Object.first == nullptr) continue;
		if (Object.first->GetDeleteFlag() == true) continue;
		//接触オブジェクトを固定
		Object.first->GetComponent<CTransform>()->Pos.x = Object.second.x;
		Object.first->GetComponent<CTransform>()->Pos.y = Object.second.y;

		if (Object.first->GetName() == PLAYER_NAME)
		{
			//SEループ再生用
			nCnt--;
			if (nCnt < 0)
			{
				CSound::Play(SE_BIND);
				nCnt = 30;	// 再生フレーム
			}

			// アニメーション
			Object.first->GetComponent<CPlayer>()->SetAnimState(PLAYER_BIND);
		}
	}
}

//***描画
void CSpiderWeb::Draw()
{

}
// *** セッター　拘束範囲
void CSpiderWeb::SetBindSize(float x, float y)
{
	m_BindSize.x = x;
	m_BindSize.y = y;
}


//***当たり判定
void CSpiderWeb::OnCollisionEnter(Object* pObject)
{

	//親の当たり判定をもらってくる
	auto Owner = Parent->GetComponent<CCollider>();
	//当たり判定のポインタからサイズを持ってくる
	auto OwnerSize = Owner->GetColliderSize();
	//当たり判定の中心を格納する変数
	auto OwnerPos = Owner->GetCenterPos();

	//引数のオブジェクトの衝突判定コンポーネントを持ってくる
	auto Other = pObject->GetComponent<CCollider>();
	//引数のトランスフォームコンポーネントを持ってくる
	auto OtherTransform = pObject->GetComponent<CTransform>();
	//引数の衝突判定の中心を持ってくる
	auto OtherPos = Other->GetCenterPos();
	//引数の当たり判定の置き差を持ってくる
	auto OtherSize = Other->GetColliderSize();



	//プレイヤーと接触
	if (pObject->GetName() == PLAYER_NAME && m_BindFlags[pObject] == false)//m_bPlayerBindFlg == false)
	{
		//拘束範囲との接触
		if (OwnerPos.x - m_BindSize.x / 2 < Other->GetCenterPos().x + OtherSize.x / 2 &&		// 2の左端 < 1の右端
			Other->GetCenterPos().x - OtherSize.x / 2 < OwnerPos.x + m_BindSize.x / 2)		// 1の左端 < 2の右端
		{
			if (OwnerPos.y - m_BindSize.y / 2 < Other->GetCenterPos().y + OtherSize.y / 2 &&	// 2の上端 < 1の下端
				Other->GetCenterPos().y - OtherSize.y / 2 < OwnerPos.y + m_BindSize.y / 2)	// 1の上端 < 2の下端

			{
				//接触オブジェクトを固定する座標を設定
				m_BindObjects[pObject] = XMFLOAT2(OtherTransform->Pos.x, OtherTransform->Pos.y);
				//拘束フラグをON
				m_BindFlags[pObject] = true;

				//上、下部分の当たり判定フラグを一時的にfalseに
				m_bUpColFlg = false;
				m_bDownColFlg = false;
				//m_bPlayerBindFlg = true;
			}
		}
	}

	//風と接触(拘束状態)
	if (pObject->GetName() == WIND_NAME && m_BindFlags.size() != 0)//&& m_bPlayerBindFlg == true)
	{
		m_nWindCnt++;			//風が当たった回数カウンタ

		if (m_nWindCnt >= WIND_ATTACK_NUM)	//風が何回か当たったら
		{
			//クモの巣破壊時SE再生
			CSound::Play(SE_WEB_DESTROY);

			//親オブジェクトを削除
			Parent->Delete();

			for (auto&& player : m_BindObjects)
			{
				if (player.first->GetName() == PLAYER_NAME)
				{
					// アニメーション
					player.first->GetComponent<CPlayer>()->SetAnimState(PLAYER_IDLE);
				}
			}

			return;
		}
	}

	//風と接触(拘束状態ではない)
	if (pObject->GetName() == WIND_NAME && m_BindFlags.size() == 0)//m_bPlayerBindFlg == false)
	{
		//上部分の当たり判定座標初期化
		m_UpColliderPos = XMFLOAT2(m_pCollider->GetCenterPos().x, m_pCollider->GetCenterPos().y + m_pCollider->GetColliderSize().y / 2 * m_pTransform->Scale.y);
		//下部分の当たり判定座標初期化
		m_DownColliderPos = XMFLOAT2(m_pCollider->GetCenterPos().x, m_pCollider->GetCenterPos().y - m_pCollider->GetColliderSize().y / 2 * m_pTransform->Scale.y);

		//上部分と接触
		if (m_bUpColFlg == false &&
			m_UpColliderPos.x <= Other->GetCenterPos().x + OtherSize.x / 2 &&
			m_UpColliderPos.x >= Other->GetCenterPos().x - OtherSize.x / 2 &&
			m_UpColliderPos.y <= Other->GetCenterPos().y + OtherSize.y / 2 &&
			m_UpColliderPos.y >= Other->GetCenterPos().y - OtherSize.y / 2)
		{
			//接触フラグをON
			m_bUpColFlg = true;
			//画像切り替え（上が取れてるクモの巣)
			Parent->GetComponent<CDrawEffect>()->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_TWO_TEX_NUM));

			//クモの巣破壊時SE再生
			CSound::Play(SE_WEB_DESTROY);
		}

		//下部分と接触
		else if (m_bDownColFlg == false &&
			m_DownColliderPos.x <= Other->GetCenterPos().x + OtherSize.x / 2 &&
			m_DownColliderPos.x >= Other->GetCenterPos().x - OtherSize.x / 2 &&
			m_DownColliderPos.y <= Other->GetCenterPos().y + OtherSize.y / 2 &&
			m_DownColliderPos.y >= Other->GetCenterPos().y - OtherSize.y / 2)
		{
			//接触フラグをON
			m_bDownColFlg = true;
			//画像切り替え（下が取れてるクモの巣）
			Parent->GetComponent<CDrawEffect>()->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_THREE_TEX_NUM));
			//クモの巣破壊時SE再生
			CSound::Play(SE_WEB_DESTROY);
		}
	}

	// *** クローバーと接触
	if (pObject->GetName() == CLOVER_NAME && m_BindFlags[pObject] == false)//m_bBindFlg == false)
	{
		//拘束範囲との接触
		if (OwnerPos.x - m_BindSize.x / 2 < Other->GetCenterPos().x + OtherSize.x / 2 &&		// 2の左端 < 1の右端
			Other->GetCenterPos().x - OtherSize.x / 2 < OwnerPos.x + m_BindSize.x / 2)			// 1の左端 < 2の右端
		{
			if (OwnerPos.y - m_BindSize.y / 2 < Other->GetCenterPos().y + OtherSize.y / 2 &&	// 2の上端 < 1の下端
				Other->GetCenterPos().y - OtherSize.y / 2 < OwnerPos.y + m_BindSize.y / 2)		// 1の上端 < 2の下端

			{
				//接触オブジェクトを固定する座標を設定
				m_BindObjects[pObject] = XMFLOAT2(OtherTransform->Pos.x, OtherTransform->Pos.y);
				//拘束フラグをON
				m_BindFlags[pObject] = true;

				// クモの巣に拘束されています
				pObject->GetComponent<CloverComponent>()->Binded(true);
			}

		}
	}
}