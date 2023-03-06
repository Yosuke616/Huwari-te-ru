/**
 * @file	TutorialComponent.h
 * @brief	風に当たった時にメニューに行けるようにするコンポーネント
 * @author	志水　陽祐
 * @date	2022/1/19
 */

 //===== インクルードガード ======
#ifndef __TUTORIALCOMPONENT_H__
#define __TUTORIALCOMPONENT_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class WindManager;
//2022/1/31/Load_Tutorial Shimizu Yosuke -----------------------
class CDraw2D;
//--------------------------------------------------------------

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CTutorial : public Component
{
private:
	WindManager* m_pWindManager;		//マネージャーを呼び出す用の変数
	//2022/1/31/Load_Tutorial Shimizu Yosuke ---------------------------------
	CDraw2D* m_pDraw2D;					//色をセットするための関数
	//------------------------------------------------------------------------

public:
	CTutorial();
	~CTutorial() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Uninit();
	void OnCollisionEnter(Object* pObject) override;


};

#endif // __機能名_H__

