//===== �C���N���[�h�@=====
#include "SceneSelect.h"
#include "fade.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "imgui.h"
#include "Sound.h"
#include "Camera.h"

#include "Component.h"
#include "ColliderComponent.h"		//	�����蔻��
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "HomingComponent.h"
#include "MenuComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "MenuManager.h"
#include "ModelManager.h"
#include "Draw3dComponent.h"
#include "CreateLeaf.h"
#include "PreviewComponent.h"
#include "Scale2DCom.h"

#include "GetUIComponent.h"
#include "Load.h"


//===== �}�N����` =====
#define WIDTH_STAGEMAP_1	(400)
#define HEIGHT_STAGEMAP_1	(280)
#define POS_X_STAGEMAP_1	(-270.0f)
#define POS_Y_STAGEMAP_1	(SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.025f

#define WIDTH_STAGEMAP_2	(400)
#define HEIGHT_STAGEMAP_2	(280)
#define POS_X_STAGEMAP_2	(270.0f)
#define POS_Y_STAGEMAP_2	(SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.025f

#define WIDTH_RIGHT_ARROW	(100)
#define HEIGHT_RIGHT_ARROW	(100)

#define POS_X_LEFT_ARROW	(-580.0f)
#define POS_X_RIGHT_ARROW	(580.0f)
#define POS_Y_RIGHT_ARROW	((SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.125f -75.0f)

#define WIDTH_SEAZON_UI		(135)
#define HEIGHT_SEAZON_UI	(100)
#define POS_X_SEAZON_UI		(-550)
#define POS_Y_SEAZON_UI		(300)

//�X�e�[�W�g
#define WIDTH_STAGE_FRAME			(480)
#define HEIGHT_STAGE_FRAME			(480)
#define POS_X_STAGE_FRAME_1			(POS_X_STAGEMAP_1)
#define POS_X_STAGE_FRAME_2			(POS_X_STAGEMAP_2)
#define POS_Y_STAGE_FRAME_1			(POS_Y_STAGEMAP_1 + 60.0f)
#define POS_Y_STAGE_FRAME_2			(POS_Y_STAGEMAP_2 + 60.0f)

//ADD
//�v���C���[
#define POS_X_SELECT_PLAYER			(0.0f)
#define POS_Y_SELECT_PLAYER			(-110.0f)
//ADD
//�N���[�o�[UI�i��j
#define WIDTH_CLOVER_UI				(50)
#define HEIGHT_CLOVER_UI			(50)
#define POS_X_CLOVER_UI_1			(POS_X_STAGEMAP_1 + WIDTH_STAGEMAP_1 * 0.25f)
#define POS_Y_CLOVER_UI_1			(POS_Y_STAGEMAP_1 - HEIGHT_STAGEMAP_1 * 0.5f) - 20.0f
#define POS_X_CLOVER_UI_2			(POS_X_STAGEMAP_2 + WIDTH_STAGEMAP_2 * 0.25f)
#define POS_Y_CLOVER_UI_2			(POS_Y_STAGEMAP_2 - HEIGHT_STAGEMAP_1 * 0.5f) - 20.0f
//ADD
//�G��UI
#define WIDTH_SEAZON_UI				(135)
#define HEIGHT_SEAZON_UI			(100)
#define POS_X_SEAZON_UI				(-550)
#define POS_Y_SEAZON_UI				(300)
//ADD
//�N���AUI�i�ԁj

#define WIDTH_CLEAR_FLOWER_UI		(100.0f)
#define HEIGHT_CLEAR_FLOWER_UI		(100.0f)
#define POS_X_CLEAR_FLOWER_UI_1		(POS_X_CLOVER_UI_1 + 110.0f)
#define POS_Y_CLEAR_FLOWER_UI_1		(POS_Y_CLOVER_UI_1)
#define POS_X_CLEAR_FLOWER_UI_2		(POS_X_CLOVER_UI_2 + 110.0f)
#define POS_Y_CLEAR_FLOWER_UI_2		(POS_Y_CLOVER_UI_2)
//ADD
//�N���AUI�i�����j

#define WIDTH_CLEAR_UI				(300)
#define HEIGHT_CLEAR_UI				(225)
#define POS_X_CLEAR_UI_1			(POS_X_STAGEMAP_1 + WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_CLEAR_UI_1			(POS_Y_STAGEMAP_1 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
#define POS_X_CLEAR_UI_2			(POS_X_STAGEMAP_2 + WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_CLEAR_UI_2			(POS_Y_STAGEMAP_2 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
//ADD
//�X�e�[�W��
#define WIDTH_STAGENAME_UI			(300)
#define HEIGHT_STAGENAME_UI			(225)
#define POS_X_STAGENAME_UI_1		(POS_X_STAGEMAP_1 - WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_STAGENAME_UI_1		(POS_Y_STAGEMAP_1 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
#define POS_X_STAGENAME_UI_2		(POS_X_STAGEMAP_2 - WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_STAGENAME_UI_2		(POS_Y_STAGEMAP_2 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f


#define INTERVAL	(60)


//===== �\���̒�` ======
int SceneSelect::m_nCurrentSeason = 0;
SceneSelect::Row SceneSelect::m_SelectGrid;
SceneSelect::Column SceneSelect::m_Clover;
int SceneSelect::m_OldSeason;
bool SceneSelect::m_bRelease1 = false;		// �X�e�[�W������o
bool SceneSelect::m_bRelease2 = false;		// �X�e�[�W������o
bool SceneSelect::m_bReleaseClover[8] = {false, false, false, false, false, false, false, false};		// �N���[�o�[�J�����o

//===== �O���[�o���錾 =====
CDrawEffect2D *g_CurrentMap1 = nullptr;
CDrawEffect2D *g_CurrentMap2 = nullptr;
CDraw2D *g_CurrentBG = nullptr;
CDraw2D *g_CurrentSeazonUI = nullptr;
CDraw2D *g_CurrentStageName1 = nullptr;
CDraw2D *g_CurrentStageName2 = nullptr;
CDraw2D *g_CurrentClearFlower1 = nullptr;
CDraw2D *g_CurrentClearFlower2 = nullptr;

CDraw2D *g_CurrentStageClearUI1 = nullptr;	//�X�e�[�W�}�b�v�P�̃N���A�󋵕\��
CDraw2D *g_CurrentStageClearUI2 = nullptr;	//�X�e�[�W�}�b�v�Q�̃N���A�󋵕\��

CDrawModel *g_CurrentPlayer = nullptr;
CDrawModel *g_CurrentBlock = nullptr;



CLeaf g_SelectLeaf;

// �R���X�g���N�^
SceneSelect::SceneSelect()
{
	SceneSelect::Load();

	m_OldSeason = SPRING;  // 01/12�ǉ�
}

