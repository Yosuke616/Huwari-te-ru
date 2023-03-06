/**
 * @file	Billboard.h
 * @brief	ビルボード機能
 * @author	AT12A 成田陸人
 * @date	2021/12/02	作成
 */

 //===== インクルードガード ======
#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

//===== インクルード =====
#include "Component.h"
#include "DrawMeshComponent.h"

//===== 前方宣言 =====
class CTransform;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CBillboard : public CDrawMesh
{
private:
	void CreateBillboardVertex();

	//=== メンバ関数 ===
public:
	CBillboard();											//	コンストラクタ
	~CBillboard();											//	デストラクタ
	void Start() override;									//	開始
	void Update() override;									//	更新
	void Draw() override;									//	描画
	void OnCollisionEnter(Object* pObject) override;		//	衝突処理
};


#endif // __BILLBOARD_H__