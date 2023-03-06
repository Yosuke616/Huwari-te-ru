//===== �C���N���[�h =====
#include "Draw2dComponent.h"
#include "TransformComponent.h"
#include "polygon.h"
#include "Light.h"
#include "ObjectManager.h"

// �R���X�g���N�^
CDraw2D::CDraw2D()
	:m_pTexture(0), m_fAngle(0.0f)
	, m_nAnimNum(0) , m_nAnimSplitX(1), m_nAnimSplitY(1)
	, m_BlendState(BS_ALPHABLEND)

{
	m_fPolygonSize = XMFLOAT2(0.0f, 0.0f);
	m_fTexFrameSize = XMFLOAT2(1.0f, 1.0f);
	m_fVertexColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Scroll = XMFLOAT2(0.0f, 0.0f);
	m_eUpdateOrder = COM_UPDATE_DRAW;
}

// �f�X�g���N�^
CDraw2D::~CDraw2D()
{
}

// �J�n
void CDraw2D::Start(void)
{
	m_pTransform = Parent->GetComponent < CTransform>();
}

// �X�V
void CDraw2D::Update(void)
{
}

//	�Z�b�g�֐� �e�N�X�`��
void CDraw2D::SetTexture(ID3D11ShaderResourceView* pTex)
{
	m_pTexture = pTex;
}

//	�Z�b�g�֐� �A�j���[�V�����ԍ�
void CDraw2D::SetAnimNum(int anim)
{
	m_nAnimNum = anim;
}



//	�Z�b�g�֐� UV�T�C�Y
void CDraw2D::SetUVsize(float UVSizeX, float UVSizeY)
{
	m_fTexFrameSize.x = UVSizeX;
	m_fTexFrameSize.y = UVSizeY;
}

//	�Z�b�g�֐� UV��U
void CDraw2D::SetU(float u)
{
	m_fTexFrameSize.x = u;
}

//	�Z�b�g�֐� UV��V
void CDraw2D::SetV(float v)
{
	m_fTexFrameSize.y = v;
}

//	�Z�b�g�֐� �|���S���T�C�Y
void CDraw2D::SetSize(float SizeX, float SizeY)
{
	m_fPolygonSize.x = SizeX;
	m_fPolygonSize.y = SizeY;
}

//	�Z�b�g�֐� �J���[
void CDraw2D::SetColor(float red, float green, float blue)
{
	m_fVertexColor.x = red;
	m_fVertexColor.y = green;
	m_fVertexColor.z = blue;
}

//	�Z�b�g�֐� �A���t�@�l
void CDraw2D::SetAlpha(float alpha)
{
	m_fVertexColor.w = alpha;
}

//	�Z�b�g�֐�	 �p�x
void CDraw2D::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}


//	�Z�b�g�֐� �������Z�b�g
void CDraw2D::SetAnimSplit(int x, int y)
{
	m_nAnimSplitX = x;
	m_nAnimSplitY = y;
	m_fTexFrameSize.x = 1.0f / x;
	m_fTexFrameSize.y = 1.0f / y;
}

//	�Z�b�g�֐� �X�N���[����
void CDraw2D::SetScroll(XMFLOAT2 scroll)
{
	m_Scroll.x = scroll.x;
	m_Scroll.y = scroll.y;
}

//	�Z�b�g�֐� �u�����h�X�e�[�g
void CDraw2D::SetBlend(EBlendState state)
{
	m_BlendState = state;
}

// �����Ɗ֐�
int CDraw2D::GetAnimSplitX()
{
	return m_nAnimSplitX;
}

// �`��
void CDraw2D::Draw(void)
{
	//	�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);						//	Z�o�b�t�@���X�V���Ȃ�
	//SetZWrite(false);
	//CLight::Get()->SetEnable();
	SetBlendState(m_BlendState);			//	�A���t�@�u�����h�L��

	SetPolygonTexture(m_pTexture);								//	�e�N�X�`��
	SetPolygonPos(m_pTransform->Pos.x, m_pTransform->Pos.y);	//	���W
	SetPolygonSize(m_fPolygonSize.x, m_fPolygonSize.y);			//	�T�C�Y
	SetPolygonFrameSize(m_fTexFrameSize.x, m_fTexFrameSize.y);	//	�e�N�X�`�����蓖��
	SetPolygonUV(
		(m_nAnimNum % m_nAnimSplitX) / (float)m_nAnimSplitX + m_Scroll.x,
		(m_nAnimNum / m_nAnimSplitX) / (float)m_nAnimSplitY + m_Scroll.y);	//	�A�j���[�V�����ԍ�
	SetPolygonColor(m_fVertexColor.x,
		m_fVertexColor.y, m_fVertexColor.z);					//	�F
	SetPolygonAlpha(m_fVertexColor.w);							//	�s�����x
	SetPolygonAngle(m_fAngle);									//	�p�x
	
	//	�`��
	DrawPolygon(pDC);

	//	�ݒ�����ɖ߂�
	SetPolygonColor(1, 1, 1);
	SetPolygonAngle(0.0f);
	SetPolygonAlpha(1.0f);							//	�s�����x
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}