// �f�X�g���N�^
SceneSelect::~SceneSelect(){
}

// ������
void SceneSelect::Init()
{
	//===== ���[�h��ʂ̃X���b�h����� =====
	Load::Begin();

	// �i�s�󋵂̓ǂݍ���
	SceneSelect::Load();

	//	�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();	// �I�u�W�F�N�g�}�l�[�W���[
	m_pMenuManager = MenuManager::GetInstance();		// ���j���[�}�l�[�W���[

	// �J�����̏�����
	CCamera::Get()->Init();

	/* �e�N�X�`���ǂݍ��� */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// UI
	pTexManager->AddTexture(PATH_TEX_ARROW_UI, ARROW_UI_TEX_NUM);				// ���
	pTexManager->AddTexture(PATH_TEX_SPRING_UI, SPRING_UI_TEX_NUM);				// �t(����
	pTexManager->AddTexture(PATH_TEX_SUMMER_UI, SUMMER_UI_TEX_NUM);				// ��(����
	pTexManager->AddTexture(PATH_TEX_AUTUMN_UI, AUTUMN_UI_TEX_NUM);				// �H(����
	pTexManager->AddTexture(PATH_TEX_WINTER_UI, WINTER_UI_TEX_NUM);				// �~(����
	
	//�@tomoya
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_SPRING, CLEAR_FLOWER_SPRING_TEX_NUM);	// �Z���N�g�̉�
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_SUMMER, CLEAR_FLOWER_SUMMER_TEX_NUM);	// �Z���N�g�̉�
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_AUTUMN, CLEAR_FLOWER_AUTUMN_TEX_NUM);	// �Z���N�g�̉�
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_WINTER, CLEAR_FLOWER_WINTER_TEX_NUM);	// �Z���N�g�̉�
	pTexManager->AddTexture(PATH_TEX_STAGE_FRAME, STAGE_FRAME_TEX_NUM);	//�X�e�[�W�̘g
	pTexManager->AddTexture(PATH_TEX_SPRING_1, SPRING_1_TEX_NUM);		//�X�e�[�W���F�t�[�P
	pTexManager->AddTexture(PATH_TEX_SPRING_2, SPRING_2_TEX_NUM);		//			 �t�[�Q
	pTexManager->AddTexture(PATH_TEX_SUMMER_1, SUMMER_1_TEX_NUM);		//		�@�@�@�ā[�P
	pTexManager->AddTexture(PATH_TEX_SUMMER_2, SUMMER_2_TEX_NUM);		//		�@�@�@�ā[�Q
	pTexManager->AddTexture(PATH_TEX_AUTUMN_1, AUTUMN_1_TEX_NUM);		//		�@�@�@�H�[�P
	pTexManager->AddTexture(PATH_TEX_AUTUMN_2, AUTUMN_2_TEX_NUM);		//		�@�@�@�H�[�Q
	pTexManager->AddTexture(PATH_TEX_WINTER_1, WINTER_1_TEX_NUM);		//		�@�@�@�~�[�P
	pTexManager->AddTexture(PATH_TEX_WINTER_2, WINTER_2_TEX_NUM);		//		�@�@�@�~�[�Q

	// �N���[�o�[���g
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI, CLOVER_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI_FRAME, CLOVER_UI_FRAME_TEX_NUM);

	pTexManager->AddTexture(PATH_TEX_CLEAR_UI, CLEAR_UI_TEX_NUM);		//�N���A����
	pTexManager->AddTexture(PATH_TEX_PARFECT_UI, PARFECT_UI_TEX_NUM);	//�p�[�t�F�N�g����



	pTexManager->AddTexture(PATH_MV_SPRING_1,   MV_SPRING_1_NUM);
	pTexManager->AddTexture(PATH_MV_SPRING_2,   MV_SPRING_2_NUM);
	pTexManager->AddTexture(PATH_MV_SUMMER_1, MV_SUMMER_1_NUM);
	pTexManager->AddTexture(PATH_MV_SUMMER_2, MV_SUMMER_2_NUM);
	pTexManager->AddTexture(PATH_MV_FALL_1,   MV_FALL_1_NUM);
	pTexManager->AddTexture(PATH_MV_FALL_2,   MV_FALL_2_NUM);
	pTexManager->AddTexture(PATH_MV_WINTER_1, MV_WINTER_1_NUM);
	pTexManager->AddTexture(PATH_MV_WINTER_2, MV_WINTER_2_NUM);



	/* ���f���ǂݍ��� */
	ModelManager* pModelManager = ModelManager::GetInstance();
	//�V���t�B
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_PLAYER, SUMMER_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_PLAYER, FALL_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_PLAYER, WINTER_PLAYER_NUM);
	//�u���b�N
	pModelManager->AddModel(PATH_MODEL_SPRING_BLOCK, SPRING_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_BLOCK, SUMMER_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_BLOCK, FALL_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_BLOCK, WINTER_BLOCK_MODEL_NUM);

	// 01/12�ǉ�
	m_nCurrentSeason = m_OldSeason;


#pragma region	---�v���C���[	2022/01/21 tomoya
	//	�I�u�W�F�N�g����
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	�R���|�[�l���g�̒ǉ�
	auto TransPlayer = Player->AddComponent<CTransform>();				//	�g�����X�t�H�[��
	auto DrawPlayer = Player->AddComponent<CDrawModel>();					//	���f���`��@�\
	//	�I�u�W�F�N�g���̐ݒ�
	DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));			// �ҋ@�A������Ă���A������A�E��
	TransPlayer->SetScale(5.0f, 5.0f, 5.0f);									// �X�P�[��
	TransPlayer->SetPosition(POS_X_SELECT_PLAYER, POS_Y_SELECT_PLAYER, 0.0f);	// �����z�u
	// �A�j���[�V�����ݒ�
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);// �ҋ@
	DrawPlayer->AlwaysAnimation(true);				// ��ɃA�j���[�V����
	DrawPlayer->Play(PLAYER_IDLE);					// �ҋ@���[�V�����Đ�
	// 3D���X�g�ɒǉ�
	m_pObjectManager->AddObject(Player);
#pragma endregion

#pragma region	--- �n�`�u���b�N	1/24 tomoya
	//	�I�u�W�F�N�g����
	Object* objBlock = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
	//	�R���|�[�l���g�̒ǉ�
	auto Trans_Block = objBlock->AddComponent<CTransform>();				//	�g�����X�t�H�[��
	auto Draw_Block = objBlock->AddComponent<CDrawModel>();					//	���f���`��@�\

	//	�I�u�W�F�N�g���̐ݒ�
	Draw_Block->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));	// ���f��

	Trans_Block->SetScale(35.0f, 2.0f, 1.0f);												//	�X�P�[��
	Trans_Block->SetPosition(POS_X_SELECT_PLAYER, POS_Y_SELECT_PLAYER - 5.0f, 0.0f);		//  �����z�u

	// 3D���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBlock);
