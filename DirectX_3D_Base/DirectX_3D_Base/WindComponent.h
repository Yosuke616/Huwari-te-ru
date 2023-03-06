/**
 * @file	Wind_Mouse.h
 * @brief	���ŕ����o��
 * @author	�u���z�S
 * @date	2021/12/8
 *			2021/12/11		
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __WIND_H__
#define __WIND_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CWind : public Component
{
	//=== �����o�ϐ�
private:
	XMFLOAT2 m_Vector;				//	���̌���

	//=== �����o�֐�
public:
	CWind();											//	�R���X�g���N�^
	~CWind() override;									//	�f�X�g���N�^
	void Start() override;								//	������
	void Update() override;								//	�X�V
	void Draw() override;								//	�`��
	void OnCollisionEnter(Object* pObject) override;	//	�Փˏ���
	//setter
	void SetWindVector(XMFLOAT2 vector);				//	���̌���(x����,y����)�̐ݒ�
	//getter
	XMFLOAT2 GetAngle();								//	���̌����̎擾
};

#endif // __�@�\��_H__