/**
 * @file	GetUIComponent.h
 * @brief	アイテムゲット時のUIコンポーネント
 * @author	AT12A 志水翔伍
 * @date	2022/01/29
 */

 //===== インクルード =====
#include "GetUIComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Sound.h"
#include "SceneSelect.h"
#include "SceneManager.h"

//===== マクロ定義 =====
#define LARGE_RATE	(6.0f)

// コンストラクタ
CGetUI::CGetUI()
	: m_vSize(XMFLOAT2(0.0f, 0.0f))
	, m_vMaxSize(XMFLOAT2(0.0f, 0.0f))
	, m_fSizeRate(0.0f)
	, m_nTimer(-1)
{

}

// デストラクタ
CGetUI::~CGetUI()
{

}

// 初期化
void CGetUI::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();	// 座標
	m_pDraw2D = Parent->GetComponent<CDraw2D>();		// 描画
}

// 更新
void CGetUI::Update()
{
	// 遅延
	if (m_nTimer > 0)
	{
		m_nTimer--;

		return;
	}

	// 拡大
	m_vSize.x += m_fSizeRate;				// 横の大きさ
	m_vSize.y += m_fSizeRate;				// 縦の大きさ

	// 拡大補正
	if (m_vSize.x >= m_vMaxSize.x)			// 値の補正(横)
	{
		m_vSize.x = m_vMaxSize.x;			// ↓
	}
	if (m_vSize.y >= m_vMaxSize.y)			// 値の補正(縦)
	{
		m_vSize.y = m_vMaxSize.y;			// ↓
	}
	if (m_vSize.x >= m_vMaxSize.x && m_vSize.y >= m_vMaxSize.y)
	{
		this->Delete();
		if (SceneManager::Instance()->GetScene() == SCENE_SELECT)
		{
			if (Parent->GetName() == CLOVER_UI_NAME ||
				Parent->GetName() == CLEAR_UI_1_NAME ||
				Parent->GetName() == CLEAR_UI_2_NAME ||
				Parent->GetName() == CLEAR_FLOWER_UI_1_NAME ||
				Parent->GetName() == CLEAR_FLOWER_UI_1_NAME) CSound::Play(SE_GET_CLOVER);
		}
	}

	// 大きさ設定
	m_pDraw2D->SetSize(m_vSize.x, m_vSize.y);
}

// 描画
void CGetUI::Draw()
{

}

// セット関数
void CGetUI::SetUISize(XMFLOAT2 vSize)
{
	m_vMaxSize = vSize;		// 大きさ設定
}

// セット関数
void CGetUI::SetUIRate(float fRate)
{
	m_fSizeRate = fRate;	// 拡大スピード設定
}

void CGetUI::SetDelayTimer(int delay)
{
	m_nTimer = delay;
}