//=============================================================================
//
// �����N���X [Light.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

class CLight
{
	//=== �����o�ϐ� ===
private:
	XMFLOAT4 m_diffuse;		//	���ˌ�(�����������Ă���)
	XMFLOAT4 m_ambient;		//	����(�����������ĂȂ����)
	XMFLOAT4 m_specular;	//	���Ȕ���
	XMFLOAT3 m_direction;	//	���C�g�̌���
	bool m_bEnable;			//	���C�e�B���O�L��/����


	//=== �����o�֐� ===
public:
	CLight();								//	�R���X�g���N�^
	void Init();							//	������
	XMFLOAT4& GetDiffuse();					//	���ˌ��̎擾
	XMFLOAT4& GetAmbient();					//	�����̎擾
	XMFLOAT4& GetSpecular();				//	���Ȕ����̎擾
	XMFLOAT3& GetDir();						//	���������擾
	void SetEnable(bool bEnable = true);	//	���C�gON
	void SetDisable(bool bDisable = true);	//	���C�gOFF

	static CLight* Get();					//	���C�g�̃C���X�^���X�擾
};
