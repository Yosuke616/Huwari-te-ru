//===== インクルード =====
#include "sceneGame.h"
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
#include "TutorialComponent.h"
#include "EffectManager.h"

#include "SceneSummer.h"
#include "SceneSpring.h"
#include "SceneFall.h"
#include "SceneWinter.h"

#include "Load.h"

//=== 静的メンバ変数
SceneGame* SceneGame::m_pInstance = nullptr;
// 仮措置
std::unordered_map<CAssimpModel*, bool> SceneGame::IceManager;	// 氷の数

CLeaf g_Leaf;

// コンストラクタ
SceneGame::SceneGame() 
	: m_pWindManager(nullptr), m_pStageManager(nullptr)
	, m_bPauseMode(false), m_bEditMode(false)
{
	//	全シーンをリストに追加
	m_GameSceneList.push_back(new SceneSpring());	// 春１
	m_GameSceneList.push_back(new SceneSpring());	// 春２
	m_GameSceneList.push_back(new SceneSummer());	// 夏１
	m_GameSceneList.push_back(new SceneSummer());	// 夏２
	m_GameSceneList.push_back(new SceneFall());		// 秋１
	m_GameSceneList.push_back(new SceneFall());		// 秋２
	m_GameSceneList.push_back(new SceneWinter());	// 冬１
	m_GameSceneList.push_back(new SceneWinter());	// 冬２

	// 最初は春
	m_eSeasonState = SPRING_1;
	m_CurrentScene = m_GameSceneList[m_eSeasonState];
}

// デストラクタ
SceneGame::~SceneGame()
{
	//	シーンリストを削除
	for (auto scene : m_GameSceneList)
		delete scene;

	//	リストを空にする
	m_GameSceneList.clear();

	// 仮措置
	IceManager.clear();
}

// 初期化
void SceneGame::Init()
{
	//===== ロード画面のスレッドを作る =====
	Load::Begin();
	
	// カメラ初期化
	CCamera::Get()->Init();

	m_bEditMode = false;	// 編集モードフラグOFF
	m_bPauseMode = false;	// ポーズフラグOFF

	//===== ゲームオブジェクトの初期化 =====

	//	管理クラスのインスタンス取得
	m_pObjectManager	= ObjectManager::GetInstance();		// オブジェクトマネージャー
	m_pWindManager		= WindManager::GetInstance();		// 風マネージャー
	m_pStageManager		= StageManager::GetInstance();		// ステージマネージャー
	m_pMenuManager		= MenuManager::GetInstance();		// メニューマネージャー

	/* テクスチャ読み込み */
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_WIND, WIND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WIND_GAUGE, WIND_GAUGE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_FRAME_GAUGE, FRAME_GAUGE_TEX_NUM);

	// 種、クローバーのUI
	pTexManager->AddTexture(PATH_SEED_UI, SEED_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI, CLOVER_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_SEED_UI_FRAME, SEED_UI_FRAME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI_FRAME, CLOVER_UI_FRAME_TEX_NUM);
	
	// 蜘蛛の巣
	pTexManager->AddTexture(PATH_TEX_SPIDER, SPIDER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPIDER_TWO, SPIDER_TWO_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPIDER_THREE, SPIDER_THREE_TEX_NUM);
	// 落ち葉
	pTexManager->AddTexture(PATH_TEX_OTIBA, OTIBA_TEX_NUM);
	// ふってくるもの
	pTexManager->AddTexture(PATH_SUMMER_LEAF, SUMMER_LEAF_TEX_NUM);	// 夏
	pTexManager->AddTexture(PATH_FALL_LEAF, FALL_LEAF_TEX_NUM);		// 秋
	pTexManager->AddTexture(PATH_TEX_SPRING_LEAF, LEAF_SPRING_NUM);	// 春
	// 種、クローバーの取得エフェクト
	pTexManager->AddTexture(PATH_TEX_EFFECT_CLOVER, EFFECT_CLOVER_TEX_NUM);	// クローバー
	// 季節表示
	pTexManager->AddTexture(PATH_TEX_SPRING_1,		 SPRING_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SPRING_2,		 SPRING_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SUMMER_1,		 SUMMER_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SUMMER_2,		 SUMMER_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_AUTUMN_1,		 AUTUMN_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_AUTUMN_2,		 AUTUMN_2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WINTER_1,		 WINTER_1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_WINTER_2,		 WINTER_2_TEX_NUM);

	//2022/1/26/WindCar Shimizu Yosuke ----------------------------------------------------------
	//風車
	pTexManager->AddTexture(PATH_TEX_WIND_CAR, WIND_CAR_TEX_NUM);
	//棒
	pTexManager->AddTexture(PATH_TEX_WINDCAR_STICK, WINDCAR_STICK_TEX_NUM);
	//-------------------------------------------------------------------------------------------

	pTexManager->AddTexture(PATH_TEX_TUTORIAL_WINTER, TUTORIAL_WINTER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_FALL, TUTORIAL_FALL_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_SPIDER, TUTORIAL_SPIDER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TUTORIAL_UI, TUTORIAL_UI_TEX_NUM);

	//2022/1/31/Load_Tutorial Shimizu Yosuke ----------------------------------------
	pTexManager->AddTexture(PATH_PLAYER_TUTORIAL, PLAYER_TUTORIAL_TEX_NUM);
	//-------------------------------------------------------------------------------

	/* モデル読み込み */
	ModelManager* pModelManager = ModelManager::GetInstance();
	pModelManager->AddModel(PATH_MODEL_CLOVER, CLOVER_MODEL_NUM);				// クローバー
	pModelManager->AddModel(PATH_MODEL_SEED, SEED_MODEL_NUM);					// たね
	pModelManager->AddModel(PATH_MODEL_POT, POT_MODEL_NUM);						// 植木鉢
	pModelManager->AddModel(PATH_MODEL_ICICLE, ICICLE_MODEL_NUM);				// つらら
	// アニメーションがあって複数を同じシーンで描画しないといけないものはその分のインスタンスを作る
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM1);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM2);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM3);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM4);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM5);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM6);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM7);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM8);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM9);		// 氷
	pModelManager->AddModel(PATH_MODEL_ICE_BLOCK, ICE_BLOCK_MODEL_NUM10);		// 氷
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = false;
	IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = false;


	


