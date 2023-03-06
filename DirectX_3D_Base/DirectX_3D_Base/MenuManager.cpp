#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "MenuComponent.h"
#include "HomingComponent.h"
#include "TextureManager.h"
#include "sceneGame.h"
#include "ComponentSoundConfig.h"
#include "Sound.h"
#include "GaugeComponent.h"
#include "Blink.h"
#include "LotationComponent.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------
#include "sceneTitle.h"
//-----------------------------------------------------------

//===== 静的メンバ変数 =====
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

// インスタンス化
MenuManager* MenuManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new MenuManager;
	}

	return m_pInstance;
}

// インスタンスの破棄
void MenuManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::MenuManager
* @brief	コンストラクタ
*/
MenuManager::MenuManager()
	: m_nDelayCnt(-1), m_nCreateState(STATE_NONE)
	, m_OldMenu_State(STATE_NONE)
{
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	全てのメニューオブジェクトの開放
*/
MenuManager::~MenuManager()
{
	//	オブジェクトリストの削除
	for (auto&& menuObject : m_MenuList)
		delete menuObject;
	m_MenuList.clear();
}

/**
* @fn		MenuManager::~Start
* @brief	初期化
*/
void MenuManager::Start()
{
	//	メニューの初期化
	for (auto&& MenuObject : m_MenuList)
		MenuObject->Start();
}

/**
* @fn		MenuManager::Update
* @brief	全てのオブジェクトの更新
* @detail	各々のオブジェクトが持つコンポーネントの更新
*/
void MenuManager::Update()
{
	if (m_nDelayCnt > 0)
	{
		// メニュー作成を遅らせる
		m_nDelayCnt--;

		if (m_nDelayCnt == 0)
		{
			switch (m_nCreateState)
			{
			case STATE_TITLE:	 CreateTitleMenu(); break;
			case STATE_OPTION:	  break;
			case STATE_SELECT:	 CreateSelectMenu(); break;
			case STATE_GAMEOVER: CreateGameoverMenu(); break;
			default:
				break;
			}

			m_nDelayCnt = -1;
		}
	}

	//	メニューオブジェクトの更新
	for (auto&& object : m_MenuList)
	{
		object->Update();
	}

	//	Colliderの更新
	CCollider::GetColliderList().remove_if([](Object* pObj)
		{
			return pObj->GetDeleteFlag();
		});

	//	オブジェクト削除の確認
	m_MenuList.remove_if([](Object* pObj)
		{
			return pObj->GetDeleteFlag();
		});
}

/**
* @fn		MenuManager::DrawMenu
* @brief	メニューオブジェクトの描画
* @detail	各々のオブジェクトが持つコンポーネントの描画
*/
void MenuManager::Draw()
{
	SetZBuffer(true);
	//	メニューオブジェクト描画
	for (auto&& menuObject : m_MenuList)
		menuObject->Draw();
}

//  指定フレーム後にメニュー作成
void MenuManager::Create(int state, int delayCnt)
{
	m_nCreateState = state;

	m_nDelayCnt = delayCnt;
}

// タイトルメニュー表示
void MenuManager::CreateTitleMenu()
{
	// オブジェクトリストの削除
	DeleteMenu();

#pragma region	---カーソル
	//	オブジェクト生成
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transCursor = pCursor->AddComponent<CTransform>();	//	トランスフォーム
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D描画機能
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	マウス追従
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	衝突処理
	pCursor->AddComponent<CLotation>();							// 回転
	
	// オブジェクト設定
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	色
	//	オブジェクトの設定
	transCursor->SetPosition(0.0f, 0.0f);						//	座標
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
	
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	this->AddMenu(pCursor);


#pragma endregion

#pragma region ---はじめから
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//---初めからの枠
	//オブジェクトの作成
	Object* pBeginning_Frame = new Object("", UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransBeginning_Frame = pBeginning_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawBeginning_Frame = pBeginning_Frame->AddComponent<CDraw2D>();					//描画
	//オブジェクトの設定
	TransBeginning_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y + 150.0f);	//座標の設定
	DrawBeginning_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(NEW_GAME_FRAME_TEX_NUM));
	DrawBeginning_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pBeginning_Frame);
	//----------------------------------------------------------------------------

	Object* pBeginning = new Object(NEW_GAME_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transBeginning = pBeginning->AddComponent<CTransform>();		// トランスフォーム
	auto DrawBeginning = pBeginning->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderBeginning = pBeginning->AddComponent<CCollider>();		//	衝突処理
	auto Beginning = pBeginning->AddComponent<CMenu>();					//	
	// オブジェクトの設定
	transBeginning->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y + 150.0f);	//	座標
	DrawBeginning->SetTexture(TextureManager::GetInstance()->GetTexture(NEW_GAME_TEX_NUM));//	テクスチャ
	DrawBeginning->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	サイズ
	ColliderBeginning->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pBeginning);
	// ゲームの進行状況の初期化が必要
