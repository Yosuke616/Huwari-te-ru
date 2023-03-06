//===== �C���N���[�h =====
#include "SceneEnding.h"
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
#include "OffScreenComponent.h"
#include "CloverManager.h"
#include "Load.h"
#include "PointCom.h"
#include "EGravityComponent.h"
#include "SceneSelect.h"
#include "NumberCom.h"
#include "EffectManager.h"
#include "LotationComponent.h"
#include "ResourceCSV.h"
#include "FileManager.h"


#define SCENE_CHANGE_TIMER	(2120)

#define CLEAR_POS_X		(-420.0f)
#define CLEAR_POS_Y		(-50.0f)
#define CLEAR_SIZE_X	(200.0f)
#define CLEAR_SIZE_Y	(170.0f)


bool SceneEnding::m_bSpawnClover[8];

//=== �O���[�o���ϐ�
CGauge* g_pWindGauge =nullptr;
float g_fScroll = 0.0f;

// 2022/1/29 narita �N���[�o�[�̐��\��----------------------------------------------
CDrawNumber* g_pNum = nullptr;
//-----------------------------------------------------------------------------


//=== �ÓI�����o�ϐ�
int	SceneEnding::m_nCurrentFrameCnt;	// �o�ߎ���
int SceneEnding::m_nClovercnt = 0;
bool SceneEnding::m_bFinish;

// �R���X�g���N�^
SceneEnding::SceneEnding()
	: m_pWindManager(nullptr), m_pCloverManager(nullptr)
{
}

// �f�X�g���N�^
SceneEnding::~SceneEnding(){
}

// ������
void SceneEnding::Init()
{
	//===== ���[�h��ʂ̃X���b�h����� =====
	Load::Begin();

	// �J����������
	CCamera::Get()->Init();

	m_nCurrentFrameCnt = 0;
	g_fScroll = 0.0f;

	m_nClovercnt = 0;
	m_bFinish = false;

	m_nSceneChangeTimer = -1;

	//===== �Q�[���I�u�W�F�N�g�̏����� =====

	//	�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();	// �I�u�W�F�N�g�}�l�[�W���[
	m_pWindManager = WindManager::GetInstance();		// ���}�l�[�W���[
	m_pMenuManager = MenuManager::GetInstance();		// ���j���[�}�l�[�W���[
	m_pCloverManager = CloverManager::GetInstance();

	/* �e�N�X�`���ǂݍ��� */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// ��
	pTexManager->AddTexture(PATH_TEX_WIND, WIND_TEX_NUM);
	// �G���h���[��
	pTexManager->AddTexture(PATH_TEX_END_ROLL, END_ROLL_TEX_NUM);
	// �^�C�g����
	pTexManager->AddTexture(PATH_TEX_SUUJI, SUUJI_TEX_NUM);

	pTexManager->AddTexture(PATH_TEX_THANK_FRAME, THANK_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_THANK, THANK_TEX_NUM);

	/* ���f���ǂݍ��� */
	ModelManager* pModelManager = ModelManager::GetInstance();
	// �V���t�B
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SPRING_GOAL, SPRING_GOAL_MODEL_NUM);
	// �l�t�̃N���[�o�[
	pModelManager->AddModel(PATH_MODEL_END_CLOVER, END_CLOVER_MODEL_NUM);

	

	
#pragma region ---�w�i
	//	�I�u�W�F�N�g����
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();				// ���W
	auto Draw_BG = objBG->AddComponent<CDraw2D>();	// �`��
	Draw_BG->SetColor(0.0f, 0.0f, 0.0f);			// �F
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// �T�C�Y
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBG);
#pragma endregion

