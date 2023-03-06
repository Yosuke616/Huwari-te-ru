/**
 * @file	FadeComponent.h
 * @brief	�G�߂̃��S���o������ɏ��X�ɏ����Ă���
 * @author	�u���Č�
 * @date	2021/12/22
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __FADE_COMPONENT_H__
#define __FADE_COMPONENT_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CFade : public Component
{
private:
	CDraw2D*	m_pDraw2D;	// 2D�`��
	float		m_fTime;	// �o�ߎ���
	float		m_fAlpha;	// ���l
	XMFLOAT2	m_vPos;		// UV���W
	XMFLOAT2	m_vScrPos;	// �X�N���[�����W

public:
	CFade();				// �R���X�g���N�^
	~CFade() override;		// �f�X�g���N�^
	void Start() override;	// ������
	void Update() override;	// �X�V
	void Draw() override;	// �`��
};

#endif // __FADE_COMPONENT_H__
