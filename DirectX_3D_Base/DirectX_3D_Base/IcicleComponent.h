/**
 * @file	IcicleComponent.h
 * @brief	���̋@�\
 * @author	�����q��
 * @date	2021/12/
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __ICICLE_H__
#define __ICICLE_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"
#include "TransformComponent.h"

//===== �O���錾 =====


//===== �N���X��` =====
class CIcicle : public Component
{
private:
	CTransform* m_pTransform;		//�I�u�W�F�N�g�̍��W
	float m_fLimitDown;				//��ʂ̉���
	int m_nIcicleHP;				//���̑ϋv�l
	bool m_bFall;					//�����܂����t���O
	bool m_bMove;					//��~�t���O
public:
	CIcicle();						// �R���X�g���N�^
	~CIcicle() override;			// �f�X�g���N�^
	void Start() override;			// ������
	void Update() override;			// �X�V
	void Draw() override;			// �`��
	void OnCollisionEnter(Object* pObject) override;

	//void SetDownIce(Object* pObject);
	void IcicleDamage();			// ���̑ϋv�l����
	void Stop();
	bool IsFalling();		// �����Ă��邩�̃t���O
	bool IsMoving();		// ���������ǂ���
};
#endif // __ICICLE_H__
