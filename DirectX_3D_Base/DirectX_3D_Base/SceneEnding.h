/**
 * @file	SceneEnding.h
 * @brief	�G���f�B���O
 * @author	AT12A ���c���l
 * @date	2021/01/24		�쐬
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_ENDING_H__
#define __SCENE_ENDING_H__

//===== �C���N���[�h =====
#include "Scene.h"


//===== �O���錾 =====
class WindManager;
class CloverManager;


#define ENDING_FRAME	(50 * 60)	// �G���f�B���O�̎��ԁi�b���~60fps

//===== �N���X��` =====
class SceneEnding : public Scene
{
private:
	WindManager*	m_pWindManager;		// ���}�l�[�W���[
	CloverManager*	m_pCloverManager;	// �N���[�o�[�Ǘ��N���X
	static int		m_nCurrentFrameCnt;	// �o�ߎ���

	// 2022/1/30 �~�j�Q�[���I���t���O-------------------------
	static bool m_bFinish;
	//---------------------------------------------------

	int m_nSceneChangeTimer;	// �V�[���؂�ւ��^�C�}�[

	// static�����o�ϐ��ɕύX
	// ��������݂��Ȃ��ϐ��̂���
	static int		m_nClovercnt;		// ���_

public:
	SceneEnding();						//	�R���X�g���N�^
	~SceneEnding() override;				//	�f�X�g���N�^
	void Init() override;				//	������
	void Uninit() override;				//	�I������
	void Update() override;				//	�X�V
	void Draw() override;				//	�`��

	// static�����o�֐��ɕύX
	// static�����o�ϐ��������邽��
	static void Cntplus();				//	�J�E���g�A�b�v
	static void Cntminus();				//	�J�E���g�}�C�i�X

	static int GetCurrentFrameCnt();	// �o�ߎ��Ԃ̎擾

	// 2022/1/30 �~�j�Q�[���I���t���O-------------------------
	static bool Finish();
	//---------------------------------------------------

	static bool m_bSpawnClover[8];
};
#endif // __SCENE_ENDING_H__
