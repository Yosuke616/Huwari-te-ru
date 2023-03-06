//===== インクルード =====
#include "SceneEnding.h"
#include "fade.h"
#include "imgui.h"

#include "MenuManager.h"
#include "ObjectManager.h"
#include "FileManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "WindManager.h"
#include "CreateLeaf.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "Sound.h"
#include "Camera.h"

#include "Component.h"				//	コンポーネント
#include "TransformComponent.h"		//	位置・速度・加速度
#include "Draw2dComponent.h"		//	2D描画
#include "Draw3dComponent.h"		//	モデル描画
#include "DrawMeshComponent.h"		//	メッシュ描画
#include "BillboardComponent.h"		//	ビルボード描画
#include "DrawEffectComponent.h"		//	エフェクト描画
#include "ColliderComponent.h"		//	あたり判定
#include "PlayerComponent.h"		//	プレイヤー操作
#include "GravityComponent.h"		//	重力
#include "HomingComponent.h"		//	マウス追従
#include "TexScrollComponent.h"		//	テクスチャスクロール
#include "GaugeComponent.h"			//	ゲージ
#include "AnimationComponent.h"		//	アニメーション
#include "MenuComponent.h"			//	メニュー
#include "FadeComponent.h"			//	フェード
#include "FallComponent.h"			//	ゆらゆら重力
#include "WindComponent.h"			//	風
#include "SeedComponent.h"			//	種
#include "FollowCompomemt.h"		//	親子関係
#include "OffScreenComponent.h"		//	画面外処理
#include "SpiderWebComponent.h"		//	クモの巣
#include "Animation.h"
#include "GoMenuComponent.h"
#include "OffScreenComponent.h"
#include "CloverManager.h"
#include "Load.h"
#include "PointCom.h"
#include "EGravityComponent.h"
#include "SceneSelect.h"
#include "NumberCom.h"
#include "EffectManager.h"
#include "LotationComponent.h"
#include "ResourceCSV.h"
#include "FileManager.h"


#define SCENE_CHANGE_TIMER	(2120)

#define CLEAR_POS_X		(-420.0f)
#define CLEAR_POS_Y		(-50.0f)
#define CLEAR_SIZE_X	(200.0f)
#define CLEAR_SIZE_Y	(170.0f)


bool SceneEnding::m_bSpawnClover[8];

//=== グローバル変数
CGauge* g_pWindGauge =nullptr;
float g_fScroll = 0.0f;

// 2022/1/29 narita クローバーの数表示----------------------------------------------
CDrawNumber* g_pNum = nullptr;
//-----------------------------------------------------------------------------


//=== 静的メンバ変数
int	SceneEnding::m_nCurrentFrameCnt;	// 経過時間
int SceneEnding::m_nClovercnt = 0;
bool SceneEnding::m_bFinish;

// コンストラクタ
SceneEnding::SceneEnding()
	: m_pWindManager(nullptr), m_pCloverManager(nullptr)
{
}

// デストラクタ
SceneEnding::~SceneEnding(){
}

// 初期化
void SceneEnding::Init()
{
	//===== ロード画面のスレッドを作る =====
	Load::Begin();

	// カメラ初期化
	CCamera::Get()->Init();

	m_nCurrentFrameCnt = 0;
	g_fScroll = 0.0f;

	m_nClovercnt = 0;
	m_bFinish = false;

	m_nSceneChangeTimer = -1;

	//===== ゲームオブジェクトの初期化 =====

	//	管理クラスのインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();	// オブジェクトマネージャー
	m_pWindManager = WindManager::GetInstance();		// 風マネージャー
	m_pMenuManager = MenuManager::GetInstance();		// メニューマネージャー
	m_pCloverManager = CloverManager::GetInstance();

	/* テクスチャ読み込み */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// 風
	pTexManager->AddTexture(PATH_TEX_WIND, WIND_TEX_NUM);
	// エンドロール
	pTexManager->AddTexture(PATH_TEX_END_ROLL, END_ROLL_TEX_NUM);
	// タイトルへ
	pTexManager->AddTexture(PATH_TEX_SUUJI, SUUJI_TEX_NUM);

	pTexManager->AddTexture(PATH_TEX_THANK_FRAME, THANK_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_THANK, THANK_TEX_NUM);

	/* モデル読み込み */
	ModelManager* pModelManager = ModelManager::GetInstance();
	// シルフィ
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SPRING_GOAL, SPRING_GOAL_MODEL_NUM);
	// 四つ葉のクローバー
	pModelManager->AddModel(PATH_MODEL_END_CLOVER, END_CLOVER_MODEL_NUM);

	

	
