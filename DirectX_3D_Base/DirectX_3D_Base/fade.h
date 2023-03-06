/**
 * @file	fade.h
 * @brief	フェードイン/アウト
 * @author	AT12A 成田陸人
 * @date	2021/07/xx
 */

//===== インクルードガード ======
#ifndef __FADE_H__
#define __FADE_H__

//===== インクルード =====
#include "SceneManager.h"

//===== 列挙体宣言 =====
enum EFade			// フェード状態
{
	FADE_NONE = 0,				// 何もしていない
	FADE_IN,					// フェードイン処理中
	FADE_OUT_SCENE_CHANGE,		// フェードアウト処理中(シーン切り替え)
	MAX_FADE
};

//===== プロトタイプ宣言 =====
void InitFade();					// 初期化
void UninitFade();					// 終了処理
void UpdateFade();					// 更新
void DrawFade();					// 描画
void StartFadeOutSceneChange(EScene eNext, int deley = 0);	// フェードアウト開始(deley)

EFade GetFade();					// 状態取得
void SetFadeColor(float fR, float fG, float fB); // フェードアウト色設定
#endif // __FADE_H__

