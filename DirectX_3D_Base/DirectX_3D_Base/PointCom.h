/**
 * @file	PointCom.h
 * @brief	�w�肵�����W�ɂ������ړ�������
 * @author	AT12A ���c���l
 * @date	2022/01/30
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __POINT_H__
#define __POINT_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CPoint : public Component
{
private:
	CTransform* m_pTransform;	// �I�u�W�F�N�g�̍��W
	XMFLOAT2	m_vDestPos;		// �ړI�̍��W

public:
	CPoint();					// �R���X�g���N�^
	~CPoint() override;			// �f�X�g���N�^
	void Start() override;		// ������
	void Update() override;		// �X�V
	void Draw() override;		// �`��

	// �ړ��ړI�̍��W�̐ݒ�
	void SetDestPos(XMFLOAT2 pos);
};
#endif // __POINT_H__