#include "CGameover.h"
#include "TransformComponent.h"
#include "DrawEffectComponent.h"
#include "Draw2dComponent.h"
#include "ObjectManager.h"
#include "FallComponent.h"		
#include "Camera.h"
#include "TextureManager.h"
#include "ColliderComponent.h"
#include "MenuManager.h"

CGoletter::CGoletter()	//�R���X�g���N�^
{

}

CGoletter::~CGoletter()	//�f�X�g���N�^
{

}

void CGoletter::Update()
{

}

void CGoletter::CreateGoletter()	//	�����̍쐬
{
	XMFLOAT2 WorldSize = CalcWorldSize();
	XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
	//�I�u�W�F�N�g�̐���
	Object* pCGameover = new Object(GOLETTER_NAME, UPDATE_GOLETTER, DRAW_GOLETTER);
	//	�R���|�[�l���g�̒ǉ�
	auto transGoletter = pCGameover->AddComponent<CTransform>();			//�g�����X�t�H�[��
	auto drawGoletter = pCGameover->AddComponent<CDraw2D>();				//�`��
	auto GoletterFall = pCGameover->AddComponent<CFall>();					//�����̗���
	auto GoletterCollider = pCGameover->AddComponent<CCollider>();

	transGoletter->SetPosition(GOLETTER_POS_X, GOLETTER_POS_Y);
	drawGoletter->SetSize(GOLETTER_SIZE_X, GOLETTER_SIZE_Y);
	drawGoletter->SetTexture(TextureManager::GetInstance()->GetTexture(GAMEOVER_TEX_NUM));

	MenuManager::GetInstance()->AddMenu(pCGameover);
}
