/*
	���\�[�X�E�E�E�Q�[�����Ŏg�p����O���f�[�^
	�e�N�X�`���A���f���A�T�E���h�A�Z�[�u�f�[�^�A�Q�[���ݒ�A
	�R���t�B�O�A���x���f�U�C���A�A�C�e���A�G�̏��

*/
#ifndef __RESOURCE_CSV_H__
#define __RESOURCE_CSV_H__

// �C���N���[�h
#include <string>
#include <vector>
#include "ResourceBase.h"


// �N���X��`
class ResourceCSV : public ResourceBase
{
public:
	// vector��2�����z��
	using Cell = std::string;			// �Z��
	using Column = std::vector<Cell>;	// �s
	using Row = std::vector<Column>;	// ��

	using IntCell = int;
	using IntColumn = std::vector<int>;
	using IntRow = std::vector<IntColumn>;

	// �ǂݍ���csv�f�[�^�̊i�[��
	Row m_grid;

public:
	ResourceCSV();
	~ResourceCSV();

	bool Load(std::string fileName) override;	//	���[�h
	bool Save(std::string fileName) override;	//	�Z�[�u

	// int �� char�ϊ����Ă���csv�t�@�C���ɕۑ�����
	bool Save(std::string fileName, IntRow data);

	//getter
	std::string GetStr(int x, int y);
	int GetInt(int x, int y);
	float GetFloat(int x, int y);
	int GetRowSize();
	int GetColumnSize(int row);

	//setter
	void SetStr(int x, int y, std::string str);
	void SetInt(int x, int y, int num);
	void SetFloat(int x, int y, float num);
	void SetRowSize(int size);
	void SetColumnSize(int row, int size);
};
#endif // !__RESOURCE_CSV_H__
