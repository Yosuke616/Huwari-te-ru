/**
 * @file	GoMenuComponent.h
 * @brief	このコンポーネントでメニューに行けるようにする
 * @author	志水　陽祐
 * @date	2022/1/12
 */

 //===== インクルードガード ======
#ifndef __GO_MENU_H__
#define __GO_MENU_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;
class ObjectManager;
class MenuManager;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CGoMenu : public Component
{
private:
	bool m_bSelect;						//メニューのボタンに触れているかどうかのフラグ
	bool m_bMenuFlg;					//メニューに行こうとしたかどうかのフラグ
	CDraw2D* m_pDraw2D;					

	ObjectManager*	m_pObjectManager;	// オブジェクト管理のポインタ
	MenuManager*	m_pMenuManager;		// メニュー管理のポインタ


public:
	bool	m_bPauseMode;				// メニュー画面

	CGoMenu();
	~CGoMenu() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Uninit();
	void OnCollisionEnter(Object* pObject) override;
};

#endif // __機能名_H__

#pragma endregion