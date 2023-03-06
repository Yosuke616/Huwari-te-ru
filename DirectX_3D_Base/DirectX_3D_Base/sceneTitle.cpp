//===== �C���N���[�h�@=====
#include "sceneTitle.h"
#include "fade.h"

#include "Object.h"
#include "ObjectManager.h"
#include "MenuManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "imgui.h"
#include "Sound.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Draw3dComponent.h"
#include "BGChangeComponent.h"
#include "FadeChangeComponent.h"
#include "MenuComponent.h"
#include "CreateLeaf.h"
#include "SceneSelect.h"
#include "Load.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------------
#include "ResourceCSV.h"
#include "FileManager.h"
#include "sceneGame.h"
//----------------------------------------------------------------------

//===== �}�N����` =====
#define WIDTH_TITLE_LOGO	(720)												// �^�C�g�����S�̉���
#define HEIGHT_TITLE_LOGO	(720)												// �^�C�g�����S�̏c��
#define POS_X_TITLE_LOGO	(-280.0f)											// �^�C�g�����S�� X ���W
#define POS_Y_TITLE_LOGO	(SCREEN_HEIGHT - HEIGHT_TITLE_LOGO)*0.5f + 100.0f	// �^�C�g�����S�� Y ���W

#define BG_CHANGE_TIME		(950)												// �e�N�X�`�����؂�ւ�鎞��
#define FADE_TIME			(BG_CHANGE_TIME - 100)								// �e�N�X�`���p�̃t�F�[�h�|���S�����؂�ւ�鎞��


#define POS_X_TITLE_PLAYER	(-150.0f)
#define POS_Y_TITLE_PLAYER	(-125.0f)


//=== �ÓI�����o�ϐ�
int SceneTitle::m_nTitleSeason;
SceneTitle::Row SceneTitle::m_TutorialVector;	// �`���[�g���A���̃x�N�^�[

CLeaf g_TitleLeaf;
CDrawModel* g_pPlayer = nullptr;

// �R���X�g���N�^
SceneTitle::SceneTitle()
{
	SceneTitle::LoadTurorial();
}

// �f�X�g���N�^
SceneTitle::~SceneTitle()
{
}

