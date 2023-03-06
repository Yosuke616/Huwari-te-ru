/**
 * @file    TextureManager.cpp
 * @brief	�e�N�X�`���ꌳ�Ǘ��v���O����
 * @date	2021/05/26
 * @version	1.0.0
 * @author	AT12A 33 �Ȃ�����
 */

//====== �C���N���[�h =====
#include "TextureManager.h"
#include "Texture.h"
#include "main.h"
#include <crtdbg.h>

//===== �ÓI�����o�ϐ� =====
TextureManager*	TextureManager::m_pInstance = nullptr;

//===== �֐���` =====

/**
 * @fn		TextureManager::TextureManager
 * @brief	�R���X�g���N�^
*/
TextureManager::TextureManager()
{
}

/**
 * @fn		TextureManager::~TextureManager
 * @brief	�f�X�g���N�^
*/
TextureManager::~TextureManager()
{
	for(auto buff : m_TextureList)
	{
		SAFE_RELEASE(buff.second);
	}
	m_TextureList.clear();
}

/**
 * @fn		TextureManager::GetInstance
 * @brief	�e�N�X�`���}�l�[�W���[�̃C���X�^���X��
*/
TextureManager* TextureManager::GetInstance(void)
{
	if(!m_pInstance)
	{
		m_pInstance = new TextureManager;
	}

	return m_pInstance;
}

/**
 * @fn		TextureManager::UninitTextureManager
 * @brief	�e�N�X�`���}�l�[�W���[�̉��
*/
void TextureManager::Uninit(void)
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
 * @fn			TextureManager::AddTexture
 * @brief		�e�N�X�`���ǉ�
 * @param[in]	pszFileName	�t�@�C����
 * @param[in]	nTexID		�e�N�X�`���ƕR�t����L�[
 * @return		������ true, ���s�� false
*/
bool TextureManager::AddTexture(const char* pszFileName, int nTexID)
{
	//*** �e�N�X�`������������Ԃ�
	if (m_TextureList[nTexID] != nullptr) return true;

	//*** �ϐ��錾
	ID3D11ShaderResourceView* pTex;
	HRESULT hr;

	//*** �e�N�X�`���ǂݍ���
	hr = CreateTextureFromFile(GetDevice(), pszFileName, &pTex);

	if (FAILED(hr))
	{
		_ASSERT_EXPR(false, L"NO_TEXTURE");

		return false;
	}

	//*** map�ɒǉ�
	m_TextureList[nTexID] = pTex;

	return true;
}

/**
 * @fn			TextureManager::GetTexture
 * @brief		�e�N�X�`���擾
 * @param[in]	nTexID		�e�N�X�`���ƕR�t���Ă���L�[
 * @return		�������� �g�p�\�ȃe�N�X�`���ԍ�, �Ȃ������� 0
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(int nTexID)
{
	//*** ����
	auto itr = m_TextureList.find(nTexID);

	//*** ���ʔ���
	if (itr == m_TextureList.end())
	{	// �Ȃ�������
		return nullptr;
	}
	else
	{	// ��������
		return itr->second;
	}

	return nullptr;
}

/**
 * @fn			TextureManager::EraseTexture
 * @brief		�e�N�X�`���폜
 * @param[in]	nTexID		�e�N�X�`���ƕR�t���Ă���L�[
 * @return		������ true, ���s�� false
*/
bool TextureManager::EraseTexture(int nTexID)
{
	//*** �T��
	auto Tex = this->GetTexture(nTexID);

	if(Tex)	// ��������
	{
		m_TextureList.erase(nTexID);
		return true;
	}

	// �Ȃ�������
	return false;
}

/**
 * @fn			TextureManager::GetSize
 * @brief		���X�g�T�C�Y
 * @return		�ǂݍ��܂�Ă���e�N�X�`���̐�
*/

int TextureManager::GetSize()
{
	//*** �T��
	return (int)(m_TextureList.size());
}