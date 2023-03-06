#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "MenuComponent.h"
#include "HomingComponent.h"
#include "TextureManager.h"
#include "sceneGame.h"
#include "ComponentSoundConfig.h"
#include "Sound.h"
#include "GaugeComponent.h"
#include "Blink.h"
#include "LotationComponent.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------
#include "sceneTitle.h"
//-----------------------------------------------------------

//===== �ÓI�����o�ϐ� =====
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

// �C���X�^���X��
MenuManager* MenuManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new MenuManager;
	}

	return m_pInstance;
}

// �C���X�^���X�̔j��
void MenuManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::MenuManager
* @brief	�R���X�g���N�^
*/
MenuManager::MenuManager()
	: m_nDelayCnt(-1), m_nCreateState(STATE_NONE)
	, m_OldMenu_State(STATE_NONE)
{
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	�S�Ẵ��j���[�I�u�W�F�N�g�̊J��
*/
MenuManager::~MenuManager()
{
	//	�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList)
		delete menuObject;
	m_MenuList.clear();
}

/**
* @fn		MenuManager::~Start
* @brief	������
*/
void MenuManager::Start()
{
	//	���j���[�̏�����
	for (auto&& MenuObject : m_MenuList)
		MenuObject->Start();
}

/**
* @fn		MenuManager::Update
* @brief	�S�ẴI�u�W�F�N�g�̍X�V
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̍X�V
*/
void MenuManager::Update()
{
	if (m_nDelayCnt > 0)
	{
		// ���j���[�쐬��x�点��
		m_nDelayCnt--;

		if (m_nDelayCnt == 0)
		{
			switch (m_nCreateState)
			{
			case STATE_TITLE:	 CreateTitleMenu(); break;
			case STATE_OPTION:	  break;
			case STATE_SELECT:	 CreateSelectMenu(); break;
			case STATE_GAMEOVER: CreateGameoverMenu(); break;
			default:
				break;
			}

			m_nDelayCnt = -1;
		}
	}

	//	���j���[�I�u�W�F�N�g�̍X�V
	for (auto&& object : m_MenuList)
	{
		object->Update();
	}

	//	Collider�̍X�V
	CCollider::GetColliderList().remove_if([](Object* pObj)
		{
			return pObj->GetDeleteFlag();
		});

	//	�I�u�W�F�N�g�폜�̊m�F
	m_MenuList.remove_if([](Object* pObj)
		{
			return pObj->GetDeleteFlag();
		});
}

/**
* @fn		MenuManager::DrawMenu
* @brief	���j���[�I�u�W�F�N�g�̕`��
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̕`��
*/
void MenuManager::Draw()
{
	SetZBuffer(true);
	//	���j���[�I�u�W�F�N�g�`��
	for (auto&& menuObject : m_MenuList)
		menuObject->Draw();
}

//  �w��t���[����Ƀ��j���[�쐬
void MenuManager::Create(int state, int delayCnt)
{
	m_nCreateState = state;

	m_nDelayCnt = delayCnt;
}

// �^�C�g�����j���[�\��
void MenuManager::CreateTitleMenu()
{
	// �I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();

#pragma region	---�J�[�\��
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	�Փˏ���
	pCursor->AddComponent<CLotation>();							// ��]
	
	// �I�u�W�F�N�g�ݒ�
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	�F
	//	�I�u�W�F�N�g�̐ݒ�
	transCursor->SetPosition(0.0f, 0.0f);						//	���W
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
	
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	this->AddMenu(pCursor);


#pragma endregion

#pragma region ---�͂��߂���
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//---���߂���̘g
	//�I�u�W�F�N�g�̍쐬
	Object* pBeginning_Frame = new Object("", UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransBeginning_Frame = pBeginning_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawBeginning_Frame = pBeginning_Frame->AddComponent<CDraw2D>();					//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransBeginning_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y + 150.0f);	//���W�̐ݒ�
	DrawBeginning_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(NEW_GAME_FRAME_TEX_NUM));
	DrawBeginning_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pBeginning_Frame);
	//----------------------------------------------------------------------------

	Object* pBeginning = new Object(NEW_GAME_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transBeginning = pBeginning->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawBeginning = pBeginning->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderBeginning = pBeginning->AddComponent<CCollider>();		//	�Փˏ���
	auto Beginning = pBeginning->AddComponent<CMenu>();					//	
	// �I�u�W�F�N�g�̐ݒ�
	transBeginning->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y + 150.0f);	//	���W
	DrawBeginning->SetTexture(TextureManager::GetInstance()->GetTexture(NEW_GAME_TEX_NUM));//	�e�N�X�`��
	DrawBeginning->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	�T�C�Y
	ColliderBeginning->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pBeginning);
	// �Q�[���̐i�s�󋵂̏��������K�v
#pragma endregion

#pragma region ---��������
	
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//---��������̘g
	//�I�u�W�F�N�g�̍쐬
	Object* pContinuity_Frame = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI_FRAME);	//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransContinuity_Frame = pContinuity_Frame->AddComponent<CTransform>();	//�g�����X�t�H�[��
	auto DrawContinuity_Frame = pContinuity_Frame->AddComponent<CDraw2D>();		//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransContinuity_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y);	//���W�̐ݒ�
	DrawContinuity_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_FRAME_TEX_NUM));
	DrawContinuity_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);					//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pContinuity_Frame);
	//----------------------------------------------------------------------------
	Object* pContinuity = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transContinuity = pContinuity->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawContinuity = pContinuity->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderContinuity = pContinuity->AddComponent<CCollider>();	//	�Փˏ���
	auto Continuity = pContinuity->AddComponent<CMenu>();				//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transContinuity->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y);				//	���W
	DrawContinuity->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_TEX_NUM));	//	�e�N�X�`��
	DrawContinuity->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	�T�C�Y
	ColliderContinuity->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);		//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pContinuity);
