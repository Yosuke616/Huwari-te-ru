//===== インクルード　=====
#include "Load.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "LotationComponent.h"
#include "EffectManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "HomingComponent.h"

//2022/1/31/Load_Tutorial Shimizu Yosuke ------------------------------------
#include "ObjectInfo.h"
#include "TutorialComponent.h"
//---------------------------------------------------------------------------

#include <process.h>

//===== マクロ定 =====
#define LOAD_FRAME		(1)		// シーンの初期化が一瞬で終わった場合待ってもらう秒数

#define WIDTH_WHEEL		(200.0f)
#define HEIGHT_WHEEL	(200.0f)
#define POS_X_WHEEL		(500.0f)
#define POS_Y_WHEEL		(-250.0f)

//=== 静的メンバ変数
HANDLE Load::m_handle;			// スレッドハンドル
bool Load::m_bLoading = false;	// ロード中
int Load::m_nLoadCount = 0;		// ロード時間のカウント
std::vector<Object*> Load::m_LoadScreenObjects;	// ロード画面のオブジェクトリスト


// コンストラクタ
Load::Load(){
}
// デストラクタ
Load::~Load(){
}

// ロード画面の初期化
void Load::Init()
{
	CCamera::Get()->Init();

	// テクスチャ読み込み
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_STOP_GAME, STOP_GAME_TEX_NUM);


#pragma region ---背景
	//	オブジェクト生成
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	コンポーネントの追加
	objBG->AddComponent<CTransform>();				// 座標
	auto Draw_BG = objBG->AddComponent<CDraw2D>();	// 描画
	Draw_BG->SetColor(0.0f, 0.0f, 0.0f);			// 色
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// サイズ
	//	リストに追加
	m_LoadScreenObjects.push_back(objBG);
#pragma endregion

#pragma region ---ローディングシルフィ
	//	オブジェクト生成
	Object* wheel = new Object("wheel", UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto Trans = wheel->AddComponent<CTransform>();				//	座標
	auto pDraw = wheel->AddComponent<CDrawEffect2D>();			//	描画
	//auto pLotate = wheel->AddComponent<CLotation>();			//	描画
	//	オブジェクトの設定
	Trans->SetPosition(POS_X_WHEEL, POS_Y_WHEEL);				//	座標の設定
	pDraw->SetTexture(pTexManager->GetTexture(LOAD_SILFY_TEX_NUM));	//	テクスチャの設定
	pDraw->SetSize(WIDTH_WHEEL, HEIGHT_WHEEL);					//	サイズの設定
	pDraw->SetLoop(true);
	pDraw->SetAnimSplit(8, 8);
	pDraw->SetSwapFrame(3);
	pDraw->SetColor(0.4f, 1.0f, 0.4f);
	//	リストに追加
	m_LoadScreenObjects.push_back(wheel);
#pragma endregion

#pragma region	---カーソル
	//	オブジェクト生成
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	コンポーネントの追加
	auto transCursor = pCursor->AddComponent<CTransform>();	//	トランスフォーム
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	マウス追従
	auto drawCursor = pCursor->AddComponent<CDraw2D>();		// 描画
	pCursor->AddComponent<CLotation>();				// 回転

	drawCursor->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));
	drawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);
	drawCursor->SetColor(1.0f, 1.0f, 1.0f);
	//	オブジェクトの設定
	transCursor->SetPosition(0.0f, 0.0f);						//	座標
	homingCursor->SetEffectNum(CURSOR_EFFECT_NUM);
	// リストに追加
	m_LoadScreenObjects.push_back(pCursor);
#pragma endregion


	//2022/1/31/Load_Tutorial Shimizu Yosuke -----------------------------
