//===== インクルード　=====
#include "sceneTitle.h"
#include "fade.h"

#include "Object.h"
#include "ObjectManager.h"
#include "MenuManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "imgui.h"
#include "Sound.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Draw3dComponent.h"
#include "BGChangeComponent.h"
#include "FadeChangeComponent.h"
#include "MenuComponent.h"
#include "CreateLeaf.h"
#include "SceneSelect.h"
#include "Load.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------------
#include "ResourceCSV.h"
#include "FileManager.h"
#include "sceneGame.h"
//----------------------------------------------------------------------

//===== マクロ定義 =====
#define WIDTH_TITLE_LOGO	(720)												// タイトルロゴの横幅
#define HEIGHT_TITLE_LOGO	(720)												// タイトルロゴの縦幅
#define POS_X_TITLE_LOGO	(-280.0f)											// タイトルロゴの X 座標
#define POS_Y_TITLE_LOGO	(SCREEN_HEIGHT - HEIGHT_TITLE_LOGO)*0.5f + 100.0f	// タイトルロゴの Y 座標

#define BG_CHANGE_TIME		(950)												// テクスチャが切り替わる時間
#define FADE_TIME			(BG_CHANGE_TIME - 100)								// テクスチャ用のフェードポリゴンが切り替わる時間


#define POS_X_TITLE_PLAYER	(-150.0f)
#define POS_Y_TITLE_PLAYER	(-125.0f)


//=== 静的メンバ変数
int SceneTitle::m_nTitleSeason;
SceneTitle::Row SceneTitle::m_TutorialVector;	// チュートリアルのベクター

CLeaf g_TitleLeaf;
CDrawModel* g_pPlayer = nullptr;

// コンストラクタ
SceneTitle::SceneTitle()
{
	SceneTitle::LoadTurorial();
}

// デストラクタ
SceneTitle::~SceneTitle()
{
}

