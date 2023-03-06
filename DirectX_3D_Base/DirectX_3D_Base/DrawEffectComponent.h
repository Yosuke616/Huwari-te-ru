/**
 * @file	DrawEffectComponent.h
 * @brief	�G�t�F�N�g�`��R���|�[�l���g
 * @author	AT12A ���c���l
 * @date	2021/12/06
 *			2021/12/17	�G�t�F�N�g�����[�v������@�\�̒ǉ�
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __EFFECT_H__
#define __EFFECT_H__

//===== �C���N���[�h =====
#include "DrawMeshComponent.h"


//===== �O���錾 =====


//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CDrawEffect : public CDrawMesh
{
	//=== �����o�ϐ�
private:
	
	int  m_nFrame;		//	�A�j���[�V�����؂�ւ��̊��o
	int	 m_nAnimCount;	//	�A�j���[�V�����J�E���^
	bool m_bLoop;		//	�A�j���[�V�������[�v�t���O

	//=== �����o�֐�
private:
	void CreateEffectVertex();				//	���_���쐬

public:
	CDrawEffect();							//	�R���X�g���N�^
	~CDrawEffect() override;				//	�f�X�g���N�^
	void Start() override;					//	�X�^�[�g
	void Update() override;					//	�X�V
	void Draw() override;					//	�`��
	//setter
	void SetSwapFrame(int nCount);			//	�A�j���[�V�����J�E���^
	void SetLoop(bool loop);

	// �G�t�F�N�g����
	static void StartEffect(ID3D11ShaderResourceView* pTex,
		XMFLOAT3 pos, XMFLOAT2 size, int split_x, int split_y
		, int swap_frame = 1
		, XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f));
};

#endif // __EFFECT_H__