//�@�C���N���[�h��
#include "GaugeComponent.h"
#include "WindManager.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"



//  �R���X�g���N�^
CGauge::CGauge()
	: m_pTransform(nullptr), m_pDraw(nullptr)
	, m_nMaxGauge(0), m_nCurrentGauge(0)
{
}

//  �f�X�g���N�^
CGauge::~CGauge() {
}

//�@������
void CGauge::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw2D>();
}

//�@�X�V����
void CGauge::Update()
{
	if (m_pTransform)
	{
		//	x���W				= �x�[�X�ʒu			+ (���� / 2)			   * HP����			- (���� / 2)
		m_pTransform->Pos.x = m_InitPosition.x  
			+ (m_MaxSize.x * 0.5f) * (m_nCurrentGauge / (float)m_nMaxGauge) - (m_MaxSize.x * 0.5f);
	}

	//	�`��̍X�V
	if (m_pDraw)
	{
		m_pDraw->SetSize(m_MaxSize.x * (m_nCurrentGauge / (float)m_nMaxGauge), m_MaxSize.y);
		m_pDraw->SetU(m_nCurrentGauge / (float)m_nMaxGauge * 1.0f / m_pDraw->GetAnimSplitX());
	}
}

//�@�����|�W�V�������Z�b�g����
void CGauge::SetInitPos(float fx,float fy)
{
	m_InitPosition.x = fx;
	m_InitPosition.y = fy;
}

//�@�傫�����Z�b�g����
void CGauge::SetSize(float fx, float fy)
{
	m_MaxSize.x = fx;
	m_MaxSize.y = fy;
}

//�@�Q�[�W�̍ő�l���Z�b�g����
void CGauge::SetMaxGauge(int max)
{
	m_nMaxGauge = m_nCurrentGauge  = max;
}

//	�Q�[�W�����炷�֐�
void CGauge::CalcGauge(CGauge* pGauge,int value)
{
	if (!pGauge) return;
	pGauge->m_nCurrentGauge += value;

	if (pGauge->m_nCurrentGauge < 0)					pGauge->m_nCurrentGauge = 0;
	if (pGauge->m_nCurrentGauge > pGauge->m_nMaxGauge)	pGauge->m_nCurrentGauge = pGauge->m_nMaxGauge;
}

// �Q�[�W�ʂ�ݒ肷��
void CGauge::SetGauge(CGauge* pGauge, int value)
{
	if(pGauge) pGauge->m_nCurrentGauge = value;
}

int CGauge::GetCurrentGauge(CGauge* pGauge)
{
	if (!pGauge) return 0;
	return pGauge->m_nCurrentGauge;
}
