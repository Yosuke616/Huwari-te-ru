/**
 * @file	FlashComponent.h
 * @brief	�_��(���l)��������
 * @author	AT12A �u���Č�
 * @date	2022/01/28
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __FLASH_H__
#define __FLASH_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CFlash : public Component
{
private:
	CTransform* m_pTransform;		// ���W
	CDraw2D* m_pDraw2D;				// �`��
	float m_fAlpha;					// ���l
	int m_nDrawTime;				// �`�掞��
	int m_nNoneTime;				// �`�悵�Ȃ�����
	int m_nKeepAlpha;				// �ۑ��p(�`�掞��)
	int m_nKeepNone;				// �ۑ��p(�`�悵�Ȃ�����)

public:
	CFlash();
	~CFlash() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	//Setter
	void SetAlphaTime(int nTime);	// �`�掞�Ԑݒ�
	void SetNoneTime(int nTime);	// �`�悵�Ȃ����Ԑݒ�
};

#endif // __FLASH_H__