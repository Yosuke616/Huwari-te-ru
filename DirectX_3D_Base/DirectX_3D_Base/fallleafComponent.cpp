#include "fallleafComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "main.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "sceneTitle.h"
#include "SceneSelect.h"
#include "SceneManager.h"

#define GRAVITY			(2.0f)		//重力

static XMFLOAT2 g_WorldPos;

// コンストラクタ
CFallLeaf::CFallLeaf()
	: m_pTransform(nullptr)
{
	g_WorldPos = CalcWorldSize();
	m_RotVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// デストラクタ
CFallLeaf::~CFallLeaf()
{
}

// 初期化
void CFallLeaf::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CFallLeaf::Update()
{
	Season Season;
	Season = SceneGame::GetInstance()->GetSeason();

	int nSeason;
	nSeason = SceneTitle::m_nTitleSeason;

	int nSeasonSelect;
	nSeasonSelect = SceneSelect::m_nCurrentSeason;

	switch (SceneManager::Instance()->GetScene())
	{
	case SCENE_TITLE:	//	タイトル画面
	//冬以外
		if (nSeason != 3)
		{
			m_pTransform->Vel.y = -1.0f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	case SCENE_GAME:
		if (Season != WINTER_1 && Season != WINTER_2)
		{
			m_pTransform->Vel.y = -1.0f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	case SCENE_SELECT:	//	セレクト画面
		nSeason = SceneSelect::m_nCurrentSeason;
		if (nSeason != 3)
		{
			m_pTransform->Vel.y = -1.0f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//落下処理
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//横流れ処理

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	}
}

//	回転速度の設定
void CFallLeaf::SetRotVelocity(float x, float y, float z)
{
	m_RotVelocity.x = x;
	m_RotVelocity.y = y;
	m_RotVelocity.z = z;
}