#pragma region ---背景
	//	オブジェクト生成
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	objBG->AddComponent<CTransform>();				// 座標
	auto Draw_BG = objBG->AddComponent<CDraw2D>();	// 描画
	Draw_BG->SetColor(0.0f, 0.0f, 0.0f);			// 色
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// サイズ
	//	リストに追加
	m_pObjectManager->AddObject(objBG);
#pragma endregion

#pragma region	---プレイヤー
	//	オブジェクト生成
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	コンポーネントの追加
	auto TransPlayer = Player->AddComponent<CTransform>();			//	トランスフォーム
	auto DrawPlayer = Player->AddComponent<CDrawModel>();				//	モデル描画機能
	auto DrawClear = Player->AddComponent<CDrawModel>();				//	モデル描画機能(クリアアニメーション)
	auto animationPlayer = Player->AddComponent<CAnimation>();		//	fbx切り替え
	// 3Dリストに追加
	m_pObjectManager->AddObject(Player);
	TransPlayer->SetRotate(0.0f, -20.0f, 0.0f);
	DrawClear->SetModel(pModelManager->GetModel(SPRING_GOAL_MODEL_NUM));//	クリア
	TransPlayer->SetScale(5.0f, 5.0f, 5.0f);							//	スケール
	TransPlayer->SetPosition(-170.0f, -100.0f, PLAYER_POS_Z);	//	初期配置
	DrawClear->AddAnimation(PLAYER_GOAL, PLAYER_ANIM_GOAL_S, PLAYER_ANIM_GOAL_E, false);
	// fbx切り替え設定
	animationPlayer->AddAnimation(DrawPlayer, FBX_ONE);		//	待機、吹かれている、落ちる、拾う
	animationPlayer->AddAnimation(DrawClear, FBX_TWO);		//	ゴール
	animationPlayer->ChangeAnimation(FBX_ONE);				// 最初は1つ目のfbx
	DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
	// アニメーション設定
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);			// 待機
	DrawPlayer->AddAnimation(PLAYER_CARRIED, PLAYER_ANIM_CARRIED_S, PLAYER_ANIM_CARRIED_E, true);	// 風に吹かれている
	DrawPlayer->AddAnimation(PLAYER_FALL, PLAYER_ANIM_FALL_S, PLAYER_ANIM_FALL_E, true);			// 落ちる
	DrawPlayer->AddAnimation(PLAYER_GET, PLAYER_ANIM_GET_S, PLAYER_ANIM_GET_E, false);				// 拾う
	DrawPlayer->AddAnimation(PLAYER_BIND, PLAYER_ANIM_BIND_S, PLAYER_ANIM_BIND_E, true);			// クモの巣
	DrawPlayer->AddAnimation(PLAYER_GAMEOVER, PLAYER_ANIM_GAMEOVER_S, PLAYER_ANIM_GAMEOVER_E, false);// ゲームオーバー
	DrawPlayer->SetPlaySpeed(PLAYER_BIND, 0.012);	// クモの巣の再生速度を設定
	DrawPlayer->AlwaysAnimation(true);				// 常にアニメーション
	DrawPlayer->Play(PLAYER_IDLE);					// 再生
#pragma endregion


#pragma region ---風ゲージ
	//　風ゲージ
	Object* pWindBar = new Object("Gauge", UPDATE_UI, DRAW_UI);
	auto pUI_WinGauge = pWindBar->AddComponent<CGauge>();					//	風ゲージ
	//　オブジェクトの設定
	pUI_WinGauge->SetMaxGauge(WIMD_GAUGE_MAX);								//	最大ゲージ量
	//	リストに追加
	m_pObjectManager->AddObject(pWindBar);
