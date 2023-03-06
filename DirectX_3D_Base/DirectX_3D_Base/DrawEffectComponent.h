/**
 * @file	DrawEffectComponent.h
 * @brief	エフェクト描画コンポーネント
 * @author	AT12A 成田陸人
 * @date	2021/12/06
 *			2021/12/17	エフェクトをループさせる機能の追加
 */

 //===== インクルードガード ======
#ifndef __EFFECT_H__
#define __EFFECT_H__

//===== インクルード =====
#include "DrawMeshComponent.h"


//===== 前方宣言 =====


//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CDrawEffect : public CDrawMesh
{
	//=== メンバ変数
private:
	
	int  m_nFrame;		//	アニメーション切り替えの感覚
	int	 m_nAnimCount;	//	アニメーションカウンタ
	bool m_bLoop;		//	アニメーションループフラグ

	//=== メンバ関数
private:
	void CreateEffectVertex();				//	頂点情報作成

public:
	CDrawEffect();							//	コンストラクタ
	~CDrawEffect() override;				//	デストラクタ
	void Start() override;					//	スタート
	void Update() override;					//	更新
	void Draw() override;					//	描画
	//setter
	void SetSwapFrame(int nCount);			//	アニメーションカウンタ
	void SetLoop(bool loop);

	// エフェクト生成
	static void StartEffect(ID3D11ShaderResourceView* pTex,
		XMFLOAT3 pos, XMFLOAT2 size, int split_x, int split_y
		, int swap_frame = 1
		, XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f));
};

#endif // __EFFECT_H__