#pragma endregion

#pragma region  ---��߂�
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//�I�v�V�����̘g
	//�I�u�W�F�N�g�̍쐬
	Object* pOption_Frame = new Object(EXIT_GAME_NAME, UPDATE_UI, DRAW_UI_FRAME);		//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransOption_Frame = pOption_Frame->AddComponent<CTransform>();		//�g�����X�t�H�[��
	auto DrawOption_Frame = pOption_Frame->AddComponent<CDraw2D>();			//�`��
	//�g������Փ˔���͂���Ȃ�
	//���j���[�R���|�[�l���g���K�v�Ȃ�
	//�I�u�W�F�N�g�̐ݒ�
	TransOption_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y - 150.0f);	//���W�̐ݒ�
	DrawOption_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(EXIT_FRAME_TEX_NUM));
	DrawOption_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pOption_Frame);
	//----------------------------------------------------------------------------

	Object* pExsit = new Object(EXIT_GAME_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transExsit = pExsit->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawExsit = pExsit->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderExsit = pExsit->AddComponent<CCollider>();	//	�Փˏ���
	auto Exsit = pExsit->AddComponent<CMenu>();		//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transExsit->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y - 150.0f);//	���W
	DrawExsit->SetTexture(TextureManager::GetInstance()->GetTexture(EXIT_TEX_NUM));							//	�e�N�X�`��
	DrawExsit->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	�T�C�Y
	ColliderExsit->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pExsit);
#pragma endregion

#pragma region ---�I�v�V����
	//2022/1/19 Shimizu Shogo ---------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pOption_Icon = new Object(OPTION_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto TransOption_Icon = pOption_Icon->AddComponent<CTransform>();	// ���W
	auto DrawOption_Icon = pOption_Icon->AddComponent<CDraw2D>();		// �`��
	auto ColliderOption = pOption_Icon->AddComponent<CCollider>();				// �Փˏ���
	auto Option = pOption_Icon->AddComponent<CMenu>();							// �}�E�X�Ǐ]
	//�I�u�W�F�N�g�̐ݒ�
	TransOption_Icon->SetPosition(TITLEMENU_POS_X + 250.0f, TITLEMENU_POS_Y + 300.0f);
	DrawOption_Icon->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));
	DrawOption_Icon->SetSize(MENU_WIDTH - 350.0f, MENU_HEIGHT - 150.0f);
	ColliderOption->SetCollisionSize(MENU_COLLISION_WIDTH - 350.0f, MENU_COLLISION_HEIGHT);
	
	//���j���[���X�g�ɒǉ�
	AddMenu(pOption_Icon);
