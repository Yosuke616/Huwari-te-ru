/**
 * @file	MenuComponent.h
 * @brief	UI�I�u�W�F�N�g�̊Ǘ�
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 *�X�V����
*/

//===== �C���N���[�h =====
#include "MenuComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "sceneGame.h"
#include "fade.h"
#include "Draw2dComponent.h"
#include "Sound.h"
#include "TransformComponent.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ----------------------------
#include "sceneTitle.h"
//------------------------------------------------------------------

#include "MenuManager.h"

//***�R���X�g���N�^
CMenu::CMenu()
	: m_bSelected(false)
	, m_pTrans(nullptr)
	, m_nRotate(0)
{
}

// �f�X�g���N�^
CMenu::~CMenu(){
}

// ������
void CMenu::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();

	//2022/1/27/Touch_Tutorial Shimizu Yosuke ------------------
	m_pTrans = Parent->GetComponent<CTransform>();
	//----------------------------------------------------------
}

// �X�V
void CMenu::Update()
{
	

	if (m_bSelected)
	{
		m_pDraw2D->SetColor(0.0f, 1.0f, 0.0f);

		//2022/1/24/TUTORIAL_UI Shimizu Yosuke---------------------
		if (Parent->GetName() == TUTORIAL_NAME || Parent->GetName() == WINDCAR_NAME) {
			m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);
		}
		//---------------------------------------------------------
	}
	else
	{
		m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);

		//2022/1/22/pouseColor Shimizu Yosuke ---------------------
		if (Parent->GetName() == STOP_GAME_NAME) {
			m_pDraw2D->SetColor(0.2f, 0.2f, 0.6f);
		}
		if (Parent->GetName() == OPTION_NAME && SceneManager::Instance()->GetScene() == SCENE_TITLE)
		{
			m_pDraw2D->SetColor(0.2f, 0.2f, 0.6f);
		}
		//---------------------------------------------------------

	}
	m_bSelected = false;

	//2022/1/26/Touch_Tutorial ------------------------------------
	if (Parent->GetName() == WINDCAR_NAME) {
		//��]�w����ϐ��������ō��
		m_nRotate += 5;
		if (m_nRotate > 180) {
			m_nRotate -= 360;
		}

		m_pDraw2D->SetAngle((float)m_nRotate);
	}
	//-------------------------------------------------------------
}

// �`��
void CMenu::Draw()
{
}

// �Փˏ���
void CMenu::OnCollisionEnter(Object* pObject)
{

	InputManager* pInput = InputManager::Instance();

	if (MenuManager::m_bOption == false)
	{
		// �J�[�\��(���j���[�I�u�W�F�N�g)
		if (pObject->GetName() == CURSOR_NAME)
		{
			// �I������Ă��܂�
			m_bSelected = true;

			if (pInput->GetMouseTrigger(MOUSE_L))
			{
				// ���艹�Đ�
				CSound::Play(SE_DECIDE);

				// �^�C�g����ʃ��j���[
#pragma region ---�ɂ�[���[��
			//	�����̖��O�ŏ������e��ς���
				if (Parent->GetName() == NEW_GAME_NAME)
				{

					SceneTitle::NewGame();

					SceneSelect::NewGame();
					// �^�C�g���V�[����
					StartFadeOutSceneChange(SCENE_SELECT);
				}
#pragma endregion

#pragma region ---�Z���N�g��
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == SELECT_NAME)
				{
					// �Z���N�g�V�[����
					StartFadeOutSceneChange(SCENE_SELECT);
				}
#pragma endregion

#pragma region ---�����
				//	�����̖��O�ŏ������e��ς���
				if (Parent->GetName() == EXIT_GAME_NAME)
				{
					PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
				}
#pragma endregion


#pragma region ---�I�v�V����
				//	�����̖��O�ŏ������e��ς���
				if (Parent->GetName() == OPTION_NAME)
				{
					MenuManager::GetInstance()->CreateOptionMenu();
				}
#pragma endregion

				// �Q�[����ʃ��j���[

				//�Q�[����ʃ��j���[�̈�ԉ��ɒǉ�
				//2022/1/19/Tutorial Shimizu Yosuke -------------------------------------------
#pragma region  ---�`���[�g���A��
			
				if (Parent->GetName() == TUTORIAL_NAME || Parent->GetName() == WINDCAR_NAME)
				{
					//�|�[�Y�t���O���I�t�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = false;
					// ���j���[�폜
					MenuManager::GetInstance()->DeleteMenu();
					// �I�u�W�F�N�g�̍X�V���ĊJ����
					ObjectManager::GetInstance()->Resume();
				}
#pragma endregion
				//------------------------------------------------------------------------------

#pragma region	 ---�Â���
			//	�����̖��O�ŏ������e��ς���
				if (Parent->GetName() == CONTINUE_NAME)
				{
					// �|�[�Y�t���O��false�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = false;

					// �I�u�W�F�N�g�X�V�ĊJ
					ObjectManager::GetInstance()->Resume();

					// ���j���[������
					MenuManager::GetInstance()->DeleteMenu();
				}
#pragma endregion

#pragma region ---�ŏ�����
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == RESTART_NAME)
				{
					// �|�[�Y�t���O��false�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = false;
					// �Q�[���V�[����
					StartFadeOutSceneChange(SCENE_GAME);
				}
#pragma endregion

#pragma region ---�|�[�Y���j���[�A�C�R��
				if (Parent->GetName() == STOP_GAME_NAME)
				{
					// �|�[�Y�t���O��true�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = true;
					// �|�[�Y��ʐ���
					MenuManager::GetInstance()->CreatePauseMenu();
					// �I�u�W�F�N�g�̍X�V�X�g�b�v
					ObjectManager::GetInstance()->NoFunction();
				}
#pragma endregion

				// �Z���N�g��ʃ��j���[
#pragma region	---�^�C�g����
			//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == TITLE_NAME)
				{
					//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------
					MenuManager::GetInstance()->SetMenu(STATE_NONE);
					//------------------------------------------------------------

					// �|�[�Y�t���O��false�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = false;
					// �^�C�g���V�[����
					StartFadeOutSceneChange(SCENE_TITLE);
				}
#pragma endregion

#pragma region	---�Z���N�g �E���
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == RIGHT_ARROW_NAME)
				{
					if (this->m_bDrawFlag == false) {
						CSound::Stop(SE_DECIDE);
					}

					SceneSelect::ChangeSceason(1);
				}
#pragma endregion

#pragma region	---�Z���N�g �����
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == LEFT_ARROW_NAME)
				{
					if (this->m_bDrawFlag == false) {
						CSound::Stop(SE_DECIDE);
					}
					SceneSelect::ChangeSceason(-1);
				}
