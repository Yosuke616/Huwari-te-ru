#include "fallleafComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "main.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "sceneTitle.h"
#include "SceneSelect.h"
#include "SceneManager.h"

#define GRAVITY			(2.0f)		//�d��

static XMFLOAT2 g_WorldPos;

// �R���X�g���N�^
CFallLeaf::CFallLeaf()
	: m_pTransform(nullptr)
{
	g_WorldPos = CalcWorldSize();
	m_RotVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
CFallLeaf::~CFallLeaf()
{
}

// ������
void CFallLeaf::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
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
	case SCENE_TITLE:	//	�^�C�g�����
	//�~�ȊO
		if (nSeason != 3)
		{
			m_pTransform->Vel.y = -1.0f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	case SCENE_GAME:
		if (Season != WINTER_1 && Season != WINTER_2)
		{
			m_pTransform->Vel.y = -1.0f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	case SCENE_SELECT:	//	�Z���N�g���
		nSeason = SceneSelect::m_nCurrentSeason;
		if (nSeason != 3)
		{
			m_pTransform->Vel.y = -1.0f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.01f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();

		}
		else {
			m_pTransform->Vel.y = -0.5f;		//��������
			m_nRandX = rand() % 10;
			m_pTransform->Vel.x += ((float)m_nRandX - 5.0f)* 0.001f;		//�����ꏈ��

			m_pTransform->Rotate.x += m_RotVelocity.x;
			m_pTransform->Rotate.y += m_RotVelocity.y;
			m_pTransform->Rotate.z += m_RotVelocity.z;
			if (m_pTransform->Pos.y < -g_WorldPos.y *0.5f) Parent->Delete();
		}
		break;
	}
}

//	��]���x�̐ݒ�
void CFallLeaf::SetRotVelocity(float x, float y, float z)
{
	m_RotVelocity.x = x;
	m_RotVelocity.y = y;
	m_RotVelocity.z = z;
}