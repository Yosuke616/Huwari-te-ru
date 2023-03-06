/**
 * @file	StageManager.h
 * @brief	ステージを管理するクラス
 * @author	成田 陸人
 * @date	2021/12/09 作成
 *			2021/12/26 csvによるマップ生成
 *					   csvの書き換えと保存
 *
 *
 */

 //===== インクルードガード ======
#ifndef __STAGE_MANAGER_H__
#define __STAGE_MANAGER_H__

//===== インクルード =====
#include "main.h"
#include "Object.h"
#include "Component.h"
#include "ResourceCSV.h"

#include <map>
#include <vector>

//===== 前方宣言 =====
class Object;




//===== クラス定義 =====
class StageManager
{
	//=== メンバ変数
public:
	static StageManager* m_pInstance;				//	インスタンス

	// vectorの2次元配列
	//using Cell = int;								//	セル
	//using Column = std::vector<Cell>;				//	行
	//using Row = std::vector<Column>;				//	列
	ResourceCSV::IntRow m_StageGrid;								//	ステージ配列

	Object* m_pShowObject;							//debug カーソルに表示するもの
	int m_ElementNumX;							//debug マウス座標の要素数
	int m_ElementNumY;							//debug マウス座標の要素数
	int m_nMap_Sta;									//debug 配置する物の種類

	int m_block_num_x;								//	ステージの要素数x
	int m_block_num_y;								//	ステージの要素数y
	XMFLOAT2 m_vStageSize;							//	ステージの大きさ
	XMFLOAT2	m_WorldSize;						//	ワールド座標で縦と横幅
	XMFLOAT2	m_BasePos;							//	ステージ描画の起点座標


	//=== メンバ関数
private:
	StageManager();									//	コンストラクタ
	bool Save(int Season);							//	ステージの書き込み
	bool Load(std::string file);					//	ステージの読み込み

	// クローバーに番号を割り振る
	void SetCloverNumber(std::list<Object*>& Clovers);

	// デバック更新
	void UpdateShowObject(float fPosX, float fPosY);
public:
	static StageManager* GetInstance();				//	インスタンス取得
	static void Destroy();							//	インスタンス破棄
	~StageManager();								//	デストラクタ
	void Update();									//	更新
	void Draw();									//	debug描画

	//	地形生成
	void CreateStage(int stage_state);

	// ステージオブジェクト生成(ゲームモード
	Object* CreateBlock(float PosX, float PosY, int state, int nBlockID);

	// ステージオブジェクト生成(編集モード
	//void CleateBlock_Deb(float PosX, float PosY, int number, int x, int y, int create_status);

};
#endif // __STAGE_MANAGER_H__
