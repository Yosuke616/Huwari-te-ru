#include "Scale2DCom.h"
#include "Draw2dComponent.h"


#define TIME	(100)

// �R���X�g���N�^
CScale2D::CScale2D()
	: m_pDraw2D(nullptr)
	, m_vDestSize(0.0f, 0.0f)
	, m_vSize(0.0f, 0.0f)
{
}

// �f�X�g���N�^
CScale2D::~CScale2D() {
}

// ������
void CScale2D::Start()
{
	// �I�u�W�F�N�g�̍��W���擾
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// �X�V
void CScale2D::Update()
{
	if (!m_pDraw2D) return;

	// �傫�����Ă���
	m_vSize.x = m_vDestSize.x * (m_nTime / (float)TIME);
	m_vSize.y = m_vDestSize.y * (m_nTime / (float)TIME);

	// �傫�����f
	m_pDraw2D->SetSize(m_vSize.x, m_vSize.y);

	// �ړI�̍��W�ɒ�������
	if (m_nTime >= TIME)
	{
		// ���̃R���|�[�l���g���폜����
		this->Delete();
		return;
	}

	m_nTime++;
}

// �`��
void CScale2D::Draw() {
}


// �ړ��ړI�̍��W�̐ݒ�
void CScale2D::SetDestSize(XMFLOAT2 size)
{
	m_vDestSize = size;
}

