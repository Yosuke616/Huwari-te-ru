/**
 * @file	PlayerComponent.h
 * @brief	�v���C���[�̓��͑���̃R���|�[�l���g
 * @author	AT12A ���c���l
 * @date	2021/12/06
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __INPUT_PLAYER_H__
#define __INPUT_PLAYER_H__

//===== �C���N���[�h =====
#include "Component.h"
//2022/1/27/Touch_Tutorial ------------------------------
#include <list>
//-------------------------------------------------------

//===== �O���錾 =====
class InputManager;
class CTransform;
class Object;
class CCollider;
class CDrawModel;


//===== �N���X��` =====
class CPlayer : public Component
{
private:
	CTransform* m_pPlayer;		//	���W�A�g�k�A��]
	CDrawModel*	m_pDraw3D;		// �`����
	CCollider*	m_Collider;
	 bool		m_bGoalFlg;		//	�S�[���ł��邩�̃t���O
	 bool		m_bGround;		//	�n��t���O

	 bool		m_bClover[4];	//	�N���[�o�[�̏�����
	 XMFLOAT3	m_rotDestModel;	//	�ړI�̌���
	 float m_fDiifRotY;			//	�p���x�̊���

	 XMFLOAT2 m_OldPos;			// �ߋ����W
	 int m_nAnimState;			// �A�j���[�V�������

	 //2022/01/11
	 int m_nColWindCnt;	// ���ɓ������Ă܂�

	 //2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------------------
	 bool m_bTouch_Tutorial;	//�@���̃t���O���I���Ȃ�΃`���[�g���A���͏o���Ȃ�
	 //----------------------------------------------------------------------------
	 //2022/1/27Touch_Tutorial Shimizu Yosuke -------------------------------------
	 std::list<Object*> m_Delete_UI;			//UI���������߂̃��X�g
	 //----------------------------------------------------------------------------


public:
	CPlayer();											//	�R���X�g���N�^
	~CPlayer() override;								//	�f�X�g���N�^
	void Start() override;								//	������
	void Update() override;								//	�X�V
	void Draw() override;								//	�`��
	void OnCollisionEnter(Object* pObject) override;	//	�Փˏ���

	//setter
	void SetDestRot(float angle);
	void SetGround(bool ground);
	void SetAnimState(int state);
};
#endif // __COMPONENT_H__

