/**
 * @file	SceneGame.h
 * @brief	�Q�[���V�[��
 * @author	AT12A ���c���l
 * @date	2021/08/xx		�쐬
 */


//===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__


//===== �C���N���[�h =====
#include "Scene.h"
#include "main.h"


#include <vector>

#include "AssimpModel.h"
#include <unordered_map>


//===== �O���錾 =====
class WindManager;
class StageManager;


//===== �񋓑̐錾 =====
enum Season
{
	SPRING_1,		// �t�P
	SPRING_2,		// �t�Q
	SUMMER_1,		// �ĂP
	SUMMER_2,		// �ĂQ
	FALL_1,			// �H�P
	FALL_2,			// �H�Q
	WINTER_1,		// �~�P
	WINTER_2,		// �~�Q

	MAX_SEASON
};

//===== �N���X��` =====
class SceneGame : public Scene
{
	//=== �����o�ϐ�
private:
	static SceneGame* m_pInstance;		// �C���X�^���X
	std::vector<Scene*> m_GameSceneList;// �V�[�����X�g
	Scene*			m_CurrentScene;		// ���݂̃V�[��
	Season			m_eSeasonState;		// �G��

	Scene*			m_pInitScene;	// �������������V�[��
	
	WindManager*	m_pWindManager;		// ���}�l�[�W���[
	StageManager*	m_pStageManager;	// �X�e�[�W�}�l�[�W���[

	//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------------------
	bool m_bTutorial;					//�������I����������`���[�g���A����\������
	//-----------------------------------------------------------------

public:
	bool	m_bEditMode;				// �ҏW���[�h
	bool	m_bPauseMode;				// ���j���[���

	//=== �����o�֐�
private:
	SceneGame();						// �R���X�g���N�^
public:
	static SceneGame* GetInstance();	// �C���X�^���X�擾
	static void Destroy();				// �C���X�^���X�j��

	~SceneGame() override;				// �f�X�g���N�^
	void Init() override;				// ������
	void Uninit() override;				// �I������
	void Update() override;				// �X�V
	void Draw() override;				// �`��

	void SetSeason(Season NextSeason);	// �V�[���؂�ւ�
	Season GetSeason();					// �V�[�����̎擾


	// ���[�u
	static std::unordered_map<CAssimpModel*, bool> IceManager;	// �X�̐�
	//static std::vector

	//2022/1/26/Touch_Tutorial Shimizu Yosuke --------------------------------
	void SetTutorial(bool bTutorial);
	//------------------------------------------------------------------------

};
#endif // __SCENE_GAME_H__

