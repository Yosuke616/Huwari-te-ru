/**
 * @file	FollowComponent.h
 * @brief	���̃I�u�W�F�N�g�ɂ��Ă���
 * @author	AT12A ���c���l
 * @date	2021/12/28
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __FOLLOW_H__
#define __FOLLOW_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;


//===== �N���X��` =====
class CFollow : public Component
{
private:
	CTransform* m_pTransform;		// �I�u�W�F�N�g�̍��W
	Object* m_pFollowObject;		// ���̃I�u�W�F�N�g�����Ă����Ώۂ̍��W
public:
	CFollow();						// �R���X�g���N�^
	~CFollow() override;			// �f�X�g���N�^
	void Start() override;			// �J�n
	void Update() override;			// �X�V
	void Draw() override;			// �`��
	void OnCollisionEnter(Object* pObject) override;	// �Փˏ���
	//setter
	void SetParent(Object* pParent);// ���̃I�u�W�F�N�g�����Ă����Ώۂ̍��W�̐ݒ�
};

#endif // __FOLLOW_H__