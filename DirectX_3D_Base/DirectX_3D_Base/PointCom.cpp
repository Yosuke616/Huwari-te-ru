#include "PointCom.h"
#include "TransformComponent.h"

#define VELOCITY		(1.0f)


//	������(���m�ł͂Ȃ�)
float squareRoot2(float x);
//	2��֐�
float pow2(float x);

// �R���X�g���N�^
CPoint::CPoint()
	: m_pTransform(nullptr)
	, m_vDestPos(0.0f, 0.0f)
{
}

// �f�X�g���N�^
CPoint::~CPoint(){
}

// ������
void CPoint::Start()
{
	// �I�u�W�F�N�g�̍��W���擾
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CPoint::Update()
{
	if (!m_pTransform) return;

	// ���ݍ��W�ƖړI���W�̍����v�Z
	XMFLOAT2 diff;
	diff.x = m_pTransform->Pos.x - m_vDestPos.x;
	diff.y = m_pTransform->Pos.y - m_vDestPos.y;


	float fDistance = squareRoot2(pow2(diff.x) + pow2(diff.y));

	// �ړI�̍��W�ɒ�������
	if (fDistance < 1)
	{
		m_pTransform->Vel.x = 0.0f;
		m_pTransform->Vel.y = 0.0f;

		// ���̃R���|�[�l���g���폜����
		this->Delete();
		return;
	}


	//--- �ړ������̌v�Z
	XMVECTOR xmvDiff = XMLoadFloat2(&diff);	//	XMFLOAT2��XMVECTOR�^�ɂ�ϊ�
	xmvDiff = XMVector2Normalize(xmvDiff);		//	�x�N�g���̐��K��
	XMFLOAT2 vDiff;									//	���K�����ꂽXMVECTOR�^��XMFLOAT2�^�ɓ���锠�̗p��
	DirectX::XMStoreFloat2(&vDiff, xmvDiff);	//	XMVECTOR��XMFLOAT2�ɕϊ�
	//	���W�A���Ŋp�x���o���Ă����
	float fRad = atan2f(vDiff.y, vDiff.x);			//	���W�A��	(�΂̕�)

	m_pTransform->Vel.x = -VELOCITY * cosf(fRad);
	m_pTransform->Vel.y = -VELOCITY * sinf(fRad);
}

// �`��
void CPoint::Draw(){
}


// �ړ��ړI�̍��W�̐ݒ�
void CPoint::SetDestPos(XMFLOAT2 pos)
{
	m_vDestPos = pos;
}

//	������(���m�ł͂Ȃ�)
float squareRoot2(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2��֐�
float pow2(float x)
{
	return x * x;
}
