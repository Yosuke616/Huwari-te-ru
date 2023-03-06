/**
 * @file	Draw3dComponent.h
 * @brief	3D�`��R���|�[�l���g
 * @author	RIKUTO NARITA
 * @date	2021/11/26
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __DRAW_3D_H__
#define __DRAW_3D_H__

//===== �C���N���[�h =====
#include "Component.h"
#include "AssimpModel.h"
#include "main.h"

//===== �O���錾 =====
class CTransform;
class Animation;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CDrawModel : public Component
{
	//=== �����o�֐� ===
private:
	CAssimpModel*	m_model;		//	���f��
	XMFLOAT4X4		m_mtxWorld;		//	���[���h�}�g���b�N�X
	CTransform*		m_pTransform;	//	�I�u�W�F�N�g�̍��W
	Animation*		m_pAnim;			//	�A�j���[�V����
	TAssimpMaterial* m_pMaterial;	//	�}�e���A�����

	//=== �����o�֐� ===
public:
	CDrawModel();						//	�R���X�g���N�^
	~CDrawModel() override;			//	�f�X�g���N�^
	void Start() override;			//	�X�^�[�g
	void Update() override;			//	�X�V
	void Draw() override;			//	�`��
	//setter
	void SetModel(CAssimpModel* model);					// ���f��
	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);	// �v���~�e�B�u�ݒ�
	void SetTexture(ID3D11ShaderResourceView* pTexture);// ���f���̃e�N�X�`��
	void SetAnimTime(double dTime);						// �A�j���[�V��������

	double GetAnimDuration();

	//getter
	XMFLOAT4X4* GetWorldPos();							// ���[���h�s��̎擾
	XMFLOAT3 GetBox();									// ���f���̑傫��

	CAssimpModel* GetModel();

	//2022/01/09 animation
	void AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame = -1);
	void Play(int AnimID);							// �A�j���[�V�����Đ�
	void AlwaysAnimation(bool always);				// ��ɃA�j���[�V����
	void SetPlaySpeed(int AnimID, double speed);	// �Đ����x
	bool AnimEnd(int AnimID);
};
#endif // __�@�\��_H__