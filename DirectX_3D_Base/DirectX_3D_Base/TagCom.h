/**
 * @file	TagCom.h
 * @brief	簡単な説明
 * @author	書いた人
 * @date	日付（開始日？）
 */

 //===== インクルードガード ======
#ifndef __TAG_COMPONENT_H__
#define __TAG_COMPONENT_H__

//===== インクルード =====
#include "Component.h"
#include <map>

//===== 前方宣言 =====

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CTag : public Component
{
private:
	int m_nID;
public:
	CTag();
	~CTag() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;
	void SetID(int id)
	{
		m_nID = id;
	}

	static Object* Find(int id);
	static std::map<int, Object> m_pStageObject;
};

#endif // __TAG_COMPONENT_H__