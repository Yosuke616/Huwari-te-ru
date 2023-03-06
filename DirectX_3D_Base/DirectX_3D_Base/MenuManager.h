/**ポーズ
 * @file	MouseManager.h
 * @brief	メニューを管理するクラスにする
 * @author	SAWADA RYOHEI
 * @date	2021/12/22
 */

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

 //インクルード部
#include "main.h"
#include <list>

//前方宣言
class Object;

//列挙隊宣言
enum MENU_STATE
{
	STATE_NONE = 0,
	STATE_TITLE,
	STATE_OPTION,
	STATE_SELECT,
	STATE_GAMEOVER,
};

//クラス定義
class MenuManager
{
	//=== メンバ変数
private:
	static MenuManager* m_pInstance;	// インスタンス用
	std::list<Object*>	m_MenuList;		// メニューリスト
	int m_nDelayCnt;					// 遅延時間
	int m_nCreateState;					// 作成するメニュー

	//2022/1/31/Load_Tutorial Shimizu Yosuke ----------------------------
	MENU_STATE m_OldMenu_State;			//過去のシーンを保存する変数
	//-------------------------------------------------------------------


public:
	// オプションフラグ
	static bool m_bOption;

	//=== メンバ関数
private:
	MenuManager();						// コンストラクタ
public:
	~MenuManager();						// デストラクタ
	void Start();						// 初期化
	void Update();						// 更新
	void Draw();						// 描画
	static MenuManager* GetInstance();	// インスタンス生成
	static void Destroy();				// インスタンス破棄

	// メニュー作成
	void Create(int state, int delayCnt = 1);

	// タイトル
	void CreateTitleMenu();
	// ポーズ
	void CreatePauseMenu();
	// ステージセレクト
	void CreateSelectMenu();
	// ゲームオーバー
	void CreateGameoverMenu();
	// オプション作成
	void CreateOptionMenu();
	// オプション削除
	void DeleteOptionMenu();

	//2022/1/19/Tutorial Shimizu Yosuke -------------------------------------
	void CreateTutorialMenu();
	//-----------------------------------------------------------------------

	//2022/1/31/Load_Tutorial Shimizu  Yosuke --------------------------
	//ロードのためにシーンをゲットするのとセットするのがひつよう
	MENU_STATE GetMenu();
	void SetMenu(MENU_STATE State_scene);
	//------------------------------------------------------------------


	// メニューリストの削除
	void DeleteMenu();

	// メニューオブジェクトの追加
	void AddMenu(Object* pMenuObject);

	// メニューリストの取得
	std::list<Object*>& GetMenuList();
};
#endif	// _MENUMANAGER_H_
