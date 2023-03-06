/**
 * @file	SceneEnding.h
 * @brief	エンディング
 * @author	AT12A 成田陸人
 * @date	2021/01/24		作成
 */


 //===== インクルードガード ======
#ifndef __SCENE_ENDING_H__
#define __SCENE_ENDING_H__

//===== インクルード =====
#include "Scene.h"


//===== 前方宣言 =====
class WindManager;
class CloverManager;


#define ENDING_FRAME	(50 * 60)	// エンディングの時間（秒数×60fps

//===== クラス定義 =====
class SceneEnding : public Scene
{
private:
	WindManager*	m_pWindManager;		// 風マネージャー
	CloverManager*	m_pCloverManager;	// クローバー管理クラス
	static int		m_nCurrentFrameCnt;	// 経過時間

	// 2022/1/30 ミニゲーム終了フラグ-------------------------
	static bool m_bFinish;
	//---------------------------------------------------

	int m_nSceneChangeTimer;	// シーン切り替えタイマー

	// staticメンバ変数に変更
	// 一つしか存在しない変数のため
	static int		m_nClovercnt;		// 得点

public:
	SceneEnding();						//	コンストラクタ
	~SceneEnding() override;				//	デストラクタ
	void Init() override;				//	初期化
	void Uninit() override;				//	終了処理
	void Update() override;				//	更新
	void Draw() override;				//	描画

	// staticメンバ関数に変更
	// staticメンバ変数をいじるため
	static void Cntplus();				//	カウントアップ
	static void Cntminus();				//	カウントマイナス

	static int GetCurrentFrameCnt();	// 経過時間の取得

	// 2022/1/30 ミニゲーム終了フラグ-------------------------
	static bool Finish();
	//---------------------------------------------------

	static bool m_bSpawnClover[8];
};
#endif // __SCENE_ENDING_H__
