//===== �C���N���[�h =====
#include "sceneGame.h"
#include "fade.h"
#include "imgui.h"

#include "MenuManager.h"
#include "ObjectManager.h"
#include "FileManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "WindManager.h"
#include "CreateLeaf.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "Sound.h"
#include "Camera.h"

#include "Component.h"				//	�R���|�[�l���g
#include "TransformComponent.h"		//	�ʒu�E���x�E�����x
#include "Draw2dComponent.h"		//	2D�`��
#include "Draw3dComponent.h"		//	���f���`��
#include "DrawMeshComponent.h"		//	���b�V���`��
#include "BillboardComponent.h"		//	�r���{�[�h�`��
#include "DrawEffectComponent.h"		//	�G�t�F�N�g�`��
#include "ColliderComponent.h"		//	�����蔻��
#include "PlayerComponent.h"		//	�v���C���[����
#include "GravityComponent.h"		//	�d��
#include "HomingComponent.h"		//	�}�E�X�Ǐ]
#include "TexScrollComponent.h"		//	�e�N�X�`���X�N���[��
#include "GaugeComponent.h"			//	�Q�[�W
#include "AnimationComponent.h"		//	�A�j���[�V����
#include "MenuComponent.h"			//	���j���[
#include "FadeComponent.h"			//	�t�F�[�h
#include "FallComponent.h"			//	�����d��
#include "WindComponent.h"			//	��
#include "SeedComponent.h"			//	��
#include "FollowCompomemt.h"		//	�e�q�֌W
#include "OffScreenComponent.h"		//	��ʊO����
#include "SpiderWebComponent.h"		//	�N���̑�
#include "Animation.h"
#include "GoMenuComponent.h"
#include "TutorialComponent.h"
#include "EffectManager.h"

#include "SceneSummer.h"
#include "SceneSpring.h"
#include "SceneFall.h"
#include "SceneWinter.h"

#include "Load.h"

//=== �ÓI�����o�ϐ�
SceneGame* SceneGame::m_pInstance = nullptr;
// ���[�u
std::unordered_map<CAssimpModel*, bool> SceneGame::IceManager;	// �X�̐�

CLeaf g_Leaf;

// �R���X�g���N�^
SceneGame::SceneGame() 
	: m_pWindManager(nullptr), m_pStageManager(nullptr)
	, m_bPauseMode(false), m_bEditMode(false)
{
	//	�S�V�[�������X�g�ɒǉ�
	m_GameSceneList.push_back(new SceneSpring());	// �t�P
	m_GameSceneList.push_back(new SceneSpring());	// �t�Q
	m_GameSceneList.push_back(new SceneSummer());	// �ĂP
	m_GameSceneList.push_back(new SceneSummer());	// �ĂQ
	m_GameSceneList.push_back(new SceneFall());		// �H�P
	m_GameSceneList.push_back(new SceneFall());		// �H�Q
	m_GameSceneList.push_back(new SceneWinter());	// �~�P
	m_GameSceneList.push_back(new SceneWinter());	// �~�Q

	// �ŏ��͏t
	m_eSeasonState = SPRING_1;
	m_CurrentScene = m_GameSceneList[m_eSeasonState];
}

// �f�X�g���N�^
SceneGame::~SceneGame()
{
	//	�V�[�����X�g���폜
	for (auto scene : m_GameSceneList)
		delete scene;

	//	���X�g����ɂ���
	m_GameSceneList.clear();

	// ���[�u
	IceManager.clear();
}

