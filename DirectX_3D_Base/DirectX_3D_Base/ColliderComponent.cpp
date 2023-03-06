//===== �C���N���[�h =====
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "BillboardComponent.h"
#include "Draw2dComponent.h"

//===== �ÓI�����o�ϐ� =====
std::list<Object*> CCollider::m_CollidrList;	//	�����蔻�胊�X�g

// �R���X�g���N�^
CCollider::CCollider()
	: m_pTransform(nullptr)
{
	m_eUpdateOrder = COM_UPDATE_COLLIDER;	//	�R���|�[�l���g���X�g�̍X�V����

	m_CenterOffset = XMFLOAT2(0.0f, 0.0f);
	m_CollisionSize = XMFLOAT3(0.0f, 0.0f,0.0f);
}

// �f�X�g���N�^
CCollider::~CCollider()
{
}

// ������
void CCollider::Start()
{
	//	�R���|�[�l���g�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();	//	�I�u�W�F�N�g���W�̎擾

	//	�����蔻�胊�X�g�ɓo�^
	AddColliderList(Parent);

#ifdef _DEBUG
	//	�f�o�b�O�p�R���|�[�l���g�̒ǉ�
	if (Parent->GetComponent<CDraw2D>() == nullptr)
	{
		auto pDebug = Parent->AddComponent<CDrawMesh>();
		//	�ݒ�
		pDebug->SetOffset(m_CenterOffset);
		pDebug->SetSize(m_CollisionSize.x, m_CollisionSize.y);

		//	������
		pDebug->Start();
	}
#endif // _DEBUG

}

// �X�V
void CCollider::Update()
{
	//	�Փ˔��胊�X�g�ւ̒ǉ����[�v
	for (auto&& OtherObject : m_CollidrList)
	{
		//	�ǂ��炩�̍폜�t���O��true�������ꍇ continue
		if (Parent->GetDeleteFlag() == true) continue;
		if (OtherObject->GetDeleteFlag() == true) continue;

		// �R���|�[�l���g���@�\��~�̎� continue
		auto OtherCollider = OtherObject->GetComponent<CCollider>();
		if (this->m_bUpdateFlag == false) continue;
		if (OtherCollider->m_bUpdateFlag == false) continue;

		//	�����蔻������Ώۂ����̃I�u�W�F�N�g�������ꍇ continue
		//if (OtherObject->GetName() == Parent->GetName()) continue;
		if (OtherObject == Parent) continue;
	
		//	�����蔻������Ώۂ̏����擾
		auto Other = OtherObject->GetComponent<CTransform>();			//	�����蔻������Ώۂ̍��W�n

		// 2��ڂ̏Փˏ����������
		bool bOnceHit = false;

		for (auto OnceCollision : this->m_IntersectObjects)
		{
			// �����������������Ƃ�����I�u�W�F�N�g�̃��X�g == ���݂̂����蔻������Ώ�
			if (OnceCollision == OtherObject)
			{
				bOnceHit = true;
				break;
			}
		}
		if (bOnceHit) continue;	// �Փ˂������Ƃ�����ꍇ continue
		
	
		

		//	x,y���ʂ̂����蔻��
		if (this->CollisionRectToRectXY(
			m_pTransform,										//	�����̍��W
			Other,												//	����̍��W
			XMFLOAT2(m_CollisionSize.x, m_CollisionSize.y),		//	�����̑傫��
			XMFLOAT2(OtherCollider->m_CollisionSize.x,			//	����̑傫��
				OtherCollider->m_CollisionSize.y),
			m_CenterOffset,										//	�����̃I�t�Z�b�g(�`��̒��S�Ƃ����蔻��̒��S�̍�)
			OtherCollider->m_CenterOffset))						//	����̃I�t�Z�b�g
		{
			// ����ɓ����������Ƃ�m�点��
			OtherCollider->m_IntersectObjects.push_back(Parent);
		}
	}
}

