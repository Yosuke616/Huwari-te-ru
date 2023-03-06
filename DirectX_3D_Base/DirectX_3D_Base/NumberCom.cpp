#include "NumberCom.h"
#include "polygon.h"
#include "TransformComponent.h"

CDrawNumber::CDrawNumber()
	: m_nSpace(30)
	, m_pNum(nullptr)
	, m_nNum(0)
{
	m_nAnimSplitY = 1;
	m_nAnimSplitX = 10;
	m_fTexFrameSize.x = 1.0f / m_nAnimSplitX;
	m_fTexFrameSize.y = 1.0f / m_nAnimSplitY;
}

CDrawNumber::~CDrawNumber()
{
}

void CDrawNumber::Update()
{
	m_nNumbers.clear();
	int num;
	int tmp = 0;

	// �|�C���^�̏ꍇ
	if(m_pNum)	tmp = *m_pNum;
	// �|�C���^�ł͂Ȃ��ꍇ
	else tmp = m_nNum;

	for (; tmp != 0;)
	{
		num = tmp % 10;
		m_nNumbers.push_back(num);

		tmp /= 10;
	}


}
void CDrawNumber::Draw()
{
	//	�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* pDC = GetDeviceContext();
	int i = 0;
	for (auto&& number : m_nNumbers)
	{
		SetZBuffer(false);						//	Z�o�b�t�@���X�V���Ȃ�
		SetBlendState(m_BlendState);			//	�A���t�@�u�����h�L��

		SetPolygonTexture(m_pTexture);								//	�e�N�X�`��
		SetPolygonPos(m_pTransform->Pos.x - i * m_nSpace, m_pTransform->Pos.y);	//	���W
		SetPolygonSize(m_fPolygonSize.x, m_fPolygonSize.y);			//	�T�C�Y
		SetPolygonFrameSize(m_fTexFrameSize.x, m_fTexFrameSize.y);	//	�e�N�X�`�����蓖��
		SetPolygonUV(
			(number % m_nAnimSplitX) / (float)m_nAnimSplitX,
			(number / m_nAnimSplitX) / (float)m_nAnimSplitY);	//	�A�j���[�V�����ԍ�
		SetPolygonColor(m_fVertexColor.x,
			m_fVertexColor.y, m_fVertexColor.z);					//	�F
		SetPolygonAlpha(m_fVertexColor.w);							//	�s�����x
		SetPolygonAngle(m_fAngle);									//	�p�x

		//	�`��
		DrawPolygon(pDC);

		i++;
	}

	//	�ݒ�����ɖ߂�
	SetPolygonColor(1, 1, 1);
	SetPolygonAngle(0.0f);
	SetPolygonAlpha(1.0f);							//	�s�����x
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}


void CDrawNumber::SetNum(int* num)
{
	m_pNum = num;
}

void CDrawNumber::SetNum(int num)
{
	m_nNum = num;
}