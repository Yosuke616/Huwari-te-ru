/**
 * @file    TextureManager.cpp
 * @brief	テクスチャ一元管理プログラム
 * @date	2021/05/26
 * @version	1.0.0
 * @author	AT12A 33 なかがみ
 */

//====== インクルード =====
#include "TextureManager.h"
#include "Texture.h"
#include "main.h"
#include <crtdbg.h>

//===== 静的メンバ変数 =====
TextureManager*	TextureManager::m_pInstance = nullptr;

//===== 関数定義 =====

/**
 * @fn		TextureManager::TextureManager
 * @brief	コンストラクタ
*/
TextureManager::TextureManager()
{
}

/**
 * @fn		TextureManager::~TextureManager
 * @brief	デストラクタ
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
 * @brief	テクスチャマネージャーのインスタンス化
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
 * @brief	テクスチャマネージャーの解放
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
 * @brief		テクスチャ追加
 * @param[in]	pszFileName	ファイル名
 * @param[in]	nTexID		テクスチャと紐付けるキー
 * @return		成功時 true, 失敗時 false
*/
bool TextureManager::AddTexture(const char* pszFileName, int nTexID)
{
	//*** テクスチャがあったら返す
	if (m_TextureList[nTexID] != nullptr) return true;

	//*** 変数宣言
	ID3D11ShaderResourceView* pTex;
	HRESULT hr;

	//*** テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), pszFileName, &pTex);

	if (FAILED(hr))
	{
		_ASSERT_EXPR(false, L"NO_TEXTURE");

		return false;
	}

	//*** mapに追加
	m_TextureList[nTexID] = pTex;

	return true;
}

/**
 * @fn			TextureManager::GetTexture
 * @brief		テクスチャ取得
 * @param[in]	nTexID		テクスチャと紐付いているキー
 * @return		あった時 使用可能なテクスチャ番号, なかった時 0
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(int nTexID)
{
	//*** 検索
	auto itr = m_TextureList.find(nTexID);

	//*** 結果判定
	if (itr == m_TextureList.end())
	{	// なかった時
		return nullptr;
	}
	else
	{	// あった時
		return itr->second;
	}

	return nullptr;
}

/**
 * @fn			TextureManager::EraseTexture
 * @brief		テクスチャ削除
 * @param[in]	nTexID		テクスチャと紐付いているキー
 * @return		成功時 true, 失敗時 false
*/
bool TextureManager::EraseTexture(int nTexID)
{
	//*** 探す
	auto Tex = this->GetTexture(nTexID);

	if(Tex)	// あった時
	{
		m_TextureList.erase(nTexID);
		return true;
	}

	// なかった時
	return false;
}

/**
 * @fn			TextureManager::GetSize
 * @brief		リストサイズ
 * @return		読み込まれているテクスチャの数
*/

int TextureManager::GetSize()
{
	//*** 探す
	return (int)(m_TextureList.size());
}