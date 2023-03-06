#include "WindComponent.h"
#include "TransformComponent.h"
#include "ObjectInfo.h"
#include "FallComponent.h"
#include "AnimationComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "ObjectInfo.h"
#include "IcicleComponent.h"
#include "Draw3dComponent.h"

// �R���X�g���N�^
CWind::CWind()
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}
// �f�X�g���N�^
CWind::~CWind()
{
}
// ������
void CWind::Start()
{
}
// �X�V
void CWind::Update()
{
}
// �`��
void CWind::Draw()
{
}
// �Փˏ���
void CWind::OnCollisionEnter(Object* pObject)
{
	//	�v���C���[�������ꍇ
	if (pObject->GetName() == PLAYER_NAME)
	{
		//	�v���C���[�ɑ��x��������
		auto Player = pObject->GetComponent<CTransform>();
		Player->Vel.x += WIND_POWER_X * m_Vector.x;
		Player->Vel.y += WIND_POWER_Y * m_Vector.y;

		//	���E�ɗh�炳�Ȃ�
		auto Leaf = pObject->GetComponent<CFall>();
		if (Leaf) Leaf->SetActive(false);

		//	�A�j���[�V���� = ���ɐ�����Ă���
		pObject->GetComponent<CPlayer>()->SetAnimState(PLAYER_CARRIED);


		//	����
		auto player = pObject->GetComponent<CPlayer>();
		if (player)
		{
			if(this->m_Vector.x < 0)
			player->SetDestRot(45.0f);
			else
			{
				player->SetDestRot(-45.0f);
			}
		}
	}

	//2021/12/22 Shimizu Yosuke-------------------------------------
	//	�N���[�o�[
	if (pObject->GetName() == CLOVER_NAME) {
		//	���ɑ��x��������
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WINDENDING_POWER_X * m_Vector.x;
		Wind->Vel.y += WINDENDING_POWER_Y * m_Vector.y;

		if (Wind->Vel.x >  MAX_CLOVER_VELOCITY) Wind->Vel.x =  MAX_CLOVER_VELOCITY;
		if (Wind->Vel.x < -MAX_CLOVER_VELOCITY) Wind->Vel.x = -MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y >  MAX_CLOVER_VELOCITY) Wind->Vel.y =  MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y < -MAX_CLOVER_VELOCITY) Wind->Vel.y = -MAX_CLOVER_VELOCITY;
	}
	else if (pObject->GetName() == ENDING_CLOVER_NAME)
	{
		//	���ɑ��x��������
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WIND_POWER_X * m_Vector.x;
		Wind->Vel.y += WIND_POWER_Y * m_Vector.y;		//�����鐔�͕ς��ĉ�����

		if (Wind->Vel.x > MAX_CLOVER_VELOCITY) Wind->Vel.x = MAX_CLOVER_VELOCITY;
		if (Wind->Vel.x < -MAX_CLOVER_VELOCITY) Wind->Vel.x = -MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y > MAX_CLOVER_VELOCITY) Wind->Vel.y = MAX_CLOVER_VELOCITY;
		if (Wind->Vel.y < -MAX_CLOVER_VELOCITY) Wind->Vel.y = -MAX_CLOVER_VELOCITY;
	}

	//---------------------------------------------------------------
	//	���𗎂Ƃ�
	if (pObject->GetName() == ICICLE_UP_NAME)
	{
		pObject->GetComponent<CIcicle>()->IcicleDamage();
	}

#ifdef BLOCK_MOVE
	else if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	���ɑ��x��������
		auto Wind = pObject->GetComponent<CTransform>();
		Wind->Vel.x += WIND_POWER_X * m_Vector.x;
		if (Wind->Vel.x > MAX_ICE_VELOCITY)
		{
			Wind->Vel.x = MAX_ICE_VELOCITY;
		}
		else if (Wind->Vel.x < -MAX_ICE_VELOCITY)
		{
			Wind->Vel.x = -MAX_ICE_VELOCITY;
		}
	}
#endif // BLOCK_MOVE

}

// ���̌���(x����,y����)�̐ݒ�
void CWind::SetWindVector(XMFLOAT2 vector)
{
	m_Vector = vector;
}

// ���̌����̎擾
XMFLOAT2 CWind::GetAngle()
{
	return m_Vector;
}