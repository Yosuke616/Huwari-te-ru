#define _CRT_SECURE_NO_WARNINGS

#include "ResourceCSV.h"
#include "FileManager.h"
#include <sstream>


// コンストラクタ
ResourceCSV::ResourceCSV(){
}

// デストラクタ
ResourceCSV::~ResourceCSV(){
}

// csvの読み込み
bool ResourceCSV::Load(std::string fileName)
{
	//	ファイルの読み込み
	FILE* fp;
	fp = fopen(fileName.c_str(), "rt");
	if (!fp)
	{
		//	読み込めなかった
		return false;
	}

	//	中身の吸い出し
	size_t fileSize;
	fseek(fp, 0, SEEK_END);		//	ファイル終端までポインタを移動
	fileSize = ftell(fp);		//	ファイルポインタが始点からどれだけ離れているか = サイズ
	fseek(fp, 0, SEEK_SET);		//	ファイルの先頭に移動
	std::string text;
	text.resize(fileSize);		//	vectorみたいに扱える???
	if(fileSize > 0)
		fread(&text.front(), fileSize, 1, fp);	//	textにtextの先頭からfileSize分の読み込みを１回する

	//	ファイルを閉じる
	fclose(fp);

	//--- csvの解析
	//	1行ずつ区切る
	std::vector<std::string> lines;
	int stsrtIndex = 0;				//	区切りの開始位置
	int endIndex = 0;				//	区切りの終了位置
	while (endIndex != std::string::npos)	// string::npos = findで見つからなかった時の値
	{
		//	'\n'(改行の文字記号が出てくる位置の探索
		endIndex = (int)text.find("\n", stsrtIndex);	//	見つからなかったらstring::nposの値を格納

		//	見つかってたら格納して読み込みますよ
		if (endIndex != std::string::npos)
		{
			//	探索開始位置から見つかった部分までの文字を切り出し
			lines.push_back(text.substr(stsrtIndex, endIndex - stsrtIndex));
		}

		//	次の探索開始位置の更新
		stsrtIndex = endIndex + 1;
	}

	//	1セルずつデータを格納
	std::vector<std::string>::iterator linesItr = lines.begin(); // stringのvector
	while (linesItr != lines.end())
	{
		Column column;	// 列

		stsrtIndex = 0;
		endIndex = 0;

		while (endIndex != std::string::npos)
		{
			//	','の位置を探索
			endIndex = (int)linesItr->find(",", stsrtIndex);	// その行の,を探す

			//	先ほど取り出した行を','で区切って取り出す
			if (endIndex != std::string::npos)
			{
				//	探索開始位置から見つかった部分までの文字を切り出し
				column.push_back(linesItr->substr(stsrtIndex, endIndex - stsrtIndex));
			}
			else
			{
				column.push_back(linesItr->substr(stsrtIndex));
			}

			//	次の探索開始位置の更新
			stsrtIndex = endIndex + 1;
		}

		//	セルごとに区切った行を全体のデータ(grid)へ追加
		m_grid.push_back(column);

		//	次の行
		linesItr++;
	}

	return true;
}

// csvに書き込み
bool ResourceCSV::Save(std::string fileName)
{
	//	書込み
	FILE *pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	書き込み先が
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;

	for (auto&& row : m_grid)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// セルの中身格納
			//fwrite(cell.c_str(), sizeof(char) * cell.size(), 1, pFile);
			fprintf(pFile, cell.c_str());
			//	csvはセルを","で区切る
			if ((int)row.size() != cnt_x)	// 最後はいらない
			{
				//fwrite(",", sizeof(char), 1, pFile);
				fprintf(pFile, ",");
			}
		}

		//	csvは"\n"で改行する
		if ((int)row.size() != cnt_y)	// 最後はいらない
		{
			//fwrite("\n", sizeof(char) * 2, 1, pFile);
			fprintf(pFile, "\n");
		}

		// カウント初期化
		cnt_x = 0;
	}

	fclose(pFile);

	return true;
}

//--- gettser
// セルの模擬を取得
std::string ResourceCSV::GetStr(int x, int y)
{
	return m_grid[y][x];
}
// セルの文字をint型で取得
int ResourceCSV::GetInt(int x, int y)
{
	//	atoi 文字列をintに変える
	return atoi(m_grid[y][x].c_str());
}
// セルの文字をfloat型で取得
float ResourceCSV::GetFloat(int x, int y)
{
	//	atof 文字列をfloatに変える
	return (float)atof(m_grid[y][x].c_str());
}
// 行の数を取得
int ResourceCSV::GetRowSize()
{
	return (int)m_grid.size();
}
// 列の数を取得
int ResourceCSV::GetColumnSize(int row)
{
	// 例外処理
	if ((int)m_grid.size() <= row) return 0;

	return (int)m_grid[row].size();
}

//--- setter
// セルに文字を入れる
void ResourceCSV::SetStr(int x, int y, std::string str)
{
	// 行のサイズ確認
	//if (y >= (int)m_grid.size()) return;
	//
	//// 列のサイズ確認
	//if (x >= (int)m_grid[y].size()) return;

	m_grid[y][x] = str;
}
// セルに整数を入れる
void ResourceCSV::SetInt(int x, int y, int num)
{
	// 足りない行は追加
	if (y >= (int)m_grid.size())
	{
		m_grid.resize(y + 1);
	}

	// 足りない列は追加
	if (x >= (int)m_grid[y].size())
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_grid[y].push_back(ss.str());
	}
	else
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_grid[y][x] = ss.str();
	}
}
// セルに浮動小数点数を入れる
void ResourceCSV::SetFloat(int x, int y, float num)
{
	// 足りない行は追加
	if (y >= (int)m_grid.size())
	{
		m_grid.resize(y + 1);
	}

	// 足りない列は追加
	if (x >= (int)m_grid[y].size())
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_grid[y].push_back(ss.str());
	}
	else
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_grid[y][x] = ss.str();
	}
}
// 行の数を設定
void ResourceCSV::SetRowSize(int size)
{
	m_grid.resize(size);
}
// 列の数を設定
void ResourceCSV::SetColumnSize(int row, int size)
{
	// 例外処理
	if ((int)m_grid.size() <= row) return;

	m_grid[row].resize(size);
}

// int → char変換してからcsvファイルに保存する
bool ResourceCSV::Save(std::string fileName, IntRow data)
{
	// 新しいデータと既存のデータのサイズが同じかどうか
	if (m_grid.size() != data.size() ||
		m_grid[0].size() != data[0].size())
	{
		return false;
	}

	//	書込み
	FILE* pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	書き込み先が
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;
	std::ostringstream ss;

	for (auto&& row : data)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// バッファをクリア
			ss.str("");
			// 状態をクリア
			ss.clear(std::stringstream::goodbit);
			// int → char変換
			ss << cell;

			// データの上書き
			m_grid[cnt_y - 1][cnt_x - 1] = ss.str().c_str();
			
			// セルの中身格納
			fprintf(pFile, ss.str().c_str());
			//	csvはセルを","で区切る
			if ((int)row.size() != cnt_x)	// 最後はいらない
			{
				fprintf(pFile, ",");
			}
		}

		//	csvは"\n"で改行する
		if ((int)row.size() != cnt_y)	// 最後はいらない
		{
			fprintf(pFile, "\n");
		}

		// カウント初期化
		cnt_x = 0;
	}

	fclose(pFile);

	// リソースの更新
	

	return true;
}