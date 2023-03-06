#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"
#include "fade.h"

#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "FallComponent.h"
#include "WindComponent.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "GaugeComponent.h"
#include "MenuManager.h"
#include "sceneGame.h"
#include "ModelManager.h"
#include "SceneSelect.h"
#include "Sound.h"
#include "Camera.h"
#include "WindManager.h"
#include "IceBlockCom.h"
#include "IcicleComponent.h"
#include "InformationComponent.h"
#include "CGameover.h"
#include "EffectManager.h"
#include "CloverComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"

//2022/1/26/Touch_Tutorial Shimizu Yosuke ----------------------
#include "WindManager.h"
#include "sceneGame.h"
#include "sceneTitle.h"
//--------------------------------------------------------------

#include "GetUIComponent.h"

//***�ÓI�����o�ϐ�
CGoletter Goletter;


#define MAX_PLAYER_VELOCITY_Y	(1.3f)		// y���̑��x����
#define MAX_PLAYER_GRAVITY		(1.0f)		// �v���C���[�̏d��
#define	RATE_MOVE_MODEL			(0.03f)		// �ړ������W��

#define	VALUE_MOVE_MODEL	(1.0f)		// �ړ����x
#define	VALUE_ROTATE_MODEL	(9.0f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.10f)		// ��]�����W��



#ifdef _DEBUG
#include "Camera.h"
#endif // _DEBUG


// �R���X�g���N�^
CPlayer::CPlayer()
	: m_pPlayer(nullptr), m_pDraw3D(nullptr)
	, m_fDiifRotY(0.0f)
	, m_bGoalFlg(false), m_bGround(false)
	, m_OldPos(0.0f,0.0f)
	, m_nAnimState(0)
	, m_nColWindCnt(-1)
{
	this->m_eUpdateOrder = COM_UPDATE_1;

	for (int i = 0; i < 4; i++)
	{
		m_bClover[i] = false;
	}

	// �N���[�o�[�擾�󋵂̊m�F
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	int nStage = SceneGame::GetInstance()->GetSeason() * 4;
	for (int i = 0; i < 4; i++)
	{
		m_bClover[i] = CloverCSV->GetInt(nStage + i, 0);

		if (m_bClover[i])
		{
			//UI��ǉ�����
			Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
			//�R���|�[�l���g����������
			auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
			auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
			//�I�u�W�F�N�g�̐ݒ�
			//�����Ń|�W�V�����̐ݒ������
			switch (i) {
			case 0:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
				break;
			case 1:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
				break;
			case 2:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
				break;
			case 3:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
				break;
			default: break;
			}

			DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_TEX_NUM));
			DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pUI_Clover);
		}
	}

	m_Delete_UI.resize(0);
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
}

// �J�n
void CPlayer::Start()
{
	m_pDraw3D = Parent->GetComponent<CDrawModel>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_Collider = Parent->GetComponent<CCollider>();
	m_pPlayer->Rotate.y = -45.0f;
	m_rotDestModel.y = -45.0f;
}