#pragma region	---プレイヤー
	//	オブジェクト生成
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	コンポーネントの追加
	auto TransPlayer = Player->AddComponent<CTransform>();			//	トランスフォーム
	auto ColliderPlayer = Player->AddComponent<CCollider>();		//	衝突機能
	auto DrawPlayer = Player->AddComponent<CDrawModel>();				//	モデル描画機能
	auto DrawClear = Player->AddComponent<CDrawModel>();				//	モデル描画機能(クリアアニメーション)
	auto OffScreenPlayer = Player->AddComponent<COffScreen>();		//	画面外処理
	auto pPlayer = Player->AddComponent<CPlayer>();					//	プレイヤー機能
	auto FallPlayer = Player->AddComponent<CFall>();				//	重力
	auto animationPlayer = Player->AddComponent<CAnimation>();		//	fbx切り替え
	// 3Dリストに追加
	m_pObjectManager->AddObject(Player);
#pragma endregion

	//季節によって変わるオブジェクト初期化(ここで季節ごとのモデルなど読み込んでる
	m_CurrentScene->Init();

#pragma region	プレイヤー情報の設定
	TransPlayer->SetScale(3.0f, 3.0f, 3.0f);							//	スケール
	TransPlayer->SetVelocity(2.0f, 0.0f, 0.0f);							//	スケール
	TransPlayer->SetPosition(PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z);	//	初期配置
	ColliderPlayer->SetCollisionSize(PLAYER_SIZE_X, PLAYER_SIZE_Y);		//	あたり判定の大きさ
	ColliderPlayer->SetOffset(PLAYER_OFFSET_X, PLAYER_OFFSET_Y);		//	中心座標のオフセット
	OffScreenPlayer->SetFinction(ON_SCREEN);							//	画面内に入れる処理
	// fbx切り替え設定
	animationPlayer->AddAnimation(DrawPlayer, FBX_ONE);		// 待機、吹かれている、落ちる、拾う
	animationPlayer->AddAnimation(DrawClear, FBX_TWO);		// ゴール
	animationPlayer->ChangeAnimation(FBX_ONE);				// 最初は1つ目のfbx
	// 季節によってプレイヤーのテクスチャを変える
	switch (m_eSeasonState)
	{
	case SPRING_1:
	case SPRING_2:	// 春のテクスチャ
		DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(SPRING_GOAL_MODEL_NUM));	// クリア
		break;
	case SUMMER_1:
	case SUMMER_2:	// 夏のテクスチャ
		DrawPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(SUMMER_GOAL_MODEL_NUM));	// クリア
		break;
	case FALL_1:
	case FALL_2:	// 秋テクスチャ
		DrawPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(FALL_GOAL_MODEL_NUM));		// クリア
		break;
	case WINTER_1:
	case WINTER_2:	// 冬テクスチャ
		DrawPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));
		DrawClear->SetModel(pModelManager->GetModel(WINTER_GOAL_MODEL_NUM));	// クリア
		break;
	case MAX_SEASON:	break;
	default:			break;
	}

	// アニメーション設定
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);			// 待機
	DrawPlayer->AddAnimation(PLAYER_CARRIED, PLAYER_ANIM_CARRIED_S, PLAYER_ANIM_CARRIED_E, true);	// 風に吹かれている
	DrawPlayer->AddAnimation(PLAYER_FALL, PLAYER_ANIM_FALL_S, PLAYER_ANIM_FALL_E, true);			// 落ちる
	DrawPlayer->AddAnimation(PLAYER_GET, PLAYER_ANIM_GET_S, PLAYER_ANIM_GET_E, false);				// 拾う
	DrawPlayer->AddAnimation(PLAYER_BIND, PLAYER_ANIM_BIND_S, PLAYER_ANIM_BIND_E, true);			// クモの巣
	DrawPlayer->AddAnimation(PLAYER_GAMEOVER, PLAYER_ANIM_GAMEOVER_S, PLAYER_ANIM_GAMEOVER_E, false);// ゲームオーバー
	DrawPlayer->SetPlaySpeed(PLAYER_BIND, 0.012);	// クモの巣の再生速度を設定
	DrawPlayer->AlwaysAnimation(true);				// 常にアニメーション
	DrawPlayer->Play(PLAYER_BIND);					// 再生
	// ゴールアニメーション
	DrawClear->AddAnimation(PLAYER_GOAL, PLAYER_ANIM_GOAL_S, PLAYER_ANIM_GOAL_E, false);
