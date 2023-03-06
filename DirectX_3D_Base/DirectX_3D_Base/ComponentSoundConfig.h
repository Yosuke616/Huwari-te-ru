//=============================================================================
//
//  コンポーネントの基底クラス[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== インクルードガード ======
#ifndef __SOUND_CONFIG_H__
#define __SOUND_CONFIG_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CGauge;

//===== 列挙体 =====

enum SOUND_CONFIG
{
	// 設定タイプ
	CONFIG_MASTER,	// マスターボリューム
	CONFIG_BGM,		// BGMボリューム
	CONFIG_SE,		// SEボリューム

	MAX_CONFIG
};

//===== マクロ定義 =====


//#define SE_CONFIG_POS_X (STICK02_POS_X - ((STICK_WIDTH - CONFIG_STICK_WIDTH) / 2))

//===== 列挙体宣言 =====

//===== クラス定義 =====
class CSoundConfig : public Component
{
	//=== メンバ変数
private:
	// ボリュームの値
	static float m_fBGM_Volume;			// BGM
	static float m_fSE_Volume;			// SE

	CTransform* m_pTransform = nullptr; // このオブジェクトの座標
	CTransform* m_Cursor = nullptr;		// カーソル座標
	CGauge* m_pGauge;
	bool m_bProcessing;					//	操作フラグ

	SOUND_CONFIG m_eSoundType;			// サウンドの種類
	//bool m_bBGM;						//	true BGMの設定, false SEの設定

	//=== メンバ関数
public:
	CSoundConfig();						// コンストラクタ
	~CSoundConfig();					// デストラクタ
	void Start();						// 初期化
	void Update();						// 更新
	void OnCollisionEnter(Object*);		// 衝突処理

	// 音量設定タイプの設定
	void SetSoundType(SOUND_CONFIG type);

	// ゲージの設定
	void SetGauge(CGauge* pGauge);

};
#endif // __SOUND_CONFIG_H__

