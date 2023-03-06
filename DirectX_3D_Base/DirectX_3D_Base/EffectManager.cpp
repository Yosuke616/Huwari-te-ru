/**
 * @file	EffekseerManager.cpp
 * @brief	エフェクシアのエフェクトを管理
 * @author	志水翔伍
 * @date	2021/12/18		作成
 */

//====== インクルード =====
#include "EffectManager.h"
#include "Camera.h"


//====== マクロ定義 =====



//===== 静的メンバ変数 =====
EffectManager* EffectManager::m_pInstance = nullptr;


/**
 * @fn			EffectManager::EffectManager
 * @brief		コンストラクタ
*/
EffectManager::EffectManager(){
}

/**
 * @fn			EffectManager::~EffectManager
 * @brief		デストラクタ
*/
EffectManager::~EffectManager(){
}

/**
 * @fn			EffectManager::GetInstance
 * @brief		インスタンス生成
*/
EffectManager* EffectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager();
	}
	return m_pInstance;
}

/**
 * @fn			EffectManager::Destroy
 * @brief		インスタンス破棄
*/
void EffectManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
 * @fn			EffectManager::Start
 * @brief		Effekseer初期化
*/
void EffectManager::Start()
{
	// Effekseer初期化
	m_renderer = EffekseerRendererDX11::Renderer::Create(GetDevice(), GetDeviceContext(), 8000);
	m_manager = Effekseer::Manager::Create(8000);
	// 描画モジュールの設定
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

/**
 * @fn			EffectManager::Uninit
 * @brief		effekseer終了処理
*/
void EffectManager::Uninit()
{
	m_manager.Reset();
	m_renderer.Reset();
}

/**
 * @fn			EffectManager::Update
 * @brief		Effekseer更新
*/
void EffectManager::Update()
{
	CCamera* pCamera = CCamera::Get();

	// 投影行列を設定
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		pCamera->GetFOV(),
		pCamera->GetAspect(),
		pCamera->GetNearClip(),
		pCamera->GetFarClip()));
	
	// カメラ行列を設定
	DirectX::XMFLOAT3 cameraPos  = pCamera->GetPos();
	DirectX::XMFLOAT3 cameraLook = pCamera->GetLook();
	DirectX::XMFLOAT3 cameraUp   = pCamera->GetUp();
	Effekseer::Vector3D eye  = ::Effekseer::Vector3D(cameraPos.x,  cameraPos.y,  cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up   = ::Effekseer::Vector3D(cameraUp.x,   cameraUp.y,   cameraUp.z);
	m_renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

/**
 * @fn			EffectManager::Draw
 * @brief		描画
*/
void EffectManager::Draw()
{
	// Effekseer
	static int time = 0;
	m_manager->Update();					// 描画速度が変えられる
	m_renderer->SetTime(time++ / 60.0f);
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();

}


/**
 * @fn			EffectManager::AddEffect
 * @brief		エフェクトの追加
 * @param[in]	const char16_t*	ファイル名
 * @param[in]	effectID		エフェクト紐付けるキー
 * @return		成功時 インスタンスを返す, 失敗時 nullptr
*/
Effekseer::EffectRef EffectManager::AddEffect(const char16_t* pszFilname, int effectID)
{
	//*** すでにあったら返す
	if (m_EffectList[effectID] != nullptr) return m_EffectList[effectID];

	//*** mapに追加
	m_EffectList[effectID] = Effekseer::Effect::Create(m_manager, pszFilname);

	if (m_EffectList[effectID] == nullptr)
	{
		//*** 読み込まれなかった場合
		MessageBoxA(GetMainWnd(), "エフェクト読み込みエラー", "AddEffect", MB_OK);
		return nullptr;
	}

	// 追加したエフェクトのインスタンスを返す
	return m_EffectList[effectID];
}

/**
 * @fn			EffectManager::GetEffect
 * @brief		エフェクトのインスタンス取得
 * @param[in]	effectID		エフェクト紐付けるキー
 * @return		成功時 インスタンスを返す
*/
Effekseer::EffectRef EffectManager::GetEffect(int effectID)
{
	return m_EffectList[effectID];
}


/**
 * @fn			EffectManager::Play
 * @brief		エフェクト再生
 * @param[in]	effectID		エフェクトと紐付けるキー
 * @param[in]	pos				エフェクトの座標
 * @param[in]	scale			エフェクトの拡大率
*/
void EffectManager::Play(int effectID, XMFLOAT3 pos, XMFLOAT3 scale)
{
	if (m_EffectList[effectID] == nullptr) return;

	// エフェクト再生
	m_handle[effectID] = m_manager->Play(m_EffectList[effectID], pos.x, pos.y, pos.z);

	// スケール
	m_manager->SetScale(m_handle[effectID], scale.x, scale.y, scale.z);
}


/**
 * @fn			EffectManager::UpdatePosition
 * @brief		指定したエフェクトの座標の設定。
 * @param[in]	effectID		エフェクトと紐付けるキー
 * @param[in]	pos				エフェクトの座標
*/
void EffectManager::SetPosition(int effectID, XMFLOAT3 pos)
{
	m_manager->SetLocation(m_handle[effectID], pos.x, pos.y, pos.z);
}

/**
 * @fn			EffectManager::SetRotation
 * @brief		指定したエフェクトの回転の設定。
 * @param[in]	effectID		エフェクトと紐付けるキー
 * @param[in]	rot				エフェクトの座標
*/
void EffectManager::SetRotation(int effectID, XMFLOAT3 rot)
{
	m_manager->SetRotation(m_handle[effectID], rot.x, rot.y, rot.z);
}

/**
 * @fn			EffectManager::SetScale
 * @brief		指定したエフェクトの拡大縮小する。
 * @param[in]	effectID		エフェクトと紐付けるキー
 * @param[in]	rot				エフェクトの座標
*/
void EffectManager::SetScale(int effectID, XMFLOAT3 scale)
{
	m_manager->SetScale(m_handle[effectID], scale.x, scale.y, scale.z);
}

/**
 * @fn			EffectManager::SetPaused
 * @brief		指定したエフェクトのパーティクルを一時停止、もしくは再開する。
 * @param[in]	effectID		エフェクトと紐付けるキー
 * @param[in]	bool			true 一時停止, false 再開
*/
void EffectManager::SetPaused(int effectID, bool pause)
{
	m_manager->SetPaused(m_handle[effectID], pause);
}