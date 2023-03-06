/**
 * @file	BGChangeComponent.h
 * @brief	背景が時間経過で変わるコンポーネント
 * @author	志水翔伍
 * @date	2021/12/28
 */

//===== インクルード =====
#include "BGChangeComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "TextureManager.h"
#include "sceneTitle.h"

//===== マクロ定義 =====
#define CHANGE_TIME	(750)

// コンストラクタ
CBgChange::CBgChange()
	: m_nTime(0)
	, m_nCmpTimer(0)
	, m_nTexNum(0)
	, m_bFlg(false)
{

}

// 描画
CBgChange::~CBgChange()
{

}

// 描画
void CBgChange::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// 描画
void CBgChange::Update()
{
	m_nTime--;
	if (!m_bFlg && m_nTime <= 50)
	{
		m_bFlg = true;
		m_nTexNum++;
		ChangeTexture();
	}
	if (m_nTime <= 0)
	{
		m_nTime = m_nCmpTimer;
		m_bFlg = false;
	}
}

// 描画
void CBgChange::Draw()
{

}

// テクスチャ切替
void CBgChange::ChangeTexture()
{
	switch (m_nTexNum % 4)
	{
		case 0:
				SceneTitle::SetSeason(0);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));	// 春
				break;
		case 1:
				SceneTitle::SetSeason(1);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SUMMER_TEX_NUM));	// 夏
				break;
		case 2:
				SceneTitle::SetSeason(2);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_AUTUMN_TEX_NUM));	// 秋
				break;
		case 3:
				SceneTitle::SetSeason(3);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_WINTER_TEX_NUM));	// 冬
				break;
	}
}

// 時間設定
void CBgChange::SetTimer(int nTimer)
{
	//m_nTimer = nTimer * 60;		// 時間設定
	//m_nCmpTimer = nTimer * 60;	// 時間保存
	m_nTime = nTimer;
	m_nCmpTimer = nTimer;
}