/**
 * @file	OffScreenComponent.h
 * @brief	画面外処理
 * @author	AT12A 成田陸人
 * @date	2021/1/1
 */

 //===== インクルードガード ======
#ifndef __OFF_SCREEN_H__
#define __OFF_SCREEN_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CTransform;
class CCollider;


//===== 列挙体宣言 =====
enum OFF_SCREEN
{
	// 画面外処理内容
	OFF_SCREEN_DELETE,	// オブジェクトの削除
	ON_SCREEN,			// 画面内に入れる

	MAX_OFF_SCREEN
};


//===== クラス定義 =====
class COffScreen : public Component
{
	//=== メンバ変数
private:
	CTransform* m_pTransform;	// オブジェクトの座標
	CCollider* m_pCollider;		// あたり判定
	OFF_SCREEN m_eFunction;		// 画面外処理内容

	//=== メンバ関数
private:
	void Delete();				// 削除
	void OnScreen(int);			// 画面内に入れる
public:
	COffScreen();				// コンストラクタ
	~COffScreen() override;		// デストラクタ
	void Start() override;		// 初期化
	void Update() override;		// 更新
	void Draw() override;		// 描画
	//setter
	void SetFinction(OFF_SCREEN fnc);
};
#endif // __OFF_SCREEN_H__