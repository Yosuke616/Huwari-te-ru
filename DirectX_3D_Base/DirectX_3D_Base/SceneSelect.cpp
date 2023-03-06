//===== インクルード　=====
#include "SceneSelect.h"
#include "fade.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "imgui.h"
#include "Sound.h"
#include "Camera.h"

#include "Component.h"
#include "ColliderComponent.h"		//	あたり判定
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "HomingComponent.h"
#include "MenuComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "MenuManager.h"
#include "ModelManager.h"
#include "Draw3dComponent.h"
#include "CreateLeaf.h"
#include "PreviewComponent.h"
#include "Scale2DCom.h"

#include "GetUIComponent.h"
#include "Load.h"


//===== マクロ定義 =====
#define WIDTH_STAGEMAP_1	(400)
#define HEIGHT_STAGEMAP_1	(280)
#define POS_X_STAGEMAP_1	(-270.0f)
#define POS_Y_STAGEMAP_1	(SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.025f

#define WIDTH_STAGEMAP_2	(400)
#define HEIGHT_STAGEMAP_2	(280)
#define POS_X_STAGEMAP_2	(270.0f)
#define POS_Y_STAGEMAP_2	(SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.025f

#define WIDTH_RIGHT_ARROW	(100)
#define HEIGHT_RIGHT_ARROW	(100)

#define POS_X_LEFT_ARROW	(-580.0f)
#define POS_X_RIGHT_ARROW	(580.0f)
#define POS_Y_RIGHT_ARROW	((SCREEN_HEIGHT - HEIGHT_STAGEMAP_1)*0.125f -75.0f)

#define WIDTH_SEAZON_UI		(135)
#define HEIGHT_SEAZON_UI	(100)
#define POS_X_SEAZON_UI		(-550)
#define POS_Y_SEAZON_UI		(300)

//ステージ枠
#define WIDTH_STAGE_FRAME			(480)
#define HEIGHT_STAGE_FRAME			(480)
#define POS_X_STAGE_FRAME_1			(POS_X_STAGEMAP_1)
#define POS_X_STAGE_FRAME_2			(POS_X_STAGEMAP_2)
#define POS_Y_STAGE_FRAME_1			(POS_Y_STAGEMAP_1 + 60.0f)
#define POS_Y_STAGE_FRAME_2			(POS_Y_STAGEMAP_2 + 60.0f)

//ADD
//プレイヤー
#define POS_X_SELECT_PLAYER			(0.0f)
#define POS_Y_SELECT_PLAYER			(-110.0f)
//ADD
//クローバーUI（空）
#define WIDTH_CLOVER_UI				(50)
#define HEIGHT_CLOVER_UI			(50)
#define POS_X_CLOVER_UI_1			(POS_X_STAGEMAP_1 + WIDTH_STAGEMAP_1 * 0.25f)
#define POS_Y_CLOVER_UI_1			(POS_Y_STAGEMAP_1 - HEIGHT_STAGEMAP_1 * 0.5f) - 20.0f
#define POS_X_CLOVER_UI_2			(POS_X_STAGEMAP_2 + WIDTH_STAGEMAP_2 * 0.25f)
#define POS_Y_CLOVER_UI_2			(POS_Y_STAGEMAP_2 - HEIGHT_STAGEMAP_1 * 0.5f) - 20.0f
//ADD
//季節UI
#define WIDTH_SEAZON_UI				(135)
#define HEIGHT_SEAZON_UI			(100)
#define POS_X_SEAZON_UI				(-550)
#define POS_Y_SEAZON_UI				(300)
//ADD
//クリアUI（花）

#define WIDTH_CLEAR_FLOWER_UI		(100.0f)
#define HEIGHT_CLEAR_FLOWER_UI		(100.0f)
#define POS_X_CLEAR_FLOWER_UI_1		(POS_X_CLOVER_UI_1 + 110.0f)
#define POS_Y_CLEAR_FLOWER_UI_1		(POS_Y_CLOVER_UI_1)
#define POS_X_CLEAR_FLOWER_UI_2		(POS_X_CLOVER_UI_2 + 110.0f)
#define POS_Y_CLEAR_FLOWER_UI_2		(POS_Y_CLOVER_UI_2)
//ADD
//クリアUI（文字）

#define WIDTH_CLEAR_UI				(300)
#define HEIGHT_CLEAR_UI				(225)
#define POS_X_CLEAR_UI_1			(POS_X_STAGEMAP_1 + WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_CLEAR_UI_1			(POS_Y_STAGEMAP_1 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
#define POS_X_CLEAR_UI_2			(POS_X_STAGEMAP_2 + WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_CLEAR_UI_2			(POS_Y_STAGEMAP_2 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
//ADD
//ステージ名
#define WIDTH_STAGENAME_UI			(300)
#define HEIGHT_STAGENAME_UI			(225)
#define POS_X_STAGENAME_UI_1		(POS_X_STAGEMAP_1 - WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_STAGENAME_UI_1		(POS_Y_STAGEMAP_1 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f
#define POS_X_STAGENAME_UI_2		(POS_X_STAGEMAP_2 - WIDTH_STAGEMAP_1 * 0.35)
#define POS_Y_STAGENAME_UI_2		(POS_Y_STAGEMAP_2 + HEIGHT_STAGEMAP_1 * 0.5) + 20.0f


#define INTERVAL	(60)


//===== 構造体定義 ======
int SceneSelect::m_nCurrentSeason = 0;
SceneSelect::Row SceneSelect::m_SelectGrid;
SceneSelect::Column SceneSelect::m_Clover;
int SceneSelect::m_OldSeason;
bool SceneSelect::m_bRelease1 = false;		// ステージ解放演出
bool SceneSelect::m_bRelease2 = false;		// ステージ解放演出
bool SceneSelect::m_bReleaseClover[8] = {false, false, false, false, false, false, false, false};		// クローバー開放演出

//===== グローバル宣言 =====
CDrawEffect2D *g_CurrentMap1 = nullptr;
CDrawEffect2D *g_CurrentMap2 = nullptr;
CDraw2D *g_CurrentBG = nullptr;
CDraw2D *g_CurrentSeazonUI = nullptr;
CDraw2D *g_CurrentStageName1 = nullptr;
CDraw2D *g_CurrentStageName2 = nullptr;
CDraw2D *g_CurrentClearFlower1 = nullptr;
CDraw2D *g_CurrentClearFlower2 = nullptr;

CDraw2D *g_CurrentStageClearUI1 = nullptr;	//ステージマップ１のクリア状況表示
CDraw2D *g_CurrentStageClearUI2 = nullptr;	//ステージマップ２のクリア状況表示

CDrawModel *g_CurrentPlayer = nullptr;
CDrawModel *g_CurrentBlock = nullptr;



