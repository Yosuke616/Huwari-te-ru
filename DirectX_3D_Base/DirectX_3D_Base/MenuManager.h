/**�|�[�Y
 * @file	MouseManager.h
 * @brief	���j���[���Ǘ�����N���X�ɂ���
 * @author	SAWADA RYOHEI
 * @date	2021/12/22
 */

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

 //�C���N���[�h��
#include "main.h"
#include <list>

//�O���錾
class Object;

//�񋓑��錾
enum MENU_STATE
{
	STATE_NONE = 0,
	STATE_TITLE,
	STATE_OPTION,
	STATE_SELECT,
	STATE_GAMEOVER,
};

//�N���X��`
class MenuManager
{
	//=== �����o�ϐ�
private:
	static MenuManager* m_pInstance;	// �C���X�^���X�p
	std::list<Object*>	m_MenuList;		// ���j���[���X�g
	int m_nDelayCnt;					// �x������
	int m_nCreateState;					// �쐬���郁�j���[

	//2022/1/31/Load_Tutorial Shimizu Yosuke ----------------------------
	MENU_STATE m_OldMenu_State;			//�ߋ��̃V�[����ۑ�����ϐ�
	//-------------------------------------------------------------------


public:
	// �I�v�V�����t���O
	static bool m_bOption;

	//=== �����o�֐�
private:
	MenuManager();						// �R���X�g���N�^
public:
	~MenuManager();						// �f�X�g���N�^
	void Start();						// ������
	void Update();						// �X�V
	void Draw();						// �`��
	static MenuManager* GetInstance();	// �C���X�^���X����
	static void Destroy();				// �C���X�^���X�j��

	// ���j���[�쐬
	void Create(int state, int delayCnt = 1);

	// �^�C�g��
	void CreateTitleMenu();
	// �|�[�Y
	void CreatePauseMenu();
	// �X�e�[�W�Z���N�g
	void CreateSelectMenu();
	// �Q�[���I�[�o�[
	void CreateGameoverMenu();
	// �I�v�V�����쐬
	void CreateOptionMenu();
	// �I�v�V�����폜
	void DeleteOptionMenu();

	//2022/1/19/Tutorial Shimizu Yosuke -------------------------------------
	void CreateTutorialMenu();
	//-----------------------------------------------------------------------

	//2022/1/31/Load_Tutorial Shimizu  Yosuke --------------------------
	//���[�h�̂��߂ɃV�[�����Q�b�g����̂ƃZ�b�g����̂��Ђ悤
	MENU_STATE GetMenu();
	void SetMenu(MENU_STATE State_scene);
	//------------------------------------------------------------------


	// ���j���[���X�g�̍폜
	void DeleteMenu();

	// ���j���[�I�u�W�F�N�g�̒ǉ�
	void AddMenu(Object* pMenuObject);

	// ���j���[���X�g�̎擾
	std::list<Object*>& GetMenuList();
};
#endif	// _MENUMANAGER_H_
