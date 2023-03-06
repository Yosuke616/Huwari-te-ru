/*

	�V�[�����Ǘ�����v���O����[SceneManager.h]
	@Author : NH_AT12A	���c�@���l

	@�X�V����
	���c	2021/08/xx		�쐬
	���c	2021/11/19		�R�����g�ҏW

	@�V�[���ǉ��ɂ���
	�w�b�_�[��EScene�ɃV�[������o�^���AInit�֐����ŃV�[�����X�g�ɒǉ����鏈����������

*/


//===== �C���N���[�h�K�[�h =====
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Scene.h"

#include <vector>

//===== �񋓑̒�` =====
enum EScene
{
	SCENE_TITLE,		//	�^�C�g�����
	SCENE_GAME,			//	�Q�[�����
	SCENE_SELECT,		//	�Z���N�g���
	SCENE_ENDING,

	SCENE_MAX
};

//===== �N���X��` =====
class SceneManager
{
	//=== �����o�ϐ�
private:
	static SceneManager* m_pInstance;	//	�C���X�^���X�����p
	std::vector<Scene*>	 m_SceneList;	//	�V�[�����X�g
	Scene* m_CurrentScene;				//	���݂̃V�[��
	EScene m_eSceneState;				//	�V�[���̏��

	//=== �����o�֐�
protected:
	SceneManager();						//	�R���X�g���N�^
public:
	static SceneManager* Instance();	//	�C���X�^���X��
	static void Destroy();				//	�C���X�^���X�̍폜

	~SceneManager();					//	�f�X�g���N�^
	void Init();						//	������
	void Update();						//	�X�V
	void Draw();						//	�`��
	void Uninit();						//	�I��

	void SetScene(EScene NextScene);	//	�V�[���؂�ւ�
	EScene GetScene() const;			//	�V�[�����̎擾
};
#endif //__SCENE_MANAGER_H__