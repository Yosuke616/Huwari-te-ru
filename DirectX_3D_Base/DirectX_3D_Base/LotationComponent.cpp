#include "LotationComponent.h"
#include "Draw2dComponent.h"

// �R���X�g���N�^
CLotation::CLotation()
	: m_pDraw2D(nullptr)
	, m_nDegree(0)
{

}

// �f�X�g���N�^
CLotation::~CLotation()
{

}

// ������
void CLotation::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// �X�V
void CLotation::Update()
{
	// �p�x�̐ݒ�
	if (m_pDraw2D) m_pDraw2D->SetAngle((float)m_nDegree);

	// �p�x�̍X�V
	m_nDegree -= 3;
	if (m_nDegree < 0) m_nDegree += 360;
}