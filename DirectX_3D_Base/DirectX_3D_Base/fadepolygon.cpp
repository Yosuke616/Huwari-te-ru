/**
 * @file	fadepolygon.cpp
 * @brief	�t�F�[�h�C��/�A�E�g�p�|���S������
 * @author	�u���Č�
 * @date	2021/12/23
 */

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "fadepolygon.h"
#include "Shader.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �V�F�[�_�ɓn���l
struct SHADER_GLOBAL {
	XMMATRIX mWorld;	// ���[���h�ϊ��s��(�]�u�s��)
	XMMATRIX mView;		// �r���[�ϊ��s��(�]�u�s��)
	XMMATRIX mProj;		// �ˉe�ϊ��s��(�]�u�s��)
	XMMATRIX mTex;		// �e�N�X�`���ϊ��s��(�]�u�s��)
	XMVECTOR vWipe;		// ���C�v����
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexFadePolygon(ID3D11Device* pDevice);
static void SetVertexFadePolygon(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture; // �e�N�X�`���ւ̃|�C���^

static VERTEX_2D					g_vertexWk[NUM_VERTEX]; // ���_���i�[���[�N

static XMFLOAT3						g_posFadePolygon; // �|���S���̈ړ���
static XMFLOAT3						g_rotFadePolygon; // �|���S���̉�]��
static XMFLOAT3						g_sizFadePolygon; // �|���S���̃T�C�Y
static XMFLOAT4						g_colFadePolygon; // �|���S���̒��_�J���[
static bool							g_bInvalidate; // ���_�f�[�^�X�V�t���O

static XMFLOAT2						g_posTexFrame; // UV���W
static XMFLOAT2						g_sizTexFrame; // �e�N�X�`�����o�T�C�Y

static ID3D11Buffer*				g_pConstantBuffer; // �萔�o�b�t�@
static ID3D11Buffer*				g_pVertexBuffer; // ���_�o�b�t�@
static ID3D11SamplerState*			g_pSamplerState; // �e�N�X�`�� �T���v��
static ID3D11VertexShader*			g_pVertexShader; // ���_�V�F�[�_
static ID3D11InputLayout*			g_pInputLayout; // ���_�t�H�[�}�b�g
static ID3D11PixelShader*			g_pPixelShader; // �s�N�Z���V�F�[�_

static XMFLOAT4X4					g_mProj;	// �ˉe�ϊ��s��
static XMFLOAT4X4					g_mView;	// �r���[�ϊ��s��
static XMFLOAT4X4					g_mWorld;	// ���[���h�ϊ��s��
static XMFLOAT4X4					g_mTex;		// �e�N�X�`���ϊ��s��
static XMFLOAT2						g_fWipe;	// ���C�v

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFadePolygon(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;
	
	// �V�F�[�_������
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// �V�F�[�_�ǂݍ���
	hr = LoadShader(L"VertexFade", L"PixelFade", // �ǂݍ��ޒ��_�V�F�[�_�A�s�N�Z���V�F�[�_
		&g_pVertexShader,	// ���_�V�F�[�_�̏��������
		&g_pInputLayout,	// ���C�A�E�g---------------------------
		&g_pPixelShader,	// �s�N�Z���V�F�[�_�̏��������
		layout, _countof(layout));
	// �ǂݍ��ݎ��s
	if (FAILED(hr)) return hr;
	
	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;			// �g����
	bd.ByteWidth = sizeof(SHADER_GLOBAL);	// �S�Ă̍s��̃T�C�Y
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ���\�[�X���p�C�v���C���Ƀo�C���h����
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer); // g_pConstantBuffer�Ƀo�b�t�@�����i�[����
	if (FAILED(hr)) return hr;
	
	// �e�N�X�`�� �T���v������
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &g_pSamplerState);		// g_pConstantBuffer�Ƀe�N�X�`�������i�[����
	if (FAILED(hr)) {
		return hr;
	}
	
	// �ϊ��s�񏉊���
	XMStoreFloat4x4(&g_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&g_mProj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&g_mTex, XMMatrixIdentity());
	g_mTex._44 = 0.0f;
	g_fWipe = XMFLOAT2(0.0f, 0.0f);						// ���C�v������
	
	g_posFadePolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotFadePolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizFadePolygon = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_colFadePolygon = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	
	g_bInvalidate = false;
	g_posTexFrame = XMFLOAT2(0.0f, 0.0f);
	g_sizTexFrame = XMFLOAT2(1.0f, 1.0f);
	
	// ���_���̍쐬
	hr = MakeVertexFadePolygon(pDevice);
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFadePolygon(void)
{
	// �e�N�X�`���@�T���v���̊J��
	SAFE_RELEASE(g_pSamplerState);
	// ���_�o�b�t�@�̉��
	SAFE_RELEASE(g_pVertexBuffer);
	// �萔�o�b�t�@�̉��
	SAFE_RELEASE(g_pConstantBuffer);
	// �s�N�Z���V�F�[�_���
	SAFE_RELEASE(g_pPixelShader);
	// ���_�t�H�[�}�b�g���
	SAFE_RELEASE(g_pInputLayout);
	// ���_�V�F�[�_���
	SAFE_RELEASE(g_pVertexShader);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFadePolygon(float fSpeed)
{
	// (�������Ȃ�)
	g_fWipe.x = fSpeed;			// ���C�v�X�V
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFadePolygon(ID3D11DeviceContext* pDeviceContext)
{
	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(g_sizFadePolygon.x, g_sizFadePolygon.y, g_sizFadePolygon.z);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotFadePolygon.x),
		XMConvertToRadians(g_rotFadePolygon.y), XMConvertToRadians(g_rotFadePolygon.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(g_posFadePolygon.x, g_posFadePolygon.y, g_posFadePolygon.z);
	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&g_mWorld, mWorld);
	
	if (g_pTexture) {
		// �g�k
		mWorld = XMMatrixScaling(g_sizTexFrame.x, g_sizTexFrame.y, 1.0f);
		// �ړ�
		mWorld *= XMMatrixTranslation(g_posTexFrame.x, g_posTexFrame.y, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&g_mTex, mWorld);
	}else {
		// �e�N�X�`������
		g_mTex._44 = 0.0f;
	}
	
	// ���_�o�b�t�@�X�V
	SetVertexFadePolygon();
	
	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(g_pInputLayout);
	
	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	
	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &g_pTexture);
	
	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&g_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&g_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
	cb.vWipe = XMLoadFloat2(&g_fWipe);			// ���C�v�̒l���V�F�[�_���ɑ���
	pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	
	// �v���~�e�B�u�`����Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	// �|���S���̕`��
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFadePolygon(ID3D11Device* pDevice)
{
	// ���_���W�̐ݒ�
	g_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	g_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	g_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	g_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	
	// �g�U���ˌ��̐ݒ�
	g_vertexWk[0].diffuse = g_colFadePolygon;
	g_vertexWk[1].diffuse = g_colFadePolygon;
	g_vertexWk[2].diffuse = g_colFadePolygon;
	g_vertexWk[3].diffuse = g_colFadePolygon;
	
	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	g_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	g_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	g_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(g_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &g_vertexWk[0];
	
	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &g_pVertexBuffer);
	
	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexFadePolygon(void)
{
	if (g_bInvalidate) {
		// ���_�o�b�t�@�̒��g�𖄂߂�
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
		HRESULT hr = S_OK;
		
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// �g�U���ˌ��̐ݒ�
			g_vertexWk[0].diffuse = g_colFadePolygon;
			g_vertexWk[1].diffuse = g_colFadePolygon;
			g_vertexWk[2].diffuse = g_colFadePolygon;
			g_vertexWk[3].diffuse = g_colFadePolygon;
			// ���_�f�[�^���㏑��
			memcpy_s(msr.pData, sizeof(g_vertexWk), g_vertexWk, sizeof(g_vertexWk));
			// ���_�f�[�^���A�����b�N����
			pDeviceContext->Unmap(g_pVertexBuffer, 0);
			// �t���O���N���A
			g_bInvalidate = false;
		}
	}
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void SetFadePolygonTexture(ID3D11ShaderResourceView* pTexture)
{
	g_pTexture = pTexture;
	g_mTex._44 = (g_pTexture) ? 1.0f : 0.0f;
}

//=============================================================================
// �\�����W�̐ݒ�
//=============================================================================
void SetFadePolygonPos(float fX, float fY)
{
	g_posFadePolygon.x = fX;
	g_posFadePolygon.y = fY;
}

//=============================================================================
// �\���T�C�Y�̐ݒ�
//=============================================================================
void SetFadePolygonSize(float fScaleX, float fScaleY)
{
	g_sizFadePolygon.x = fScaleX;
	g_sizFadePolygon.y = fScaleY;
}

//=============================================================================
// �\���p�x�̐ݒ�(�P��:�x)
//=============================================================================
void SetFadePolygonAngle(float fAngle)
{
	g_rotFadePolygon.z = fAngle;
}

//=============================================================================
// ����e�N�X�`�����W�̐ݒ� (0.0��fU��1.0, 0.0��fV��1.0)
//=============================================================================
void SetFadePolygonUV(float fU, float fV)
{
	g_posTexFrame.x = fU;
	g_posTexFrame.y = fV;
}

//=============================================================================
// �e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
//=============================================================================
void SetFadePolygonFrameSize(float fWidth, float fHeight)
{
	g_sizTexFrame.x = fWidth;
	g_sizTexFrame.y = fHeight;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetFadePolygonColor(float fRed, float fGreen, float fBlue)
{
	if (fRed != g_colFadePolygon.x || fGreen != g_colFadePolygon.y || fBlue != g_colFadePolygon.z){
		g_colFadePolygon.x = fRed;
		g_colFadePolygon.y = fGreen;
		g_colFadePolygon.z = fBlue;
		g_bInvalidate = true;
	}
}

//=============================================================================
// �s�����x�̐ݒ�
//=============================================================================
void SetFadePolygonAlpha(float fAlpha)
{
	if (fAlpha != g_colFadePolygon.w) {
		g_colFadePolygon.w = fAlpha;
		g_bInvalidate = true;
	}
}