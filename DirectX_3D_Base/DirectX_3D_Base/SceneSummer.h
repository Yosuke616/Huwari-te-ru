/**
 * @file	SceneSummer.h
 * @brief	�ẴV�[��
 * @author	AT12A ���c���l
 * @date	2021/12/21
 */


//===== �C���N���[�h�K�[�h ======
#ifndef __SUMMER_H__
#define __SUMMER_H__


//===== �C���N���[�h =====
#include "Scene.h"
#include "main.h"


//===== �N���X��` =====
class SceneSummer : public Scene
{
	//=== �����o�֐�
public:
	SceneSummer();				//	�R���X�g���N�^
	~SceneSummer() override;	//	�f�X�g���N�^
	void Init() override;		//	������
	void Uninit() override;		//	�I������
	void Update() override;		//	�X�V
	void Draw() override;		//	�`��
};
#endif // __SUMMER_H__

