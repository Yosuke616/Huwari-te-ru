#include "PostCollisionComponent.h"
#include "ObjectInfo.h"
#include "InformationComponent.h"
#include "IcicleComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "InformationComponent.h"
#include "DeleteTimerCom.h"
#include "Draw3dComponent.h"
#include "Sound.h"
#include "CloverComponent.h"
#include "EffectManager.h"


// �R���X�g���N�^
CPostCollision::CPostCollision()
	: m_pTransform(nullptr), m_OldPos(0.0f,0.0f)
{
}

// �f�X�g���N�^
CPostCollision::~CPostCollision(){
}

// ������
void CPostCollision::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

void CPostCollision::Update()
{
	m_OldPos.x = m_pTransform->Pos.x;
	m_OldPos.y = m_pTransform->Pos.y;
}

// �Փˏ���
void CPostCollision::OnCollisionEnter(Object* pObject)
{
	// ���ƃu���b�N�̂����蔻��
	if (Parent->GetName() == ICICLE_DOWN_NAME)
	{
		// �u���b�N�Ƃ̂����蔻��
		if (pObject->GetName() == BLOCK_NAME)
		{
			auto InfoCom = Parent->GetComponent<CInformation>();
			if (InfoCom)
			{
				auto pIcicle = InfoCom->GetObjetInfo()->GetComponent<CIcicle>();
				if (pIcicle)
				{
					pIcicle->Stop();
				}
			}
		}
	}
}