#pragma endregion

#pragma region ---続きから
	
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//---続きからの枠
	//オブジェクトの作成
	Object* pContinuity_Frame = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI_FRAME);	//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransContinuity_Frame = pContinuity_Frame->AddComponent<CTransform>();	//トランスフォーム
	auto DrawContinuity_Frame = pContinuity_Frame->AddComponent<CDraw2D>();		//描画
	//オブジェクトの設定
	TransContinuity_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y);	//座標の設定
	DrawContinuity_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_FRAME_TEX_NUM));
	DrawContinuity_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);					//サイズ
	//メニューリストに追加
	AddMenu(pContinuity_Frame);
	//----------------------------------------------------------------------------
	Object* pContinuity = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transContinuity = pContinuity->AddComponent<CTransform>();		// トランスフォーム
	auto DrawContinuity = pContinuity->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderContinuity = pContinuity->AddComponent<CCollider>();	//	衝突処理
	auto Continuity = pContinuity->AddComponent<CMenu>();				//	マウス追従
	// オブジェクトの設定
	transContinuity->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y);				//	座標
	DrawContinuity->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_TEX_NUM));	//	テクスチャ
	DrawContinuity->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	サイズ
	ColliderContinuity->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);		//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pContinuity);
#pragma endregion

#pragma region  ---やめる
	//2022/1/10 Shimizu Yosuke ---------------------------------------------------
	//オプションの枠
	//オブジェクトの作成
	Object* pOption_Frame = new Object(EXIT_GAME_NAME, UPDATE_UI, DRAW_UI_FRAME);		//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransOption_Frame = pOption_Frame->AddComponent<CTransform>();		//トランスフォーム
	auto DrawOption_Frame = pOption_Frame->AddComponent<CDraw2D>();			//描画
	//枠だから衝突判定はいらない
	//メニューコンポーネントも必要ない
	//オブジェクトの設定
	TransOption_Frame->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y - 150.0f);	//座標の設定
	DrawOption_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(EXIT_FRAME_TEX_NUM));
	DrawOption_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pOption_Frame);
	//----------------------------------------------------------------------------

	Object* pExsit = new Object(EXIT_GAME_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transExsit = pExsit->AddComponent<CTransform>();		// トランスフォーム
	auto DrawExsit = pExsit->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderExsit = pExsit->AddComponent<CCollider>();	//	衝突処理
	auto Exsit = pExsit->AddComponent<CMenu>();		//	マウス追従
	// オブジェクトの設定
	transExsit->SetPosition(TITLEMENU_POS_X, TITLEMENU_POS_Y - 150.0f);//	座標
	DrawExsit->SetTexture(TextureManager::GetInstance()->GetTexture(EXIT_TEX_NUM));							//	テクスチャ
	DrawExsit->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	サイズ
	ColliderExsit->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pExsit);
#pragma endregion

#pragma region ---オプション
	//2022/1/19 Shimizu Shogo ---------------------------------------------------
	//オブジェクトの作成
	Object* pOption_Icon = new Object(OPTION_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto TransOption_Icon = pOption_Icon->AddComponent<CTransform>();	// 座標
	auto DrawOption_Icon = pOption_Icon->AddComponent<CDraw2D>();		// 描画
	auto ColliderOption = pOption_Icon->AddComponent<CCollider>();				// 衝突処理
	auto Option = pOption_Icon->AddComponent<CMenu>();							// マウス追従
	//オブジェクトの設定
	TransOption_Icon->SetPosition(TITLEMENU_POS_X + 250.0f, TITLEMENU_POS_Y + 300.0f);
	DrawOption_Icon->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));
	DrawOption_Icon->SetSize(MENU_WIDTH - 350.0f, MENU_HEIGHT - 150.0f);
	ColliderOption->SetCollisionSize(MENU_COLLISION_WIDTH - 350.0f, MENU_COLLISION_HEIGHT);
	
	//メニューリストに追加
	AddMenu(pOption_Icon);