#pragma endregion

#pragma region ---スタッフロール
	Object* pEndRoll = new Object("endroll", UPDATE_UI, DRAW_UI);
	// components
	pEndRoll->AddComponent<CTransform>();
	auto drawEndRoll = pEndRoll->AddComponent<CDraw2D>();	// スクリーン描画
	auto Scroll = pEndRoll->AddComponent<CTexScroll>();		// スクロール量
	// settings
	drawEndRoll->SetTexture(pTexManager->GetTexture(END_ROLL_TEX_NUM)); // テクスチャ
	drawEndRoll->SetSize(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT);	// サイズ
	drawEndRoll->SetUVsize(1.0f, 1.0f / 5);
	drawEndRoll->SetAlpha(0.8f);								// アルファ
	Scroll->SetAxizY(&g_fScroll);
	// add to list
	m_pObjectManager->AddObject(pEndRoll);
#pragma endregion


	// 2022/1/29 narita クローバーの数表示----------------------------------------------
#pragma region ---数字
	Object* pNum = new Object(CLOVER_NUMBER_NAME, UPDATE_UI, DRAW_UI);
	// components
	//	コンポーネントの追加
	auto transNum = pNum->AddComponent<CTransform>();	//	トランスフォーム
	auto Number = pNum->AddComponent<CDrawNumber>();
	// settings
	transNum->SetPosition(-420.0f, 280.0f);
	Number->SetSize(120.0f, 120.0f);
	Number->SetTexture(pTexManager->GetTexture(SUUJI_TEX_NUM));
	Number->SetNum(0);
	// add to list
	m_pObjectManager->AddObject(pNum);
#pragma endregion
	//-----------------------------------------------------------------------------


#pragma region	---カーソル
	XMFLOAT2 MousePos = INPUT->GetMousePos();
	//	オブジェクト生成
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transCursor = pCursor->AddComponent<CTransform>();	//	トランスフォーム
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D描画機能
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	マウス追従
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	衝突処理
	pCursor->AddComponent<CLotation>();							// 回転
	// オブジェクト設定
	DrawCursor->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(1.0f,1.0f, 1.0f);						//	色
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	座標
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	homingCursor->SetEffectNum(CURSOR_EFFECT_SPRING_NUM);
	// メニューリストに追加
	m_pObjectManager->AddObject(pCursor);
#pragma endregion


	// 注視点は中央
	CCamera::Get()->SetAxisX(nullptr);
	CCamera::Get()->SetLimit(XMFLOAT2(0, 0));
	CTexScroll::Scroll(true);

	// 風ゲージセット
	WindManager::GetInstance()->SetWindGauge(pUI_WinGauge);
	g_pWindGauge = pUI_WinGauge;

	// 風を発生させれるようにする
	m_pWindManager->SetSpawnWind(true);

	// エンディングBGM
	CSound::Play(BGM_ENDING);


	/*--------- クローバー取得状況に応じて出現するクローバーの数を変える ----------*/
	SceneSelect::Column CloverInfo;			
	CloverInfo = SceneSelect::GetClovers();	// クローバーの取得状況
	
	for (int i = 0; i < 8; i++)
	{
		m_bSpawnClover[i] = false;
	}

	// 4つ全て取得していたらクローバーを出す
	if (CloverInfo[0]  == 1 && CloverInfo[1]  == 1 && CloverInfo[2]  == 1 && CloverInfo[3]  == 1) m_bSpawnClover[0] = true;
	if (CloverInfo[4]  == 1 && CloverInfo[5]  == 1 && CloverInfo[6]  == 1 && CloverInfo[7]  == 1) m_bSpawnClover[1] = true;
	if (CloverInfo[8]  == 1 && CloverInfo[9]  == 1 && CloverInfo[10] == 1 && CloverInfo[11] == 1) m_bSpawnClover[2] = true;
	if (CloverInfo[12] == 1 && CloverInfo[13] == 1 && CloverInfo[14] == 1 && CloverInfo[15] == 1) m_bSpawnClover[3] = true;
	if (CloverInfo[16] == 1 && CloverInfo[17] == 1 && CloverInfo[18] == 1 && CloverInfo[19] == 1) m_bSpawnClover[4] = true;
	if (CloverInfo[20] == 1 && CloverInfo[21] == 1 && CloverInfo[22] == 1 && CloverInfo[23] == 1) m_bSpawnClover[5] = true;
	if (CloverInfo[24] == 1 && CloverInfo[25] == 1 && CloverInfo[26] == 1 && CloverInfo[27] == 1) m_bSpawnClover[6] = true;
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	//CSV->GetInt(
	if (CSV->GetInt(28, 0)== 1 && CSV->GetInt(29, 0) == 1 && CSV->GetInt(30, 0) == 1 && CSV->GetInt(31, 0) == 1) m_bSpawnClover[7] = true;



	/*----------------------------------------------------------------*/
	
	

	//===== ロード終了処理 =====
	Load::End();
}

