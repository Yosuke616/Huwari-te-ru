/**
 * @file	FadeChangeComponent.h
 * @brief	時間経過でポリゴンを表示
 * @author	志水翔伍
 * @date	2022/01/06
 */

 //===== インクルードガード ======
#ifndef __FADE_CHANGE_H__
#define __FADE_CHANGE_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====
enum EChange
{
	CHANGE_NONE = 0,
	CHANGE_IN,
	CHANGE_OUT,

	MAX_CHANGE
};


//===== クラス定義 =====
class CFadeChange : public Component
{
private:
	CTransform* m_pTransform;	// トランスフォーム
	CDraw2D* m_pDraw2D;			// 2D描画
	int m_nTimer;				// 時間
	int m_nCmpTimer;
	float m_fAlpha;				// α値
	XMFLOAT3 m_fColor;			// 色
	bool m_bFlg;				// 切替フラグ

public:
	CFadeChange();
	~CFadeChange() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	void SetTimer(int nTimer);
};

#endif // __FADE_CHANGE_H__