#pragma region ---チュートリアル
	//オブジェクトの生成
	Object* pLoad_Tutorial = new Object(LOAD_TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto TransTutorial = pLoad_Tutorial->AddComponent<CTransform>();			//トランスフォーム
	auto DrawTutorial = pLoad_Tutorial->AddComponent<CDraw2D>();				//2D描画
	//多分何かしらのコンポーネントの追加が必要
	pLoad_Tutorial->AddComponent<CTutorial>();	//チュートリアル
	//オブジェクトの設定
	TransTutorial->SetPosition(-75.0f, 75.0f);										//座標の設定
	DrawTutorial->SetTexture(pTexManager->GetTexture(TUTORIAL_UI_TEX_NUM));			//テクスチャの設定
	DrawTutorial->SetSize(TUTORIAL_SIZE_X - 40.0f, TUTORIAL_SIZE_Y - 40.0f);			//大きさ
	//リストに追加
	m_LoadScreenObjects.push_back(pLoad_Tutorial);

#pragma endregion

#pragma region ---くるくる
	//オブジェクトの生成
	Object* pLoad_Cursor = new Object(LOAD_TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto TransTutorial2 = pLoad_Cursor->AddComponent<CTransform>();			//トランスフォーム
	auto DrawTutorial2 = pLoad_Cursor->AddComponent<CDraw2D>();				//2D描画
	//多分何かしらのコンポーネントの追加が必要
	pLoad_Cursor->AddComponent<CTutorial>();								//チュートリアル
	pLoad_Cursor->AddComponent<CLotation>();
	//オブジェクトの設定
	TransTutorial2->SetPosition(-120.0f, 130.0f);										//座標の設定
	DrawTutorial2->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));			//テクスチャの設定
	DrawTutorial2->SetSize(STOP_GAME_SIZE_X, STOP_GAME_SIZE_Y);					//大きさ
	DrawTutorial2->SetColor(0.2f, 0.2f, 0.6f);
	//リストに追加
	m_LoadScreenObjects.push_back(pLoad_Cursor);
#pragma endregion
	//--------------------------------------------------------------------


	// 全オブジェクトの初期化
	for (auto object : m_LoadScreenObjects)
		object->Start();
}

// ロード画面のオブジェクト終了処理
void Load::Uninit()
{
	// オブジェクトの開放
	for (auto object : m_LoadScreenObjects)
		delete object;

	// リストを空にする
	m_LoadScreenObjects.clear();
}

// ロード画面の開始
void Load::Begin()
{
	// ロード画面初期化
	Load::Init();

	// ロード画面開始のお知らせ
	m_bLoading = true;

	// ロード画面ののスレッドを作る
	m_handle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Load::LoadScreen, NULL, 0, NULL);
}

// ロード画面の終了
void Load::End()
{
	// ロードが早く終わっても指定されたフレーム数はロード画面を表示する
	while (m_nLoadCount < LOAD_FRAME * 60)
	{
		Sleep(1);	// 待機
	}

	// ロード画面終了のお知らせ
	m_bLoading = false;

	// ロード画面のスレッドが終了するまで待つ
	WaitForSingleObject(m_handle, INFINITE);

	// ハンドルを閉じる
	CloseHandle(m_handle);

	// ロード画面のオブジェクトリストの開放
	Load::Uninit();
}

// ロード画面の更新＆描画
unsigned __stdcall Load::LoadScreen()
{
	// フレーム制御用
	DWORD dwExecLastTime = timeGetTime();	//	最後に実行した時刻（executive＝実行）
	DWORD dwCurrentTime	 = 0;				//	現在時刻

	// ロード時間のカウンタの初期化
	m_nLoadCount = 0;

	// ロード画面終了のお知らせがくるまでループ
	while (m_bLoading)
	{
		//	システム時刻をミリ秒単位で取得
		dwCurrentTime = timeGetTime();
		
		// フレーム制御(1/60秒ごとに実行)
		if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
		{
			dwExecLastTime = dwCurrentTime;	//	最後に実行した時刻 = 現在時刻

			//--- 更新
			InputManager::Instance()->Update();		// 入力
			CCamera::Get()->Update();				// カメラ
			for (auto object : m_LoadScreenObjects)	// オブジェクト
				object->Update();

			// スクリーン座標からワールド座標に変換
			InputManager* Input = InputManager::Instance();
			CCamera* pCamera = CCamera::Get();
			XMFLOAT2 vPos = Input->GetMousePos();
			XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
			XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
			XMFLOAT3 vWorldPos;
			CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);
			// カーソルエフェクトの座標
			vWorldPos.z = CURSOR_POS_Z;
			EffectManager::GetInstance()->SetPosition(CURSOR_EFFECT_NUM, vWorldPos);
			// エフェクシア更新
			EffectManager::GetInstance()->Update();

			// バックバッファ＆Ｚバッファのクリア
			float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
			GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
			GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//--- 描画
			for (auto object : m_LoadScreenObjects)	// オブジェクト
				object->Draw();
			EffectManager::GetInstance()->Draw();	// エフェクシア

			// バックバッファとフロントバッファの入れ替え
			GetSwapChain()->Present(0, 0);

			// ロード時間をカウント
			m_nLoadCount++;
		}
	}

	// スレッド終了
	_endthreadex(0);

	return 0;
}

// ロード画面のフレーム数取得
int Load::GetLoadCount()
{
	return m_nLoadCount;
}