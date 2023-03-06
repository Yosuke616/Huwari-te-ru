#include "WindManager.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2dComponent.h"
#include "imgui.h"
#include "ObjectManager.h"
#include <list>
#include "Camera.h"
#include "ColliderComponent.h"
#include "DrawEffectComponent.h"
#include "TextureManager.h"
#include "WindComponent.h"
#include "GaugeComponent.h"

#include "Sound.h"

//テクスチャ分割用
#define UV_DXCHAN_WIDTH		(4)
#define UV_DXCHAN_HEIGHT	(4)

#define CREATE_FRAME		(1)

#define BIG_SOUND			(5)
#define SE_FRAME			(100)

//	平方根(正確ではない)
float squareRoot(float x);
//	2乗関数
float pow(float x);

//===== 静的メンバ変数 =====
WindManager* WindManager::m_pInstance = nullptr;


//=== グローバル変数
int g_nSE = 0;

// インスタンス生成
WindManager* WindManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new WindManager();
	}
	return m_pInstance;
}

// 終了処理
void WindManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// コンストラクタ
WindManager::WindManager()
	: m_nFrameCnt(0)
	, m_nDeleteCnt(0)
	, m_bDeleteFlg(false)
	, m_bTutorialFlg(false)
	, m_bSpawnWind(false)

{
	m_WindCenterList.resize(0);
	m_WindVectorList.resize(0);

	g_nSE = 0;
}

// デストラクタ
WindManager::~WindManager()
{
}

// 更新
void WindManager::Update()
{
	InputManager* Input = InputManager::Instance();

	// se制御
	if (g_nSE > 0)
	{
		g_nSE--;
	}

	if (m_bSpawnWind == false) return;
	
	//	Lマウスプレス
	if (Input->GetMouseButton(MOUSE_L))
		{
			//	風ゲージが0の場合作らない
			if (CGauge::GetCurrentGauge(m_pWindGauge) <= 0) return;

			//	Lマウスを押してるフラグON
			m_bDeleteFlg = false;

			//	マウスの座標を取得する
			m_P = Input->GetMousePos();

			//数フレームごとにマウス座標を取得する
			if (m_nFrameCnt >= CREATE_FRAME)
			{
				//	マウス座標を保存
				m_WindCenterList.push_back(m_P);

				//	//二つ以上の座標があったときの場合
				if (m_WindCenterList.size() >= 2)
				{
					// 座補油間の距離を計算する
					std::list<XMFLOAT2>::iterator itrPoint = m_WindCenterList.end();
					itrPoint--;	itrPoint--;		//	座標リストの後ろから二番目
					m_WindPower.x = m_P.x - (*itrPoint).x;				// 座標リスト一番後ろ - 座標リストの後ろから二番目
					m_WindPower.y = m_P.y - (*itrPoint).y;				// 座標リスト一番後ろ - 座標リストの後ろから二番目
					float fDistance = squareRoot(pow(m_WindPower.x) + pow(m_WindPower.y));

					// 一定以上の距離以上離れていたら続ける
					if (fDistance < WIND_CREATE_DIFF)
					{
						// 離れていなかったら最新のマウス座標をポップする
						m_WindCenterList.pop_back();
						return;
					}

					//---- 風を作ることが確定 -----

					// ↑の一定の距離×n以上離れていたらn個の風を作る

					
					//--- 風のベクトル計算
					// 風のパワーを単位ベクトルに直して保存
					XMVECTOR xmvWindNorm = XMLoadFloat2(&m_WindPower);	//	XMFLOAT2をXMVECTOR型にを変換
					xmvWindNorm = XMVector2Normalize(xmvWindNorm);		//	ベクトルの正規化
					XMFLOAT2 vWindNorm;									//	正規化されたXMVECTOR型をXMFLOAT2型に入れる箱の用意
					DirectX::XMStoreFloat2(&vWindNorm, xmvWindNorm);	//	XMVECTORをXMFLOAT2に変換
					m_WindVectorList.push_back(vWindNorm);				// 保存

					//	風の数をカウントする
					CGauge::CalcGauge(m_pWindGauge, -1);
				}
				m_nFrameCnt = 0;	//	保存したらフレーム数をリセット
			}

			//フレーム数をカウント
			m_nFrameCnt++;
		}

	//	Lマウスリリース
	if (Input->GetMouseRelease(MOUSE_L))
	{
		if (m_WindVectorList.size() > 0 && g_nSE <= 0)
		{
			// 小さい方の音
			CSound::Play(SE_WIND);

			g_nSE = SE_FRAME;
		}
		m_bDeleteFlg = true;		//	Lマウスを押してるフラグOFF
	}

	//	リリースした後
	if (m_bDeleteFlg)
	{
		if (m_nDeleteCnt >= CREATE_FRAME)
		{
			//風の発生位置
			std::list<XMFLOAT2>::iterator itrPos = m_WindCenterList.begin();
			//風の向き
			std::list<XMFLOAT2>::iterator itrPower = m_WindVectorList.begin();

			//	風の生成リストのサイズが0の場合、return
			if (m_WindVectorList.size() <= 0)
			{
				if (m_WindCenterList.size())
				{
					m_WindCenterList.pop_front();
				}
				return;
			}

			//	このタイミングで風を作る
			WindManager::CreateWind((*itrPos), (*itrPower));

			//	マウス座標のリストの先頭から消していく
			m_WindCenterList.pop_front();
			//	風のベクトルリストも消す
			if (m_WindVectorList.size())
			{
				m_WindVectorList.pop_front();
			}

			//	カウントを0にする
			m_nDeleteCnt = 0;
		}
		m_nDeleteCnt++;

		//	すべての風を生成したらフラグをOFF
		if (!m_WindCenterList.size())
		{
			m_bDeleteFlg = false;
		}
	}

	//オブジェクトリストを全部見て風が無かったらfalseに変える
	//m_bTutorialFlgがオンだった時だけ検索する
	if (m_bTutorialFlg) {
		Object* pWind = ObjectManager::GetInstance()->GetGameObject(WIND_NAME);

		if (!pWind) {
			//チュートリアルフラグをオフにする
			m_bTutorialFlg = false;
		}
	}
}

