#include "ModelManager.h"
#include <crtdbg.h>

//===== 静的メンバ変数 =====
ModelManager* ModelManager::m_pInstance = nullptr;

// コンストラクタ
ModelManager::ModelManager()
{
}

// デストラクタ
ModelManager::~ModelManager()
{
	for (auto buff : m_ModelList)
	{
		buff.second->Release();
		delete buff.second;
	}
	m_ModelList.clear();
}

// インスタンス化
ModelManager* ModelManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new ModelManager;
	}

	return m_pInstance;
}

// モデルマネージャー解放
void ModelManager::Uninit()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// モデル追加
bool ModelManager::AddModel(const char* pszFileName, int nModelID)
{
	//*** モデルが既にあったら返す
	if (m_ModelList[nModelID] != nullptr) return true;

	//*** 変数宣言
	CAssimpModel* model = new CAssimpModel();

	// モデルデータの読み込み
	if (!model->Load(GetDevice(), GetDeviceContext(), pszFileName))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
		return false;
	}

	//*** mapに追加
	m_ModelList[nModelID] = model;

	return true;
}

// モデル取得
CAssimpModel* ModelManager::GetModel(int nModelID)
{
	//*** 検索
	auto itr = m_ModelList.find(nModelID);

	//*** 結果判定
	if (itr == m_ModelList.end())
	{	// なかった時
		return nullptr;
	}
	else
	{	// あった時
		return (*itr).second;
	}
	return nullptr;
}

// モデル削除
bool ModelManager::EraseModel(int nModelID)
{
	//*** 探す
	auto Model = this->GetModel(nModelID);

	//if (Model)	// あった時
	//{
		m_ModelList.erase(nModelID);
		return true;
	//}

	// なかった時
	return false;
}
