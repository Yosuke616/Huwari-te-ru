/*
	リソース・・・ゲーム内で使用する外部データ
	テクスチャ、モデル、サウンド、セーブデータ、ゲーム設定、
	コンフィグ、レベルデザイン、アイテム、敵の情報

*/
#ifndef __RESOURCE_CSV_H__
#define __RESOURCE_CSV_H__

// インクルード
#include <string>
#include <vector>
#include "ResourceBase.h"


// クラス定義
class ResourceCSV : public ResourceBase
{
public:
	// vectorの2次元配列
	using Cell = std::string;			// セル
	using Column = std::vector<Cell>;	// 行
	using Row = std::vector<Column>;	// 列

	using IntCell = int;
	using IntColumn = std::vector<int>;
	using IntRow = std::vector<IntColumn>;

	// 読み込んだcsvデータの格納先
	Row m_grid;

public:
	ResourceCSV();
	~ResourceCSV();

	bool Load(std::string fileName) override;	//	ロード
	bool Save(std::string fileName) override;	//	セーブ

	// int → char変換してからcsvファイルに保存する
	bool Save(std::string fileName, IntRow data);

	//getter
	std::string GetStr(int x, int y);
	int GetInt(int x, int y);
	float GetFloat(int x, int y);
	int GetRowSize();
	int GetColumnSize(int row);

	//setter
	void SetStr(int x, int y, std::string str);
	void SetInt(int x, int y, int num);
	void SetFloat(int x, int y, float num);
	void SetRowSize(int size);
	void SetColumnSize(int row, int size);
};
#endif // !__RESOURCE_CSV_H__
