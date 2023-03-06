#define _CRT_SECURE_NO_WARNINGS

#include "ResourceCSV.h"
#include "FileManager.h"
#include <sstream>


// �R���X�g���N�^
ResourceCSV::ResourceCSV(){
}

// �f�X�g���N�^
ResourceCSV::~ResourceCSV(){
}

// csv�̓ǂݍ���
bool ResourceCSV::Load(std::string fileName)
{
	//	�t�@�C���̓ǂݍ���
	FILE* fp;
	fp = fopen(fileName.c_str(), "rt");
	if (!fp)
	{
		//	�ǂݍ��߂Ȃ�����
		return false;
	}

	//	���g�̋z���o��
	size_t fileSize;
	fseek(fp, 0, SEEK_END);		//	�t�@�C���I�[�܂Ń|�C���^���ړ�
	fileSize = ftell(fp);		//	�t�@�C���|�C���^���n�_����ǂꂾ������Ă��邩 = �T�C�Y
	fseek(fp, 0, SEEK_SET);		//	�t�@�C���̐擪�Ɉړ�
	std::string text;
	text.resize(fileSize);		//	vector�݂����Ɉ�����???
	if(fileSize > 0)
		fread(&text.front(), fileSize, 1, fp);	//	text��text�̐擪����fileSize���̓ǂݍ��݂��P�񂷂�

	//	�t�@�C�������
	fclose(fp);

	//--- csv�̉��
	//	1�s����؂�
	std::vector<std::string> lines;
	int stsrtIndex = 0;				//	��؂�̊J�n�ʒu
	int endIndex = 0;				//	��؂�̏I���ʒu
	while (endIndex != std::string::npos)	// string::npos = find�Ō�����Ȃ��������̒l
	{
		//	'\n'(���s�̕����L�����o�Ă���ʒu�̒T��
		endIndex = (int)text.find("\n", stsrtIndex);	//	������Ȃ�������string::npos�̒l���i�[

		//	�������Ă���i�[���ēǂݍ��݂܂���
		if (endIndex != std::string::npos)
		{
			//	�T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
			lines.push_back(text.substr(stsrtIndex, endIndex - stsrtIndex));
		}

		//	���̒T���J�n�ʒu�̍X�V
		stsrtIndex = endIndex + 1;
	}

	//	1�Z�����f�[�^���i�[
	std::vector<std::string>::iterator linesItr = lines.begin(); // string��vector
	while (linesItr != lines.end())
	{
		Column column;	// ��

		stsrtIndex = 0;
		endIndex = 0;

		while (endIndex != std::string::npos)
		{
			//	','�̈ʒu��T��
			endIndex = (int)linesItr->find(",", stsrtIndex);	// ���̍s��,��T��

			//	��قǎ��o�����s��','�ŋ�؂��Ď��o��
			if (endIndex != std::string::npos)
			{
				//	�T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
				column.push_back(linesItr->substr(stsrtIndex, endIndex - stsrtIndex));
			}
			else
			{
				column.push_back(linesItr->substr(stsrtIndex));
			}

			//	���̒T���J�n�ʒu�̍X�V
			stsrtIndex = endIndex + 1;
		}

		//	�Z�����Ƃɋ�؂����s��S�̂̃f�[�^(grid)�֒ǉ�
		m_grid.push_back(column);

		//	���̍s
		linesItr++;
	}

	return true;
}

// csv�ɏ�������
bool ResourceCSV::Save(std::string fileName)
{
	//	������
	FILE *pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	�������ݐ悪
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;

	for (auto&& row : m_grid)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// �Z���̒��g�i�[
			//fwrite(cell.c_str(), sizeof(char) * cell.size(), 1, pFile);
			fprintf(pFile, cell.c_str());
			//	csv�̓Z����","�ŋ�؂�
			if ((int)row.size() != cnt_x)	// �Ō�͂���Ȃ�
			{
				//fwrite(",", sizeof(char), 1, pFile);
				fprintf(pFile, ",");
			}
		}

		//	csv��"\n"�ŉ��s����
		if ((int)row.size() != cnt_y)	// �Ō�͂���Ȃ�
		{
			//fwrite("\n", sizeof(char) * 2, 1, pFile);
			fprintf(pFile, "\n");
		}

		// �J�E���g������
		cnt_x = 0;
	}

	fclose(pFile);

	return true;
}