#pragma endregion

#pragma region ---�w�i
		//	�I�u�W�F�N�g����
	Object* objBG = new Object("BG1", UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();											//	���W
	auto Draw_BG = objBG->AddComponent<CDraw2D>();								//	�`��
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_BG->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));//	�e�N�X�`��
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);								//	�T�C�Y
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBG);											//	�I�u�W�F�N�g�̒ǉ�
#pragma endregion


#pragma region --- �N���AUI�\���i�����j
		//�N���A��UI��ǉ�����
	Object* objClearUI_1 = new Object(CLEAR_UI_1_NAME, UPDATE_UI, DRAW_UI);
	Object* objClearUI_2 = new Object(CLEAR_UI_2_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g����������
	auto Trans_ClearUI_1 = objClearUI_1->AddComponent<CTransform>();		//	���W
	auto Draw_ClearUI_1 = objClearUI_1->AddComponent<CDraw2D>();
	auto Trans_ClearUI_2 = objClearUI_2->AddComponent<CTransform>();		//	���W
	auto Draw_ClearUI_2 = objClearUI_2->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	Trans_ClearUI_1->SetPosition(POS_X_CLEAR_UI_1, POS_Y_CLEAR_UI_1);
	Trans_ClearUI_2->SetPosition(POS_X_CLEAR_UI_2, POS_Y_CLEAR_UI_2);

	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
	Draw_ClearUI_1->SetSize(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI);
	Draw_ClearUI_1->SetColor(1.0f, 1.0f, 1.0f);	//�F���Z�b�g�i���F�j
	Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
	Draw_ClearUI_2->SetSize(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI);
	Draw_ClearUI_2->SetColor(1.0f, 1.0f, 1.0f);	//�F���Z�b�g(���F)


	//objClearUI_1->NoUse();	//�`����~
	//objClearUI_2->NoUse();
	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(objClearUI_1);
	ObjectManager::GetInstance()->AddObject(objClearUI_2);

#pragma endregion

#pragma region --- �X�e�[�W��UI�\��
	//�X�e�[�W��UI��ǉ�����
	Object* objStageNameUI_1 = new Object("StageNameUI", UPDATE_UI, DRAW_UI);
	Object* objStageNameUI_2 = new Object("StageNameUI", UPDATE_UI, DRAW_UI);

	//�R���|�[�l���g����������
	auto Trans_StageNameUI_1 = objStageNameUI_1->AddComponent<CTransform>();						//	���W
	auto Trans_StageNameUI_2 = objStageNameUI_2->AddComponent<CTransform>();						//	���W
	auto Draw_StageNameUI_1 = objStageNameUI_1->AddComponent<CDraw2D>();
	auto Draw_StageNameUI_2 = objStageNameUI_2->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	Trans_StageNameUI_1->SetPosition(POS_X_STAGENAME_UI_1, POS_Y_STAGENAME_UI_1);
	Trans_StageNameUI_2->SetPosition(POS_X_STAGENAME_UI_2, POS_Y_STAGENAME_UI_2);
	Draw_StageNameUI_1->SetTexture(nullptr);
	Draw_StageNameUI_2->SetTexture(nullptr);
	Draw_StageNameUI_1->SetSize(WIDTH_STAGENAME_UI, HEIGHT_STAGENAME_UI);
	Draw_StageNameUI_2->SetSize(WIDTH_STAGENAME_UI, HEIGHT_STAGENAME_UI);

	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(objStageNameUI_1);
	ObjectManager::GetInstance()->AddObject(objStageNameUI_2);

#pragma endregion

#pragma region  --- �N���AUI�\���i�ԁj

//�N���A�i�ԁj��UI��ǉ�����
	Object* objClearFlowerUI_1 = new Object(CLEAR_FLOWER_UI_1_NAME, UPDATE_UI, DRAW_UI);
	Object* objClearFlowerUI_2 = new Object(CLEAR_FLOWER_UI_2_NAME, UPDATE_UI, DRAW_UI);

	//�R���|�[�l���g����������
	auto Trans_ClearFlowerUI_1 = objClearFlowerUI_1->AddComponent<CTransform>();
	auto Trans_ClearFlowerUI_2 = objClearFlowerUI_2->AddComponent<CTransform>();
	auto Draw_ClearFlowerUI_1 = objClearFlowerUI_1->AddComponent<CDraw2D>();
	auto Draw_ClearFlowerUI_2 = objClearFlowerUI_2->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	Trans_ClearFlowerUI_1->SetPosition(POS_X_CLEAR_FLOWER_UI_1, POS_Y_CLEAR_FLOWER_UI_1);
	Trans_ClearFlowerUI_2->SetPosition(POS_X_CLEAR_FLOWER_UI_2, POS_Y_CLEAR_FLOWER_UI_2);
	Draw_ClearFlowerUI_1->SetTexture(nullptr);
	Draw_ClearFlowerUI_2->SetTexture(nullptr);
	Draw_ClearFlowerUI_1->SetSize(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI);
	Draw_ClearFlowerUI_2->SetSize(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI);

	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(objClearFlowerUI_1);
	ObjectManager::GetInstance()->AddObject(objClearFlowerUI_2);

#pragma endregion


#pragma region ---�擾�N���[�o�[UI�i�g
	//�X�e�[�W�}�b�v�P�̃N���[�o�[
	for (int i = 0; i < 4; i++)
	{
		Object* objBlankClover = new Object("CloverBlank", UPDATE_UI, DRAW_UI_FRAME);
		//�R���|�[�l���g����������
		auto TransClov_UI = objBlankClover->AddComponent<CTransform>();
		auto DrawClov_UI = objBlankClover->AddComponent<CDraw2D>();
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		switch (i)
		{
			//�����Ń|�W�V�����̐ݒ������
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		m_pObjectManager->AddObject(objBlankClover);
	}
	//�X�e�[�W�}�b�v�Q�̃N���[�o�[
	for (int i = 0; i < 4; i++)
	{
		Object* objBlankClover = new Object("CloverBlank", UPDATE_UI, DRAW_UI_FRAME);
		//�R���|�[�l���g����������
		auto TransClov_UI = objBlankClover->AddComponent<CTransform>();
		auto DrawClov_UI = objBlankClover->AddComponent<CDraw2D>();
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		switch (i)
		{
			//�I�u�W�F�N�g�̐ݒ�
			//�����Ń|�W�V�����̐ݒ������
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		m_pObjectManager->AddObject(objBlankClover);
	}
#pragma endregion


	// �X�e�[�W�}�b�v1
#pragma region ---�X�e�[�W�}�b�v�P
//�I�u�W�F�N�g����
	Object* objStageMap1 = new Object(STAGE1_NAME, UPDATE_UI, DRAW_EFFECT);
	//�R���|�[�l���g�̒ǉ�
	auto trans1 = objStageMap1->AddComponent<CTransform>();				//	�I�u�W�F�N�g���W
	auto Draw_StageMap1 = objStageMap1->AddComponent<CDrawEffect2D>();	//	�`��
	auto Col_StageMap1 = objStageMap1->AddComponent<CCollider>();		//	�����蔻��
	auto Menu_StageMap1 = objStageMap1->AddComponent<CMenu>();			//	���j���[�@�\
	objStageMap1->AddComponent<CPreview>();
	//�I�u�W�F�N�g�̐ݒ�
	Draw_StageMap1->SetTexture(TextureManager::GetInstance()->GetTexture(MV_SPRING_1_NUM));//	�e�N�X�`��(��)
	Draw_StageMap1->SetSize(WIDTH_STAGEMAP_1, HEIGHT_STAGEMAP_1);	//	�T�C�Y
	Draw_StageMap1->SetLoop(true);
	Draw_StageMap1->SetAnimSplit(4, 19);		// ��
	Draw_StageMap1->SetSwapFrame(3);
	Draw_StageMap1->SetPlayONOFF(false);
	trans1->SetPosition(POS_X_STAGEMAP_1, POS_Y_STAGEMAP_1);						//	���W
	Col_StageMap1->SetCollisionSize(WIDTH_STAGEMAP_1, HEIGHT_STAGEMAP_1);			//	�����蔻��̑傫��
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objStageMap1);										//	�I�u�W�F�N�g�̒ǉ�
#pragma endregion

// �X�e�[�W�}�b�v2
#pragma region ---�X�e�[�W�}�b�v�Q
//�I�u�W�F�N�g����
	Object* objStageMap2 = new Object(STAGE2_NAME, UPDATE_UI, DRAW_EFFECT);
	//�R���|�[�l���g�̒ǉ�
	auto Trabs2 = objStageMap2->AddComponent<CTransform>();									//	���W
	auto Draw_StageMap2 = objStageMap2->AddComponent<CDrawEffect2D>();						//	�`��
	auto Col_StageMap2 = objStageMap2->AddComponent<CCollider>();							//	�����蔻��
	auto Menu_StageMap2 = objStageMap2->AddComponent<CMenu>();								//	
	objStageMap2->AddComponent<CPreview>();
	//�I�u�W�F�N�g�̐ݒ�
	Draw_StageMap2->SetTexture(TextureManager::GetInstance()->GetTexture(MV_SPRING_2_NUM));	//	�e�N�X�`��
	Draw_StageMap2->SetSize(WIDTH_STAGEMAP_2, HEIGHT_STAGEMAP_2);							//	�T�C�Y
	Draw_StageMap2->SetLoop(true);
	Draw_StageMap2->SetAnimSplit(4, 19);
	Draw_StageMap2->SetSwapFrame(3);
	Draw_StageMap2->SetPlayONOFF(false);
	Trabs2->SetPosition(POS_X_STAGEMAP_2, POS_Y_STAGEMAP_2);
	Col_StageMap2->SetCollisionSize(WIDTH_STAGEMAP_2, HEIGHT_STAGEMAP_2);		//	�����蔻��̑傫��
	//���X�g�ɒǉ�
	m_pObjectManager->AddObject(objStageMap2);									//	�I�u�W�F�N�g�̒ǉ�
#pragma endregion



	// ���(���E�܂�)�������������ق��������ł�
#pragma region ---�J�ږ�� ��
		//	�I�u�W�F�N�g����
	Object* objArrowLeft = new Object(LEFT_ARROW_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto Trans_ArrowLeft = objArrowLeft->AddComponent<CTransform>();			//	���W
	auto Draw_ArrowLeft = objArrowLeft->AddComponent<CDraw2D>();				//	�`��
	auto Col_ArrowLeft = objArrowLeft->AddComponent<CCollider>();				//	�����蔻��
	auto Menu_ArrowLeft = objArrowLeft->AddComponent<CMenu>();					//	���j���[
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_ArrowLeft->SetTexture(pTexManager->GetTexture(ARROW_UI_TEX_NUM));	//	�e�N�X�`��TextureManager::GetInstance()->GetTexture(ARROW_TEX_NUM)
	Draw_ArrowLeft->SetSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);				//	�T�C�Y
	Trans_ArrowLeft->SetPosition(POS_X_LEFT_ARROW, POS_Y_RIGHT_ARROW);
	Col_ArrowLeft->SetCollisionSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);		//	�����蔻��̑傫��
	Draw_ArrowLeft->SetAngle(0.0f);
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objArrowLeft);
#pragma endregion