#pragma region	---�v���C���[
	//	�I�u�W�F�N�g����
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	�R���|�[�l���g�̒ǉ�
	auto TransPlayer = Player->AddComponent<CTransform>();			//	�g�����X�t�H�[��
	auto DrawPlayer = Player->AddComponent<CDrawModel>();				//	���f���`��@�\
	auto DrawClear = Player->AddComponent<CDrawModel>();				//	���f���`��@�\(�N���A�A�j���[�V����)
	auto animationPlayer = Player->AddComponent<CAnimation>();		//	fbx�؂�ւ�
	// 3D���X�g�ɒǉ�
	m_pObjectManager->AddObject(Player);
	TransPlayer->SetRotate(0.0f, -20.0f, 0.0f);
	DrawClear->SetModel(pModelManager->GetModel(SPRING_GOAL_MODEL_NUM));//	�N���A
	TransPlayer->SetScale(5.0f, 5.0f, 5.0f);							//	�X�P�[��
	TransPlayer->SetPosition(-170.0f, -100.0f, PLAYER_POS_Z);	//	�����z�u
	DrawClear->AddAnimation(PLAYER_GOAL, PLAYER_ANIM_GOAL_S, PLAYER_ANIM_GOAL_E, false);
	// fbx�؂�ւ��ݒ�
	animationPlayer->AddAnimation(DrawPlayer, FBX_ONE);		//	�ҋ@�A������Ă���A������A�E��
	animationPlayer->AddAnimation(DrawClear, FBX_TWO);		//	�S�[��
	animationPlayer->ChangeAnimation(FBX_ONE);				// �ŏ���1�ڂ�fbx
	DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
	// �A�j���[�V�����ݒ�
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);			// �ҋ@
	DrawPlayer->AddAnimation(PLAYER_CARRIED, PLAYER_ANIM_CARRIED_S, PLAYER_ANIM_CARRIED_E, true);	// ���ɐ�����Ă���
	DrawPlayer->AddAnimation(PLAYER_FALL, PLAYER_ANIM_FALL_S, PLAYER_ANIM_FALL_E, true);			// ������
	DrawPlayer->AddAnimation(PLAYER_GET, PLAYER_ANIM_GET_S, PLAYER_ANIM_GET_E, false);				// �E��
	DrawPlayer->AddAnimation(PLAYER_BIND, PLAYER_ANIM_BIND_S, PLAYER_ANIM_BIND_E, true);			// �N���̑�
	DrawPlayer->AddAnimation(PLAYER_GAMEOVER, PLAYER_ANIM_GAMEOVER_S, PLAYER_ANIM_GAMEOVER_E, false);// �Q�[���I�[�o�[
	DrawPlayer->SetPlaySpeed(PLAYER_BIND, 0.012);	// �N���̑��̍Đ����x��ݒ�
	DrawPlayer->AlwaysAnimation(true);				// ��ɃA�j���[�V����
	DrawPlayer->Play(PLAYER_IDLE);					// �Đ�
#pragma endregion


#pragma region ---���Q�[�W
	//�@���Q�[�W
	Object* pWindBar = new Object("Gauge", UPDATE_UI, DRAW_UI);
	auto pUI_WinGauge = pWindBar->AddComponent<CGauge>();					//	���Q�[�W
	//�@�I�u�W�F�N�g�̐ݒ�
	pUI_WinGauge->SetMaxGauge(WIMD_GAUGE_MAX);								//	�ő�Q�[�W��
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(pWindBar);
#pragma endregion

#pragma region ---�X�^�b�t���[��
	Object* pEndRoll = new Object("endroll", UPDATE_UI, DRAW_UI);
	// components
	pEndRoll->AddComponent<CTransform>();
	auto drawEndRoll = pEndRoll->AddComponent<CDraw2D>();	// �X�N���[���`��
	auto Scroll = pEndRoll->AddComponent<CTexScroll>();		// �X�N���[����
	// settings
	drawEndRoll->SetTexture(pTexManager->GetTexture(END_ROLL_TEX_NUM)); // �e�N�X�`��
	drawEndRoll->SetSize(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT);	// �T�C�Y
	drawEndRoll->SetUVsize(1.0f, 1.0f / 5);
	drawEndRoll->SetAlpha(0.8f);								// �A���t�@
	Scroll->SetAxizY(&g_fScroll);
	// add to list
	m_pObjectManager->AddObject(pEndRoll);
#pragma endregion


	// 2022/1/29 narita �N���[�o�[�̐��\��----------------------------------------------
#pragma region ---����
	Object* pNum = new Object(CLOVER_NUMBER_NAME, UPDATE_UI, DRAW_UI);
	// components
	//	�R���|�[�l���g�̒ǉ�
	auto transNum = pNum->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto Number = pNum->AddComponent<CDrawNumber>();
	// settings
	transNum->SetPosition(-420.0f, 280.0f);
	Number->SetSize(120.0f, 120.0f);
	Number->SetTexture(pTexManager->GetTexture(SUUJI_TEX_NUM));
	Number->SetNum(0);
	// add to list
	m_pObjectManager->AddObject(pNum);
