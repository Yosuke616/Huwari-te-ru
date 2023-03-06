//=============================================================================
//
//  座標コンポーネントクラス[TransformComponent.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== インクルードガード ======
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== クラス定義 =====
class CTransform : public Component
{
	//=== メンバ変数 ===
public:
	XMFLOAT3 Pos;				//	座標
	XMFLOAT3 Vel;				//	速度
	XMFLOAT3 Acc;				//	加速度
	XMFLOAT3 Scale;				//	スケール
	XMFLOAT3 Rotate;			//	回転

	//=== メンバ関数 ===
public:
	CTransform();				//	コンストラクタ
	~CTransform() override;		//	デストラクタ
	void Update() override;		//	更新

	void SetPosition(float fx, float fy, float fz = 0);	//	座標の設定
	void SetVelocity(float fx, float fy, float fz = 0);	//	速度の設定
	void SetScale(float fx, float fy, float fz = 0);	//	スケールの設定
	void SetRotate(float fx, float fy, float fz = 0);	//	回転の設定
};
#endif // __TRANSFORM_H__
