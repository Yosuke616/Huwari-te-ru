//=============================================================================
//
// 光源クラス [Light.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Light.h"

using namespace DirectX;

// グローバル変数
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 1.0f);			//	ライトの向き(画面の奥下方向)
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);		//	反射光(光が当たってる状態)	
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);	//	環境光(光が当たってない状態)	
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);		//	自己発光	

	//	インスタンス作成
	CLight g_light;
};

// コンストラクタ
CLight::CLight()
{
	//	初期化
	Init();
}

// 初期化
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;										//	ライトの向き(画面の奥下方向)
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));	//	単位ベクトルにしてストアに保存

	m_diffuse = LIGHT0_DIFFUSE;		//	反射光(光が当たってる状態)	
	m_ambient = LIGHT0_AMBIENT;		//	環境光(光が当たってない状態)
	m_specular = LIGHT0_SPECULAR;	//	自己発光	
	m_bEnable = true;
}

// 反射光の取得
XMFLOAT4& CLight::GetDiffuse()
{
	return m_diffuse;
}

// 環境光の取得
XMFLOAT4& CLight::GetAmbient()
{
	return m_ambient;
}

// 自己発光の取得
XMFLOAT4& CLight::GetSpecular()
{
	return m_specular;
}


// 光源方向取得
XMFLOAT3& CLight::GetDir()
{
	//	ライトON
	if (m_bEnable) return m_direction;

	//	ライトOFF
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

// 光源インスタンス取得
CLight* CLight::Get()
{
	return &g_light;
}

// ライトON
void CLight::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

// ライトO
void CLight::SetDisable(bool bDisable)
{
	m_bEnable = !bDisable;
}
