/**
 * @file	FallComponent.h
 * @brief	大きい葉の場合の重力
 * @author	内藤航己
 * @date	2021/12/07
 */

//===== インクルードガード ======
#ifndef __COM_FALL_H__
#define __COM_FALL_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====
#define BIGLEAFGRAVITY	(0.015f)
#define VIBRATION		(0.05f)

//===== 前方宣言 =====
class CTransform;

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CFall : public Component
{
private:
	CTransform*	m_pTransform;		//	トランスフォーム
	float m_fDegree;				//　	角度
	float m_fResistance;			//　	抵抗力
	bool m_bActive;					//	左右移動フラグ
	float m_fAngle;					//	傾き具合

	//2022/01/03
	bool m_bUpFlg;					//　上昇しているかどうかフラグ　trueの時に上昇している
	bool m_bRorL;					//　右に進むか左に進むかのやる  tureの時にひだりにすすむ
	XMFLOAT3 m_FallPos;				//　落ち始めの座標を保存する為の変数

public:
	CFall();				//	コンストラクタ
	~CFall() override;	//	デストラクタ
	void Start() override;			//	初期化
	void Update() override;			//	更新
	void SetActive(bool bActive);	//	左右移動フラグ
};
#endif // __GRAVITY_H__
