#pragma once
/**
 * @file	FallenLeavesComponent.h
 * @brief	落ち葉のコンポーネント
 * @author	AT12B 吉田涼雅
 * @date	2021/12/21
 */

 //===== インクルードガード ======
#ifndef __FALLENLEAVES_H__
#define __FALLENLEAVES_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class InputManager;
class CTransform;
class Object;


//===== クラス定義 =====
class CFallenLeaves : public Component
{
private:
	CTransform* m_pTransform;	//	トランスフォーム
	InputManager* m_pInput;		//	入力情報
	bool m_bClover;				//	クローバーをつくるフラグ

public:
	CFallenLeaves();								    //	コンストラクタ
	~CFallenLeaves() override;							//	デストラクタ
	void Start() override;								//	初期化
	void Update() override;								//	更新
	void Draw() override;								//	描画
	void OnCollisionEnter(Object*) override;
	
	void Clover();

	int m_nCloverNumber;
	void SetNumber(int num);
	int GetNumber();
};
#endif // __COMPONENT_H__