#include "CloverManager.h"

#include "SceneEnding.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Draw3dComponent.h"
#include "Draw2dComponent.h"
#include "DrawEffectComponent.h"

#include "fallleafComponent.h"
#include "CloverComponent.h"
#include "HomingComponent.h"

#include "Camera.h"

#include "ObjectManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "OffScreenComponent.h"
#include "NumberCom.h"
#include "EGravityComponent.h"

#include "GravityComponent.h"

//===== �}�N����`
#define CREATE_FRAME	(300)

#define CREATE_TIME_1	(500)
#define CREATE_TIME_2	(500 + CREATE_FRAME * 1)
#define CREATE_TIME_3	(500 + CREATE_FRAME * 2)
#define CREATE_TIME_4	(500 + CREATE_FRAME * 3)
#define CREATE_TIME_5	(500 + CREATE_FRAME * 4)
#define CREATE_TIME_6	(500 + CREATE_FRAME * 5)
#define CREATE_TIME_7	(500 + CREATE_FRAME * 6)
#define CREATE_TIME_8	(500 + CREATE_FRAME * 7)

//=== �ÓI�����o�ϐ�
CloverManager* CloverManager::m_pInstance = nullptr;

//--- �V���O���g��------------------------------------

// �C���X�^���X�̐������擾
CloverManager* CloverManager::GetInstance()
{
	// �C���X�^���X������Ƃ��Ȃ��ꍇ�͍��
	if (!m_pInstance)
	{
		m_pInstance = new CloverManager();
	}

	return m_pInstance;
}

// �C���X�^���X�j��
void CloverManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//-------------------------------------------------


// �R���X�g���N�^
CloverManager::CloverManager()
	:m_nFrameCnt(0)
{

}
// �f�X�g���N�^
CloverManager::~CloverManager()
{

}

// �X�V
void CloverManager::Update()
{
	m_nFrameCnt++;

	
	if ((m_nFrameCnt == CREATE_TIME_1 && SceneEnding::m_bSpawnClover[0]) ||
		(m_nFrameCnt == CREATE_TIME_2 && SceneEnding::m_bSpawnClover[1]) ||
		(m_nFrameCnt == CREATE_TIME_3 && SceneEnding::m_bSpawnClover[2]) ||
		(m_nFrameCnt == CREATE_TIME_4 && SceneEnding::m_bSpawnClover[3]) ||
		(m_nFrameCnt == CREATE_TIME_5 && SceneEnding::m_bSpawnClover[4]) ||
		(m_nFrameCnt == CREATE_TIME_6 && SceneEnding::m_bSpawnClover[5]) ||
		(m_nFrameCnt == CREATE_TIME_7 && SceneEnding::m_bSpawnClover[6]) ||
		(m_nFrameCnt == CREATE_TIME_8 && SceneEnding::m_bSpawnClover[7]))
	{

		// �N���[�o�[����
		CreateClover();
	}

	//if (m_nFrameCnt > CREATE_FRAME)
	//{
	//
	//	// �J�E���g���Z�b�g
	//	m_nFrameCnt = 0;
	//}
}


// �N���[�o�[����
void CloverManager::CreateClover()
{
	// �I�u�W�F�N�g����
	Object* pClover = new Object(ENDING_CLOVER_NAME, UPDATE_MODEL, DRAW_MODEL);
	// �R���|�[�l���g�̒ǉ�
	auto transClover = pClover->AddComponent<CTransform>();		// �I�u�W�F�N�g���W
	auto drawClover = pClover->AddComponent<CDrawModel>();			// ���f���`��
	auto ColliderClover = pClover->AddComponent<CCollider>();	//�@�Փ˔���
	auto homing = pClover->AddComponent<CHoming>();				// �}�E�X�Ǐ]
	auto Fall = pClover->AddComponent<CEGravity>();
	auto offScreen = pClover->AddComponent<COffScreen>();
	// ���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pClover);
	// �I�u�W�F�N�g�ݒ�
	transClover->SetRotate(0.0f, 0.0f, 0.0f);
	transClover->SetScale(15.0f, 15.0f, 15.0f); 
	drawClover->SetModel(ModelManager::GetInstance()->GetModel(END_CLOVER_MODEL_NUM));	//�@���f��
	ColliderClover->SetCollisionSize(50.0f, 50.0f);					//�@�����蔻��̑傫��
	homing->SetEffectNum(CURSOR_EFFECT_NUM);
	offScreen->SetFinction(ON_SCREEN);


	//@@@ �����͓����ɂ܂����܂�

	// �@���W�A���x�����߂�(���݃e�L�g�[�ɐݒ肵�Ă��܂�



	// �A�G���f�B���O�̌o�ߎ��Ԃɂ���ċ�����ς���
	int nTime = SceneEnding::GetCurrentFrameCnt();
	if (375 > nTime)
	{
		// �����̍��W
		transClover->SetPosition(-70.0f, 200.0f, -10.0f);
	}
	else if (375 <= nTime && nTime < 750)
	{
		// �����̍��W
		transClover->SetPosition(0.0f, 120.0f, -10.0f);			//�N���[�o�[1

	}
	else if (750 <= nTime && nTime < 1125)
	{
		// �����̍��W
		transClover->SetPosition(170.0f, 200.0f, -10.0f);		//�N���[�o�[2

	}
	else if (1125 <= nTime && nTime < 1500)
	{
		// �����̍��W
		transClover->SetPosition(-170.0f, 200.0f, -10.0f);		//�N���[�o�[3

	}
	else if (1500 <= nTime && nTime < 1875)
	{
		// �����̍��W
		transClover->SetPosition(100.0f, 200.0f, -10.0f);		//�N���[�o�[4

	}
	else if (1875 <= nTime && nTime < 2250)
	{
		// �����̍��W
		transClover->SetPosition(-100.0f, 200.0f, -10.0f);		//�N���[�o�[5

	}
	else if (2250 <= nTime && nTime < 2625)
	{
		// �����̍��W
		transClover->SetPosition(-70.0f, 200.0f, -10.0f);		//�N���[�o�[6
	}
	else if (2625 <= nTime && nTime < 3000)
	{
		// �����̍��W
		transClover->SetPosition(70.0f, 200.0f, -10.0f);		//�N���[�o�[7
	}
	else if (3000 <= nTime && nTime < ENDING_FRAME)
	{
		// �����̍��W
		transClover->SetPosition(0.0f, 200.0f, -10.0f);			//�N���[�o�[8
	}
	//���_�v�Z
	SceneEnding::Cntplus();
}

// 2022/1/29 narita �N���[�o�[�̐��\��----------------------------------------------
int CloverManager::GetCloverNum()
{
	std::list<Object*> Clovers;

	ObjectManager::GetInstance()->GetGameObjects(ENDING_CLOVER_NAME, Clovers);

	return (int)Clovers.size();
}
//-----------------------------------------------------------------------------