//  インクルード部
#include "SeedComponent.h"
#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "Draw2dComponent.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "Object.h"

//  コンストラクタ
CSeed::CSeed() 
	: m_bUIFlg(false)
{
}

//  デストラクタ
CSeed::~CSeed() {
}

//  初期化
void CSeed::Start() {
	//m_pTransform = Parent->GetComponent<CTransform>();
	//m_pDraw = Parent->GetComponent<CDrawModel>();
	//m_pCollider = Parent->GetComponent<CCollider>();
}

//  更新
void CSeed::Update() {

}

//  描画
void CSeed::Draw() {

}

//  衝突したときの処理
void CSeed::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == PLAYER_NAME)
	{
		
	}
}
















