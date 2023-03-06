/**
 * @file	Draw3dComponent.h
 * @brief	3D描画コンポーネント
 * @author	RIKUTO NARITA
 * @date	2021/11/26
 */

 //===== インクルードガード ======
#ifndef __DRAW_3D_H__
#define __DRAW_3D_H__

//===== インクルード =====
#include "Component.h"
#include "AssimpModel.h"
#include "main.h"

//===== 前方宣言 =====
class CTransform;
class Animation;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CDrawModel : public Component
{
	//=== メンバ関数 ===
private:
	CAssimpModel*	m_model;		//	モデル
	XMFLOAT4X4		m_mtxWorld;		//	ワールドマトリックス
	CTransform*		m_pTransform;	//	オブジェクトの座標
	Animation*		m_pAnim;			//	アニメーション
	TAssimpMaterial* m_pMaterial;	//	マテリアル情報

	//=== メンバ関数 ===
public:
	CDrawModel();						//	コンストラクタ
	~CDrawModel() override;			//	デストラクタ
	void Start() override;			//	スタート
	void Update() override;			//	更新
	void Draw() override;			//	描画
	//setter
	void SetModel(CAssimpModel* model);					// モデル
	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);	// プリミティブ設定
	void SetTexture(ID3D11ShaderResourceView* pTexture);// モデルのテクスチャ
	void SetAnimTime(double dTime);						// アニメーション時間

	double GetAnimDuration();

	//getter
	XMFLOAT4X4* GetWorldPos();							// ワールド行列の取得
	XMFLOAT3 GetBox();									// モデルの大きさ

	CAssimpModel* GetModel();

	//2022/01/09 animation
	void AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame = -1);
	void Play(int AnimID);							// アニメーション再生
	void AlwaysAnimation(bool always);				// 常にアニメーション
	void SetPlaySpeed(int AnimID, double speed);	// 再生速度
	bool AnimEnd(int AnimID);
};
#endif // __機能名_H__