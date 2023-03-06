/**
 * @file	CGameover.h
 * @brief	季節に合うオブジェクトを出す
 * @author	内藤航己
 * @date	2022/01/19
 */

 //===== インクルードガード ======
#ifndef __GOLETTER_H__
#define __GOLETTER_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====

//===== 前方宣言 =====

//===== クラス定義 =====
class CGoletter
{
private:
	//int cnt;			//出す秒数を決めるカウント
public:
	CGoletter();			//	コンストラクタ
	~CGoletter();			//	デストラクタ
	void Update();		//	ゲーム更新
	//void UpdateTitleScene();	// セレクト
	void CreateGoletter();	//	文字の作成
};
#endif // __GOLETTERr_H__
