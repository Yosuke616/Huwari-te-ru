/**
 * @file	EffekseerManager.h
 * @brief	エフェクシアのエフェクトを管理
 * @author	志水翔伍
 * @date	2021/12/18		作成
 */

 //===== インクルードガード ======
#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

//===== インクルード =====
#include <unordered_map>
#include <vector>
#include "main.h"

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <Effekseer.Modules.h>
#include <Effekseer.SIMD.h>

#if _DEBUG

#pragma comment(lib, "Effekseer_2017_d.lib")
#pragma comment(lib, "Effekseer_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_d.lib")

#else

#pragma comment(lib, "Effekseer_2017.lib")
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")

#endif // _DEBUG


//===== 前方宣言 =====

//===== マクロ定義 =====

// カーソルのエフェクト
#define CURSOR_EFFECT_NUM			(0)
#define PATH_CURSOR_EFFECT			(u"data/Effect/sampleCursor002.efk")
#define CURSOR_EFFECT_SPRING_NUM	(1)
#define PATH_CURSOR_EFFECT_SPRING	(u"data/Effect/sampleCursor_SPRING002.efk")
#define CURSOR_EFFECT_SUMMER_NUM	(2)
#define PATH_CURSOR_EFFECT_SUMMER	(u"data/Effect/sampleCursor_SUMMER002.efk")
#define CURSOR_EFFECT_AUTUMN_NUM	(3)
#define PATH_CURSOR_EFFECT_AUTUMN	(u"data/Effect/sampleCursor_AUTUMN002.efk")
#define CURSOR_EFFECT_WINTER_NUM	(4)
#define PATH_CURSOR_EFFECT_WINTER	(u"data/Effect/sampleCursor_WINTER002.efk")
#define CURSOR_POS_Z				(-10.0f)	// 手前に表示するため

#define ICICLE_EFFECT_NUM			(5)
#define PATH_EFFECT_ICICLE			(u"data/Effect/sampleTsurara.efk")

#define CLOVER_GOAL_EFFECT_NUM		(6)
#define PATH_CLOVER_GOAL_EFFECT		(u"data/Effect/sampleGoal.efk")

#define SIMPLE_GOAL_EFFECT_NUM		(7)
#define PATH_SIMPLE_GOAL_EFFECT		(u"data/Effect/simpleGoal.efk")
// アイテム獲得エフェクト
#define GET_ITEM_EFFECT_NUM			(8)
#define PATH_GET_ITEM_EFFECT		(u"data/Effect/GetItem.efk")

//===== 列挙体宣言 =====


//===== クラス定義 =====
class EffectManager
{
	//=== メンバ変数
private:
	Effekseer::ManagerRef m_manager;								// エフェクト管理
	EffekseerRendererDX11::RendererRef m_renderer;					// レンダリング
	std::unordered_map<int, Effekseer::EffectRef> 	m_EffectList; 	// エフェクトリスト
	std::unordered_map<int, Effekseer::Handle>		m_handle;		// ハンドル
	static EffectManager* m_pInstance;								// このクラスのインスタンス

	//=== メンバ関数
protected:
	EffectManager();							// コンストラクタ
public:
	~EffectManager();							// デストラクタ
	static EffectManager* GetInstance(void);	// インスタンス化
	static void Destroy();						// インスタンス破棄
	void Start();								// 初期化
	void Uninit();								// エフェクトマネージャー解放
	void Update();								// 更新
	void Draw();								// 描画

	// エフェクトの追加(エフェクトを追加して番号を割り振る)
	Effekseer::EffectRef AddEffect(const char16_t* pszFilname, int effectID);

	//	エフェクト取得
	Effekseer::EffectRef GetEffect(int effectID);

	// エフェクト再生
	void Play(int effectID, XMFLOAT3 pos, XMFLOAT3 size);

	/* setter */
	// 座標
	void SetPosition(int effectID, XMFLOAT3 pos);

	// 回転
	void SetRotation(int effectID, XMFLOAT3 rot);

	// 拡大縮小
	void SetScale(int effectID, XMFLOAT3 scale);

	// 指定したエフェクトのパーティクルを一時停止、もしくは再開する。
	void SetPaused(int effectID, bool pause);

};
#endif // __EFFECT_MANAGER_H__