#pragma endregion


	//CreateTitleMenu�̈�ԉ��ɒǉ�
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	m_OldMenu_State = STATE_TITLE;
	//------------------------------------------------------------------
}

// �|�[�Y���j���[�\��
void MenuManager::CreatePauseMenu()
{
	// �I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();

#pragma region	---�J�[�\��
	XMFLOAT2 MousePos = INPUT->GetMousePos();
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	�Փˏ���
	pCursor->AddComponent<CLotation>();							// ��]
	//	�I�u�W�F�N�g�̐ݒ�
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	�F
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	���W							//	�e�N�X�`��
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
									//	�����x
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	this->AddMenu(pCursor);
#pragma endregion

#pragma region ---�Â���
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//�Â���̘g----------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pContinue_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransContinue_Frame = pContinue_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawContinue_Frame = pContinue_Frame->AddComponent<CDraw2D>();					//�`��
	//�g������Փ˔���͂���Ȃ�
	//���j���[�R���|�[�l���g���K�v�Ȃ�
	//�I�u�W�F�N�g�̐ݒ�
	TransContinue_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 225.0f);	//���W�̐ݒ�
	DrawContinue_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_FRAME_TEX_NUM));
	DrawContinue_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pContinue_Frame);
	//----------------------------------------------------------------------------

	Object* pContinue = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transContinue = pContinue->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawContinue = pContinue->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderContinue = pContinue->AddComponent<CCollider>();	//	�Փˏ���
	auto Continue = pContinue->AddComponent<CMenu>();				//	
	// �I�u�W�F�N�g�̐ݒ�
	transContinue->SetPosition(MENU_POS_X, MENU_POS_Y + 225.0f);	//	���W
	DrawContinue->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_TEX_NUM));	//	�e�N�X�`��
	DrawContinue->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	�T�C�Y
	ColliderContinue->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pContinue);
#pragma endregion

#pragma region ---���X�^�[�g
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//���X�^�[�g�̘g--------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pRestart_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransRestart_Frame = pRestart_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawRestart_Frame = pRestart_Frame->AddComponent<CDraw2D>();					//�`��
	//�g������Փ˔���͂���Ȃ�
	//���j���[�R���|�[�l���g���K�v�Ȃ�
	//�I�u�W�F�N�g�̐ݒ�
	TransRestart_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 75.0f);	//���W�̐ݒ�
	DrawRestart_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_FRAME_TEX_NUM));
	DrawRestart_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pRestart_Frame);
	//----------------------------------------------------------------------------

	Object* pRestart = new Object(RESTART_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transRestart = pRestart->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawRestart = pRestart->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderRestart = pRestart->AddComponent<CCollider>();		//	�Փˏ���
	auto Restart = pRestart->AddComponent<CMenu>();					//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transRestart->SetPosition(MENU_POS_X, MENU_POS_Y + 75.0f);		//	���W
	DrawRestart->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_TEX_NUM));//	�e�N�X�`��
	DrawRestart->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	�T�C�Y
	ColliderRestart->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pRestart);
#pragma endregion

#pragma region  ---�Z���N�g
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//�Z���N�g�̘g----------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pSelect_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransSelect_Frame = pSelect_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawSelect_Frame = pSelect_Frame->AddComponent<CDraw2D>();					//�`��
	//�g������Փ˔���͂���Ȃ�
	//���j���[�R���|�[�l���g���K�v�Ȃ�

	//�I�u�W�F�N�g�̐ݒ�
	TransSelect_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 75.0f);	//���W�̐ݒ�
	DrawSelect_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_FRAME_TEX_NUM));
	DrawSelect_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y

	//���j���[���X�g�ɒǉ�
	AddMenu(pSelect_Frame);

	//----------------------------------------------------------------------------

	Object* pMenu = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transMenu = pMenu->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawMenu = pMenu->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderMenu = pMenu->AddComponent<CCollider>();	//	�Փˏ���
	auto Menu = pMenu->AddComponent<CMenu>();				//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transMenu->SetPosition(MENU_POS_X, MENU_POS_Y - 75.0f);//	���W
	DrawMenu->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_TEX_NUM));//	�e�N�X�`��
	DrawMenu->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	�T�C�Y
	ColliderMenu->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pMenu);
