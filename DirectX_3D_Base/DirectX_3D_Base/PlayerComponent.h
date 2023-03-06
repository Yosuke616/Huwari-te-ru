/**
 * @file	PlayerComponent.h
 * @brief	プレイヤーの入力操作のコンポーネント
 * @author	AT12A 成田陸人
 * @date	2021/12/06
 */

//===== インクルードガード ======
#ifndef __INPUT_PLAYER_H__
#define __INPUT_PLAYER_H__

//===== インクルード =====
#include "Component.h"
//2022/1/27/Touch_Tutorial ------------------------------
#include <list>
//-------------------------------------------------------

//===== 前方宣言 =====
class InputManager;
class CTransform;
class Object;
class CCollider;
class CDrawModel;


//===== クラス定義 =====
class CPlayer : public Component
{
private:
	CTransform* m_pPlayer;		//	座標、拡縮、回転
	CDrawModel*	m_pDraw3D;		// 描画情報
	CCollider*	m_Collider;
	 bool		m_bGoalFlg;		//	ゴールできるかのフラグ
	 bool		m_bGround;		//	地上フラグ

	 bool		m_bClover[4];	//	クローバーの所持数
	 XMFLOAT3	m_rotDestModel;	//	目的の向き
	 float m_fDiifRotY;			//	角速度の慣性

	 XMFLOAT2 m_OldPos;			// 過去座標
	 int m_nAnimState;			// アニメーション状態

	 //2022/01/11
	 int m_nColWindCnt;	// 風に当たってます

	 //2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------------------
	 bool m_bTouch_Tutorial;	//　このフラグがオンならばチュートリアルは出さない
	 //----------------------------------------------------------------------------
	 //2022/1/27Touch_Tutorial Shimizu Yosuke -------------------------------------
	 std::list<Object*> m_Delete_UI;			//UIを消すためのリスト
	 //----------------------------------------------------------------------------


public:
	CPlayer();											//	コンストラクタ
	~CPlayer() override;								//	デストラクタ
	void Start() override;								//	初期化
	void Update() override;								//	更新
	void Draw() override;								//	描画
	void OnCollisionEnter(Object* pObject) override;	//	衝突処理

	//setter
	void SetDestRot(float angle);
	void SetGround(bool ground);
	void SetAnimState(int state);
};
#endif // __COMPONENT_H__

