#include "OffScreenComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "main.h"
#include "Camera.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "sceneGame.h"
#include "SceneEnding.h"
#include "Sound.h"

enum DIRECT
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};


// コンストラクタ
COffScreen::COffScreen()
	: m_pTransform(nullptr), m_pCollider(nullptr)
	, m_eFunction(OFF_SCREEN_DELETE)
{
	this->m_eUpdateOrder = COM_UPDATE_OFF_SCREEN;
}

// デストラクタ
COffScreen::~COffScreen(){
}

// 初期化
void COffScreen::Start()
{
	// オブジェクト座標を取得
	m_pTransform = Parent->GetComponent<CTransform>();

	// あたり判定取得
	m_pCollider = Parent->GetComponent<CCollider>();
}

// 更新
void COffScreen::Update()
{
	// ワールド座標の画面内の大きさ
	XMFLOAT2 WorldHalfSize = XMFLOAT2(CalcWorldSize().x / 2, CalcWorldSize().y / 2);

	// カメラ座標
	XMFLOAT3 CameraPos = CCamera::Get()->GetPos();

	// 画面外にあるかチェック
	XMFLOAT2 CenterPos = m_pCollider->GetCenterPos();
	XMFLOAT2 ParentHalfSize = XMFLOAT2(m_pCollider->GetColliderSize().x / 2, m_pCollider->GetColliderSize().y / 2);

	// 左
	if (CenterPos.x - ParentHalfSize.x < CameraPos.x - WorldHalfSize.x)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// 消す
		case ON_SCREEN: this->OnScreen(LEFT);	break;	// 画面内に入れる
		default:
			break;
		}
	}
	// 右
	if (CenterPos.x + ParentHalfSize.x > CameraPos.x + WorldHalfSize.x)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// 消す
		case ON_SCREEN: this->OnScreen(RIGHT);	break;	// 画面内に入れる
		default:
			break;
		}
	}
	// 上
	if (CenterPos.y + ParentHalfSize.y > CameraPos.y + WorldHalfSize.y)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// 消す
		case ON_SCREEN: this->OnScreen(UP);	break;		// 画面内に入れる
		default:
			break;
		}
	}
	// 下
	if (CenterPos.y + ParentHalfSize.y + 10.0f < CameraPos.y - WorldHalfSize.y)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// 消す
		case ON_SCREEN: this->OnScreen(DOWN);	break;	// 画面内に入れる
		default:
			break;
		}
	}
}

// 描画
void COffScreen::Draw() {
}

void COffScreen::SetFinction(OFF_SCREEN fnc)
{
	m_eFunction = fnc;
}

// 削除
void COffScreen::Delete()
{
	Parent->Delete();
}

// 画面内に入れる
void COffScreen::OnScreen(int dir)
{
	// ワールド座標の画面内の大きさ
	XMFLOAT2 WorldHalfSize = XMFLOAT2(CalcWorldSize().x / 2, CalcWorldSize().y / 2);
	// カメラ座標
	XMFLOAT3 CameraPos = CCamera::Get()->GetPos();
	XMFLOAT2 CenterPos = m_pCollider->GetCenterPos();
	XMFLOAT2 ParentHalfSize = XMFLOAT2(m_pCollider->GetColliderSize().x / 2, m_pCollider->GetColliderSize().y / 2);
	XMFLOAT2 Offset = m_pCollider->GetOffSet();

	if (dir == LEFT)
	{
		m_pTransform->Pos.x = CameraPos.x - WorldHalfSize.x + ParentHalfSize.x - Offset.x;

		if (Parent->GetName() == ENDING_CLOVER_NAME) m_pTransform->Vel.x *= -1;
	}
	else if (dir == RIGHT)
	{
		m_pTransform->Pos.x = CameraPos.x + WorldHalfSize.x - ParentHalfSize.x - Offset.x;

		if (Parent->GetName() == ENDING_CLOVER_NAME) m_pTransform->Vel.x *= -1;
	}
	else if (dir == UP)
	{
		m_pTransform->Pos.y = CameraPos.y + WorldHalfSize.y - ParentHalfSize.y - Offset.y;

		if (Parent->GetName() == ENDING_CLOVER_NAME) m_pTransform->Vel.y = 0.0f;
	}
	else if (dir == DOWN && Parent->GetName() == PLAYER_NAME)
	{
		SceneGame::GetInstance()->m_bPauseMode = true;
		MenuManager::GetInstance()->CreateGameoverMenu();
		ObjectManager::GetInstance()->NoFunction();

		// game over
		CSound::Play(SE_GAME_OVER);
	}
	else if (dir == DOWN && Parent->GetName() == ENDING_CLOVER_NAME)
	{
		// カウンタマイナス
		SceneEnding::Cntminus();

		// オブジェクト削除
		Parent->Delete();
	}
}