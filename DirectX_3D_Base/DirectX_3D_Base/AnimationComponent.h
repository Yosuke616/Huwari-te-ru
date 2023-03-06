/**
 * @file	AnimationComponent.h
 * @brief	fbx切り替え機能
 * @author	成田陸人
 * @date	2022/12/31
 */

 //===== インクルードガード ======
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

#include <unordered_map>

//===== マクロ定義 =====

//===== 前方宣言 =====
class CDrawModel;

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CAnimation : public Component
{
private:
	int m_nAnim;										//	アニメーション番号(0 いろんなモーション 1 クリア)
	std::unordered_map<int, CDrawModel*> m_AnimationList;	//	アニメーションリスト

public:
	CAnimation();							//	コンストラクタ
	~CAnimation() override;					//	デストラクタ
	void Start() override;					//	初期化
	void Update() override;					//	更新
	void Draw() override;					//	描画
	void ChangeAnimation(int anim);			//	アニメーション番号変更
	void AddAnimation(CDrawModel* fbx_anim, int number);
	//getter
	CDrawModel* GetMoel(int fbx_number);
};
#endif // __GRAVITY_H__
