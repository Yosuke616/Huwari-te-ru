/**
 * @file	SceneSelect.h
 * @brief	�X�e�[�W�Z���N�g�V�[��
 * @author	AT12A �鑺����
 * @date	2021/12/21		�쐬
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_SELECT_H__
#define __SCENE_SELECT_H__

//===== �C���N���[�h =====
#include "Scene.h"

#include <vector>

//===== �񋓑̐錾 =====
enum S_Season
{
	SPRING = 0,
	SUMMER,
	AUTUMN,
	WINTER
};

//===== �萔��` =====
#define STAGE_1_SELECTABLE	(0)
#define STAGE_2_SELECTABLE	(1)
#define STAGE_1_CLOVER		(2)
#define STAGE_2_CLOVER		(3)

#define CLEAR_FLOWER_UI_1_NAME		("ClearFlowerUI1")
#define CLEAR_FLOWER_UI_2_NAME		("ClearFlowerUI2")
#define CLEAR_UI_1_NAME				("ClearUI1")
#define CLEAR_UI_2_NAME				("ClearUI2")

//===== �N���X��` =====
class SceneSelect : public Scene
{
	//=== �����o�ϐ� ===
public:
	// vector��2�����z��
	using Cell = int;					// �Z��
	using Column = std::vector<Cell>;	// �s
	using Row = std::vector<Column>;	// ��
private:
	bool bMV1_PlayONOFF;	//MV�P�Đ�
	bool bMV2_PlayONOFF;	//MV�Q�Đ�

	static bool m_bRelease1;			// �X�e�[�W������o
	static bool m_bRelease2;			// �X�e�[�W������o
	static bool m_bReleaseClover[8];

	static int		m_OldSeason;
	static Row		m_SelectGrid;		// �N���A�󋵂̔z��
	static Column	m_Clover;			// �N���[�o�[�擾�󋵂̔z��

private:
	// ���[�h
	static bool Load();
	// �Z�[�u
	static bool Save();

	// �X�e�[�W������o
	void Clear();

	//=== �����o�֐� ===
public:
	SceneSelect();								// �R���X�g���N�^
	~SceneSelect() override;					// �f�X�g���N�^
	void Init() override;						// ������
	void Uninit() override;						// �I������
	void Update() override;						// �X�V
	void Draw() override;						// �`��

	static int m_nCurrentSeason;				// �I������Ă���G��

	// �Z���N�g�V�[���̋G�ߐ؂�ւ�
	static void ChangeSceason(int n);

	// �I���\��Ԃ̎擾
	static bool GetSelectable(int stage_num);

	// �I���\��Ԃ̔z��擾
	static Row GetSelectGrid();

	// csv�t�@�C��������
	static bool NewGame();

	// �N���A�󋵂̕ۑ�
	static bool SaveClearInfo(int nStage, bool bClover[4]);

	// �N���[�o�[�擾
	static bool SaveCloverInfo(int nStage, int cloverNumber);

	// �N���[�o�[�̎擾�󋵂̎擾
	static Column GetClovers();
};
#endif // __SCENE_SELECT_H__