// �X�V
void CPlayer::Update()
{
	m_OldPos = m_Collider->GetCenterPos();

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

#ifdef _DEBUG
	if (INPUT->GetKeyPress(DIK_A))
	{
		// ���ړ�
		m_pPlayer->Vel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
	
		m_rotDestModel.y = rotCamera.y + 45.0f;
	}
	else if (INPUT->GetKeyPress(DIK_D))
	{
		//	�E�ړ�
		m_pPlayer->Vel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
	
		m_rotDestModel.y = rotCamera.y + -45.0f;
	}

	if (INPUT->GetKeyPress(DIK_W))
	{
		m_pPlayer->Vel.y = 1.0f;
	}
	else if (INPUT->GetKeyPress(DIK_W))
	{
		m_pPlayer->Vel.y = -1.0f;
	}
#endif // _DEBUG

	// �ړI�̊p�x�܂ł̍���
	m_fDiifRotY = m_rotDestModel.y - m_pPlayer->Rotate.y;
	if (m_fDiifRotY >= 180.0f) m_fDiifRotY -= 360.0f;
	if (m_fDiifRotY < -180.0f) m_fDiifRotY += 360.0f;
	// �ړI�̊p�x�܂Ŋ�����������
	m_pPlayer->Rotate.y += m_fDiifRotY * RATE_ROTATE_MODEL;
	if (m_pPlayer->Rotate.y > 360.0f)	m_pPlayer->Rotate.y -= 360.0f;
	if (m_pPlayer->Rotate.y < 0.0f)		m_pPlayer->Rotate.y += 360.0f;

	// X�ړ��ʂɊ�����������
	m_pPlayer->Vel.x += -m_pPlayer->Vel.x * RATE_MOVE_MODEL;

	//	���x���
	if (m_pPlayer->Vel.x > MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.x = MAX_PLAYER_VELOCITY_Y;
	if (m_pPlayer->Vel.x < -MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.x =  -MAX_PLAYER_VELOCITY_Y;

	if (m_pPlayer->Vel.y > MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.y = MAX_PLAYER_VELOCITY_Y;
	if (m_pPlayer->Vel.y < -MAX_PLAYER_GRAVITY)	 m_pPlayer->Vel.y = -MAX_PLAYER_GRAVITY;


	//	���E�ɗh���t���O
	if (m_pPlayer->Vel.y < 0.0f)
	{
		auto Leaf = Parent->GetComponent<CFall>();
		if (Leaf) Leaf->SetActive(true);
	}
	else
	{
		//	�n��t���OOFF
		m_bGround = false;
	}

	// �A�j���[�V��������	2022/01/11
	if (m_nColWindCnt > 0)
	{
		m_nColWindCnt--;
		if (m_nColWindCnt == 0)
		{
			// ���ɐ�����Ă����Ԃ�������
			if (m_nAnimState == PLAYER_CARRIED)
			{
				//	�n��t���OOFF
				m_bGround = false;
			}

			// ���Z�b�g
			m_nColWindCnt = -1;
		}
	}

	// �A�j���[�V�������I�����
	CAnimation* pAnim = Parent->GetComponent<CAnimation>();
	if (pAnim && pAnim->GetMoel(FBX_ONE)->AnimEnd(PLAYER_GET) == true)
	{
		SetAnimState(PLAYER_IDLE);
	}

	if (m_nAnimState == PLAYER_GET)
	{
		m_pPlayer->Vel.x = 0.0f;
		m_pPlayer->Vel.y = 0.0f;
	}
	
	if (pAnim)
	{
		// �A�j���[�V�����Đ�
		pAnim->GetMoel(FBX_ONE)->Play(m_nAnimState);
	}
	
	
}

//	�`��
void CPlayer::Draw()
{
#ifdef _DEBUG
	//	debug
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Player");
	Text("Pos	: %3.0f %3.0f %3.0f", m_pPlayer->Pos.x, m_pPlayer->Pos.y, m_pPlayer->Pos.z);
	Text("Vel	: %.0f %.0f", m_pPlayer->Vel.x, m_pPlayer->Vel.y);
	Text("goal   %d", m_bGoalFlg);
	Text("ground %d", m_bGround);
	End();
#endif // _DEBUG
}

//	�Փˏ���
void CPlayer::OnCollisionEnter(Object* pObject)
{
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ----------------------------------------------
	//�`���[�g���A�����o�����߂̏ꏊ
	//��񂵂��ʂ炸�A�����o����悤�ɂ���
	//������ʂ�Ȃ��ƕ��͏o���Ȃ�
	//���̃t���O��false�Œn�ʂɓ��������Ƃ��ɃI���ɂ���
	int nSeason = SceneGame::GetInstance()->GetSeason();
	if (SceneTitle::FirstPlay(nSeason) == true)
	{
		if (!m_bTouch_Tutorial) {
			if (pObject->GetName() == BLOCK_NAME)
			{
				SceneTitle::SaveTutorial();

				//  �`���[�g���A���t���OON
				m_bTouch_Tutorial = true;

				//�`���[�g���A���̃t���O���I���ɂ��ă`���[�g���A�����o����悤�ɂ���
				SceneGame::GetInstance()->SetTutorial(true);
			}
		}
	}
	//---------------------------------------------------------------------------------------

#pragma region ---�A�ؔ��S�[��
	if (pObject->GetName() == POT_NAME)
	{
		//	�S�[���ł����Ԃ�������
		if (m_bGoalFlg && m_bGround)
		{
			m_bGoalFlg = false;

			m_rotDestModel.y = -45.0f;

			// 2022/01/16
			// �J�����̈ʒu��ς���
			CCamera::Get()->SetDestPos(GOAL_POS_Z);

			//	�A�j���[�V���� = �N���A
			auto Anime = Parent->GetComponent<CAnimation>();
			if (Anime)
			{
				Anime->ChangeAnimation(FBX_TWO);
				Anime->Update();	//	�A�j���[�V�����X�V
			}
			//	�v���C���[�@�\��~
			Parent->StopUpdate();
			// �A�j���[�V�����@�\�����@�\������
			Parent->GetComponent<CAnimation>()->GetMoel(FBX_TWO)->m_bUpdateFlag = true;
			Parent->GetComponent<CAnimation>()->GetMoel(FBX_TWO)->Play(PLAYER_GOAL);

#pragma region ---�Ԃ��炩����
			Object* pFlower = new Object("Flower", UPDATE_MODEL, DRAW_MODEL);
			// �R���|�[�l���g�ǉ�
			auto Trans = pFlower->AddComponent<CTransform>();		// �I�u�W�F�N�g�̍��W
			auto Draw = pFlower->AddComponent<CDrawModel>();			// ���f���`��
			// �I�u�W�F�N�g�ݒ�
			Trans->Pos = pObject->GetComponent<CTransform>()->Pos;	// ���W
			Trans->Pos.x += FLOAER_POS_X_OFFSET;					// ���W
			Trans->Pos.y += FLOAER_POS_Y_OFFSET;					// ���W
			Trans->Pos.z += FLOAER_POS_Z_OFFSET;					// ���W
			Trans->SetScale(FLOAER_SCALE_X, FLOAER_SCALE_Y, FLOAER_SCALE_Z);// �X�P�[��
			Trans->SetRotate(FLOAER_ROTATE_X, 0.0f, 0.0f);
			// �l�G�ɂ���ăe�N�X�`����ς���
			ModelManager* pModelManager = ModelManager::GetInstance();
			switch (SceneGame::GetInstance()->GetSeason())
			{
			case SPRING_1:	case SPRING_2:	// �t
				Draw->SetModel(pModelManager->GetModel(SPRING_FLOWER_MODEL_NUM));
				break;
			case SUMMER_1:	case SUMMER_2:	// ��
				Draw->SetModel(pModelManager->GetModel(SUMMER_FLOWER_MODEL_NUM));
				break;
			case FALL_1:	case FALL_2:	// �H
				Draw->SetModel(pModelManager->GetModel(FALL_FLOWER_MODEL_NUM));
				break;
			case WINTER_1:	case WINTER_2:	// �~
				Draw->SetModel(pModelManager->GetModel(WINTER_FLOWER_MODEL_NUM));
				break;
			default:break;
			}
			// �A�j���[�V����
			Draw->AddAnimation(0, FLOAER_ANIM_S, FLOAER_ANIM_E, false);
			Draw->Play(0);
			// ���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pFlower);
#pragma endregion

			// ���̃X�e�[�W���
			SceneSelect::SaveClearInfo(SceneGame::GetInstance()->GetSeason(), m_bClover);


			//	BGM��~
			CSound::Stop(BGM_SPRING);
			CSound::Stop(BGM_SUMMER);
			CSound::Stop(BGM_FALL);
			CSound::Stop(BGM_WINTER);

			// �N���A�̉� 
			CSound::Play((SE_GAME_CLEAR));


			// �S�[���G�t�F�N�g
			// �ʏ�Ver.
			EffectManager::GetInstance()->Play(SIMPLE_GOAL_EFFECT_NUM, Trans->Pos, XMFLOAT3(20.0f, 20.0f, 20.0f));
			bool bAllClover = true;
			for (int i = 0; i < 4; i++)
			{
				if (m_bClover[i] == false) bAllClover = false;
			}
			if (bAllClover)
			{
				// �N���[Ver.
				EffectManager::GetInstance()->Play(CLOVER_GOAL_EFFECT_NUM, Trans->Pos, XMFLOAT3(20.0f, 20.0f, 20.0f));
			}


			// �Ō�̃X�e�[�W��������
			if (SceneGame::GetInstance()->GetSeason() == WINTER_2)
			{
				//	�V�[���J�ڗ\��
				StartFadeOutSceneChange(SCENE_ENDING, 240);
			}
			else
			{
				//	�V�[���J�ڗ\��
				StartFadeOutSceneChange(SCENE_SELECT, 240);
			}

			

			

			// ���j���[���J���{�^��������
			Object* pMenu = ObjectManager::GetInstance()->GetGameObject(STOP_GAME_NAME);
			if (pMenu) pMenu->Delete();

			//���j���[���J���{�^���������̉��ɒǉ�
//2022/1/26/Touch_Tutorial Shimizu Yosuke --------------------------------
			Object* pGauge = ObjectManager::GetInstance()->GetGameObject("Gauge");
			if (pGauge) pGauge->Delete();
			Object* pGauge_frame = ObjectManager::GetInstance()->GetGameObject("frame");
			if (pGauge_frame) pGauge_frame->Delete();
			//�V�������X�g�����
			//����Ă��ꂽ�֐��Ń��X�g�̒��ɒǉ�����
			ObjectManager::GetInstance()->GetGameObjects(SEED_UI_NAME, m_Delete_UI);
			//���g�̃f���[�g�t���O���I���ɂ��Ă���
			ObjectManager::GetInstance()->DeleteObjectByName(SEED_UI_NAME, m_Delete_UI);

			//�g���܂킵��������0�ɖ߂�
			m_Delete_UI.resize(0);

			//����Ă��ꂽ�֐��Ń��X�g�̒��ɒǉ�����2
			ObjectManager::GetInstance()->GetGameObjects(CLOVER_UI_NAME, m_Delete_UI);
			//���g�̃f���[�g�t���O���I���ɂ��Ă���
			ObjectManager::GetInstance()->DeleteObjectByName(CLOVER_UI_NAME, m_Delete_UI);

			Object* pWindCar = ObjectManager::GetInstance()->GetGameObject(WINDCAR_NAME);
			if (pWindCar) pWindCar->Delete();
			Object* pWindCar_Stick = ObjectManager::GetInstance()->GetGameObject(WINDCAR_STICK_NAME);
			if (pWindCar_Stick) pWindCar_Stick->Delete();
			//------------------------------------------------------------------------


		}

		return;
	}
#pragma endregion

#pragma region ---�u���b�N
	if (pObject->GetName() == BLOCK_NAME)
	{
		//	�v���C���[�̏����擾
		auto Owner = Parent->GetComponent<CCollider>();		//	�����蔻��̑傫�����~����
		auto OwnerPos = Owner->GetCenterPos();				// ���S���W
		auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
		auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
		//	�Ԃ���������̏��̎擾
		auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
		auto OtherPos = Other->GetCenterPos();				// ���S���W
		auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
		auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

		//	���ꂼ��̔����̑傫�����i�[
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize  = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);


		//	�u���b�N�̂����蔻�胉�C��
		float BlockLeftLine	 = OtherPos.x - BlockHalfSize.x;	//	�u���b�N�̍��[
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;	//		�@  �E�[
		float BlockUpLine	 = OtherPos.y + BlockHalfSize.y;	//		�@  ��[
		float BlockDownLine	 = OtherPos.y - BlockHalfSize.y;	//		�@  ���[

		/*		   __
			�@	�Z |P|				
				�@ |_|			 __
			_________________ �~ |P|
		  �@|���@�u���b�N�̉��� ��|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
		{
			//	P
			//	��
			//	��
			if (//m_pPlayer->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
							   OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;

				// ���W�␳
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

				// ���̂�
				if (!m_bGround)
				{
					//�����ŕ����o����悤�ɂ���
					WindManager::GetInstance()->SetSpawnWind(true);

					// �A�j���[�V���� = �ҋ@���[�V����
					SetAnimState(PLAYER_IDLE);
				}

				// �n��t���OON
				m_bGround = true;

				// �h���t���OOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);

				// �Q�[���I�[�o�[�̊m�F
				if (WindManager::GetInstance()->GetWindGauge() <= 0)
				{
					//	BGM��~
					CSound::Stop(BGM_SPRING);
					CSound::Stop(BGM_SUMMER);
					CSound::Stop(BGM_FALL);
					CSound::Stop(BGM_WINTER);

					CCamera::Get()->SetDestPos(GOAL_POS_Z);		//�Y�[��

					//�Q�[���I�[�o�[�����̕\��
					Goletter.CreateGoletter();

					ObjectManager::GetInstance()->NoFunction();

					//�A�j���[�V����
					CAnimation* pAnim = Parent->GetComponent<CAnimation>();
					if (pAnim)
					{
						pAnim->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);

						// ��ɃA�j���[�V����������̂���߂�
						pAnim->GetMoel(FBX_ONE)->AlwaysAnimation(false);
						//	�v���C���[�@�\��~
						Parent->StopUpdate();
						// �A�j���[�V��������������
						pAnim->GetMoel(FBX_ONE)->m_bUpdateFlag = true;
					}

					// game over
					CSound::Play(SE_GAME_OVER);

					// ���j���[�֘A
					MenuManager::GetInstance()->Create(STATE_GAMEOVER, 350);
				}
			}
			//	��
			//	��
			//	P
			else if (//m_pPlayer->Vel.y > 0 &&
					 BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
									  OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;

				// ���W�␳
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// ���S���W

		/*
				�~ __
				  |P|
				  |_|
		  __
		 �@�u|
		 �@��|	�Z __
		�@ �b|	  |P|
		�@ �N|	  |_|
		  __|	
		  		
		*/		
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// P����
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&				// �O�͂߂荞��ł��Ȃ�
								 OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)	// ���͂߂荞��ł�
			{
				//	x�ړ��ʂ�����
				m_pPlayer->Vel.x = 0.0f;
			
				// ���W�␳
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
			// ����P
			else if(BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
									  OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
			{
				//	x�ړ��ʂ�����
				m_pPlayer->Vel.x = 0.0f;
			
				// ���W�␳
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
		}


		return;
	}
#pragma endregion

#pragma region ---���
	if (pObject->GetName() == ICICLE_UP_NAME ||
		pObject->GetName() == ICICLE_DOWN_NAME)
	{
		//	�v���C���[�̏����擾
		auto Owner = Parent->GetComponent<CCollider>();		//	�����蔻��̑傫�����~����
		auto OwnerPos = Owner->GetCenterPos();				// ���S���W
		auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
		auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
		//	�Ԃ���������̏��̎擾
		auto OtherTrans = pObject->GetComponent<CTransform>();
		auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
		auto OtherPos = Other->GetCenterPos();				// ���S���W
		auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
		auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

		//	���ꂼ��̔����̑傫�����i�[
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);


		//	�u���b�N�̂����蔻�胉�C��
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;		//	�u���b�N�̍��[
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;	//		�@  �E�[
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;		//		�@  ��[
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;		//		�@  ���[

		// �����������ׂ�
		CIcicle* pIcicle;
		if (pObject->GetName() == ICICLE_DOWN_NAME)	pIcicle = pObject->GetComponent<CInformation>()->GetObjetInfo()->GetComponent<CIcicle>();
		else										pIcicle = nullptr;
		if (pObject->GetName() == ICICLE_UP_NAME)	pIcicle = pObject->GetComponent<CIcicle>();

		// ��炪�������@�������́@�������������ƂȂ�v���[���[�����E�ɒǂ��o��
		if (pIcicle && pIcicle->IsFalling() && pIcicle->IsMoving())
		{
			// �����E���ɂ���Ƃ��E�ɂ��炵�Ă���
			if (OwnerPos.x > OtherPos.x)
			{
				m_pPlayer->Vel.x += 0.5f;
			}
			else if (OwnerPos.x < OtherPos.x)
			{
				m_pPlayer->Vel.x += -0.5f;
			}
		}
		// ��炪�������łȂ��Ƃ��͕��ʂ̃u���b�N�Ɠ�������
		else
		{
			/*		   __
			�@	�Z |P|
				�@ |_|			 __
			_________________ �~ |P|
		  �@|���@�u���b�N�̉��� ��|   |_|
		*/
			if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
				(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
				(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
			{
				//	P
				//	��
				//	��
				if (//m_pPlayer->Vel.y < 0 &&
					BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
					OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// ���͂߂荞��ł�
				{
					// y�̑��x���Ȃ���
					m_pPlayer->Vel.y = 0.0f;

					// ���W�␳
					m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

					// ���̂�
					if (!m_bGround)
					{
						// �A�j���[�V���� = �ҋ@���[�V����
						SetAnimState(PLAYER_IDLE);
					}

					// �n��t���OON
					m_bGround = true;

					// �h���t���OOFF
					auto Leaf = Parent->GetComponent<CFall>();
					if (Leaf) Leaf->SetActive(false);

					// �Q�[���I�[�o�[�̊m�F
					if (WindManager::GetInstance()->GetWindGauge() <= 0)
					{
						//�A�j���[�V����
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);
						// ��ɃA�j���[�V����������̂���߂�
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->AlwaysAnimation(false);
						//	�v���C���[�@�\��~
						Parent->StopUpdate();
						// �A�j���[�V��������������
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->m_bUpdateFlag = true;

						// game over
						CSound::Play(SE_GAME_OVER);

						// ���j���[�֘A
						MenuManager::GetInstance()->Create(STATE_GAMEOVER, 240);
					}
				}
				//	��
				//	��
				//	P
				else if (//m_pPlayer->Vel.y > 0 &&
					BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
					OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// ���͂߂荞��ł�
				{
					// y�̑��x���Ȃ���
					m_pPlayer->Vel.y = 0.0f;

					// ���W�␳
					m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
				}
			}


			OwnerPos = Owner->GetCenterPos();				// ���S���W

			/*
					�~ __
					  |P|
					  |_|
			  __
			 �@�u|
			 �@��|	�Z __
			�@ �b|	  |P|
			�@ �N|	  |_|
			  __|

			*/
			if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
			{
				// P����
				if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&				// �O�͂߂荞��ł��Ȃ�
					OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)	// ���͂߂荞��ł�
				{
					//	x�ړ��ʂ�����
					m_pPlayer->Vel.x = 0.0f;

					// ���W�␳
					m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
				}
				// ����P
				else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
					OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
				{
					//	x�ړ��ʂ�����
					m_pPlayer->Vel.x = 0.0f;

					// ���W�␳
					m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
				}
			}
		}

		

		return;
	}
#pragma endregion

#pragma region ---�X�u���b�N
	if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	�v���C���[�̏����擾
		auto Owner = Parent->GetComponent<CCollider>();		//	�����蔻��̑傫�����~����
		auto OwnerPos = Owner->GetCenterPos();				// ���S���W
		auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
		auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
		//	�Ԃ���������̏��̎擾
		auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
		auto OtherPos = Other->GetCenterPos();				// ���S���W
		auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
		auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

		//	���ꂼ��̔����̑傫�����i�[
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

		//	�u���b�N�̂����蔻�胉�C��
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	�u���b�N�̍��[
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		�@  �E�[
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		�@  ��[
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		�@  ���[

		/*		   __
			�@	�Z |P|
				�@ |_|			 __
			_________________ �~ |P|
		  �@|���@�u���b�N�̉��� ��|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
		{
			//	P
			//	��
			//	��
			if (m_pPlayer->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&	// �O�͂߂荞��ł��Ȃ�
				OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;

				// ���W�␳
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

				// ���̂�
				if (!m_bGround)
				{
					// �A�j���[�V���� = �ҋ@���[�V����
					SetAnimState(PLAYER_IDLE);
				}

				// �n��t���OON
				m_bGround = true;

				// �h���t���OOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);

				// �Q�[���I�[�o�[�̊m�F
				if (WindManager::GetInstance()->GetWindGauge() <= 0)
				{
					//�A�j���[�V����
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);
					// ��ɃA�j���[�V����������̂���߂�
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->AlwaysAnimation(false);
					//	�v���C���[�@�\��~
					Parent->StopUpdate();
					// �A�j���[�V��������������
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->m_bUpdateFlag = true;

					// game over
					CSound::Play(SE_GAME_OVER);

					// ���j���[�֘A
					MenuManager::GetInstance()->Create(STATE_GAMEOVER, 240);
				}
			}
			//	��
			//	��
			//	P
			else if (//m_pPlayer->Vel.y > 0 &&
				BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
				OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;

				// ���W�␳
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// ���S���W

		/*
				�~ __
				  |P|
				  |_|
		  __
		 �@�u|
		 �@��|	�Z __
		�@ �b|	  |P|
		�@ �N|	  |_|
		  __|

		*/
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// ���͂���
			m_pPlayer->Vel.x = pObject->GetComponent<CTransform>()->Vel.x;	// ���x���Փ˂����I�u�W�F�N�g�Ɉڂ�
			pObject->GetComponent<CTransform>()->Vel.x = 0.0f;				// �ڂ������̑��x������

			// ���W�␳
			// �X�u���b�N�̑��x�Ō��߂�
			if (OwnerPos.x > OtherPos.x)		// �X��������
			{
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
			else if (OwnerPos.x < OtherPos.x)	// �X��������
			{
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
		}

		return;
	}
#pragma endregion

#pragma region ---��
	if (m_bGround && pObject->GetName() == SEED_NAME)
	{
		//����擾�������������悤�ɂ���
		pObject->Delete();

		EffectManager::GetInstance()->Play(GET_ITEM_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(10.0f, 10.0f, 10.0f));

		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// �t
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_SPRING_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// ���W
				XMFLOAT2(150.0f, 150.0f),					// �傫��
				5, 6);										// ������(��)
			break;
		case SUMMER_1:	case SUMMER_2:	// ��
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_SUMMER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// ���W
				XMFLOAT2(150.0f, 150.0f),					// �傫��
				5, 6);										// ������(��)
			break;
		case FALL_1:	case FALL_2:	// �H
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_AUTUMN_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// ���W
				XMFLOAT2(150.0f, 150.0f),					// �傫��
				5, 6);										// ������(��)
			break;
		case WINTER_1:	case WINTER_2:	// �~
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_WINTER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// ���W
				XMFLOAT2(150.0f, 150.0f),					// �傫��
				5, 6);										// ������(��)
			break;
		default:break;
		}

		//�I�u�W�F�N�g�̐���
		Object* pSeed_UI = new Object(SEED_UI_NAME, UPDATE_UI, DRAW_UI);
		//  �R���|�l���g�̒ǉ�
		auto TransSeed_UI = pSeed_UI->AddComponent<CTransform>();			//�@���W
		auto DrawSeed_UI = pSeed_UI->AddComponent<CDraw2D>();				//�@�`��
		auto GetSeed_UI = pSeed_UI->AddComponent<CGetUI>();					//�@UI���o
		// �I�u�W�F�N�g�̐ݒ�
		// ��0.0f�ɕύX���Ă��܂��B
		DrawSeed_UI->SetSize(0.0f, 0.0f);				//  �T�C�Y
		GetSeed_UI->SetUISize(XMFLOAT2(SEED_UI_SIZE_X, SEED_UI_SIZE_Y));	//�@UI�ő�T�C�Y
		GetSeed_UI->SetUIRate(9.0f);										//�@�g��X�s�[�h

		//  �I�u�W�F�N�g�̐ݒ�
		TransSeed_UI->SetPosition(SEED_UI_POS_X, SEED_UI_POS_Y);			//  ���W
		DrawSeed_UI->SetTexture(TextureManager::GetInstance()->GetTexture(SEED_UI_TEX_NUM));	//  �e�N�X�`���̃Z�b�g
		//DrawSeed_UI->SetSize(SEED_UI_SIZE_X, SEED_UI_SIZE_Y);				//  �T�C�Y
		//  ���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(pSeed_UI);
		// �A�j���[�V���� = �Q�b�g
		SetAnimState(PLAYER_GET);
		// se
		CSound::Play(SE_GET_CLOVER);	// �������ɒǉ�

		// ���x���Ȃ���
		m_pPlayer->Vel.x = 0.0f;

		//	�S�[���t���OON
		m_bGoalFlg = true;

		return;
	}
#pragma endregion

	//2021/12/24 Shimizu Yosuke
#pragma region ---�N���[�o�[
	// 2022/01/24 �N���̑��ɍS������Ă��Ȃ�������ǉ�
	if (pObject->GetName() == CLOVER_NAME && pObject->GetComponent<CloverComponent>()->IsBinded() == false)
	{
		// �N���[�o�[�폜
		pObject->Delete();

		// �Q�b�gSE
		CSound::Play(SE_GET_CLOVER);

		// �G�t�F�N�g
		EffectManager::GetInstance()->Play(GET_ITEM_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(10.0f, 10.0f, 10.0f));

		// �n��ɂ���Ƃ�
		if (m_bGround)
		{
			// �A�j���[�V���� = �Q�b�g
			SetAnimState(PLAYER_GET);
		}

		// ���x���Ȃ���
		m_pPlayer->Vel.x = 0.0f;

		//�N���[�o�[�̔ԍ�
		int CloverNumber = pObject->GetComponent<CloverComponent>()->GetNumber();
		if (CloverNumber >= 4)	return;	// ��O����
		
		m_bClover[CloverNumber] = true;

		//UI��ǉ�����
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g����������
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
		auto GetClov_UI = pUI_Clover->AddComponent<CGetUI>();
		// �I�u�W�F�N�g�̐ݒ�
		// ��0.0f�ɕύX���Ă��܂��B
		DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_TEX_NUM));
		//DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);
		DrawClov_UI->SetSize(0.0f, 0.0f);
		GetClov_UI->SetUISize(XMFLOAT2(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y));
		GetClov_UI->SetUIRate(3.0f);
		//�I�u�W�F�N�g�̐ݒ�
		//�����Ń|�W�V�����̐ݒ������
		switch (CloverNumber) {
		case 0:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		//���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(pUI_Clover);

		// �N���[�o�[
		switch (CloverNumber)
		{
		case 0:
			//TransClov_UI->SetPosition(CLOVER_UI_POS_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE),	// ���W
				XMFLOAT2(80.0f, 80.0f),										// �傫��
				5,																// ������(��)
				6);																// ������(�c)
			break;
		case 1:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y),	// ���W
				XMFLOAT2(80.0f, 80.0f),										// �傫��
				5,																// ������(��)
				6);																// ������(�c)
			break;
		case 2:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y),	// ���W
				XMFLOAT2(80.0f, 80.0f),										// �傫��
				5,																// ������(��)
				6);																// ������(�c)
			break;
		case 3:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// �e�N�X�`��
				XMFLOAT2(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE),	// ���W
				XMFLOAT2(80.0f, 80.0f),										// �傫��
				5,																// ������(��)
				6);																// ������(�c)
			break;
		default:
			break;
		}

		return;
	}