// 初期化
void SceneTitle::Init()
{
	//===== ロード画面のスレッドを作る =====
	Load::Begin();

	// 最初は春
	m_nTitleSeason = SPRING;

	//	管理クラスのインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();			// オブジェクトマネージャー
	m_pMenuManager = MenuManager::GetInstance();				// メニューマネージャー


	ModelManager *pModelManager = ModelManager::GetInstance();	// モデルマネージャー
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_PLAYER, SUMMER_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_PLAYER  , FALL_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_PLAYER, WINTER_PLAYER_NUM);

	/* テクスチャ読み込み */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// メニューアイコン
	pTexManager->AddTexture(PATH_TEX_STOP_GAME, STOP_GAME_TEX_NUM);
	// ロゴ
	pTexManager->AddTexture(PATH_TEX_TITLE_LOGO, TITLE_LOGO_TEX_NUM);
	// 背景
	pTexManager->AddTexture(PATH_TEX_TITLE_SPRING, TITLE_SPRING_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_SUMMER, TITLE_SUMMER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_AUTUMN, TITLE_AUTUMN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TITLE_WINTER, TITLE_WINTER_TEX_NUM);
	// ビネット
	pTexManager->AddTexture(PATH_TEX_VINETTO, VINETTO_TEX_NUM);
	// 文字
	pTexManager->AddTexture(PATH_TEX_GAMEOVER, GAMEOVER_TEX_NUM);			// ゲームオーバー
	pTexManager->AddTexture(PATH_TEX_CONTINUE, CONTINUE_TEX_NUM);			// つづける
	pTexManager->AddTexture(PATH_TEX_RESTART, RESTART_TEX_NUM);				// やりなおす
	pTexManager->AddTexture(PATH_TEX_EXIT, EXIT_TEX_NUM);					// おわる
	pTexManager->AddTexture(PATH_TEX_NEW_GAME, NEW_GAME_TEX_NUM);			// ニューゲーム
	pTexManager->AddTexture(PATH_TEX_OPTION, OPTION_TEX_NUM);				// オプション
	pTexManager->AddTexture(PATH_TEX_RESUME, RESUME_TEX_NUM);				// さいかい
	pTexManager->AddTexture(PATH_TEX_SELECT, SELECT_TEX_NUM);				// セレクトへ
	
	// ふってくるもの
	pTexManager->AddTexture(PATH_TEX_SPRING_LEAF, LEAF_SPRING_NUM);			// 春のはっぱ
	pTexManager->AddTexture(PATH_SUMMER_LEAF, SUMMER_LEAF_TEX_NUM);			// 夏のはっぱ
	pTexManager->AddTexture(PATH_FALL_LEAF, FALL_LEAF_TEX_NUM);				// 秋のはっぱ
	pTexManager->AddTexture(PATH_TEX_SNOW, SNOW_TEX_NUM);					// 冬の雪

	pTexManager->AddTexture(PATH_TEX_SPIDER, SPIDER_TEX_NUM);				// クモの巣
	// オプション関連
	pTexManager->AddTexture(PATH_TEX_CONFIG_GAUGE, CONFIG_GAUGE_TEX_NUM);		// 音量ゲージ
	//pTexManager->AddTexture(PATH_TEX_CONFIG_GAUGE_F, CONFIG_GAUGE_F_TEX_NUM);	// 音量ゲージ枠
	pTexManager->AddTexture(PATH_TEX_BGM, BGM_TEX_NUM);							// bgm
	pTexManager->AddTexture(PATH_TEX_SE, SE_TEX_NUM);							// se
	pTexManager->AddTexture(PATH_TEX_BACK, BACK_TEX_NUM);						// もどる

	//文字のRESTARTをこれに変えてほしい
	//2022/1/10 Shimizu Yosuke ----------------------------------------
	//枠を追加
	pTexManager->AddTexture(PATH_TEX_CONTINUE_FRAME, CONTINUE_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_RESUME_FRAME, RESUME_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_EXIT_FRAME, EXIT_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_OPTION_FRAME, OPTION_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_NEW_GAME_FRAME, NEW_GAME_FRAME_TEX_NUM);
	//-----------------------------------------------------------------

	pTexManager->AddTexture(PATH_TEX_TITLE, TITLE_TEX_NUM);

	// sawada 文字の枠
	pTexManager->AddTexture(PATH_TEX_RESTART_FRAME, RESTART_FRAME_TEX_NUM);					// やりなおし
	pTexManager->AddTexture(PATH_TEX_SELECT_FRAME, SELECT_FRAME_TEX_NUM);					// セレクトへ
	pTexManager->AddTexture(PATH_TEX_TITLE_FRAME, TITLE_FRAME_TEX_NUM);						// タイトルへ

	//2022/1/30/UI_Change Shimizu Yosuke ---------------------------
	pTexManager->AddTexture(PATH_TEX_CLICK, CLICK_TEX_NUM);
	//--------------------------------------------------------------



#pragma region	---プレイヤー	2022/01/21 tomoya
	//	オブジェクト生成
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	コンポーネントの追加
	auto TransPlayer = Player->AddComponent<CTransform>();				//	トランスフォーム
	auto DrawPlayer = Player->AddComponent<CDrawModel>();					//	モデル描画機能
	//	オブジェクト情報の設定
	DrawPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));			//	待機、吹かれている、落ちる、拾う
	TransPlayer->SetScale(12.0f, 12.0f, 12.0f);									//	スケール
	TransPlayer->SetVelocity(0.0f, 0.0f, 0.0f);									//	
	TransPlayer->SetPosition(POS_X_TITLE_PLAYER, POS_Y_TITLE_PLAYER, 0.0f);		//	初期配置
	TransPlayer->SetRotate(0.0f, -45.0f, 0.0f);
	// アニメーション設定
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);// 待機
	DrawPlayer->AlwaysAnimation(true);				// 常にアニメーション
	DrawPlayer->Play(PLAYER_IDLE);					//待機モーション再生
	// 3Dリストに追加
	m_pObjectManager->AddObject(Player);
#pragma endregion