//--- gettser
// �Z���̖͋[���擾
std::string ResourceCSV::GetStr(int x, int y)
{
	return m_grid[y][x];
}
// �Z���̕�����int�^�Ŏ擾
int ResourceCSV::GetInt(int x, int y)
{
	//	atoi �������int�ɕς���
	return atoi(m_grid[y][x].c_str());
}
// �Z���̕�����float�^�Ŏ擾
float ResourceCSV::GetFloat(int x, int y)
{
	//	atof �������float�ɕς���
	return (float)atof(m_grid[y][x].c_str());
}
// �s�̐����擾
int ResourceCSV::GetRowSize()
{
	return (int)m_grid.size();
}
// ��̐����擾
int ResourceCSV::GetColumnSize(int row)
{
	// ��O����
	if ((int)m_grid.size() <= row) return 0;

	return (int)m_grid[row].size();
}

//--- setter
// �Z���ɕ���������
void ResourceCSV::SetStr(int x, int y, std::string str)
{
	// �s�̃T�C�Y�m�F
	//if (y >= (int)m_grid.size()) return;
	//
	//// ��̃T�C�Y�m�F
	//if (x >= (int)m_grid[y].size()) return;

	m_grid[y][x] = str;
}
// �Z���ɐ���������
void ResourceCSV::SetInt(int x, int y, int num)
{
	// ����Ȃ��s�͒ǉ�
	if (y >= (int)m_grid.size())
	{
		m_grid.resize(y + 1);
	}

	// ����Ȃ���͒ǉ�
	if (x >= (int)m_grid[y].size())
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_grid[y].push_back(ss.str());
	}
	else
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_grid[y][x] = ss.str();
	}
}
// �Z���ɕ��������_��������
void ResourceCSV::SetFloat(int x, int y, float num)
{
	// ����Ȃ��s�͒ǉ�
	if (y >= (int)m_grid.size())
	{
		m_grid.resize(y + 1);
	}

	// ����Ȃ���͒ǉ�
	if (x >= (int)m_grid[y].size())
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_grid[y].push_back(ss.str());
	}
	else
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_grid[y][x] = ss.str();
	}
}
// �s�̐���ݒ�
void ResourceCSV::SetRowSize(int size)
{
	m_grid.resize(size);
}
// ��̐���ݒ�
void ResourceCSV::SetColumnSize(int row, int size)
{
	// ��O����
	if ((int)m_grid.size() <= row) return;

	m_grid[row].resize(size);
}

// int �� char�ϊ����Ă���csv�t�@�C���ɕۑ�����
bool ResourceCSV::Save(std::string fileName, IntRow data)
{
	// �V�����f�[�^�Ɗ����̃f�[�^�̃T�C�Y���������ǂ���
	if (m_grid.size() != data.size() ||
		m_grid[0].size() != data[0].size())
	{
		return false;
	}

	//	������
	FILE* pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	�������ݐ悪
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;
	std::ostringstream ss;

	for (auto&& row : data)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// �o�b�t�@���N���A
			ss.str("");
			// ��Ԃ��N���A
			ss.clear(std::stringstream::goodbit);
			// int �� char�ϊ�
			ss << cell;

			// �f�[�^�̏㏑��
			m_grid[cnt_y - 1][cnt_x - 1] = ss.str().c_str();
			
			// �Z���̒��g�i�[
			fprintf(pFile, ss.str().c_str());
			//	csv�̓Z����","�ŋ�؂�
			if ((int)row.size() != cnt_x)	// �Ō�͂���Ȃ�
			{
				fprintf(pFile, ",");
			}
		}

		//	csv��"\n"�ŉ��s����
		if ((int)row.size() != cnt_y)	// �Ō�͂���Ȃ�
		{
			fprintf(pFile, "\n");
		}

		// �J�E���g������
		cnt_x = 0;
	}

	fclose(pFile);

	// ���\�[�X�̍X�V
	

	return true;
}