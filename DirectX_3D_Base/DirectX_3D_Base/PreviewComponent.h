/**
 * @file	PreviewComponent.h
 * @brief	プレビューの処理
 * @author	AT12A 鈴村朋也
 * @date	2022/01/17
 */

 //===== インクルードガード ======
#ifndef __PREVIEW_H__
#define __PREVIEW_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====


//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CPreview : public Component
{
private:
	bool m_bCollision;			// 衝突フラグ
	bool m_bOldCollision;		// 衝突フラグ

public:
	CPreview();					// コンストラクタ
	~CPreview() override;		// デストラクタ
	void Start() override;	// 初期化
	void Update() override;	// 更新
	void Draw() override;	// 描画
	void OnCollisionEnter(Object* pObject) override;
};
#endif