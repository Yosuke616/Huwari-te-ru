#include "FileManager.h"
#include "ResourceCSV.h"

//=== 静的メンバ変数
FileManager* FileManager::m_pInstance = nullptr;

// インスタンス取得
FileManager* FileManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new FileManager();
	}

	return m_pInstance;
}

// インスタンスの破棄
void FileManager::Uninit()
{
	if (m_pInstance)
	{
		delete m_pInstance;
	}
}

// コンストラクタ
FileManager::FileManager(){
}

// デストラクタ
FileManager::~FileManager()
{
	//	読み込まれたデータを解放
	ResourceMapIt it = m_resources.begin();
	while (it != m_resources.end())
	{
		delete it->second;
		it++;
	}
}

// ロード
bool FileManager::Load(std::string file)
{
	//	読み込み済みのデータであれば処理しない
	if (m_resources.find(file) != m_resources.end())
	{
		//	きーがみつからなかった = 既に読み込まれている
		return true;
	}

	//--- キーがないので読み込み処理
	//	拡張子ごとにデータクラスをメモリ確保
	ResourceBase* pResource;
	if (file.find(".csv") != std::string::npos)
	{
		pResource = new ResourceCSV();
	}
	else return false;
	

	//	データの読み込み
	bool result = false;
	result = pResource->Load(file.c_str());
	if (!result)
	{
		//	読み込めなかった場合
		delete pResource;
		return false;
	}

	//	マップに登録
	m_resources.insert(ResourceMapPair(file, pResource));

	return true;
}

// データ取得
ResourceBase* FileManager::Get(std::string fileName)
{
	//	データが存在するか
	if (m_resources.find(fileName) == m_resources.end())
	{
		//	なかったらnullptrを返す
		return nullptr;
	}

	//	存在するデータを返す
	return m_resources.at(fileName);
}