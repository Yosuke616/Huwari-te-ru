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

CGoletter::CGoletter()	//コンストラクタ
{

}

CGoletter::~CGoletter()	//デストラクタ
{

}

void CGoletter::Update()
{

}

void CGoletter::CreateGoletter()	//	文字の作成
{
	XMFLOAT2 WorldSize = CalcWorldSize();
	XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
	//オブジェクトの生成
	Object* pCGameover = new Object(GOLETTER_NAME, UPDATE_GOLETTER, DRAW_GOLETTER);
	//	コンポーネントの追加
	auto transGoletter = pCGameover->AddComponent<CTransform>();			//トランスフォーム
	auto drawGoletter = pCGameover->AddComponent<CDraw2D>();				//描画
	auto GoletterFall = pCGameover->AddComponent<CFall>();					//文字の落下
	auto GoletterCollider = pCGameover->AddComponent<CCollider>();

	transGoletter->SetPosition(GOLETTER_POS_X, GOLETTER_POS_Y);
	drawGoletter->SetSize(GOLETTER_SIZE_X, GOLETTER_SIZE_Y);
	drawGoletter->SetTexture(TextureManager::GetInstance()->GetTexture(GAMEOVER_TEX_NUM));

	MenuManager::GetInstance()->AddMenu(pCGameover);
}
