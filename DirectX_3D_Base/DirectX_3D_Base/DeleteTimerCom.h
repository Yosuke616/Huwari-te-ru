/**
 * @file	ファイル名.h
 * @brief	簡単な説明
 * @author	書いた人
 * @date	日付（開始日？）
 */

 //===== インクルードガード ======
#ifndef __DELETE_H__
#define __DELETE_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
enum E_FUNCTION
{
	FUNC_DELETE = 0,
	FUNC_CREATE_CLOVER,
};

//===== クラス定義 =====
class CTimer : public Component
{
private:
	int m_nTime;
	int m_nFuncState;
public:
	CTimer();
	~CTimer() override;
	void Update() override;
	//setter
	void SetTime(int time);
	void SetFunction(E_FUNCTION time);
	//getter
	int GetFunktion();

};

#endif // __機能名_H__