/*
	リソース・・・ゲーム内で使用する外部データ
	テクスチャ、モデル、サウンド、セーブデータ、ゲーム設定、
	コンフィグ、レベルデザイン、アイテム、敵の情報

*/
#ifndef __RESOURCE_BASE_H__
#define __RESOURCE_BASE_H__

//	インクルード
#include <string>

class ResourceBase
{
protected:
	std::string m_fileName;	// ファイル名
public:
	ResourceBase();			// コンストラクタ
	~ResourceBase();		// デストラクタ

	virtual bool Load(std::string fileName) = 0;	// ロード
	virtual bool Save(std::string fileName) = 0;	// セーブ
};
#endif // !__RESOURCE_BASE_H__
