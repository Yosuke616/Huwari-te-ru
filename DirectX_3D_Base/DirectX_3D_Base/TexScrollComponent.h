/**
 * @file	TexScrollComponent.h
 * @brief	テクスチャのスクロール
 * @author	AT12A 成田陸人
 * @date	2021/12/10
 */

 //===== インクルードガード ======
#ifndef __TEX_SCROLL_H__
#define __TEX_SCROLL_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;


//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CTexScroll : public Component
{
	//=== メンバ変数
private:
	CDraw2D*	m_pDraw2D;				// 描画設定
	float*		m_pPosX;				// スクロールの基準とする座標X
	float*		m_pPosY;				// スクロールの基準とする座標Y

	XMFLOAT2	m_Scroll;				// スクロールの値
	XMFLOAT2	m_UVscroll;				// スクロールの値(0~1)
	XMFLOAT2	m_ScrollValue;			// スクロールの量
	static bool m_bScroll;				// スクロールフラグ

	//=== メンバ関数
public:
	CTexScroll();						//	コンストラクタ
	~CTexScroll() override;				//	デストラクタ
	void Start() override;				//	初期化
	void Update() override;				//	更新
	void Draw() override;				//	描画

	//setter
	void SetAxizX(float* px);
	void SetAxizY(float* py);
	void SetScrollValue(float fx);
	void SetScrollValueY(float fy);
	static void Scroll(bool bScroll);
};

#endif // __TEX_SCROLL_H__
