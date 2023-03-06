#include "PreviewComponent.h"
#include "Draw2dComponent.h"

// コンストラクタ
CPreview::CPreview()
	: m_bCollision(false), m_bOldCollision(false)
{
}

// デストラクタ
CPreview::~CPreview(){
}

// 初期化
void CPreview::Start(){
}

// 更新
void CPreview::Update()
{
	// もし衝突していたら
	if (m_bCollision)
	{
		// 前フレームに当たっていなかったら
		if (m_bOldCollision == false)
		{

			//プレビュー再生
			Parent->GetComponent<CDrawEffect2D>()->SetPlayONOFF(true);

		}
	}
	else
	{
		// 前フレームに当たっていたら
		if (m_bOldCollision == true)
		{
			//プレビューのアニメーションを０にセット
			Parent->GetComponent<CDrawEffect2D>()->SetAnimNum(0);
			//プレビュー停止
			Parent->GetComponent<CDrawEffect2D>()->SetPlayONOFF(false);
		}


	}

	// 退避
	m_bOldCollision = m_bCollision;

	// 毎フレームフラグをOFF
	m_bCollision = false;
}

// 描画
void CPreview::Draw(){
}

// 衝突処理
void CPreview::OnCollisionEnter(Object* pObject)
{
	// カーソル
	if (pObject->GetName() == CURSOR_NAME)
	{
		m_bCollision = true;
	}
}