/**
 * @file	FadeComponent.cpp
 * @brief	季節のロゴを出した後に徐々に消していく
 * @author	志水翔伍
 * @date	2021/12/22
 */

//===== インクルード =====
#include "FadeComponent.h"
#include "Draw2dComponent.h"

// ===== マクロ定義 =====
#define START_FADE_LOGO		(180)			// フェード開始時間

#define ALPHA_ADDITION		(0.01f)			// 加算(α値)
#define ALPHA_SUBTRACTION	(0.01f)			// 減算(α値)

#define UV_ADDITION			(0.01f)			// 加算(UV座標)
#define UV_SUBTRACTION		(0.01f)			// 減算(UV座標)

#define SCROLL_ADDITION		(0.01f)			// 加算(スクロール座標)

// コンストラクタ
CFade::CFade()
	: m_pDraw2D(nullptr)
	, m_fTime(0.0f)
	, m_fAlpha(0.0f)
	, m_vPos(XMFLOAT2(0.0f, 1.0f))
	, m_vScrPos(XMFLOAT2(0.0f, 0.0f))
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}

// デストラクタ
CFade::~CFade()
{

}

// 初期化
void CFade::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// 更新
void CFade::Update()
{
	m_fTime++;									// 経過時間更新
	if (m_fTime < START_FADE_LOGO)				// 季節のロゴを描画
	{
		m_fAlpha += ALPHA_ADDITION;				// α値加算
		m_vPos.x += UV_ADDITION;				// UV座標(x)を加算
		if (m_fAlpha >= 1.0f)					// α値補正
			m_fAlpha = 1.0f;
		if (m_vPos.x >= 1.0f)					// UV座標補正
			m_vPos.x = 1.0f;
	}
	if (m_fTime >= START_FADE_LOGO)				// 季節のロゴを消していく
	{
		m_fAlpha -= ALPHA_SUBTRACTION;			// α値減らす
		m_vPos.x -= UV_SUBTRACTION;				// UV座標(x)を減算
		m_vScrPos.x += SCROLL_ADDITION;			// スクロール座標(x)を加算
		if (m_vPos.x <= 0.0f)					// UV座標補正
			m_vPos.x = 0.0f;
		if (m_vScrPos.x >= 1.0f)				// スクロール座標補正
			m_vScrPos.x = 1.0f;
		if (m_fAlpha <= 0.0f)					// α値が0.0fを超えた時
			Parent->Delete();					// コンポーネントを削除する
	}
	
	m_pDraw2D->SetUVsize(m_vPos.x, m_vPos.y);	// UV座標設定
	m_pDraw2D->SetScroll(m_vScrPos);			// スクロール設定
	m_pDraw2D->SetAlpha(m_fAlpha);				// α値設定
}

// 描画
void CFade::Draw()
{

}