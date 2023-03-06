/**
 * @file	FallLeafComponent.h
 * @brief	大きい葉の場合の重力
 * @author	内藤航己
 * @date	2021/12/07
 */

//===== インクルードガード ======
#ifndef __FALLLeaf_H__
#define __FALLLeaf_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====


//===== 前方宣言 =====
class CTransform;

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CFallLeaf : public Component
{
private:
	CTransform*	m_pTransform;		//	トランスフォーム
	int m_nDegree;					//　角度
	int m_nRandX;					//	横にずれるための乱数
	
	XMFLOAT3 m_RotVelocity;			//	回転速度

public:
	CFallLeaf();				//	コンストラクタ
	~CFallLeaf() override;	//	デストラクタ
	void Start() override;			//	初期化
	void Update() override;			//	更新
	void SetRotVelocity(float x, float y, float z);
};
#endif // __GRAVITY_H__