// ������
void SceneGame::Init()
{
	//===== ���[�h��ʂ̃X���b�h����� =====
	Load::Begin();
	
	// �J����������
	CCamera::Get()->Init();

	m_bEditMode = false;	// �ҏW���[�h�t���OOFF
	m_bPauseMode = false;	// �|�[�Y�t���OOFF

	//===== �Q�[���I�u�W�F�N�g�̏����� =====

	//	�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager	= ObjectManager::GetInstance();		// �I�u�W�F�N�g�}�l�[�W���[
	m_pWindManager		= WindManager::GetInstance();		// ���}�l�[�W���[
	m_pStageManager		= StageManager::GetInstance();		// �X�e�[�W�}�l�[�W���[
	m_pMenuManager		= MenuManager::GetInstance();		// ���j���[�}�l�[�W���[

	/* �e�N�X�`���ǂݍ��� */
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_WIND, WIND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WIND_GAUGE, WIND_GAUGE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_FRAME_GAUGE, FRAME_GAUGE_TEX_NUM);

	// ��A�N���[�o�[��UI
	pTexManager->AddTexture(PATH_SEED_UI, SEED_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI, CLOVER_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_SEED_UI_FRAME, SEED_UI_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI_FRAME, CLOVER_UI_FRAME_TEX_NUM);
	
	// �w偂̑�
	pTexManager->AddTexture(PATH_TEX_SPIDER, SPIDER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPIDER_TWO, SPIDER_TWO_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPIDER_THREE, SPIDER_THREE_TEX_NUM);
	// �����t
	pTexManager->AddTexture(PATH_TEX_OTIBA, OTIBA_TEX_NUM);
	// �ӂ��Ă������
	pTexManager->AddTexture(PATH_SUMMER_LEAF, SUMMER_LEAF_TEX_NUM);	// ��
	pTexManager->AddTexture(PATH_FALL_LEAF, FALL_LEAF_TEX_NUM);		// �H
	pTexManager->AddTexture(PATH_TEX_SPRING_LEAF, LEAF_SPRING_NUM);	// �t
	// ��A�N���[�o�[�̎擾�G�t�F�N�g
	pTexManager->AddTexture(PATH_TEX_EFFECT_CLOVER, EFFECT_CLOVER_TEX_NUM);	// �N���[�o�[
	// �G�ߕ\��
	pTexManager->AddTexture(PATH_TEX_SPRING_1,		 SPRING_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPRING_2,		 SPRING_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SUMMER_1,		 SUMMER_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SUMMER_2,		 SUMMER_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_AUTUMN_1,		 AUTUMN_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_AUTUMN_2,		 AUTUMN_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WINTER_1,		 WINTER_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WINTER_2,		 WINTER_2_TEX_NUM);

	//2022/1/26/WindCar Shimizu Yosuke ----------------------------------------------------------
	//����
	pTexManager->AddTexture(PATH_TEX_WIND_CAR, WIND_CAR_TEX_NUM);
	//�_
	pTexManager->AddTexture(PATH_TEX_WINDCAR_STICK, WINDCAR_STICK_TEX_NUM);
	//-------------------------------------------------------------------------------------------

	pTexManager->AddTexture(PATH_TEX_TUTORIAL_WINTER, TUTORIAL_WINTER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_FALL, TUTORIAL_FALL_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_SPIDER, TUTORIAL_SPIDER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_UI, TUTORIAL_UI_TEX_NUM);

	//2022/1/31/Load_Tutorial Shimizu Yosuke ----------------------------------------
	pTexManager->AddTexture(PATH_PLAYER_TUTORIAL, PLAYER_TUTORIAL_TEX_NUM);
	//-------------------------------------------------------------------------------

	/* ���f���ǂݍ��� */
	ModelManager* pModelManager = ModelManager::GetInstance();
	pModelManager->AddModel(PATH_MODEL_CLOVER, CLOVER_MODEL_NUM);				// �N���[�o�[
	pModelManager->AddModel(PATH_MODEL_SEED, SEED_MODEL_NUM);					// ����
	pModelManager->AddModel(PATH_MODEL_POT, POT_MODEL_NUM);						// �A�ؔ�
	pModelManager->AddModel(PATH_MODEL_ICICLE, ICICLE_MODEL_NUM);				// ���
	// �A�j���[�V�����������ĕ����𓯂��V�[���ŕ`�悵�Ȃ��Ƃ����Ȃ����̂͂��̕��̃C���X�^���X�����
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM1);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM2);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM3);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM4);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM5);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM6);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM7);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM8);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM9);		// �X
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM10);		// �X
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = false;


	


