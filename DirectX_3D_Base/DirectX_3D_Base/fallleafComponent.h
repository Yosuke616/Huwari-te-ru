/**
 * @file	FallLeafComponent.h
 * @brief	�傫���t�̏ꍇ�̏d��
 * @author	�����q��
 * @date	2021/12/07
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __FALLLeaf_H__
#define __FALLLeaf_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====


//===== �O���錾 =====
class CTransform;

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CFallLeaf : public Component
{
private:
	CTransform*	m_pTransform;		//	�g�����X�t�H�[��
	int m_nDegree;					//�@�p�x
	int m_nRandX;					//	���ɂ���邽�߂̗���
	
	XMFLOAT3 m_RotVelocity;			//	��]���x

public:
	CFallLeaf();				//	�R���X�g���N�^
	~CFallLeaf() override;	//	�f�X�g���N�^
	void Start() override;			//	������
	void Update() override;			//	�X�V
	void SetRotVelocity(float x, float y, float z);
};
#endif // __GRAVITY_H__