#pragma endregion


	//CreateTitleMenuの一番下に追加
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	m_OldMenu_State = STATE_TITLE;
	//------------------------------------------------------------------
}

// ポーズメニュー表示
void MenuManager::CreatePauseMenu()
{
	// オブジェクトリストの削除
	DeleteMenu();

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
	//	オブジェクトの設定
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	色
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	座標							//	テクスチャ
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
									//	透明度
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	this->AddMenu(pCursor);
#pragma endregion

#pragma region ---つづける
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//つづけるの枠----------------------------------------------------------------
	//オブジェクトの作成
	Object* pContinue_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransContinue_Frame = pContinue_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawContinue_Frame = pContinue_Frame->AddComponent<CDraw2D>();					//描画
	//枠だから衝突判定はいらない
	//メニューコンポーネントも必要ない
	//オブジェクトの設定
	TransContinue_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 225.0f);	//座標の設定
	DrawContinue_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_FRAME_TEX_NUM));
	DrawContinue_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pContinue_Frame);
	//----------------------------------------------------------------------------

	Object* pContinue = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transContinue = pContinue->AddComponent<CTransform>();		// トランスフォーム
	auto DrawContinue = pContinue->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderContinue = pContinue->AddComponent<CCollider>();	//	衝突処理
	auto Continue = pContinue->AddComponent<CMenu>();				//	
	// オブジェクトの設定
	transContinue->SetPosition(MENU_POS_X, MENU_POS_Y + 225.0f);	//	座標
	DrawContinue->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_TEX_NUM));	//	テクスチャ
	DrawContinue->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	サイズ
	ColliderContinue->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pContinue);
#pragma endregion

#pragma region ---リスタート
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//リスタートの枠--------------------------------------------------------------
	//オブジェクトの作成
	Object* pRestart_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransRestart_Frame = pRestart_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawRestart_Frame = pRestart_Frame->AddComponent<CDraw2D>();					//描画
	//枠だから衝突判定はいらない
	//メニューコンポーネントも必要ない
	//オブジェクトの設定
	TransRestart_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 75.0f);	//座標の設定
	DrawRestart_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_FRAME_TEX_NUM));
	DrawRestart_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pRestart_Frame);
	//----------------------------------------------------------------------------

	Object* pRestart = new Object(RESTART_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transRestart = pRestart->AddComponent<CTransform>();		// トランスフォーム
	auto DrawRestart = pRestart->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderRestart = pRestart->AddComponent<CCollider>();		//	衝突処理
	auto Restart = pRestart->AddComponent<CMenu>();					//	マウス追従
	// オブジェクトの設定
	transRestart->SetPosition(MENU_POS_X, MENU_POS_Y + 75.0f);		//	座標
	DrawRestart->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_TEX_NUM));//	テクスチャ
	DrawRestart->SetSize(MENU_WIDTH, MENU_HEIGHT);					//	サイズ
	ColliderRestart->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pRestart);
#pragma endregion

#pragma region  ---セレクト
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//セレクトの枠----------------------------------------------------------------
	//オブジェクトの作成
	Object* pSelect_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransSelect_Frame = pSelect_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawSelect_Frame = pSelect_Frame->AddComponent<CDraw2D>();					//描画
	//枠だから衝突判定はいらない
	//メニューコンポーネントも必要ない

	//オブジェクトの設定
	TransSelect_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 75.0f);	//座標の設定
	DrawSelect_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_FRAME_TEX_NUM));
	DrawSelect_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ

	//メニューリストに追加
	AddMenu(pSelect_Frame);

	//----------------------------------------------------------------------------

	Object* pMenu = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transMenu = pMenu->AddComponent<CTransform>();		// トランスフォーム
	auto DrawMenu = pMenu->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderMenu = pMenu->AddComponent<CCollider>();	//	衝突処理
	auto Menu = pMenu->AddComponent<CMenu>();				//	マウス追従
	// オブジェクトの設定
	transMenu->SetPosition(MENU_POS_X, MENU_POS_Y - 75.0f);//	座標
	DrawMenu->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_TEX_NUM));//	テクスチャ
	DrawMenu->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	サイズ
	ColliderMenu->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pMenu);