CLeaf g_SelectLeaf;

// コンストラクタ
SceneSelect::SceneSelect()
{
	SceneSelect::Load();

	m_OldSeason = SPRING;  // 01/12追加
}

// デストラクタ
SceneSelect::~SceneSelect(){
}

// 初期化
void SceneSelect::Init()
{
	//===== ロード画面のスレッドを作る =====
	Load::Begin();

	// 進行状況の読み込み
	SceneSelect::Load();

	//	管理クラスのインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();	// オブジェクトマネージャー
	m_pMenuManager = MenuManager::GetInstance();		// メニューマネージャー

	// カメラの初期化
	CCamera::Get()->Init();

	/* テクスチャ読み込み */
	TextureManager* pTexManager = TextureManager::GetInstance();
	// UI
	pTexManager->AddTexture(PATH_TEX_ARROW_UI, ARROW_UI_TEX_NUM);				// 矢印
	pTexManager->AddTexture(PATH_TEX_SPRING_UI, SPRING_UI_TEX_NUM);				// 春(文字
	pTexManager->AddTexture(PATH_TEX_SUMMER_UI, SUMMER_UI_TEX_NUM);				// 夏(文字
	pTexManager->AddTexture(PATH_TEX_AUTUMN_UI, AUTUMN_UI_TEX_NUM);				// 秋(文字
	pTexManager->AddTexture(PATH_TEX_WINTER_UI, WINTER_UI_TEX_NUM);				// 冬(文字
	
	//　tomoya
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_SPRING, CLEAR_FLOWER_SPRING_TEX_NUM);	// セレクトの花
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_SUMMER, CLEAR_FLOWER_SUMMER_TEX_NUM);	// セレクトの花
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_AUTUMN, CLEAR_FLOWER_AUTUMN_TEX_NUM);	// セレクトの花
	pTexManager->AddTexture(PATH_TEX_CLEAR_FLOWER_WINTER, CLEAR_FLOWER_WINTER_TEX_NUM);	// セレクトの花
	pTexManager->AddTexture(PATH_TEX_STAGE_FRAME, STAGE_FRAME_TEX_NUM);	//ステージの枠
	pTexManager->AddTexture(PATH_TEX_SPRING_1, SPRING_1_TEX_NUM);		//ステージ名：春ー１
	pTexManager->AddTexture(PATH_TEX_SPRING_2, SPRING_2_TEX_NUM);		//			 春ー２
	pTexManager->AddTexture(PATH_TEX_SUMMER_1, SUMMER_1_TEX_NUM);		//		　　　夏ー１
	pTexManager->AddTexture(PATH_TEX_SUMMER_2, SUMMER_2_TEX_NUM);		//		　　　夏ー２
	pTexManager->AddTexture(PATH_TEX_AUTUMN_1, AUTUMN_1_TEX_NUM);		//		　　　秋ー１
	pTexManager->AddTexture(PATH_TEX_AUTUMN_2, AUTUMN_2_TEX_NUM);		//		　　　秋ー２
	pTexManager->AddTexture(PATH_TEX_WINTER_1, WINTER_1_TEX_NUM);		//		　　　冬ー１
	pTexManager->AddTexture(PATH_TEX_WINTER_2, WINTER_2_TEX_NUM);		//		　　　冬ー２

	// クローバー中身
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI, CLOVER_UI_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOVER_UI_FRAME, CLOVER_UI_FRAME_TEX_NUM);

	pTexManager->AddTexture(PATH_TEX_CLEAR_UI, CLEAR_UI_TEX_NUM);		//クリア文字
	pTexManager->AddTexture(PATH_TEX_PARFECT_UI, PARFECT_UI_TEX_NUM);	//パーフェクト文字



	pTexManager->AddTexture(PATH_MV_SPRING_1,   MV_SPRING_1_NUM);
	pTexManager->AddTexture(PATH_MV_SPRING_2,   MV_SPRING_2_NUM);
	pTexManager->AddTexture(PATH_MV_SUMMER_1, MV_SUMMER_1_NUM);
	pTexManager->AddTexture(PATH_MV_SUMMER_2, MV_SUMMER_2_NUM);
	pTexManager->AddTexture(PATH_MV_FALL_1,   MV_FALL_1_NUM);
	pTexManager->AddTexture(PATH_MV_FALL_2,   MV_FALL_2_NUM);
	pTexManager->AddTexture(PATH_MV_WINTER_1, MV_WINTER_1_NUM);
	pTexManager->AddTexture(PATH_MV_WINTER_2, MV_WINTER_2_NUM);



	/* モデル読み込み */
	ModelManager* pModelManager = ModelManager::GetInstance();
	//シルフィ
	pModelManager->AddModel(PATH_MODEL_SPRING_PLAYER, SPRING_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_PLAYER, SUMMER_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_PLAYER, FALL_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_PLAYER, WINTER_PLAYER_NUM);
	//ブロック
	pModelManager->AddModel(PATH_MODEL_SPRING_BLOCK, SPRING_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_SUMMER_BLOCK, SUMMER_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_FALL_BLOCK, FALL_BLOCK_MODEL_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_BLOCK, WINTER_BLOCK_MODEL_NUM);

	// 01/12追加
	m_nCurrentSeason = m_OldSeason;


#pragma region	---プレイヤー	2022/01/21 tomoya
	//	オブジェクト生成
	Object* Player = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//	コンポーネントの追加
	auto TransPlayer = Player->AddComponent<CTransform>();				//	トランスフォーム
	auto DrawPlayer = Player->AddComponent<CDrawModel>();					//	モデル描画機能
	//	オブジェクト情報の設定
	DrawPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));			// 待機、吹かれている、落ちる、拾う
	TransPlayer->SetScale(5.0f, 5.0f, 5.0f);									// スケール
	TransPlayer->SetPosition(POS_X_SELECT_PLAYER, POS_Y_SELECT_PLAYER, 0.0f);	// 初期配置
	// アニメーション設定
	DrawPlayer->AddAnimation(PLAYER_IDLE, PLAYER_ANIM_IDLE_S, PLAYER_ANIM_IDLE_E, true);// 待機
	DrawPlayer->AlwaysAnimation(true);				// 常にアニメーション
	DrawPlayer->Play(PLAYER_IDLE);					// 待機モーション再生
	// 3Dリストに追加
	m_pObjectManager->AddObject(Player);
#pragma endregion

#pragma region	--- 地形ブロック	1/24 tomoya
	//	オブジェクト生成
	Object* objBlock = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
	//	コンポーネントの追加
	auto Trans_Block = objBlock->AddComponent<CTransform>();				//	トランスフォーム
	auto Draw_Block = objBlock->AddComponent<CDrawModel>();					//	モデル描画機能

	//	オブジェクト情報の設定
	Draw_Block->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));	// モデル

	Trans_Block->SetScale(35.0f, 2.0f, 1.0f);												//	スケール
	Trans_Block->SetPosition(POS_X_SELECT_PLAYER, POS_Y_SELECT_PLAYER - 5.0f, 0.0f);		//  初期配置

	// 3Dリストに追加
	m_pObjectManager->AddObject(objBlock);
