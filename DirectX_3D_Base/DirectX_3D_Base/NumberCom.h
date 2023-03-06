/**
 * @file	NumberCom.h
 * @brief	数字描画
 * @author	AT12A 成田陸人
 * @date	2022/01/14
 */

 //===== インクルードガード ======
#ifndef __NUMBER_H__
#define __NUMBER_H__

//===== インクルード =====
#include "Component.h"
#include "Draw2dComponent.h"
#include <list>

//===== 前方宣言 =====
class CDrawNumber : public CDraw2D
{
private:
	int m_nNum;
	int* m_pNum;
	std::list<int> m_nNumbers;	// 数値を分ける
	int m_nSpace;


public:
	CDrawNumber();
	~CDrawNumber() override;
	void Update() override;
	void Draw() override;

	void SetNum(int* num);
	void SetNum(int num);
};

#endif // __機能名_H__