#pragma endregion

	//--- すべてのシーンに共通するオブジェクトの生成
#pragma region ---ビネット
//	オブジェクト生成
	Object* objVinetto = new Object("Vinetto", UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	objVinetto->AddComponent<CTransform>();								// 座標
	auto Draw_Vinetto = objVinetto->AddComponent<CDraw2D>();			// 描画
	//	オブジェクトの設定
	Draw_Vinetto->SetTexture(pTexManager->GetTexture(VINETTO_TEX_NUM));	// テクスチャ
	Draw_Vinetto->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);					// サイズ
	//	リストに追加
	m_pObjectManager->AddObject(objVinetto);							// オブジェクトの追加
#pragma endregion


	// Playerを注視点に設定
	CCamera::Get()->SetAxisX(&(Player->GetComponent<CTransform>()->Pos.x));

#pragma region ---風ゲージ
	//　風のトランスフォームと描画、風のゲージをのコンポーネントを追加してやる
	Object* pWindBar = new Object("Gauge", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto pUI_WinTransform = pWindBar->AddComponent<CTransform>();			//	座標
	auto pUI_WinDraw = pWindBar->AddComponent<CDrawEffect2D>();				//	スクリーン描画
	auto pUI_WinGauge = pWindBar->AddComponent<CGauge>();					//	風ゲージ
	//　オブジェクトの設定
	pUI_WinTransform->SetPosition(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);		//　座標
	pUI_WinDraw->SetTexture(pTexManager->GetTexture(WIND_GAUGE_TEX_NUM));	//　テクスチャ
	pUI_WinDraw->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);				//　サイズ
	pUI_WinDraw->SetAlpha(0.8f);
	pUI_WinGauge->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);				//	ゲージのサイズ
	pUI_WinGauge->SetInitPos(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);			//	初期の座標
	pUI_WinGauge->SetMaxGauge(WIMD_GAUGE_MAX);								//	最大ゲージ量
	pUI_WinDraw->SetAnimSplit(4, 4);
	pUI_WinDraw->SetLoop(true);
	pUI_WinDraw->SetSwapFrame(1);
	//	リストに追加
	m_pObjectManager->AddObject(pWindBar);
#pragma endregion

