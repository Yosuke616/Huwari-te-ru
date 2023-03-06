//インクルード部
#include "GoMenuComponent.h"
#include "ObjectInfo.h"
#include "Draw2dComponent.h"
#include "InputManager.h"
#include "Scene.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "TransformComponent.h"		//	位置・速度・加速度


//コンストラクタ
CGoMenu::CGoMenu()
	:m_bSelect(false),m_bPauseMode(false),m_bMenuFlg(false){

}

//デストラクタ
CGoMenu::~CGoMenu() {

}

//初期化
void CGoMenu::Start() {
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	m_pObjectManager = ObjectManager::GetInstance();	//オブジェクトマネージャー
	m_pMenuManager = MenuManager::GetInstance();		// メニューマネージャー
}

//更新
void CGoMenu::Update() {
	//ポーズボタンの色を変える
	if (m_bSelect) {
		m_pDraw2D->SetColor(0.5f,0.5f,1.0f);
	}
	else {
		m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
	}
	m_bSelect = false;

	//メニューに行こうとしたかどうかのフラグが立っていたら実際にメニューに入るようにする
	if (m_bMenuFlg) {

		// ポーズ画面の場合
		if (m_bPauseMode == true)
		{		
				m_pObjectManager->NoFunction();
				m_pMenuManager->CreatePauseMenu();	// ポーズ画面生成			
		}
		else
		{
			m_pObjectManager->Resume();
			m_pMenuManager->DeleteMenu();		// メニューオブジェクトを空にする		
		}
	}

	//メニューの更新
	//m_pMenuManager->Update();

}

//終了処理
void CGoMenu::Uninit() {
	//	メニューの終了処理
	//MenuManager::Destroy();
}

//描画
void CGoMenu::Draw() {

	//メニューの描画
	//if (m_bPauseMode) {
	//	m_pMenuManager->Draw();
	//}

}

//当たり判定力学
void CGoMenu::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == CURSOR_NAME)
	{
		//インプットマネージャーの作成
		InputManager* pInput = INPUT;
	
		//色変えようのやつ
		m_bSelect = true;

		//ボタンを押してポーズ
		//ポーズ画面の切り替え
		if (pInput->GetMouseButton(MOUSE_L))
		{
			//ボタン押してメニューが作られたかどうかのフラグを立ててそこで
			//実際にメニュー画面になるかどうかを決める
			//Updateの中で処理をしたい
			m_bMenuFlg = true;		

			//m_bPauseMode = m_bPauseMode ^ 1;	// 切り替え
			m_bPauseMode = true;	// 切り替え

		}
	}
}