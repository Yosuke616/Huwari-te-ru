/**
 * @file	SceneSelect.h
 * @brief	ステージセレクトシーン
 * @author	AT12A 鈴村朋也
 * @date	2021/12/21		作成
 */


 //===== インクルードガード ======
#ifndef __SCENE_SELECT_H__
#define __SCENE_SELECT_H__

//===== インクルード =====
#include "Scene.h"

#include <vector>

//===== 列挙体宣言 =====
enum S_Season
{
	SPRING = 0,
	SUMMER,
	AUTUMN,
	WINTER
};

//===== 定数定義 =====
#define STAGE_1_SELECTABLE	(0)
#define STAGE_2_SELECTABLE	(1)
#define STAGE_1_CLOVER		(2)
#define STAGE_2_CLOVER		(3)

#define CLEAR_FLOWER_UI_1_NAME		("ClearFlowerUI1")
#define CLEAR_FLOWER_UI_2_NAME		("ClearFlowerUI2")
#define CLEAR_UI_1_NAME				("ClearUI1")
#define CLEAR_UI_2_NAME				("ClearUI2")

//===== クラス定義 =====
class SceneSelect : public Scene
{
	//=== メンバ変数 ===
public:
	// vectorの2次元配列
	using Cell = int;					// セル
	using Column = std::vector<Cell>;	// 行
	using Row = std::vector<Column>;	// 列
private:
	bool bMV1_PlayONOFF;	//MV１再生
	bool bMV2_PlayONOFF;	//MV２再生

	static bool m_bRelease1;			// ステージ解放演出
	static bool m_bRelease2;			// ステージ解放演出
	static bool m_bReleaseClover[8];

	static int		m_OldSeason;
	static Row		m_SelectGrid;		// クリア状況の配列
	static Column	m_Clover;			// クローバー取得状況の配列

private:
	// ロード
	static bool Load();
	// セーブ
	static bool Save();

	// ステージ解放演出
	void Clear();

	//=== メンバ関数 ===
public:
	SceneSelect();								// コンストラクタ
	~SceneSelect() override;					// デストラクタ
	void Init() override;						// 初期化
	void Uninit() override;						// 終了処理
	void Update() override;						// 更新
	void Draw() override;						// 描画

	static int m_nCurrentSeason;				// 選択されている季節

	// セレクトシーンの季節切り替え
	static void ChangeSceason(int n);

	// 選択可能状態の取得
	static bool GetSelectable(int stage_num);

	// 選択可能状態の配列取得
	static Row GetSelectGrid();

	// csvファイル初期化
	static bool NewGame();

	// クリア状況の保存
	static bool SaveClearInfo(int nStage, bool bClover[4]);

	// クローバー取得
	static bool SaveCloverInfo(int nStage, int cloverNumber);

	// クローバーの取得状況の取得
	static Column GetClovers();
};
#endif // __SCENE_SELECT_H__