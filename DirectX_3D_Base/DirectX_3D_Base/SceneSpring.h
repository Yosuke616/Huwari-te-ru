/**
 * @file	SceneSpring.h
 * @brief	春のシーン
 * @author	AT12A 成田陸人
 * @date	2021/12/21
 */


 //===== インクルードガード ======
#ifndef __SPRING_H__
#define __SPRING_H__


//===== インクルード =====
#include "Scene.h"
#include "main.h"


//===== クラス定義 =====
class SceneSpring : public Scene
{
	//=== メンバ関数
public:
	SceneSpring();				//	コンストラクタ
	~SceneSpring() override;	//	デストラクタ
	void Init() override;		//	初期化
	void Uninit() override;		//	終了処理
	void Update() override;		//	更新
	void Draw() override;		//	描画
};
#endif // __SPRING_H__

