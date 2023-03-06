/**
 * @file	TexScrollComponent.h
 * @brief	�e�N�X�`���̃X�N���[��
 * @author	AT12A ���c���l
 * @date	2021/12/10
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __TEX_SCROLL_H__
#define __TEX_SCROLL_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;


//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CTexScroll : public Component
{
	//=== �����o�ϐ�
private:
	CDraw2D*	m_pDraw2D;				// �`��ݒ�
	float*		m_pPosX;				// �X�N���[���̊�Ƃ�����WX
	float*		m_pPosY;				// �X�N���[���̊�Ƃ�����WY

	XMFLOAT2	m_Scroll;				// �X�N���[���̒l
	XMFLOAT2	m_UVscroll;				// �X�N���[���̒l(0~1)
	XMFLOAT2	m_ScrollValue;			// �X�N���[���̗�
	static bool m_bScroll;				// �X�N���[���t���O

	//=== �����o�֐�
public:
	CTexScroll();						//	�R���X�g���N�^
	~CTexScroll() override;				//	�f�X�g���N�^
	void Start() override;				//	������
	void Update() override;				//	�X�V
	void Draw() override;				//	�`��

	//setter
	void SetAxizX(float* px);
	void SetAxizY(float* py);
	void SetScrollValue(float fx);
	void SetScrollValueY(float fy);
	static void Scroll(bool bScroll);
};

#endif // __TEX_SCROLL_H__