// 終了処理
void SceneEnding::Uninit()
{
	g_pNum = nullptr;

	CSound::Stop(BGM_ENDING);

	//	オブジェクトの終了
	m_pObjectManager->Uninit();

	//	メニューの終了処理
	m_pMenuManager->DeleteMenu();
	MenuManager::Destroy();

	//	風の終了処理
	m_pWindManager->Destroy();

	// クローバーマネージャー終了処理
	m_pCloverManager->Destroy();
}

// 更新
void SceneEnding::Update()
{
	// 2022/1/29 narita クローバーの数表示----------------------------------------------
	if (g_pNum == nullptr)
	{
		g_pNum = m_pObjectManager->GetGameObject(CLOVER_NUMBER_NAME)->GetComponent<CDrawNumber>();
	}
	else
	{
		g_pNum->SetNum(m_nClovercnt);
	}
	//-----------------------------------------------------------------------------

	// スクロール量加算
	g_fScroll = m_nCurrentFrameCnt / (float)ENDING_FRAME;

	// 風ゲージを常に最大にする
	CGauge::SetGauge(g_pWindGauge, 10000);

	//	カメラ更新
	CCamera::Get()->Update();

	//	メニューの更新
	m_pMenuManager->Update();

	if (!m_bFinish)
	{
		// 風の更新
		m_pWindManager->Update();

		// クローバーマネージャーの更新
		m_pCloverManager->Update();


		// カウント
		m_nCurrentFrameCnt++;
	}

	// ゲームオブジェクトの更新
	m_pObjectManager->Update();

	// 終了確認
	if (m_nCurrentFrameCnt > ENDING_FRAME)
	{
		// カウンタリセット
		m_nCurrentFrameCnt = 0;

		// シーン切り替えタイマー始動
		m_nSceneChangeTimer = SCENE_CHANGE_TIMER;

		// 終わり
		m_bFinish = true;

		// メニューをつくる
#pragma region  ---タイトル
		//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
		//セレクトの枠----------------------------------------------------------------
		//オブジェクトの作成
		Object* pTitle_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
		//コンポーネントの追加
		auto TransTitle_Frame = pTitle_Frame->AddComponent<CTransform>();				//トランスフォーム
		auto DrawTitle_Frame = pTitle_Frame->AddComponent<CDraw2D>();					//描画
		//オブジェクトの設定
		TransTitle_Frame->SetPosition(MENU_POS_X + 350.0f, MENU_POS_Y - 250.0f);	//座標の設定
		DrawTitle_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_FRAME_TEX_NUM));
		DrawTitle_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
		//メニューリストに追加
		m_pObjectManager->AddObject(pTitle_Frame);
		//----------------------------------------------------------------------------

		Object* pTitle = new Object(TITLE_NAME, UPDATE_UI, DRAW_UI);
		//	コンポーネントの追加
		auto transTitle = pTitle->AddComponent<CTransform>();		// トランスフォーム
		auto DrawTitle = pTitle->AddComponent<CDraw2D>();			//	2D描画機能
		auto ColliderTitle = pTitle->AddComponent<CCollider>();		//	衝突処理
		auto Title = pTitle->AddComponent<CMenu>();					//	マウス追従
		// オブジェクトの設定
		transTitle->SetPosition(MENU_POS_X + 350.0f, MENU_POS_Y - 250.0f);//	座標 ここを-200.0fにしてください
		DrawTitle->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_TEX_NUM));//	テクスチャ
		DrawTitle->SetSize(MENU_WIDTH, MENU_HEIGHT);
		ColliderTitle->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	あたり判定の大きさ
		// メニューリストに追加
		m_pObjectManager->AddObject(pTitle);