// ������
void SceneTitle::Init()
{
	//===== ���[�h��ʂ̃X���b�h����� =====
	Load::Begin();

	// �ŏ��͏t
	m_nTitleSeason = SPRING;

	//	�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();			// �I�u�W�F�N�g�}�l�[�W���[
	m_pMenuManager = MenuManager::GetInstance();				// ���j���[�}�l�[�W���[


	ModelManager *pModelManager = ModelManager::GetInstance();	// ���f���}�l�[�W���[
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_PLAYER, SUMMER_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_PLAYER  , FALL_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_PLAYER, WINTER_PLAYER_NUM);

	/* �e�N�X�`���ǂݍ��� */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// ���j���[�A�C�R��
	pTexManager->AddTexture(PATH_TEX_STOP_GAME, STOP_GAME_TEX_NUM);
	// ���S
	pTexManager->AddTexture(PATH_TEX_TITLE_LOGO, TITLE_LOGO_TEX_NUM);
	// �w�i
	pTexManager->AddTexture(PATH_TEX_TITLE_SPRING, TITLE_SPRING_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_SUMMER, TITLE_SUMMER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_AUTUMN, TITLE_AUTUMN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_WINTER, TITLE_WINTER_TEX_NUM);
	// �r�l�b�g
	pTexManager->AddTexture(PATH_TEX_VINETTO, VINETTO_TEX_NUM);
	// ����
	pTexManager->AddTexture(PATH_TEX_GAMEOVER, GAMEOVER_TEX_NUM);			// �Q�[���I�[�o�[
	pTexManager->AddTexture(PATH_TEX_CONTINUE, CONTINUE_TEX_NUM);			// �Â���
	pTexManager->AddTexture(PATH_TEX_RESTART, RESTART_TEX_NUM);				// ���Ȃ���
	pTexManager->AddTexture(PATH_TEX_EXIT, EXIT_TEX_NUM);					// �����
	pTexManager->AddTexture(PATH_TEX_NEW_GAME, NEW_GAME_TEX_NUM);			// �j���[�Q�[��
	pTexManager->AddTexture(PATH_TEX_OPTION, OPTION_TEX_NUM);				// �I�v�V����
	pTexManager->AddTexture(PATH_TEX_RESUME, RESUME_TEX_NUM);				// ��������
	pTexManager->AddTexture(PATH_TEX_SELECT, SELECT_TEX_NUM);				// �Z���N�g��
	
	// �ӂ��Ă������
	pTexManager->AddTexture(PATH_TEX_SPRING_LEAF, LEAF_SPRING_NUM);			// �t�̂͂���
	pTexManager->AddTexture(PATH_SUMMER_LEAF, SUMMER_LEAF_TEX_NUM);			// �Ă̂͂���
	pTexManager->AddTexture(PATH_FALL_LEAF, FALL_LEAF_TEX_NUM);				// �H�̂͂���
	pTexManager->AddTexture(PATH_TEX_SNOW, SNOW_TEX_NUM);					// �~�̐�

	pTexManager->AddTexture(PATH_TEX_SPIDER, SPIDER_TEX_NUM);				// �N���̑�
	// �I�v�V�����֘A
	pTexManager->AddTexture(PATH_TEX_CONFIG_GAUGE, CONFIG_GAUGE_TEX_NUM);		// ���ʃQ�[�W
	//pTexManager->AddTexture(PATH_TEX_CONFIG_GAUGE_F, CONFIG_GAUGE_F_TEX_NUM);	// ���ʃQ�[�W�g
	pTexManager->AddTexture(PATH_TEX_BGM, BGM_TEX_NUM);							// bgm
	pTexManager->AddTexture(PATH_TEX_SE, SE_TEX_NUM);							// se
	pTexManager->AddTexture(PATH_TEX_BACK, BACK_TEX_NUM);						// ���ǂ�

	//������RESTART������ɕς��Ăق���
	//2022/1/10 Shimizu Yosuke ----------------------------------------
	//�g��ǉ�
	pTexManager->AddTexture(PATH_TEX_CONTINUE_FRAME, CONTINUE_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_RESUME_FRAME, RESUME_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_EXIT_FRAME, EXIT_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_OPTION_FRAME, OPTION_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_NEW_GAME_FRAME, NEW_GAME_FRAME_TEX_NUM);
	//-----------------------------------------------------------------

	pTexManager->AddTexture(PATH_TEX_TITLE, TITLE_TEX_NUM);

	// sawada �����̘g
	pTexManager->AddTexture(PATH_TEX_RESTART_FRAME, RESTART_FRAME_TEX_NUM);					// ���Ȃ���
	pTexManager->AddTexture(PATH_TEX_SELECT_FRAME, SELECT_FRAME_TEX_NUM);					// �Z���N�g��
	pTexManager->AddTexture(PATH_TEX_TITLE_FRAME, TITLE_FRAME_TEX_NUM);						// �^�C�g����

	//2022/1/30/UI_Change Shimizu Yosuke ---------------------------
	pTexManager->AddTexture(PATH_TEX_CLICK, CLICK_TEX_NUM);
	//--------------------------------------------------------------



#pragma region	---�v���C���[	2022/01/21 tomoya
	//	�I�u�W�F�N�g����
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	�R���|�[�l���g�̒ǉ�
	auto TransPlayer = Player->AddComponent<CTransform>();				//	�g�����X�t�H�[��
	auto DrawPlayer = Player->AddComponent<CDrawModel>();					//	���f���`��@�\
	//	�I�u�W�F�N�g���̐ݒ�
	DrawPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));			//	�ҋ@�A������Ă���A������A�E��
	TransPlayer->SetScale(12.0f, 12.0f, 12.0f);									//	�X�P�[��
	TransPlayer->SetVelocity(0.0f, 0.0f, 0.0f);									//	
	TransPlayer->SetPosition(POS_X_TITLE_PLAYER, POS_Y_TITLE_PLAYER, 0.0f);		//	�����z�u
	TransPlayer->SetRotate(0.0f, -45.0f, 0.0f);
	// �A�j���[�V�����ݒ�
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);// �ҋ@
	DrawPlayer->AlwaysAnimation(true);				// ��ɃA�j���[�V����
	DrawPlayer->Play(PLAYER_IDLE);					//�ҋ@���[�V�����Đ�
	// 3D���X�g�ɒǉ�
	m_pObjectManager->AddObject(Player);
#pragma endregion

