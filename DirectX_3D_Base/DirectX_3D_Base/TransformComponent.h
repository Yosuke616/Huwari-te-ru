//=============================================================================
//
//  ���W�R���|�[�l���g�N���X[TransformComponent.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== �C���N���[�h�K�[�h ======
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �N���X��` =====
class CTransform : public Component
{
	//=== �����o�ϐ� ===
public:
	XMFLOAT3 Pos;				//	���W
	XMFLOAT3 Vel;				//	���x
	XMFLOAT3 Acc;				//	�����x
	XMFLOAT3 Scale;				//	�X�P�[��
	XMFLOAT3 Rotate;			//	��]

	//=== �����o�֐� ===
public:
	CTransform();				//	�R���X�g���N�^
	~CTransform() override;		//	�f�X�g���N�^
	void Update() override;		//	�X�V

	void SetPosition(float fx, float fy, float fz = 0);	//	���W�̐ݒ�
	void SetVelocity(float fx, float fy, float fz = 0);	//	���x�̐ݒ�
	void SetScale(float fx, float fy, float fz = 0);	//	�X�P�[���̐ݒ�
	void SetRotate(float fx, float fy, float fz = 0);	//	��]�̐ݒ�
};
#endif // __TRANSFORM_H__
