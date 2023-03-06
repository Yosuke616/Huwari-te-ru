/**
 * @file	BGChangeComponent.h
 * @brief	背景が時間経過で変わっていくコンポーネント
 * @author	志水翔伍
 * @date	2021/12/28
 */

//===== インクルードガード ======
#ifndef __BG_CHANGE_H__
#define __BG_CHANGE_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CBgChange : public Component
{
private:
	CDraw2D* m_pDraw2D;			// 2D描画
	int m_nTime;				// 時間
	int m_nCmpTimer;			// 時間保存
	int m_nTexNum;				// テクスチャ
	bool m_bFlg;				// フラグ

public:
	CBgChange();				// コンストラクタ
	~CBgChange() override;		// デストラクタ
	void Start() override;		// 初期化
	void Update() override;		// 更新
	void Draw() override;		// 描画

	void ChangeTexture();		// テクスチャ切替
	void SetTimer(int nTimer);	// 時間設定
};

#endif // __BG_CHANGE_H__