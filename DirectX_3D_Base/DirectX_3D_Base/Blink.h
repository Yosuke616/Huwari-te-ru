/**
 * @file	Blink.h
 * @brief	������_�ł�����R���|�[�l���g
 * @author	�u���z�S
 * @date	2022/1/26
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __BLINK_H__
#define __BLINK_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CBlink : public Component
{
private:
	CDraw2D* m_pDraw;	//�����x��ω������邽�߂̕ϐ�
	int m_nBlink;		//�_�ł����邽�߂̕ϐ�
	int m_nBlink_Cnt;	//�ǂ̈ʂ̊Ԋu�œ_�ł����邩�̕ϐ�
public:
	CBlink();
	~CBlink() override;
	void Start() override;
	void Update() override;
};

#endif // __�@�\��_H__

//if(FAILED(hr)) MessageBox(GetMainWnd(), _T(""), NULL, MB_OK);
