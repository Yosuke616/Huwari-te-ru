/**
 * @file	AinmChangeCom.h
 * @brief	アニメーション再生のフレームを指定する
 * @author	AT12A 成田陸人
 * @date	2022/01/09
 */

 //===== インクルードガード ======
#ifndef __ANIM_CHANGE_H__
#define __ANIM_CHANGE_H__

//===== インクルード =====
#include "Component.h"
#include <unordered_map>

//===== 前方宣言 =====
class CDrawModel;


//===== クラス定義 =====
class Animation
{
	//=== メンバ変数
private:
	std::unordered_map<int, double>	m_dStart;	// アニメーションの始まりのフレーム数
	std::unordered_map<int, double>	m_dEnd;		// アニメーションの終わりのフレーム数
	std::unordered_map<int, double>	m_dPlaySpeed;// 再生速度
	std::unordered_map<int, bool>	m_bLoop;	// ループ再生
	std::unordered_map<int, int>	m_nOrder;	// アニメーションの優先度
	//std::unordered_map<int, bool>	m_bCall;	// Play関数が呼ばれているか
	std::unordered_map<int, bool>	m_bEnd;		// Play関数が呼ばれているか
	double		m_dCurrentAnimTime;				// 現在のアニメーション時間
	int			m_nCurrerntAnimState;			// 現愛のアニメーション状態
	bool		m_bIdle;						// ture ループが終われば0番目のアニメーション false アニメーションが終われば終了
	bool		m_bAnimation;					// 再生フラグ
	CDrawModel*	m_pDraw3D;						// モデル
public:

	//=== メンバ関数
public:
	void Play(int AnimState);			// 再生
	void Stop();						// 停止

	Animation();						// コンストラクタ
	~Animation();						// デストラクタ
	void Update();						// 更新
	void Draw();						// 描画
	//setter
	void SetModel(CDrawModel* model);

	// アニメーションの追加
	void AddAnimation(int AnimID, double start, double end, bool loop, double loopStartFrame);

	// ループ再生の設定
	void SetLoop(int AnimID, bool loop);
	// 再生速度の設定
	void SetPlaySpeed(int AnimID, double speed);
	// アニメーションが終わったかのフラグ
	bool AnimEnd(int AnimID);
	// 常に
	void AlwaysAnimation(bool always);
};
#endif // __ANIM_CHANGE_H__