#pragma endregion

#pragma region ---��
	if (pObject->GetName() == WIND_NAME)
	{
		m_nColWindCnt = 20;
	}
#pragma endregion

#pragma region ---�G���f�B���O�̃N���[�o�[
	// 2022/01/24 �N���̑��ɍS������Ă��Ȃ�������ǉ�
	if (pObject->GetName() == ENDING_CLOVER_NAME)
	{
		// �N���[�o�[�폜
		pObject->Delete();

		CSound::Play(SE_GET_CLOVER);	// �������ɒǉ�

		return;
	}
#pragma endregion
}

// �����Ă�����̐ݒ�
void CPlayer::SetDestRot(float angle)
{
	m_rotDestModel.y = CCamera::Get()->GetPos().y + angle;
}

// �n��t���O�̐ݒ�
void CPlayer::SetGround(bool ground)
{
	m_bGround = ground;
}

// �A�j���[�V����
void CPlayer::SetAnimState(int next_state)
{
	// �S����Ԃ̎��͑ҋ@���[�V�����ɂ����ڍs�ł��Ȃ�
	if (m_nAnimState == PLAYER_BIND && next_state != PLAYER_IDLE) return;

	// �Q�b�g���[�V�����̎��͑ҋ@���[�V�����ɂ����ڍs�ł��Ȃ�
	if (m_nAnimState == PLAYER_GET && next_state != PLAYER_IDLE) return;

	// �A�j���[�V����state�؂�ւ�
	m_nAnimState = next_state;
}