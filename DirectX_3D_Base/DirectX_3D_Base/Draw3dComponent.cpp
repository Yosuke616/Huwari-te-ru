#include "Draw3dComponent.h"
#include "TransformComponent.h"
#include "Animation.h"

#include "InputManager.h"
#include "ModelManager.h"
#include "ObjectInfo.h"
#include "sceneGame.h"

// �R���X�g���N�^
CDrawModel::CDrawModel()
	: m_pTransform(nullptr), m_pAnim(nullptr), m_pMaterial(nullptr)
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;


	//mat.Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// �A���r�G���g+�e�N�X�`���L��
	//mat.Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// �f�B�t���[�Y
	//mat.Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// �X�y�L����+�X�y�L�������x
	//mat.Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// �G�~�b�V�u
	//mat.pTexture = nullptr;
	//mat.pTexEmmisive = nullptr;
	//mat.pTexSpecular = nullptr;
	//mat.pTexTransparent = nullptr;
}

// �f�X�g���N�^
CDrawModel::~CDrawModel()
{
	// ���[�u
	if (Parent->GetName() == ICE_BLOCK_NAME)
	{
		// ���[�u
		SceneGame::IceManager[this->GetModel()] = false;
	}

	if (m_pMaterial)
	{
		delete m_pMaterial;
		m_pMaterial = m_pMaterial;
	}

	if (m_pAnim)
	{
		delete m_pAnim;
		m_pAnim = nullptr;
	}
}

// �J�n
void CDrawModel::Start()
{
	// �I�u�W�F�N�g�̍��W���擾
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CDrawModel::Update()
{
	if (m_pAnim) m_pAnim->Update();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	//	�g�k�𔽉f
	mtxScale = XMMatrixScaling(
		m_pTransform->Scale.x,
		m_pTransform->Scale.y,
		m_pTransform->Scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	// X��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		0.0f,
		0.0f);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	// Y��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.y),
		0.0f);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	// Z��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.z));
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);



	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(
		m_pTransform->Pos.x,
		m_pTransform->Pos.y,
		m_pTransform->Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);
}

// �`��
void CDrawModel::Draw()
{
#ifdef _DEBUG
	if (m_pAnim)
	{
		m_pAnim->Draw();
	}
#endif // _DEBUG


	ID3D11DeviceContext* pDC = GetDeviceContext();	//	�f�o�C�X�R���e�L�X�g�̎擾

	// �s����������`��
	CLight::Get()->SetDisable();	//	���C�gOFF
	SetBlendState(BS_NONE);			//	�A���t�@�u�����h�L��
	SetZBuffer(true);				//	Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	m_model->Draw(pDC, m_mtxWorld, eOpacityOnly);
	
	// ������������`��
	SetBlendState(BS_ALPHABLEND);	//	�A���t�@�u�����h�L��
	SetZWrite(false);				//	Z�o�b�t�@�X�V���Ȃ�
	m_model->Draw(pDC, m_mtxWorld, eTransparentOnly);
	
	
	CLight::Get()->SetEnable();		//	���C�gOFF
	SetZWrite(true);				//	Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			//	�A���t�@�u�����h����
}



// ���f���̃v���~�e�B�u�ݒ�
void CDrawModel::SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type)
{
	if (m_model != nullptr)
		m_model->SetPrimitiveType(type);
}

// ���[���h�s��̎擾
XMFLOAT4X4* CDrawModel::GetWorldPos()
{
	if (m_model != nullptr)
	{
		return &(m_model->GetWorldMatrix());
	}
	return nullptr;
}

// �e�N�X�`���̐ݒ�
void CDrawModel::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	if (pTexture)
	{
		if (!m_pMaterial)
		{
			m_pMaterial = new TAssimpMaterial();
		}
		m_pMaterial->pTexture = pTexture;
		//m_pMaterial->pTexture->AddRef();		//	�Q�ƃJ�E���^�{�P
		m_model->SetMaterial(m_pMaterial);		// ���f���Ƀe�N�X�`�����Z�b�g
	}
}

// �傫���̎擾
XMFLOAT3 CDrawModel::GetBox()
{
	return m_model->GetBBox();
}




CAssimpModel* CDrawModel::GetModel()
{
	return m_model;
}


//----------------------�A�j���[�V�����֘A
//	���f�����̐ݒ�
void CDrawModel::SetModel(CAssimpModel* model)
{
	// ���f���̃Z�b�g
	m_model = model;

	// �A�j���[�V��������������ǉ�
	if (model->HasAnimation())
	{
		// �A�j���[�V�����N���X�쐬
		if (!m_pAnim)	m_pAnim = new Animation();

		// ���f�����𑗂�
		m_pAnim->SetModel(this);
	}

}

// �A�j���[�V�����ǉ�(id, �J�n���ԁA�I�����ԁA���[�v�̎n�܂莞��)
void CDrawModel::AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame)
{
	if (m_pAnim)
	{
		m_pAnim->AddAnimation(AnimID, Start, end, loop, loopStartFrame);
	}
}

// �A�j���[�V�����Đ�
void CDrawModel::Play(int AnimID)
{
	if (m_pAnim)
	{
		m_pAnim->Play(AnimID);
	}
}

// �A�j���[�V�����̃t���[�����ݒ�
void CDrawModel::SetAnimTime(double dTime)
{
	m_model->SetAnimTime(dTime);
}

double CDrawModel::GetAnimDuration()
{
	// �H
	return 0;
}

void CDrawModel::AlwaysAnimation(bool always)
{
	if (m_pAnim)
	{
		m_pAnim->AlwaysAnimation(always);
	}
}

// �Đ����x
void CDrawModel::SetPlaySpeed(int AnimID, double speed)
{
	if (m_pAnim)
	{
		m_pAnim->SetPlaySpeed(AnimID, speed);
	}
}

bool CDrawModel::AnimEnd(int AnimID)
{
	if (m_pAnim)
	{
		return m_pAnim->AnimEnd(AnimID);
	}
	else return false;
}