#pragma region ---�J�ږ�� �E
	//	�I�u�W�F�N�g����
	Object* objArrow = new Object(RIGHT_ARROW_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto Trans_Arrow = objArrow->AddComponent<CTransform>();				//	���W
	auto Draw_Arrow = objArrow->AddComponent<CDraw2D>();					//	�`��
	auto Col_Arrow = objArrow->AddComponent<CCollider>();					//	�����蔻��
	auto Menu_Arrow = objArrow->AddComponent<CMenu>();						//	���j���[
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_Arrow->SetTexture(pTexManager->GetTexture(ARROW_UI_TEX_NUM));	//	�e�N�X�`��
	Draw_Arrow->SetSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);				//	�T�C�Y
	Trans_Arrow->SetPosition(POS_X_RIGHT_ARROW, POS_Y_RIGHT_ARROW);
	Col_Arrow->SetCollisionSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);		//	�����蔻��̑傫��
	Draw_Arrow->SetAngle(180.0f);
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objArrow);									//	�I�u�W�F�N�g�̒ǉ�
#pragma endregion

#pragma region --- �G�ߕ\��UI
	//	�I�u�W�F�N�g����
	Object* objSeazonUI = new Object("seazonUI", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto Trans_SeazonUI = objSeazonUI->AddComponent<CTransform>();							//	���W
	auto Draw_SeazonUI = objSeazonUI->AddComponent<CDraw2D>();								//	�`��
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_SeazonUI->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));//	�e�N�X�`��
	Trans_SeazonUI->SetPosition(POS_X_SEAZON_UI, POS_Y_SEAZON_UI);
	Draw_SeazonUI->SetSize(WIDTH_SEAZON_UI, HEIGHT_SEAZON_UI);							//	�T�C�Y
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objSeazonUI);											//	�I�u�W�F�N�g�̒ǉ�
#pragma endregion


