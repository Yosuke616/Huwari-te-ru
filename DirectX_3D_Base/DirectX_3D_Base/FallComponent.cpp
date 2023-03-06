#include "FallComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "PlayerComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"

#define R_AND_L_MOVE    (15.0f)

// コンストラクタ
CFall::CFall()
	: m_pTransform(nullptr)
	, m_bActive(true)
	, m_fAngle(0.0f)
{
	//落下しはじめの座標を保存する変数
	m_FallPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// デストラクタ
CFall::~CFall()
{
}

// 初期化
void CFall::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CFall::Update()
{
	//　上昇しているか
	//　上昇していたらフラグをtrueにする
	if (m_pTransform->Vel.y > (BIGLEAFGRAVITY + m_fResistance)) {
		//上昇した段階で基準点をなくす
		m_FallPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fDegree = 0;
		m_bUpFlg = true;
	}

	//	y軸の移動量の緩急
	m_fResistance = SinDeg(m_fDegree * 2) * BIGLEAFGRAVITY;
	m_pTransform->Vel.y -= (BIGLEAFGRAVITY + m_fResistance);
	//	角度の加算
	m_fDegree += 2;
	if (m_fDegree > 180) m_fDegree -= 360;

	//	x軸の加速度
	if (!m_bActive) return;	//	地面にいる時や風に流されているときは更新なし

	//x軸の加速度の下に追加
	//2022/1/3 Shimizu Yosuke --------------------------------------------------------------
	//落ち始めたら揺らすよにする
	if (m_pTransform->Vel.y < 0 && m_bUpFlg == true) {
		//落ち始めの時「偽」に変えて落ち始めたときにしかここに入らないようにする
		m_bUpFlg = false;

		//落ち始めの座標を保存する
		m_FallPos = m_pTransform->Pos;
		//落ち始めた瞬間に0にして毎回落ち始めたときに同じ方向に落ちるようにする
		m_fDegree = 0;
		m_bRorL = false;
	}
	//-------------------------------------------------------------------------------------

	//	落ちているとき
	//落ちているときの下に追加
	//2022/1/3 Shimizu Yosuke ----------------------------------------------------
	//	保存した座標を起点にゆらゆらさせたい
	//起点より進んだら反対方向に切り返すようにする
	//一回だけ返れるようにする
	//右に進んだパターン
	if (m_pTransform->Pos.x > m_FallPos.x + R_AND_L_MOVE && m_bRorL == false) {
		m_fDegree = -180.0f;
		m_bRorL = true;
	}
	//左に進んだパターン
	if (m_pTransform->Pos.x < m_FallPos.x - R_AND_L_MOVE && m_bRorL == true) {
		m_fDegree = 0;
		m_bRorL = false;
	}
	m_pTransform->Vel.x += SinDeg(m_fDegree) * VIBRATION;	//	左右に揺れる

	//----------------------------------------------------------------------------

	//	落ちてくアニメーション
	auto animation = Parent->GetComponent<CPlayer>();
	if(animation)
		animation->SetAnimState(PLAYER_FALL);


	//	地面フラグ
	auto Player = Parent->GetComponent<CPlayer>();
	if (Player) Player->SetGround(false);

	//ゲームオーバー文字の停止
	if (Parent->GetName() == GOLETTER_NAME)
	{
		CCollider* letterstop = Parent->GetComponent<CCollider>();
		auto stoppos = letterstop->GetCenterPos();		// 文字の中心座標
		if (stoppos.y < -250.0f)
		{
			m_pTransform->Vel.x = 0.0f;
			m_pTransform->Vel.y = 0.0f;
		}
	}
}


void CFall::SetActive(bool bActive)
{
	m_bActive = bActive;
}