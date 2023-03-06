//===== インクルードガード =====
#pragma once


//===== インクルード =====
#include <map>
#include "ResourceBase.h"


//===== マクロ定義 =====
// クリア状況
#define SAVE_DATA_CSV			("Asserts/save_data.csv")
#define COMPLETE				(16)						// コンプリートしてるかの情報がこの数字の列にセーブされる
// クローバー取得数
#define CLOVER_DATA_CSV			("Asserts/clover_data.csv")

// ステージ情報
#define SPRING_1_STAGE_CSV		("Asserts/spring_1.csv")
#define SPRING_2_STAGE_CSV		("Asserts/spring_2.csv")
#define SUMMER_1_STAGE_CSV		("Asserts/summer_1.csv")
#define SUMMER_2_STAGE_CSV		("Asserts/summer_2.csv")
#define FALL_1_STAGE_CSV		("Asserts/fall_1.csv")
#define FALL_2_STAGE_CSV		("Asserts/fall_2.csv")
#define WINTER_1_STAGE_CSV		("Asserts/winter_1.csv")
#define WINTER_2_STAGE_CSV		("Asserts/winter_2.csv")

#define SOUND_DATA_CSV			("Asserts/sound_data.csv")

//2022/1/28/Tutorial_CSV Shimizu Yosuke --------------------------
#define TUTORIAL_DATA_CSV		("Asserts/tutorial.csv")	//チュートリアルを一回出しているかどうかのデータ
//----------------------------------------------------------------

//===== クラス定義 =====
class FileManager
{
	//=== メンバ変数
private:
	//	各データの置き換え
	using ResourceMap = std::map<std::string, ResourceBase*>;
	using ResourceMapIt = ResourceMap::iterator;
	using ResourceMapPair = std::pair<std::string, ResourceBase*>;

	ResourceMap m_resources;					// リソースの一括管理
	static FileManager* m_pInstance;			// インスタンス

	//=== メンバ関数
private:
	FileManager();								// コンストラクタ
public:
	static FileManager* GetInstance();			// インスタンス取得
	static void Uninit();						// インスタンスの破棄
	~FileManager();								// デストラクタ
	bool Load(std::string fileName);			// ロード

	ResourceBase* Get(std::string fileName);	// データ取得
};