#pragma region --- �X�e�[�W�t���[���\��
	for (int i = 0; i < 2; i++)
	{
		//�X�e�[�W��UI��ǉ�����
		Object* objStageFrame = new Object("StageFrame", UPDATE_UI, DRAW_FIELD);
		//�R���|�[�l���g����������
		auto Trans_StageFrame = objStageFrame->AddComponent<CTransform>();						//	���W
		auto Draw_StageFrame = objStageFrame->AddComponent<CDraw2D>();
		//�I�u�W�F�N�g�̐ݒ�
		//�����Ń|�W�V�����̐ݒ������
		switch (i) {
		case 0: Trans_StageFrame->SetPosition(POS_X_STAGE_FRAME_1, POS_Y_STAGE_FRAME_1); break;
		case 1: Trans_StageFrame->SetPosition(POS_X_STAGE_FRAME_2, POS_Y_STAGE_FRAME_2); break;

		default: break;
		}

		Draw_StageFrame->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_FRAME_TEX_NUM));
		Draw_StageFrame->SetSize(WIDTH_STAGE_FRAME, HEIGHT_STAGE_FRAME);
		//���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(objStageFrame);
	}
#pragma endregion

	std::list<Object*> Clovers;	// �N���[�o�[UI�����锠
#pragma region ---�擾�N���[�o�[UI
	//�X�e�[�W�}�b�v�P
	for (int i = 0; i < 4; i++)
	{
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g����������
		auto TransClov_UI	= pUI_Clover->AddComponent<CTransform>();			// �I�u�W�F�N�g���W
		auto DrawClov_UI	= pUI_Clover->AddComponent<CDraw2D>();				// �X�N���[���`��
		// �I�u�W�F�N�g�ݒ�
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);				// �T�C�Y
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_TEX_NUM));	// �e�N�X�`��
		// ���X�g�ɒǉ�
		m_pObjectManager->AddObject(pUI_Clover);

		// ���W�ݒ�
		switch (i)
		{
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}

		// �ꎞ�I�ɃN���[�o�[�����̃��X�g�ɒǉ�
		Clovers.push_back(pUI_Clover);
	}

	//�X�e�[�W�}�b�v�Q
	for (int i = 0; i < 4; i++)
	{
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g����������
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();				// �I�u�W�F�N�g���W
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();					// �X�N���[���`��
		// �I�u�W�F�N�g�ݒ�
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);				// �T�C�Y
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_TEX_NUM));	// �e�N�X�`��
		// ���X�g�ɒǉ�
		m_pObjectManager->AddObject(pUI_Clover);

		// ���W�ݒ�
		switch (i)
		{
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}

		// �ꎞ�I�ɃN���[�o�[�����̃��X�g�ɒǉ�
		Clovers.push_back(pUI_Clover);
	}
#pragma endregion


#pragma region --- �擾�N���[�o�[�\��/��\��
	/* clover_data.csv�̎d�l
		�s�F1�s�ڂ̂ݎg�p
		��F		�t-1		 0,  1,  2,  3
				�t-2		 4,  5,  6,  7
				�E
				�E
				�E
				�~-1		24, 25, 26, 27
				�~-2		28, 29, 30, 31
	*/
	int nNuber = 0;
	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));

	for (auto&& clover : Clovers)
	{
		if (m_Clover[(m_nCurrentSeason * 8) + nNuber] == 0)
		{
			clover->StopDraw();		// �`�悵�Ȃ�

			if (nNuber <= 3)	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
			else				Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
		}
		else
		{
			clover->ResumeDraw();	// �`�悷��
		}

		nNuber++;
	}
#pragma endregion

	
	//���݂̔w�i�ɕۑ�
	g_CurrentBG = Draw_BG;
	//���݂̃X�e�[�W�}�b�v�ɕۑ�
	g_CurrentMap1 = Draw_StageMap1;
	//���݂̃X�e�[�W�}�b�v�ɕۑ�
	g_CurrentMap2 = Draw_StageMap2;
	//���݂̋G��UI�ɕۑ�
	g_CurrentSeazonUI = Draw_SeazonUI;

	//���݂̃X�e�[�W����ۑ�
	g_CurrentStageName1 = Draw_StageNameUI_1;
	g_CurrentStageName2 = Draw_StageNameUI_2;
	//���݂̉ԃA�C�R����ۑ�
	g_CurrentClearFlower1 = Draw_ClearFlowerUI_1;
	g_CurrentClearFlower2 = Draw_ClearFlowerUI_2;

	//���݂̃X�e�[�W�N���A���UI��ۑ�
	g_CurrentStageClearUI1 = Draw_ClearUI_1;
	g_CurrentStageClearUI2 = Draw_ClearUI_2;

	//���݂̃v���C���[���f���ۑ�
	g_CurrentPlayer = DrawPlayer;

	//���݂̃u���b�N��ۑ�
	g_CurrentBlock = Draw_Block;

	// �Z���N�g���j���[
	m_pMenuManager->CreateSelectMenu();

	bMV1_PlayONOFF = false;
	bMV2_PlayONOFF = false;

	//	BGM�Đ�
	CSound::Play(BGM_STAGESELECT);

	//===== ���[�h�I������ =====
	Load::End();
}

// �I������
void SceneSelect::Uninit()
{
	// BGM��~
	CSound::Stop(BGM_STAGESELECT);

	//	�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();

	// ���j���[�̏I��
	MenuManager::GetInstance()->DeleteMenu();
}

