/**
 * @file	Load.h
 * @brief	ロード画面
 * @author	AT12A 成田陸人
 * @date	2021/12/31		作成
 */


 //===== インクルードガード ======
#ifndef __LOAD_H__
#define __LOAD_H__

//===== インクルード =====
#include "Scene.h"
#include <vector>

//===== クラス定義 =====
class Load
{
	//=== メンバ変数
private:
	static HANDLE	m_handle;		// スレッドハンドル
	static bool		m_bLoading;		// ロード中
	static int		m_nLoadCount;	// ロード時間のカウント
	// ロード画面のオブジェクトリスト
	static std::vector<Object*> m_LoadScreenObjects;
	
	//=== メンバ関数
private:
	static void Init();			// ロード画面のオブジェクト初期化
	static void Uninit();		// ロード画面のオブジェクト終了処理
	static unsigned __stdcall LoadScreen();	// ロード画面更新＆描画
public:
	Load();						// コンストラクタ
	~Load();					// デストラクタ

	static void Begin();		// ロード画面のスレッド
	static void End();			// 初期化が早く終わり過ぎたらsleepしてもらう
	static int GetLoadCount();	// ロード画面のフレーム数取得

};
#endif // __SCENE_LOAD_H__