#pragma region	---�v���C���[
	//	�I�u�W�F�N�g����
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	�R���|�[�l���g�̒ǉ�
	auto TransPlayer = Player->AddComponent<CTransform>();			//	�g�����X�t�H�[��
	auto ColliderPlayer = Player->AddComponent<CCollider>();		//	�Փˋ@�\
	auto DrawPlayer = Player->AddComponent<CDrawModel>();				//	���f���`��@�\
	auto DrawClear = Player->AddComponent<CDrawModel>();				//	���f���`��@�\(�N���A�A�j���[�V����)
	auto OffScreenPlayer = Player->AddComponent<COffScreen>();		//	��ʊO����
	auto pPlayer = Player->AddComponent<CPlayer>();					//	�v���C���[�@�\
	auto FallPlayer = Player->AddComponent<CFall>();				//	�d��
	auto animationPlayer = Player->AddComponent<CAnimation>();		//	fbx�؂�ւ�
	// 3D���X�g�ɒǉ�
	m_pObjectManager->AddObject(Player);
#pragma endregion

	//�G�߂ɂ���ĕς��I�u�W�F�N�g������(�����ŋG�߂��Ƃ̃��f���ȂǓǂݍ���ł�
	m_CurrentScene->Init();

#pragma region	�v���C���[���̐ݒ�
	TransPlayer->SetScale(3.0f, 3.0f, 3.0f);							//	�X�P�[��
	TransPlayer->SetVelocity(2.0f, 0.0f, 0.0f);							//	�X�P�[��
	TransPlayer->SetPosition(PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z);	//	�����z�u
	ColliderPlayer->SetCollisionSize(PLAYER_SIZE_X, PLAYER_SIZE_Y);		//	�����蔻��̑傫��
	ColliderPlayer->SetOffset(PLAYER_OFFSET_X, PLAYER_OFFSET_Y);		//	���S���W�̃I�t�Z�b�g
	OffScreenPlayer->SetFinction(ON_SCREEN);							//	��ʓ��ɓ���鏈��
	// fbx�؂�ւ��ݒ�
	animationPlayer->AddAnimation(DrawPlayer, FBX_ONE);		// �ҋ@�A������Ă���A������A�E��
	animationPlayer->AddAnimation(DrawClear, FBX_TWO);		// �S�[��
	animationPlayer->ChangeAnimation(FBX_ONE);				// �ŏ���1�ڂ�fbx
	// �G�߂ɂ���ăv���C���[�̃e�N�X�`����ς���
	switch (m_eSeasonState)
	{
	case SPRING_1:
	case SPRING_2:	// �t�̃e�N�X�`��
		DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(SPRING_GOAL_MODEL_NUM));	// �N���A
		break;
	case SUMMER_1:
	case SUMMER_2:	// �Ẵe�N�X�`��
		DrawPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(SUMMER_GOAL_MODEL_NUM));	// �N���A
		break;
	case FALL_1:
	case FALL_2:	// �H�e�N�X�`��
		DrawPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(FALL_GOAL_MODEL_NUM));		// �N���A
		break;
	case WINTER_1:
	case WINTER_2:	// �~�e�N�X�`��
		DrawPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(WINTER_GOAL_MODEL_NUM));	// �N���A
		break;
	case MAX_SEASON:	break;
	default:			break;
	}

	// �A�j���[�V�����ݒ�
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);			// �ҋ@
	DrawPlayer->AddAnimation(PLAYER_CARRIED, PLAYER_ANIM_CARRIED_S, PLAYER_ANIM_CARRIED_E, true);	// ���ɐ�����Ă���
	DrawPlayer->AddAnimation(PLAYER_FALL, PLAYER_ANIM_FALL_S, PLAYER_ANIM_FALL_E, true);			// ������
	DrawPlayer->AddAnimation(PLAYER_GET, PLAYER_ANIM_GET_S, PLAYER_ANIM_GET_E, false);				// �E��
	DrawPlayer->AddAnimation(PLAYER_BIND, PLAYER_ANIM_BIND_S, PLAYER_ANIM_BIND_E, true);			// �N���̑�
	DrawPlayer->AddAnimation(PLAYER_GAMEOVER, PLAYER_ANIM_GAMEOVER_S, PLAYER_ANIM_GAMEOVER_E, false);// �Q�[���I�[�o�[
	DrawPlayer->SetPlaySpeed(PLAYER_BIND, 0.012);	// �N���̑��̍Đ����x��ݒ�
	DrawPlayer->AlwaysAnimation(true);				// ��ɃA�j���[�V����
	DrawPlayer->Play(PLAYER_BIND);					// �Đ�
	// �S�[���A�j���[�V����
	DrawClear->AddAnimation(PLAYER_GOAL, PLAYER_ANIM_GOAL_S, PLAYER_ANIM_GOAL_E, false);
