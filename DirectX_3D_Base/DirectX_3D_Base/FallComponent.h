/**
 * @file	FallComponent.h
 * @brief	�傫���t�̏ꍇ�̏d��
 * @author	�����q��
 * @date	2021/12/07
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __COM_FALL_H__
#define __COM_FALL_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====
#define BIGLEAFGRAVITY	(0.015f)
#define VIBRATION		(0.05f)

//===== �O���錾 =====
class CTransform;

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CFall : public Component
{
private:
	CTransform*	m_pTransform;		//	�g�����X�t�H�[��
	float m_fDegree;				//�@	�p�x
	float m_fResistance;			//�@	��R��
	bool m_bActive;					//	���E�ړ��t���O
	float m_fAngle;					//	�X���

	//2022/01/03
	bool m_bUpFlg;					//�@�㏸���Ă��邩�ǂ����t���O�@true�̎��ɏ㏸���Ă���
	bool m_bRorL;					//�@�E�ɐi�ނ����ɐi�ނ��̂��  ture�̎��ɂЂ���ɂ�����
	XMFLOAT3 m_FallPos;				//�@�����n�߂̍��W��ۑ�����ׂ̕ϐ�

public:
	CFall();				//	�R���X�g���N�^
	~CFall() override;	//	�f�X�g���N�^
	void Start() override;			//	������
	void Update() override;			//	�X�V
	void SetActive(bool bActive);	//	���E�ړ��t���O
};
#endif // __GRAVITY_H__
