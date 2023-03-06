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


// �R���X�g���N�^
COffScreen::COffScreen()
	: m_pTransform(nullptr), m_pCollider(nullptr)
	, m_eFunction(OFF_SCREEN_DELETE)
{
	this->m_eUpdateOrder = COM_UPDATE_OFF_SCREEN;
}

// �f�X�g���N�^
COffScreen::~COffScreen(){
}

// ������
void COffScreen::Start()
{
	// �I�u�W�F�N�g���W���擾
	m_pTransform = Parent->GetComponent<CTransform>();

	// �����蔻��擾
	m_pCollider = Parent->GetComponent<CCollider>();
}

// �X�V
void COffScreen::Update()
{
	// ���[���h���W�̉�ʓ��̑傫��
	XMFLOAT2 WorldHalfSize = XMFLOAT2(CalcWorldSize().x / 2, CalcWorldSize().y / 2);

	// �J�������W
	XMFLOAT3 CameraPos = CCamera::Get()->GetPos();

	// ��ʊO�ɂ��邩�`�F�b�N
	XMFLOAT2 CenterPos = m_pCollider->GetCenterPos();
	XMFLOAT2 ParentHalfSize = XMFLOAT2(m_pCollider->GetColliderSize().x / 2, m_pCollider->GetColliderSize().y / 2);

	// ��
	if (CenterPos.x - ParentHalfSize.x < CameraPos.x - WorldHalfSize.x)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// ����
		case ON_SCREEN: this->OnScreen(LEFT);	break;	// ��ʓ��ɓ����
		default:
			break;
		}
	}
	// �E
	if (CenterPos.x + ParentHalfSize.x > CameraPos.x + WorldHalfSize.x)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// ����
		case ON_SCREEN: this->OnScreen(RIGHT);	break;	// ��ʓ��ɓ����
		default:
			break;
		}
	}
	// ��
	if (CenterPos.y + ParentHalfSize.y > CameraPos.y + WorldHalfSize.y)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// ����
		case ON_SCREEN: this->OnScreen(UP);	break;		// ��ʓ��ɓ����
		default:
			break;
		}
	}
	// ��
	if (CenterPos.y + ParentHalfSize.y + 10.0f < CameraPos.y - WorldHalfSize.y)
	{
		switch (m_eFunction)
		{
		case OFF_SCREEN_DELETE: this->Delete(); break;	// ����
		case ON_SCREEN: this->OnScreen(DOWN);	break;	// ��ʓ��ɓ����
		default:
			break;
		}
	}
}

// �`��
void COffScreen::Draw() {
}

void COffScreen::SetFinction(OFF_SCREEN fnc)
{
	m_eFunction = fnc;
}

// �폜
void COffScreen::Delete()
{
	Parent->Delete();
}

// ��ʓ��ɓ����
void COffScreen::OnScreen(int dir)
{
	// ���[���h���W�̉�ʓ��̑傫��
	XMFLOAT2 WorldHalfSize = XMFLOAT2(CalcWorldSize().x / 2, CalcWorldSize().y / 2);
	// �J�������W
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
		// �J�E���^�}�C�i�X
		SceneEnding::Cntminus();

		// �I�u�W�F�N�g�폜
		Parent->Delete();
	}
}