/**
 * @file	PostCollision.h
 * @brief	衝突時の処理内容
 * @author	AT12A 成田陸人
 * @date	2021/12/30 作成
 */

 //===== インクルードガード ======
#ifndef __POST_COLLISION_H__
#define __POST_COLLISION_H__

//===== インクルード =====
#include "Component.h"


class CTransform;

//===== クラス定義 =====
class CPostCollision : public Component
{
private:
	CTransform* m_pTransform;
	XMFLOAT2 m_OldPos;
public:
	CPostCollision();
	~CPostCollision() override;
	void OnCollisionEnter(Object* pObject) override;
	void Start()override;
	void Update()override;
};
#endif // __POST_COLLISION_H__