#pragma once
/**
 * @file	FallenLeavesComponent.h
 * @brief	�����t�̃R���|�[�l���g
 * @author	AT12B �g�c����
 * @date	2021/12/21
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __FALLENLEAVES_H__
#define __FALLENLEAVES_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class InputManager;
class CTransform;
class Object;


//===== �N���X��` =====
class CFallenLeaves : public Component
{
private:
	CTransform* m_pTransform;	//	�g�����X�t�H�[��
	InputManager* m_pInput;		//	���͏��
	bool m_bClover;				//	�N���[�o�[������t���O

public:
	CFallenLeaves();								    //	�R���X�g���N�^
	~CFallenLeaves() override;							//	�f�X�g���N�^
	void Start() override;								//	������
	void Update() override;								//	�X�V
	void Draw() override;								//	�`��
	void OnCollisionEnter(Object*) override;
	
	void Clover();

	int m_nCloverNumber;
	void SetNumber(int num);
	int GetNumber();
};
#endif // __COMPONENT_H__