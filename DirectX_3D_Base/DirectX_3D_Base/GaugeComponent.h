/**
 * @file Gauge.h
 * @brief 風の力の消費を検知するコンポーネント
 * @author 志水　陽祐
 * @date 2021/12/11
 */

//===== インクルードガード ======
#ifndef __GAUGE_H__
#define __GAUGE_H__

//===== インクルード =====
#include "Component.h"


////===== 前方宣言 =====
class CTransform;
class CDraw2D;


////===== クラス定義 =====
class CTransform;
class CDraw2D;

class CGauge : public Component
{
private:
	CTransform*		m_pTransform;		//	トランスフォームをいじれるように	
	CDraw2D*		m_pDraw;			//	描画をいじれるように

	XMFLOAT2        m_InitPosition;		//	ポジションの初期値(後で大きさを変更する際に使用)
	XMFLOAT2        m_MaxSize;			//	サイズの最大値(後で計算するために必要)
	int m_nMaxGauge;					//　	風のゲージのバーの長さを保存する変数
	bool m_bBarFlg;						//　	ゲージが無くなったらオンにする
	int m_nCurrentGauge;				//　	ゲージをゆっくり減らしていくための変数

protected:

public:
	CGauge();							//	コンストラクタ
	~CGauge();							//	デストラクタ
	void Start() override;				//	初期化
	void Update() override;				//	更新

	//	ゲージの大きさの最大値を設定する
	void SetMaxGauge(int max);
	//	ポジションをセットする　いらないんじゃないかな
	void SetInitPos(float fx,float fy);
	//	大きさをセットする
	void SetSize(float fx,float fy);

	// ゲージ量を設定する
	static void SetGauge(CGauge* pGauge, int value);
	//	ゲージを減らす
	static void CalcGauge(CGauge* pGauge, int value);
	//getter
	static int GetCurrentGauge(CGauge* pGauge);
};

#endif // __WIND_GAUGE_H__
