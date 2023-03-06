/**
 * @file	Scale2DCom.h
 * @brief	目的の大きさにする
 * @author	AT12A 成田陸人
 * @date	2022/01/30
 */

 //===== インクルードガード ======
#ifndef __SCALE_2D_H__
#define __SCALE_2D_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CScale2D : public Component
{
private:
	CDraw2D*	m_pDraw2D;		// オブジェクトの描画情報
	XMFLOAT2	m_vDestSize;	// 目的の座標

	XMFLOAT2	m_vSize;

	int m_nTime;

public:
	CScale2D();					// コンストラクタ
	~CScale2D() override;			// デストラクタ
	void Start() override;		// 初期化
	void Update() override;		// 更新
	void Draw() override;		// 描画

	// 移動目的の座標の設定
	void SetDestSize(XMFLOAT2 pos);
};
#endif // __POINT_H__