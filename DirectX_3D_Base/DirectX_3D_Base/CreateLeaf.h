/**
 * @file	CreateLeaf.h
 * @brief	�G�߂ɍ����I�u�W�F�N�g���o��
 * @author	�����q��
 * @date	2021/12/07
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __LEAF_H__
#define __LEAF_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====
#define MAXLEAF		(10)

//===== �O���錾 =====

//�񋓑��錾
//2022/1/7 Shimizu Yosuke ----------------------------------------
enum Season_Leaf {

	SPRING_LEAF,		//�t�̗t����
	SUMMER_LEAF,		//�Ă̗t����
	AUTUMN_LEAF,		//�H�̗t����
	WINTER_LEAF,		//�~�̕���

	MAX_SEASON_LEAF
};
//----------------------------------------------------------------

//===== �N���X��` =====
class CLeaf
{
private:
	int cnt;			//�o���b�������߂�J�E���g
public:
	CLeaf();			//	�R���X�g���N�^
	~CLeaf();			//	�f�X�g���N�^
	void Update();		//	�Q�[���X�V
	void UpdateTitleScene();	// �^�C�g����
	void UpdateSelectScene();	// �Z���N�g��
	void CreateLeaf();	//	�t���ς̍쐬
};
#endif // __LEAF_H__
