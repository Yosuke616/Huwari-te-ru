/**
 * @file	SceneWinter.h
 * @brief	�~�̃V�[��
 * @author	AT12A ���c���l
 * @date	2021/12/21
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __WINTER_H__
#define __WINTER_H__


//===== �C���N���[�h =====
#include "Scene.h"
#include "main.h"


//===== �N���X��` =====
class SceneWinter : public Scene
{
	//=== �����o�֐�

public:
	SceneWinter();				//	�R���X�g���N�^
	~SceneWinter() override;		//	�f�X�g���N�^
	void Init() override;		//	������
	void Uninit() override;		//	�I������
	void Update() override;		//	�X�V
	void Draw() override;		//	�`��
};
#endif // __WINTER_H__

