/*

	�V�[���̊��N���X[Scene.h]
	@Author : NH_AT12A	���c�@���l

	@�X�V����
	���c	2021/08/xx		�쐬

*/


//===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_H__
#define __SCENE_H__

//===== �C���N���[�h ======
#include "ObjectManager.h"
#include "MenuManager.h"


//===== �N���X��` ======
class Scene
{
protected:
	//=== �����o�ϐ� ===
	ObjectManager*	m_pObjectManager;	// �I�u�W�F�N�g�Ǘ��̃|�C���^
	MenuManager*	m_pMenuManager;		// ���j���[�Ǘ��̃|�C���^
	
public:
	//=== �����o�֐� ===
	Scene(){}							//	�R���X�g���N�^
	virtual ~Scene(){}					//	(���z�֐�)�f�X�g���N�^
	virtual void Init()	  = 0;			//	(�������z�֐�)������
	virtual void Uninit() = 0;			//	(�������z�֐�)�I��
	virtual void Update() = 0;			//	(�������z�֐�)�X�V
	virtual void Draw()	  = 0;			//	(�������z�֐�)�`��
};
#endif // __SCENE_H__