#pragma endregion

#pragma region	---�X�e�[�W1
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == STAGE1_NAME)
				{
					switch (SceneSelect::m_nCurrentSeason)
					{
					case 0: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SPRING_1); break;
					case 1: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SUMMER_1); break;
					case 2: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(FALL_1);   break;
					case 3: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(WINTER_1); break;
					default: break;
					}
				}
#pragma endregion

#pragma region	---�X�e�[�W2
				//	�����̖��O�ŏ������e��ς���
				else if (Parent->GetName() == STAGE2_NAME)
				{
					switch (SceneSelect::m_nCurrentSeason)
					{
					case 0: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SPRING_2); break;
					case 1: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SUMMER_2); break;
					case 2: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(FALL_2); break;
					case 3: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(WINTER_2); break;
					default:
						break;
					}
				}
#pragma endregion
			}
		}
		// �Q�[�����̃J�[�\���Ƃ̏Փ�
		else if (pObject->GetName() == GAME_CURSOR_NAME)
		{
			// �I������Ă��܂�
			m_bSelected = true;

			if (pInput->GetMouseTrigger(MOUSE_L))
			{
				// ���艹�Đ�
				CSound::Play(SE_DECIDE);

				if (Parent->GetName() == STOP_GAME_NAME)
				{
					// �|�[�Y�t���O��true�ɂ���
					SceneGame::GetInstance()->m_bPauseMode = true;
					// �|�[�Y��ʐ���
					MenuManager::GetInstance()->CreatePauseMenu();
					// �I�u�W�F�N�g�̍X�V�X�g�b�v
					ObjectManager::GetInstance()->NoFunction();
				}


				//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------
				else if (Parent->GetName() == WINDCAR_NAME) {

					//�|�[�Y�t���O���I���ɂ���
					SceneGame::GetInstance()->m_bPauseMode = true;

					//�`���[�g���A���ɍs��������
					MenuManager::GetInstance()->CreateTutorialMenu();

					//�I�u�W�F�N�g�̍X�V���~�߂�
					ObjectManager::GetInstance()->NoFunction();
				}
				//----------------------------------------------------------------
			}
		}
	}
	// �I�v�V�������J���Ă�Ƃ��͂�����
	else
	{
	// �J�[�\��(���j���[�I�u�W�F�N�g)
		if (pObject->GetName() == CURSOR_NAME)
		{
			// ���ǂ�
			if (Parent->GetName() == BACK_NAME)
			{
				// �I������Ă��܂�
				m_bSelected = true;

				if (pInput->GetMouseTrigger(MOUSE_L))
				{
					// �I�v�V�������j���[�폜
					MenuManager::GetInstance()->DeleteOptionMenu();
				}
			}
		}
	}

}