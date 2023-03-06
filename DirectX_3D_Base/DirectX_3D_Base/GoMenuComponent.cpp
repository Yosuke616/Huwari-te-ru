//�C���N���[�h��
#include "GoMenuComponent.h"
#include "ObjectInfo.h"
#include "Draw2dComponent.h"
#include "InputManager.h"
#include "Scene.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "TransformComponent.h"		//	�ʒu�E���x�E�����x


//�R���X�g���N�^
CGoMenu::CGoMenu()
	:m_bSelect(false),m_bPauseMode(false),m_bMenuFlg(false){

}

//�f�X�g���N�^
CGoMenu::~CGoMenu() {

}

//������
void CGoMenu::Start() {
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	m_pObjectManager = ObjectManager::GetInstance();	//�I�u�W�F�N�g�}�l�[�W���[
	m_pMenuManager = MenuManager::GetInstance();		// ���j���[�}�l�[�W���[
}

//�X�V
void CGoMenu::Update() {
	//�|�[�Y�{�^���̐F��ς���
	if (m_bSelect) {
		m_pDraw2D->SetColor(0.5f,0.5f,1.0f);
	}
	else {
		m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
	}
	m_bSelect = false;

	//���j���[�ɍs�����Ƃ������ǂ����̃t���O�������Ă�������ۂɃ��j���[�ɓ���悤�ɂ���
	if (m_bMenuFlg) {

		// �|�[�Y��ʂ̏ꍇ
		if (m_bPauseMode == true)
		{		
				m_pObjectManager->NoFunction();
				m_pMenuManager->CreatePauseMenu();	// �|�[�Y��ʐ���			
		}
		else
		{
			m_pObjectManager->Resume();
			m_pMenuManager->DeleteMenu();		// ���j���[�I�u�W�F�N�g����ɂ���		
		}
	}

	//���j���[�̍X�V
	//m_pMenuManager->Update();

}

//�I������
void CGoMenu::Uninit() {
	//	���j���[�̏I������
	//MenuManager::Destroy();
}

//�`��
void CGoMenu::Draw() {

	//���j���[�̕`��
	//if (m_bPauseMode) {
	//	m_pMenuManager->Draw();
	//}

}

//�����蔻��͊w
void CGoMenu::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == CURSOR_NAME)
	{
		//�C���v�b�g�}�l�[�W���[�̍쐬
		InputManager* pInput = INPUT;
	
		//�F�ς��悤�̂��
		m_bSelect = true;

		//�{�^���������ă|�[�Y
		//�|�[�Y��ʂ̐؂�ւ�
		if (pInput->GetMouseButton(MOUSE_L))
		{
			//�{�^�������ă��j���[�����ꂽ���ǂ����̃t���O�𗧂ĂĂ�����
			//���ۂɃ��j���[��ʂɂȂ邩�ǂ��������߂�
			//Update�̒��ŏ�����������
			m_bMenuFlg = true;		

			//m_bPauseMode = m_bPauseMode ^ 1;	// �؂�ւ�
			m_bPauseMode = true;	// �؂�ւ�

		}
	}
}