#pragma endregion

	//--- ���ׂẴV�[���ɋ��ʂ���I�u�W�F�N�g�̐���
#pragma region ---�r�l�b�g
//	�I�u�W�F�N�g����
	Object* objVinetto = new Object("Vinetto", UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	objVinetto->AddComponent<CTransform>();								// ���W
	auto Draw_Vinetto = objVinetto->AddComponent<CDraw2D>();			// �`��
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_Vinetto->SetTexture(pTexManager->GetTexture(VINETTO_TEX_NUM));	// �e�N�X�`��
	Draw_Vinetto->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);					// �T�C�Y
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objVinetto);							// �I�u�W�F�N�g�̒ǉ�
#pragma endregion


	// Player�𒍎��_�ɐݒ�
	CCamera::Get()->SetAxisX(&(Player->GetComponent<CTransform>()->Pos.x));

#pragma region ---���Q�[�W
	//�@���̃g�����X�t�H�[���ƕ`��A���̃Q�[�W���̃R���|�[�l���g��ǉ����Ă��
	Object* pWindBar = new Object("Gauge", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto pUI_WinTransform = pWindBar->AddComponent<CTransform>();			//	���W
	auto pUI_WinDraw = pWindBar->AddComponent<CDrawEffect2D>();				//	�X�N���[���`��
	auto pUI_WinGauge = pWindBar->AddComponent<CGauge>();					//	���Q�[�W
	//�@�I�u�W�F�N�g�̐ݒ�
	pUI_WinTransform->SetPosition(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);		//�@���W
	pUI_WinDraw->SetTexture(pTexManager->GetTexture(WIND_GAUGE_TEX_NUM));	//�@�e�N�X�`��
	pUI_WinDraw->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);				//�@�T�C�Y
	pUI_WinDraw->SetAlpha(0.8f);
	pUI_WinGauge->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);				//	�Q�[�W�̃T�C�Y
	pUI_WinGauge->SetInitPos(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);			//	�����̍��W
	pUI_WinGauge->SetMaxGauge(WIMD_GAUGE_MAX);								//	�ő�Q�[�W��
	pUI_WinDraw->SetAnimSplit(4, 4);
	pUI_WinDraw->SetLoop(true);
	pUI_WinDraw->SetSwapFrame(1);
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(pWindBar);
#pragma endregion

#pragma region ---���Q�[�W�̘g
	//	�I�u�W�F�N�g����
	Object* pGaugeFrame = new Object("frame", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto TransFrame = pGaugeFrame->AddComponent<CTransform>();			//	���W
	auto DrawFrame = pGaugeFrame->AddComponent<CDrawEffect2D>();		//	�X�N���[���`��
	//	�I�u�W�F�N�g�̐ݒ�
	TransFrame->SetPosition(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);		//	���W
	DrawFrame->SetTexture(pTexManager->GetTexture(FRAME_GAUGE_TEX_NUM));//	�e�N�X�`��
	DrawFrame->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);			//	�T�C�Y
	DrawFrame->SetAnimSplit(4, 4);
	DrawFrame->SetLoop(true);
	DrawFrame->SetSwapFrame(1);
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(pGaugeFrame);
#pragma endregion

//#pragma region	---�J�[�\��
//	//	�I�u�W�F�N�g����
//	Object* objCursor = new Object(CURSOR_NAME, UPDATE_DEBUG, DRAW_DEBUG);
//	//	�R���|�[�l���g�̒ǉ�
//	objCursor->AddComponent<CTransform>();					//	�g�����X�t�H�[��
//	auto Draw_cursor = objCursor->AddComponent<CDraw2D>();	//	2D�`��@�\
//	auto homing = objCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
//	auto collider = objCursor->AddComponent<CCollider>();	//	�����蔻��
//	//	�I�u�W�F�N�g�̐ݒ�
//	Draw_cursor->SetTexture(nullptr);						//	�e�N�X�`��
//	Draw_cursor->SetSize(30.0f, 30.0f);						//	�T�C�Y
//	Draw_cursor->SetAlpha(0.5f);
//	Draw_cursor->SetColor(0.5f, 1.0f, 0.5f);
//	collider->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);
//	//	���X�g�ɒǉ�
//	m_pObjectManager->AddObject(objCursor);
//
//#ifdef NO_SHOW
//	Draw_cursor->SetSize(0.0f, 0.0f);			//	�T�C�Y
//#endif // _RELEASE
//#pragma endregion

