#include "TexScrollComponent.h"
#include "Draw2dComponent.h"

#include "Camera.h"

#define RATE_CHASE_SCROLL	(0.1f)

// 静的メンバ変数
bool CTexScroll::m_bScroll = true;

// コンストラクタ
CTexScroll::CTexScroll()
	: m_pDraw2D(nullptr)
	, m_pPosX(nullptr), m_pPosY(nullptr)
{
	m_Scroll = XMFLOAT2(0.0f, 0.0f);
	m_UVscroll = XMFLOAT2(0.0f, 0.0f);
	m_ScrollValue = XMFLOAT2(0.0f, 0.0f);
}

// デストラクタ
CTexScroll::~CTexScroll(){
}

// 初期化
void CTexScroll::Start()
{
	//	描画クラスのポインタの取得
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}
// 更新
void CTexScroll::Update()
{
	// スクロールしない場合
	if (!m_bScroll)
	{
		// 現在のUV移動量をセットして終わる
		m_pDraw2D->SetScroll(m_UVscroll);
		return;
	}

	// X方向のスクロール基準が設定されていたら
	if (m_pPosX)
	{
		// 現在のスクロール量と目的のスクロール量の差分を求める
		float differenceX = (*m_pPosX * m_ScrollValue.x) - m_Scroll.x;

		// 今回のスクロール = 今までのスクロール量 + 差分 * 割合
		m_Scroll.x = m_Scroll.x + differenceX * RATE_CHASE_SCROLL;

		// 小数点だけ取り出す
		m_UVscroll.x = m_Scroll.x - (int)m_Scroll.x;
	}
	// Y方向のスクロール基準が設定されていたら
	if (m_pPosY)
	{
		//// 現在のスクロール量と目的のスクロール量の差分を求める
		//float differenceY = (*m_pPosY * m_ScrollValue.y) - m_Scroll.y;
		//
		//// 今回のスクロール = 今までのスクロール量 + 差分 * 割合
		//m_Scroll.y = m_Scroll.y + differenceY * RATE_CHASE_SCROLL;
		//
		//// 小数点だけ取り出す
		//m_UVscroll.y = m_Scroll.y - (int)m_Scroll.y;

		m_UVscroll.y = *m_pPosY;
	}

	// テクスチャのUV移動量をセット
	m_pDraw2D->SetScroll(m_UVscroll);
}

// 描画
void CTexScroll::Draw()
{
}

void CTexScroll::SetAxizX(float* px)
{
	m_pPosX = px;
}
void CTexScroll::SetAxizY(float* py)
{
	m_pPosY = py;
}

void CTexScroll::SetScrollValue(float fx)
{
	m_ScrollValue.x = fx;
}

void CTexScroll::SetScrollValueY(float fy)
{
	m_ScrollValue.y = fy;
}

void  CTexScroll::Scroll(bool bScroll)
{
	m_bScroll = bScroll;
}