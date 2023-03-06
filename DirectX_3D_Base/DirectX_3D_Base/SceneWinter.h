/**
 * @file	SceneWinter.h
 * @brief	冬のシーン
 * @author	AT12A 成田陸人
 * @date	2021/12/21
 */


 //===== インクルードガード ======
#ifndef __WINTER_H__
#define __WINTER_H__


//===== インクルード =====
#include "Scene.h"
#include "main.h"


//===== クラス定義 =====
class SceneWinter : public Scene
{
	//=== メンバ関数

public:
	SceneWinter();				//	コンストラクタ
	~SceneWinter() override;		//	デストラクタ
	void Init() override;		//	初期化
	void Uninit() override;		//	終了処理
	void Update() override;		//	更新
	void Draw() override;		//	描画
};
#endif // __WINTER_H__

