/**
 * @file	OutRangCom.h
 * @brief	簡単な説明
 * @author	書いた人
 * @date	日付（開始日？）
 */

 //===== インクルードガード ======
#ifndef __OUT_RANG_H__
#define __OUT_RANG_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;

//===== マクロ定義 =====
#define OUT_RANGE_X	(300.0f)
#define OUT_RANGE_Y	(140.0f)

//===== 列挙体宣言 =====


//===== クラス定義 =====
class COutRange : public Component
{
private:
	CTransform* m_pTransform;

	XMFLOAT2 m_LimitRange;

private:
	void NoFunction();
public:
	COutRange();
	~COutRange() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	void SetLimitRange(float x, float y);
};
#endif // __OUT_RANG_H__