#pragma endregion
	//-----------------------------------------------------------------------------


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
	DrawCursor->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));		//	�e�N�X�`��
	DrawCursor->SetColor(1.0f,1.0f, 1.0f);						//	�F
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	���W
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	�T�C�Y
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	�����蔻��̑傫��
	homingCursor->SetEffectNum(CURSOR_EFFECT_SPRING_NUM);
	// ���j���[���X�g�ɒǉ�
	m_pObjectManager->AddObject(pCursor);
#pragma endregion


	// �����_�͒���
	CCamera::Get()->SetAxisX(nullptr);
	CCamera::Get()->SetLimit(XMFLOAT2(0, 0));
	CTexScroll::Scroll(true);

	// ���Q�[�W�Z�b�g
	WindManager::GetInstance()->SetWindGauge(pUI_WinGauge);
	g_pWindGauge = pUI_WinGauge;

	// ���𔭐��������悤�ɂ���
	m_pWindManager->SetSpawnWind(true);

	// �G���f�B���OBGM
	CSound::Play(BGM_ENDING);


	/*--------- �N���[�o�[�擾�󋵂ɉ����ďo������N���[�o�[�̐���ς��� ----------*/
	SceneSelect::Column CloverInfo;			
	CloverInfo = SceneSelect::GetClovers();	// �N���[�o�[�̎擾��
	
	for (int i = 0; i < 8; i++)
	{
		m_bSpawnClover[i] = false;
	}

	// 4�S�Ď擾���Ă�����N���[�o�[���o��
	if (CloverInfo[0]  == 1 && CloverInfo[1]  == 1 && CloverInfo[2]  == 1 && CloverInfo[3]  == 1) m_bSpawnClover[0] = true;
	if (CloverInfo[4]  == 1 && CloverInfo[5]  == 1 && CloverInfo[6]  == 1 && CloverInfo[7]  == 1) m_bSpawnClover[1] = true;
	if (CloverInfo[8]  == 1 && CloverInfo[9]  == 1 && CloverInfo[10] == 1 && CloverInfo[11] == 1) m_bSpawnClover[2] = true;
	if (CloverInfo[12] == 1 && CloverInfo[13] == 1 && CloverInfo[14] == 1 && CloverInfo[15] == 1) m_bSpawnClover[3] = true;
	if (CloverInfo[16] == 1 && CloverInfo[17] == 1 && CloverInfo[18] == 1 && CloverInfo[19] == 1) m_bSpawnClover[4] = true;
	if (CloverInfo[20] == 1 && CloverInfo[21] == 1 && CloverInfo[22] == 1 && CloverInfo[23] == 1) m_bSpawnClover[5] = true;
	if (CloverInfo[24] == 1 && CloverInfo[25] == 1 && CloverInfo[26] == 1 && CloverInfo[27] == 1) m_bSpawnClover[6] = true;
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	//CSV->GetInt(
	if (CSV->GetInt(28, 0)== 1 && CSV->GetInt(29, 0) == 1 && CSV->GetInt(30, 0) == 1 && CSV->GetInt(31, 0) == 1) m_bSpawnClover[7] = true;



	/*----------------------------------------------------------------*/
	
	

	//===== ���[�h�I������ =====
	Load::End();
}

// �I������
void SceneEnding::Uninit()
{
	g_pNum = nullptr;

	CSound::Stop(BGM_ENDING);

	//	�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();

	//	���j���[�̏I������
	m_pMenuManager->DeleteMenu();
	MenuManager::Destroy();

	//	���̏I������
	m_pWindManager->Destroy();

	// �N���[�o�[�}�l�[�W���[�I������
	m_pCloverManager->Destroy();
}