#pragma endregion

#pragma region  ---せってい
	//2022/1/21 RYOHEI SAWADA-----------------------------------------------------
	//せっていの枠----------------------------------------------------------------
	//オブジェクトの作成
	Object* pOption_Frame = new Object("", UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransOption_Frame = pOption_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawOption_Frame = pOption_Frame->AddComponent<CDraw2D>();					//描画
	//オブジェクトの設定
	TransOption_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 225.0f);	//座標の設定
	DrawOption_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_FRAME_TEX_NUM));
	DrawOption_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pOption_Frame);
	//----------------------------------------------------------------------------

	Object* pOption = new Object(OPTION_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transOption = pOption->AddComponent<CTransform>();		// トランスフォーム
	auto DrawOption = pOption->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderOption = pOption->AddComponent<CCollider>();	//	衝突処理
	auto Option = pOption->AddComponent<CMenu>();				//	マウス追従
	// オブジェクトの設定
	transOption->SetPosition(MENU_POS_X, MENU_POS_Y - 225.0f);//	座標
	DrawOption->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_TEX_NUM));//	テクスチャ
	DrawOption->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	サイズ
	ColliderOption->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pOption);
#pragma endregion

}

// ステージセレクト表示
void MenuManager::CreateSelectMenu()
{
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
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	色
	//	オブジェクトの設定
	transCursor->SetPosition(MousePos.x, MousePos.y);						//	座標
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	this->AddMenu(pCursor);
#pragma endregion

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
	AddMenu(pTitle_Frame);
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
	AddMenu(pTitle);
#pragma endregion

	//CreateTitleMenuの一番下に追加
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------
	m_OldMenu_State = STATE_SELECT;
	//------------------------------------------------------------------
}

// ゲームオーバー表示
void MenuManager::CreateGameoverMenu()
{
	// ポーズ
	SceneGame::GetInstance()->m_bPauseMode = true;

	//DeleteMenu();

#pragma region ---暗くする
	Object* pDark = new Object("black_out", UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto transDark = pDark->AddComponent<CTransform>();	// オブジェクト座標
	auto drawDark = pDark->AddComponent<CDraw2D>();		// スクリーン描画
	// settings
	transDark->SetPosition(0.0f, 0.0f);					// 座標
	drawDark->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// サイズ
	drawDark->SetColor(0.0f, 0.0f, 0.0f);				// 色
	drawDark->SetAlpha(0.5f);							// 透明度
	// add to list
	AddMenu(pDark);
#pragma endregion

#pragma region	---カーソル
	//	オブジェクト生成
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transCursor = pCursor->AddComponent<CTransform>();	//	トランスフォーム
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D描画機能
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	マウス追従
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	衝突処理
	pCursor->AddComponent<CLotation>();							// 回転
	// オブジェクト設定
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	色
	//	オブジェクトの設定
	transCursor->SetPosition(0.0f, 0.0f);						//	座標
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	// リストに追加
	AddMenu(pCursor);
#pragma endregion

#pragma region ---リスタート
	//2022/1/22 RYOHEI SAWADA-----------------------------------------------------
	//リスタートの枠--------------------------------------------------------------
	//オブジェクトの作成
	Object* pRestart_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransRestart_Frame = pRestart_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawRestart_Frame = pRestart_Frame->AddComponent<CDraw2D>();					//描画
	//オブジェクトの設定
	TransRestart_Frame->SetPosition(MENU_POS_X, MENU_POS_Y + 100.0f);	//座標の設定
	DrawRestart_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_FRAME_TEX_NUM));
	DrawRestart_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pRestart_Frame);
	//----------------------------------------------------------------------------

	Object* pRestart = new Object(RESTART_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transRestart = pRestart->AddComponent<CTransform>();	// トランスフォーム
	auto DrawRestart = pRestart->AddComponent<CDraw2D>();		//	2D描画機能
	auto ColliderRestart = pRestart->AddComponent<CCollider>();	//	衝突処理
	auto Restart = pRestart->AddComponent<CMenu>();				//	メニュー機能
	// オブジェクトの設定
	transRestart->SetPosition(MENU_POS_X, MENU_POS_Y + 100.0f);	//	座標
	DrawRestart->SetTexture(TextureManager::GetInstance()->GetTexture(RESTART_TEX_NUM));							//	テクスチャ
	DrawRestart->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	サイズ
	ColliderRestart->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);	//	あたり判定の大きさ
	AddMenu(pRestart);
