//===== �C���N���[�h�K�[�h =====
#pragma once


//===== �C���N���[�h =====
#include <map>
#include "ResourceBase.h"


//===== �}�N����` =====
// �N���A��
#define SAVE_DATA_CSV			("Asserts/save_data.csv")
#define COMPLETE				(16)						// �R���v���[�g���Ă邩�̏�񂪂��̐����̗�ɃZ�[�u�����
// �N���[�o�[�擾��
#define CLOVER_DATA_CSV			("Asserts/clover_data.csv")

// �X�e�[�W���
#define SPRING_1_STAGE_CSV		("Asserts/spring_1.csv")
#define SPRING_2_STAGE_CSV		("Asserts/spring_2.csv")
#define SUMMER_1_STAGE_CSV		("Asserts/summer_1.csv")
#define SUMMER_2_STAGE_CSV		("Asserts/summer_2.csv")
#define FALL_1_STAGE_CSV		("Asserts/fall_1.csv")
#define FALL_2_STAGE_CSV		("Asserts/fall_2.csv")
#define WINTER_1_STAGE_CSV		("Asserts/winter_1.csv")
#define WINTER_2_STAGE_CSV		("Asserts/winter_2.csv")

#define SOUND_DATA_CSV			("Asserts/sound_data.csv")

//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------
#define TUTORIAL_DATA_CSV		("Asserts/tutorial.csv")	//�`���[�g���A�������o���Ă��邩�ǂ����̃f�[�^
//----------------------------------------------------------------

//===== �N���X��` =====
class FileManager
{
	//=== �����o�ϐ�
private:
	//	�e�f�[�^�̒u������
	using ResourceMap = std::map<std::string, ResourceBase*>;
	using ResourceMapIt = ResourceMap::iterator;
	using ResourceMapPair = std::pair<std::string, ResourceBase*>;

	ResourceMap m_resources;					// ���\�[�X�̈ꊇ�Ǘ�
	static FileManager* m_pInstance;			// �C���X�^���X

	//=== �����o�֐�
private:
	FileManager();								// �R���X�g���N�^
public:
	static FileManager* GetInstance();			// �C���X�^���X�擾
	static void Uninit();						// �C���X�^���X�̔j��
	~FileManager();								// �f�X�g���N�^
	bool Load(std::string fileName);			// ���[�h

	ResourceBase* Get(std::string fileName);	// �f�[�^�擾
};

