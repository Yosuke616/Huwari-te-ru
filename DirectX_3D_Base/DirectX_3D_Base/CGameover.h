/**
 * @file	CGameover.h
 * @brief	�G�߂ɍ����I�u�W�F�N�g���o��
 * @author	�����q��
 * @date	2022/01/19
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __GOLETTER_H__
#define __GOLETTER_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====

//===== �O���錾 =====

//===== �N���X��` =====
class CGoletter
{
private:
	//int cnt;			//�o���b�������߂�J�E���g
public:
	CGoletter();			//	�R���X�g���N�^
	~CGoletter();			//	�f�X�g���N�^
	void Update();		//	�Q�[���X�V
	//void UpdateTitleScene();	// �Z���N�g
	void CreateGoletter();	//	�����̍쐬
};
#endif // __GOLETTERr_H__
