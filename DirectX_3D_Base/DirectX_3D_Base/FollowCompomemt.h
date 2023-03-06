/**
 * @file	FollowComponent.h
 * @brief	他のオブジェクトについていく
 * @author	AT12A 成田陸人
 * @date	2021/12/28
 */

 //===== インクルードガード ======
#ifndef __FOLLOW_H__
#define __FOLLOW_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;


//===== クラス定義 =====
class CFollow : public Component
{
private:
	CTransform* m_pTransform;		// オブジェクトの座標
	Object* m_pFollowObject;		// このオブジェクトがついていく対象の座標
public:
	CFollow();						// コンストラクタ
	~CFollow() override;			// デストラクタ
	void Start() override;			// 開始
	void Update() override;			// 更新
	void Draw() override;			// 描画
	void OnCollisionEnter(Object* pObject) override;	// 衝突処理
	//setter
	void SetParent(Object* pParent);// このオブジェクトがついていく対象の座標の設定
};

#endif // __FOLLOW_H__