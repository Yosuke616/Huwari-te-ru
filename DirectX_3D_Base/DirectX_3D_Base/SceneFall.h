/**
 * @file	SceneFall.h
 * @brief	�H�̃V�[��
 * @author	AT12A ���c���l
 * @date	2021/12/21
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __FALL_H__
#define __FALL_H__


//===== �C���N���[�h =====
#include "Scene.h"
#include "main.h"


//===== �N���X��` =====
class SceneFall : public Scene
{
	//=== �����o�֐�
public:
	SceneFall();				//	�R���X�g���N�^
	~SceneFall() override;		//	�f�X�g���N�^
	void Init() override;		//	������
	void Uninit() override;		//	�I������
	void Update() override;		//	�X�V
	void Draw() override;		//	�`��
};
#endif // __FALL_H__

