/**
 * @file	FadeChangeComponent.cpp
 * @brief	時間経過でポリゴンを表示するコンポーネント
 * @author	志水翔伍
 * @date	2022/01/06
 */

 //===== インクルード =====
#include "FadeChangeComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "polygon.h"

#include "imgui.h"

//===== マクロ定義 =====
#define FADE_COLOR	(XMFLOAT3(1.0f, 1.0f, 1.0f))		// ポリゴンカラー
#define FADE_RATE	(0.02f)

//===== グローバル変数 =====
static EChange g_eChange = CHANGE_IN;			// 状態

// コンストラクタ
CFadeChange::CFadeChange()
	: m_nTimer(0)
	, m_nCmpTimer(0)
	, m_fAlpha(0.0f)
	, m_fColor(FADE_COLOR)
	, m_bFlg(false)
{
}

// デストラクタ
CFadeChange::~CFadeChange()
{

}

// 初期化
void CFadeChange::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw2D = Parent->GetComponent<CDraw2D>();

	g_eChange = CHANGE_IN;
}

// 更新
void CFadeChange::Update()
{
	m_nTimer--;
	if (!m_bFlg && m_nTimer <= 0)
	{
		g_eChange = CHANGE_OUT;
		m_bFlg = true;
	}

	switch (g_eChange)
	{
		case CHANGE_NONE:
							break;
		case CHANGE_OUT:
							m_fAlpha += FADE_RATE;
							if (m_fAlpha >= 1.0f)
							{
								m_fAlpha = 1.0f;
								g_eChange = CHANGE_IN;
							}
							m_pDraw2D->SetAlpha(m_fAlpha);
							break;
		case CHANGE_IN:
							m_fAlpha -= FADE_RATE;
							if (m_fAlpha <= 0.0f)
							{
								m_fAlpha = 0.0f;
								g_eChange = CHANGE_NONE;
								m_nTimer = 0;
								m_nTimer = m_nCmpTimer;
								m_bFlg = false;
							}
							m_pDraw2D->SetAlpha(m_fAlpha);
							break;
	}
}

// 描画
void CFadeChange::Draw()
{
	////	半透明描画はZバッファを更新しない(Zチェックは行う)
	//SetZWrite(false);
	//SetBlendState(BS_ALPHABLEND);

	//// 画面全体に半透明ポリゴンを描画
	//SetPolygonPos(0.0f, 0.0f);
	//SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//SetPolygonTexture(nullptr);
	//SetPolygonFrameSize(1.0f, 1.0f);
	//SetPolygonUV(0.0f, 0.0f);
	//SetPolygonColor(m_fColor.x, m_fColor.y, m_fColor.z);
	////SetPolygonAlpha(m_fAlpha);
	//DrawPolygon(GetDeviceContext());

	//// 元に戻す
	//SetZWrite(true);
	//SetPolygonColor(1.0f, 1.0f, 1.0f);
	//SetPolygonAlpha(1.0f);

#ifdef _DEBUG
	//	デバッグ用
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 240), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	Text("m_nTimer : %d", m_nTimer);
	End();
#endif // _DEBUG
}

// 時間設定
void CFadeChange::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
	m_nCmpTimer = nTimer;
}