#pragma endregion

#pragma region  ---�����Ă�
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//�����Ă��̘g----------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pOption_Frame = new Object("", UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransOption_Frame = pOption_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawOption_Frame = pOption_Frame->AddComponent<CDraw2D>();					//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransOption_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 225.0f);	//���W�̐ݒ�
	DrawOption_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_FRAME_TEX_NUM));
	DrawOption_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pOption_Frame);
	//----------------------------------------------------------------------------

	Object* pOption = new Object(OPTION_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transOption = pOption->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawOption = pOption->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderOption = pOption->AddComponent<CCollider>();	//	�Փˏ���
	auto Option = pOption->AddComponent<CMenu>();				//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transOption->SetPosition(MENU_POS_X, MENU_POS_Y - 225.0f);//	���W
	DrawOption->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_TEX_NUM));//	�e�N�X�`��
	DrawOption->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	�T�C�Y
	ColliderOption->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pOption);
#pragma endregion

}

// �X�e�[�W�Z���N�g�\��
void MenuManager::CreateSelectMenu()
{
#pragma region	---�J�[�\��
	XMFLOAT2 MousePos = INPUT->GetMousePos();
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	�Փˏ���
	pCursor->AddComponent<CLotation>();							// ��]
	// �I�u�W�F�N�g�ݒ�
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	�F
	//	�I�u�W�F�N�g�̐ݒ�
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	���W
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	this->AddMenu(pCursor);
#pragma endregion

#pragma region  ---�^�C�g��
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//�Z���N�g�̘g----------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pTitle_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransTitle_Frame = pTitle_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawTitle_Frame = pTitle_Frame->AddComponent<CDraw2D>();					//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransTitle_Frame->SetPosition(MENU_POS_X + 350.0f, MENU_POS_Y - 250.0f);	//���W�̐ݒ�
	DrawTitle_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_FRAME_TEX_NUM));
	DrawTitle_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pTitle_Frame);
	//----------------------------------------------------------------------------

	Object* pTitle = new Object(TITLE_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transTitle = pTitle->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawTitle = pTitle->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderTitle = pTitle->AddComponent<CCollider>();		//	�Փˏ���
	auto Title = pTitle->AddComponent<CMenu>();					//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transTitle->SetPosition(MENU_POS_X + 350.0f, MENU_POS_Y - 250.0f);//	���W ������-200.0f�ɂ��Ă�������
	DrawTitle->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_TEX_NUM));//	�e�N�X�`��
	DrawTitle->SetSize(MENU_WIDTH, MENU_HEIGHT);
	ColliderTitle->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pTitle);
#pragma endregion

	//CreateTitleMenu�̈�ԉ��ɒǉ�
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	m_OldMenu_State = STATE_SELECT;
	//------------------------------------------------------------------
}

// �Q�[���I�[�o�[�\��
void MenuManager::CreateGameoverMenu()
{
	// �|�[�Y
	SceneGame::GetInstance()->m_bPauseMode = true;

	//DeleteMenu();

#pragma region ---�Â�����
	Object* pDark = new Object("black_out", UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto transDark = pDark->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto drawDark = pDark->AddComponent<CDraw2D>();		// �X�N���[���`��
	// settings
	transDark->SetPosition(0.0f, 0.0f);					// ���W
	drawDark->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// �T�C�Y
	drawDark->SetColor(0.0f, 0.0f, 0.0f);				// �F
	drawDark->SetAlpha(0.5f);							// �����x
	// add to list
	AddMenu(pDark);
#pragma endregion

#pragma region	---�J�[�\��
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	�Փˏ���
	pCursor->AddComponent<CLotation>();							// ��]
	// �I�u�W�F�N�g�ݒ�
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	�F
	//	�I�u�W�F�N�g�̐ݒ�
	transCursor->SetPosition(0.0f, 0.0f);						//	���W
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	// ���X�g�ɒǉ�
	AddMenu(pCursor);
#pragma endregion

#pragma region ---���X�^�[�g
	//2022/1/22 RYOHEI SAWADA-----------------------------------------------------
	//���X�^�[�g�̘g--------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pRestart_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransRestart_Frame = pRestart_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawRestart_Frame = pRestart_Frame->AddComponent<CDraw2D>();					//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransRestart_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 100.0f);	//���W�̐ݒ�
	DrawRestart_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_FRAME_TEX_NUM));
	DrawRestart_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pRestart_Frame);
	//----------------------------------------------------------------------------

	Object* pRestart = new Object(RESTART_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transRestart = pRestart->AddComponent<CTransform>();	// �g�����X�t�H�[��
	auto DrawRestart = pRestart->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto ColliderRestart = pRestart->AddComponent<CCollider>();	//	�Փˏ���
	auto Restart = pRestart->AddComponent<CMenu>();				//	���j���[�@�\
	// �I�u�W�F�N�g�̐ݒ�
	transRestart->SetPosition(MENU_POS_X, MENU_POS_Y + 100.0f);	//	���W
	DrawRestart->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_TEX_NUM));							//	�e�N�X�`��
	DrawRestart->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	�T�C�Y
	ColliderRestart->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	�����蔻��̑傫��
	AddMenu(pRestart);