#pragma region ---UI�g
	for (int i = 0; i < 4; i++)
	{
		//�N���[�o�[��UI��ǉ�����
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI_FRAME);
		//�R���|�[�l���g����������
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
		//�I�u�W�F�N�g�̐ݒ�
		//�����Ń|�W�V�����̐ݒ������
		switch (i)
		{
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

		DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);

		//���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(pUI_Clover);
	}
	
	//�I�u�W�F�N�g�̐���
	Object* pSeed_UI = new Object(SEED_UI_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//  �R���|�l���g�̒ǉ�
	auto TransSeed_UI = pSeed_UI->AddComponent<CTransform>();			//�@���W
	auto DrawSeed_UI = pSeed_UI->AddComponent<CDraw2D>();				//�@�`��
	//  �I�u�W�F�N�g�̐ݒ�
	TransSeed_UI->SetPosition(SEED_UI_POS_X, SEED_UI_POS_Y);			//  ���W
	DrawSeed_UI->SetTexture(TextureManager::GetInstance()->GetTexture(SEED_UI_FRAME_TEX_NUM));	//  �e�N�X�`���̃Z�b�g
	DrawSeed_UI->SetSize(SEED_UI_SIZE_X, SEED_UI_SIZE_Y);				//  �T�C�Y
	DrawSeed_UI->SetColor(1.0f, 1.0f, 1.0f);
	//  ���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pSeed_UI);

#pragma endregion

	//2022/1/12 Shimizu Yosuke -----------------------------------
#pragma region ----�|�[�Y�{�^��
	//�|�[�Y��ʗp�̃{�^��
	//�I�u�W�F�N�g�̍쐬
	Object* pStop_Game = new Object(STOP_GAME_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//�R���|�l���g�̍쐬
	auto TransGame_Stop = pStop_Game->AddComponent<CTransform>();								//�@�g�����X�t�H�[��
	auto DrawGame_Stop = pStop_Game->AddComponent<CDraw2D>();									//�@�`��
	auto ColliderGame_Stop = pStop_Game->AddComponent<CCollider>();								//�@�����蔻��
	pStop_Game->AddComponent<CMenu>();														//  ���j���[�ɍs�����߂̃R���|�l���g
	//�I�u�W�F�N�g�̐ݒ�
	TransGame_Stop->SetPosition(STOP_GAME_POS_X, STOP_GAME_POS_Y);								//�@���W
	DrawGame_Stop->SetSize(STOP_GAME_SIZE_X, STOP_GAME_SIZE_Y);									//�@�T�C�Y
	DrawGame_Stop->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));	//�@�e�N�X�`��
	ColliderGame_Stop->SetCollisionSize(STOP_GAME_COLLSION_WIDTH, STOP_GAME_COLLSION_HEIGHT);	//�@�����蔻��̃T�C�Y
	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pStop_Game);