#pragma endregion

#pragma region ---背景
		//	オブジェクト生成
	Object* objBG = new Object("BG1", UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	objBG->AddComponent<CTransform>();											//	座標
	auto Draw_BG = objBG->AddComponent<CDraw2D>();								//	描画
	//	オブジェクトの設定
	Draw_BG->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));//	テクスチャ
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);								//	サイズ
	//	リストに追加
	m_pObjectManager->AddObject(objBG);											//	オブジェクトの追加
#pragma endregion


#pragma region --- クリアUI表示（文字）
		//クリアのUIを追加する
	Object* objClearUI_1 = new Object(CLEAR_UI_1_NAME, UPDATE_UI, DRAW_UI);
	Object* objClearUI_2 = new Object(CLEAR_UI_2_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントをついかする
	auto Trans_ClearUI_1 = objClearUI_1->AddComponent<CTransform>();		//	座標
	auto Draw_ClearUI_1 = objClearUI_1->AddComponent<CDraw2D>();
	auto Trans_ClearUI_2 = objClearUI_2->AddComponent<CTransform>();		//	座標
	auto Draw_ClearUI_2 = objClearUI_2->AddComponent<CDraw2D>();
	//オブジェクトの設定
	Trans_ClearUI_1->SetPosition(POS_X_CLEAR_UI_1, POS_Y_CLEAR_UI_1);
	Trans_ClearUI_2->SetPosition(POS_X_CLEAR_UI_2, POS_Y_CLEAR_UI_2);

	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
	Draw_ClearUI_1->SetSize(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI);
	Draw_ClearUI_1->SetColor(1.0f, 1.0f, 1.0f);	//色をセット（白色）
	Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
	Draw_ClearUI_2->SetSize(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI);
	Draw_ClearUI_2->SetColor(1.0f, 1.0f, 1.0f);	//色をセット(白色)


	//objClearUI_1->NoUse();	//描画を停止
	//objClearUI_2->NoUse();
	//リストに追加
	ObjectManager::GetInstance()->AddObject(objClearUI_1);
	ObjectManager::GetInstance()->AddObject(objClearUI_2);

#pragma endregion

#pragma region --- ステージ名UI表示
	//ステージ名UIを追加する
	Object* objStageNameUI_1 = new Object("StageNameUI", UPDATE_UI, DRAW_UI);
	Object* objStageNameUI_2 = new Object("StageNameUI", UPDATE_UI, DRAW_UI);

	//コンポーネントをついかする
	auto Trans_StageNameUI_1 = objStageNameUI_1->AddComponent<CTransform>();						//	座標
	auto Trans_StageNameUI_2 = objStageNameUI_2->AddComponent<CTransform>();						//	座標
	auto Draw_StageNameUI_1 = objStageNameUI_1->AddComponent<CDraw2D>();
	auto Draw_StageNameUI_2 = objStageNameUI_2->AddComponent<CDraw2D>();

	//オブジェクトの設定
	Trans_StageNameUI_1->SetPosition(POS_X_STAGENAME_UI_1, POS_Y_STAGENAME_UI_1);
	Trans_StageNameUI_2->SetPosition(POS_X_STAGENAME_UI_2, POS_Y_STAGENAME_UI_2);
	Draw_StageNameUI_1->SetTexture(nullptr);
	Draw_StageNameUI_2->SetTexture(nullptr);
	Draw_StageNameUI_1->SetSize(WIDTH_STAGENAME_UI, HEIGHT_STAGENAME_UI);
	Draw_StageNameUI_2->SetSize(WIDTH_STAGENAME_UI, HEIGHT_STAGENAME_UI);

	//リストに追加
	ObjectManager::GetInstance()->AddObject(objStageNameUI_1);
	ObjectManager::GetInstance()->AddObject(objStageNameUI_2);

#pragma endregion

#pragma region  --- クリアUI表示（花）

//クリア（花）のUIを追加する
	Object* objClearFlowerUI_1 = new Object(CLEAR_FLOWER_UI_1_NAME, UPDATE_UI, DRAW_UI);
	Object* objClearFlowerUI_2 = new Object(CLEAR_FLOWER_UI_2_NAME, UPDATE_UI, DRAW_UI);

	//コンポーネントをついかする
	auto Trans_ClearFlowerUI_1 = objClearFlowerUI_1->AddComponent<CTransform>();
	auto Trans_ClearFlowerUI_2 = objClearFlowerUI_2->AddComponent<CTransform>();
	auto Draw_ClearFlowerUI_1 = objClearFlowerUI_1->AddComponent<CDraw2D>();
	auto Draw_ClearFlowerUI_2 = objClearFlowerUI_2->AddComponent<CDraw2D>();

	//オブジェクトの設定
	Trans_ClearFlowerUI_1->SetPosition(POS_X_CLEAR_FLOWER_UI_1, POS_Y_CLEAR_FLOWER_UI_1);
	Trans_ClearFlowerUI_2->SetPosition(POS_X_CLEAR_FLOWER_UI_2, POS_Y_CLEAR_FLOWER_UI_2);
	Draw_ClearFlowerUI_1->SetTexture(nullptr);
	Draw_ClearFlowerUI_2->SetTexture(nullptr);
	Draw_ClearFlowerUI_1->SetSize(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI);
	Draw_ClearFlowerUI_2->SetSize(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI);

	//リストに追加
	ObjectManager::GetInstance()->AddObject(objClearFlowerUI_1);
	ObjectManager::GetInstance()->AddObject(objClearFlowerUI_2);

#pragma endregion


#pragma region ---取得クローバーUI（枠
	//ステージマップ１のクローバー
	for (int i = 0; i < 4; i++)
	{
		Object* objBlankClover = new Object("CloverBlank", UPDATE_UI, DRAW_UI_FRAME);
		//コンポーネントをついかする
		auto TransClov_UI = objBlankClover->AddComponent<CTransform>();
		auto DrawClov_UI = objBlankClover->AddComponent<CDraw2D>();
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		switch (i)
		{
			//ここでポジションの設定をする
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		m_pObjectManager->AddObject(objBlankClover);
	}
	//ステージマップ２のクローバー
	for (int i = 0; i < 4; i++)
	{
		Object* objBlankClover = new Object("CloverBlank", UPDATE_UI, DRAW_UI_FRAME);
		//コンポーネントをついかする
		auto TransClov_UI = objBlankClover->AddComponent<CTransform>();
		auto DrawClov_UI = objBlankClover->AddComponent<CDraw2D>();
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_FRAME_TEX_NUM));
		switch (i)
		{
			//オブジェクトの設定
			//ここでポジションの設定をする
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		m_pObjectManager->AddObject(objBlankClover);
	}
#pragma endregion


	// ステージマップ1
#pragma region ---ステージマップ１
//オブジェクト生成
	Object* objStageMap1 = new Object(STAGE1_NAME, UPDATE_UI, DRAW_EFFECT);
	//コンポーネントの追加
	auto trans1 = objStageMap1->AddComponent<CTransform>();				//	オブジェクト座標
	auto Draw_StageMap1 = objStageMap1->AddComponent<CDrawEffect2D>();	//	描画
	auto Col_StageMap1 = objStageMap1->AddComponent<CCollider>();		//	当たり判定
	auto Menu_StageMap1 = objStageMap1->AddComponent<CMenu>();			//	メニュー機能
	objStageMap1->AddComponent<CPreview>();
	//オブジェクトの設定
	Draw_StageMap1->SetTexture(TextureManager::GetInstance()->GetTexture(MV_SPRING_1_NUM));//	テクスチャ(仮)
	Draw_StageMap1->SetSize(WIDTH_STAGEMAP_1, HEIGHT_STAGEMAP_1);	//	サイズ
	Draw_StageMap1->SetLoop(true);
	Draw_StageMap1->SetAnimSplit(4, 19);		// 仮
	Draw_StageMap1->SetSwapFrame(3);
	Draw_StageMap1->SetPlayONOFF(false);
	trans1->SetPosition(POS_X_STAGEMAP_1, POS_Y_STAGEMAP_1);						//	座標
	Col_StageMap1->SetCollisionSize(WIDTH_STAGEMAP_1, HEIGHT_STAGEMAP_1);			//	あたり判定の大きさ
	//	リストに追加
	m_pObjectManager->AddObject(objStageMap1);										//	オブジェクトの追加
#pragma endregion

// ステージマップ2
#pragma region ---ステージマップ２
//オブジェクト生成
	Object* objStageMap2 = new Object(STAGE2_NAME, UPDATE_UI, DRAW_EFFECT);
	//コンポーネントの追加
	auto Trabs2 = objStageMap2->AddComponent<CTransform>();									//	座標
	auto Draw_StageMap2 = objStageMap2->AddComponent<CDrawEffect2D>();						//	描画
	auto Col_StageMap2 = objStageMap2->AddComponent<CCollider>();							//	当たり判定
	auto Menu_StageMap2 = objStageMap2->AddComponent<CMenu>();								//	
	objStageMap2->AddComponent<CPreview>();
	//オブジェクトの設定
	Draw_StageMap2->SetTexture(TextureManager::GetInstance()->GetTexture(MV_SPRING_2_NUM));	//	テクスチャ
	Draw_StageMap2->SetSize(WIDTH_STAGEMAP_2, HEIGHT_STAGEMAP_2);							//	サイズ
	Draw_StageMap2->SetLoop(true);
	Draw_StageMap2->SetAnimSplit(4, 19);
	Draw_StageMap2->SetSwapFrame(3);
	Draw_StageMap2->SetPlayONOFF(false);
	Trabs2->SetPosition(POS_X_STAGEMAP_2, POS_Y_STAGEMAP_2);
	Col_StageMap2->SetCollisionSize(WIDTH_STAGEMAP_2, HEIGHT_STAGEMAP_2);		//	あたり判定の大きさ
	//リストに追加
	m_pObjectManager->AddObject(objStageMap2);									//	オブジェクトの追加
#pragma endregion



	// 矢印(左右含め)を書き換えたほうが早いです
#pragma region ---遷移矢印 左
		//	オブジェクト生成
	Object* objArrowLeft = new Object(LEFT_ARROW_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto Trans_ArrowLeft = objArrowLeft->AddComponent<CTransform>();			//	座標
	auto Draw_ArrowLeft = objArrowLeft->AddComponent<CDraw2D>();				//	描画
	auto Col_ArrowLeft = objArrowLeft->AddComponent<CCollider>();				//	当たり判定
	auto Menu_ArrowLeft = objArrowLeft->AddComponent<CMenu>();					//	メニュー
	//	オブジェクトの設定
	Draw_ArrowLeft->SetTexture(pTexManager->GetTexture(ARROW_UI_TEX_NUM));	//	テクスチャTextureManager::GetInstance()->GetTexture(ARROW_TEX_NUM)
	Draw_ArrowLeft->SetSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);				//	サイズ
	Trans_ArrowLeft->SetPosition(POS_X_LEFT_ARROW, POS_Y_RIGHT_ARROW);
	Col_ArrowLeft->SetCollisionSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);		//	あたり判定の大きさ
	Draw_ArrowLeft->SetAngle(0.0f);
	//	リストに追加
	m_pObjectManager->AddObject(objArrowLeft);