//effect
CDrawEffect2D::CDrawEffect2D()
	: m_nAnimCount(1)
	, m_nFrame(10)
	, m_bLoop(false)
	, m_bPlay(true)
{
}

CDrawEffect2D::~CDrawEffect2D(){
}

//	�X�V
void CDrawEffect2D::Update()
{
	//	�J�E���^�X�V
	if (m_bPlay == true)
	{
		//	�J�E���^�X�V
		--m_nAnimCount;
	}

	if (m_nAnimCount <= 0)
	{
		//	�A�j���[�V�����؂�ւ�
		++m_nAnimNum;

		//	�Ō�̃A�j���[�V�����̏ꍇ
		if (m_nAnimNum >= m_nAnimSplitX * m_nAnimSplitY)
		{
			if (m_bLoop)
			{
				//	�A�j���[�V�������Z�b�g
				m_nAnimNum = 0;
			}
			else
			{
				//	�I�u�W�F�N�g�폜
				Parent->Delete();
			}
		}
		m_nAnimCount = m_nFrame;
	}
}

//setter
void CDrawEffect2D::SetSwapFrame(int nCount)
{
	m_nAnimCount = m_nFrame = nCount;
}
void CDrawEffect2D::SetLoop(bool loop)
{
	m_bLoop = loop;
}
void CDrawEffect2D::SetPlayONOFF(bool onoff)
{
	m_bPlay = onoff;
}

/**
* @fn		CDrawEffect::StartEffect
* @brief	�G�t�F�N�g�`��J�n
* @param[in]	(ID3D11ShaderResourceView*)	pTex �e�N�X�`��
* @param[in]	(XMFLOAT3)	pos					 �I�u�W�F�N�g�̍��W
* @param[in]	(XMFLOAT2)	size				 �I�u�W�F�N�g�̕`��T�C�Y
* @param[in]	(int)		split_x				 �e�N�X�`���̕�����X
* @param[in]	(int)		split_y				 �e�N�X�`���̕�����Y
* @param[in]	(int)		swap_frame			 �e�N�X�`���؂�ւ��t���[����
* @param[in]	(XMFLOAT3)	color				 �F
* @detail
*			�F�̃f�t�H���g�ݒ��(1.0f,1.0f,1.0f)
*			�e�N�X�`���؂�ւ��t���[�����̃f�t�H���g�ݒ��1�t���[��
*/
void CDrawEffect2D::StartEffect(ID3D11ShaderResourceView* pTex,
	XMFLOAT2 pos, XMFLOAT2 size, int split_x, int split_y, int swap_frame, XMFLOAT3 color)
{
	// generate
	Object* pEffect = new Object("effect", UPDATE_EFFECT, DRAW_UI);
	// components
	auto trans = pEffect->AddComponent<CTransform>();	// �I�u�W�F�N�g���W
	auto draw = pEffect->AddComponent<CDrawEffect2D>();	// �G�t�F�N�g�`��
	// settiongs
	trans->Pos.x = pos.x;								// X���W
	trans->Pos.y = pos.y;								// Y���W
	draw->SetSize(size.x, size.y);						// �`��T�C�Y
	draw->SetColor(color.x, color.y, color.z);			// �F
	draw->SetLoop(false);								// ���[�v�Ȃ��ɐݒ�
	draw->SetTexture(pTex);								// �e�N�X�`��
	draw->SetAnimSplit(split_x, split_y);				// �e�N�X�`���̕�����
	draw->SetSwapFrame(swap_frame);						// �e�N�X�`���؂�ւ��t���[����
	// add to list
	ObjectManager::GetInstance()->AddObject(pEffect);
}