#pragma endregion


#pragma region  ---�X�e�[�W�Z���N�g
	//2022/1/22 RYOHEI SAWADA-----------------------------------------------------
	//�Z���N�g�̘g----------------------------------------------------------------
	//�I�u�W�F�N�g�̍쐬
	Object* pSelect_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//���O�͕ς���K�v�����邩��
	//�R���|�[�l���g�̒ǉ�
	auto TransSelect_Frame = pSelect_Frame->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawSelect_Frame = pSelect_Frame->AddComponent<CDraw2D>();					//�`��
	//�I�u�W�F�N�g�̐ݒ�
	TransSelect_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 100.0f);	//���W�̐ݒ�
	DrawSelect_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_FRAME_TEX_NUM));
	DrawSelect_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//�T�C�Y
	//���j���[���X�g�ɒǉ�
	AddMenu(pSelect_Frame);
	//----------------------------------------------------------------------------

	Object* pMenu = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transMenu = pMenu->AddComponent<CTransform>();		// �g�����X�t�H�[��
	auto DrawMenu = pMenu->AddComponent<CDraw2D>();			//	2D�`��@�\
	auto ColliderMenu = pMenu->AddComponent<CCollider>();	//	�Փˏ���
	auto Menu = pMenu->AddComponent<CMenu>();		//	�}�E�X�Ǐ]
	// �I�u�W�F�N�g�̐ݒ�
	transMenu->SetPosition(MENU_POS_X, MENU_POS_Y - 100.0f);//	���W
	DrawMenu->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_TEX_NUM));							//	�e�N�X�`��
	DrawMenu->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	�T�C�Y
	ColliderMenu->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	AddMenu(pMenu);
#pragma endregion
}