#pragma endregion

#pragma region ---遷移矢印 右
	//	オブジェクト生成
	Object* objArrow = new Object(RIGHT_ARROW_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto Trans_Arrow = objArrow->AddComponent<CTransform>();				//	座標
	auto Draw_Arrow = objArrow->AddComponent<CDraw2D>();					//	描画
	auto Col_Arrow = objArrow->AddComponent<CCollider>();					//	当たり判定
	auto Menu_Arrow = objArrow->AddComponent<CMenu>();						//	メニュー
	//	オブジェクトの設定
	Draw_Arrow->SetTexture(pTexManager->GetTexture(ARROW_UI_TEX_NUM));	//	テクスチャ
	Draw_Arrow->SetSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);				//	サイズ
	Trans_Arrow->SetPosition(POS_X_RIGHT_ARROW, POS_Y_RIGHT_ARROW);
	Col_Arrow->SetCollisionSize(WIDTH_RIGHT_ARROW, HEIGHT_RIGHT_ARROW);		//	あたり判定の大きさ
	Draw_Arrow->SetAngle(180.0f);
	//	リストに追加
	m_pObjectManager->AddObject(objArrow);									//	オブジェクトの追加
#pragma endregion

#pragma region --- 季節表示UI
	//	オブジェクト生成
	Object* objSeazonUI = new Object("seazonUI", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto Trans_SeazonUI = objSeazonUI->AddComponent<CTransform>();							//	座標
	auto Draw_SeazonUI = objSeazonUI->AddComponent<CDraw2D>();								//	描画
	//	オブジェクトの設定
	Draw_SeazonUI->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));//	テクスチャ
	Trans_SeazonUI->SetPosition(POS_X_SEAZON_UI, POS_Y_SEAZON_UI);
	Draw_SeazonUI->SetSize(WIDTH_SEAZON_UI, HEIGHT_SEAZON_UI);							//	サイズ
	//	リストに追加
	m_pObjectManager->AddObject(objSeazonUI);											//	オブジェクトの追加