// �X�V
void SceneSelect::Update()
{
	// ���j���[�X�V
	m_pMenuManager->Update();

	//	�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();

	//�~���Ă��郂�m�̍X�V
	g_SelectLeaf.UpdateSelectScene();

	// �G�߂��Ƃŕ\��������̂�ς���
	TextureManager* pTex = TextureManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();
	switch (m_nCurrentSeason)
	{

	case SPRING:
		// �G�ߏ��ۑ�
		m_OldSeason = SPRING;
		//�@�w�i(�t
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_SPRING_TEX_NUM));

		// ����̕���(�t
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(SPRING_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(1.0f, 0.55f, 1.0f);	//�Z���s���N

		//�X�e�[�W��
		g_CurrentStageName1->SetTexture(pTex->GetTexture(SPRING_1_TEX_NUM));//�t-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(SPRING_2_TEX_NUM));//�t-2

		//�N���A�A�C�R��
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SPRING_TEX_NUM));//���A�C�R��
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SPRING_TEX_NUM));

		//�V���t�B�[���f��
		g_CurrentPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
		//�u���b�N���f��
		g_CurrentBlock->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));

		//	����
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_SPRING_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_SPRING_2_NUM));

		break;

	case SUMMER:
		// �G�ߏ��ۑ�
		m_OldSeason = SUMMER;
		//�e�N�X�`���ǂݍ��݁i�āj
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_SUMMER_TEX_NUM));
		// ����̕���(��
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(SUMMER_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(0.0f, 1.0f, 0.0f);	//��

		//ADD
		g_CurrentStageName1->SetTexture(pTex->GetTexture(SUMMER_1_TEX_NUM));//�X�e�[�W��:��-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(SUMMER_2_TEX_NUM));//�X�e�[�W��:��-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SUMMER_TEX_NUM));//�q�}�����A�C�R��
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SUMMER_TEX_NUM));

		//�V���t�B�[���f��
		g_CurrentPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));

		//�u���b�N���f��
		g_CurrentBlock->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));

		//	����
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_SUMMER_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_SUMMER_2_NUM));

		break;

	case AUTUMN:
		// �G�ߏ��ۑ�
		m_OldSeason = AUTUMN;
		//�e�N�X�`���ǂݍ��݁i�H�j
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_AUTUMN_TEX_NUM));
		//�e�N�X�`���ݒ�
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(AUTUMN_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(1.0f, 0.5f, 0.0f);	//�I�����W

		g_CurrentStageName1->SetTexture(pTex->GetTexture(AUTUMN_1_TEX_NUM));//�X�e�[�W��:�H-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(AUTUMN_2_TEX_NUM));//�X�e�[�W��:�H-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_AUTUMN_TEX_NUM));//�R�X���X�A�C�R��
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_AUTUMN_TEX_NUM));

		//�V���t�B�[���f��
		g_CurrentPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));
		//�u���b�N���f��
		g_CurrentBlock->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));

		//	����
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_FALL_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_FALL_2_NUM));

		break;

	case WINTER:
		// �G�ߏ��ۑ�
		m_OldSeason = WINTER;
		//�e�N�X�`���ǂݍ��݁i�~�j
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_WINTER_TEX_NUM));
		//�e�N�X�`���ݒ�
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(WINTER_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(0.5f, 1.0f, 1.0f);	//���F
		//ADD
		g_CurrentStageName1->SetTexture(pTex->GetTexture(WINTER_1_TEX_NUM));//�X�e�[�W��:�~-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(WINTER_2_TEX_NUM));//�X�e�[�W��:�~-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_WINTER_TEX_NUM));//�փA�C�R��
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_WINTER_TEX_NUM));

		//�V���t�B�[���f��
		g_CurrentPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));
		//�u���b�N���f��
		g_CurrentBlock->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));

		//	����
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_WINTER_2_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_WINTER_1_NUM));

		break;
	}

	// �I���\���ǂ���
	if (m_SelectGrid[0][0 + m_nCurrentSeason * 2] == 0)	g_CurrentMap1->SetColor(0.3f, 0.3f, 0.3f);
	else												g_CurrentMap1->SetColor(1.0f, 1.0f, 1.0f);
	if (m_SelectGrid[0][1 + m_nCurrentSeason * 2] == 0)	g_CurrentMap2->SetColor(0.3f, 0.3f, 0.3f);
	else												g_CurrentMap2->SetColor(1.0f, 1.0f, 1.0f);

	//ADD
	// ***** �N���A(��)�̕\��/��\��
	//�N���A���Ă��Ȃ�������ԃA�C�R����\�����Ȃ�
	//�X�e�[�W�P
	if (m_SelectGrid[0][1 + m_nCurrentSeason * 2] == 1)
	{
		g_CurrentClearFlower1->SetAlpha(1.0f);	//�\��
		g_CurrentStageClearUI1->SetAlpha(1.0f);
		//bAnimPlaying = true;
	}
	else
	{
		g_CurrentClearFlower1->SetAlpha(0.0f);	//��\��
		g_CurrentStageClearUI1->SetAlpha(0.0f);

	}
	//�X�e�[�W�Q
	if (m_SelectGrid[0][0 + (m_nCurrentSeason + 1) * 2] == 1)
	{
		g_CurrentClearFlower2->SetAlpha(1.0f);	//�\��
		g_CurrentStageClearUI2->SetAlpha(1.0f);
	}
	else
	{
		g_CurrentClearFlower2->SetAlpha(0.0f);	//��\��
		g_CurrentStageClearUI2->SetAlpha(0.0f);
	}

	// �~��2�����Ⴄ����
	if (m_nCurrentSeason == WINTER)
	{
		if (m_SelectGrid[0][COMPLETE] == 1)
		{
			g_CurrentClearFlower2->SetAlpha(1.0f);	//�\��
			g_CurrentStageClearUI2->SetAlpha(1.0f);
			//g_CurrentStageClearUI1->SetAlpha(1.0f);
		}
		else
		{
			g_CurrentClearFlower2->SetAlpha(0.0f);	//��\��
			g_CurrentStageClearUI2->SetAlpha(0.0f);
			//g_CurrentStageClearUI1->SetAlpha(0.0f);
		}
	}

	// 2022/01/23 �t�A�~�̎��͖���\�����Ȃ�
	if (m_nCurrentSeason == SPRING)
	{
		Object* pLeftArrow = m_pObjectManager->GetGameObject(LEFT_ARROW_NAME);
		pLeftArrow->NoUse();
	}
	else if (m_nCurrentSeason == WINTER)
	{

		Object* pRightArrow = m_pObjectManager->GetGameObject(RIGHT_ARROW_NAME);
		pRightArrow->NoUse();
	}



	// 2022/01/24 �N���A�A�p�[�t�F�N�g�\���X�V tomoya
	// 
	//TODO:�@�p�[�t�F�N�g����肭���f����Ȃ�
	// 
	//�X�e�[�W�}�b�v�P
	if (m_SelectGrid[0][2 + m_nCurrentSeason * 2] >= 4)	//�N���[�o�[���ʂ�Max
	{
		g_CurrentStageClearUI1->SetTexture(pTex->GetTexture(PARFECT_UI_TEX_NUM));
		g_CurrentStageClearUI1->SetColor(1.0f, 0.75f, 0.0f);	//�F���Z�b�g(���F)
	}
	//�X�e�[�W�}�b�v�Q
	if (m_SelectGrid[0][3 + m_nCurrentSeason * 2] >= 4)	//�N���[�o�[���ʂ�Max
	{
		g_CurrentStageClearUI2->SetTexture(pTex->GetTexture(PARFECT_UI_TEX_NUM));
		g_CurrentStageClearUI2->SetColor(1.0f, 0.75f, 0.0f);	//�F���Z�b�g(���F)

	}


	/*
	// 2022/01/24 �N���A�A�j���[�V�����i�ԁjtomoya
	if (bAnimPlaying == true)
	{
		if (fFlowerSize.x <= WIDTH_CLEAR_FLOWER_UI)
		{
			fFlowerSize.x += 2.0f;
			fFlowerSize.y += 2.0f;
		}
		else
			bAnimPlaying == false;
		g_CurrentClearFlower1->SetSize(fFlowerSize.x, fFlowerSize.y);
	}
	*/

	// �J�����o
	SceneSelect::Clear();
}

