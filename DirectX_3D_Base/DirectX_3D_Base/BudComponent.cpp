#include "BudComponent.h"
#include "Draw3dComponent.h"

// コンストラクタ
CBud::CBud()
	: m_bCollision(false), m_bOldCollision(false), m_bOpen(false)
{
}

// デストラクタ
CBud::~CBud(){
}

// 初期化
void CBud::Start(){
}

// 更新
void CBud::Update()
{
	// もし衝突していたら
	if (m_bCollision)
	{
		// 前フレームに当たっていなかったら
		if (m_bOldCollision == false)
		{
			// アニメーション再生
			Parent->GetComponent<CDrawModel>()->Play(0);

			// 開きました
			m_bOpen = true;
		}
	}
	else
	{
		// 前フレームに当たっていたら
		if (m_bOldCollision == true)
		{
			// アニメーション再生
			Parent->GetComponent<CDrawModel>()->Play(1);
		}
		// 開いていません
		m_bOpen = false;
	}

	// 退避
	m_bOldCollision = m_bCollision;

	// 毎フレームフラグをOFF
	m_bCollision = false;
}

// 描画
void CBud::Draw(){
}

// 衝突処理
void CBud::OnCollisionEnter(Object* pObject)
{
	// プレイヤー
	if (pObject->GetName() == PLAYER_NAME)
	{
		// つぼみを開く
		m_bCollision = true;
	}
}