#pragma endregion


#pragma region --- ステージフレーム表示
	for (int i = 0; i < 2; i++)
	{
		//ステージ名UIを追加する
		Object* objStageFrame = new Object("StageFrame", UPDATE_UI, DRAW_FIELD);
		//コンポーネントをついかする
		auto Trans_StageFrame = objStageFrame->AddComponent<CTransform>();						//	座標
		auto Draw_StageFrame = objStageFrame->AddComponent<CDraw2D>();
		//オブジェクトの設定
		//ここでポジションの設定をする
		switch (i) {
		case 0: Trans_StageFrame->SetPosition(POS_X_STAGE_FRAME_1, POS_Y_STAGE_FRAME_1); break;
		case 1: Trans_StageFrame->SetPosition(POS_X_STAGE_FRAME_2, POS_Y_STAGE_FRAME_2); break;

		default: break;
		}

		Draw_StageFrame->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_FRAME_TEX_NUM));
		Draw_StageFrame->SetSize(WIDTH_STAGE_FRAME, HEIGHT_STAGE_FRAME);
		//リストに追加
		ObjectManager::GetInstance()->AddObject(objStageFrame);
	}
#pragma endregion

	std::list<Object*> Clovers;	// クローバーUIを入れる箱
#pragma region ---取得クローバーUI
	//ステージマップ１
	for (int i = 0; i < 4; i++)
	{
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントをついかする
		auto TransClov_UI	= pUI_Clover->AddComponent<CTransform>();			// オブジェクト座標
		auto DrawClov_UI	= pUI_Clover->AddComponent<CDraw2D>();				// スクリーン描画
		// オブジェクト設定
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);				// サイズ
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_TEX_NUM));	// テクスチャ
		// リストに追加
		m_pObjectManager->AddObject(pUI_Clover);

		// 座標設定
		switch (i)
		{
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_1 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_1 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}

		// 一時的にクローバーだけのリストに追加
		Clovers.push_back(pUI_Clover);
	}

	//ステージマップ２
	for (int i = 0; i < 4; i++)
	{
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントをついかする
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();				// オブジェクト座標
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();					// スクリーン描画
		// オブジェクト設定
		DrawClov_UI->SetSize(WIDTH_CLOVER_UI, HEIGHT_CLOVER_UI);				// サイズ
		DrawClov_UI->SetTexture(pTexManager->GetTexture(CLOVER_UI_TEX_NUM));	// テクスチャ
		// リストに追加
		m_pObjectManager->AddObject(pUI_Clover);

		// 座標設定
		switch (i)
		{
		case 0:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(POS_X_CLOVER_UI_2 + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, POS_Y_CLOVER_UI_2 + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}

		// 一時的にクローバーだけのリストに追加
		Clovers.push_back(pUI_Clover);
	}
#pragma endregion


#pragma region --- 取得クローバー表示/非表示
	/* clover_data.csvの仕様
		行：1行目のみ使用
		列：		春-1		 0,  1,  2,  3
				春-2		 4,  5,  6,  7
				・
				・
				・
				冬-1		24, 25, 26, 27
				冬-2		28, 29, 30, 31
	*/
	int nNuber = 0;
	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));

	for (auto&& clover : Clovers)
	{
		if (m_Clover[(m_nCurrentSeason * 8) + nNuber] == 0)
		{
			clover->StopDraw();		// 描画しない

			if (nNuber <= 3)	Draw_ClearUI_1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
			else				Draw_ClearUI_2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
		}
		else
		{
			clover->ResumeDraw();	// 描画する
		}

		nNuber++;
	}
#pragma endregion

	
	//現在の背景に保存
	g_CurrentBG = Draw_BG;
	//現在のステージマップに保存
	g_CurrentMap1 = Draw_StageMap1;
	//現在のステージマップに保存
	g_CurrentMap2 = Draw_StageMap2;
	//現在の季節UIに保存
	g_CurrentSeazonUI = Draw_SeazonUI;

	//現在のステージ名を保存
	g_CurrentStageName1 = Draw_StageNameUI_1;
	g_CurrentStageName2 = Draw_StageNameUI_2;
	//現在の花アイコンを保存
	g_CurrentClearFlower1 = Draw_ClearFlowerUI_1;
	g_CurrentClearFlower2 = Draw_ClearFlowerUI_2;

	//現在のステージクリア情報UIを保存
	g_CurrentStageClearUI1 = Draw_ClearUI_1;
	g_CurrentStageClearUI2 = Draw_ClearUI_2;

	//現在のプレイヤーモデル保存
	g_CurrentPlayer = DrawPlayer;

	//現在のブロックを保存
	g_CurrentBlock = Draw_Block;

	// セレクトメニュー
	m_pMenuManager->CreateSelectMenu();

	bMV1_PlayONOFF = false;
	bMV2_PlayONOFF = false;

	//	BGM再生
	CSound::Play(BGM_STAGESELECT);

	//===== ロード終了処理 =====
	Load::End();
}

// 終了処理
void SceneSelect::Uninit()
{
	// BGM停止
	CSound::Stop(BGM_STAGESELECT);

	//	オブジェクトの終了
	m_pObjectManager->Uninit();

	// メニューの終了
	MenuManager::GetInstance()->DeleteMenu();
}

