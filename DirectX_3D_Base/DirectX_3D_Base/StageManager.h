/**
 * @file	StageManager.h
 * @brief	�X�e�[�W���Ǘ�����N���X
 * @author	���c ���l
 * @date	2021/12/09 �쐬
 *			2021/12/26 csv�ɂ��}�b�v����
 *					   csv�̏��������ƕۑ�
 *
 *
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __STAGE_MANAGER_H__
#define __STAGE_MANAGER_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Object.h"
#include "Component.h"
#include "ResourceCSV.h"

#include <map>
#include <vector>

//===== �O���錾 =====
class Object;




//===== �N���X��` =====
class StageManager
{
	//=== �����o�ϐ�
public:
	static StageManager* m_pInstance;				//	�C���X�^���X

	// vector��2�����z��
	//using Cell = int;								//	�Z��
	//using Column = std::vector<Cell>;				//	�s
	//using Row = std::vector<Column>;				//	��
	ResourceCSV::IntRow m_StageGrid;								//	�X�e�[�W�z��

	Object* m_pShowObject;							//debug �J�[�\���ɕ\���������
	int m_ElementNumX;							//debug �}�E�X���W�̗v�f��
	int m_ElementNumY;							//debug �}�E�X���W�̗v�f��
	int m_nMap_Sta;									//debug �z�u���镨�̎��

	int m_block_num_x;								//	�X�e�[�W�̗v�f��x
	int m_block_num_y;								//	�X�e�[�W�̗v�f��y
	XMFLOAT2 m_vStageSize;							//	�X�e�[�W�̑傫��
	XMFLOAT2	m_WorldSize;						//	���[���h���W�ŏc�Ɖ���
	XMFLOAT2	m_BasePos;							//	�X�e�[�W�`��̋N�_���W


	//=== �����o�֐�
private:
	StageManager();									//	�R���X�g���N�^
	bool Save(int Season);							//	�X�e�[�W�̏�������
	bool Load(std::string file);					//	�X�e�[�W�̓ǂݍ���

	// �N���[�o�[�ɔԍ�������U��
	void SetCloverNumber(std::list<Object*>& Clovers);

	// �f�o�b�N�X�V
	void UpdateShowObject(float fPosX, float fPosY);
public:
	static StageManager* GetInstance();				//	�C���X�^���X�擾
	static void Destroy();							//	�C���X�^���X�j��
	~StageManager();								//	�f�X�g���N�^
	void Update();									//	�X�V
	void Draw();									//	debug�`��

	//	�n�`����
	void CreateStage(int stage_state);

	// �X�e�[�W�I�u�W�F�N�g����(�Q�[�����[�h
	Object* CreateBlock(float PosX, float PosY, int state, int nBlockID);

	// �X�e�[�W�I�u�W�F�N�g����(�ҏW���[�h
	//void CleateBlock_Deb(float PosX, float PosY, int number, int x, int y, int create_status);

};
#endif // __STAGE_MANAGER_H__