// 描画
void WindManager::Draw()
{
//#ifdef _DEBUG
//	using namespace ImGui;
//
//	//マウスの座標の大きさ
//	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	Begin("StorePosition");			//タイトルバーと一致したらそこに追加する
//	Text("MousePosList : %d", m_WindCenterList.size());
//	//マウスの座標の表示
//	for (auto&& Pos : m_WindCenterList)
//	{
//		Text("MousePos_X_Y     : (%.0f , %.0f) ", Pos.x, Pos.y);
//	}
//	End();
//
//	////	風のパワー
//	//SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	//SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	//Begin("StoreVector");
//	//Text("WindPowerList : %d", m_WindVectorList.size());
//	//for (auto&& Vector : m_WindVectorList)
//	//{
//	//	Text("WindVector   : (%.0f , %.0f) ", Vector.x, Vector.y);
//	//}
//	//End();
//
//#endif // _DEBUG
}


/**
* @fn		CreateBlock
* @brief	風の生成
* @param	(XMFLOAT2) pos	 スクリーン座標xy
* @param	(XMFLOAT2) power 風の力の量
*/
void WindManager::CreateWind(XMFLOAT2 pos, XMFLOAT2 power)
{
	TextureManager* pTex = TextureManager::GetInstance();

	//	スクリーン座標をワールド座標に変換
	XMMATRIX view = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
	XMFLOAT3 worldPos;
	CalcScreenToXY(&worldPos, pos.x, pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view,	prj);
	
	//	ワールド座標用の大きさに変換
	XMFLOAT2 WindSize(WIND_WIDTH , WIND_HEIGHT);

	//	ラジアンで角度を出してくれる
	float fRad = atan2f(power.y, power.x);			//	ラジアン	(πの方)
	//360度に変換してくれる(最終的なゴール)
	float fAngle = XMConvertToDegrees(fRad);		//	°		(360°の方)

	//	オブジェクト生成
	Object* pObjBlock = new Object(WIND_NAME, UPDATE_EFFECT, DRAW_EFFECT);
	//	コンポーネントの追加
	auto TransformBlock = pObjBlock->AddComponent<CTransform>();	//	トランスフォーム
	auto DrawBlock = pObjBlock->AddComponent<CDrawEffect>();		//	エフェクト描画
	auto CollisionBlock = pObjBlock->AddComponent<CCollider>();		//	当たり判定
	auto Wind = pObjBlock->AddComponent<CWind>();					//	風
	//	オブジェクトの設定
	TransformBlock->SetPosition(worldPos.x, worldPos.y);			//	座標
	DrawBlock->SetSize(WindSize.x, WindSize.y);						//	サイズ
	DrawBlock->SetTexture(pTex->GetTexture(WIND_TEX_NUM));			//	テクスチャ
	DrawBlock->SetAnimSplit(WIND_SPLIT_X, WIND_SPLIT_Y);			//	分割数
	DrawBlock->SetSwapFrame(SWAP_FRAME);							//	アニメーション切り替え
	CollisionBlock->SetCollisionSize(WindSize.x, WindSize.y);		//	当たり判定の設定
	TransformBlock->SetRotate(0.0f, 0.0f, fAngle);					//	回転
	Wind->SetWindVector(power);										//	風の力(単位ベクトルの力)
	//	リストに追加
	ObjectManager::GetInstance()->AddObject(pObjBlock);
}


//	平方根(正確ではない)
float squareRoot(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2乗関数
float pow(float x)
{
	return x * x;
}


// 風ゲージの設定
void WindManager::SetWindGauge(CGauge* pGauge)
{
	m_pWindGauge = pGauge;
}

// 風ゲージの取得
int WindManager::GetWindGauge()
{
	if (m_pWindGauge)	return CGauge::GetCurrentGauge(m_pWindGauge);
	else				return 0;
}

//2022/1/21/Tutorial Shimizu Yosuke ----------------------------

//　メニューが開けるかどうかのフラグを送る
bool WindManager::GetTutorial() {
	return m_bTutorialFlg;
}

//　メニューが開けるかどうかをセットで決める
void WindManager::SetTutorial(bool bTutorial) {
	m_bTutorialFlg = bTutorial;
}

//---------------------------------------------------------------


//2022/1/26/Touch_Tutorial Shimizu Yosuke -----------------------
/**
* @fn		SetSpawnWind
* @brief	風を生成できるようにするセッター
* @param	(bool) bSpawnWind 風を出せるかどうかの真偽
*/
void WindManager::SetSpawnWind(bool bSpawnWind) {
	m_bSpawnWind = bSpawnWind;
}
//---------------------------------------------------------------