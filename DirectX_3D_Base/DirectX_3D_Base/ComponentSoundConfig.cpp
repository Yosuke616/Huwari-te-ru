//===== インクルード =====
#include "ComponentSoundConfig.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "ObjectInfo.h"
#include "GaugeComponent.h"
#include "ObjectManager.h"

#include "InputManager.h"
#include "Sound.h"

//=== 静的メンバ変数
float CSoundConfig::m_fBGM_Volume = 1.0f;
float CSoundConfig::m_fSE_Volume = 1.0f;

// コンストラクタ
CSoundConfig::CSoundConfig()
	: m_bProcessing(false)
	, m_pGauge(nullptr)
{
}

// デストラクタ
CSoundConfig::~CSoundConfig(){
}

// 初期化
void CSoundConfig::Start()
{
	// オブジェクト座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CSoundConfig::Update()
{
	// カーソルと当たっていたら
	if (m_bProcessing)
	{
		// X座標をカーソルと一緒にする
		m_pTransform->Pos.x = m_Cursor->Pos.x;

		// 座標の補正
		if (m_pTransform->Pos.x < GAUGE_LEFT)				// 左端
			m_pTransform->Pos.x = GAUGE_LEFT;
		if (m_pTransform->Pos.x > GAUGE_LEFT + STICK_WIDTH)	// 右端
			m_pTransform->Pos.x = GAUGE_LEFT + STICK_WIDTH;

		// 音量設定
		if (m_eSoundType == CONFIG_BGM)
		{
			/*
			ボリューム			0					 BGMボリューム					   1
							|------------------------+-------------------------|
			X座標			ゲージの左のX座標				現在X座標					ゲージの右のX座標
			*/
			// BGMボリューム = (現在X座標 - ゲージの左のX座標) ÷ ゲージの横幅
			m_fBGM_Volume = (m_pTransform->Pos.x - GAUGE_LEFT) / STICK_WIDTH;

			// 移動限界
			if (m_fBGM_Volume < 0.0f) m_fBGM_Volume = 0.0f;
			if (m_fBGM_Volume > 1.0f) m_fBGM_Volume = 1.0f;

			// BGMのボリューム設定
			CSound::SetVolume_BGM(m_fBGM_Volume);

			// ゲージ設定
			if (m_pGauge)
			{
				int value = (int)(m_fBGM_Volume * STICK_WIDTH);
				CGauge::SetGauge(m_pGauge, value);
			}
		}
		else if(m_eSoundType == CONFIG_SE)
		{
			/*
			ボリューム			0					 SEボリューム					   1
							|------------------------+-------------------------|
			X座標			ゲージの左のX座標				現在X座標					ゲージの右のX座標
			*/
			// SEボリューム = (現在X座標 - ゲージの左のX座標) ÷ ゲージの横幅
			m_fSE_Volume = (m_pTransform->Pos.x - GAUGE_LEFT) / STICK_WIDTH;

			// 移動限界
			if (m_fSE_Volume < 0.0f) m_fSE_Volume = 0.0f;
			if (m_fSE_Volume > 1.0f) m_fSE_Volume = 1.0f;

			// SEのボリューム設定
			CSound::SetVolume_SE(m_fSE_Volume);

			// ゲージ設定
			if (m_pGauge)
			{
				int value = (int)(m_fSE_Volume * STICK_WIDTH);
				CGauge::SetGauge(m_pGauge, value);
			}
		}

		//	ドラッグ終了時
		if (INPUT->GetMouseRelease(MOUSE_L))
		{
			// 操作終了
			m_bProcessing = false;

			// SEの設定の場合
			if(m_eSoundType == CONFIG_SE) CSound::Play(SE_DECIDE);	// 決定音を出す
		}
	}

	//	座標設定
	//Parent->GetComponent<CDraw2D>()->SetPosition(m_pTransform->Pos.x, m_pTransform->Pos.y);
}

// 衝突処理
void CSoundConfig::OnCollisionEnter(Object* pObject)
{
	// カーソルと衝突
	if (pObject->GetName() == CURSOR_NAME)
	{
		// 左クリックしていたら
		if (INPUT->GetMouseButton(MOUSE_L))
		{
			// カーソルの座標情報を抜き取る
			m_Cursor = pObject->GetComponent<CTransform>();

			// 音量操作可能
			m_bProcessing = true;
		}
	}
}

// 音量設定タイプの設定
void CSoundConfig::SetSoundType(SOUND_CONFIG type)
{
	m_eSoundType = type;
}

// ゲージの設定
void CSoundConfig::SetGauge(CGauge* pGauge)
{
	m_pGauge = pGauge;
}