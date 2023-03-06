/**
 * @file	FlashComponent.h
 * @brief	点滅(α値)をいじる
 * @author	AT12A 志水翔伍
 * @date	2022/01/28
 */

 //===== インクルードガード ======
#ifndef __FLASH_H__
#define __FLASH_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CFlash : public Component
{
private:
	CTransform* m_pTransform;		// 座標
	CDraw2D* m_pDraw2D;				// 描画
	float m_fAlpha;					// α値
	int m_nDrawTime;				// 描画時間
	int m_nNoneTime;				// 描画しない時間
	int m_nKeepAlpha;				// 保存用(描画時間)
	int m_nKeepNone;				// 保存用(描画しない時間)

public:
	CFlash();
	~CFlash() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	//Setter
	void SetAlphaTime(int nTime);	// 描画時間設定
	void SetNoneTime(int nTime);	// 描画しない時間設定
};

#endif // __FLASH_H__