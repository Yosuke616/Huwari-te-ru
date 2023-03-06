/**
 * @file	MenuComponent.h
 * @brief	UIオブジェクトの管理
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 *更新履歴
*/

//===== インクルード =====
#include "MenuComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "sceneGame.h"
#include "fade.h"
#include "Draw2dComponent.h"
#include "Sound.h"
#include "TransformComponent.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ----------------------------
#include "sceneTitle.h"
//------------------------------------------------------------------

#include "MenuManager.h"

//***コンストラクタ
CMenu::CMenu()
	: m_bSelected(false)
	, m_pTrans(nullptr)
	, m_nRotate(0)
{
}

// デストラクタ
CMenu::~CMenu(){
}

// 初期化
void CMenu::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();

	//2022/1/27/Touch_Tutorial Shimizu Yosuke ------------------
	m_pTrans = Parent->GetComponent<CTransform>();
	//----------------------------------------------------------
}

// 更新
void CMenu::Update()
{
	

	if (m_bSelected)
	{
		m_pDraw2D->SetColor(0.0f, 1.0f, 0.0f);

		//2022/1/24/TUTORIAL_UI Shimizu Yosuke---------------------
		if (Parent->GetName() == TUTORIAL_NAME || Parent->GetName() == WINDCAR_NAME) {
			m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);
		}
		//---------------------------------------------------------
	}
	else
	{
		m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);

		//2022/1/22/pouseColor Shimizu Yosuke ---------------------
		if (Parent->GetName() == STOP_GAME_NAME) {
			m_pDraw2D->SetColor(0.2f, 0.2f, 0.6f);
		}
		if (Parent->GetName() == OPTION_NAME && SceneManager::Instance()->GetScene() == SCENE_TITLE)
		{
			m_pDraw2D->SetColor(0.2f, 0.2f, 0.6f);
		}
		//---------------------------------------------------------

	}
	m_bSelected = false;

	//2022/1/26/Touch_Tutorial ------------------------------------
	if (Parent->GetName() == WINDCAR_NAME) {
		//回転指せる変数をここで作る
		m_nRotate += 5;
		if (m_nRotate > 180) {
			m_nRotate -= 360;
		}

		m_pDraw2D->SetAngle((float)m_nRotate);
	}
	//-------------------------------------------------------------
}

// 描画
void CMenu::Draw()
{
}

// 衝突処理
void CMenu::OnCollisionEnter(Object* pObject)
{

	InputManager* pInput = InputManager::Instance();

	if (MenuManager::m_bOption == false)
	{
		// カーソル(メニューオブジェクト)
		if (pObject->GetName() == CURSOR_NAME)
		{
			// 選択されています
			m_bSelected = true;

			if (pInput->GetMouseTrigger(MOUSE_L))
			{
				// 決定音再生
				CSound::Play(SE_DECIDE);

				// タイトル画面メニュー
#pragma region ---にゅーげーむ
			//	自分の名前で処理内容を変える
				if (Parent->GetName() == NEW_GAME_NAME)
				{

					SceneTitle::NewGame();

					SceneSelect::NewGame();
					// タイトルシーンへ
					StartFadeOutSceneChange(SCENE_SELECT);
				}
#pragma endregion

#pragma region ---セレクトへ
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == SELECT_NAME)
				{
					// セレクトシーンへ
					StartFadeOutSceneChange(SCENE_SELECT);
				}
#pragma endregion

#pragma region ---おわる
				//	自分の名前で処理内容を変える
				if (Parent->GetName() == EXIT_GAME_NAME)
				{
					PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
				}
#pragma endregion


#pragma region ---オプション
				//	自分の名前で処理内容を変える
				if (Parent->GetName() == OPTION_NAME)
				{
					MenuManager::GetInstance()->CreateOptionMenu();
				}
#pragma endregion

				// ゲーム画面メニュー

				//ゲーム画面メニューの一番下に追加
				//2022/1/19/Tutorial Shimizu Yosuke -------------------------------------------
#pragma region  ---チュートリアル
			
				if (Parent->GetName() == TUTORIAL_NAME || Parent->GetName() == WINDCAR_NAME)
				{
					//ポーズフラグをオフにする
					SceneGame::GetInstance()->m_bPauseMode = false;
					// メニュー削除
					MenuManager::GetInstance()->DeleteMenu();
					// オブジェクトの更新を再開する
					ObjectManager::GetInstance()->Resume();
				}
#pragma endregion
				//------------------------------------------------------------------------------

#pragma region	 ---つづける
			//	自分の名前で処理内容を変える
				if (Parent->GetName() == CONTINUE_NAME)
				{
					// ポーズフラグをfalseにする
					SceneGame::GetInstance()->m_bPauseMode = false;

					// オブジェクト更新再開
					ObjectManager::GetInstance()->Resume();

					// メニューを消す
					MenuManager::GetInstance()->DeleteMenu();
				}
#pragma endregion

#pragma region ---最初から
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == RESTART_NAME)
				{
					// ポーズフラグをfalseにする
					SceneGame::GetInstance()->m_bPauseMode = false;
					// ゲームシーンへ
					StartFadeOutSceneChange(SCENE_GAME);
				}
#pragma endregion

#pragma region ---ポーズメニューアイコン
				if (Parent->GetName() == STOP_GAME_NAME)
				{
					// ポーズフラグをtrueにする
					SceneGame::GetInstance()->m_bPauseMode = true;
					// ポーズ画面生成
					MenuManager::GetInstance()->CreatePauseMenu();
					// オブジェクトの更新ストップ
					ObjectManager::GetInstance()->NoFunction();
				}
#pragma endregion

				// セレクト画面メニュー
#pragma region	---タイトルへ
			//	自分の名前で処理内容を変える
				else if (Parent->GetName() == TITLE_NAME)
				{
					//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------
					MenuManager::GetInstance()->SetMenu(STATE_NONE);
					//------------------------------------------------------------

					// ポーズフラグをfalseにする
					SceneGame::GetInstance()->m_bPauseMode = false;
					// タイトルシーンへ
					StartFadeOutSceneChange(SCENE_TITLE);
				}
#pragma endregion

#pragma region	---セレクト 右矢印
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == RIGHT_ARROW_NAME)
				{
					if (this->m_bDrawFlag == false) {
						CSound::Stop(SE_DECIDE);
					}

					SceneSelect::ChangeSceason(1);
				}
#pragma endregion

#pragma region	---セレクト 左矢印
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == LEFT_ARROW_NAME)
				{
					if (this->m_bDrawFlag == false) {
						CSound::Stop(SE_DECIDE);
					}
					SceneSelect::ChangeSceason(-1);
				}