#pragma region ---風ゲージの枠
	//	オブジェクト生成
	Object* pGaugeFrame = new Object("frame", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto TransFrame = pGaugeFrame->AddComponent<CTransform>();			//	座標
	auto DrawFrame = pGaugeFrame->AddComponent<CDrawEffect2D>();		//	スクリーン描画
	//	オブジェクトの設定
	TransFrame->SetPosition(WIND_GAUGE_POS_X, WIND_GAUGE_POS_Y);		//	座標
	DrawFrame->SetTexture(pTexManager->GetTexture(FRAME_GAUGE_TEX_NUM));//	テクスチャ
	DrawFrame->SetSize(WIND_GAUGE_WIDTH, WIND_GAUGE_HEIGHT);			//	サイズ
	DrawFrame->SetAnimSplit(4, 4);
	DrawFrame->SetLoop(true);
	DrawFrame->SetSwapFrame(1);
	//	リストに追加
	m_pObjectManager->AddObject(pGaugeFrame);
#pragma endregion

//#pragma region	---カーソル
//	//	オブジェクト生成
//	Object* objCursor = new Object(CURSOR_NAME, UPDATE_DEBUG, DRAW_DEBUG);
//	//	コンポーネントの追加
//	objCursor->AddComponent<CTransform>();					//	トランスフォーム
//	auto Draw_cursor = objCursor->AddComponent<CDraw2D>();	//	2D描画機能
//	auto homing = objCursor->AddComponent<CHoming>();		//	マウス追従
//	auto collider = objCursor->AddComponent<CCollider>();	//	あたり判定
//	//	オブジェクトの設定
//	Draw_cursor->SetTexture(nullptr);						//	テクスチャ
//	Draw_cursor->SetSize(30.0f, 30.0f);						//	サイズ
//	Draw_cursor->SetAlpha(0.5f);
//	Draw_cursor->SetColor(0.5f, 1.0f, 0.5f);
//	collider->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);
//	//	リストに追加
//	m_pObjectManager->AddObject(objCursor);
//
//#ifdef NO_SHOW
//	Draw_cursor->SetSize(0.0f, 0.0f);			//	サイズ
//#endif // _RELEASE
//#pragma endregion

#pragma region ---UI枠
	for (int i = 0; i < 4; i++)
	{
		//クローバーのUIを追加する
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI_FRAME);
		//コンポーネントをついかする
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
		//オブジェクトの設定
		//ここでポジションの設定をする
		switch (i)
		{
		case 0:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}

		DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);

		//リストに追加
		ObjectManager::GetInstance()->AddObject(pUI_Clover);
	}
	
	//オブジェクトの生成
	Object* pSeed_UI = new Object(SEED_UI_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//  コンポネントの追加
	auto TransSeed_UI = pSeed_UI->AddComponent<CTransform>();			//　座標
	auto DrawSeed_UI = pSeed_UI->AddComponent<CDraw2D>();				//　描画
	//  オブジェクトの設定
	TransSeed_UI->SetPosition(SEED_UI_POS_X, SEED_UI_POS_Y);			//  座標
	DrawSeed_UI->SetTexture(TextureManager::GetInstance()->GetTexture(SEED_UI_FRAME_TEX_NUM));	//  テクスチャのセット
	DrawSeed_UI->SetSize(SEED_UI_SIZE_X, SEED_UI_SIZE_Y);				//  サイズ
	DrawSeed_UI->SetColor(1.0f, 1.0f, 1.0f);
	//  リストに追加
	ObjectManager::GetInstance()->AddObject(pSeed_UI);

#pragma endregion

	//2022/1/12 Shimizu Yosuke -----------------------------------
#pragma region ----ポーズボタン
	//ポーズ画面用のボタン
	//オブジェクトの作成
	Object* pStop_Game = new Object(STOP_GAME_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//コンポネントの作成
	auto TransGame_Stop = pStop_Game->AddComponent<CTransform>();								//　トランスフォーム
	auto DrawGame_Stop = pStop_Game->AddComponent<CDraw2D>();									//　描画
	auto ColliderGame_Stop = pStop_Game->AddComponent<CCollider>();								//　当たり判定
	pStop_Game->AddComponent<CMenu>();														//  メニューに行くためのコンポネント
	//オブジェクトの設定
	TransGame_Stop->SetPosition(STOP_GAME_POS_X, STOP_GAME_POS_Y);								//　座標
	DrawGame_Stop->SetSize(STOP_GAME_SIZE_X, STOP_GAME_SIZE_Y);									//　サイズ
	DrawGame_Stop->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));	//　テクスチャ
	ColliderGame_Stop->SetCollisionSize(STOP_GAME_COLLSION_WIDTH, STOP_GAME_COLLSION_HEIGHT);	//　当たり判定のサイズ
	//リストに追加
	ObjectManager::GetInstance()->AddObject(pStop_Game);
