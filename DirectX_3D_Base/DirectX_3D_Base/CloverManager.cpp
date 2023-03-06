#include "CloverManager.h"

#include "SceneEnding.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Draw3dComponent.h"
#include "Draw2dComponent.h"
#include "DrawEffectComponent.h"

#include "fallleafComponent.h"
#include "CloverComponent.h"
#include "HomingComponent.h"

#include "Camera.h"

#include "ObjectManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "OffScreenComponent.h"
#include "NumberCom.h"
#include "EGravityComponent.h"

#include "GravityComponent.h"

//===== マクロ定義
#define CREATE_FRAME	(300)

#define CREATE_TIME_1	(500)
#define CREATE_TIME_2	(500 + CREATE_FRAME * 1)
#define CREATE_TIME_3	(500 + CREATE_FRAME * 2)
#define CREATE_TIME_4	(500 + CREATE_FRAME * 3)
#define CREATE_TIME_5	(500 + CREATE_FRAME * 4)
#define CREATE_TIME_6	(500 + CREATE_FRAME * 5)
#define CREATE_TIME_7	(500 + CREATE_FRAME * 6)
#define CREATE_TIME_8	(500 + CREATE_FRAME * 7)

//=== 静的メンバ変数
CloverManager* CloverManager::m_pInstance = nullptr;

//--- シングルトン------------------------------------

// インスタンスの生成＆取得
CloverManager* CloverManager::GetInstance()
{
	// インスタンスが作られとぃない場合は作る
	if (!m_pInstance)
	{
		m_pInstance = new CloverManager();
	}

	return m_pInstance;
}

// インスタンス破棄
void CloverManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//-------------------------------------------------


// コンストラクタ
CloverManager::CloverManager()
	:m_nFrameCnt(0)
{

}
// デストラクタ
CloverManager::~CloverManager()
{

}

// 更新
void CloverManager::Update()
{
	m_nFrameCnt++;

	
	if ((m_nFrameCnt == CREATE_TIME_1 && SceneEnding::m_bSpawnClover[0]) ||
		(m_nFrameCnt == CREATE_TIME_2 && SceneEnding::m_bSpawnClover[1]) ||
		(m_nFrameCnt == CREATE_TIME_3 && SceneEnding::m_bSpawnClover[2]) ||
		(m_nFrameCnt == CREATE_TIME_4 && SceneEnding::m_bSpawnClover[3]) ||
		(m_nFrameCnt == CREATE_TIME_5 && SceneEnding::m_bSpawnClover[4]) ||
		(m_nFrameCnt == CREATE_TIME_6 && SceneEnding::m_bSpawnClover[5]) ||
		(m_nFrameCnt == CREATE_TIME_7 && SceneEnding::m_bSpawnClover[6]) ||
		(m_nFrameCnt == CREATE_TIME_8 && SceneEnding::m_bSpawnClover[7]))
	{

		// クローバー生成
		CreateClover();
	}

	//if (m_nFrameCnt > CREATE_FRAME)
	//{
	//
	//	// カウントリセット
	//	m_nFrameCnt = 0;
	//}
}


// クローバー生成
void CloverManager::CreateClover()
{
	// オブジェクト生成
	Object* pClover = new Object(ENDING_CLOVER_NAME, UPDATE_MODEL, DRAW_MODEL);
	// コンポーネントの追加
	auto transClover = pClover->AddComponent<CTransform>();		// オブジェクト座標
	auto drawClover = pClover->AddComponent<CDrawModel>();			// モデル描画
	auto ColliderClover = pClover->AddComponent<CCollider>();	//　衝突判定
	auto homing = pClover->AddComponent<CHoming>();				// マウス追従
	auto Fall = pClover->AddComponent<CEGravity>();
	auto offScreen = pClover->AddComponent<COffScreen>();
	// リストに追加
	ObjectManager::GetInstance()->AddObject(pClover);
	// オブジェクト設定
	transClover->SetRotate(0.0f, 0.0f, 0.0f);
	transClover->SetScale(15.0f, 15.0f, 15.0f); 
	drawClover->SetModel(ModelManager::GetInstance()->GetModel(END_CLOVER_MODEL_NUM));	//　モデル
	ColliderClover->SetCollisionSize(50.0f, 50.0f);					//　当たり判定の大きさ
	homing->SetEffectNum(CURSOR_EFFECT_NUM);
	offScreen->SetFinction(ON_SCREEN);


	//@@@ ここは内藤にまかせます

	// ①座標、速度を決める(現在テキトーに設定しています



	// ②エンディングの経過時間によって挙動を変える
	int nTime = SceneEnding::GetCurrentFrameCnt();
	if (375 > nTime)
	{
		// 生成の座標
		transClover->SetPosition(-70.0f, 200.0f, -10.0f);
	}
	else if (375 <= nTime && nTime < 750)
	{
		// 生成の座標
		transClover->SetPosition(0.0f, 120.0f, -10.0f);			//クローバー1

	}
	else if (750 <= nTime && nTime < 1125)
	{
		// 生成の座標
		transClover->SetPosition(170.0f, 200.0f, -10.0f);		//クローバー2

	}
	else if (1125 <= nTime && nTime < 1500)
	{
		// 生成の座標
		transClover->SetPosition(-170.0f, 200.0f, -10.0f);		//クローバー3

	}
	else if (1500 <= nTime && nTime < 1875)
	{
		// 生成の座標
		transClover->SetPosition(100.0f, 200.0f, -10.0f);		//クローバー4

	}
	else if (1875 <= nTime && nTime < 2250)
	{
		// 生成の座標
		transClover->SetPosition(-100.0f, 200.0f, -10.0f);		//クローバー5

	}
	else if (2250 <= nTime && nTime < 2625)
	{
		// 生成の座標
		transClover->SetPosition(-70.0f, 200.0f, -10.0f);		//クローバー6
	}
	else if (2625 <= nTime && nTime < 3000)
	{
		// 生成の座標
		transClover->SetPosition(70.0f, 200.0f, -10.0f);		//クローバー7
	}
	else if (3000 <= nTime && nTime < ENDING_FRAME)
	{
		// 生成の座標
		transClover->SetPosition(0.0f, 200.0f, -10.0f);			//クローバー8
	}
	//得点計算
	SceneEnding::Cntplus();
}

// 2022/1/29 narita クローバーの数表示----------------------------------------------
int CloverManager::GetCloverNum()
{
	std::list<Object*> Clovers;

	ObjectManager::GetInstance()->GetGameObjects(ENDING_CLOVER_NAME, Clovers);

	return (int)Clovers.size();
}
//-----------------------------------------------------------------------------