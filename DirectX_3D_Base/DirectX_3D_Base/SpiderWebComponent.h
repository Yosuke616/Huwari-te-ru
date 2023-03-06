/**
 * @file	SpiderWebComponent.h
 * @brief	「クモの巣」ギミックの実装
 * @author	AT12A 鈴村朋也
 * @date	2021/12/25 作成
 */

 //===== インクルードガード ======
#ifndef __SPIDER_H__
#define __SPIDER_H__

//===== インクルード =====
#include "Component.h"
#include <map>

//===== 前方宣言 =====
class CTransform;
class CCollider;

//===== マクロ定義 =====
#define WIND_ATTACK_NUM		(200)	//拘束を解除する際に必要な風の数

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CSpiderWeb : public Component
{
private:
	CTransform	*m_pTransform;		// クモの巣のトランスフォーム情報
	CCollider	*m_pCollider;		// クモの巣そのものの当たり判定
	XMFLOAT2 m_UpColliderPos;		// クモの巣上部分の当たり判定座標
	XMFLOAT2 m_DownColliderPos;		// クモの巣下部分の当たり判定座標

	XMFLOAT2 m_BindSize;			// 拘束範囲

	bool m_bUpColFlg;				// 上部分が風に飛ばされたかフラグ
	bool m_bDownColFlg;				// 下部分　…

	std::map<Object*, XMFLOAT2>	m_BindObjects;
	std::map<Object*, bool>		m_BindFlags;

	//std::map<Object*, bool>		m_bBindFlags;
	//std::list<Object*> m_pBindObject;
	//Object*		m_pBindObject;		//拘束したオブジェクトのトランスフォーム情報
	//XMFLOAT2 m_BindPlayerPos;		//プレイヤーオブジェクト拘束座標
	//std::list<XMFLOAT2> m_BindPos;	//その他接触オブジェクト拘束座標
	//bool m_bPlayerBindFlg;			//プレイヤーオブジェクトの拘束フラグ
	//bool m_bBindFlg;				//その他接触オブジェクトの拘束フラグ

	int m_nWindCnt;
	int nCnt;

public:
	CSpiderWeb();
	~CSpiderWeb() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	void SetBindSize(float, float);	//拘束範囲		
};
#endif // __SPIDER_H__
