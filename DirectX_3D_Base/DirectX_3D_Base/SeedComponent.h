/**
 * @file	SeedComponent.h
 * @brief	種
 * @author	志水陽祐
 * @date	2021/20
 */

 //===== インクルードガード ======
#ifndef __SEED_COMPONENT_H__
#define __SEED_COMPONENT_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CDrawModel;
class CCollider;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CSeed : public Component
{
private:
	//CTransform* m_pTransform;								//　トラスフォームを扱うポインタの変数
	//CDrawModel* m_pDraw;										//　描画を扱うポインタの変数
	//CCollider* m_pCollider;									//　衝突判定を扱うポインタ変数
	bool m_bUIFlg;											//  UIを表示してもよいかのフラグ

public:
	CSeed();
	~CSeed() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	//bool GetSeedFlg();
};

#endif // __SEED_COMPONENT_H__
