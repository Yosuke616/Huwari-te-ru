//=============================================================================
//
// �����N���X [Light.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Light.h"

using namespace DirectX;

// �O���[�o���ϐ�
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 1.0f);			//	���C�g�̌���(��ʂ̉�������)
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);		//	���ˌ�(�����������Ă���)	
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);	//	����(�����������ĂȂ����)	
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);		//	���Ȕ���	

	//	�C���X�^���X�쐬
	CLight g_light;
};

// �R���X�g���N�^
CLight::CLight()
{
	//	������
	Init();
}

// ������
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;										//	���C�g�̌���(��ʂ̉�������)
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));	//	�P�ʃx�N�g���ɂ��ăX�g�A�ɕۑ�

	m_diffuse = LIGHT0_DIFFUSE;		//	���ˌ�(�����������Ă���)	
	m_ambient = LIGHT0_AMBIENT;		//	����(�����������ĂȂ����)
	m_specular = LIGHT0_SPECULAR;	//	���Ȕ���	
	m_bEnable = true;
}

// ���ˌ��̎擾
XMFLOAT4& CLight::GetDiffuse()
{
	return m_diffuse;
}

// �����̎擾
XMFLOAT4& CLight::GetAmbient()
{
	return m_ambient;
}

// ���Ȕ����̎擾
XMFLOAT4& CLight::GetSpecular()
{
	return m_specular;
}


// ���������擾
XMFLOAT3& CLight::GetDir()
{
	//	���C�gON
	if (m_bEnable) return m_direction;

	//	���C�gOFF
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

// �����C���X�^���X�擾
CLight* CLight::Get()
{
	return &g_light;
}

// ���C�gON
void CLight::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

// ���C�gO
void CLight::SetDisable(bool bDisable)
{
	m_bEnable = !bDisable;
}