#pragma endregion
	//------------------------------------------------------------


	// �|�[�Y�{�^���̉��ɒǉ�
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------
#pragma region ---�`���[�g���A���{�^��
	//�I�u�W�F�N�g�̐���
		Object* pTutorial_UI = new Object(WINDCAR_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//�R���|�[�l���g�̍쐬
	auto TransTutorial_UI = pTutorial_UI->AddComponent<CTransform>();									//  �g�����X�t�H�[��
	auto DrawTutorial_UI = pTutorial_UI->AddComponent<CDraw2D>();										//�@�`��
	auto CollTutorial_UI = pTutorial_UI->AddComponent<CCollider>();										//  �Փ˔���
	pTutorial_UI->AddComponent<CMenu>();																//  ���j���[�p�̃R���|�[�l���g
	pTutorial_UI->AddComponent<CTutorial>();															//  �`���[�g���A��
	//�I�u�W�F�N�g�̐ݒ�
	TransTutorial_UI->SetPosition(WINDCAR_POS_X, WINDCAR_POS_Y);											//  ���W
	DrawTutorial_UI->SetSize(WINDCAR_SIZE_X, WINDCAR_SIZE_Y);											//  �T�C�Y
	DrawTutorial_UI->SetTexture(TextureManager::GetInstance()->GetTexture(WIND_CAR_TEX_NUM));																//�@�e�N�X�`��
	//DrawTutorial_UI->SetColor(1.0f,0.0f,1.0f);														//  �J���[
	CollTutorial_UI->SetCollisionSize(WINDCAR_COLL_WIDTH, WINDCAR_COLL_HEIGHT);							//  �Փ˔���̑傫��
	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pTutorial_UI);
#pragma endregion
	//----------------------------------------------------------------

	//2022/1/26/Touch_Totorial Shimizu Yosuke ------------------------
#pragma region ---�`���[�g���A���X�e�B�b�N
	//�I�u�W�F�N�g�̐���
	Object* pStick = new Object(WINDCAR_STICK_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//�R���|�[�l���g�̒ǉ�
	auto TransStick = pStick->AddComponent<CTransform>();												//  �g�����X�t�H�[��
	auto DrawStick = pStick->AddComponent<CDraw2D>();													//  �`��
	//�I�u�W�F�N�g�̐ݒ�
	TransStick->SetPosition(WINDCAR_STICK_POS_X, WINDCAR_STICK_POS_Y);									//�@���W
	DrawStick->SetSize(WINDCAR_STICK_SIZE_X, WINDCAR_STICK_SIZE_Y);										//  �傫��
	DrawStick->SetTexture(TextureManager::GetInstance()->GetTexture(WINDCAR_STICK_TEX_NUM));																		//  �e�N�X�`��
	//���X�g�̒ǉ�
	ObjectManager::GetInstance()->AddObject(pStick);
#pragma endregion
	//----------------------------------------------------------------


#pragma region	---�X�e�[�W
	m_pStageManager->CreateStage(m_eSeasonState);
#pragma endregion


	// �E�B���h�}�l�[�W���[�ɃQ�[�W��ݒ肷��
	m_pWindManager->SetWindGauge(pUI_WinGauge);

	//===== ���[�h�I������ =====
	Load::End();

	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	MenuManager::GetInstance()->SetMenu(STATE_NONE);
	//------------------------------------------------------------------
}

// �I������
void SceneGame::Uninit()
{
	//�G�߂ɂ���ĕς��-----------------
	m_CurrentScene->Uninit();

	//	�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();

	//	�X�e�[�W�̏I������
	StageManager::Destroy();

	//	���j���[�̏I������
	MenuManager::Destroy();

	//	���̏I������
	m_pWindManager->Destroy();
}

