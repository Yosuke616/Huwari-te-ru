/**
 * @file	Billboard.h
 * @brief	�r���{�[�h�@�\
 * @author	AT12A ���c���l
 * @date	2021/12/02	�쐬
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

//===== �C���N���[�h =====
#include "Component.h"
#include "DrawMeshComponent.h"

//===== �O���錾 =====
class CTransform;

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CBillboard : public CDrawMesh
{
private:
	void CreateBillboardVertex();

	//=== �����o�֐� ===
public:
	CBillboard();											//	�R���X�g���N�^
	~CBillboard();											//	�f�X�g���N�^
	void Start() override;									//	�J�n
	void Update() override;									//	�X�V
	void Draw() override;									//	�`��
	void OnCollisionEnter(Object* pObject) override;		//	�Փˏ���
};


#endif // __BILLBOARD_H__