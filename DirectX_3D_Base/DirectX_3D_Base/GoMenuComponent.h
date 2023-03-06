/**
 * @file	GoMenuComponent.h
 * @brief	���̃R���|�[�l���g�Ń��j���[�ɍs����悤�ɂ���
 * @author	�u���@�z�S
 * @date	2022/1/12
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __GO_MENU_H__
#define __GO_MENU_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;
class ObjectManager;
class MenuManager;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CGoMenu : public Component
{
private:
	bool m_bSelect;						//���j���[�̃{�^���ɐG��Ă��邩�ǂ����̃t���O
	bool m_bMenuFlg;					//���j���[�ɍs�����Ƃ������ǂ����̃t���O
	CDraw2D* m_pDraw2D;					

	ObjectManager*	m_pObjectManager;	// �I�u�W�F�N�g�Ǘ��̃|�C���^
	MenuManager*	m_pMenuManager;		// ���j���[�Ǘ��̃|�C���^


public:
	bool	m_bPauseMode;				// ���j���[���

	CGoMenu();
	~CGoMenu() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Uninit();
	void OnCollisionEnter(Object* pObject) override;
};

#endif // __�@�\��_H__

#pragma endregion