/**
* @fn		MenuManager::AddMenu
* @brief	���j���[���X�g�ɒǉ�
* @param	(Object*) �ǉ�����I�u�W�F�N�g
*/
void MenuManager::AddMenu(Object* pMenuObject)
{
	//	�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList)
	{
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	//	���j���[���X�g�ɒǉ�
	m_MenuList.insert(itr, pMenuObject);


	// �I�u�W�F�N�g������
	pMenuObject->Start();
}

// ���j���[���X�g�̎擾
std::list<Object*>& MenuManager::GetMenuList()
{
	return m_MenuList;
}

// ���j���[���X�g����ɂ���
void MenuManager::DeleteMenu()
{
	//	�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList)
		menuObject->Delete();

	m_MenuList.clear();
}



// �I�v�V����
void MenuManager::CreateOptionMenu()
{
	// ���łɃI�v�V�������J���Ă�������Ȃ�
	if (m_bOption) return;

	// �I�v�V�����t���OON
	m_bOption = true;

	TextureManager* pTex = TextureManager::GetInstance();

#pragma region ---�Â�����
	Object* pDark = new Object("black_out", UPDATE_OPTION, DRAW_OPTION_BLACK_OUT);
	// components
	auto transDark = pDark->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto drawDark = pDark->AddComponent<CDraw2D>();		// �X�N���[���`��
	// settings
	transDark->SetPosition(0.0f, 0.0f);					// ���W
	drawDark->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// �T�C�Y
	drawDark->SetColor(0.0f, 0.0f, 0.0f);				// �F
	drawDark->SetAlpha(0.5f);							// �����x
	// add to list
	AddMenu(pDark);
#pragma endregion

#pragma region ---BGM
	Object* pBGM = new Object("", UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBGM = pBGM->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto drawBGM = pBGM->AddComponent<CDraw2D>();		// �X�N���[���`��
	// settings
	transBGM->SetPosition(-300.0f, BGM_POS_Y);			// ���W
	drawBGM->SetSize(MENU_WIDTH, MENU_HEIGHT);			// �T�C�Y
	drawBGM->SetTexture(pTex->GetTexture(BGM_TEX_NUM));	// �e�N�X�`��
	// add to list
	AddMenu(pBGM);
#pragma endregion

#pragma region ---BGM�Q�[�W
	Object* pBGM_G = new Object("gauge_bgm", UPDATE_OPTION, DRAW_OPTION_FRAME);
	// components
	auto transBGM_G = pBGM_G->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto drawBGM_G = pBGM_G->AddComponent<CDraw2D>();		// �X�N���[���`��
	auto gaugeBGM_G = pBGM_G->AddComponent<CGauge>();		// �Q�[�W
	// settings
	transBGM_G->SetPosition(MENU_POS_X, BGM_POS_Y);			// ���W
	drawBGM_G->SetSize(STICK_WIDTH, STICK_HEIGHT);			// �T�C�Y
	drawBGM_G->SetTexture(pTex->GetTexture(CONFIG_GAUGE_TEX_NUM));	// �e�N�X�`��
	drawBGM_G->SetAnimSplit(1, 3);							// ������
	drawBGM_G->SetAnimNum(0);								// �A�j���[�V�����ԍ�
	gaugeBGM_G->SetInitPos(MENU_POS_X, BGM_POS_Y);			// �������W
	gaugeBGM_G->SetMaxGauge((int)STICK_WIDTH);				// �ő�Q�[�W
	gaugeBGM_G->SetSize(STICK_WIDTH, STICK_HEIGHT);			// �Q�[�W�T�C�Y
	// add to list
	AddMenu(pBGM_G);
#pragma endregion

#pragma region ---BGM�����̃o�[
	Object* pBGM_B = new Object(BGM_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBGM_B		= pBGM_B->AddComponent<CTransform>();		// �I�u�W�F�N�g���W
	auto drawBGM_B		= pBGM_B->AddComponent<CDraw2D>();			// �X�N���[���`��
	auto colliderBGM_B	= pBGM_B->AddComponent<CCollider>();		// �����蔻��
	auto configBGM_B	= pBGM_B->AddComponent<CSoundConfig>();		// �T�E���h����
	// settings
	float posBGM_B = GAUGE_LEFT + CSound::GetVolume_BGM() * STICK_WIDTH;// ���݉��ʂɂ���č��W��ς���
	transBGM_B->SetPosition(posBGM_B, BGM_POS_Y);					// ���W
	drawBGM_B->SetTexture(nullptr);									// �e�N�X�`��
	drawBGM_B->SetSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);	// �T�C�Y
	colliderBGM_B->SetCollisionSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);	// �����蔻��̑傫��
	configBGM_B->SetSoundType(CONFIG_BGM);							// �����^�C�v
	configBGM_B->SetGauge(gaugeBGM_G);								// �Q�[�W�̐ݒ�
	// add to list
	AddMenu(pBGM_B);
#pragma endregion

#pragma region ---SE
	Object* pSE = new Object("", UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transSE = pSE->AddComponent<CTransform>();		// �I�u�W�F�N�g���W
	auto drawSE = pSE->AddComponent<CDraw2D>();			// �X�N���[���`��
	// settings
	transSE->SetPosition(-300.0f, SE_POS_Y);			// ���W
	drawSE->SetSize(MENU_WIDTH, MENU_HEIGHT);			// �T�C�Y
	drawSE->SetTexture(pTex->GetTexture(SE_TEX_NUM));	// �e�N�X�`��
	// add to list
	AddMenu(pSE);
#pragma endregion

#pragma region ---SE�Q�[�W
	Object* pSE_G = new Object("", UPDATE_OPTION, DRAW_OPTION_FRAME);
	// components
	auto transSE_G = pSE_G->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto drawSE_G  = pSE_G->AddComponent<CDraw2D>();	// �X�N���[���`��
	auto gaugeSE_G = pSE_G->AddComponent<CGauge>();		// �Q�[�W
	// settings
	transSE_G->SetPosition(MENU_POS_X, SE_POS_Y);		// ���W
	drawSE_G->SetSize(STICK_WIDTH, STICK_HEIGHT);		// �T�C�Y
	drawSE_G->SetTexture(pTex->GetTexture(CONFIG_GAUGE_TEX_NUM));	// �e�N�X�`��
	drawSE_G->SetAnimSplit(1, 3);						// ������
	drawSE_G->SetAnimNum(0);							// �A�j���[�V�����ԍ�
	gaugeSE_G->SetInitPos(MENU_POS_X, SE_POS_Y);		// �������W
	gaugeSE_G->SetMaxGauge((int)STICK_WIDTH);			// �ő�Q�[�W
	gaugeSE_G->SetSize(STICK_WIDTH, STICK_HEIGHT);		// �Q�[�W�T�C�Y
	// add to list
	AddMenu(pSE_G);
#pragma endregion

#pragma region ---SE�����̃o�[
	Object* pSE_B = new Object(SE_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transSE_B		= pSE_B->AddComponent<CTransform>();		// �I�u�W�F�N�g���W
	auto drawSE_B		= pSE_B->AddComponent<CDraw2D>();			// �X�N���[���`��
	auto colliderSE_B	= pSE_B->AddComponent<CCollider>();			// �����蔻��
	auto configSE_B		= pSE_B->AddComponent<CSoundConfig>();		// �T�E���h����
	// settings
	float posSE_B = GAUGE_LEFT + CSound::GetVolume_SE() * STICK_WIDTH;// ���݉��ʂɂ���č��W��ς���
	transSE_B->SetPosition(posSE_B, SE_POS_Y);						// ���W
	drawSE_B->SetTexture(nullptr);									// �e�N�X�`��
	drawSE_B->SetSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);		// �T�C�Y
	colliderSE_B->SetCollisionSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);// �����蔻��̑傫��
	configSE_B->SetSoundType(CONFIG_SE);							// �����^�C�v
	configSE_B->SetGauge(gaugeSE_G);								// �Q�[�W�̐ݒ�
	// add to list
	AddMenu(pSE_B);
#pragma endregion

#pragma region ---�߂�{�^��
	Object* pBack = new Object(BACK_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBack		= pBack->AddComponent<CTransform>();	// �I�u�W�F�N�g�̍��W
	auto drawBack		= pBack->AddComponent<CDraw2D>();		// �X�N���[���`��
	auto colliderBack	= pBack->AddComponent<CCollider>();		// �����蔻��
	auto menuBack		= pBack->AddComponent<CMenu>();			// ���j���[�@�\
	// settings
	transBack->SetPosition(350.0f, -250.0f);					// ���W
	drawBack->SetSize(MENU_WIDTH, MENU_HEIGHT);					// �T�C�Y
	colliderBack->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);// �����蔻��̑傫��
	drawBack->SetTexture(pTex->GetTexture(BACK_TEX_NUM));		// �e�N�X�`��
	// add to list
	AddMenu(pBack);
#pragma endregion

	// ���݂̃{�����[�����Q�[�W�ɂ����f������
	CGauge::SetGauge(gaugeBGM_G, (int)(CSound::GetVolume_BGM() * STICK_WIDTH));
	CGauge::SetGauge(gaugeSE_G,  (int)(CSound::GetVolume_SE()  * STICK_WIDTH));
	gaugeBGM_G->Update();
	gaugeSE_G->Update();
}

// �I�v�V�����폜
void MenuManager::DeleteOptionMenu()
{
	// �I�v�V�����t���OOFF
	m_bOption = false;

	for (auto&& obj : m_MenuList)
	{
		// �I�v�V�����̃I�u�W�F�N�g�������ꍇ
		if (obj->GetUpdateOrder() == UPDATE_OPTION)
		{
			// �폜
			obj->Delete();
		}
	}
}



//2022/1/19/Tutorial Shimizu Yosuke -------------------------------
void MenuManager::CreateTutorialMenu() {

	//�`���[�g���A���p�̉摜���o���܂���
	DeleteMenu();

#pragma region	---�J�[�\��
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D�`��@�\
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	�Փˏ���
	pCursor->AddComponent<CLotation>();							// ��]
	// �I�u�W�F�N�g�ݒ�
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	�F
	transCursor->SetPosition(0.0f, 0.0f);						//	���W
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	// ���j���[���X�g�ɒǉ�
	this->AddMenu(pCursor);
#pragma endregion

#pragma region ---�`���[�g���A���Ŕ�
	//�I�u�W�F�N�g�̐���
	Object* pTutorial = new Object(TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto TransTutorial = pTutorial->AddComponent<CTransform>();				//�g�����X�t�H�[��
	auto DrawTutorial = pTutorial->AddComponent <CDraw2D>();				//�`��
	auto CollTutorial = pTutorial->AddComponent<CCollider>();				//�Փ˔���
	pTutorial->AddComponent<CMenu>();										//���j���[�̋@�\�̒ǉ�

	//�I�u�W�F�N�g�̐ݒ�
	TransTutorial->SetPosition(0.0f, 55.0f);									//���W
	DrawTutorial->SetSize(TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y);			//�傫��

	//2022/1/24/TUTORIAL_UI Shimizu Yosuke ------------------------------------------------------------
	//�Z�b�g����e�N�X�`����ς���
	switch (SceneGame::GetInstance()->GetSeason()) {
	case SPRING_1:
	case SPRING_2:
		//�������炳��ɕ����� UI or ����


		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(PLAYER_TUTORIAL_TEX_NUM));//�e�N�X�`��
		break;
	case SUMMER_1:
	case SUMMER_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_SPIDER_TEX_NUM));//�e�N�X�`��
		break;
	case FALL_1:
	case FALL_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_FALL_TEX_NUM));//�e�N�X�`��
		break;
	case WINTER_1:
	case WINTER_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_WINTER_TEX_NUM));//�e�N�X�`��
		break;
	}
	//-------------------------------------------------------------------------------------------------


	CollTutorial->SetCollisionSize(SCREEN_WIDTH , 1000.0f);	//�����蔻��̑傫��

	//���X�g�ɒǉ�
	MenuManager::GetInstance()->AddMenu(pTutorial);
