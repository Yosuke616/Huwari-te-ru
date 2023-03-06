/**
 * @file	BudComponent.h
 * @brief	�ڂ݂̏���
 * @author	AT12A ���c���l
 * @date	2022/01/17
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __BUD_H__
#define __BUD_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====


//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CBud : public Component
{
private:
	bool m_bCollision;			// �Փ˃t���O
	bool m_bOldCollision;		// �Փ˃t���O
	bool m_bOpen;				// �J���t���O

public:
	CBud();					// �R���X�g���N�^
	~CBud() override;		// �f�X�g���N�^
	void Start() override;	// ������
	void Update() override;	// �X�V
	void Draw() override;	// �`��
	void OnCollisionEnter(Object* pObject) override;
};
#endif