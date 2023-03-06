#include "DrawMeshComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Object.h"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(10.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

// �R���X�g���N�^
CDrawMesh::CDrawMesh()
	:m_pTransform(nullptr)					//	���W
	, m_fWidth(100.0f), m_fHeight(100.0f)	//	�傫��
	, m_Offset(0.0f, 0.0f)
	, m_nAnim(0)							//	�A�j���[�V�����t���[��
	, m_nSplitX(1), m_nSplitY(1)			//	������
	, m_fTexSizeX(1.0f), m_fTexSizeZ(1.0f)
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	m_Mesh.primitiveType = PT_TRIANGLESTRIP;
	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	// �ʒu�E��]�̏����ݒ�
	m_Mesh.pos	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Mesh.rot	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fNormal	= XMFLOAT3(0.0f, -1.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_Material.Diffuse	= M_DIFFUSE;
	m_Material.Ambient	= M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power	= M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial	= &m_Material;
}

// �f�X�g���N�^
CDrawMesh::~CDrawMesh()
{
	// ���b�V�����
	ReleaseMesh(&m_Mesh);
}

// �J�n
void CDrawMesh::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();

	// ���_���̍쐬
	 MakeVertex(
		//m_NumX, m_NumZ,
		m_fWidth, m_fHeight,
		m_fTexSizeX, m_fTexSizeZ,
		m_fNormal);

	 CDrawMesh::Update();
}

// �X�V
void CDrawMesh::Update()
{
	m_Mesh.pos.x = m_pTransform->Pos.x + m_Offset.x;
	m_Mesh.pos.y = m_pTransform->Pos.y + m_Offset.y;
	m_Mesh.pos.z = m_pTransform->Pos.z;
	m_Mesh.rot = m_pTransform->Rotate;

	// ���b�V���X�V
	UpdateMesh(&m_Mesh);
}

// �`��
void CDrawMesh::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//static float u = 0.0f;
	//	u += 0.01f;
	////	�e�N�X�`�� �}�g���N�X�X�V
	//XMMATRIX mtxTex = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation(u / 4, 1.0f, 0.0f));
	//XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	// ���b�V���̕`��
	DrawMesh(pDC, &m_Mesh);
}


 /**
 * @fn		CDrawMesh::MakeVertex
 * @brief	���_���̍쐬
 * @param	(float) ���̑傫��
 * @param	(float) �c�̑傫��
 * @param	(float) UV�̉���(0�`1)
 * @param	(float) UV�̏c��(0�`1)
 * @param	(XMFLOAT3) �@���x�N�g��
 * @return	������ S_OK
 */
HRESULT CDrawMesh::MakeVertex(
	float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ,
	XMFLOAT3 normal)
{
	//	�v���~�e�B�u��ʐݒ�
	m_Mesh.primitiveType = PT_LINE;

	//	���_���̐ݒ�
	m_Mesh.nNumVertex = (1 + 1) * (1 + 1);

	//	�C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	m_Mesh.nNumIndex = 8;

	//	���_�z��̍쐬
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	//	�C���f�b�N�X�z��̍쐬
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	//	���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	//	���_���W�̐ݒ�
	pVtx[0].vtx.x = m_fWidth *-0.5f;
	pVtx[1].vtx.x = m_fWidth * 0.5f;
	pVtx[2].vtx.x = m_fWidth * 0.5f;
	pVtx[3].vtx.x = m_fWidth *-0.5f;
	pVtx[0].vtx.y =  m_fHeight * 0.5f;
	pVtx[1].vtx.y =  m_fHeight *-0.5f;
	pVtx[2].vtx.y =  m_fHeight * 0.5f;
	pVtx[3].vtx.y =  m_fHeight *-0.5f;
	pVtx[0].vtx.z =  0.0f;	pVtx[1].vtx.z = 0.0f;	pVtx[2].vtx.z = 0.0f;  pVtx[3].vtx.z = 0.0f;

	for (int z = 0; z < 2; ++z)
	{
		for (int x = 0; x < 2; ++x)
		{
			//	�@���̐ݒ�
			pVtx->nor = XMFLOAT3(normal.x, normal.y, normal.z);

			//	���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			//	�e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//	�C���f�b�N�X�z��̒��g�𖄂߂�
	int* pIdx = pIndexWk;
	pIdx[0] = 0;
	pIdx[1] = 2;

	pIdx[2] = 2;
	pIdx[3] = 1;

	pIdx[4] = 1;
	pIdx[5] = 3;
	
	pIdx[6] = 3;
	pIdx[7] = 0;


	//	���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	HRESULT hr = MakeMeshVertex(GetDevice(), &m_Mesh, pVertexWk, pIndexWk);

	//	�ꎞ�z��̉��
	delete[] pVertexWk;
	delete[] pIndexWk;

	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("���b�V���쐬���s"), NULL, MB_OK);
	}

	return hr;
}

