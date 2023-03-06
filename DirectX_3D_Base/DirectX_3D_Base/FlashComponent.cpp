/**
 * @file	FlashComponent.h
 * @brief	点滅(α値)をいじる
 * @author	AT12A 志水翔伍
 * @date	2022/01/28
 */

//===== インクルード =====
#include "FlashComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"

//===== マクロ定義 =====
#define ALPHA_RATE	(0.01f)

// コンストラクタ
CFlash::CFlash()
	: m_pTransform(nullptr)
	, m_pDraw2D(nullptr)
	, m_fAlpha(1.0f)
	, m_nDrawTime(0)
	, m_nNoneTime(0)
	, m_nKeepAlpha(0)
	, m_nKeepNone(0)
{

}

// デストラクタ
CFlash::~CFlash()
{

}

// 初期化
void CFlash::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();	// 座標
	m_pDraw2D = Parent->GetComponent<CDraw2D>();		// 描画
}

// 更新
void CFlash::Update()
{
	// 描画しない
	if (m_nNoneTime > 0)
	{
		m_nNoneTime--;							// 時間減算
		if (m_nNoneTime == 0)					// 描画しない時間が無くなったとき
		{
			m_nNoneTime = 0;					// 描画しないフラグ削除
			m_nDrawTime = m_nKeepAlpha;
			//m_nDrawTime = rand() % 600 + 600;	// 描画時間指定
		}
		m_fAlpha -= ALPHA_RATE;
		if (m_fAlpha < 0.0f)					// α値補正
			m_fAlpha = 0.0f;					// ↓
		m_pDraw2D->SetAlpha(m_fAlpha);			// α値なし
		return;									// 以降の処理をさせない
	}
	
	// 描画する
	if (m_nDrawTime > 0)
	{
		m_nDrawTime--;							// 描画時間減算
		if (m_nDrawTime == 0)					// 描画時間が無くなったとき
		{
			m_nDrawTime = 0;					// 描画時間なし
			m_nNoneTime = m_nKeepNone;
			//m_nNoneTime = rand() % 600 + 600;	// 描画しない時間設定
		}
		m_fAlpha += ALPHA_RATE;					// α値加算
		if (m_fAlpha >= 1.0f)					// α値補正
			m_fAlpha = 1.0f;					// ↓
		m_pDraw2D->SetAlpha(m_fAlpha);			// α値設定
	}

}

// 描画
void CFlash::Draw()
{

}

// セット関数
void CFlash::SetAlphaTime(int nTime)
{
	m_nDrawTime = nTime * 60;		// 描画時間設定
	m_nKeepAlpha = nTime * 60;		// 描画時間保存用
}

// セット関数
void CFlash::SetNoneTime(int nTime)
{
	m_nNoneTime = nTime * 60;		// 描画しない時間設定
	m_nKeepNone = nTime * 60;		// 描画しない時間保存用
}