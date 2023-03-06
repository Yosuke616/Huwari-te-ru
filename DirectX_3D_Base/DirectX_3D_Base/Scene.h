/*

	シーンの基底クラス[Scene.h]
	@Author : NH_AT12A	成田　陸人

	@更新履歴
	成田	2021/08/xx		作成

*/


//===== インクルードガード ======
#ifndef __SCENE_H__
#define __SCENE_H__

//===== インクルード ======
#include "ObjectManager.h"
#include "MenuManager.h"


//===== クラス定義 ======
class Scene
{
protected:
	//=== メンバ変数 ===
	ObjectManager*	m_pObjectManager;	// オブジェクト管理のポインタ
	MenuManager*	m_pMenuManager;		// メニュー管理のポインタ
	
public:
	//=== メンバ関数 ===
	Scene(){}							//	コンストラクタ
	virtual ~Scene(){}					//	(仮想関数)デストラクタ
	virtual void Init()	  = 0;			//	(純粋仮想関数)初期化
	virtual void Uninit() = 0;			//	(純粋仮想関数)終了
	virtual void Update() = 0;			//	(純粋仮想関数)更新
	virtual void Draw()	  = 0;			//	(純粋仮想関数)描画
};
#endif // __SCENE_H__
