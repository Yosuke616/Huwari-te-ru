/**
 * @file	BudComponent.h
 * @brief	つぼみの処理
 * @author	AT12A 成田陸人
 * @date	2022/01/17
 */

 //===== インクルードガード ======
#ifndef __BUD_H__
#define __BUD_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====


//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CBud : public Component
{
private:
	bool m_bCollision;			// 衝突フラグ
	bool m_bOldCollision;		// 衝突フラグ
	bool m_bOpen;				// 開くフラグ

public:
	CBud();					// コンストラクタ
	~CBud() override;		// デストラクタ
	void Start() override;	// 初期化
	void Update() override;	// 更新
	void Draw() override;	// 描画
	void OnCollisionEnter(Object* pObject) override;
};
#endif