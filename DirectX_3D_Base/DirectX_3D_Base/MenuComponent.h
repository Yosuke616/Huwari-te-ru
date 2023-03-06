/**
 * @file	MenuComponent.h
 * @brief	�|�[�Y�̋@�\
 * @author	SAWADA RYOHEI
 * @date	2021/12/21
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __MENUCOMPONENT_H__
#define __MENUCOMPONENT_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;
class CTransform;

//===== �}�N����` =====


//===== �񋓑̐錾 =====

//===== �N���X��` =====
class CMenu : public Component
{
private:
	bool m_bSelected;
	CDraw2D* m_pDraw2D;
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------
	CTransform* m_pTrans;				//��]�����邽�߂̕ϐ�
	int m_nRotate;					//��]������
	//----------------------------------------------------------------



public:
	CMenu();
	~CMenu() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

};

#endif // __MENUCOMPONENT_H__
