//===== インクルード =====
#include "SceneManager.h"
#include "fade.h"
	
#include "sceneTitle.h"
#include "sceneGame.h"
#include "SceneSelect.h"
#include "SceneEnding.h"

//===== 静的メンバ変数 =====
SceneManager* SceneManager::m_pInstance = nullptr;

// インスタンス生成
SceneManager* SceneManager::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager();
	}

	return m_pInstance;
}

// インスタンスの削除
void SceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// コンストラクタ
SceneManager::SceneManager()
	: m_CurrentScene(nullptr)
{
}

// デストラクタ
SceneManager::~SceneManager()
{
	//	シーンリストを削除
	for (auto scene : m_SceneList)
		delete scene;

	//	リストを空にする
	m_SceneList.clear();
}

// 初期化
void SceneManager::Init()
{
	//	全シーンをリストに追加
	m_SceneList.push_back(new SceneTitle());			//	タイトルシーンの追加
	m_SceneList.push_back(SceneGame::GetInstance());	//	ゲームシーンの追加
	m_SceneList.push_back(new SceneSelect());			//	セレクトシーンの追加
	m_SceneList.push_back(new SceneEnding());			//	エンディングの追加

	//	最初はタイトル画面へ
	SetScene(SCENE_TITLE);

	//	フェード初期化
	InitFade();
}

// 更新
void SceneManager::Update()
{
	//	シーン更新
	m_CurrentScene->Update();

	//	フェードイン/アウト更新
	UpdateFade();
}

// 描画
void SceneManager::Draw()
{
	//	シーン描画
	m_CurrentScene->Draw();
	//	フェードイン/アウト描画
	DrawFade();

}

// 終了
void SceneManager::Uninit()
{
	// フェード終了処理
	UninitFade();

	//	インスタンス破棄
	SceneManager::Destroy();
}

// シーンの入手
EScene SceneManager::GetScene() const
{
	return m_eSceneState;
}

// シーンの切り替え
void SceneManager::SetScene(EScene NextScene)
{
	//	例外処理
	if (NextScene >= SCENE_MAX)
	{
		MessageBox(GetMainWnd(), _T("シーン切り替え失敗"), NULL, MB_OK);
		return;
	}

	//	前シーンの終了処理
	if(m_CurrentScene)
		m_CurrentScene->Uninit();

	//	現在シーンの開始
	m_CurrentScene = m_SceneList[NextScene];	//	シーン切り替え
	m_eSceneState = NextScene;					//	シーン状態の設定
	m_CurrentScene->Init();						//	初期化
}