#pragma endregion

#pragma region	---ステージ1
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == STAGE1_NAME)
				{
					switch (SceneSelect::m_nCurrentSeason)
					{
					case 0: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SPRING_1); break;
					case 1: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SUMMER_1); break;
					case 2: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(FALL_1);   break;
					case 3: if (SceneSelect::GetSelectable(STAGE_1_SELECTABLE)) SceneGame::GetInstance()->SetSeason(WINTER_1); break;
					default: break;
					}
				}
#pragma endregion

#pragma region	---ステージ2
				//	自分の名前で処理内容を変える
				else if (Parent->GetName() == STAGE2_NAME)
				{
					switch (SceneSelect::m_nCurrentSeason)
					{
					case 0: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SPRING_2); break;
					case 1: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(SUMMER_2); break;
					case 2: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(FALL_2); break;
					case 3: if (SceneSelect::GetSelectable(STAGE_2_SELECTABLE)) SceneGame::GetInstance()->SetSeason(WINTER_2); break;
					default:
						break;
					}
				}
#pragma endregion
			}
		}
		// ゲーム中のカーソルとの衝突
		else if (pObject->GetName() == GAME_CURSOR_NAME)
		{
			// 選択されています
			m_bSelected = true;

			if (pInput->GetMouseTrigger(MOUSE_L))
			{
				// 決定音再生
				CSound::Play(SE_DECIDE);

				if (Parent->GetName() == STOP_GAME_NAME)
				{
					// ポーズフラグをtrueにする
					SceneGame::GetInstance()->m_bPauseMode = true;
					// ポーズ画面生成
					MenuManager::GetInstance()->CreatePauseMenu();
					// オブジェクトの更新ストップ
					ObjectManager::GetInstance()->NoFunction();
				}


				//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------
				else if (Parent->GetName() == WINDCAR_NAME) {

					//ポーズフラグをオンにする
					SceneGame::GetInstance()->m_bPauseMode = true;

					//チュートリアルに行くやつを作る
					MenuManager::GetInstance()->CreateTutorialMenu();

					//オブジェクトの更新を止める
					ObjectManager::GetInstance()->NoFunction();
				}
				//----------------------------------------------------------------
			}
		}
	}
	// オプションを開いてるときはこっち
	else
	{
	// カーソル(メニューオブジェクト)
		if (pObject->GetName() == CURSOR_NAME)
		{
			// もどる
			if (Parent->GetName() == BACK_NAME)
			{
				// 選択されています
				m_bSelected = true;

				if (pInput->GetMouseTrigger(MOUSE_L))
				{
					// オプションメニュー削除
					MenuManager::GetInstance()->DeleteOptionMenu();
				}
			}
		}
	}

}