// �傫���̐ݒ�
void CCollider::SetCollisionSize(float fWidth, float fHeight , float fz)
{
	m_CollisionSize.x = fWidth;
	m_CollisionSize.y = fHeight;
	m_CollisionSize.z = fz;
}

//	�I�t�Z�b�g
void CCollider::SetOffset(float fx, float fy)
{
	m_CenterOffset.x = fx;
	m_CenterOffset.y = fy;
}
XMFLOAT2 CCollider::GetOffSet()
{
	return m_CenterOffset;
}

// �T�C�Y�擾
XMFLOAT3 CCollider::GetColliderSize()
{
	return m_CollisionSize;
}
XMFLOAT2 CCollider::GetCenterPos()
{
	return XMFLOAT2(m_pTransform->Pos.x + m_CenterOffset.x, m_pTransform->Pos.y + m_CenterOffset.y);
}

// ��`�Ƌ�`
bool CCollider::CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
	XMFLOAT2 size1, XMFLOAT2 size2, XMFLOAT2 Velocity1, XMFLOAT2 Velocity2)
{
	XMFLOAT2 halfSize1 = XMFLOAT2(size1.x / 2.0f, size1.y / 2.0f);
	XMFLOAT2 halfSize2 = XMFLOAT2(size2.x / 2.0f, size2.y / 2.0f);


	if (centerPos2.x - halfSize2.x + Velocity2.x < centerPos1.x + halfSize1.x + Velocity1.x &&		// 2�̍��[ < 1�̉E�[
		centerPos1.x - halfSize1.x + Velocity1.x < centerPos2.x + halfSize2.x + Velocity2.x)		// 1�̍��[ < 2�̉E�[
	{
		if (centerPos2.y - halfSize2.y + Velocity2.y < centerPos1.y + halfSize1.y + Velocity1.y  &&	// 2�̏�[ < 1�̉��[
			centerPos1.y - halfSize1.y + Velocity1.y < centerPos2.y + halfSize2.y + Velocity2.y)	// 1�̏�[ < 2�̉��[
		{
			return true;
		}
	}

	return false;
}

// �~�Ɖ~
bool CCollider::CheckCollisionCircleToCircle(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
	float radius1, float radius2)
{
	if ((centerPos1.x - centerPos2.x) * (centerPos1.x - centerPos2.x)
		+ (centerPos1.y - centerPos2.y) * (centerPos1.y - centerPos2.y)	// (�~1�̒��S���WX - �~2�̒��S���WX)��2�� + (�~1�̒��S���WY - �~2�̒��S���WY)��2��
		<= (radius1 + radius2) * (radius1 + radius2))	// (�~1�̔��a+�~2�̔��a)��2��
	{
		return true;
	}

	return false;
}

// ���Ƌ�
bool CCollider::CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// AABB���m�̏Փ˔���
bool CCollider::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&	//	A�̍��[ <= B�̉E�[
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&		//	B�̍��[ <= A�̉E�[
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&		//	A�̉��[ <= B�̏�[
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&		//	B�̉��[ <= A�̏�[
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&		//	A�̎�O <= B�̉�
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);			//	B�̎�O <= A�̉�
}

//	xy���ʂ̂����蔻��
bool CCollider::CollisionRectToRectXY(CTransform* pA, CTransform* pB
	, XMFLOAT2 Asize, XMFLOAT2 Bsize
	, XMFLOAT2 Aoffset, XMFLOAT2 Boffset)
{
	//	���S���W
	//XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + pA->Vel.x + Aoffset.x, pA->Pos.y + pA->Vel.y + Aoffset.y);
	//XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + pB->Vel.x + Aoffset.x, pB->Pos.y + pB->Vel.y + Boffset.y);
	XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + Aoffset.x, pA->Pos.y + Aoffset.y);
	XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + Aoffset.x, pB->Pos.y + Boffset.y);
	XMFLOAT2 halfSizeA = XMFLOAT2(Asize.x / 2.0f, Asize.y / 2.0f);
	XMFLOAT2 halfSizeB = XMFLOAT2(Bsize.x / 2.0f, Bsize.y / 2.0f);

	if (centerPosB.x - halfSizeB.x < centerPosA.x + halfSizeA.x  &&		// B�̍��[ < A�̉E�[
		centerPosA.x - halfSizeA.x < centerPosB.x + halfSizeB.x )		// A�̍��[ < B�̉E�[
	{
		if (centerPosB.y - halfSizeB.y < centerPosA.y + halfSizeA.y  &&	// B�̏�[ < A�̉��[
			centerPosA.y - halfSizeA.y < centerPosB.y + halfSizeB.y)	// A�̏�[ < B�̉��[
		{
			return true;
		}
	}

	return false;
}


