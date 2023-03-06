/**
 * @file	HormingComponent.h
 * @brief	座標をマウスにセットする
 * @author	AT12A 成田陸人
 * @date	2021/12/10
 *			2022/01/17	志水翔伍：マウス追従のエフェクト追加
 */

 //===== インクルードガード ======
#ifndef __HOMING_H__
#define __HOMING_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CHoming : public Component
{
	//=== メンバ変数
private:
	CTransform* m_pTransform;
	XMFLOAT2 m_vCmpMousePos;			// 過去の座標
	int m_nStopTime;					// 止まっているときの生成時間
	int m_nMoveTime;					// 動いてるときの生成時間
	int m_nEffectID;					// テクスチャの番号

	//=== メンバ関数
public:
	CHoming();											// コンストラクタ
	~CHoming() override;								// デストラクタ
	void Start() override;								// 初期化
	void Update() override;								// 更新
	void Draw() override;								// 描画
	void OnCollisionEnter(Object* pObject) override;	// 衝突処理

	void SetEffectNum(int nEffectNum);					// エフェクトを設定
};

#endif // __HOMING_H__