// �`��
void SceneSelect::Draw()
{
	//	�I�u�W�F�N�g���X�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	m_pMenuManager->Draw();
	SetZBuffer(false);

#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	//Text("Seazon : %d", i);
	Text("Seazon   : %d", m_nCurrentSeason);
	Text("GridSize : %d", m_SelectGrid.size());
	End();
#endif // _DEBUG
}

// �Z���N�g��ʂ̋G�ߐ؂�ւ�
void SceneSelect::ChangeSceason(int n)
{
	m_nCurrentSeason += n;
	if (m_nCurrentSeason < 0) m_nCurrentSeason = 0;
	if (m_nCurrentSeason > 3) m_nCurrentSeason = 3;

	ObjectManager* pObjectManager = ObjectManager::GetInstance();
	TextureManager* pTexManager = TextureManager::GetInstance();

	// ����\�����Ă���
	Object* pLeftArrow = pObjectManager->GetGameObject(LEFT_ARROW_NAME);
	pLeftArrow->ResumeUpdate();
	pLeftArrow->ResumeDraw();
	Object* pRightArrow = pObjectManager->GetGameObject(RIGHT_ARROW_NAME);
	pRightArrow->ResumeUpdate();
	pRightArrow->ResumeDraw();

	g_CurrentStageClearUI1->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	g_CurrentStageClearUI2->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	

	bool bPerfect1 = true;
	bool bPerfect2 = true;
	std::list<Object*> Clovers;	// �N���[�o�[���ꎞ�I�ɓ���锠
	int nNuber = 7;				// �N���[�o�[�ԍ�
	pObjectManager->GetGameObjects(CLOVER_UI_NAME, Clovers);	// �N���[�o�[�����X�g�ɓ˂�����
#pragma region --- �擾�N���[�o�[�\��/��\��

	/* clover_data.csv�̎d�l
		�s�F1�s�ڂ̂ݎg�p
		��F		�t-1		 0,  1,  2,  3
				�t-2		 4,  5,  6,  7
				�E
				�E
				�E
				�~-1		24, 25, 26, 27
				�~-2		28, 29, 30, 31
	*/

	for (auto&& clover : Clovers)
	{
		// �N���[�o�[������Ă��邩���Ȃ���
		if (m_Clover[(m_nCurrentSeason * 8) + nNuber] == 0)
		{
			clover->StopDraw();		// �`�悵�Ȃ�

			if (nNuber <= 3)	g_CurrentStageClearUI1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
			else				g_CurrentStageClearUI2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
		}
		else
		{
			clover->ResumeDraw();	// �`�悷��
		}

		nNuber--;
	}
#pragma endregion

	
}

// �X�e�[�W�I���\��
bool SceneSelect::GetSelectable(int stage_num)
{
	
	int x = m_nCurrentSeason * 2 + stage_num;

	if (m_SelectGrid[0][x] == 0)
	{
		if(SceneManager::Instance()->GetScene() == SCENE_SELECT)	CSound::Play(SE_ERROR);
		return false;
	}
	else return true;

}

// load
bool SceneSelect::Load()
{
	// �N���[�o�[�擾���̓ǂݍ���
	if (FileManager::GetInstance()->Load(CLOVER_DATA_CSV) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}
	// �N���A�󋵂̓ǂݍ���
	if (FileManager::GetInstance()->Load(SAVE_DATA_CSV) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}
	
	// �N���A�󋵂���������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
	m_SelectGrid.resize(CSV->GetRowSize());				// �s�̐���ݒ�
	for (int y = 0; y < CSV->GetRowSize(); ++y)
	{
		m_SelectGrid[y].resize(CSV->GetColumnSize(y));	// ��̐���ݒ�
	
		for (int x = 0; x < CSV->GetColumnSize(y); x++)
		{
			// �v�f���i�[����
			m_SelectGrid[y][x] = CSV->GetInt(x, y);
		}
	}

	// �N���[�o�[�̎擾�󋵂��ڂ�
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	m_Clover.clear();
	for (int x = 0; x < CloverCSV->GetColumnSize(0); x++)
	{
		// �v�f���i�[����
		m_Clover.push_back(CloverCSV->GetInt(x, 0));
	}
	
	return true;
}

