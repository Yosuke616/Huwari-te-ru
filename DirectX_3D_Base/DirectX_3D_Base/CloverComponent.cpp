/**
 * @file	CloverComponent.h
 * @brief	�N���[�o�[�̏���
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 *			2021/12/22 shimizu yousuke ���g��ύX
*/

//===== �C���N���[�h =====
#include "CloverComponent.h"
#include "InputManager.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "FallComponent.h"
#include "Draw2DComponent.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "StageManager.h"
#include "Sound.h"
#include "Draw3dComponent.h"
#include "GravityComponent.h"
#include "ModelManager.h"
#include "FallComponent.h"
#include "OutRangeCom.h"
#include "TagCom.h"

//***�R���X�g���N�^
CloverComponent::CloverComponent()
	: m_bCloverflag(false), m_pTransform(nullptr),
	m_nCnt_Clover(0)
	, m_OldPos(0.0f,0.0f)
	, m_bBind(false)
	, m_nNumber(0)
{
}

// �f�X�g���N�^
CloverComponent::~CloverComponent(){
}

// ������
void CloverComponent::Start()
{
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CloverComponent::Update()
{
	m_OldPos = m_pCollider->GetCenterPos();

	// X�ړ��ʂɊ�����������
	m_pTransform->Vel.x += -m_pTransform->Vel.x * RATE_MOVE_CLOVER;

}

// �`��
void CloverComponent::Draw(){
}

// �Փˏ���
void CloverComponent::OnCollisionEnter(Object* pObject)
{
	//	�u���b�N
	if (pObject->GetName() == BLOCK_NAME ||
		pObject->GetName() == ICICLE_UP_NAME ||
		pObject->GetName() == ICICLE_DOWN_NAME ||
		pObject->GetName() == ICE_BLOCK_NAME)
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

#ifdef COLVER_FALL
				// �h���t���OOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);
#endif
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


// �N���[�o�[����
Object* CloverComponent::CreateClover(XMFLOAT3 pos, int id)
{
#pragma region ---�N���[�o�[
	Object* pClover = new Object(CLOVER_NAME, UPDATE_MODEL, DRAW_MODEL);
	// components
	auto TransClover = pClover->AddComponent<CTransform>();				//�@�I�u�W�F�N�g�̍��W�Ƃ�
	auto DrawClover = pClover->AddComponent<CDrawModel>();					//�@���f���@�\
	auto ColliderClover = pClover->AddComponent<CCollider>();			//�@�Փ˔���@�\
	auto Clover = pClover->AddComponent<CloverComponent>();				//�@�N���[�o�[�̃R���|�[�l���g
	pClover->AddComponent<CGravity>();									//  ���ɏ���ė��Ƃ����p��
	auto outRange = pClover->AddComponent<COutRange>();
	auto comID = pClover->AddComponent<CTag>();
	comID->SetID(id);
	outRange->SetLimitRange(OUT_RANGE_X - 40.0f, OUT_RANGE_Y);
	// ssettings
	TransClover->SetPosition(pos.x, pos.y + CLOVER_OFFSET_Y, pos.z + CLOVER_POS_Z);				//�@���W
	TransClover->SetRotate(CLOVER_ROTATE_X, CLOVER_ROTATE_Y, CLOVER_ROTATE_Z);	// ��]
	TransClover->SetScale(CLOVER_SCALE_X, CLOVER_SCALE_Y, CLOVER_SCALE_Z);		//�@�傫��
	DrawClover->SetModel(ModelManager::GetInstance()->GetModel(CLOVER_MODEL_NUM));//�@���f��
	ColliderClover->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);			//�@�����蔻��̑傫��
	// add to list
	ObjectManager::GetInstance()->AddObject(pClover);
#pragma endregion

	return pClover;
}

// �S������Ă���t���O�̐ݒ�	true �S������Ă���
void CloverComponent::Binded(bool bBind)
{
	m_bBind = bBind;
}

// �S������Ă��邩�̊m�F
bool CloverComponent::IsBinded()
{
	return m_bBind;
}


// �N���[�o�[�̔ԍ�(�X�e�[�W�̍�����0, 1, 2, 3
void CloverComponent::SetNumber(int number)
{
	m_nNumber = number;
}
// �N���[�o�[�̔ԍ����擾����
int CloverComponent::GetNumber()
{
	return m_nNumber;
}