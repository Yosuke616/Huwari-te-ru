//=============================================================================
//
// 光源クラス [Light.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

class CLight
{
	//=== メンバ変数 ===
private:
	XMFLOAT4 m_diffuse;		//	反射光(光が当たってる状態)
	XMFLOAT4 m_ambient;		//	環境光(光が当たってない状態)
	XMFLOAT4 m_specular;	//	自己発光
	XMFLOAT3 m_direction;	//	ライトの向き
	bool m_bEnable;			//	ライティング有効/無効


	//=== メンバ関数 ===
public:
	CLight();								//	コンストラクタ
	void Init();							//	初期化
	XMFLOAT4& GetDiffuse();					//	反射光の取得
	XMFLOAT4& GetAmbient();					//	環境光の取得
	XMFLOAT4& GetSpecular();				//	自己発光の取得
	XMFLOAT3& GetDir();						//	光源方向取得
	void SetEnable(bool bEnable = true);	//	ライトON
	void SetDisable(bool bDisable = true);	//	ライトOFF

	static CLight* Get();					//	ライトのインスタンス取得
};
