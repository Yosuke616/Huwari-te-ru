/**
 * @file	IceBlockCom.h
 * @brief	�X�u���b�N�̏���
 * @author	AT12A ���c���l
 * @date	���t�i�J�n���H�j
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __ICE_BLOCK_H__
#define __ICE_BLOCK_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CIceBlock : public Component
{
private:
	CTransform* m_pTransform;
	int m_nDirect;				// �i��ł������
	XMFLOAT2 m_OldPos;
public:
	CIceBlock();
	~CIceBlock() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	int GetDirect();

	int m_nCloverNumber;
	void SetNumber(int num);
	int GetNumber();
};

#endif // __�@�\��_H__