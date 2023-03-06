//=============================================================================
//
//  �R���|�[�l���g�̊��N���X[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== �C���N���[�h�K�[�h ======
#ifndef __EGRAVITY_H__
#define __EGRAVITY_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====
#define EGRAVITY			(0.01f)

//===== �O���錾 =====
class CTransform;

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CEGravity : public Component
{
private:
	CTransform*	m_pTransform;	//	�g�����X�t�H�[��

public:
	CEGravity();					//	�R���X�g���N�^
	~CEGravity() override;		//	�f�X�g���N�^
	void Start() override;		//	������
	void Update() override;		//	�X�V

};
#endif // __EGRAVITY_H__
