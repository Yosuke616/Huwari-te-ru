/**
 * @file	NumberCom.h
 * @brief	�����`��
 * @author	AT12A ���c���l
 * @date	2022/01/14
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __NUMBER_H__
#define __NUMBER_H__

//===== �C���N���[�h =====
#include "Component.h"
#include "Draw2dComponent.h"
#include <list>

//===== �O���錾 =====
class CDrawNumber : public CDraw2D
{
private:
	int m_nNum;
	int* m_pNum;
	std::list<int> m_nNumbers;	// ���l�𕪂���
	int m_nSpace;


public:
	CDrawNumber();
	~CDrawNumber() override;
	void Update() override;
	void Draw() override;

	void SetNum(int* num);
	void SetNum(int num);
};

#endif // __�@�\��_H__