#pragma region ---�w�i�e
	// �I�u�W�F�N�g����
	Object* objBG_Shadow = new Object("BG_Shadow", UPDATE_BG, DRAW_BG);
	// �R���|�[�l���g�̒ǉ�
	objBG_Shadow->AddComponent<CTransform>();
	auto Draw_BG_Shadow = objBG_Shadow->AddComponent<CDraw2D>();
	// �I�u�W�F�N�g�̐ݒ�
	Draw_BG_Shadow->SetTexture(pTexManager->GetTexture(VINETTO_TEX_NUM));
	Draw_BG_Shadow->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// ���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBG_Shadow);
#pragma endregion

#pragma region ---�w�i
	//	�I�u�W�F�N�g����
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();									// ���W
	auto Draw_BG = objBG->AddComponent<CDraw2D>();						// �`��
	auto Change_BG = objBG->AddComponent<CBgChange>();					// �w�i�ؑ�
	//	�I�u�W�F�N�g�̐ݒ�
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_SPRING_TEX_NUM));	// �e�N�X�`��
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);						// �T�C�Y
	Change_BG->SetTimer(BG_CHANGE_TIME);								// ���Ԑݒ�
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBG);									// �I�u�W�F�N�g�̒ǉ�
#pragma endregion

#pragma region ---�^�C�g�����S
	//	�I�u�W�F�N�g����
	Object* objTitleLogo = new Object("titlelogo", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto TransTitleLogo = objTitleLogo->AddComponent<CTransform>();			// ���W
	auto pTitleDraw = objTitleLogo->AddComponent<CDraw2D>();				// �`��
	//	�I�u�W�F�N�g�̐ݒ�
	pTitleDraw->SetTexture(pTexManager->GetTexture(TITLE_LOGO_TEX_NUM));	// �e�N�X�`���̐ݒ�
	pTitleDraw->SetSize(WIDTH_TITLE_LOGO, HEIGHT_TITLE_LOGO);				// �T�C�Y�̐ݒ�
	TransTitleLogo->SetPosition(POS_X_TITLE_LOGO, POS_Y_TITLE_LOGO);		// ���W�̐ݒ�
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objTitleLogo);								// �I�u�W�F�N�g�̒ǉ�
#pragma endregion

#pragma region ---�t�F�[�h�C���A�E�g�I�u�W�F�N�g
//	�I�u�W�F�N�g����
	Object* objFade = new Object("fade", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto TransFade = objFade->AddComponent<CTransform>();			// ���W
	auto DrawFade = objFade->AddComponent<CDraw2D>();				// �`��
	auto Fade = objFade->AddComponent<CFadeChange>();				// �t�F�[�h����
	//	�I�u�W�F�N�g�̐ݒ�
	DrawFade->SetTexture(nullptr);									// �e�N�X�`���̐ݒ�
	DrawFade->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);					// �T�C�Y�̐ݒ�
	DrawFade->SetColor(1.0f, 1.0f, 1.0f);							// �F
	Fade->SetTimer(FADE_TIME);										// �t�F�[�h����
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objFade);							// �I�u�W�F�N�g�̒ǉ�
#pragma endregion

	//	���j���[����	
	m_pMenuManager->CreateTitleMenu();

	//	�v���C���[�`����̊i�[
	g_pPlayer = DrawPlayer;

	//	BGM�Đ�
	CSound::Play(BGM_TITLE);

	//===== ���[�h�I������ =====
	Load::End();
}

// �I������
void SceneTitle::Uninit()
{
	//	sound stop
	CSound::Stop(BGM_TITLE);
	

	//	���j���[�̏I������
	MenuManager::Destroy();

	// �I������
	m_pObjectManager->Uninit();
}

// �X�V
void SceneTitle::Update()
{
	ModelManager *pModelManager = ModelManager::GetInstance();	// ���f���}�l�[�W���[
	switch (m_nTitleSeason)
	{
	case SPRING: g_pPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));	break;
	case SUMMER: g_pPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));	break;
	case AUTUMN: g_pPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));		break;
	case WINTER: g_pPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));	break;
	default:	break;
	}

	// �ӂ��Ă�����̍X�V
	g_TitleLeaf.UpdateTitleScene();

	//	�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();

	//	���j���[��ʂ̍X�V
	m_pMenuManager->Update();
}

