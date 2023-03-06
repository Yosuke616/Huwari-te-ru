/**
 * @file	InputManager.h
 * @brief	�L�[�{�[�h���͂��Ǘ�����v���O����
 * @author	AT12A ���c���l
 * @date	���c	2021/11/17	�L�[�{�[�h�̃v���X�A�g���K�A�����[�X��Ԃ��擾����֐��̒ǉ�
 * 
 * 			���c 2021/11/20	�}�E�X�̃v���X�A�g���K�A�����[�X��Ԃ��擾����֐��̒ǉ�
 * 							�}�E�X���W���擾����֐��̒ǉ�
 * 							�}�E�X�̈ړ��ʂ��擾����֐��̒ǉ�
 * 
 * @detail	���͂̈����ɂ���
			�L�[�{�[�h		DIK_ + �����L�[	��j	DIK_1, DIK_RETURN�iENTER�L�[�j
			�}�E�X		MOUSE_ + (L�A R�A C)
 */


//===== �C���N���[�h�K�[�h ======
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

//===== �C���N���[�h =====
#include "main.h"
#define DIRECTINPUT_VERSION	0x0800	// DirectInput�̃o�[�W�����w��
#include <dinput.h>


//===== �}�N����` =====
#define KEY_MAX	(256)

#define INPUT (InputManager::Instance())

//=====	�񋓑� =====
enum MOUSE_BUTTON
{
	MOUSE_L,	//	���N���b�N
	MOUSE_R,	//	�E�N���b�N
	MOUSE_C		//	�z�C�[��
};

//===== �N���X��` =====
class InputManager
{
private:
	//=== �����o�ϐ�
	static InputManager* m_pInstance;			//	�C���X�^���X�p
	unsigned char m_CurrentKeyState[KEY_MAX];	//	���݂̃L�[�{�[�h���͏��̊i�[��
	unsigned char m_PrevKeyState[KEY_MAX];		//	�O�t���[���̃L�[�{�[�h���͏��̊i�[��
	DIMOUSESTATE m_CurrentMouseState;			//	�}�E�X�̌��݂̓��͏��
	DIMOUSESTATE m_PrevMouseState;				//	�}�E�X�̈�t���[���O�̓��͏��
	XMFLOAT2	 m_MousePos;					//	�}�E�X���W

	HRESULT InitKeyBoard();						//	�L�[�{�[�h������
	void UpdateKeyBoard();						//	�L�[�{�[�h�X�V
	HRESULT InitMouse();						//	�}�E�X������
	void UpdateMouse();							//	�}�E�X�X�V

protected:
	//=== �����o�֐�
	InputManager();								//	�R���X�g���N�^

public:
	static InputManager* Instance();			//	�C���X�^���X����
	~InputManager();							//	�f�X�g���N�^
	HRESULT Init();								//	������
	void Uninit();								//	�I��
	void Update();								//	�X�V
	void Draw();								//	�`��(debug�p)

	bool GetKeyPress(unsigned char   key);		//	�L�[�{�[�h�̃v���X��Ԃ��擾
	bool GetKeyTrigger(unsigned char key);		//	�L�[�{�[�h�̃g���K��Ԃ��擾
	bool GetKeyRelease(unsigned char key);		//	�L�[�{�[�h�̃����[�X��Ԃ��擾

	bool GetMouseButton( MOUSE_BUTTON Btn);		//	�}�E�X�̃v���X��Ԃ��擾
	bool GetMouseTrigger(MOUSE_BUTTON Btn);		//	�}�E�X�̃g���K��Ԃ��擾
	bool GetMouseRelease(MOUSE_BUTTON Btn);		//	�}�E�X�̃����[�X��Ԃ��擾

	XMFLOAT2 GetMousePos() const;				//	�}�E�X���W�̎擾
	XMFLOAT2 GetMouseVelocity() const;			//	�}�E�X�̈ړ��ʂ̎擾
};
#endif // __INPUT_MANAGER_H__

