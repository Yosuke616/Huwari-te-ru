//インクルード部
#include "TutorialComponent.h"
#include "ColliderComponent.h"
#include "MenuComponent.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "MenuComponent.h"
#include "Object.h"
#include "sceneGame.h"
#include "WindManager.h"
#include "InputManager.h"

//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------
#include "SceneManager.h"
#include "Draw2dComponent.h"
//------------------------------------------------------------------------

//コンストラクタ
CTutorial::CTutorial() {

}

//デストラクタ
CTutorial::~CTutorial() {

}

//初期化
void CTutorial::Start() {
	//風のマネージャーを呼び出す
	m_pWindManager = WindManager::GetInstance();

	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	//------------------------------------------------------------
}

//終了
void CTutorial::Uninit() {
	//風のマネージャーを破壊する
	m_pWindManager->Destroy();
}

//更新
void CTutorial::Update() {
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------------
	if (Parent->GetName() == LOAD_TUTORIAL_NAME) {
		if (MenuManager::GetInstance()->GetMenu() == STATE_SELECT) {
			m_pDraw2D->SetAlpha(1.0f);
		}
		else {
			m_pDraw2D->SetAlpha(0.0f);
		}
	}
	//------------------------------------------------------------------------------
}

//描画
void CTutorial::Draw() {

}

//衝突判定
void CTutorial::OnCollisionEnter(Object* pObject) {
	//OnCollisionEnterの中身全換え
//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------------------------------------
	if (pObject->GetName() == CURSOR_NAME) {

		//インプットマネージャーのインスタンスをゲットする
		InputManager* Input = InputManager::Instance();

		if (Input->GetMouseTrigger(MOUSE_L)) {
			//メニューに入れるかどうかのフラグが立っていたらここに入れる
			if (!m_pWindManager->GetTutorial()) {

				//セッターでメニューに入れるかのフラグをオンにして入れないようにする
				m_pWindManager->SetTutorial(true);

				//親がチュートリアルかつぶつかった相手が風だった時に入る
				//この中には入る
				//メニューマネージャー

				//ゲーム画面を止める
				SceneGame::GetInstance()->m_bPauseMode = true;

				//チュートリアルに行くやつを作る
				MenuManager::GetInstance()->CreateTutorialMenu();

				//オブジェクトの更新を止める
				ObjectManager::GetInstance()->NoFunction();

			}
		}
	}
	//----------------------------------------------------------------------------------------

}





