/**
 * @file	SceneSummer.h
 * @brief	夏のシーン
 * @author	AT12A 成田陸人
 * @date	2021/12/21
 */


//===== インクルードガード ======
#ifndef __SUMMER_H__
#define __SUMMER_H__


//===== インクルード =====
#include "Scene.h"
#include "main.h"


//===== クラス定義 =====
class SceneSummer : public Scene
{
	//=== メンバ関数
public:
	SceneSummer();				//	コンストラクタ
	~SceneSummer() override;	//	デストラクタ
	void Init() override;		//	初期化
	void Uninit() override;		//	終了処理
	void Update() override;		//	更新
	void Draw() override;		//	描画
};
#endif // __SUMMER_H__

