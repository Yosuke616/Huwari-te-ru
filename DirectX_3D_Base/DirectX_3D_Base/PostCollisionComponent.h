/**
 * @file	PostCollision.h
 * @brief	�Փˎ��̏������e
 * @author	AT12A ���c���l
 * @date	2021/12/30 �쐬
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __POST_COLLISION_H__
#define __POST_COLLISION_H__

//===== �C���N���[�h =====
#include "Component.h"


class CTransform;

//===== �N���X��` =====
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