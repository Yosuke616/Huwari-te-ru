/**
 * @file	LotationComponent.h
 * @brief	��]
 * @author	AT12A ���c���l
 * @date	2021/1/1	�쐬
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __LOTATION_H__
#define __LOTATION_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;


//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CLotation : public Component
{
private:
	CDraw2D* m_pDraw2D;		// �`��ݒ�
	int m_nDegree;			// �p�x

public:
	CLotation();
	~CLotation() override;
	void Start() override;
	void Update() override;
};
#endif // __LOTATION_H__