// �X�V
void SceneEnding::Update()
{
	// 2022/1/29 narita �N���[�o�[�̐��\��----------------------------------------------
	if (g_pNum == nullptr)
	{
		g_pNum = m_pObjectManager->GetGameObject(CLOVER_NUMBER_NAME)->GetComponent<CDrawNumber>();
	}
	else
	{
		g_pNum->SetNum(m_nClovercnt);
	}
	//-----------------------------------------------------------------------------

	// �X�N���[���ʉ��Z
	g_fScroll = m_nCurrentFrameCnt / (float)ENDING_FRAME;

	// ���Q�[�W����ɍő�ɂ���
	CGauge::SetGauge(g_pWindGauge, 10000);

	//	�J�����X�V
	CCamera::Get()->Update();

	//	���j���[�̍X�V
	m_pMenuManager->Update();

	if (!m_bFinish)
	{
		// ���̍X�V
		m_pWindManager->Update();

		// �N���[�o�[�}�l�[�W���[�̍X�V
		m_pCloverManager->Update();


		// �J�E���g
		m_nCurrentFrameCnt++;
	}

	// �Q�[���I�u�W�F�N�g�̍X�V
	m_pObjectManager->Update();

	// �I���m�F
	if (m_nCurrentFrameCnt > ENDING_FRAME)
	{
		// �J�E���^���Z�b�g
		m_nCurrentFrameCnt = 0;

		// �V�[���؂�ւ��^�C�}�[�n��
		m_nSceneChangeTimer = SCENE_CHANGE_TIMER;

		// �I���
		m_bFinish = true;

		// ���j���[������
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
		m_pObjectManager->AddObject(pTitle_Frame);
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
		m_pObjectManager->AddObject(pTitle);
#pragma endregion

#pragma region ---�Q�[���N���A���߂łƂ�
		Object* pMessage = new Object("", UPDATE_UI, DRAW_UI);
		// components
		auto trans = pMessage->AddComponent<CTransform>();	// �I�u�W�F�N�g�̍��W
		auto draw  = pMessage->AddComponent<CDraw2D>();		// �X�N���[���`��
		// settings
		trans->SetPosition(CLEAR_POS_X, CLEAR_POS_Y);		// ���W
		draw->SetTexture(TextureManager::GetInstance()->GetTexture(THANK_TEX_NUM));// �e�N�X�`��
		draw->SetSize(CLEAR_SIZE_X, CLEAR_SIZE_Y);			// �傫��
		// add to list
		m_pObjectManager->AddObject(pMessage);
#pragma endregion

#pragma region ---�Q�[���N���A���߂łƂ�(�g)
		Object* pMessageFrame = new Object("", UPDATE_UI, DRAW_UI_FRAME);
		// components
		auto transFrame = pMessageFrame->AddComponent<CTransform>();	// �I�u�W�F�N�g�̍��W
		auto drawFrame = pMessageFrame->AddComponent<CDraw2D>();		// �X�N���[���`��
		// settings
		transFrame->SetPosition(CLEAR_POS_X - 5.0f, CLEAR_POS_Y - 8.0f);		// ���W
		drawFrame->SetTexture(TextureManager::GetInstance()->GetTexture(THANK_FRAME_TEX_NUM));// �e�N�X�`��
		drawFrame->SetSize(CLEAR_SIZE_X + 140.0f, CLEAR_SIZE_Y + 115.0f);			// �傫��
		// add to list
		m_pObjectManager->AddObject(pMessageFrame);
#pragma endregion

		// �N���[�o�[�̏d�͂��Ȃ���
		std::list<Object*> Clovers;
		m_pObjectManager->GetGameObjects(ENDING_CLOVER_NAME, Clovers);
		float PosX = -80.0f;
		float PosY = 50.0f;
		int CloverCnt = 0;

		for (auto&& clover : Clovers)
		{
			CloverCnt++;

			// �d�͂̃R���|�[�l���g������
			auto gravity = clover->GetComponent<CEGravity>();
			if (gravity) gravity->Delete();

			// �ړI���W�Ɍ����킹��
			auto dest = clover->AddComponent<CPoint>();
			dest->SetDestPos(XMFLOAT2(PosX, PosY));
			dest->Start();

			PosX += 60.0f;

			// 4�ڈȍ~�͉��̒i
			if (CloverCnt == 4)
			{
				PosX = -80.0f;
				PosY -= 75.0f;
			}
		}

		// se �~�j�Q�[���I��
		CSound::Play(SE_FINISH);

	}

	if (m_nSceneChangeTimer > 0)
	{
		m_nSceneChangeTimer--;

		if (m_nSceneChangeTimer <= 0)
		{
			StartFadeOutSceneChange(SCENE_TITLE);
		}
	}
}

// �`��
void SceneEnding::Draw()
{
	//	�Q�[���I�u�W�F�N�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

	// ���j���[�I�u�W�F�N�g�̕`��
	m_pMenuManager->Draw();

}

// �o�ߎ��Ԃ̎擾
int SceneEnding::GetCurrentFrameCnt()
{
	return m_nCurrentFrameCnt;
}

void SceneEnding::Cntplus()
{
	m_nClovercnt++;
}

void SceneEnding::Cntminus()
{
	m_nClovercnt--;
}

// 2022/1/30 �~�j�Q�[���I���t���O-------------------------
// true�������ꍇ�̓~�j�Q�[�����I������
bool SceneEnding::Finish()
{
	return m_bFinish;
}
//---------------------------------------------------