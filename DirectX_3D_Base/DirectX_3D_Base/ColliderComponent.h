/**
 * @file	ColliderComponent.h
 * @brief	このコンポーネントを持つオブジェクト同士が当たっているか判断する
 * @author	AT12A 成田陸人
 * @date	2021/07/xx	作成
 *
 * @detail	必須の設定項目
 *			あたり判定の大きさ	SetCollisionSize(float fx, float fy, float fz = 0.0f)
 *	
 */

//===== インクルードガード =====
#ifndef __COLLIDER_H__
#define __COLLIDER_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

#include <vector>

//===== 前方宣言 =====
class CTransform;
class ObjectManager;
class Object;

//===== 構造体定義 =====

//	あたり判定のタイプ
//enum COLLISION_TYPE
//{
//};

//===== マクロ定義 =====


//===== クラス定義 =====
class CCollider : public Component
{
private:
	//=== メンバ変数
	CTransform*		m_pTransform;				//	座標
	XMFLOAT3		m_CollisionSize;			//	あたり判定の大きさ
	XMFLOAT2		m_CenterOffset;				//	モデルの中心のずれ
	float			m_fRadius;					//	半径
	static std::list<Object*> m_CollidrList;	//	あたり判定リスト

	//2021/01/06
	std::list<Object*> m_IntersectObjects;	//	衝突したオブジェクトたち

	//=== メンバ関数
public:
	CCollider();								//	コンストラクタ
	~CCollider() override;						//	デストラクタ
	void Start() override;						//	初期化
	void Update() override;						//	更新
	//setter
	void SetCollisionSize(float fx, float fy, float fz = 0.0f);	// あたり判定の大きさの設定
	void SetOffset(float fx, float fy);							// あたり判定の中心とオブジェクトの中心とのずれ
	//getter
	XMFLOAT2 GetOffSet();										// オブジェクトの中心とのずれ
	XMFLOAT3 GetColliderSize();									// あたり判定の大きさ
	XMFLOAT2 GetCenterPos();									// あたり判定の中心

	static void AddColliderList(Object* pObject);				// あたり判定リストに追加
	static std::list<Object*>& GetColliderList();				// あたり判定を持っているオブジェクトたち
	static void UpdateColliders();								// あたり判定を持っているオブジェクトたちの更新
	
private:
	//--- あたり判定
	//	矩形同士の当たり判定
	bool CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
		XMFLOAT2 size1, XMFLOAT2 size2, XMFLOAT2 Velocity1, XMFLOAT2 Velocity2);
	//	円と円
	bool CheckCollisionCircleToCircle(XMFLOAT2, XMFLOAT2, float, float);
	//	球と球
	bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
	//	直方体と直方体
	bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	//	xy平面のあたり判定
	bool CollisionRectToRectXY(CTransform* pA, CTransform* pB
		, XMFLOAT2 Asize, XMFLOAT2 Bsize
		, XMFLOAT2 Aoffset = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 Boffset = XMFLOAT2(0.0f, 0.0f));
};
#endif // __COLLIDER_H__
