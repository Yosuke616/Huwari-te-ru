/**
 * @file	TutorialComponent.h
 * @brief	���ɓ����������Ƀ��j���[�ɍs����悤�ɂ���R���|�[�l���g
 * @author	�u���@�z�S
 * @date	2022/1/19
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __TUTORIALCOMPONENT_H__
#define __TUTORIALCOMPONENT_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class WindManager;
//2022/1/31/Load_Tutorial Shimizu Yosuke -----------------------
class CDraw2D;
//--------------------------------------------------------------

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CTutorial : public Component
{
private:
	WindManager* m_pWindManager;		//�}�l�[�W���[���Ăяo���p�̕ϐ�
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------
	CDraw2D* m_pDraw2D;					//�F���Z�b�g���邽�߂̊֐�
	//------------------------------------------------------------------------

public:
	CTutorial();
	~CTutorial() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Uninit();
	void OnCollisionEnter(Object* pObject) override;


};

#endif // __�@�\��_H__

