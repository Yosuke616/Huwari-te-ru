#include "FallComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "PlayerComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"

#define R_AND_L_MOVE    (15.0f)

// �R���X�g���N�^
CFall::CFall()
	: m_pTransform(nullptr)
	, m_bActive(true)
	, m_fAngle(0.0f)
{
	//�������͂��߂̍��W��ۑ�����ϐ�
	m_FallPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
CFall::~CFall()
{
}

// ������
void CFall::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CFall::Update()
{
	//�@�㏸���Ă��邩
	//�@�㏸���Ă�����t���O��true�ɂ���
	if (m_pTransform->Vel.y > (BIGLEAFGRAVITY + m_fResistance)) {
		//�㏸�����i�K�Ŋ�_���Ȃ���
		m_FallPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fDegree = 0;
		m_bUpFlg = true;
	}

	//	y���̈ړ��ʂ̊ɋ}
	m_fResistance = SinDeg(m_fDegree * 2) * BIGLEAFGRAVITY;
	m_pTransform->Vel.y -= (BIGLEAFGRAVITY + m_fResistance);
	//	�p�x�̉��Z
	m_fDegree += 2;
	if (m_fDegree > 180) m_fDegree -= 360;

	//	x���̉����x
	if (!m_bActive) return;	//	�n�ʂɂ��鎞�╗�ɗ�����Ă���Ƃ��͍X�V�Ȃ�

	//x���̉����x�̉��ɒǉ�
	//2022/1/3 Shimizu Yosuke --------------------------------------------------------------
	//�����n�߂���h�炷��ɂ���
	if (m_pTransform->Vel.y < 0 && m_bUpFlg == true) {
		//�����n�߂̎��u�U�v�ɕς��ė����n�߂��Ƃ��ɂ��������ɓ���Ȃ��悤�ɂ���
		m_bUpFlg = false;

		//�����n�߂̍��W��ۑ�����
		m_FallPos = m_pTransform->Pos;
		//�����n�߂��u�Ԃ�0�ɂ��Ė��񗎂��n�߂��Ƃ��ɓ��������ɗ�����悤�ɂ���
		m_fDegree = 0;
		m_bRorL = false;
	}
	//-------------------------------------------------------------------------------------

	//	�����Ă���Ƃ�
	//�����Ă���Ƃ��̉��ɒǉ�
	//2022/1/3 Shimizu Yosuke ----------------------------------------------------
	//	�ۑ��������W���N�_�ɂ���炳������
	//�N�_���i�񂾂甽�Ε����ɐ؂�Ԃ��悤�ɂ���
	//��񂾂��Ԃ��悤�ɂ���
	//�E�ɐi�񂾃p�^�[��
	if (m_pTransform->Pos.x > m_FallPos.x + R_AND_L_MOVE && m_bRorL == false) {
		m_fDegree = -180.0f;
		m_bRorL = true;
	}
	//���ɐi�񂾃p�^�[��
	if (m_pTransform->Pos.x < m_FallPos.x - R_AND_L_MOVE && m_bRorL == true) {
		m_fDegree = 0;
		m_bRorL = false;
	}
	m_pTransform->Vel.x += SinDeg(m_fDegree) * VIBRATION;	//	���E�ɗh���

	//----------------------------------------------------------------------------

	//	�����Ă��A�j���[�V����
	auto animation = Parent->GetComponent<CPlayer>();
	if(animation)
		animation->SetAnimState(PLAYER_FALL);


	//	�n�ʃt���O
	auto Player = Parent->GetComponent<CPlayer>();
	if (Player) Player->SetGround(false);

	//�Q�[���I�[�o�[�����̒�~
	if (Parent->GetName() == GOLETTER_NAME)
	{
		CCollider* letterstop = Parent->GetComponent<CCollider>();
		auto stoppos = letterstop->GetCenterPos();		// �����̒��S���W
		if (stoppos.y < -250.0f)
		{
			m_pTransform->Vel.x = 0.0f;
			m_pTransform->Vel.y = 0.0f;
		}
	}
}


void CFall::SetActive(bool bActive)
{
	m_bActive = bActive;
}