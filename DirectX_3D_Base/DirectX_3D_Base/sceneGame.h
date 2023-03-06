/**
 * @file	SceneGame.h
 * @brief	ゲームシーン
 * @author	AT12A 成田陸人
 * @date	2021/08/xx		作成
 */


//===== インクルードガード ======
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__


//===== インクルード =====
#include "Scene.h"
#include "main.h"


#include <vector>

#include "AssimpModel.h"
#include <unordered_map>


//===== 前方宣言 =====
class WindManager;
class StageManager;


//===== 列挙体宣言 =====
enum Season
{
	SPRING_1,		// 春１
	SPRING_2,		// 春２
	SUMMER_1,		// 夏１
	SUMMER_2,		// 夏２
	FALL_1,			// 秋１
	FALL_2,			// 秋２
	WINTER_1,		// 冬１
	WINTER_2,		// 冬２

	MAX_SEASON
};

//===== クラス定義 =====
class SceneGame : public Scene
{
	//=== メンバ変数
private:
	static SceneGame* m_pInstance;		// インスタンス
	std::vector<Scene*> m_GameSceneList;// シーンリスト
	Scene*			m_CurrentScene;		// 現在のシーン
	Season			m_eSeasonState;		// 季節

	Scene*			m_pInitScene;	// 初期化したいシーン
	
	WindManager*	m_pWindManager;		// 風マネージャー
	StageManager*	m_pStageManager;	// ステージマネージャー

	//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------------------
	bool m_bTutorial;					//ここがオンだったらチュートリアルを表示する
	//-----------------------------------------------------------------

public:
	bool	m_bEditMode;				// 編集モード
	bool	m_bPauseMode;				// メニュー画面

	//=== メンバ関数
private:
	SceneGame();						// コンストラクタ
public:
	static SceneGame* GetInstance();	// インスタンス取得
	static void Destroy();				// インスタンス破棄

	~SceneGame() override;				// デストラクタ
	void Init() override;				// 初期化
	void Uninit() override;				// 終了処理
	void Update() override;				// 更新
	void Draw() override;				// 描画

	void SetSeason(Season NextSeason);	// シーン切り替え
	Season GetSeason();					// シーン情報の取得


	// 仮措置
	static std::unordered_map<CAssimpModel*, bool> IceManager;	// 氷の数
	//static std::vector

	//2022/1/26/Touch_Tutorial Shimizu Yosuke --------------------------------
	void SetTutorial(bool bTutorial);
	//------------------------------------------------------------------------

};
#endif // __SCENE_GAME_H__

