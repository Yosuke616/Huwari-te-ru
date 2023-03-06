//===== �C���N���[�h =====
#include "sceneFall.h"
#include "fade.h"
#include "imgui.h"

#include "Object.h"
#include "ObjectInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "WindManager.h"
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
#include "InformationComponent.h"			
#include "WindComponent.h"
#include "GravityComponent.h"		//	�d��
#include "FallComponent.h"
#include "HomingComponent.h"
#include "TexScrollComponent.h"
#include "GaugeComponent.h"
#include "FadeComponent.h"
#include "EffectManager.h"
#include "sceneGame.h"
#include "LotationComponent.h"

#include "FlashComponent.h"

// �R���X�g���N�^
SceneFall::SceneFall() {
}

// �f�X�g���N�^
SceneFall::~SceneFall() {
}

// ������
void SceneFall::Init()
{
	//	�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();				//	�I�u�W�F�N�g�}�l�[�W���[

	/* �e�N�X�`���ǂݍ��� */
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_BG_C0, BG_C0_TEX_NUM);	//BG
	pTexManager->AddTexture(PATH_TEX_BG_C1, BG_C1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C2, BG_C2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C3, BG_C3_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C4, BG_C4_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C5, BG_C5_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C6, BG_C6_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_C7, BG_C7_TEX_NUM);

	pTexManager->AddTexture(PATH_TEX_EFFECT_AUTUMN, EFFECT_AUTUMN_TEX_NUM);	// ��(�H)

	/* ���f���ǂݍ��� */
	ModelManager* pModelManager = ModelManager::GetInstance();			// ���f���}�l�[�W���[
	// �V���t�B
	pModelManager->AddModel(PATH_MODEL_FALL_PLAYER, FALL_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_GOAL, FALL_GOAL_MODEL_NUM);	// �S�[����
	// �X�e�[�W�u���b�N
	pModelManager->AddModel(PATH_MODEL_FALL_BLOCK, FALL_BLOCK_MODEL_NUM);
	// �X�e�[�W�u���b�N(���̕�
	pModelManager->AddModel(PATH_MODEL_FALL_BLOCK_LOW, FALL_BLOCK_LOW_MODEL_NUM);
	// �ڂ�
	pModelManager->AddModel(PATH_MODEL_FALL_BUD, FALL_BUD_MODEL_NUM);
	// ��
	pModelManager->AddModel(PATH_MODEL_FALL_FLOWER, FALL_FLOWER_MODEL_NUM);

	/* �G�t�F�N�g(�G�t�F�N�V�A)�ǂݍ��� */
	EffectManager* pEffectManager = EffectManager::GetInstance();
	// �}�E�X�G�t�F�N�g
	pEffectManager->AddEffect(PATH_CURSOR_EFFECT_AUTUMN, CURSOR_EFFECT_AUTUMN_NUM);

	// �v���C���[���̎擾
	Object* pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	// �w�i��2���ׂ�
	for (int i = 0; i < 2; i++)
	{
#pragma region	---�w�i(0
		//	�I�u�W�F�N�g����
		Object* objBG0 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto transBG0 = objBG0->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG0 = objBG0->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG0scroll = objBG0->AddComponent<CTexScroll>();				//	UV�X�N���[��
		auto BG0Flash = objBG0->AddComponent<CFlash>();						//�@���̏���
		//	�I�u�W�F�N�g�̐ݒ�
		transBG0->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG0->SetTexture(pTexManager->GetTexture(BG_C0_TEX_NUM));		//	�e�N�X�`��
		Draw_BG0->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG0scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG0scroll->SetScrollValue(0.0016f);									//	�X�N���[���ʂ̐ݒ�
		BG0Flash->SetAlphaTime(15);											//�@�`�掞��(�����͕b���w��)
		BG0Flash->SetNoneTime(8);											//�@�`�悵�Ȃ�����(�����͕b���w��)
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG0);
#pragma endregion

#pragma region	---�w�i(1
		//	�I�u�W�F�N�g����
		Object* objBG1 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto transBG1 = objBG1->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG1 = objBG1->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG1scroll = objBG1->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		transBG1->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG1->SetTexture(pTexManager->GetTexture(BG_C1_TEX_NUM));		//	�e�N�X�`��
		//Draw_BG1->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG1->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG1scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG1scroll->SetScrollValue(0.0015f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG1);
#pragma endregion

#pragma region	---�w�i(2
		//	�I�u�W�F�N�g����
		Object* objBG2 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto TransBG2 = objBG2->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG2 = objBG2->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG2scroll = objBG2->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		TransBG2->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG2->SetTexture(pTexManager->GetTexture(BG_C2_TEX_NUM));		//	�e�N�X�`��
		//Draw_BG2->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG2->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG2scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG2scroll->SetScrollValue(0.0013f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG2);
#pragma endregion

#pragma region	---�w�i(3
		//	�I�u�W�F�N�g����
		Object* objBG3 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto TransBG3 = objBG3->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG3 = objBG3->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG3scroll = objBG3->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		TransBG3->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG3->SetTexture(pTexManager->GetTexture(BG_C3_TEX_NUM));		//	�e�N�X�`��
		Draw_BG3->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG3->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG3scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG3scroll->SetScrollValue(0.0011f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG3);
#pragma endregion

