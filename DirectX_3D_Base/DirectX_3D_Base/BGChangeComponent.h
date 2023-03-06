/**
 * @file	BGChangeComponent.h
 * @brief	�w�i�����Ԍo�߂ŕς���Ă����R���|�[�l���g
 * @author	�u���Č�
 * @date	2021/12/28
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __BG_CHANGE_H__
#define __BG_CHANGE_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CBgChange : public Component
{
private:
	CDraw2D* m_pDraw2D;			// 2D�`��
	int m_nTime;				// ����
	int m_nCmpTimer;			// ���ԕۑ�
	int m_nTexNum;				// �e�N�X�`��
	bool m_bFlg;				// �t���O

public:
	CBgChange();				// �R���X�g���N�^
	~CBgChange() override;		// �f�X�g���N�^
	void Start() override;		// ������
	void Update() override;		// �X�V
	void Draw() override;		// �`��

	void ChangeTexture();		// �e�N�X�`���ؑ�
	void SetTimer(int nTimer);	// ���Ԑݒ�
};

#endif // __BG_CHANGE_H__