//===== インクルード =====
#include "sceneWinter.h"
#include "fade.h"
#include "imgui.h"

#include "Object.h"
#include "ObjectInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "WindManager.h"
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
#include "InformationComponent.h"			
#include "WindComponent.h"
#include "GravityComponent.h"		//	重力
#include "FallComponent.h"
#include "HomingComponent.h"
#include "TexScrollComponent.h"
#include "GaugeComponent.h"
#include "FadeComponent.h"
#include "IcicleComponent.h"
#include "FollowCompomemt.h"
#include "PostCollisionComponent.h"
#include "EffectManager.h"
#include "sceneGame.h"
#include "FlashComponent.h"
#include "LotationComponent.h"

// コンストラクタ
SceneWinter::SceneWinter() {
}

// デストラクタ
SceneWinter::~SceneWinter() {
}

// 初期化
void SceneWinter::Init()
{
	//	管理クラスのインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();				//	オブジェクトマネージャー

	/* テクスチャ読み込み */
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_BG_D0, BG_D0_TEX_NUM);	//BG
	pTexManager->AddTexture(PATH_TEX_BG_D1, BG_D1_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D2, BG_D2_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D3, BG_D3_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D4, BG_D4_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D5, BG_D5_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D6, BG_D6_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BG_D7, BG_D7_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_EFFECT_WINTER, EFFECT_WINTER_TEX_NUM);	// 種(冬)
	

	/* モデル読み込み */
	ModelManager* pModelManager = ModelManager::GetInstance();	//	モデルマネージャー
	// シルフィ
	pModelManager->AddModel(PATH_MODEL_WINTER_PLAYER, WINTER_PLAYER_NUM);
	pModelManager->AddModel(PATH_MODEL_WINTER_GOAL, WINTER_GOAL_MODEL_NUM);	//ゴール時
	// ステージブロック
	pModelManager->AddModel(PATH_MODEL_WINTER_BLOCK, WINTER_BLOCK_MODEL_NUM);
	// ステージブロック(下の方
	pModelManager->AddModel(PATH_MODEL_WINTER_BLOCK_LOW, WINTER_BLOCK_LOW_MODEL_NUM);
	// つぼみ
	pModelManager->AddModel(PATH_MODEL_WINTER_BUD, WINTER_BUD_MODEL_NUM);
	// 花
	pModelManager->AddModel(PATH_MODEL_WINTER_FLOWER, WINTER_FLOWER_MODEL_NUM);

	/* エフェクト(エフェクシア)読み込み */
	EffectManager* pEffectManager = EffectManager::GetInstance();
	// つらら破壊エフェクト
	pEffectManager->AddEffect(PATH_EFFECT_ICICLE, ICICLE_EFFECT_NUM);
	// マウスエフェクト
	pEffectManager->AddEffect(PATH_CURSOR_EFFECT_WINTER, CURSOR_EFFECT_WINTER_NUM);


	// プレイヤー情報の取得
	Object* pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	// 背景を2個並べる
	for (int i = 0; i < 2; i++)
	{
#pragma region	---背景(0
		//	オブジェクト生成
		Object* objBG0 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto transBG0 = objBG0->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG0 = objBG0->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG0scroll = objBG0->AddComponent<CTexScroll>();				//	UVスクロール
		auto BG0Flash = objBG0->AddComponent<CFlash>();						//　光の処理
		//	オブジェクトの設定
		transBG0->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG0->SetTexture(pTexManager->GetTexture(BG_D0_TEX_NUM));		//	テクスチャ
		Draw_BG0->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG0scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG0scroll->SetScrollValue(0.0016f);									//	スクロール量の設定
		BG0Flash->SetAlphaTime(15);											//　描画時間(引数は秒数指定)
		BG0Flash->SetNoneTime(8);											//　描画しない時間(引数は秒数指定)
		//	リストに追加
		m_pObjectManager->AddObject(objBG0);
#pragma endregion

#pragma region	---背景(1
		//	オブジェクト生成
		Object* objBG1 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto transBG1 = objBG1->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG1 = objBG1->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG1scroll = objBG1->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		transBG1->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG1->SetTexture(pTexManager->GetTexture(BG_D1_TEX_NUM));		//	テクスチャ
		//Draw_BG1->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG1->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG1scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG1scroll->SetScrollValue(0.0015f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG1);
#pragma endregion

#pragma region	---背景(2
		//	オブジェクト生成
		Object* objBG2 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto TransBG2 = objBG2->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG2 = objBG2->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG2scroll = objBG2->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		TransBG2->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG2->SetTexture(pTexManager->GetTexture(BG_D2_TEX_NUM));		//	テクスチャ
		//Draw_BG2->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG2->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG2scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG2scroll->SetScrollValue(0.0013f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG2);
#pragma endregion

#pragma region	---背景(3
		//	オブジェクト生成
		Object* objBG3 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto TransBG3 = objBG3->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG3 = objBG3->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG3scroll = objBG3->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		TransBG3->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG3->SetTexture(pTexManager->GetTexture(BG_D3_TEX_NUM));		//	テクスチャ
		Draw_BG3->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG3->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG3scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG3scroll->SetScrollValue(0.0011f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG3);
#pragma endregion

#pragma region	---背景(4
		//	オブジェクト生成
		Object* objBG4 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto TransBG4 = objBG4->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG4 = objBG4->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG4scroll = objBG4->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		TransBG4->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG4->SetTexture(pTexManager->GetTexture(BG_D4_TEX_NUM));		//	テクスチャ
		//Draw_BG4->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG4->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG4scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG4scroll->SetScrollValue(0.001f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG4);
#pragma endregion

#pragma region	---背景(5
		//	オブジェクト生成
		Object* objBG5 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto TransBG5 = objBG5->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG5 = objBG5->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG5scroll = objBG5->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		TransBG5->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG5->SetTexture(pTexManager->GetTexture(BG_D5_TEX_NUM));		//	テクスチャ
		Draw_BG5->SetUVsize(1.0f, 1.0f);									//	UV
		Draw_BG5->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG5scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG5scroll->SetScrollValue(0.0005f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG5);
#pragma endregion

#pragma region	---背景(6
		//	オブジェクト生成
		Object* objBG6 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
		//	コンポーネントの追加
		auto TransBG6 = objBG6->AddComponent<CTransform>();					//	トランスフォーム
		auto Draw_BG6 = objBG6->AddComponent<CDraw2D>();					//	2D描画機能
		auto BG6scroll = objBG6->AddComponent<CTexScroll>();				//	UVスクロール
		//	オブジェクトの設定
		TransBG6->SetPosition(-BG_LENGTH * 0.5f + i * BG_LENGTH, 0.0f);		//	座標
		Draw_BG6->SetTexture(pTexManager->GetTexture(BG_D6_TEX_NUM));		//	テクスチャ
		Draw_BG6->SetSize(BG_LENGTH, BG_LENGTH);							//	サイズ
		BG6scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
		BG6scroll->SetScrollValue(0.0003f);									//	スクロール量
		//	リストに追加
		m_pObjectManager->AddObject(objBG6);
#pragma endregion
	}

