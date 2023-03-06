//�C���N���[�h��
#include "TutorialComponent.h"
#include "ColliderComponent.h"
#include "MenuComponent.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "MenuComponent.h"
#include "Object.h"
#include "sceneGame.h"
#include "WindManager.h"
#include "InputManager.h"

//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------
#include "SceneManager.h"
#include "Draw2dComponent.h"
//------------------------------------------------------------------------

//�R���X�g���N�^
CTutorial::CTutorial() {

}

//�f�X�g���N�^
CTutorial::~CTutorial() {

}

//������
void CTutorial::Start() {
	//���̃}�l�[�W���[���Ăяo��
	m_pWindManager = WindManager::GetInstance();

	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	//------------------------------------------------------------
}

//�I��
void CTutorial::Uninit() {
	//���̃}�l�[�W���[��j�󂷂�
	m_pWindManager->Destroy();
}

//�X�V
void CTutorial::Update() {
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------------
	if (Parent->GetName() == LOAD_TUTORIAL_NAME) {
		if (MenuManager::GetInstance()->GetMenu() == STATE_SELECT) {
			m_pDraw2D->SetAlpha(1.0f);
		}
		else {
			m_pDraw2D->SetAlpha(0.0f);
		}
	}
	//------------------------------------------------------------------------------
}

//�`��
void CTutorial::Draw() {

}

//�Փ˔���
void CTutorial::OnCollisionEnter(Object* pObject) {
	//OnCollisionEnter�̒��g�S����
//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------------------------------------
	if (pObject->GetName() == CURSOR_NAME) {

		//�C���v�b�g�}�l�[�W���[�̃C���X�^���X���Q�b�g����
		InputManager* Input = InputManager::Instance();

		if (Input->GetMouseTrigger(MOUSE_L)) {
			//���j���[�ɓ���邩�ǂ����̃t���O�������Ă����炱���ɓ����
			if (!m_pWindManager->GetTutorial()) {

				//�Z�b�^�[�Ń��j���[�ɓ���邩�̃t���O���I���ɂ��ē���Ȃ��悤�ɂ���
				m_pWindManager->SetTutorial(true);

				//�e���`���[�g���A�����Ԃ��������肪�����������ɓ���
				//���̒��ɂ͓���
				//���j���[�}�l�[�W���[

				//�Q�[����ʂ��~�߂�
				SceneGame::GetInstance()->m_bPauseMode = true;

				//�`���[�g���A���ɍs��������
				MenuManager::GetInstance()->CreateTutorialMenu();

				//�I�u�W�F�N�g�̍X�V���~�߂�
				ObjectManager::GetInstance()->NoFunction();

			}
		}
	}
	//----------------------------------------------------------------------------------------

}





