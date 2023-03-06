//===== �C���N���[�h =====
#include "TransformComponent.h"

// �R���X�g���N�^
CTransform::CTransform()
{
	m_eUpdateOrder = COM_UPDATE_TRANSRATE;	//	�R���|�[�l���gList�̍X�V�����̐ݒ�

	Pos		= XMFLOAT3(0.0f, 0.0f, 0.0f); 
	Vel		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Acc		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scale	= XMFLOAT3(1.0f, 1.0f, 1.0f);
	Rotate	= XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
CTransform::~CTransform()
{
}

// �X�V
void CTransform::Update()
{
	Vel.x += Acc.x;	//	�����x�����Z
	Vel.y += Acc.y;
	Vel.z += Acc.z;
	Pos.x += Vel.x;	//	���W�ɑ��x�����Z
	Pos.y += Vel.y;
	Pos.z += Vel.z;
}

// ���W�̐ݒ�
void CTransform::SetPosition(float fx, float fy, float fz)
{
	Pos.x = fx;
	Pos.y = fy;
	Pos.z = fz;
}

// ���x�̐ݒ�
void CTransform::SetVelocity(float fx, float fy, float fz)
{
	Vel.x = fx;
	Vel.y = fy;
	Vel.z = fz;
}

// �X�P�[���̐ݒ�
void CTransform::SetScale(float fx, float fy, float fz)
{
	Scale.x = fx;
	Scale.y = fy;
	Scale.z = fz;
}

// ��]�̐ݒ�
void CTransform::SetRotate(float fx, float fy, float fz)
{
	Rotate.x = fx;
	Rotate.y = fy;
	Rotate.z = fz;
}