#pragma endregion


#pragma region  ---ステージセレクト
	//2022/1/22 RYOHEI SAWADA-----------------------------------------------------
	//セレクトの枠----------------------------------------------------------------
	//オブジェクトの作成
	Object* pSelect_Frame = new Object(CONTINUE_NAME, UPDATE_UI, DRAW_UI_FRAME);				//名前は変える必要があるかも
	//コンポーネントの追加
	auto TransSelect_Frame = pSelect_Frame->AddComponent<CTransform>();				//トランスフォーム
	auto DrawSelect_Frame = pSelect_Frame->AddComponent<CDraw2D>();					//描画
	//オブジェクトの設定
	TransSelect_Frame->SetPosition(MENU_POS_X, MENU_POS_Y - 100.0f);	//座標の設定
	DrawSelect_Frame->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_FRAME_TEX_NUM));
	DrawSelect_Frame->SetSize(MENU_WIDTH, MENU_HEIGHT);							//サイズ
	//メニューリストに追加
	AddMenu(pSelect_Frame);
	//----------------------------------------------------------------------------

	Object* pMenu = new Object(SELECT_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transMenu = pMenu->AddComponent<CTransform>();		// トランスフォーム
	auto DrawMenu = pMenu->AddComponent<CDraw2D>();			//	2D描画機能
	auto ColliderMenu = pMenu->AddComponent<CCollider>();	//	衝突処理
	auto Menu = pMenu->AddComponent<CMenu>();		//	マウス追従
	// オブジェクトの設定
	transMenu->SetPosition(MENU_POS_X, MENU_POS_Y - 100.0f);//	座標
	DrawMenu->SetTexture(TextureManager::GetInstance()->GetTexture(SELECT_TEX_NUM));							//	テクスチャ
	DrawMenu->SetSize(MENU_WIDTH, MENU_HEIGHT);				//	サイズ
	ColliderMenu->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	AddMenu(pMenu);
#pragma endregion
}




/**
* @fn		MenuManager::AddMenu
* @brief	メニューリストに追加
* @param	(Object*) 追加するオブジェクト
*/
void MenuManager::AddMenu(Object* pMenuObject)
{
	//	更新リストに追加
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList)
	{
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	//	メニューリストに追加
	m_MenuList.insert(itr, pMenuObject);


	// オブジェクト初期化
	pMenuObject->Start();
}

// メニューリストの取得
std::list<Object*>& MenuManager::GetMenuList()
{
	return m_MenuList;
}

// メニューリストを空にする
void MenuManager::DeleteMenu()
{
	//	オブジェクトリストの削除
	for (auto&& menuObject : m_MenuList)
		menuObject->Delete();

	m_MenuList.clear();
}



