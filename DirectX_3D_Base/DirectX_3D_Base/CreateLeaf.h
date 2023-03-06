/**
 * @file	CreateLeaf.h
 * @brief	季節に合うオブジェクトを出す
 * @author	内藤航己
 * @date	2021/12/07
 */

 //===== インクルードガード ======
#ifndef __LEAF_H__
#define __LEAF_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====
#define MAXLEAF		(10)

//===== 前方宣言 =====

//列挙隊宣言
//2022/1/7 Shimizu Yosuke ----------------------------------------
enum Season_Leaf {

	SPRING_LEAF,		//春の葉っぱ
	SUMMER_LEAF,		//夏の葉っぱ
	AUTUMN_LEAF,		//秋の葉っぱ
	WINTER_LEAF,		//冬の粉雪

	MAX_SEASON_LEAF
};
//----------------------------------------------------------------

//===== クラス定義 =====
class CLeaf
{
private:
	int cnt;			//出す秒数を決めるカウント
public:
	CLeaf();			//	コンストラクタ
	~CLeaf();			//	デストラクタ
	void Update();		//	ゲーム更新
	void UpdateTitleScene();	// タイトルで
	void UpdateSelectScene();	// セレクトで
	void CreateLeaf();	//	葉っぱの作成
};
#endif // __LEAF_H__
