/**
 * @file	SceneTitle.h
 * @brief	�^�C�g���V�[��
 * @author	AT12A ���c���l
 * @date	2021/08/xx		�쐬
 */


//===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

//===== �C���N���[�h =====
#include "Scene.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ------------------------------
#include "sceneGame.h"
#include <vector>
//---------------------------------------------------------------------



//===== �N���X��` =====
class SceneTitle : public Scene
{
	//2022/1/28/Tutorial_CSV Shimizu Youske --------------------------
public:
	using Cell = int;						//	�Z��
	using Column = std::vector<Cell>;		//	�s
	using Row = std::vector<Column>;		//	��

private:
	static Row m_TutorialVector;	// �`���[�g���A���̃x�N�^�[

	//Tutorial�p��CSV��ǂݍ��ނ��߂̊֐�
	static bool LoadTurorial();
	//----------------------------------------------------------------

public:
	static int m_nTitleSeason;			//	�^�C�g���̋G��
	static void SetSeason(int season);	//	�G�ߐݒ�
	SceneTitle();						//	�R���X�g���N�^
	~SceneTitle() override;				//	�f�X�g���N�^
	void Init() override;				//	������
	void Uninit() override;				//	�I������
	void Update() override;				//	�X�V
	void Draw() override;				//	�`��

	//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------------
	//Tutorial�p��CSV��ǂݍ��ނ��߂̊֐�
	static bool SaveTutorial();
	//CSV�����������邽�߂̂��
	static bool NewGame();

	//�x�N�^�[�̒��g�������Ŏw�肵�āA0��������false�E1��������true��Ԃ�
	//�Z���̒��g�𒲂ׂ�@�G�߂ɂ����
	static bool FirstPlay(int nSeason);
	//-----------------------------------------------------------------
};
#endif // __SCENE_TITLE_H__


