//===== インクルード =====
#include "TransformComponent.h"

// コンストラクタ
CTransform::CTransform()
{
	m_eUpdateOrder = COM_UPDATE_TRANSRATE;	//	コンポーネントListの更新順序の設定

	Pos		= XMFLOAT3(0.0f, 0.0f, 0.0f); 
	Vel		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Acc		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale	= XMFLOAT3(1.0f, 1.0f, 1.0f);
	Rotate	= XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// デストラクタ
CTransform::~CTransform()
{
}

// 更新
void CTransform::Update()
{
	Vel.x += Acc.x;	//	加速度を加算
	Vel.y += Acc.y;
	Vel.z += Acc.z;
	Pos.x += Vel.x;	//	座標に速度を加算
	Pos.y += Vel.y;
	Pos.z += Vel.z;
}

// 座標の設定
void CTransform::SetPosition(float fx, float fy, float fz)
{
	Pos.x = fx;
	Pos.y = fy;
	Pos.z = fz;
}

// 速度の設定
void CTransform::SetVelocity(float fx, float fy, float fz)
{
	Vel.x = fx;
	Vel.y = fy;
	Vel.z = fz;
}

// スケールの設定
void CTransform::SetScale(float fx, float fy, float fz)
{
	Scale.x = fx;
	Scale.y = fy;
	Scale.z = fz;
}

// 回転の設定
void CTransform::SetRotate(float fx, float fy, float fz)
{
	Rotate.x = fx;
	Rotate.y = fy;
	Rotate.z = fz;
}