#pragma region ---背景影
	// オブジェクト生成
	Object* objBG_Shadow = new Object("BG_Shadow", UPDATE_BG, DRAW_BG);
	// コンポーネントの追加
	objBG_Shadow->AddComponent<CTransform>();
	auto Draw_BG_Shadow = objBG_Shadow->AddComponent<CDraw2D>();
	// オブジェクトの設定
	Draw_BG_Shadow->SetTexture(pTexManager->GetTexture(VINETTO_TEX_NUM));
	Draw_BG_Shadow->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// リストに追加
	m_pObjectManager->AddObject(objBG_Shadow);
#pragma endregion

#pragma region ---背景
	//	オブジェクト生成
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	objBG->AddComponent<CTransform>();									// 座標
	auto Draw_BG = objBG->AddComponent<CDraw2D>();						// 描画
	auto Change_BG = objBG->AddComponent<CBgChange>();					// 背景切替
	//	オブジェクトの設定
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_SPRING_TEX_NUM));	// テクスチャ
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);						// サイズ
	Change_BG->SetTimer(BG_CHANGE_TIME);								// 時間設定
	//	リストに追加
	m_pObjectManager->AddObject(objBG);									// オブジェクトの追加
#pragma endregion

#pragma region ---タイトルロゴ
	//	オブジェクト生成
	Object* objTitleLogo = new Object("titlelogo", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto TransTitleLogo = objTitleLogo->AddComponent<CTransform>();			// 座標
	auto pTitleDraw = objTitleLogo->AddComponent<CDraw2D>();				// 描画
	//	オブジェクトの設定
	pTitleDraw->SetTexture(pTexManager->GetTexture(TITLE_LOGO_TEX_NUM));	// テクスチャの設定
	pTitleDraw->SetSize(WIDTH_TITLE_LOGO, HEIGHT_TITLE_LOGO);				// サイズの設定
	TransTitleLogo->SetPosition(POS_X_TITLE_LOGO, POS_Y_TITLE_LOGO);		// 座標の設定
	//	リストに追加
	m_pObjectManager->AddObject(objTitleLogo);								// オブジェクトの追加
#pragma endregion

#pragma region ---フェードインアウトオブジェクト
//	オブジェクト生成
	Object* objFade = new Object("fade", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto TransFade = objFade->AddComponent<CTransform>();			// 座標
	auto DrawFade = objFade->AddComponent<CDraw2D>();				// 描画
	auto Fade = objFade->AddComponent<CFadeChange>();				// フェード処理
	//	オブジェクトの設定
	DrawFade->SetTexture(nullptr);									// テクスチャの設定
	DrawFade->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);					// サイズの設定
	DrawFade->SetColor(1.0f, 1.0f, 1.0f);							// 色
	Fade->SetTimer(FADE_TIME);										// フェード時間
	//	リストに追加
	m_pObjectManager->AddObject(objFade);							// オブジェクトの追加
#pragma endregion

	//	メニュー生成	
	m_pMenuManager->CreateTitleMenu();

	//	プレイヤー描画情報の格納
	g_pPlayer = DrawPlayer;

	//	BGM再生
	CSound::Play(BGM_TITLE);

	//===== ロード終了処理 =====
	Load::End();
}

// 終了処理
void SceneTitle::Uninit()
{
	//	sound stop
	CSound::Stop(BGM_TITLE);
	

	//	メニューの終了処理
	MenuManager::Destroy();

	// 終了処理
	m_pObjectManager->Uninit();
}

// 更新
void SceneTitle::Update()
{
	ModelManager *pModelManager = ModelManager::GetInstance();	// モデルマネージャー
	switch (m_nTitleSeason)
	{
	case SPRING: g_pPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));	break;
	case SUMMER: g_pPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));	break;
	case AUTUMN: g_pPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));		break;
	case WINTER: g_pPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));	break;
	default:	break;
	}

	// ふってくるもの更新
	g_TitleLeaf.UpdateTitleScene();

	//	オブジェクトリストの更新
	m_pObjectManager->Update();

	//	メニュー画面の更新
	m_pMenuManager->Update();
}

