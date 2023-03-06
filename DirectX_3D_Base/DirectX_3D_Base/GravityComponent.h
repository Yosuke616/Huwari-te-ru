//=============================================================================
//
//  コンポーネントの基底クラス[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== インクルードガード ======
#ifndef __GRAVITY_H__
#define __GRAVITY_H__

//===== インクルード =====
#include "main.h"
#include "Component.h"

//===== マクロ定義 =====
#define GRAVITY		(0.03f)

//===== 前方宣言 =====
class CTransform;

//===== 列挙体宣言 =====


//===== クラス定義 =====
class CGravity : public Component
{
private:
	CTransform*	m_pTransform;	//	トランスフォーム

public:
	CGravity();					//	コンストラクタ
	~CGravity() override;		//	デストラクタ
	void Start() override;		//	初期化
	void Update() override;		//	更新

};
#endif // __GRAVITY_H__
