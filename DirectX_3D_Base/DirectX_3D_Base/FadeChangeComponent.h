/**
 * @file	FadeChangeComponent.h
 * @brief	���Ԍo�߂Ń|���S����\��
 * @author	�u���Č�
 * @date	2022/01/06
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __FADE_CHANGE_H__
#define __FADE_CHANGE_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CDraw2D;

//===== �}�N����` =====


//===== �񋓑̐錾 =====
enum EChange
{
	CHANGE_NONE = 0,
	CHANGE_IN,
	CHANGE_OUT,

	MAX_CHANGE
};


//===== �N���X��` =====
class CFadeChange : public Component
{
private:
	CTransform* m_pTransform;	// �g�����X�t�H�[��
	CDraw2D* m_pDraw2D;			// 2D�`��
	int m_nTimer;				// ����
	int m_nCmpTimer;
	float m_fAlpha;				// ���l
	XMFLOAT3 m_fColor;			// �F
	bool m_bFlg;				// �ؑփt���O

public:
	CFadeChange();
	~CFadeChange() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	void SetTimer(int nTimer);
};

#endif // __FADE_CHANGE_H__