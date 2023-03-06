#include"FallenLeavesComponent.h"
#include"WindComponent.h"
#include"TransformComponent.h"
#include"Sound.h"
#include "DrawEffectComponent.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "CloverComponent.h"

// �R���X�g���N�^
CFallenLeaves::CFallenLeaves()
	: m_pTransform(nullptr), m_bClover(false)
	, m_nCloverNumber(100)
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}

// �f�X�g���N�^
CFallenLeaves::~CFallenLeaves()
{

}

//  �J�n
void CFallenLeaves::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

//  �X�V
void CFallenLeaves::Update()
{

}

//  
void CFallenLeaves::Draw()
{

}
void CFallenLeaves::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == WIND_NAME)
	{
		Parent->Delete();
		//���ʉ�
		//CSound::Play(SE_FALLENLEAVES);

#pragma region--- �����t
		//	�I�u�W�F�N�g����
		Object* obj = new Object("fallen_leaves", UPDATE_BG, DRAW_DEBUG);
		//	�R���|�[�l���g�̒ǉ�
		auto Transobj = obj->AddComponent<CTransform>();			//	�g�����X�t�H�[��
		auto Draw_obj = obj->AddComponent<CDrawEffect>();			//	�G�t�F�N�g�`��@�\
		//	�I�u�W�F�N�g�̐ݒ�
		Transobj->SetPosition(m_pTransform->Pos.x, m_pTransform->Pos.y, OTIBA_POS_Z);//	���W
		Draw_obj->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);				//	�T�C�Y
		Draw_obj->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));	//	�e�N�X�`��
		Draw_obj->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		Draw_obj->SetSwapFrame(OTIBA_ANIM_SPEED);
		//	���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion

		if (m_bClover)
		{
			XMFLOAT3 create_pos = Parent->GetComponent<CTransform>()->Pos;
			create_pos.y += 8.0f;

			// �N���[�o�[����
			Object* pClover;
			pClover = CloverComponent::CreateClover(create_pos, -1);

			// �N���[�o�[�ɔԍ�������U��i�X�u���b�N�ɐݒ肳��Ă���ԍ���^����
			pClover->GetComponent<CloverComponent>()->SetNumber(this->GetNumber());
		}
	}
}

// �����t�����������ɃN���[�o�[����t���O
void CFallenLeaves::Clover()
{
	m_bClover = true;
}

void CFallenLeaves::SetNumber(int num)
{
	m_nCloverNumber = num;
}
int CFallenLeaves::GetNumber()
{
	return m_nCloverNumber;
}