#pragma region	---�w�i(4
		//	�I�u�W�F�N�g����
		Object* objBG4 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto TransBG4 = objBG4->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG4 = objBG4->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG4scroll = objBG4->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		TransBG4->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG4->SetTexture(pTexManager->GetTexture(BG_C4_TEX_NUM));		//	�e�N�X�`��
		//Draw_BG4->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG4->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG4scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG4scroll->SetScrollValue(0.001f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG4);
#pragma endregion

#pragma region	---�w�i(5
		//	�I�u�W�F�N�g����
		Object* objBG5 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto TransBG5 = objBG5->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG5 = objBG5->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG5scroll = objBG5->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		TransBG5->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG5->SetTexture(pTexManager->GetTexture(BG_C5_TEX_NUM));		//	�e�N�X�`��
		Draw_BG5->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG5->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG5scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG5scroll->SetScrollValue(0.0005f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG5);
#pragma endregion

#pragma region	---�w�i(6
		//	�I�u�W�F�N�g����
		Object* objBG6 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	�R���|�[�l���g�̒ǉ�
		auto TransBG6 = objBG6->AddComponent<CTransform>();					//	�g�����X�t�H�[��
		auto Draw_BG6 = objBG6->AddComponent<CDraw2D>();					//	2D�`��@�\
		auto BG6scroll = objBG6->AddComponent<CTexScroll>();				//	UV�X�N���[��
		//	�I�u�W�F�N�g�̐ݒ�
		TransBG6->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	���W
		Draw_BG6->SetTexture(pTexManager->GetTexture(BG_C6_TEX_NUM));		//	�e�N�X�`��
		Draw_BG6->SetSize(BG_LENGTH, BG_LENGTH);							//	�T�C�Y
		BG6scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
		BG6scroll->SetScrollValue(0.0003f);									//	�X�N���[����
		//	���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG6);
#pragma endregion
	}

#pragma region	---�w�i(7
	//	�I�u�W�F�N�g����
	Object* objBG7 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	auto TransBG7 = objBG7->AddComponent<CTransform>();					//	�g�����X�t�H�[��
	auto Draw_BG7 = objBG7->AddComponent<CDraw2D>();					//	2D�`��@�\
	auto BG7scroll = objBG7->AddComponent<CTexScroll>();				//	UV�X�N���[��
	//	�I�u�W�F�N�g�̐ݒ�
	TransBG7->SetPosition(0.0f, 0.0f);		//	���W
	Draw_BG7->SetTexture(pTexManager->GetTexture(BG_C7_TEX_NUM));		//	�e�N�X�`��
	Draw_BG7->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);							//	�T�C�Y
	BG7scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X���̊���v���C���[�ɂ���
	BG7scroll->SetScrollValue(0.0003f);									//	�X�N���[����
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objBG7);
#pragma endregion

#pragma region ---�G�߃��S
	// �I�u�W�F�N�g����
	Object* pSeasonLogo = new Object("SeasonLogo", UPDATE_UI, DRAW_UI);
	// �R���|�[�l���g�̒ǉ�
	auto TransSLogo = pSeasonLogo->AddComponent<CTransform>();			// ���W
	auto DrawSLogo = pSeasonLogo->AddComponent<CDraw2D>();				// �X�N���[���`��
	pSeasonLogo->AddComponent<CFade>();									// �`��̎��Ԏw��
	// �I�u�W�F�N�g�̐ݒ�
	TransSLogo->SetPosition(SEASON_LOGO_X, SEASON_LOGO_Y);				// ���W�ݒ�
	DrawSLogo->SetSize(SEASON_LOGO_WIDTH, SEASON_LOGO_HEIGHT);			// �傫���ݒ�
	// ���X�g�ɒǉ�
	m_pObjectManager->AddObject(pSeasonLogo);
#pragma endregion

	// �e�N�X�`���ݒ�
	if (SceneGame::GetInstance()->GetSeason() == FALL_1)
	{
		DrawSLogo->SetTexture(pTexManager->GetTexture(AUTUMN_1_TEX_NUM));	// �e�N�X�`���ݒ�
	}
	else if (SceneGame::GetInstance()->GetSeason() == FALL_2)
	{
		DrawSLogo->SetTexture(pTexManager->GetTexture(AUTUMN_2_TEX_NUM));	// �e�N�X�`���ݒ�
	}

#pragma region	---�J�[�\��
	//	�I�u�W�F�N�g����
	Object* objCursor = new Object(GAME_CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	objCursor->AddComponent<CTransform>();					//	�g�����X�t�H�[��
	auto homing = objCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto collider = objCursor->AddComponent<CCollider>();	//	�����蔻��
	auto draw = objCursor->AddComponent<CDraw2D>();			//�@�`��
	objCursor->AddComponent<CLotation>();					//�@��]
	// �I�u�W�F�N�g�̐ݒ�
	draw->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));	// �e�N�X�`��
	draw->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);						// �T�C�Y
	draw->SetColor(0.0f, 1.0f, 0.0f);
	homing->SetEffectNum(CURSOR_EFFECT_AUTUMN_NUM);
	collider->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);
	//	���X�g�ɒǉ�
	m_pObjectManager->AddObject(objCursor);
#pragma endregion

	//--- BGM�Đ�
	CSound::Play(BGM_FALL);
}

// �I������
void SceneFall::Uninit()
{
	//	BGM��~
	CSound::Stop(BGM_FALL);
}

// �X�V
void SceneFall::Update() {
}

// �`��
void SceneFall::Draw() {
}
