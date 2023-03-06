/**
 * @file	GetUIComponent.h
 * @brief	�A�C�e���Q�b�g����UI�R���|�[�l���g
 * @author	AT12A �u���Č�
 * @date	2022/01/29
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __GET_UI_H__
#define __GET_UI_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CGetUI : public Component
{
private:
	CTransform* m_pTransform;		// ���W
	CDraw2D* m_pDraw2D;				// �`��
	XMFLOAT2 m_vSize;				// �T�C�Y
	XMFLOAT2 m_vMaxSize;			// �ő�T�C�Y
	float m_fSizeRate;				// �g��Ԋu

	int			m_nTimer;

public:
	CGetUI();
	~CGetUI() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetUISize(XMFLOAT2 vSize);	// �傫���ݒ�
	void SetUIRate(float fRate);	// �g��X�s�[�h�ݒ�

	void SetDelayTimer(int delay);
};

#endif // __GET_UI_H__