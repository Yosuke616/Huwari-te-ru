/**
 * @file	MenuComponent.h
 * @brief	ポーズの機能
 * @author	SAWADA RYOHEI
 * @date	2021/12/21
 */

 //===== インクルードガード ======
#ifndef __MENUCOMPONENT_H__
#define __MENUCOMPONENT_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;
class CTransform;

//===== マクロ定義 =====


//===== 列挙体宣言 =====

//===== クラス定義 =====
class CMenu : public Component
{
private:
	bool m_bSelected;
	CDraw2D* m_pDraw2D;
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ------------------------
	CTransform* m_pTrans;				//回転させるための変数
	int m_nRotate;					//回転させる
	//----------------------------------------------------------------



public:
	CMenu();
	~CMenu() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

};

#endif // __MENUCOMPONENT_H__