#pragma region	---背景(7
	//	オブジェクト生成
	Object* objBG7 = new Object(BG_NAME, UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	auto TransBG7 = objBG7->AddComponent<CTransform>();					//	トランスフォーム
	auto Draw_BG7 = objBG7->AddComponent<CDraw2D>();					//	2D描画機能
	auto BG7scroll = objBG7->AddComponent<CTexScroll>();				//	UVスクロール
	//	オブジェクトの設定
	TransBG7->SetPosition(0.0f, 0.0f);		//	座標
	Draw_BG7->SetTexture(pTexManager->GetTexture(BG_D7_TEX_NUM));		//	テクスチャ
	Draw_BG7->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);							//	サイズ
	BG7scroll->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));	//	X軸の基準をプレイヤーにする
	BG7scroll->SetScrollValue(0.0003f);									//	スクロール量
	//	リストに追加
	m_pObjectManager->AddObject(objBG7);
#pragma endregion

#pragma region ---季節ロゴ
	// オブジェクト生成
	Object* pSeasonLogo = new Object("SeasonLogo", UPDATE_UI, DRAW_UI);
	// コンポーネントの追加
	auto TransSLogo = pSeasonLogo->AddComponent<CTransform>();			// 座標
	auto DrawSLogo = pSeasonLogo->AddComponent<CDraw2D>();				// スクリーン描画
	pSeasonLogo->AddComponent<CFade>();									// 描画の時間指定
	// オブジェクトの設定
	TransSLogo->SetPosition(SEASON_LOGO_X, SEASON_LOGO_Y);					// 座標設定
	DrawSLogo->SetSize(SEASON_LOGO_WIDTH, SEASON_LOGO_HEIGHT);				// 大きさ設定
	// リストに追加
	m_pObjectManager->AddObject(pSeasonLogo);
#pragma endregion

	// テクスチャ設定
	if (SceneGame::GetInstance()->GetSeason() == WINTER_1)
	{
		DrawSLogo->SetTexture(pTexManager->GetTexture(WINTER_1_TEX_NUM));	// テクスチャ設定
	}
	else if (SceneGame::GetInstance()->GetSeason() == WINTER_2)
	{
		DrawSLogo->SetTexture(pTexManager->GetTexture(WINTER_2_TEX_NUM));	// テクスチャ設定
	}

#pragma region	---カーソル
	//	オブジェクト生成
	Object* objCursor = new Object(GAME_CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	objCursor->AddComponent<CTransform>();					//	トランスフォーム
	auto homing = objCursor->AddComponent<CHoming>();		//	マウス追従
	auto collider = objCursor->AddComponent<CCollider>();	//	あたり判定
	auto draw = objCursor->AddComponent<CDraw2D>();			//　描画
	objCursor->AddComponent<CLotation>();					//　回転
	// オブジェクトの設定
	draw->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));	// テクスチャ
	draw->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);						// サイズ
	draw->SetColor(0.0f, 1.0f, 0.0f);
	homing->SetEffectNum(CURSOR_EFFECT_WINTER_NUM);
	collider->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);
	//	リストに追加
	m_pObjectManager->AddObject(objCursor);
#pragma endregion

	//--- BGM再生
	CSound::Play(BGM_WINTER);
}

// 終了処理
void SceneWinter::Uninit()
{
	//	BGM停止
	CSound::Stop(BGM_WINTER);
}

// 更新
void SceneWinter::Update() {
}

// 描画
void SceneWinter::Draw() {
}


