//  �C���N���[�h��
#include "SeedComponent.h"
#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "Draw2dComponent.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "Object.h"

//  �R���X�g���N�^
CSeed::CSeed() 
	: m_bUIFlg(false)
{
}

//  �f�X�g���N�^
CSeed::~CSeed() {
}

//  ������
void CSeed::Start() {
	//m_pTransform = Parent->GetComponent<CTransform>();
	//m_pDraw = Parent->GetComponent<CDrawModel>();
	//m_pCollider = Parent->GetComponent<CCollider>();
}

//  �X�V
void CSeed::Update() {

}

//  �`��
void CSeed::Draw() {

}

//  �Փ˂����Ƃ��̏���
void CSeed::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == PLAYER_NAME)
	{
		
	}
}
















