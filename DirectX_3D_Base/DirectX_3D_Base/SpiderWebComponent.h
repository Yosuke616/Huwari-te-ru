/**
 * @file	SpiderWebComponent.h
 * @brief	�u�N���̑��v�M�~�b�N�̎���
 * @author	AT12A �鑺����
 * @date	2021/12/25 �쐬
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __SPIDER_H__
#define __SPIDER_H__

//===== �C���N���[�h =====
#include "Component.h"
#include <map>

//===== �O���錾 =====
class CTransform;
class CCollider;

//===== �}�N����` =====
#define WIND_ATTACK_NUM		(200)	//�S������������ۂɕK�v�ȕ��̐�

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CSpiderWeb : public Component
{
private:
	CTransform	*m_pTransform;		// �N���̑��̃g�����X�t�H�[�����
	CCollider	*m_pCollider;		// �N���̑����̂��̂̓����蔻��
	XMFLOAT2 m_UpColliderPos;		// �N���̑��㕔���̓����蔻����W
	XMFLOAT2 m_DownColliderPos;		// �N���̑��������̓����蔻����W

	XMFLOAT2 m_BindSize;			// �S���͈�

	bool m_bUpColFlg;				// �㕔�������ɔ�΂��ꂽ���t���O
	bool m_bDownColFlg;				// �������@�c

	std::map<Object*, XMFLOAT2>	m_BindObjects;
	std::map<Object*, bool>		m_BindFlags;

	//std::map<Object*, bool>		m_bBindFlags;
	//std::list<Object*> m_pBindObject;
	//Object*		m_pBindObject;		//�S�������I�u�W�F�N�g�̃g�����X�t�H�[�����
	//XMFLOAT2 m_BindPlayerPos;		//�v���C���[�I�u�W�F�N�g�S�����W
	//std::list<XMFLOAT2> m_BindPos;	//���̑��ڐG�I�u�W�F�N�g�S�����W
	//bool m_bPlayerBindFlg;			//�v���C���[�I�u�W�F�N�g�̍S���t���O
	//bool m_bBindFlg;				//���̑��ڐG�I�u�W�F�N�g�̍S���t���O

	int m_nWindCnt;
	int nCnt;

public:
	CSpiderWeb();
	~CSpiderWeb() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	void SetBindSize(float, float);	//�S���͈�		
};
#endif // __SPIDER_H__