// 描画
void SceneTitle::Draw()
{
	//	オブジェクトリストの描画
	m_pObjectManager->Draw();

	//	ポーズ画面の描画
	m_pMenuManager->Draw();

#ifdef _DEBUG
	//	デバッグ用
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	Text("Objects : %d", m_pObjectManager->GetDrawList().size());
	Text("Menu    : %d", m_pMenuManager->GetMenuList().size());
	Text("GridSize : %d", SceneSelect::GetSelectGrid().size());
	End();
#endif // _DEBUG
}

//	季節設定
void SceneTitle::SetSeason(int season)
{
	m_nTitleSeason = season;

	if (m_nTitleSeason < 0) m_nTitleSeason = 0;
	if (m_nTitleSeason > 3) m_nTitleSeason = 3;
}



//2022/1/28/Tutorial_CSV Shimizu Yosuke -----------------------
//LoadTutorial
bool SceneTitle::LoadTurorial() {
	// load
	if (FileManager::GetInstance()->Load(TUTORIAL_DATA_CSV) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	//配列にほぞんする　
	//行の数の設定
	m_TutorialVector.resize(CSV->GetRowSize());
	for (int y = 0; y < CSV->GetRowSize(); ++y) {
		//列の数の設定
		m_TutorialVector[y].resize(CSV->GetColumnSize(y));
		for (int x = 0; x < CSV->GetColumnSize(y); ++x) {
			//要素を格納
			m_TutorialVector[y][x] = CSV->GetInt(x, y);
		}
	}

	return true;

}

//SaveTutorial 
bool SceneTitle::SaveTutorial() {
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);
	if (CSV == nullptr) {
		return false;
	}

	//ゲーム画面内だったら書き換えるようにする
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		switch (SceneGame::GetInstance()->GetSeason()) {
		case SPRING_1:	CSV->SetInt(0, 0, 1);	break;
		case SUMMER_1:	CSV->SetInt(1, 0, 1);	break;
		case FALL_1:	CSV->SetInt(2, 0, 1);	break;
		case WINTER_1:	CSV->SetInt(3, 0, 1);	break;
		default:break;
		}
	}
	else if(SceneManager::Instance()->GetScene() == SCENE_TITLE)
	{
		for (int nRow = 0; nRow < m_TutorialVector.size(); nRow++)
		{
			for (int nColumn = 0; nColumn < m_TutorialVector[nRow].size(); nColumn++)
			{
				CSV->SetInt(nColumn, nRow, 0);
			}
		}
	}

	
	//save
	if (CSV->Save(TUTORIAL_DATA_CSV) == false) {
		//エラーが起きたら
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

//NewGame
bool SceneTitle::NewGame() {
	//書き換え
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	for (int nRow = 0; nRow < m_TutorialVector.size(); nRow++)
	{
		for (int nColumn = 0; nColumn < m_TutorialVector[nRow].size(); nColumn++)
		{
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	SceneTitle::SaveTutorial();

	return true;
}

//FirstPlay
bool SceneTitle::FirstPlay(int nSeason)
{
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(TUTORIAL_DATA_CSV);

	// 念のため
	if (CSV == nullptr) return false;

	//ここに値を保存して1だったらtrue、0だったらfalse
	//int nTutorial_Num;

	// 初回かどうか
	bool bFirst = false;

	//セルを取得する
	switch (nSeason)
	{
		// 四季の最初のステージ
	case SPRING_1:	if(CSV->GetInt(0, 0) == 0) bFirst = true; break;	// 初回だった場合セルの中身は0なので
	case SUMMER_1:	if(CSV->GetInt(1, 0) == 0) bFirst = true; break;
	case FALL_1:	if(CSV->GetInt(2, 0) == 0) bFirst = true; break;
	case WINTER_1:	if(CSV->GetInt(3, 0) == 0) bFirst = true; break;

	case SPRING_2:	case SUMMER_2:	case FALL_2:	case WINTER_2: bFirst = false; break;

		// それ以外
	default:	bFirst = false;
	}

	return bFirst;

	//if (nTutorial_Num) {
	//	return true;
	//}
	//return false;
}

//-------------------------------------------------------------