//setter
// ���b�V���̃T�C�Y�ݒ�
void CDrawMesh::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;
}
// �e�N�X�`���̐ݒ�
void CDrawMesh::SetTexture(ID3D11ShaderResourceView* pTex)
{
	if(pTex)	pTex->AddRef();				//	�Q�ƃJ�E���^�{�P
	m_Mesh.pTexture = pTex;
	if (m_Mesh.pTexture)
		XMStoreFloat4x4(&m_Mesh.mtxTexture, XMMatrixIdentity());
}
// ���ˌ��̐ݒ�
void CDrawMesh::SetDiffuse(XMFLOAT4 Diffuse)
{
	m_Material.Diffuse = Diffuse;
}
// �����̐ݒ�
void CDrawMesh::SetAmbient(XMFLOAT4 Ambient)
{
	m_Material.Ambient = Ambient;
}
// ���ʔ��˂̐ݒ�
void CDrawMesh::SetSpecular(XMFLOAT4 Specular)
{
	m_Material.Specular = Specular;
}
// ���Ȕ����̐ݒ�
void CDrawMesh::SetEmissive(XMFLOAT4 Emissive)
{
	m_Material.Emissive = Emissive;
}
// ����̐ݒ�
void CDrawMesh::SetPower(float power)
{
	m_Material.Power = power;
}
// �@���x�N�g���̐ݒ�
void CDrawMesh::SetNormal(float fx, float fy, float fz)
{
	m_fNormal.x = fx;
	m_fNormal.y = fy;
	m_fNormal.z = fz;
}
// �e�N�X�`�����W
void CDrawMesh::SetUVsize(XMFLOAT2 uv)
{
	m_fTexSizeX = uv.x;
	m_fTexSizeZ = uv.y;
}
// �A�j���[�V�����ԍ�
void CDrawMesh::SetAnimFrame(int anim)
{
	m_nAnim = anim;
}
// �������̐ݒ�
void CDrawMesh::SetAnimSplit(int nx, int ny)
{
	m_nSplitX = nx;
	m_nSplitY = ny;
}


void CDrawMesh::SetOffset(XMFLOAT2 offset)
{
	m_Offset = offset;
}

//getter
// ���b�V�����̎擾
MESH* CDrawMesh::GetMesh()
{
	return &m_Mesh;
}

// �F
void CDrawMesh::SetColor(float x, float y, float z)
{
	m_Mesh.pMaterial->Diffuse = XMFLOAT4(x, y, z, 1.0f);
}



//===== �f�o�b�O���N���X
bool CSphere::m_bVisible = true;

// �R���X�g���N�^
CSphere::CSphere()
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;
}

// �f�X�g���N�^
CSphere::~CSphere()
{
	ReleaseMesh(&m_Mesh);
}

// ������
void CSphere::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();

	//	���̂̒��_�쐬
	InitSphere();
}

// �X�V
void CSphere::Update()
{
	if (INPUT->GetKeyTrigger(DIK_B))
		m_bVisible = !m_bVisible;
}