// save
bool SceneSelect::Save()
{

	// �N���A�󋵂̕ۑ�
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
	if (CSV == nullptr) return false;
	if (CSV->Save(SAVE_DATA_CSV) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	// �N���[�o�[�̎擾�󋵂̕ۑ�
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	if (CloverCSV == nullptr) return false;
	if (CloverCSV->Save(CLOVER_DATA_CSV) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

// csv�t�@�C��������
bool SceneSelect::NewGame()
{
	m_OldSeason = SPRING;

	// ��������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);

	for (int nRow = 0; nRow < m_SelectGrid.size(); nRow++)
	{
		for (int nColumn = 0; nColumn < m_SelectGrid[nRow].size(); nColumn++)
		{
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	// �ŏ��̃X�e�[�W�I���͉\
	CSV->SetInt(0, 0, 1);

	// �N���[�o�[�̎擾��
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	for (int nColumn = 0; nColumn < m_Clover.size(); nColumn++)
	{
		CloverCSV->SetInt(nColumn, 0, 0);
	}


	SceneSelect::Save();

	return true;
}

// �N���A�X�e�[�W��ۑ�
bool SceneSelect::SaveClearInfo(int nStage, bool bClover[4])
{
	// �N���[�o�[��csv�Q�b�g
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);

	/*---------------- �O��Q�b�g���ĂȂ��č���Q�b�g�����ꍇ�A������o�t���O��ON�ɂ��� ----------------*/
	int nX, nY;
	int nGameSeason = SceneGame::GetInstance()->GetSeason();
	nX = nGameSeason * 4;	// �N���[�o�[�擾�󋵂����̗�ɓ����Ă���
	nY = 0;

	// �l��̃��[�v
	for (int i = 0; i < 4; i++)
	{
		if (nGameSeason == WINTER_2) continue;
		// �V�����N���[�o�[��������ꍇ
		if (CloverCSV->GetInt(nX + i, nY) == 0 && bClover[i] == true)
		{
			if (nGameSeason == SPRING_1 || nGameSeason == SUMMER_1 || nGameSeason == FALL_1 || nGameSeason == WINTER_1)
				m_bReleaseClover[i] = true;
			if (nGameSeason == SPRING_2 || nGameSeason == SUMMER_2 || nGameSeason == FALL_2 || nGameSeason == WINTER_2)
				m_bReleaseClover[i + 4] = true;
		}

	}
	/*----------------------------------------------------------------------------------*/

	// �N���[�o�[������������
	int Clovernumber = nStage * 4;
	for (int i = 0; i < 4; i++)
	{
		CloverCSV->SetInt(Clovernumber + i, 0, bClover[i]);	// (��, �s, �i�[���鐔��)
	}

	// �~��2stage���N���A�����Ƃ�
	if (nStage == WINTER_2)
	{
		// ��������
		ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);

		// �N���A�󋵂�����������
		CSV->SetInt(COMPLETE, 0, 1);

		// �Z�[�u
		SceneSelect::Save();

		return true;
	}
	else
	{
		// �N���A�󋵂�����������
		ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
		// ���̃X�e�[�W�̃t���O��
		int x = nStage + 1;

		/*---------------- �O��N���A���ĂȂ��č���N���A�����ꍇ�A������o�t���O��ON�ɂ��� ----------------*/
		int nGameSeason = SceneGame::GetInstance()->GetSeason();
		
		// �V�����N���[�o�[��������ꍇ
		if (CSV->GetInt(x , 0) == 0)
		{
			if (nGameSeason == SPRING_1 || nGameSeason == SUMMER_1 || nGameSeason == FALL_1 || nGameSeason == WINTER_1)
				m_bRelease1 = true;
			if (nGameSeason == SPRING_2 || nGameSeason == SUMMER_2 || nGameSeason == FALL_2 || nGameSeason == WINTER_2)
				m_bRelease2 = true;
		}

		/*----------------------------------------------------------------------------------*/


		CSV->SetInt(x, 0, 1);	// (��, �s, �i�[���鐔��)

		// csv�ɕۑ�
		SceneSelect::Save();
	}

	return true;
}


// �N���[�o�[�̎擾�󋵂��L�^����
bool SceneSelect::SaveCloverInfo(int nStage, int cloverNumber)
{
	/* clover_data.csv�̎d�l
		�s�F1�s�ڂ̂ݎg�p
		��F		�t-1		 0,  1,  2,  3
				�t-2		 4,  5,  6,  7
				�E
				�E
				�E
				�~-1		24, 25, 26, 27
				�~-2		28, 29, 30, 31
	*/
	int x = nStage * 4 + cloverNumber;

	//// �N���[�o�[�̎擾�󋵂̕ۑ�
	//ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	//CloverCSV->SetInt(x, 0, 1);	// (��, �s, �i�[���鐔��)

	m_Clover[x] = 1;

	return true;
}

// debug�p
SceneSelect::Row SceneSelect::GetSelectGrid()
{
	return m_SelectGrid;
}




SceneSelect::Column SceneSelect::GetClovers()
{
	return m_Clover;
}


void SceneSelect::Clear()
{
	// �N���[�o�[�擾���o
	for (int nCloverNumber = 0; nCloverNumber < 8; nCloverNumber++)
	{
		if (m_bReleaseClover[nCloverNumber])
		{
			std::list<Object*> Clovers;	// �N���[�o�[UI�����锠
			// �N���[�o�[UI�I�u�W�F�N�g�����X�g�ɓ˂�����
			ObjectManager::GetInstance()->GetGameObjects(CLOVER_UI_NAME, Clovers);

			// �N���[�o�[����肷��
			std::list<Object*>::iterator itr = Clovers.begin();
			for (int i = 0; i < 7 - nCloverNumber; i++)	itr++;

			// �ړI�̃N���[�o�[�ɏo�����o�̃R���|�[�l���g��ǉ�����
			auto scale1 = (*itr)->AddComponent<CGetUI>();

			// �R���|�[�l���g�̏�����
			scale1->Start();

			// �ړI�̑傫����ݒ�
			scale1->SetUISize(XMFLOAT2(WIDTH_CLOVER_UI, WIDTH_CLOVER_UI));

			//�@�g��X�s�[�h
			scale1->SetUIRate(1.0f);

			// �x������
			if (nCloverNumber == 0 || nCloverNumber == 4)	scale1->SetDelayTimer(120);
			if (nCloverNumber == 1 || nCloverNumber == 5)	scale1->SetDelayTimer(160);
			if (nCloverNumber == 2 || nCloverNumber == 6)	scale1->SetDelayTimer(200);
			if (nCloverNumber == 3 || nCloverNumber == 7)	scale1->SetDelayTimer(240);
			
			// �傫����0�ɂ��Ă���
			(*itr)->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

			// �t���O���~�낷
			m_bReleaseClover[nCloverNumber] = false;
		}
	}
	// �X�e�[�W1�̉�����o
	if (m_bRelease1)
	{
		auto pClear = ObjectManager::GetInstance()->GetGameObject(CLEAR_UI_1_NAME);
		auto pFlower = ObjectManager::GetInstance()->GetGameObject(CLEAR_FLOWER_UI_1_NAME);

		// �ړI�̃N���[�o�[�ɏo�����o�̃R���|�[�l���g��ǉ�����
		auto scaleClear = pClear->AddComponent<CGetUI>();
		auto scaleFlower = pFlower->AddComponent<CGetUI>();

		// �R���|�[�l���g�̏�����
		scaleClear->Start();
		scaleFlower->Start();

		// �ړI�̑傫����ݒ�
		scaleClear->SetUISize(XMFLOAT2(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI));
		scaleFlower->SetUISize(XMFLOAT2(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI));

		//�@�g��X�s�[�h
		scaleClear->SetUIRate(4.0f);
		scaleFlower->SetUIRate(3.0f);

		// �x������
		scaleClear->SetDelayTimer(0);
		scaleFlower->SetDelayTimer(60);

		// �傫����0�ɂ��Ă���
		pClear->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);
		pFlower->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

		// �t���O���~�낷
		m_bRelease1 = false;
	}

	// �X�e�[�W1�̉�����o
	if (m_bRelease2)
	{
		auto pClear = ObjectManager::GetInstance()->GetGameObject(CLEAR_UI_2_NAME);
		auto pFlower = ObjectManager::GetInstance()->GetGameObject(CLEAR_FLOWER_UI_2_NAME);

		// �ړI�̃N���[�o�[�ɏo�����o�̃R���|�[�l���g��ǉ�����
		auto scaleClear = pClear->AddComponent<CGetUI>();
		auto scaleFlower = pFlower->AddComponent<CGetUI>();

		// �R���|�[�l���g�̏�����
		scaleClear->Start();
		scaleFlower->Start();

		// �ړI�̑傫����ݒ�
		scaleClear->SetUISize(XMFLOAT2(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI));
		scaleFlower->SetUISize(XMFLOAT2(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI));

		//�@�g��X�s�[�h
		scaleClear->SetUIRate(4.0f);
		scaleFlower->SetUIRate(3.0f);

		// �x������
		scaleClear->SetDelayTimer(0);
		scaleFlower->SetDelayTimer(60);

		// �傫����0�ɂ��Ă���
		pClear->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);
		pFlower->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

		// �t���O���~�낷
		m_bRelease2 = false;
	}
}