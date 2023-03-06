/**
 * @file	SceneTitle.h
 * @brief	タイトルシーン
 * @author	AT12A 成田陸人
 * @date	2021/08/xx		作成
 */


//===== インクルードガード ======
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

//===== インクルード =====
#include "Scene.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ------------------------------
#include "sceneGame.h"
#include <vector>
//---------------------------------------------------------------------



//===== クラス定義 =====
class SceneTitle : public Scene
{
	//2022/1/28/Tutorial_CSV Shimizu Youske --------------------------
public:
	using Cell = int;						//	セル
	using Column = std::vector<Cell>;		//	行
	using Row = std::vector<Column>;		//	列

private:
	static Row m_TutorialVector;	// チュートリアルのベクター

	//Tutorial用のCSVを読み込むための関数
	static bool LoadTurorial();
	//----------------------------------------------------------------

public:
	static int m_nTitleSeason;			//	タイトルの季節
	static void SetSeason(int season);	//	季節設定
	SceneTitle();						//	コンストラクタ
	~SceneTitle() override;				//	デストラクタ
	void Init() override;				//	初期化
	void Uninit() override;				//	終了処理
	void Update() override;				//	更新
	void Draw() override;				//	描画

	//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------------
	//Tutorial用のCSVを読み込むための関数
	static bool SaveTutorial();
	//CSVを初期化するためのやつ
	static bool NewGame();

	//ベクターの中身を引数で指定して、0だったらfalse・1だったらtrueを返す
	//セルの中身を調べる　季節によって
	static bool FirstPlay(int nSeason);
	//-----------------------------------------------------------------
};
#endif // __SCENE_TITLE_H__


