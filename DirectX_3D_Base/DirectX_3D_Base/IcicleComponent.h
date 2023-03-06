/**
 * @file	IcicleComponent.h
 * @brief	つららの機能
 * @author	内藤航己
 * @date	2021/12/
 */

 //===== インクルードガード ======
#ifndef __ICICLE_H__
#define __ICICLE_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"
#include "TransformComponent.h"

//===== 前方宣言 =====


//===== クラス定義 =====
class CIcicle : public Component
{
private:
	CTransform* m_pTransform;		//オブジェクトの座標
	float m_fLimitDown;				//画面の下限
	int m_nIcicleHP;				//つららの耐久値
	bool m_bFall;					//落ちましたフラグ
	bool m_bMove;					//停止フラグ
public:
	CIcicle();						// コンストラクタ
	~CIcicle() override;			// デストラクタ
	void Start() override;			// 初期化
	void Update() override;			// 更新
	void Draw() override;			// 描画
	void OnCollisionEnter(Object* pObject) override;

	//void SetDownIce(Object* pObject);
	void IcicleDamage();			// つららの耐久値減少
	void Stop();
	bool IsFalling();		// 落ちているかのフラグ
	bool IsMoving();		// 落下中かどうか
};
#endif // __ICICLE_H__
