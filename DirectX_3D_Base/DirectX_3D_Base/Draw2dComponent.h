/**
 * @file	Draw2dComponent.h
 * @brief	2D��ʕ`��N���X
 * @author	AT12A ���c���l
 * @date	2021\07/XX �쐬
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __DRAW_H__
#define __DRAW_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

class CTransform;

//===== �N���X��` =====
class CDraw2D : public Component
{
private:

	

protected:
	//=== �����o�ϐ� ===
	CTransform* m_pTransform;					//	���W
	ID3D11ShaderResourceView*	m_pTexture;		//	�e�N�X�`��
	XMFLOAT2	m_fPolygonSize;					//	�l�p�`�|���S���̑傫��
	XMFLOAT2	m_fTexFrameSize;				//	�e�N�X�`�������͈�
	XMFLOAT4	m_fVertexColor;					//	���_�J���[
	float		m_fAngle;						//	�p�x
	XMFLOAT2	m_Scroll;						//	�X�N���[��
	EBlendState m_BlendState;					//	�u�����h�̎��
	int			m_nAnimNum;						//	�A�j���[�V�����ԍ�
	int			m_nAnimSplitX;					//	���̃A�j���[�V����������
	int			m_nAnimSplitY;					//	�c�̃A�j���[�V����������

public:
	//=== �����o�֐� ===
	CDraw2D();									//	�R���X�g���N�^
	~CDraw2D() override;						//	�f�X�g���N�^
	void Start() override;						//	�J�n
	void Update() override;						//	�X�V
	void Draw() override;						//	�`��

	//setter
	void SetSize(float, float);					//	�|���S���T�C�Y
	void SetUVsize(float, float);				//	UV�T�C�Y
	void SetAnimSplit(int, int);				//	�A�j���[�V����������
	void SetAnimNum(int);						//	�A�j���[�V�����ԍ�
	void SetTexture(ID3D11ShaderResourceView*);	//	�e�N�X�`��
	void SetColor(float r, float g, float b);	//	�J���[
	void SetAlpha(float alpha);					//	�A���t�@�l
	void SetAngle(float fAngle);				//	�p�x
	void SetScroll(XMFLOAT2 scroll);			//	�X�N���[����
	void SetBlend(EBlendState state);			//	�u�����h�X�e�[�g
	void SetU(float u);
	void SetV(float v);

	int GetAnimSplitX();
};

class CDrawEffect2D : public CDraw2D
{
private:
	int  m_nFrame;		//	�A�j���[�V�����؂�ւ��̊��o
	int	 m_nAnimCount;	//	�A�j���[�V�����J�E���^
	bool m_bLoop;		//	�A�j���[�V�������[�v�t���O
	bool m_bPlay;
public:
	CDrawEffect2D();
	~CDrawEffect2D();
	void Update() override;					//	�X�V
	//setter
	void SetSwapFrame(int nCount);			//	�A�j���[�V�����J�E���^
	void SetLoop(bool loop);
	void SetPlayONOFF(bool onoff);			//�A�j���[�V�����Đ����邩�ǂ���

	// �G�t�F�N�g����
	static void StartEffect(ID3D11ShaderResourceView* pTex,
		XMFLOAT2 pos, XMFLOAT2 size, int split_x, int split_y
		, int swap_frame = 1
		, XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f));
};


#endif // __DRAW_H__