// オプション
void MenuManager::CreateOptionMenu()
{
	// すでにオプションを開いていたら作らない
	if (m_bOption) return;

	// オプションフラグON
	m_bOption = true;

	TextureManager* pTex = TextureManager::GetInstance();

#pragma region ---暗くする
	Object* pDark = new Object("black_out", UPDATE_OPTION, DRAW_OPTION_BLACK_OUT);
	// components
	auto transDark = pDark->AddComponent<CTransform>();	// オブジェクト座標
	auto drawDark = pDark->AddComponent<CDraw2D>();		// スクリーン描画
	// settings
	transDark->SetPosition(0.0f, 0.0f);					// 座標
	drawDark->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// サイズ
	drawDark->SetColor(0.0f, 0.0f, 0.0f);				// 色
	drawDark->SetAlpha(0.5f);							// 透明度
	// add to list
	AddMenu(pDark);
#pragma endregion

#pragma region ---BGM
	Object* pBGM = new Object("", UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBGM = pBGM->AddComponent<CTransform>();	// オブジェクト座標
	auto drawBGM = pBGM->AddComponent<CDraw2D>();		// スクリーン描画
	// settings
	transBGM->SetPosition(-300.0f, BGM_POS_Y);			// 座標
	drawBGM->SetSize(MENU_WIDTH, MENU_HEIGHT);			// サイズ
	drawBGM->SetTexture(pTex->GetTexture(BGM_TEX_NUM));	// テクスチャ
	// add to list
	AddMenu(pBGM);
#pragma endregion

#pragma region ---BGMゲージ
	Object* pBGM_G = new Object("gauge_bgm", UPDATE_OPTION, DRAW_OPTION_FRAME);
	// components
	auto transBGM_G = pBGM_G->AddComponent<CTransform>();	// オブジェクト座標
	auto drawBGM_G = pBGM_G->AddComponent<CDraw2D>();		// スクリーン描画
	auto gaugeBGM_G = pBGM_G->AddComponent<CGauge>();		// ゲージ
	// settings
	transBGM_G->SetPosition(MENU_POS_X, BGM_POS_Y);			// 座標
	drawBGM_G->SetSize(STICK_WIDTH, STICK_HEIGHT);			// サイズ
	drawBGM_G->SetTexture(pTex->GetTexture(CONFIG_GAUGE_TEX_NUM));	// テクスチャ
	drawBGM_G->SetAnimSplit(1, 3);							// 分割数
	drawBGM_G->SetAnimNum(0);								// アニメーション番号
	gaugeBGM_G->SetInitPos(MENU_POS_X, BGM_POS_Y);			// 初期座標
	gaugeBGM_G->SetMaxGauge((int)STICK_WIDTH);				// 最大ゲージ
	gaugeBGM_G->SetSize(STICK_WIDTH, STICK_HEIGHT);			// ゲージサイズ
	// add to list
	AddMenu(pBGM_G);
#pragma endregion

#pragma region ---BGM調整のバー
	Object* pBGM_B = new Object(BGM_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBGM_B		= pBGM_B->AddComponent<CTransform>();		// オブジェクト座標
	auto drawBGM_B		= pBGM_B->AddComponent<CDraw2D>();			// スクリーン描画
	auto colliderBGM_B	= pBGM_B->AddComponent<CCollider>();		// あたり判定
	auto configBGM_B	= pBGM_B->AddComponent<CSoundConfig>();		// サウンド調整
	// settings
	float posBGM_B = GAUGE_LEFT + CSound::GetVolume_BGM() * STICK_WIDTH;// 現在音量によって座標を変える
	transBGM_B->SetPosition(posBGM_B, BGM_POS_Y);					// 座標
	drawBGM_B->SetTexture(nullptr);									// テクスチャ
	drawBGM_B->SetSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);	// サイズ
	colliderBGM_B->SetCollisionSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);	// あたり判定の大きさ
	configBGM_B->SetSoundType(CONFIG_BGM);							// 調整タイプ
	configBGM_B->SetGauge(gaugeBGM_G);								// ゲージの設定
	// add to list
	AddMenu(pBGM_B);
#pragma endregion

#pragma region ---SE
	Object* pSE = new Object("", UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transSE = pSE->AddComponent<CTransform>();		// オブジェクト座標
	auto drawSE = pSE->AddComponent<CDraw2D>();			// スクリーン描画
	// settings
	transSE->SetPosition(-300.0f, SE_POS_Y);			// 座標
	drawSE->SetSize(MENU_WIDTH, MENU_HEIGHT);			// サイズ
	drawSE->SetTexture(pTex->GetTexture(SE_TEX_NUM));	// テクスチャ
	// add to list
	AddMenu(pSE);
#pragma endregion

#pragma region ---SEゲージ
	Object* pSE_G = new Object("", UPDATE_OPTION, DRAW_OPTION_FRAME);
	// components
	auto transSE_G = pSE_G->AddComponent<CTransform>();	// オブジェクト座標
	auto drawSE_G  = pSE_G->AddComponent<CDraw2D>();	// スクリーン描画
	auto gaugeSE_G = pSE_G->AddComponent<CGauge>();		// ゲージ
	// settings
	transSE_G->SetPosition(MENU_POS_X, SE_POS_Y);		// 座標
	drawSE_G->SetSize(STICK_WIDTH, STICK_HEIGHT);		// サイズ
	drawSE_G->SetTexture(pTex->GetTexture(CONFIG_GAUGE_TEX_NUM));	// テクスチャ
	drawSE_G->SetAnimSplit(1, 3);						// 分割数
	drawSE_G->SetAnimNum(0);							// アニメーション番号
	gaugeSE_G->SetInitPos(MENU_POS_X, SE_POS_Y);		// 初期座標
	gaugeSE_G->SetMaxGauge((int)STICK_WIDTH);			// 最大ゲージ
	gaugeSE_G->SetSize(STICK_WIDTH, STICK_HEIGHT);		// ゲージサイズ
	// add to list
	AddMenu(pSE_G);
#pragma endregion

#pragma region ---SE調整のバー
	Object* pSE_B = new Object(SE_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transSE_B		= pSE_B->AddComponent<CTransform>();		// オブジェクト座標
	auto drawSE_B		= pSE_B->AddComponent<CDraw2D>();			// スクリーン描画
	auto colliderSE_B	= pSE_B->AddComponent<CCollider>();			// あたり判定
	auto configSE_B		= pSE_B->AddComponent<CSoundConfig>();		// サウンド調整
	// settings
	float posSE_B = GAUGE_LEFT + CSound::GetVolume_SE() * STICK_WIDTH;// 現在音量によって座標を変える
	transSE_B->SetPosition(posSE_B, SE_POS_Y);						// 座標
	drawSE_B->SetTexture(nullptr);									// テクスチャ
	drawSE_B->SetSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);		// サイズ
	colliderSE_B->SetCollisionSize(CONFIG_STICK_WIDTH, CONFIG_STICK_HEIGHT);// あたり判定の大きさ
	configSE_B->SetSoundType(CONFIG_SE);							// 調整タイプ
	configSE_B->SetGauge(gaugeSE_G);								// ゲージの設定
	// add to list
	AddMenu(pSE_B);
#pragma endregion

#pragma region ---戻るボタン
	Object* pBack = new Object(BACK_NAME, UPDATE_OPTION, DRAW_OPTION);
	// components
	auto transBack		= pBack->AddComponent<CTransform>();	// オブジェクトの座標
	auto drawBack		= pBack->AddComponent<CDraw2D>();		// スクリーン描画
	auto colliderBack	= pBack->AddComponent<CCollider>();		// あたり判定
	auto menuBack		= pBack->AddComponent<CMenu>();			// メニュー機能
	// settings
	transBack->SetPosition(350.0f, -250.0f);					// 座標
	drawBack->SetSize(MENU_WIDTH, MENU_HEIGHT);					// サイズ
	colliderBack->SetCollisionSize(MENU_COLLISION_WIDTH, MENU_COLLISION_HEIGHT);// あたり判定の大きさ
	drawBack->SetTexture(pTex->GetTexture(BACK_TEX_NUM));		// テクスチャ
	// add to list
	AddMenu(pBack);
#pragma endregion

	// 現在のボリュームをゲージにも反映させる
	CGauge::SetGauge(gaugeBGM_G, (int)(CSound::GetVolume_BGM() * STICK_WIDTH));
	CGauge::SetGauge(gaugeSE_G,  (int)(CSound::GetVolume_SE()  * STICK_WIDTH));
	gaugeBGM_G->Update();
	gaugeSE_G->Update();
}

// オプション削除
void MenuManager::DeleteOptionMenu()
{
	// オプションフラグOFF
	m_bOption = false;

	for (auto&& obj : m_MenuList)
	{
		// オプションのオブジェクトだった場合
		if (obj->GetUpdateOrder() == UPDATE_OPTION)
		{
			// 削除
			obj->Delete();
		}
	}
}



//2022/1/19/Tutorial Shimizu Yosuke -------------------------------
void MenuManager::CreateTutorialMenu() {

	//チュートリアル用の画像を出しまする
	DeleteMenu();

#pragma region	---カーソル
	//	オブジェクト生成
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transCursor = pCursor->AddComponent<CTransform>();	//	トランスフォーム
	auto DrawCursor = pCursor->AddComponent<CDraw2D>();		//	2D描画機能
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	マウス追従
	auto ColliderCursor = pCursor->AddComponent<CCollider>();	//	衝突処理
	pCursor->AddComponent<CLotation>();							// 回転
	// オブジェクト設定
	DrawCursor->SetTexture(TextureManager::GetInstance()->GetTexture(STOP_GAME_TEX_NUM));		//	テクスチャ
	DrawCursor->SetColor(0.2f, 0.2f, 0.6f);						//	色
	transCursor->SetPosition(0.0f, 0.0f);						//	座標
	DrawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);			//	サイズ
	ColliderCursor->SetCollisionSize(CURSOR_WIDTH, CURSOR_HEIGHT);//	あたり判定の大きさ
	// メニューリストに追加
	this->AddMenu(pCursor);
#pragma endregion

#pragma region ---チュートリアル看板
	//オブジェクトの生成
	Object* pTutorial = new Object(TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto TransTutorial = pTutorial->AddComponent<CTransform>();				//トランスフォーム
	auto DrawTutorial = pTutorial->AddComponent <CDraw2D>();				//描画
	auto CollTutorial = pTutorial->AddComponent<CCollider>();				//衝突判定
	pTutorial->AddComponent<CMenu>();										//メニューの機能の追加

	//オブジェクトの設定
	TransTutorial->SetPosition(0.0f, 55.0f);									//座標
	DrawTutorial->SetSize(TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y);			//大きさ

	//2022/1/24/TUTORIAL_UI Shimizu Yosuke ------------------------------------------------------------
	//セットするテクスチャを変える
	switch (SceneGame::GetInstance()->GetSeason()) {
	case SPRING_1:
	case SPRING_2:
		//ここからさらに分ける UI or 挙動


		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(PLAYER_TUTORIAL_TEX_NUM));//テクスチャ
		break;
	case SUMMER_1:
	case SUMMER_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_SPIDER_TEX_NUM));//テクスチャ
		break;
	case FALL_1:
	case FALL_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_FALL_TEX_NUM));//テクスチャ
		break;
	case WINTER_1:
	case WINTER_2:
		DrawTutorial->SetTexture(TextureManager::GetInstance()->GetTexture(TUTORIAL_WINTER_TEX_NUM));//テクスチャ
		break;
	}
	//-------------------------------------------------------------------------------------------------


	CollTutorial->SetCollisionSize(SCREEN_WIDTH , 1000.0f);	//当たり判定の大きさ

	//リストに追加
	MenuManager::GetInstance()->AddMenu(pTutorial);
