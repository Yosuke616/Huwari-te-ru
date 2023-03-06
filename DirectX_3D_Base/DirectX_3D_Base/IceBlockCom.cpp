#include "IceBlockCom.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "EffectManager.h"
#include "DeleteTimerCom.h"
#include "Draw3dComponent.h"
#include "Sound.h"
#include "InformationComponent.h"
#include "CloverComponent.h"
#include "IcicleComponent.h"


CIceBlock::CIceBlock()
	: m_nDirect(DIR_NONE)
	, m_pTransform(nullptr)
	, m_OldPos(0.0f, 0.0f)
	, m_nCloverNumber(100)
{
}

CIceBlock::~CIceBlock(){
}
void CIceBlock::Start()
{
	// �I�u�W�F�N�g���W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
}
void CIceBlock::Update()
{
	m_OldPos.x = m_pTransform->Pos.x;
	m_OldPos.y = m_pTransform->Pos.y;

	if (m_pTransform->Vel.x > 0) m_nDirect = DIR_RIGHT;
	else if (m_pTransform->Vel.x < 0) m_nDirect = DIR_LEFT;
}
void CIceBlock::Draw(){
}
void CIceBlock::OnCollisionEnter(Object* pObject)
{
	// �X�ƃu���b�N
	if (pObject->GetName() == BLOCK_NAME)
	{
		//	�v���C���[�̏����擾
		auto Owner = Parent->GetComponent<CCollider>();		//	�����蔻��̑傫�����~����
		auto OwnerPos = Owner->GetCenterPos();				// ���S���W
		auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
		auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
		//	�Ԃ���������̏��̎擾
		auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
		auto OtherPos = Other->GetCenterPos();				// ���S���W
		auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
		auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

		//	���ꂼ��̔����̑傫�����i�[
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

		//	�u���b�N�̂����蔻�胉�C��
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	�u���b�N�̍��[
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		�@  �E�[
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		�@  ��[
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		�@  ���[

		/*		   __
			�@	�Z |P|
				�@ |_|			 __
			_________________ �~ |P|
		  �@|���@�u���b�N�̉��� ��|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
		{
			//	P
			//	��
			//	��
			if (m_pTransform->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&		// �O�͂߂荞��ł��Ȃ�
				OwnerPos.y - PlayerHalhSize.y < BlockUpLine)		// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pTransform->Vel.y = 0.0f;

				// ���W�␳
				m_pTransform->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;
			}
			//	��
			//	��
			//	P
			else if (m_pTransform->Vel.y > 0 &&
				BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
				OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// ���͂߂荞��ł�
			{
				// y�̑��x���Ȃ���
				m_pTransform->Vel.y = 0.0f;

				// ���W�␳
				m_pTransform->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// ���S���W

		/*
				�~ __
				  |P|
				  |_|
		  __
		 �@�u|
		 �@��|	�Z __
		�@ �b|	  |P|
		�@ �N|	  |_|
		  __|

		*/
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// P����
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&	// �O�͂߂荞��ł��Ȃ�
				OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)		// ���͂߂荞��ł�
			{
				//	x�ړ��ʂ�����
				m_pTransform->Vel.x = 0.0f;

				// ���W�␳
				m_pTransform->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
			// ����P
			else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
				OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
			{
				//	x�ړ��ʂ�����
				m_pTransform->Vel.x = 0.0f;

				// ���W�␳
				m_pTransform->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
		}


		return;
	}

	// �X�ƕX
	if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	�v���C���[�̏����擾
		auto Owner = Parent->GetComponent<CCollider>();		// �����蔻��̑傫�����~����
		auto OwnerPos = Owner->GetCenterPos();				// ���S���W
		auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
		auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
		//	�Ԃ���������̏��̎擾
		auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
		auto OtherPos = Other->GetCenterPos();				// ���S���W
		auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
		auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

		//	���ꂼ��̔����̑傫�����i�[
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

		//	�u���b�N�̂����蔻�胉�C��
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	�u���b�N�̍��[
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		�@  �E�[
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		�@  ��[
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		�@  ���[

		/*		   __
			�@	�Z |P|
				�@ |_|			 __
			_________________ �~ |P|
		  �@|���@�u���b�N�̉��� ��|   |_|
		*/
		//if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
		//	(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
		//	(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
		//{
		//	//	P
		//	//	��
		//	//	��
		//	if (m_pTransform->Vel.y < 0 &&
		//		BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&		// �O�͂߂荞��ł��Ȃ�
		//		OwnerPos.y - PlayerHalhSize.y < BlockUpLine)		// ���͂߂荞��ł�
		//	{
		//		// y�̑��x���Ȃ���
		//		m_pTransform->Vel.y = 0.0f;
		//
		//		// ���W�␳
		//		m_pTransform->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;
		//	}
		//}
		//
		//OwnerPos = Owner->GetCenterPos();				// ���S���W

			/*
				�~ __
				  |P|
				  |_|
		  __
		 �@�u|
		 �@��|	�Z __
		�@ �b|	  |P|
		�@ �N|	  |_|
		  __|

		*/
		//if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
		//	(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
		//	(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		//{
			// ���͂���
			m_pTransform->Vel.x = pObject->GetComponent<CTransform>()->Vel.x;	// ���x���Փ˂����I�u�W�F�N�g�Ɉڂ�
			pObject->GetComponent<CTransform>()->Vel.x = 0.0f;				// �ڂ������̑��x������

			// ���W�␳
			// �X�u���b�N�̑��x�Ō��߂�
			if (OwnerPos.x > OtherPos.x)		// �X��������
			{
				m_pTransform->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
			else if (OwnerPos.x < OtherPos.x)	// �X��������
			{
				m_pTransform->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
		//}

		return;
	}

	// �X�Ƃ��
	if (pObject->GetName() == ICICLE_DOWN_NAME)
	{
		if (pObject->GetComponent<CInformation>()->GetObjetInfo()->GetComponent<CIcicle>()->IsMoving() == true)
		{
			// ���㕔���폜
			pObject->GetComponent<CInformation>()->GetObjetInfo()->Delete();

			// ��牺�����폜
			pObject->Delete();

			//se
			CSound::Play(SE_ICICLE_DESTROY);	// �������ǉ�
			CSound::Stop(SE_ICICLE_FALL);		// �������ǉ�

			// �G�t�F�N�g�J�n
			EffectManager::GetInstance()->Play(ICICLE_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(3.0f, 3.0f, 3.0f));

			// �X�̃A�j���[�V�����Đ�
			Parent->GetComponent<CDrawModel>()->Play(0);

			// ���t���[����ɍ폜����
			Parent->GetComponent<CTimer>()->SetTime(60);

			// �����蔻��ƍ��W�̍X�V���~�߂�
			Parent->StopUpdate();
			Parent->GetComponent<CTimer>()->m_bUpdateFlag = true;
			Parent->GetComponent<CDrawModel>()->m_bUpdateFlag = true;

			// �@�\�̃^�C�v��FUNC_CREATE_CLOVER�������ꍇ�N���[�o�[������
			if (Parent->GetComponent<CTimer>()->GetFunktion() == FUNC_CREATE_CLOVER)
			{
				// ���W
				XMFLOAT3 create_pos = Parent->GetComponent<CTransform>()->Pos;
				create_pos.y += 8.0f;

				// �N���[�o�[������
				Object* pClover;
				pClover = CloverComponent::CreateClover(create_pos, -1);

				// �N���[�o�[�ɔԍ�������U��i�X�u���b�N�ɐݒ肳��Ă���ԍ���^����
				pClover->GetComponent<CloverComponent>()->SetNumber(this->GetNumber());
			}
		}
		else
		{
			//	�v���C���[�̏����擾
			auto Owner = Parent->GetComponent<CCollider>();		//	�����蔻��̑傫�����~����
			auto OwnerPos = Owner->GetCenterPos();				// ���S���W
			auto OwnerSize = Owner->GetColliderSize();			// �����蔻��̑傫��
			auto OwnerOffset = Owner->GetOffSet();				// ���S�Ƃ̂���
			//	�Ԃ���������̏��̎擾
			auto Other = pObject->GetComponent<CCollider>();	// �����蔻��̑傫�����~����
			auto OtherPos = Other->GetCenterPos();				// ���S���W
			auto OtherSize = Other->GetColliderSize();			// �����蔻��̑傫��
			auto OtherOffset = Other->GetOffSet();				// ���S�Ƃ̂���

			//	���ꂼ��̔����̑傫�����i�[
			XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
			XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

			//	�u���b�N�̂����蔻�胉�C��
			float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	�u���b�N�̍��[
			float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		�@  �E�[
			float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		�@  ��[
			float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		�@  ���[

			/*		   __
				�@	�Z |P|
					�@ |_|			 __
				_________________ �~ |P|
			  �@|���@�u���b�N�̉��� ��|   |_|
			*/
			if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
				(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// ���̓_
				(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// �E�̓_
			{
				//	P
				//	��
				//	��
				if (m_pTransform->Vel.y < 0 &&
					BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&		// �O�͂߂荞��ł��Ȃ�
					OwnerPos.y - PlayerHalhSize.y < BlockUpLine)		// ���͂߂荞��ł�
				{
					// y�̑��x���Ȃ���
					m_pTransform->Vel.y = 0.0f;

					// ���W�␳
					m_pTransform->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;
				}
				//	��
				//	��
				//	P
				else if (m_pTransform->Vel.y > 0 &&
					BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// �O�͂߂荞��ł��Ȃ�
					OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// ���͂߂荞��ł�
				{
					// y�̑��x���Ȃ���
					m_pTransform->Vel.y = 0.0f;

					// ���W�␳
					m_pTransform->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
				}
			}


			OwnerPos = Owner->GetCenterPos();				// ���S���W

			/*
					�~ __
					  |P|
					  |_|
			  __
			 �@�u|
			 �@��|	�Z __
			�@ �b|	  |P|
			�@ �N|	  |_|
			  __|

			*/
			if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
			{
				// P����
				if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&	// �O�͂߂荞��ł��Ȃ�
					OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)		// ���͂߂荞��ł�
				{
					//	x�ړ��ʂ�����
					m_pTransform->Vel.x = 0.0f;

					// ���W�␳
					m_pTransform->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
				}
				// ����P
				else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
					OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
				{
					//	x�ړ��ʂ�����
					m_pTransform->Vel.x = 0.0f;

					// ���W�␳
					m_pTransform->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
				}
			}


			return;
		}
		
	}
}

// ������ł�������̎擾
int CIceBlock::GetDirect()
{
	return m_nDirect;
}


void CIceBlock::SetNumber(int num)
{
	m_nCloverNumber = num;
}
int CIceBlock::GetNumber()
{
	return m_nCloverNumber;
}