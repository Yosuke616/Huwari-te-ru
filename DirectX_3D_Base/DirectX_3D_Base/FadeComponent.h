/**
 * @file	FadeComponent.h
 * @brief	季節のロゴを出した後に徐々に消していく
 * @author	志水翔伍
 * @date	2021/12/22
 */

 //===== インクルードガード ======
#ifndef __FADE_COMPONENT_H__
#define __FADE_COMPONENT_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CFade : public Component
{
private:
	CDraw2D*	m_pDraw2D;	// 2D描画
	float		m_fTime;	// 経過時間
	float		m_fAlpha;	// α値
	XMFLOAT2	m_vPos;		// UV座標
	XMFLOAT2	m_vScrPos;	// スクロール座標

public:
	CFade();				// コンストラクタ
	~CFade() override;		// デストラクタ
	void Start() override;	// 初期化
	void Update() override;	// 更新
	void Draw() override;	// 描画
};

#endif // __FADE_COMPONENT_H__
