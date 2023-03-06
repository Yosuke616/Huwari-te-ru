/**
 * @file	CloverObjectComponent.h
 * @brief	UI�I�u�W�F�N�g�̊Ǘ�
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __CloverComponent_H__
#define __CloverComponent_H__

//===== �C���N���[�h =====
#include"Component.h"

//===== �O���錾 =====
class CloverComponent;
class CTransform;
class CCollider;
class Object;
class TextureManager;
class ObjectManager;
class CInputPlayer;

//===== �}�N����` =====
//#define CLOVER_NUM_MAX	(5)
#define RATE_MOVE_CLOVER	(0.02f)

//2021/12/24 Shimizu Yosuke ---------------------------------------

//-----------------------------------------------------------------

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CloverComponent : public Component
{
	//=== �����o�ϐ�
private:
	CTransform* m_pTransform;	//	�g�����X�t�H�[��

	//2021/12/22 Shimizu Yosuke-----------------------------------
	int m_nNumber;					//�}�b�v�`�b�v�̔ԍ���ۑ�����ϐ�
	//------------------------------------------------------------

	CCollider* m_pCollider;
	XMFLOAT2 m_OldPos;

	bool m_bCloverflag;

	int m_nCnt_Clover;			//�@�N���[�o�[�̐����i�[����ϐ�

	bool m_bBind;				// �N���̑��Ɉ����������Ă�t���O

	//=== �����o�ϐ�
public:
	CloverComponent();		//	�R���X�g���N�^
	~CloverComponent();		//	�f�X�g���N�^
	void Start() override;	//	������
	void Update() override;	//	�X�V
	void Draw() override;	//	�`��
	void OnCollisionEnter(Object* pObject) override;	//	�Փˏ���

	static Object* CreateClover(XMFLOAT3 pos, int id);

	//2021/12/23 Shimizu Yosuke---------------------------------
	void SetNumber(int number);
	int GetNumber();
	//----------------------------------------------------------

	void Binded(bool bBind);

	bool IsBinded();

};

#endif  __CloverComponent_H___

