#include "IcicleComponent.h"
#include "GravityComponent.h"
#include "Draw3dComponent.h"
#include "sceneGame.h"
#include "Sound.h"
#include "ObjectInfo.h"
#include "DeleteTimerCom.h"
#include "CloverComponent.h"
#include "InformationComponent.h"

// �R���X�g���N�^
CIcicle::CIcicle()
	: m_pTransform(nullptr)
	, m_nIcicleHP(50)
	, m_bFall(false)
	, m_bMove(true)
{
	m_fLimitDown = -(CalcWorldSize().y / 2);
}

// �f�X�g���N�^
CIcicle::~CIcicle()
{
	
}

// ������
void CIcicle::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CIcicle::Update()
{
	// ��x�����d�͋@�\��ǉ�����
	if (!m_bFall && m_nIcicleHP <= 0)
	{
		// �I�u�W�F�N�g�ɋ@�\��ǉ�
		auto icicles = Parent->AddComponent<CGravity>();
		icicles->Start();

		// ������t���OON
		m_bFall = true;

		//se
		CSound::Play(SE_ICICLE_FALL);	// �������ǉ�
	}

	// ���̉������u���b�N�ƐڐG�������͎~�߂�
	if (!m_bMove)
	{
		// ���x�𖳂���
		m_pTransform->Vel.y = 0.0f;

		//�d�̓R���|�[�l���g�̋@�\��~
		auto G = Parent->GetComponent<CGravity>();
		if (G && G->m_bUpdateFlag == true)
		{
			G->Delete();// = false;
			CSound::Stop(SE_ICICLE_FALL);	// �������ǉ�
		}
	}

	//��ʉ��ɍs���Ȃ��悤�ɂ�����
	if (m_pTransform->Pos.y - ICICLE_SIZE_Y / 2 < m_fLimitDown)
	{
		CIcicle::Stop();

		//���W���Œ肳����
		m_pTransform->Pos.y = m_fLimitDown + ICICLE_SIZE_Y / 2;
		m_pTransform->Vel.y = 0.0f;
		//�d�̓R���|�[�l���g�̋@�\��~
		auto G = Parent->GetComponent<CGravity>();
		if (G)
		{
			G->Delete();// m_bUpdateFlag = false;
			CSound::Stop(SE_ICICLE_FALL);	// �������ǉ�
		}
	}
}

// �`��
void CIcicle::Draw(){
}

// �Փˋ@�\
void CIcicle::OnCollisionEnter(Object* pObject)
{
	//�u���b�N�ɓ���������~�܂�悤�ɂ��鏈��
	if (pObject->GetName() == BLOCK_NAME)
	{
		m_pTransform->Vel.y = 0.0f;
	}

}

// ���̑ϋv�l����
void CIcicle::IcicleDamage()
{
	m_nIcicleHP--;
	if (m_nIcicleHP < 0)
	{
		m_nIcicleHP = 0;
	}
}

// ����
void CIcicle::Stop()
{
	m_bMove = false;
}

bool CIcicle::IsFalling()
{
	return m_bFall;
}

bool CIcicle::IsMoving()
{
	return m_bMove;
}