// �`��
void SceneTitle::Draw()
{
	//	�I�u�W�F�N�g���X�g�̕`��
	m_pObjectManager->Draw();

	//	�|�[�Y��ʂ̕`��
	m_pMenuManager->Draw();

#ifdef _DEBUG
	//	�f�o�b�O�p
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	Text("Objects : %d", m_pObjectManager->GetDrawList().size());
	Text("Menu    : %d", m_pMenuManager->GetMenuList().size());
	Text("GridSize : %d", SceneSelect::GetSelectGrid().size());
	End();
#endif // _DEBUG
}

//	�G�ߐݒ�
void SceneTitle::SetSeason(int season)
{
	m_nTitleSeason = season;

	if (m_nTitleSeason < 0) m_nTitleSeason = 0;
	if (m_nTitleSeason > 3) m_nTitleSeason = 3;
}



//2022/1/28/Tutorial_CSV Shimizu Yosuke -----------------------
//LoadTutorial
bool SceneTitle::LoadTurorial() {
	// load
	if (FileManager::GetInstance()->Load(TUTORIAL_DATA_CSV) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	//�z��ɂق��񂷂�@
	//�s�̐��̐ݒ�
	m_TutorialVector.resize(CSV->GetRowSize());
	for (int y = 0; y < CSV->GetRowSize(); ++y) {
		//��̐��̐ݒ�
		m_TutorialVector[y].resize(CSV->GetColumnSize(y));
		for (int x = 0; x < CSV->GetColumnSize(y); ++x) {
			//�v�f���i�[
			m_TutorialVector[y][x] = CSV->GetInt(x, y);
		}
	}

	return true;

}

//SaveTutorial 
bool SceneTitle::SaveTutorial() {
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);
	if (CSV == nullptr) {
		return false;
	}

	//�Q�[����ʓ��������珑��������悤�ɂ���
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		switch (SceneGame::GetInstance()->GetSeason()) {
		case SPRING_1:	CSV->SetInt(0, 0, 1);	break;
		case SUMMER_1:	CSV->SetInt(1, 0, 1);	break;
		case FALL_1:	CSV->SetInt(2, 0, 1);	break;
		case WINTER_1:	CSV->SetInt(3, 0, 1);	break;
		default:break;
		}
	}
	else if(SceneManager::Instance()->GetScene() == SCENE_TITLE)
	{
		for (int nRow = 0; nRow < m_TutorialVector.size(); nRow++)
		{
			for (int nColumn = 0; nColumn < m_TutorialVector[nRow].size(); nColumn++)
			{
				CSV->SetInt(nColumn, nRow, 0);
			}
		}
	}

	
	//save
	if (CSV->Save(TUTORIAL_DATA_CSV) == false) {
		//�G���[���N������
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

//NewGame
bool SceneTitle::NewGame() {
	//��������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	for (int nRow = 0; nRow < m_TutorialVector.size(); nRow++)
	{
		for (int nColumn = 0; nColumn < m_TutorialVector[nRow].size(); nColumn++)
		{
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	SceneTitle::SaveTutorial();

	return true;
}

//FirstPlay
bool SceneTitle::FirstPlay(int nSeason)
{
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	// �O�̂���
	if (CSV == nullptr) return false;

	//�����ɒl��ۑ�����1��������true�A0��������false
	//int nTutorial_Num;

	// ���񂩂ǂ���
	bool bFirst = false;

	//�Z�����擾����
	switch (nSeason)
	{
		// �l�G�̍ŏ��̃X�e�[�W
	case SPRING_1:	if(CSV->GetInt(0, 0) == 0) bFirst = true; break;	// ���񂾂����ꍇ�Z���̒��g��0�Ȃ̂�
	case SUMMER_1:	if(CSV->GetInt(1, 0) == 0) bFirst = true; break;
	case FALL_1:	if(CSV->GetInt(2, 0) == 0) bFirst = true; break;
	case WINTER_1:	if(CSV->GetInt(3, 0) == 0) bFirst = true; break;

	case SPRING_2:	case SUMMER_2:	case FALL_2:	case WINTER_2: bFirst = false; break;

		// ����ȊO
	default:	bFirst = false;
	}

	return bFirst;

	//if (nTutorial_Num) {
	//	return true;
	//}
	//return false;
}

//-------------------------------------------------------------