#include "FileManager.h"
#include "ResourceCSV.h"

//=== �ÓI�����o�ϐ�
FileManager* FileManager::m_pInstance = nullptr;

// �C���X�^���X�擾
FileManager* FileManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new FileManager();
	}

	return m_pInstance;
}

// �C���X�^���X�̔j��
void FileManager::Uninit()
{
	if (m_pInstance)
	{
		delete m_pInstance;
	}
}

// �R���X�g���N�^
FileManager::FileManager(){
}

// �f�X�g���N�^
FileManager::~FileManager()
{
	//	�ǂݍ��܂ꂽ�f�[�^�����
	ResourceMapIt it = m_resources.begin();
	while (it != m_resources.end())
	{
		delete it->second;
		it++;
	}
}

// ���[�h
bool FileManager::Load(std::string file)
{
	//	�ǂݍ��ݍς݂̃f�[�^�ł���Ώ������Ȃ�
	if (m_resources.find(file) != m_resources.end())
	{
		//	���[���݂���Ȃ����� = ���ɓǂݍ��܂�Ă���
		return true;
	}

	//--- �L�[���Ȃ��̂œǂݍ��ݏ���
	//	�g���q���ƂɃf�[�^�N���X���������m��
	ResourceBase* pResource;
	if (file.find(".csv") != std::string::npos)
	{
		pResource = new ResourceCSV();
	}
	else return false;
	

	//	�f�[�^�̓ǂݍ���
	bool result = false;
	result = pResource->Load(file.c_str());
	if (!result)
	{
		//	�ǂݍ��߂Ȃ������ꍇ
		delete pResource;
		return false;
	}

	//	�}�b�v�ɓo�^
	m_resources.insert(ResourceMapPair(file, pResource));

	return true;
}

// �f�[�^�擾
ResourceBase* FileManager::Get(std::string fileName)
{
	//	�f�[�^�����݂��邩
	if (m_resources.find(fileName) == m_resources.end())
	{
		//	�Ȃ�������nullptr��Ԃ�
		return nullptr;
	}

	//	���݂���f�[�^��Ԃ�
	return m_resources.at(fileName);
}