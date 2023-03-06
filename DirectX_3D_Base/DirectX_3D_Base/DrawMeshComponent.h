/**
 * @file	DrawMeshComponent.h
 * @brief	���b�V���`��R���|�[�l���g
 * @author	AT12A ���c���l
 * @date	2021/12/01
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __DRAW_MESH_H__
#define __DRAW_MESH_H__

//===== �C���N���[�h =====
#include "Component.h"
#include "mesh.h"
#include "main.h"

//===== �O���錾 =====
class CTransform;


//===== �N���X��` =====
class CDrawMesh : public Component
{
	//=== �����o�ϐ�
private:
	float m_fTexSizeX;
	float m_fTexSizeZ;

protected:
	XMFLOAT2 m_Offset;			//	�I�t�Z�b�g
	float	m_fWidth;				//	1�}�X�̉���
	float	m_fHeight;				//	1�}�X�̏c��
	XMFLOAT3		m_fNormal;		//	�@���x�N�g��
	MESH			m_Mesh;			//	���b�V��
	MATERIAL		m_Material;		//	�}�e���A��
	CTransform*		m_pTransform;	//	���W
	int m_nAnim;					//	�A�j���[�V�����ԍ�
	int m_nSplitX;					//	������X
	int m_nSplitY;					//	������Y


	//=== �����o�֐�
private:
	//	���_���쐬
	HRESULT MakeVertex(
		//int nNumBlockX, int nNumBlockZ,
		float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ,
		XMFLOAT3 normal);

protected:

public:
	CDrawMesh();					//	�R���X�g���N�^
	~CDrawMesh() override;			//	�f�X�g���N�^
	void Start() override;			//	�X�^�[�g
	void Update() override;			//	�X�V
	void Draw() override;			//	�`��
	//setter
	void SetTexture(ID3D11ShaderResourceView* pTex);//	�e�N�X�`��
	void SetUVsize(XMFLOAT2 uv);					//	�e�N�X�`�����W
	void SetSize(float width, float height);		//	�}�X�̑傫��
	void SetDiffuse(XMFLOAT4 Diffuse);				//	���ˌ�
	void SetAmbient(XMFLOAT4 Ambient);				//	����
	void SetSpecular(XMFLOAT4 Specular);			//	���ʔ��˂̐ݒ�
	void SetEmissive(XMFLOAT4 Emissive);			//	���Ȕ����̐ݒ�
	void SetPower(float power);						//	����̐ݒ�
	void SetNormal(float fx, float fy, float fz);	//	�@���x�N�g���̐ݒ�
	void SetAnimFrame(int anim);					//	�A�j���[�V�����ԍ�
	void SetAnimSplit(int nx, int ny);				//	�������̐ݒ�
	void SetOffset(XMFLOAT2 offset);					//	�I�t�Z�b�g
	void SetColor(float x, float y, float z);	// �F

	//getter
	MESH* GetMesh();								//	���b�V�����̎擾
};


class CSphere : public CDrawMesh
{
	//=== �����o�ϐ�
private:
	static bool m_bVisible;		//	�����邩�ǂ���
	float m_fRadius;			//	���̔��a
	XMFLOAT4X4* m_pWorld;		//	���[���h���W
	
	//=== �����o�֐�
private:
	void InitSphere();								//	���_���̍쐬

public:
	CSphere();										//	�R���X�g���N�^
	~CSphere() override;								//	�f�X�g���N�^
	void Start() override;								//	�X�^�[�g
	void Update() override;								//	�X�V
	void Draw() override;								//	�`��
	//setter
	void SetRadius(float rad);							//	���̔��a
	void SetTransform(CTransform* pTransform);			//	�\��������W
	void SetWorldPos(XMFLOAT4X4* pWorld);				//	���[���h���W
	

	static void SetSphereVisible(bool visible = true);	//	�\���t���O�̐ݒ�
	static void CreateSphere(CTransform* transform, float radius, XMFLOAT4X4* world);	//	�f�o�b�O���̐���
};
#endif // __DRAW_MESH_H__