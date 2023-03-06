#include "IcicleComponent.h"
#include "GravityComponent.h"
#include "Draw3dComponent.h"
#include "sceneGame.h"
#include "Sound.h"
#include "ObjectInfo.h"
#include "DeleteTimerCom.h"
#include "CloverComponent.h"
#include "InformationComponent.h"

// コンストラクタ
CIcicle::CIcicle()
	: m_pTransform(nullptr)
	, m_nIcicleHP(50)
	, m_bFall(false)
	, m_bMove(true)
{
	m_fLimitDown = -(CalcWorldSize().y / 2);
}

// デストラクタ
CIcicle::~CIcicle()
{
	
}

// 初期化
void CIcicle::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CIcicle::Update()
{
	// 一度だけ重力機能を追加する
	if (!m_bFall && m_nIcicleHP <= 0)
	{
		// オブジェクトに機能を追加
		auto icicles = Parent->AddComponent<CGravity>();
		icicles->Start();

		// 落ちるフラグON
		m_bFall = true;

		//se
		CSound::Play(SE_ICICLE_FALL);	// ←これを追加
	}

	// つららの下部がブロックと接触した時は止める
	if (!m_bMove)
	{
		// 速度を無くす
		m_pTransform->Vel.y = 0.0f;

		//重力コンポーネントの機能停止
		auto G = Parent->GetComponent<CGravity>();
		if (G && G->m_bUpdateFlag == true)
		{
			G->Delete();// = false;
			CSound::Stop(SE_ICICLE_FALL);	// ←これを追加
		}
	}

	//画面下に行かないようにするやつ
	if (m_pTransform->Pos.y - ICICLE_SIZE_Y / 2 < m_fLimitDown)
	{
		CIcicle::Stop();

		//座標を固定させる
		m_pTransform->Pos.y = m_fLimitDown + ICICLE_SIZE_Y / 2;
		m_pTransform->Vel.y = 0.0f;
		//重力コンポーネントの機能停止
		auto G = Parent->GetComponent<CGravity>();
		if (G)
		{
			G->Delete();// m_bUpdateFlag = false;
			CSound::Stop(SE_ICICLE_FALL);	// ←これを追加
		}
	}
}

// 描画
void CIcicle::Draw(){
}

// 衝突機能
void CIcicle::OnCollisionEnter(Object* pObject)
{
	//ブロックに当たったら止まるようにする処理
	if (pObject->GetName() == BLOCK_NAME)
	{
		m_pTransform->Vel.y = 0.0f;
	}

}

// つららの耐久値減少
void CIcicle::IcicleDamage()
{
	m_nIcicleHP--;
	if (m_nIcicleHP < 0)
	{
		m_nIcicleHP = 0;
	}
}

// つららを
void CIcicle::Stop()
{
	m_bMove = false;
}

bool CIcicle::IsFalling()
{
	return m_bFall;
}

bool CIcicle::IsMoving()
{
	return m_bMove;
}