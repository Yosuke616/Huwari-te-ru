/**
 * @file	SceneSpring.h
 * @brief	�t�̃V�[��
 * @author	AT12A ���c���l
 * @date	2021/12/21
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __SPRING_H__
#define __SPRING_H__


//===== �C���N���[�h =====
#include "Scene.h"
#include "main.h"


//===== �N���X��` =====
class SceneSpring : public Scene
{
	//=== �����o�֐�
public:
	SceneSpring();				//	�R���X�g���N�^
	~SceneSpring() override;	//	�f�X�g���N�^
	void Init() override;		//	������
	void Uninit() override;		//	�I������
	void Update() override;		//	�X�V
	void Draw() override;		//	�`��
};
#endif // __SPRING_H__

