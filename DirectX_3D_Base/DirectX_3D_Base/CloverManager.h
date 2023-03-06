/**
 * @file	CloverManager.h
 * @brief	時間経過でクローバーを生成していく
 * @author	成田陸人
 * @date	2021/01/24
 */

 //===== インクルードガード ======
#ifndef __CLOVER_MANAGER_H__
#define __CLOVER_MANAGER_H__

//===== インクルード =====
#include "main.h"


//===== クラス定義 =====
class CloverManager
{
	//=== メンバ変数
private:
	static CloverManager* m_pInstance;
	int m_nFrameCnt;	//	出す秒数を決めるカウント
	void CreateClover();//	葉っぱの作成


	//=== メンバ関数
private:
	CloverManager();	//	コンストラクタ
public:
	~CloverManager();	//	デストラクタ
	void Update();		//	ゲーム更新

	static CloverManager* GetInstance();
	static void Destroy();

	// 2022/1/29 narita クローバーの数表示----------------------------------------------
	int GetCloverNum();
	//-----------------------------------------------------------------------------
};
#endif // __CLOVER_MANAGER_H__
