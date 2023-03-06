/*
	���\�[�X�E�E�E�Q�[�����Ŏg�p����O���f�[�^
	�e�N�X�`���A���f���A�T�E���h�A�Z�[�u�f�[�^�A�Q�[���ݒ�A
	�R���t�B�O�A���x���f�U�C���A�A�C�e���A�G�̏��

*/
#ifndef __RESOURCE_BASE_H__
#define __RESOURCE_BASE_H__

//	�C���N���[�h
#include <string>

class ResourceBase
{
protected:
	std::string m_fileName;	// �t�@�C����
public:
	ResourceBase();			// �R���X�g���N�^
	~ResourceBase();		// �f�X�g���N�^

	virtual bool Load(std::string fileName) = 0;	// ���[�h
	virtual bool Save(std::string fileName) = 0;	// �Z�[�u
};
#endif // !__RESOURCE_BASE_H__
