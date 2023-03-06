/**
 * @file	HormingComponent.h
 * @brief	���W���}�E�X�ɃZ�b�g����
 * @author	AT12A ���c���l
 * @date	2021/12/10
 *			2022/01/17	�u���ČށF�}�E�X�Ǐ]�̃G�t�F�N�g�ǉ�
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __HOMING_H__
#define __HOMING_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CHoming : public Component
{
	//=== �����o�ϐ�
private:
	CTransform* m_pTransform;
	XMFLOAT2 m_vCmpMousePos;			// �ߋ��̍��W
	int m_nStopTime;					// �~�܂��Ă���Ƃ��̐�������
	int m_nMoveTime;					// �����Ă�Ƃ��̐�������
	int m_nEffectID;					// �e�N�X�`���̔ԍ�

	//=== �����o�֐�
public:
	CHoming();											// �R���X�g���N�^
	~CHoming() override;								// �f�X�g���N�^
	void Start() override;								// ������
	void Update() override;								// �X�V
	void Draw() override;								// �`��
	void OnCollisionEnter(Object* pObject) override;	// �Փˏ���

	void SetEffectNum(int nEffectNum);					// �G�t�F�N�g��ݒ�
};

#endif // __HOMING_H__