// 更新
void SceneSelect::Update()
{
	// メニュー更新
	m_pMenuManager->Update();

	//	オブジェクトリストの更新
	m_pObjectManager->Update();

	//降ってくるモノの更新
	g_SelectLeaf.UpdateSelectScene();

	// 季節ごとで表示するものを変える
	TextureManager* pTex = TextureManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();
	switch (m_nCurrentSeason)
	{

	case SPRING:
		// 季節情報保存
		m_OldSeason = SPRING;
		//　背景(春
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_SPRING_TEX_NUM));

		// 左上の文字(春
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(SPRING_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(1.0f, 0.55f, 1.0f);	//濃いピンク

		//ステージ名
		g_CurrentStageName1->SetTexture(pTex->GetTexture(SPRING_1_TEX_NUM));//春-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(SPRING_2_TEX_NUM));//春-2

		//クリアアイコン
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SPRING_TEX_NUM));//桜アイコン
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SPRING_TEX_NUM));

		//シルフィーモデル
		g_CurrentPlayer->SetModel(pModelManager->GetModel(SPRING_PLAYER_NUM));
		//ブロックモデル
		g_CurrentBlock->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));

		//	動画
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_SPRING_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_SPRING_2_NUM));

		break;

	case SUMMER:
		// 季節情報保存
		m_OldSeason = SUMMER;
		//テクスチャ読み込み（夏）
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_SUMMER_TEX_NUM));
		// 左上の文字(夏
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(SUMMER_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(0.0f, 1.0f, 0.0f);	//緑

		//ADD
		g_CurrentStageName1->SetTexture(pTex->GetTexture(SUMMER_1_TEX_NUM));//ステージ名:夏-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(SUMMER_2_TEX_NUM));//ステージ名:夏-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SUMMER_TEX_NUM));//ヒマワリアイコン
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_SUMMER_TEX_NUM));

		//シルフィーモデル
		g_CurrentPlayer->SetModel(pModelManager->GetModel(SUMMER_PLAYER_NUM));

		//ブロックモデル
		g_CurrentBlock->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));

		//	動画
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_SUMMER_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_SUMMER_2_NUM));

		break;

	case AUTUMN:
		// 季節情報保存
		m_OldSeason = AUTUMN;
		//テクスチャ読み込み（秋）
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_AUTUMN_TEX_NUM));
		//テクスチャ設定
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(AUTUMN_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(1.0f, 0.5f, 0.0f);	//オレンジ

		g_CurrentStageName1->SetTexture(pTex->GetTexture(AUTUMN_1_TEX_NUM));//ステージ名:秋-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(AUTUMN_2_TEX_NUM));//ステージ名:秋-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_AUTUMN_TEX_NUM));//コスモスアイコン
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_AUTUMN_TEX_NUM));

		//シルフィーモデル
		g_CurrentPlayer->SetModel(pModelManager->GetModel(FALL_PLAYER_NUM));
		//ブロックモデル
		g_CurrentBlock->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));

		//	動画
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_FALL_1_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_FALL_2_NUM));

		break;

	case WINTER:
		// 季節情報保存
		m_OldSeason = WINTER;
		//テクスチャ読み込み（冬）
		g_CurrentBG->SetTexture(pTex->GetTexture(TITLE_WINTER_TEX_NUM));
		//テクスチャ設定
		g_CurrentSeazonUI->SetTexture(pTex->GetTexture(WINTER_UI_TEX_NUM));
		g_CurrentSeazonUI->SetColor(0.5f, 1.0f, 1.0f);	//水色
		//ADD
		g_CurrentStageName1->SetTexture(pTex->GetTexture(WINTER_1_TEX_NUM));//ステージ名:冬-1
		g_CurrentStageName2->SetTexture(pTex->GetTexture(WINTER_2_TEX_NUM));//ステージ名:冬-2
		g_CurrentClearFlower1->SetTexture(pTex->GetTexture(CLEAR_FLOWER_WINTER_TEX_NUM));//椿アイコン
		g_CurrentClearFlower2->SetTexture(pTex->GetTexture(CLEAR_FLOWER_WINTER_TEX_NUM));

		//シルフィーモデル
		g_CurrentPlayer->SetModel(pModelManager->GetModel(WINTER_PLAYER_NUM));
		//ブロックモデル
		g_CurrentBlock->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));

		//	動画
		g_CurrentMap1->SetTexture(pTex->GetTexture(MV_WINTER_2_NUM));
		g_CurrentMap2->SetTexture(pTex->GetTexture(MV_WINTER_1_NUM));

		break;
	}

	// 選択可能かどうか
	if (m_SelectGrid[0][0 + m_nCurrentSeason * 2] == 0)	g_CurrentMap1->SetColor(0.3f, 0.3f, 0.3f);
	else												g_CurrentMap1->SetColor(1.0f, 1.0f, 1.0f);
	if (m_SelectGrid[0][1 + m_nCurrentSeason * 2] == 0)	g_CurrentMap2->SetColor(0.3f, 0.3f, 0.3f);
	else												g_CurrentMap2->SetColor(1.0f, 1.0f, 1.0f);

	//ADD
	// ***** クリア(花)の表示/非表示
	//クリアしていなかったら花アイコンを表示しない
	//ステージ１
	if (m_SelectGrid[0][1 + m_nCurrentSeason * 2] == 1)
	{
		g_CurrentClearFlower1->SetAlpha(1.0f);	//表示
		g_CurrentStageClearUI1->SetAlpha(1.0f);
		//bAnimPlaying = true;
	}
	else
	{
		g_CurrentClearFlower1->SetAlpha(0.0f);	//非表示
		g_CurrentStageClearUI1->SetAlpha(0.0f);

	}
	//ステージ２
	if (m_SelectGrid[0][0 + (m_nCurrentSeason + 1) * 2] == 1)
	{
		g_CurrentClearFlower2->SetAlpha(1.0f);	//表示
		g_CurrentStageClearUI2->SetAlpha(1.0f);
	}
	else
	{
		g_CurrentClearFlower2->SetAlpha(0.0f);	//非表示
		g_CurrentStageClearUI2->SetAlpha(0.0f);
	}

	// 冬の2だけ違う処理
	if (m_nCurrentSeason == WINTER)
	{
		if (m_SelectGrid[0][COMPLETE] == 1)
		{
			g_CurrentClearFlower2->SetAlpha(1.0f);	//表示
			g_CurrentStageClearUI2->SetAlpha(1.0f);
			//g_CurrentStageClearUI1->SetAlpha(1.0f);
		}
		else
		{
			g_CurrentClearFlower2->SetAlpha(0.0f);	//非表示
			g_CurrentStageClearUI2->SetAlpha(0.0f);
			//g_CurrentStageClearUI1->SetAlpha(0.0f);
		}
	}

	// 2022/01/23 春、冬の時は矢印を表示しない
	if (m_nCurrentSeason == SPRING)
	{
		Object* pLeftArrow = m_pObjectManager->GetGameObject(LEFT_ARROW_NAME);
		pLeftArrow->NoUse();
	}
	else if (m_nCurrentSeason == WINTER)
	{

		Object* pRightArrow = m_pObjectManager->GetGameObject(RIGHT_ARROW_NAME);
		pRightArrow->NoUse();
	}



	// 2022/01/24 クリア、パーフェクト表示更新 tomoya
	// 
	//TODO:　パーフェクトが上手く反映されない
	// 
	//ステージマップ１
	if (m_SelectGrid[0][2 + m_nCurrentSeason * 2] >= 4)	//クローバー総量がMax
	{
		g_CurrentStageClearUI1->SetTexture(pTex->GetTexture(PARFECT_UI_TEX_NUM));
		g_CurrentStageClearUI1->SetColor(1.0f, 0.75f, 0.0f);	//色をセット(黄色)
	}
	//ステージマップ２
	if (m_SelectGrid[0][3 + m_nCurrentSeason * 2] >= 4)	//クローバー総量がMax
	{
		g_CurrentStageClearUI2->SetTexture(pTex->GetTexture(PARFECT_UI_TEX_NUM));
		g_CurrentStageClearUI2->SetColor(1.0f, 0.75f, 0.0f);	//色をセット(黄色)

	}


	/*
	// 2022/01/24 クリアアニメーション（花）tomoya
	if (bAnimPlaying == true)
	{
		if (fFlowerSize.x <= WIDTH_CLEAR_FLOWER_UI)
		{
			fFlowerSize.x += 2.0f;
			fFlowerSize.y += 2.0f;
		}
		else
			bAnimPlaying == false;
		g_CurrentClearFlower1->SetSize(fFlowerSize.x, fFlowerSize.y);
	}
	*/

	// 開放演出
	SceneSelect::Clear();
}

