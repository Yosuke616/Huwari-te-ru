//=============================================================================
//
//  �R���|�[�l���g�̊��N���X[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== �C���N���[�h�K�[�h ======
#ifndef __GRAVITY_H__
#define __GRAVITY_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �}�N����` =====
#define GRAVITY		(0.03f)

//===== �O���錾 =====
class CTransform;

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CGravity : public Component
{
private:
	CTransform*	m_pTransform;	//	�g�����X�t�H�[��

public:
	CGravity();					//	�R���X�g���N�^
	~CGravity() override;		//	�f�X�g���N�^
	void Start() override;		//	������
	void Update() override;		//	�X�V

};
#endif // __GRAVITY_H__