#pragma endregion

	//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------------------------------------
#pragma region ---�N���b�N���đ�����
	//�I�u�W�F�N�g����
	Object* pClick_Text = new Object(CLICK_TEXT_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//�R���|�[�l���g�̒ǉ�
	auto TransClick = pClick_Text->AddComponent<CTransform>();					//�g�����X�t�H�[��
	auto DrawClick = pClick_Text->AddComponent<CDraw2D>();						//�`��
	//�_�ł���R���|�[�l���g�̒ǉ�
	pClick_Text->AddComponent<CBlink>();
	//�I�u�W�F�N�g�̐ݒ�
	TransClick->SetPosition(CLICK_TEXT_POS_X, CLICK_TEXT_POS_Y);					//���W
	DrawClick->SetSize(CLICK_TEXT_SIZE_X, CLICK_TEXT_SIZE_Y);						//�T�C�Y
	DrawClick->SetTexture(TextureManager::GetInstance()->GetTexture(CLICK_TEX_NUM));//�e�N�X�`��
	//���X�g�ɒǉ�
	MenuManager::GetInstance()->AddMenu(pClick_Text);
#pragma endregion
	//-------------------------------------------------------------------------------------
}
//-----------------------------------------------------------------


//2022/1/31/Load_Tutorial Shimizu Yosuke --------------------------------------------------
//�V�[�����Q�b�g����֐�
MENU_STATE MenuManager::GetMenu() {
	return m_OldMenu_State;
}

//�V�[�����Z�b�g����֐�
void MenuManager::SetMenu(MENU_STATE State_scene) {
	m_OldMenu_State = State_scene;
}
//-----------------------------------------------------------------------------------------