// 描画
void SceneSelect::Draw()
{
	//	オブジェクトリストの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	m_pMenuManager->Draw();
	SetZBuffer(false);

#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	//Text("Seazon : %d", i);
	Text("Seazon   : %d", m_nCurrentSeason);
	Text("GridSize : %d", m_SelectGrid.size());
	End();
#endif // _DEBUG
}

// セレクト画面の季節切り替え
void SceneSelect::ChangeSceason(int n)
{
	m_nCurrentSeason += n;
	if (m_nCurrentSeason < 0) m_nCurrentSeason = 0;
	if (m_nCurrentSeason > 3) m_nCurrentSeason = 3;

	ObjectManager* pObjectManager = ObjectManager::GetInstance();
	TextureManager* pTexManager = TextureManager::GetInstance();

	// 矢印を表示しておく
	Object* pLeftArrow = pObjectManager->GetGameObject(LEFT_ARROW_NAME);
	pLeftArrow->ResumeUpdate();
	pLeftArrow->ResumeDraw();
	Object* pRightArrow = pObjectManager->GetGameObject(RIGHT_ARROW_NAME);
	pRightArrow->ResumeUpdate();
	pRightArrow->ResumeDraw();

	g_CurrentStageClearUI1->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	g_CurrentStageClearUI2->SetTexture(pTexManager->GetTexture(PARFECT_UI_TEX_NUM));
	

	bool bPerfect1 = true;
	bool bPerfect2 = true;
	std::list<Object*> Clovers;	// クローバーを一時的に入れる箱
	int nNuber = 7;				// クローバー番号
	pObjectManager->GetGameObjects(CLOVER_UI_NAME, Clovers);	// クローバーをリストに突っ込む
#pragma region --- 取得クローバー表示/非表示

	/* clover_data.csvの仕様
		行：1行目のみ使用
		列：		春-1		 0,  1,  2,  3
				春-2		 4,  5,  6,  7
				・
				・
				・
				冬-1		24, 25, 26, 27
				冬-2		28, 29, 30, 31
	*/

	for (auto&& clover : Clovers)
	{
		// クローバーを取っているかいないか
		if (m_Clover[(m_nCurrentSeason * 8) + nNuber] == 0)
		{
			clover->StopDraw();		// 描画しない

			if (nNuber <= 3)	g_CurrentStageClearUI1->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
			else				g_CurrentStageClearUI2->SetTexture(pTexManager->GetTexture(CLEAR_UI_TEX_NUM));
		}
		else
		{
			clover->ResumeDraw();	// 描画する
		}

		nNuber--;
	}
#pragma endregion

	
}

// ステージ選択可能か
bool SceneSelect::GetSelectable(int stage_num)
{
	
	int x = m_nCurrentSeason * 2 + stage_num;

	if (m_SelectGrid[0][x] == 0)
	{
		if(SceneManager::Instance()->GetScene() == SCENE_SELECT)	CSound::Play(SE_ERROR);
		return false;
	}
	else return true;

}

// load
bool SceneSelect::Load()
{
	// クローバー取得数の読み込み
	if (FileManager::GetInstance()->Load(CLOVER_DATA_CSV) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}
	// クリア状況の読み込み
	if (FileManager::GetInstance()->Load(SAVE_DATA_CSV) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}
	
	// クリア状況を書き込む
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
	m_SelectGrid.resize(CSV->GetRowSize());				// 行の数を設定
	for (int y = 0; y < CSV->GetRowSize(); ++y)
	{
		m_SelectGrid[y].resize(CSV->GetColumnSize(y));	// 列の数を設定
	
		for (int x = 0; x < CSV->GetColumnSize(y); x++)
		{
			// 要素を格納する
			m_SelectGrid[y][x] = CSV->GetInt(x, y);
		}
	}

	// クローバーの取得状況を移す
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	m_Clover.clear();
	for (int x = 0; x < CloverCSV->GetColumnSize(0); x++)
	{
		// 要素を格納する
		m_Clover.push_back(CloverCSV->GetInt(x, 0));
	}
	
	return true;
}