// �����蔻�胊�X�g�ɒǉ�
void CCollider::AddColliderList(Object* pObject)
{
	//	�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_CollidrList.begin();
	for (auto&& object : m_CollidrList)
	{
		if (pObject->GetUpdateOrder() > object->GetUpdateOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	m_CollidrList.insert(itr, pObject);
}

//	�����蔻��̃��X�g���擾
std::list<Object*>& CCollider::GetColliderList()
{
	return m_CollidrList;
}

void CCollider::UpdateColliders()
{
	for (auto this_object : m_CollidrList)
	{
		if (this_object->GetName() != PLAYER_NAME)
		{
			// �R���C�_�[�@�\�������Ă���
			auto this_collider = this_object->GetComponent<CCollider>();

			// �Փˎ��̏���
			//	 auto �Փ˂�������		  : �Փ˂������肽��
			for (auto collided_object : this_collider->m_IntersectObjects)
			{
				//	���̃I�u�W�F�N�g�̏Փˏ��������s
				this_object->OnCollisionEnter(collided_object);
				//	�Փ˂�������̏Փˏ��������s
				collided_object->OnCollisionEnter(this_object);
			}
		}
		// �v���C���[
		else
		{
			// �R���C�_�[�@�\�������Ă���
			auto this_collider = this_object->GetComponent<CCollider>();
			
			if (this_collider->m_IntersectObjects.size() > 1)
			{
				this_collider->m_IntersectObjects.sort([=](Object* A, Object* B)
					{
						auto thisPos = this_collider->GetCenterPos();
						auto PosA = A->GetComponent<CCollider>()->GetCenterPos();
						auto PosB = B->GetComponent<CCollider>()->GetCenterPos();
						// ����
						XMFLOAT2 DiffA(thisPos.x - PosA.x, thisPos.y - PosA.y);
						XMFLOAT2 DiffB(thisPos.x - PosB.x, thisPos.y - PosB.y);

						XMVECTOR vDiffA = DirectX::XMLoadFloat2(&DiffA);
						XMVECTOR vDiffB = DirectX::XMLoadFloat2(&DiffB);
						XMVECTOR vLengthA;
						XMVECTOR vLengthB;
						vLengthA = DirectX::XMVector2LengthEst(vDiffA);
						vLengthB = DirectX::XMVector2LengthEst(vDiffB);
						// Diff�ė��p
						DirectX::XMStoreFloat2(&DiffA, vLengthA);
						DirectX::XMStoreFloat2(&DiffB, vLengthB);

						return DiffA.x < DiffB.x;
					});
			}

			// �Փˎ��̏���
			//	 auto �Փ˂�������		  : �Փ˂������肽��
			for (auto collided_object : this_collider->m_IntersectObjects)
			{
				//	���̃I�u�W�F�N�g�̏Փˏ��������s
				this_object->OnCollisionEnter(collided_object);
				//	�Փ˂�������̏Փˏ��������s
				collided_object->OnCollisionEnter(this_object);
			}
		}

		// �Փ˂������Ƃ�m�点��̂���߂�
		this_object->GetComponent<CCollider>()->m_IntersectObjects.clear();
	}
}