// �`��
void CSphere::Draw()
{
	if (!m_bVisible) return;
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//	���C�gOFF
	SetBlendState(BS_ADDITIVE);		//	���Z����
	SetZWrite(false);				//	�������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	
	// ���S���W���ړ�
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVector3TransformCoord(XMLoadFloat3(&m_pTransform->Pos), XMLoadFloat4x4(m_pWorld)));
	// �T�C�Y�𔽉f
	m_Mesh.mtxWorld._11 = m_Mesh.mtxWorld._22 = m_Mesh.mtxWorld._33 = m_fRadius;
	// �ʒu�𔽉f
	m_Mesh.mtxWorld._41 = pos.x + m_Offset.x;
	m_Mesh.mtxWorld._42 = pos.y + m_Offset.y;
	m_Mesh.mtxWorld._43 = pos.z;

	// �`��
	DrawMesh(pDC, &m_Mesh);
	
	SetZWrite(true);				//	Z�o�b�t�@�X�V�L��
	SetBlendState(BS_NONE);			//	�u�����h�X�e�[�g
	CLight::Get()->SetEnable();		//	���C�gON
}

// ���a�̐ݒ�
void CSphere::SetRadius(float rad)
{
	m_fRadius = rad;
}

// �g�����X�t�H�[���̐ݒ�
void CSphere::SetTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
}

// ���[���h���W�̐ݒ�
void CSphere::SetWorldPos(XMFLOAT4X4* pWorld)
{
	m_pWorld = pWorld;
}

 /**
 * @fn		CSphere::SetSphereVisible
 * @brief	���̒��_�쐬
 * @return	�Ȃ�
 * @detail	CSphere::Start�֐��ȊO�ł͎g��Ȃ�
 */
void CSphere::InitSphere()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �}�e���A���̏����ݒ�
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial = &m_Material;

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());


	// �v���~�e�B�u�̎w��
	m_Mesh.primitiveType = PT_LINE;

	// �I�u�W�F�N�g�̒��_�z��𐶐�
	m_Mesh.nNumVertex = 384 * 3;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	for (int axis = 0; axis < 3; ++axis) {
		for (int i = 0; i < 384; ++i, ++pVtx) {
			// ���_���W�̐ݒ�
			switch (axis) {
			case 0:
				pVtx->vtx.x = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = 0.0f;
				break;
			case 1:
				pVtx->vtx.y = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = 0.0f;
				break;
			case 2:
				pVtx->vtx.z = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = 0.0f;
				break;
			}

			// �@���̐ݒ�
			pVtx->nor = pVtx->vtx;

			// ���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex = XMFLOAT2(0.0f, 0.0f);
		}
	}

	// �C���f�b�N�X�z��𐶐�
	m_Mesh.nNumIndex = (384 * 2) * 3;
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	for (int axis = 0; axis < 3; ++axis)
	{
		int* pIdx = &pIndexWk[axis * (384 * 2)];
		for (int i = 0; i < 384; ++i)
		{
			*pIdx++ = axis * 384 + i;
			*pIdx++ = axis * 384 + (i + 1) % 384;
		}
	}

	// ���_�o�b�t�@����
	hr = MakeMeshVertex(pDevice, &m_Mesh, pVertexWk, pIndexWk);

	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("���_���쐬���s"), NULL, MB_OK);
	}

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;
}

 /**
 * @fn		CSphere::SetSphereVisible
 * @brief	���ׂĂ̋��̂̕\��ON/OFF
 * @param	(bool) ���̂�\�� true : ��\�� false
 * @return	�Ȃ�
 */
void CSphere::SetSphereVisible(bool visible)
{
	m_bVisible = visible;
}

/**
* @fn		CSphere::CreateSphere
* @brief	�f�o�b�O�p�̋��̂����X�g�ɒǉ�
* @param	(CTransform*) �ǔ�����I�u�W�F�N�g�̃g�����X�t�H�[��
* @param	(float) ���̂̔��a
* @param	(XMFLOAT4x4*) �ǔ�����I�u�W�F�N�g�̃��[���h���W
* @return	�Ȃ�
*/
void CSphere::CreateSphere(CTransform* pTransform, float radius, XMFLOAT4X4* world)
{
	//	�I�u�W�F�N�g����
	Object* pSphereObject = new Object("Sphere", UPDATE_DEBUG, DRAW_DEBUG);

	//	�R���|�[�l���g�̒ǉ�
	auto transform = pSphereObject->AddComponent<CTransform>();
	auto sphere = pSphereObject->AddComponent<CSphere>();

	//	�I�u�W�F�N�g���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pSphereObject);

	//	�I�u�W�F�N�g���̐ݒ�
	sphere->SetRadius(radius);
	sphere->SetTransform(pTransform);
	sphere->SetWorldPos(world);
}