#pragma endregion

#pragma region ---ゲームクリアおめでとう
		Object* pMessage = new Object("", UPDATE_UI, DRAW_UI);
		// components
		auto trans = pMessage->AddComponent<CTransform>();	// オブジェクトの座標
		auto draw  = pMessage->AddComponent<CDraw2D>();		// スクリーン描画
		// settings
		trans->SetPosition(CLEAR_POS_X, CLEAR_POS_Y);		// 座標
		draw->SetTexture(TextureManager::GetInstance()->GetTexture(THANK_TEX_NUM));// テクスチャ
		draw->SetSize(CLEAR_SIZE_X, CLEAR_SIZE_Y);			// 大きさ
		// add to list
		m_pObjectManager->AddObject(pMessage);
#pragma endregion

#pragma region ---ゲームクリアおめでとう(枠)
		Object* pMessageFrame = new Object("", UPDATE_UI, DRAW_UI_FRAME);
		// components
		auto transFrame = pMessageFrame->AddComponent<CTransform>();	// オブジェクトの座標
		auto drawFrame = pMessageFrame->AddComponent<CDraw2D>();		// スクリーン描画
		// settings
		transFrame->SetPosition(CLEAR_POS_X - 5.0f, CLEAR_POS_Y - 8.0f);		// 座標
		drawFrame->SetTexture(TextureManager::GetInstance()->GetTexture(THANK_FRAME_TEX_NUM));// テクスチャ
		drawFrame->SetSize(CLEAR_SIZE_X + 140.0f, CLEAR_SIZE_Y + 115.0f);			// 大きさ
		// add to list
		m_pObjectManager->AddObject(pMessageFrame);
#pragma endregion

		// クローバーの重力をなくす
		std::list<Object*> Clovers;
		m_pObjectManager->GetGameObjects(ENDING_CLOVER_NAME, Clovers);
		float PosX = -80.0f;
		float PosY = 50.0f;
		int CloverCnt = 0;

		for (auto&& clover : Clovers)
		{
			CloverCnt++;

			// 重力のコンポーネントを消す
			auto gravity = clover->GetComponent<CEGravity>();
			if (gravity) gravity->Delete();

			// 目的座標に向かわせる
			auto dest = clover->AddComponent<CPoint>();
			dest->SetDestPos(XMFLOAT2(PosX, PosY));
			dest->Start();

			PosX += 60.0f;

			// 4つ目以降は下の段
			if (CloverCnt == 4)
			{
				PosX = -80.0f;
				PosY -= 75.0f;
			}
		}

		// se ミニゲーム終了
		CSound::Play(SE_FINISH);

	}

	if (m_nSceneChangeTimer > 0)
	{
		m_nSceneChangeTimer--;

		if (m_nSceneChangeTimer <= 0)
		{
			StartFadeOutSceneChange(SCENE_TITLE);
		}
	}
}

// 描画
void SceneEnding::Draw()
{
	//	ゲームオブジェクトの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

	// メニューオブジェクトの描画
	m_pMenuManager->Draw();

}

// 経過時間の取得
int SceneEnding::GetCurrentFrameCnt()
{
	return m_nCurrentFrameCnt;
}

void SceneEnding::Cntplus()
{
	m_nClovercnt++;
}

void SceneEnding::Cntminus()
{
	m_nClovercnt--;
}

// 2022/1/30 ミニゲーム終了フラグ-------------------------
// trueだった場合はミニゲームが終了する
bool SceneEnding::Finish()
{
	return m_bFinish;
}
//---------------------------------------------------