// �X�V
void SceneGame::Update()
{
	//	�J�����X�V
	CCamera::Get()->Update();

	//	�X�e�[�W�̍X�V
	m_pStageManager->Update();

	//	���j���[�̍X�V
	m_pMenuManager->Update();

	// �G�߂̍X�V
	m_CurrentScene->Update();

	//2022/1/26/Touch_Tutorial Shimizu Yosuke ---------------------------
	//�����ň�񂾂��`���[�g���A�����o����悤�ɂ���
	if (m_bTutorial) {
		SceneGame::GetInstance()->m_bPauseMode = true;

		//�`���[�g���A���ɍs��������
		MenuManager::GetInstance()->CreateTutorialMenu();

		//�I�u�W�F�N�g�̍X�V���~�߂�
		ObjectManager::GetInstance()->NoFunction();

		m_bTutorial = false;
	}
	//-------------------------------------------------------------------

	//--- �Q�[�����[�h
	if (m_bEditMode == false)
	{
		if (m_bPauseMode == false)
		{
			// ���̍X�V
			m_pWindManager->Update();

			// �ӂ��Ă�����̍X�V
			g_Leaf.Update();
		}

		// �Q�[���I�u�W�F�N�g�̍X�V
		m_pObjectManager->Update();

	}
	//--- �ҏW���[�h
	else if(m_bEditMode == true)
	{
		// �ҏW�̍X�V
		m_pObjectManager->UpdateEdit();
	}

#ifdef _DEBUG
	//	�ҏW���[�h�̐؂�ւ�
	if (INPUT->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// �؂�ւ�

		CSound::Play(SE_PAUSE);	// ������ǉ�

		//	�؂�ւ��Ɠ����ɃJ�����̒����_�̕ύX
		if (m_bEditMode == true)
		{
			//edit	�v���C���[���璍���X���O��
			CCamera::Get()->SetAxisX(nullptr);

			// ���ׂẴI�u�W�F�N�g�̕`��i�X�V�͖����j
			ObjectManager::GetInstance()->BeginEdit();
		}
		else
		{
			// �I�u�W�F�N�g�̍X�V�̍ĊJ
			ObjectManager::GetInstance()->EndEdit();

			//game	�v���C���[�𒍎��X�ɐݒ�
			CCamera::Get()->SetAxisX(
			&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);
		}
	}
#endif // _DEBUG
	

	//	�|�[�Y��ʂ̐؂�ւ�
	//if (INPUT->GetKeyTrigger(DIK_ESCAPE))
	//{
	//	m_bPauseMode = m_bPauseMode ^ 1;	// �؂�ւ�
	//
	//	// �|�[�Y��ʂ̏ꍇ
	//	if (m_bPauseMode == true)
	//	{
	//		m_pObjectManager->NoFunction();
	//		m_pMenuManager->CreatePauseMenu();	// �|�[�Y��ʐ���
	//	}
	//	else
	//	{
	//		m_pObjectManager->Resume();
	//		m_pMenuManager->DeleteMenu();		// ���j���[�I�u�W�F�N�g����ɂ���
	//	}
	//}
}

// �`��
void SceneGame::Draw()
{
	//	�Q�[���I�u�W�F�N�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);


	//--- �|�[�Y���
	//if (m_bPauseMode)
	//{
	//}
	//	�|�[�Y��ʂ̕`��
	m_pMenuManager->Draw();


#ifdef _DEBUG
	//	debug
	CCamera::Get()->Draw();
	//m_pWindManager->Draw();
	m_pStageManager->Draw();

	using namespace ImGui;
	
	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	// �I�u�W�F�N�g�̐�
	Text("Objects   : %d", m_pObjectManager->GetDrawList().size());
	// �R���C�_�[���X�g
	Text("Colliders : %d", (int)CCollider::GetColliderList().size());
	// ���j���[�̐�
	Text("Menu      : %d", m_pMenuManager->GetMenuList().size());
	// ���[�h�t���[��
	Text("LoadFrame : %d", Load::GetLoadCount());
	// ���Q�[�W
	Text("Wind      : %d", WindManager::GetInstance()->GetWindGauge());

	End();
#endif // _DEBUG
}

// �G�߂̃V�[���؂�ւ�
void SceneGame::SetSeason(Season NextSeason)
{
	//	��O����
	if (NextSeason >= MAX_SEASON)
	{
		MessageBox(GetMainWnd(), _T("�G�ߐ؂�ւ����s"), NULL, MB_OK);
		return;
	}

	//	���݂̋G�߂̏I��
	if (m_CurrentScene)
		m_CurrentScene->Uninit();

	//	�G�ߐ؂�ւ�
	m_CurrentScene = m_GameSceneList[NextSeason];	//	���̋G�߂�
	m_eSeasonState = NextSeason;					//	�G�߂̐ݒ�

	StartFadeOutSceneChange(SCENE_GAME);
}

// �G�ߏ��̎擾
Season SceneGame::GetSeason()
{
	return m_eSeasonState;
}

// �C���X�^���X�擾
SceneGame* SceneGame::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneGame();
	}

	return m_pInstance;
}

// �C���X�^���X�j��
void SceneGame::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//��ԉ��ɒǉ�
//2022/1/26/Touch_Tutorial Shimizu Yosuke -----------------------------
void SceneGame::SetTutorial(bool bTutorial) {
	m_bTutorial = bTutorial;
}
//---------------------------------------------------------------------