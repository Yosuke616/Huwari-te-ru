/**
 * @file	CloverObjectComponent.h
 * @brief	UIオブジェクトの管理
 * @author	RYOHEI SAWADA
 * @date	2021/12/13
 */

 //===== インクルードガード ======
#ifndef __CloverComponent_H__
#define __CloverComponent_H__

//===== インクルード =====
#include"Component.h"

//===== 前方宣言 =====
class CloverComponent;
class CTransform;
class CCollider;
class Object;
class TextureManager;
class ObjectManager;
class CInputPlayer;

//===== マクロ定義 =====
//#define CLOVER_NUM_MAX	(5)
#define RATE_MOVE_CLOVER	(0.02f)

//2021/12/24 Shimizu Yosuke ---------------------------------------

//-----------------------------------------------------------------

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CloverComponent : public Component
{
	//=== メンバ変数
private:
	CTransform* m_pTransform;	//	トランスフォーム

	//2021/12/22 Shimizu Yosuke-----------------------------------
	int m_nNumber;					//マップチップの番号を保存する変数
	//------------------------------------------------------------

	CCollider* m_pCollider;
	XMFLOAT2 m_OldPos;

	bool m_bCloverflag;

	int m_nCnt_Clover;			//　クローバーの数を格納する変数

	bool m_bBind;				// クモの巣に引っかかってるフラグ

	//=== メンバ変数
public:
	CloverComponent();		//	コンストラクタ
	~CloverComponent();		//	デストラクタ
	void Start() override;	//	初期化
	void Update() override;	//	更新
	void Draw() override;	//	描画
	void OnCollisionEnter(Object* pObject) override;	//	衝突処理

	static Object* CreateClover(XMFLOAT3 pos, int id);

	//2021/12/23 Shimizu Yosuke---------------------------------
	void SetNumber(int number);
	int GetNumber();
	//----------------------------------------------------------

	void Binded(bool bBind);

	bool IsBinded();

};

#endif  __CloverComponent_H___

