/**
 * @file	InformationComponent.h
 * @brief	���̃I�u�W�F�N�g����m�点��
 * @author	AT12A ���c���l
 * @date	2021/12/30
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __UI_H__
#define __UI_H__

//===== �C���N���[�h =====
#include "Component.h"


//===== �N���X��` =====
class CInformation : public Component
{
private:
	Object* m_pObject;	// �I�u�W�F�N�g���
public:
	CInformation();							// �R���X�g���N�^
	~CInformation() override;				// �f�X�g���N�^
	void Update() override;					// �X�V
	void SetObjectInfo(Object* pObject);	// �I�u�W�F�N�g���̎擾
	Object* GetObjetInfo();					// �I�u�W�F�N�g���̐ݒ�
};
#endif // __COMPONENT_H__

