/**
 * @file	�t�@�C����.h
 * @brief	�ȒP�Ȑ���
 * @author	�������l
 * @date	���t�i�J�n���H�j
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __DELETE_H__
#define __DELETE_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
enum E_FUNCTION
{
	FUNC_DELETE = 0,
	FUNC_CREATE_CLOVER,
};

//===== �N���X��` =====
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

#endif // __�@�\��_H__