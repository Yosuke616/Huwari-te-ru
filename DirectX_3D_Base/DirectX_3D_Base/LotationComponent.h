/**
 * @file	LotationComponent.h
 * @brief	回転
 * @author	AT12A 成田陸人
 * @date	2021/1/1	作成
 */

 //===== インクルードガード ======
#ifndef __LOTATION_H__
#define __LOTATION_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;


//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CLotation : public Component
{
private:
	CDraw2D* m_pDraw2D;		// 描画設定
	int m_nDegree;			// 角度

public:
	CLotation();
	~CLotation() override;
	void Start() override;
	void Update() override;
};
#endif // __LOTATION_H__