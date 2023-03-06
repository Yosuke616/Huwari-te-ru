/**
 * @file	MouseManager.h
 * @brief	マウス座標を管理するクラスにする
 * @author	志水陽祐
 * @date	2021/12/08
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

//インクルード部
#include "main.h"
#include <list>

//前方宣言
class Object;
class CTransform;
class InputManager;
class CGauge;

//列挙隊宣言

//クラス定義
class WindManager
{
	//=== メンバ変数
private:
	static WindManager* m_pInstance;		//	インスタンス用
	CGauge* m_pWindGauge;					//	ゲージの値を入手できるよう

	std::list<XMFLOAT2> m_WindCenterList;	//　マウス座標を保存するリスト
	XMFLOAT2 m_P;							//　マウスの座標を入れる変数
	std::list<XMFLOAT2> m_WindVectorList;	//	風の力を保存する
	XMFLOAT2 m_WindPower;					//	座標間の差を保存する変数

	int m_nFrameCnt;						//  フレームを保存するための変数
	int m_nDeleteCnt;						//	消すためのカウント（必要ないかも）
	bool m_bDeleteFlg;						//	これがオンになったら風を作り始める

	// 2022/01/22 yousuke
	bool m_bTutorialFlg;

	// 2022/1/26/Touch_Tutorial Shinmizu Yosuke ----------------------------------------
	bool m_bSpawnWind;						//　このフラグがfalseなら風は出せない
	//----------------------------------------------------------------------------------

	//=== メンバ関数
protected:
	WindManager();							//	コンストラクタ
public:
	~WindManager();							//	デストラクタ
	void Update();							//	更新
	void Draw();							//	描画

	static WindManager* GetInstance();		//	インスタンス生成
	static void Destroy();					//	インスタンス破棄

	//2022/1/26/Touch_Tutorial Shimizu Yosuke ---------------
	//風を出せるようにするためのセッター
	void SetSpawnWind(bool bSpawnWind);
	//-------------------------------------------------------

	//	複数の風
	static void CreateWind(XMFLOAT2 pos, XMFLOAT2 vector);

	// 風ゲージの設定
	void SetWindGauge(CGauge* pGauge);

	// 風ゲージの取得
	int GetWindGauge();

	//2022/1/21/Tutorial Shimizu Yosuke ---------------------
	//　メニューに触れてよいかどうか
	void SetTutorial(bool bTutorial);
	bool GetTutorial();
	//-------------------------------------------------------
};
#endif