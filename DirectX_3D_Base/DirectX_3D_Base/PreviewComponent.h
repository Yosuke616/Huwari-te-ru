/**
 * @file	PreviewComponent.h
 * @brief	�v���r���[�̏���
 * @author	AT12A �鑺����
 * @date	2022/01/17
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __PREVIEW_H__
#define __PREVIEW_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====


//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CPreview : public Component
{
private:
	bool m_bCollision;			// �Փ˃t���O
	bool m_bOldCollision;		// �Փ˃t���O

public:
	CPreview();					// �R���X�g���N�^
	~CPreview() override;		// �f�X�g���N�^
	void Start() override;	// ������
	void Update() override;	// �X�V
	void Draw() override;	// �`��
	void OnCollisionEnter(Object* pObject) override;
};
#endif