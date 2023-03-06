/**
 * @file	ColliderComponent.h
 * @brief	���̃R���|�[�l���g�����I�u�W�F�N�g���m���������Ă��邩���f����
 * @author	AT12A ���c���l
 * @date	2021/07/xx	�쐬
 *
 * @detail	�K�{�̐ݒ荀��
 *			�����蔻��̑傫��	SetCollisionSize(float fx, float fy, float fz = 0.0f)
 *	
 */

//===== �C���N���[�h�K�[�h =====
#ifndef __COLLIDER_H__
#define __COLLIDER_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

#include <vector>

//===== �O���錾 =====
class CTransform;
class ObjectManager;
class Object;

//===== �\���̒�` =====

//	�����蔻��̃^�C�v
//enum COLLISION_TYPE
//{
//};

//===== �}�N����` =====


//===== �N���X��` =====
class CCollider : public Component
{
private:
	//=== �����o�ϐ�
	CTransform*		m_pTransform;				//	���W
	XMFLOAT3		m_CollisionSize;			//	�����蔻��̑傫��
	XMFLOAT2		m_CenterOffset;				//	���f���̒��S�̂���
	float			m_fRadius;					//	���a
	static std::list<Object*> m_CollidrList;	//	�����蔻�胊�X�g

	//2021/01/06
	std::list<Object*> m_IntersectObjects;	//	�Փ˂����I�u�W�F�N�g����

	//=== �����o�֐�
public:
	CCollider();								//	�R���X�g���N�^
	~CCollider() override;						//	�f�X�g���N�^
	void Start() override;						//	������
	void Update() override;						//	�X�V
	//setter
	void SetCollisionSize(float fx, float fy, float fz = 0.0f);	// �����蔻��̑傫���̐ݒ�
	void SetOffset(float fx, float fy);							// �����蔻��̒��S�ƃI�u�W�F�N�g�̒��S�Ƃ̂���
	//getter
	XMFLOAT2 GetOffSet();										// �I�u�W�F�N�g�̒��S�Ƃ̂���
	XMFLOAT3 GetColliderSize();									// �����蔻��̑傫��
	XMFLOAT2 GetCenterPos();									// �����蔻��̒��S

	static void AddColliderList(Object* pObject);				// �����蔻�胊�X�g�ɒǉ�
	static std::list<Object*>& GetColliderList();				// �����蔻��������Ă���I�u�W�F�N�g����
	static void UpdateColliders();								// �����蔻��������Ă���I�u�W�F�N�g�����̍X�V
	
private:
	//--- �����蔻��
	//	��`���m�̓����蔻��
	bool CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
		XMFLOAT2 size1, XMFLOAT2 size2, XMFLOAT2 Velocity1, XMFLOAT2 Velocity2);
	//	�~�Ɖ~
	bool CheckCollisionCircleToCircle(XMFLOAT2, XMFLOAT2, float, float);
	//	���Ƌ�
	bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
	//	�����̂ƒ�����
	bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	//	xy���ʂ̂����蔻��
	bool CollisionRectToRectXY(CTransform* pA, CTransform* pB
		, XMFLOAT2 Asize, XMFLOAT2 Bsize
		, XMFLOAT2 Aoffset = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 Boffset = XMFLOAT2(0.0f, 0.0f));
};
#endif // __COLLIDER_H__
