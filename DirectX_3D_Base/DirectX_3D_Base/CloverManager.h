/**
 * @file	CloverManager.h
 * @brief	���Ԍo�߂ŃN���[�o�[�𐶐����Ă���
 * @author	���c���l
 * @date	2021/01/24
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __CLOVER_MANAGER_H__
#define __CLOVER_MANAGER_H__

//===== �C���N���[�h =====
#include "main.h"


//===== �N���X��` =====
class CloverManager
{
	//=== �����o�ϐ�
private:
	static CloverManager* m_pInstance;
	int m_nFrameCnt;	//	�o���b�������߂�J�E���g
	void CreateClover();//	�t���ς̍쐬


	//=== �����o�֐�
private:
	CloverManager();	//	�R���X�g���N�^
public:
	~CloverManager();	//	�f�X�g���N�^
	void Update();		//	�Q�[���X�V

	static CloverManager* GetInstance();
	static void Destroy();

	// 2022/1/29 narita �N���[�o�[�̐��\��----------------------------------------------
	int GetCloverNum();
	//-----------------------------------------------------------------------------
};
#endif // __CLOVER_MANAGER_H__