#pragma endregion
	//------------------------------------------------------------


	// ポーズボタンの下に追加
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------
#pragma region ---チュートリアルボタン
	//オブジェクトの生成
		Object* pTutorial_UI = new Object(WINDCAR_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//コンポーネントの作成
	auto TransTutorial_UI = pTutorial_UI->AddComponent<CTransform>();									//  トランスフォーム
	auto DrawTutorial_UI = pTutorial_UI->AddComponent<CDraw2D>();										//　描画
	auto CollTutorial_UI = pTutorial_UI->AddComponent<CCollider>();										//  衝突判定
	pTutorial_UI->AddComponent<CMenu>();																//  メニュー用のコンポーネント
	pTutorial_UI->AddComponent<CTutorial>();															//  チュートリアル
	//オブジェクトの設定
	TransTutorial_UI->SetPosition(WINDCAR_POS_X, WINDCAR_POS_Y);											//  座標
	DrawTutorial_UI->SetSize(WINDCAR_SIZE_X, WINDCAR_SIZE_Y);											//  サイズ
	DrawTutorial_UI->SetTexture(TextureManager::GetInstance()->GetTexture(WIND_CAR_TEX_NUM));																//　テクスチャ
	//DrawTutorial_UI->SetColor(1.0f,0.0f,1.0f);														//  カラー
	CollTutorial_UI->SetCollisionSize(WINDCAR_COLL_WIDTH, WINDCAR_COLL_HEIGHT);							//  衝突判定の大きさ
	//リストに追加
	ObjectManager::GetInstance()->AddObject(pTutorial_UI);
#pragma endregion
	//----------------------------------------------------------------

	//2022/1/26/Touch_Totorial Shimizu Yosuke ------------------------
#pragma region ---チュートリアルスティック
	//オブジェクトの生成
	Object* pStick = new Object(WINDCAR_STICK_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//コンポーネントの追加
	auto TransStick = pStick->AddComponent<CTransform>();												//  トランスフォーム
	auto DrawStick = pStick->AddComponent<CDraw2D>();													//  描画
	//オブジェクトの設定
	TransStick->SetPosition(WINDCAR_STICK_POS_X, WINDCAR_STICK_POS_Y);									//　座標
	DrawStick->SetSize(WINDCAR_STICK_SIZE_X, WINDCAR_STICK_SIZE_Y);										//  大きさ
	DrawStick->SetTexture(TextureManager::GetInstance()->GetTexture(WINDCAR_STICK_TEX_NUM));																		//  テクスチャ
	//リストの追加
	ObjectManager::GetInstance()->AddObject(pStick);
#pragma endregion
	//----------------------------------------------------------------


#pragma region	---ステージ
	m_pStageManager->CreateStage(m_eSeasonState);
#pragma endregion


	// ウィンドマネージャーにゲージを設定する
	m_pWindManager->SetWindGauge(pUI_WinGauge);

	//===== ロード終了処理 =====
	Load::End();

	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	MenuManager::GetInstance()->SetMenu(STATE_NONE);
	//------------------------------------------------------------------
}

// 終了処理
void SceneGame::Uninit()
{
	//季節によって変わる-----------------
	m_CurrentScene->Uninit();

	//	オブジェクトの終了
	m_pObjectManager->Uninit();

	//	ステージの終了処理
	StageManager::Destroy();

	//	メニューの終了処理
	MenuManager::Destroy();

	//	風の終了処理
	m_pWindManager->Destroy();
}

// 更新
void SceneGame::Update()
{
	//	カメラ更新
	CCamera::Get()->Update();

	//	ステージの更新
	m_pStageManager->Update();

	//	メニューの更新
	m_pMenuManager->Update();

	// 季節の更新
	m_CurrentScene->Update();

	//2022/1/26/Touch_Tutorial Shimizu Yosuke ---------------------------
	//ここで一回だけチュートリアルを出せるようにする
	if (m_bTutorial) {
		SceneGame::GetInstance()->m_bPauseMode = true;

		//チュートリアルに行くやつを作る
		MenuManager::GetInstance()->CreateTutorialMenu();

		//オブジェクトの更新を止める
		ObjectManager::GetInstance()->NoFunction();

		m_bTutorial = false;
	}
	//-------------------------------------------------------------------

	//--- ゲームモード
	if (m_bEditMode == false)
	{
		if (m_bPauseMode == false)
		{
			// 風の更新
			m_pWindManager->Update();

			// ふってくるもの更新
			g_Leaf.Update();
		}

		// ゲームオブジェクトの更新
		m_pObjectManager->Update();

	}
	//--- 編集モード
	else if(m_bEditMode == true)
	{
		// 編集の更新
		m_pObjectManager->UpdateEdit();
	}

#ifdef _DEBUG
	//	編集モードの切り替え
	if (INPUT->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// 切り替え

		CSound::Play(SE_PAUSE);	// ←これ追加

		//	切り替えと同時にカメラの注視点の変更
		if (m_bEditMode == true)
		{
			//edit	プレイヤーから注視店を外す
			CCamera::Get()->SetAxisX(nullptr);

			// すべてのオブジェクトの描画（更新は無し）
			ObjectManager::GetInstance()->BeginEdit();
		}
		else
		{
			// オブジェクトの更新の再開
			ObjectManager::GetInstance()->EndEdit();

			//game	プレイヤーを注視店に設定
			CCamera::Get()->SetAxisX(
			&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);
		}
	}
#endif // _DEBUG
	

	//	ポーズ画面の切り替え
	//if (INPUT->GetKeyTrigger(DIK_ESCAPE))
	//{
	//	m_bPauseMode = m_bPauseMode ^ 1;	// 切り替え
	//
	//	// ポーズ画面の場合
	//	if (m_bPauseMode == true)
	//	{
	//		m_pObjectManager->NoFunction();
	//		m_pMenuManager->CreatePauseMenu();	// ポーズ画面生成
	//	}
	//	else
	//	{
	//		m_pObjectManager->Resume();
	//		m_pMenuManager->DeleteMenu();		// メニューオブジェクトを空にする
	//	}
	//}
}

// 描画
void SceneGame::Draw()
{
	//	ゲームオブジェクトの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);


	//--- ポーズ画面
	//if (m_bPauseMode)
	//{
	//}
	//	ポーズ画面の描画
	m_pMenuManager->Draw();


#ifdef _DEBUG
	//	debug
	CCamera::Get()->Draw();
	//m_pWindManager->Draw();
	m_pStageManager->Draw();

	using namespace ImGui;
	
	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	// オブジェクトの数
	Text("Objects   : %d", m_pObjectManager->GetDrawList().size());
	// コライダーリスト
	Text("Colliders : %d", (int)CCollider::GetColliderList().size());
	// メニューの数
	Text("Menu      : %d", m_pMenuManager->GetMenuList().size());
	// ロードフレーム
	Text("LoadFrame : %d", Load::GetLoadCount());
	// 風ゲージ
	Text("Wind      : %d", WindManager::GetInstance()->GetWindGauge());

	End();
#endif // _DEBUG
}

// 季節のシーン切り替え
void SceneGame::SetSeason(Season NextSeason)
{
	//	例外処理
	if (NextSeason >= MAX_SEASON)
	{
		MessageBox(GetMainWnd(), _T("季節切り替え失敗"), NULL, MB_OK);
		return;
	}

	//	現在の季節の終了
	if (m_CurrentScene)
		m_CurrentScene->Uninit();

	//	季節切り替え
	m_CurrentScene = m_GameSceneList[NextSeason];	//	次の季節へ
	m_eSeasonState = NextSeason;					//	季節の設定

	StartFadeOutSceneChange(SCENE_GAME);
}

// 季節情報の取得
Season SceneGame::GetSeason()
{
	return m_eSeasonState;
}

// インスタンス取得
SceneGame* SceneGame::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneGame();
	}

	return m_pInstance;
}

// インスタンス破棄
void SceneGame::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//一番下に追加
//2022/1/26/Touch_Tutorial Shimizu Yosuke -----------------------------
void SceneGame::SetTutorial(bool bTutorial) {
	m_bTutorial = bTutorial;
}
//---------------------------------------------------------------------