// save
bool SceneSelect::Save()
{

	// クリア状況の保存
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
	if (CSV == nullptr) return false;
	if (CSV->Save(SAVE_DATA_CSV) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	// クローバーの取得状況の保存
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	if (CloverCSV == nullptr) return false;
	if (CloverCSV->Save(CLOVER_DATA_CSV) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

// csvファイル初期化
bool SceneSelect::NewGame()
{
	m_OldSeason = SPRING;

	// 書き換え
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);

	for (int nRow = 0; nRow < m_SelectGrid.size(); nRow++)
	{
		for (int nColumn = 0; nColumn < m_SelectGrid[nRow].size(); nColumn++)
		{
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	// 最初のステージ選択は可能
	CSV->SetInt(0, 0, 1);

	// クローバーの取得状況
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	for (int nColumn = 0; nColumn < m_Clover.size(); nColumn++)
	{
		CloverCSV->SetInt(nColumn, 0, 0);
	}


	SceneSelect::Save();

	return true;
}

// クリアステージを保存
bool SceneSelect::SaveClearInfo(int nStage, bool bClover[4])
{
	// クローバーのcsvゲット
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);

	/*---------------- 前回ゲットしてなくて今回ゲットした場合、解放演出フラグをONにする ----------------*/
	int nX, nY;
	int nGameSeason = SceneGame::GetInstance()->GetSeason();
	nX = nGameSeason * 4;	// クローバー取得状況がこの列に入っている
	nY = 0;

	// 四回のループ
	for (int i = 0; i < 4; i++)
	{
		if (nGameSeason == WINTER_2) continue;
		// 新しくクローバーを取った場合
		if (CloverCSV->GetInt(nX + i, nY) == 0 && bClover[i] == true)
		{
			if (nGameSeason == SPRING_1 || nGameSeason == SUMMER_1 || nGameSeason == FALL_1 || nGameSeason == WINTER_1)
				m_bReleaseClover[i] = true;
			if (nGameSeason == SPRING_2 || nGameSeason == SUMMER_2 || nGameSeason == FALL_2 || nGameSeason == WINTER_2)
				m_bReleaseClover[i + 4] = true;
		}

	}
	/*----------------------------------------------------------------------------------*/

	// クローバーを書き換える
	int Clovernumber = nStage * 4;
	for (int i = 0; i < 4; i++)
	{
		CloverCSV->SetInt(Clovernumber + i, 0, bClover[i]);	// (列, 行, 格納する数字)
	}

	// 冬の2stageをクリアしたとき
	if (nStage == WINTER_2)
	{
		// 書き換え
		ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);

		// クリア状況を書き換える
		CSV->SetInt(COMPLETE, 0, 1);

		// セーブ
		SceneSelect::Save();

		return true;
	}
	else
	{
		// クリア状況を書き換える
		ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(SAVE_DATA_CSV);
		// 次のステージのフラグへ
		int x = nStage + 1;

		/*---------------- 前回クリアしてなくて今回クリアした場合、解放演出フラグをONにする ----------------*/
		int nGameSeason = SceneGame::GetInstance()->GetSeason();
		
		// 新しくクローバーを取った場合
		if (CSV->GetInt(x , 0) == 0)
		{
			if (nGameSeason == SPRING_1 || nGameSeason == SUMMER_1 || nGameSeason == FALL_1 || nGameSeason == WINTER_1)
				m_bRelease1 = true;
			if (nGameSeason == SPRING_2 || nGameSeason == SUMMER_2 || nGameSeason == FALL_2 || nGameSeason == WINTER_2)
				m_bRelease2 = true;
		}

		/*----------------------------------------------------------------------------------*/


		CSV->SetInt(x, 0, 1);	// (列, 行, 格納する数字)

		// csvに保存
		SceneSelect::Save();
	}

	return true;
}


// クローバーの取得状況を記録する
bool SceneSelect::SaveCloverInfo(int nStage, int cloverNumber)
{
	/* clover_data.csvの仕様
		行：1行目のみ使用
		列：		春-1		 0,  1,  2,  3
				春-2		 4,  5,  6,  7
				・
				・
				・
				冬-1		24, 25, 26, 27
				冬-2		28, 29, 30, 31
	*/
	int x = nStage * 4 + cloverNumber;

	//// クローバーの取得状況の保存
	//ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	//CloverCSV->SetInt(x, 0, 1);	// (列, 行, 格納する数字)

	m_Clover[x] = 1;

	return true;
}

// debug用
SceneSelect::Row SceneSelect::GetSelectGrid()
{
	return m_SelectGrid;
}




SceneSelect::Column SceneSelect::GetClovers()
{
	return m_Clover;
}


void SceneSelect::Clear()
{
	// クローバー取得演出
	for (int nCloverNumber = 0; nCloverNumber < 8; nCloverNumber++)
	{
		if (m_bReleaseClover[nCloverNumber])
		{
			std::list<Object*> Clovers;	// クローバーUIを入れる箱
			// クローバーUIオブジェクトをリストに突っ込む
			ObjectManager::GetInstance()->GetGameObjects(CLOVER_UI_NAME, Clovers);

			// クローバーを特定する
			std::list<Object*>::iterator itr = Clovers.begin();
			for (int i = 0; i < 7 - nCloverNumber; i++)	itr++;

			// 目的のクローバーに出現演出のコンポーネントを追加する
			auto scale1 = (*itr)->AddComponent<CGetUI>();

			// コンポーネントの初期化
			scale1->Start();

			// 目的の大きさを設定
			scale1->SetUISize(XMFLOAT2(WIDTH_CLOVER_UI, WIDTH_CLOVER_UI));

			//　拡大スピード
			scale1->SetUIRate(1.0f);

			// 遅延時間
			if (nCloverNumber == 0 || nCloverNumber == 4)	scale1->SetDelayTimer(120);
			if (nCloverNumber == 1 || nCloverNumber == 5)	scale1->SetDelayTimer(160);
			if (nCloverNumber == 2 || nCloverNumber == 6)	scale1->SetDelayTimer(200);
			if (nCloverNumber == 3 || nCloverNumber == 7)	scale1->SetDelayTimer(240);
			
			// 大きさを0にしておく
			(*itr)->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

			// フラグを降ろす
			m_bReleaseClover[nCloverNumber] = false;
		}
	}
	// ステージ1の解放演出
	if (m_bRelease1)
	{
		auto pClear = ObjectManager::GetInstance()->GetGameObject(CLEAR_UI_1_NAME);
		auto pFlower = ObjectManager::GetInstance()->GetGameObject(CLEAR_FLOWER_UI_1_NAME);

		// 目的のクローバーに出現演出のコンポーネントを追加する
		auto scaleClear = pClear->AddComponent<CGetUI>();
		auto scaleFlower = pFlower->AddComponent<CGetUI>();

		// コンポーネントの初期化
		scaleClear->Start();
		scaleFlower->Start();

		// 目的の大きさを設定
		scaleClear->SetUISize(XMFLOAT2(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI));
		scaleFlower->SetUISize(XMFLOAT2(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI));

		//　拡大スピード
		scaleClear->SetUIRate(4.0f);
		scaleFlower->SetUIRate(3.0f);

		// 遅延時間
		scaleClear->SetDelayTimer(0);
		scaleFlower->SetDelayTimer(60);

		// 大きさを0にしておく
		pClear->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);
		pFlower->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

		// フラグを降ろす
		m_bRelease1 = false;
	}

	// ステージ1の解放演出
	if (m_bRelease2)
	{
		auto pClear = ObjectManager::GetInstance()->GetGameObject(CLEAR_UI_2_NAME);
		auto pFlower = ObjectManager::GetInstance()->GetGameObject(CLEAR_FLOWER_UI_2_NAME);

		// 目的のクローバーに出現演出のコンポーネントを追加する
		auto scaleClear = pClear->AddComponent<CGetUI>();
		auto scaleFlower = pFlower->AddComponent<CGetUI>();

		// コンポーネントの初期化
		scaleClear->Start();
		scaleFlower->Start();

		// 目的の大きさを設定
		scaleClear->SetUISize(XMFLOAT2(WIDTH_CLEAR_UI, HEIGHT_CLEAR_UI));
		scaleFlower->SetUISize(XMFLOAT2(WIDTH_CLEAR_FLOWER_UI, HEIGHT_CLEAR_FLOWER_UI));

		//　拡大スピード
		scaleClear->SetUIRate(4.0f);
		scaleFlower->SetUIRate(3.0f);

		// 遅延時間
		scaleClear->SetDelayTimer(0);
		scaleFlower->SetDelayTimer(60);

		// 大きさを0にしておく
		pClear->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);
		pFlower->GetComponent<CDraw2D>()->SetSize(0.0f, 0.0f);

		// フラグを降ろす
		m_bRelease2 = false;
	}
}