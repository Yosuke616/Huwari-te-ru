/**
 * @file	GetUIComponent.h
 * @brief	アイテムゲット時のUIコンポーネント
 * @author	AT12A 志水翔伍
 * @date	2022/01/29
 */

 //===== インクルードガード ======
#ifndef __GET_UI_H__
#define __GET_UI_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CGetUI : public Component
{
private:
	CTransform* m_pTransform;		// 座標
	CDraw2D* m_pDraw2D;				// 描画
	XMFLOAT2 m_vSize;				// サイズ
	XMFLOAT2 m_vMaxSize;			// 最大サイズ
	float m_fSizeRate;				// 拡大間隔

	int			m_nTimer;

public:
	CGetUI();
	~CGetUI() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	// Setter
	void SetUISize(XMFLOAT2 vSize);	// 大きさ設定
	void SetUIRate(float fRate);	// 拡大スピード設定

	void SetDelayTimer(int delay);
};

#endif // __GET_UI_H__