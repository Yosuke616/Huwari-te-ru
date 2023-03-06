/**
 * @file	IceBlockCom.h
 * @brief	氷ブロックの処理
 * @author	AT12A 成田陸人
 * @date	日付（開始日？）
 */

 //===== インクルードガード ======
#ifndef __ICE_BLOCK_H__
#define __ICE_BLOCK_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CIceBlock : public Component
{
private:
	CTransform* m_pTransform;
	int m_nDirect;				// 進んでいる方向
	XMFLOAT2 m_OldPos;
public:
	CIceBlock();
	~CIceBlock() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	int GetDirect();

	int m_nCloverNumber;
	void SetNumber(int num);
	int GetNumber();
};

#endif // __機能名_H__