#pragma endregion

	//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------------------------------------
#pragma region ---クリックして続ける
	//オブジェクト生成
	Object* pClick_Text = new Object(CLICK_TEXT_NAME, UPDATE_UI, DRAW_UI_FRAME);
	//コンポーネントの追加
	auto TransClick = pClick_Text->AddComponent<CTransform>();					//トランスフォーム
	auto DrawClick = pClick_Text->AddComponent<CDraw2D>();						//描画
	//点滅するコンポーネントの追加
	pClick_Text->AddComponent<CBlink>();
	//オブジェクトの設定
	TransClick->SetPosition(CLICK_TEXT_POS_X, CLICK_TEXT_POS_Y);					//座標
	DrawClick->SetSize(CLICK_TEXT_SIZE_X, CLICK_TEXT_SIZE_Y);						//サイズ
	DrawClick->SetTexture(TextureManager::GetInstance()->GetTexture(CLICK_TEX_NUM));//テクスチャ
	//リストに追加
	MenuManager::GetInstance()->AddMenu(pClick_Text);
#pragma endregion
	//-------------------------------------------------------------------------------------
}
//-----------------------------------------------------------------


//2022/1/31/Load_Tutorial Shimizu Yosuke --------------------------------------------------
//シーンをゲットする関数
MENU_STATE MenuManager::GetMenu() {
	return m_OldMenu_State;
}

//シーンをセットする関数
void MenuManager::SetMenu(MENU_STATE State_scene) {
	m_OldMenu_State = State_scene;
}
//-----------------------------------------------------------------------------------------

