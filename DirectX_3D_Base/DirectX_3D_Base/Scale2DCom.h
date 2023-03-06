/**
 * @file	Scale2DCom.h
 * @brief	�ړI�̑傫���ɂ���
 * @author	AT12A ���c���l
 * @date	2022/01/30
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __SCALE_2D_H__
#define __SCALE_2D_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CScale2D : public Component
{
private:
	CDraw2D*	m_pDraw2D;		// �I�u�W�F�N�g�̕`����
	XMFLOAT2	m_vDestSize;	// �ړI�̍��W

	XMFLOAT2	m_vSize;

	int m_nTime;

public:
	CScale2D();					// �R���X�g���N�^
	~CScale2D() override;			// �f�X�g���N�^
	void Start() override;		// ������
	void Update() override;		// �X�V
	void Draw() override;		// �`��

	// �ړ��ړI�̍��W�̐ݒ�
	void SetDestSize(XMFLOAT2 pos);
};
#endif // __POINT_H__