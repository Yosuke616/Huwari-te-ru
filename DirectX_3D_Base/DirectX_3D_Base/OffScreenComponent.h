/**
 * @file	OffScreenComponent.h
 * @brief	��ʊO����
 * @author	AT12A ���c���l
 * @date	2021/1/1
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __OFF_SCREEN_H__
#define __OFF_SCREEN_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CCollider;


//===== �񋓑̐錾 =====
enum OFF_SCREEN
{
	// ��ʊO�������e
	OFF_SCREEN_DELETE,	// �I�u�W�F�N�g�̍폜
	ON_SCREEN,			// ��ʓ��ɓ����

	MAX_OFF_SCREEN
};


//===== �N���X��` =====
class COffScreen : public Component
{
	//=== �����o�ϐ�
private:
	CTransform* m_pTransform;	// �I�u�W�F�N�g�̍��W
	CCollider* m_pCollider;		// �����蔻��
	OFF_SCREEN m_eFunction;		// ��ʊO�������e

	//=== �����o�֐�
private:
	void Delete();				// �폜
	void OnScreen(int);			// ��ʓ��ɓ����
public:
	COffScreen();				// �R���X�g���N�^
	~COffScreen() override;		// �f�X�g���N�^
	void Start() override;		// ������
	void Update() override;		// �X�V
	void Draw() override;		// �`��
	//setter
	void SetFinction(OFF_SCREEN fnc);
};
#endif // __OFF_SCREEN_H__