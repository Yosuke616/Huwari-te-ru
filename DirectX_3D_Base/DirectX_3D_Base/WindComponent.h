/**
 * @file	Wind_Mouse.h
 * @brief	矢印で風が出る
 * @author	志水陽祐
 * @date	2021/12/8
 *			2021/12/11		
 */

 //===== インクルードガード ======
#ifndef __WIND_H__
#define __WIND_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CWind : public Component
{
	//=== メンバ変数
private:
	XMFLOAT2 m_Vector;				//	風の向き

	//=== メンバ関数
public:
	CWind();											//	コンストラクタ
	~CWind() override;									//	デストラクタ
	void Start() override;								//	初期化
	void Update() override;								//	更新
	void Draw() override;								//	描画
	void OnCollisionEnter(Object* pObject) override;	//	衝突処理
	//setter
	void SetWindVector(XMFLOAT2 vector);				//	風の向き(x成分,y成分)の設定
	//getter
	XMFLOAT2 GetAngle();								//	風の向きの取得
};

#endif // __機能名_H__