#include "OutRangeCom.h"
#include "TransformComponent.h"
#include "Camera.h"


// 中心から距離


// コンストラクタ
COutRange::COutRange()
	: m_pTransform(nullptr)
{
	m_LimitRange.x = OUT_RANGE_X;
	m_LimitRange.y = OUT_RANGE_Y;
}

// デストラクタ
COutRange::~COutRange(){
}

// 初期化
void COutRange::Start()
{
	// オブジェクト座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
}
// 更新
void COutRange::Update()
{
	// カメラを基準にして
	// そこから一定キョリ離れたオブジェクトの機能を停止させる

	// 画面の中心座標
	XMFLOAT3 Center = CCamera::Get()->GetPos();

	// 一定距離離れていたら更新を止める
	if (Center.x + m_LimitRange.x < m_pTransform->Pos.x ||	
		Center.x - m_LimitRange.x > m_pTransform->Pos.x)	
		//Center.y + m_LimitRange.y < m_pTransform->Pos.y ||
		//Center.y - m_LimitRange.y > m_pTransform->Pos.y)	
	{
		// 機能停止
		Parent->StopUpdate();
		Parent->StopDraw();

		// このコンポーネントだけ更新する
		this->m_bUpdateFlag = true;
	}
	// 一定キョリ以内にある場合、更新をする
	else
	{
		// 更新を再開
		Parent->Use();
	}
}
// 描画
void COutRange::Draw()
{

}
// 衝突処理
void COutRange::OnCollisionEnter(Object* pObject)
{

}


// このコンポーネント以外のオブジェクトの機能を停止させる
void COutRange::NoFunction()
{
	// 機能停止
	Parent->NoUse();

	// このコンポーネントだけ更新する
	this->m_bUpdateFlag = true;
}

void COutRange::SetLimitRange(float x, float y)
{
	m_LimitRange.x = x;
	m_LimitRange.y = y;
}