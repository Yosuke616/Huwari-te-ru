/**
 * @file	PointCom.h
 * @brief	指定した座標にゆっくり移動させる
 * @author	AT12A 成田陸人
 * @date	2022/01/30
 */

 //===== インクルードガード ======
#ifndef __POINT_H__
#define __POINT_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CPoint : public Component
{
private:
	CTransform* m_pTransform;	// オブジェクトの座標
	XMFLOAT2	m_vDestPos;		// 目的の座標

public:
	CPoint();					// コンストラクタ
	~CPoint() override;			// デストラクタ
	void Start() override;		// 初期化
	void Update() override;		// 更新
	void Draw() override;		// 描画

	// 移動目的の座標の設定
	void SetDestPos(XMFLOAT2 pos);
};
#endif // __POINT_H__