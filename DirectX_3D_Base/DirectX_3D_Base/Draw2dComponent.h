/**
 * @file	Draw2dComponent.h
 * @brief	2D画面描画クラス
 * @author	AT12A 成田陸人
 * @date	2021\07/XX 作成
 */

//===== インクルードガード ======
#ifndef __DRAW_H__
#define __DRAW_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

class CTransform;

//===== クラス定義 =====
class CDraw2D : public Component
{
private:

	

protected:
	//=== メンバ変数 ===
	CTransform* m_pTransform;					//	座標
	ID3D11ShaderResourceView*	m_pTexture;		//	テクスチャ
	XMFLOAT2	m_fPolygonSize;					//	四角形ポリゴンの大きさ
	XMFLOAT2	m_fTexFrameSize;				//	テクスチャ割当範囲
	XMFLOAT4	m_fVertexColor;					//	頂点カラー
	float		m_fAngle;						//	角度
	XMFLOAT2	m_Scroll;						//	スクロール
	EBlendState m_BlendState;					//	ブレンドの種類
	int			m_nAnimNum;						//	アニメーション番号
	int			m_nAnimSplitX;					//	横のアニメーション分割数
	int			m_nAnimSplitY;					//	縦のアニメーション分割数

public:
	//=== メンバ関数 ===
	CDraw2D();									//	コンストラクタ
	~CDraw2D() override;						//	デストラクタ
	void Start() override;						//	開始
	void Update() override;						//	更新
	void Draw() override;						//	描画

	//setter
	void SetSize(float, float);					//	ポリゴンサイズ
	void SetUVsize(float, float);				//	UVサイズ
	void SetAnimSplit(int, int);				//	アニメーション分割数
	void SetAnimNum(int);						//	アニメーション番号
	void SetTexture(ID3D11ShaderResourceView*);	//	テクスチャ
	void SetColor(float r, float g, float b);	//	カラー
	void SetAlpha(float alpha);					//	アルファ値
	void SetAngle(float fAngle);				//	角度
	void SetScroll(XMFLOAT2 scroll);			//	スクロール量
	void SetBlend(EBlendState state);			//	ブレンドステート
	void SetU(float u);
	void SetV(float v);

	int GetAnimSplitX();
};

class CDrawEffect2D : public CDraw2D
{
private:
	int  m_nFrame;		//	アニメーション切り替えの感覚
	int	 m_nAnimCount;	//	アニメーションカウンタ
	bool m_bLoop;		//	アニメーションループフラグ
	bool m_bPlay;
public:
	CDrawEffect2D();
	~CDrawEffect2D();
	void Update() override;					//	更新
	//setter
	void SetSwapFrame(int nCount);			//	アニメーションカウンタ
	void SetLoop(bool loop);
	void SetPlayONOFF(bool onoff);			//アニメーション再生するかどうか

	// エフェクト生成
	static void StartEffect(ID3D11ShaderResourceView* pTex,
		XMFLOAT2 pos, XMFLOAT2 size, int split_x, int split_y
		, int swap_frame = 1
		, XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f));
};


#endif // __DRAW_H__
