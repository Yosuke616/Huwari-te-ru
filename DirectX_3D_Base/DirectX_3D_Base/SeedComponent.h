/**
 * @file	SeedComponent.h
 * @brief	��
 * @author	�u���z�S
 * @date	2021/20
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __SEED_COMPONENT_H__
#define __SEED_COMPONENT_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CDrawModel;
class CCollider;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CSeed : public Component
{
private:
	//CTransform* m_pTransform;								//�@�g���X�t�H�[���������|�C���^�̕ϐ�
	//CDrawModel* m_pDraw;										//�@�`��������|�C���^�̕ϐ�
	//CCollider* m_pCollider;									//�@�Փ˔���������|�C���^�ϐ�
	bool m_bUIFlg;											//  UI��\�����Ă��悢���̃t���O

public:
	CSeed();
	~CSeed() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	//bool GetSeedFlg();
};

#endif // __SEED_COMPONENT_H__
