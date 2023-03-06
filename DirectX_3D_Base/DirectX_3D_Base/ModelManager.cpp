#include "ModelManager.h"
#include <crtdbg.h>

//===== �ÓI�����o�ϐ� =====
ModelManager* ModelManager::m_pInstance = nullptr;

// �R���X�g���N�^
ModelManager::ModelManager()
{
}

// �f�X�g���N�^
ModelManager::~ModelManager()
{
	for (auto buff : m_ModelList)
	{
		buff.second->Release();
		delete buff.second;
	}
	m_ModelList.clear();
}

// �C���X�^���X��
ModelManager* ModelManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new ModelManager;
	}

	return m_pInstance;
}

// ���f���}�l�[�W���[���
void ModelManager::Uninit()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ���f���ǉ�
bool ModelManager::AddModel(const char* pszFileName, int nModelID)
{
	//*** ���f�������ɂ�������Ԃ�
	if (m_ModelList[nModelID] != nullptr) return true;

	//*** �ϐ��錾
	CAssimpModel* model = new CAssimpModel();

	// ���f���f�[�^�̓ǂݍ���
	if (!model->Load(GetDevice(), GetDeviceContext(), pszFileName))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return false;
	}

	//*** map�ɒǉ�
	m_ModelList[nModelID] = model;

	return true;
}

// ���f���擾
CAssimpModel* ModelManager::GetModel(int nModelID)
{
	//*** ����
	auto itr = m_ModelList.find(nModelID);

	//*** ���ʔ���
	if (itr == m_ModelList.end())
	{	// �Ȃ�������
		return nullptr;
	}
	else
	{	// ��������
		return (*itr).second;
	}
	return nullptr;
}

// ���f���폜
bool ModelManager::EraseModel(int nModelID)
{
	//*** �T��
	auto Model = this->GetModel(nModelID);

	//if (Model)	// ��������
	//{
		m_ModelList.erase(nModelID);
		return true;
	//}

	// �Ȃ�������
	return false;
}
