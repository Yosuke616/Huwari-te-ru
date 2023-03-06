//=============================================================================
//
//  コンポーネントの基底クラス[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== インクルードガード ======
#ifndef __EGRAVITY_H__
#define __EGRAVITY_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====
#define EGRAVITY			(0.01f)

//===== 前方宣言 =====
class CTransform;

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CEGravity : public Component
{
private:
	CTransform*	m_pTransform;	//	トランスフォーム

public:
	CEGravity();					//	コンストラクタ
	~CEGravity() override;		//	デストラクタ
	void Start() override;		//	初期化
	void Update() override;		//	更新

};
#endif // __EGRAVITY_H__
