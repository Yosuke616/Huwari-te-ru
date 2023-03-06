//===== �C���N���[�h =====
#include "InputManager.h"
#include <memory.h>
#include "imgui.h"

//===== ���C�u���������N =====
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//===== �O���[�o���ϐ� =====
LPDIRECTINPUT8		 lpDI			= nullptr;	//	DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8 lpKeyboard		= nullptr;	//	�L�[�{�[�h�f�o�C�X
LPDIRECTINPUTDEVICE8 lpMousewDevice = nullptr;	//	�}�E�X�f�o�C�X

//===== �ÓI�����o�ϐ� =====
InputManager* InputManager::m_pInstance = nullptr;

// �R���X�g���N�g
InputManager::InputManager()
{
	//	�����o�ϐ��̏�����
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));
	SecureZeroMemory(m_PrevKeyState, sizeof(m_PrevKeyState));
	m_MousePos = XMFLOAT2(0.0f, 0.0f);
}

// �f�X�g���N�^
InputManager::~InputManager()
{
}

// �C���X�^���X����
InputManager* INPUT
{
	if (!m_pInstance)
	{
		m_pInstance = new InputManager();
	}
	return m_pInstance;
}

// �I��
void InputManager::Uninit()
{
	//	�f�o�C�X�����~
	lpKeyboard->Unacquire();
	lpMousewDevice->Unacquire();

	//	�J��
	lpMousewDevice->Release();
	lpKeyboard->Release();
	lpDI->Release();

	//	�C���X�^���X�j��
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ������
HRESULT InputManager::Init()
{
	HRESULT ret = S_OK;

	// IDirectInput8�̍쐬(DirectInput8�̍\�z�ɐ��������ꍇ�ɂ��̃|�C���^�̓���̂�LPDIRECTINPUT8�̕ϐ��̃|�C���^)
	ret = DirectInput8Create(GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret)) {
		// �쐬�Ɏ��s
		return -1;
	}

	//	�L�[�{�[�h������
	ret = InitKeyBoard();
	if (FAILED(ret)) {
		return -1;
	}

	//	�}�E�X������
	ret = InitMouse();
	if (FAILED(ret)) {
		return -1;
	}

	return ret;
}
// �L�[�{�[�h������
HRESULT InputManager::InitKeyBoard()
{
	//	IDirectInputDevice8�̎擾
	HRESULT ret = lpDI->CreateDevice(
		GUID_SysKeyboard,	//��1�����͓��̓f�o�C�X��GUID�A�����ł̓f�t�H���g�̃V�X�e���L�[�{�[�h�Ƃ������Ƃ�GUID_SysKeyboard���`
		&lpKeyboard,		//��2�����ɂ͎擾�����f�o�C�X�I�u�W�F�N�g������ϐ��̃|�C���^
		NULL);
	if (FAILED(ret))
	{
		lpDI->Release();
		return -1;
	}

	//	���̓f�[�^�`���̃Z�b�g
	ret = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	//	�r������̃Z�b�g
	ret = lpKeyboard->SetCooperativeLevel(GetMainWnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {

		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	// ����J�n
	lpKeyboard->Acquire();

	return ret;
}
// �}�E�X������
HRESULT InputManager::InitMouse()
{
	//	�}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	HRESULT ret = lpDI->CreateDevice(
		GUID_SysMouse, 
		&lpMousewDevice, 
		NULL);
	if (FAILED(ret))
	{
		lpDI->Release();
		return -1;
	}

	//	���̓f�[�^�`���̃Z�b�g
	ret = lpMousewDevice->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret)) {// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return -1;
	}

	//	�r������̃Z�b�g
	/*
		DISCL_EXCLUSIVE		���̃A�v���P�[�V�����͂��̓��̓f�o�C�X���擾�ł��Ȃ�
		DISCL_NONEXCLUSIVE	���̃A�v���P�[�V�����ł����̂܂ܓ��̓f�o�C�X�̎擾���ł���

		DISCL_FOREGROUND	�E�B���h�E���o�b�N�O���E���h�Ɉړ�������f�o�C�X�̎擾���ł��Ȃ�
		DISCL_BACKGROUND	�E�B���h�E����A�N�e�B�u��Ԃł��f�o�C�X���擾�ł���

	*/
	ret = lpMousewDevice->SetCooperativeLevel(GetMainWnd(),
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(ret)) {
		return -1;
	}

	//	�f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = lpMousewDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}

	//	����J�n
	lpMousewDevice->Acquire();

	return ret;
}

// �X�V
void InputManager::Update()
{
	//	�L�[�{�[�h���͍X�V
	UpdateKeyBoard();

	//	�}�E�X�X�V
	UpdateMouse();
}
// �L�[�{�[�h�X�V
void InputManager::UpdateKeyBoard()
{
	//	�L�[���͏��̑ޔ�
	memcpy_s(m_PrevKeyState, KEY_MAX, m_CurrentKeyState, KEY_MAX);

	//	�L�[���͏��̃N���A
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));

	//	�L�[���͏��̎擾
	HRESULT ret = lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	if (FAILED(ret))	// ���s�Ȃ�ĊJ�����Ă�����x�擾
	{
		//	����ĊJ
		lpKeyboard->Acquire();

		//	�L�[���͏��̎擾
		lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	}

}
// �}�E�X�X�V
void InputManager::UpdateMouse()
{
	//	�}�E�X���̑ޔ�
	m_PrevMouseState = m_CurrentMouseState;

	//	�}�E�X���̎擾
	HRESULT	hr = lpMousewDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		//	����ĊJ
		lpMousewDevice->Acquire();

		//	�}�E�X�����Ď擾
		hr = lpMousewDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	//	�}�E�X���W���擾����
	POINT p;
	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(GetMainWnd(), &p);

	m_MousePos = XMFLOAT2((float)(p.x - SCREEN_CENTER_X), -(float)(p.y - SCREEN_CENTER_Y));
}

// �L�[�{�[�h�v���X
bool InputManager::GetKeyPress(unsigned char key)
{
	return (m_CurrentKeyState[key] & 0x80);
}
// �L�[�{�[�h�g���K
bool InputManager::GetKeyTrigger(unsigned char key)
{
	//	�O�t���[���ɉ�����Ă��Ȃ� && ���݉�����Ă���
	return (m_CurrentKeyState[key] & 0x80) &&
			!(m_PrevKeyState[key] & 0x80);
}
// �L�[�{�[�h�����[�X
bool InputManager::GetKeyRelease(unsigned char key)
{
	//	�O�t���[���ɉ�����Ă��� && ���݉�����Ă��Ȃ�
	return ((m_PrevKeyState[key] & 0x80) &&
			!(m_CurrentKeyState[key] & 0x80));
}

// �}�E�X�v���X
bool InputManager::GetMouseButton(MOUSE_BUTTON Btn)
{
	return (m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}
// �}�E�X�g���K
bool InputManager::GetMouseTrigger(MOUSE_BUTTON Btn)
{
	//	�O�t���[���ɃN���b�N����Ă��Ȃ� && ���݃N���b�N����Ă���
	return (!(m_PrevMouseState.rgbButtons[Btn] & 0x80) &&
			m_CurrentMouseState.rgbButtons[Btn] & 0x80);
	
}
// �}�E�X�����[�X
bool InputManager::GetMouseRelease(MOUSE_BUTTON Btn)
{
	//	�O�t���[���ɃN���b�N����Ă��� && ���݃N���b�N����Ă��Ȃ�
	return (m_PrevMouseState.rgbButtons[Btn] & 0x80 &&
			!(m_CurrentMouseState.rgbButtons[Btn] & 0x80));
}

// �}�E�X���W�̎擾
XMFLOAT2 InputManager::GetMousePos() const
{
	return m_MousePos;
}
// �}�E�X�̈ړ��ʂ̎擾
XMFLOAT2 InputManager::GetMouseVelocity() const
{
	return XMFLOAT2((float)m_CurrentMouseState.lX, (float)-m_CurrentMouseState.lY);
}

// �f�o�b�O�`��
void InputManager::Draw()
{
//#ifdef _DEBUG
//	using namespace ImGui;
//
//	//	�E�B���h�E�쐬
//	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	SetNextWindowPos(ImVec2(1000, 300), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	Begin("InputTest");
//
//	//	�L�[�{�[�h����
//	unsigned char TriggerTest1 = 0;
//	unsigned char ReleaseTest1 = 0;
//	if (GetKeyTrigger(DIK_RETURN))	TriggerTest1 = 0x80;
//	if (GetKeyRelease(DIK_RETURN))	ReleaseTest1 = 0x80;
//
//	//	�}�E�X���W
//	Text("POSSITION");
//	Text("X   : %.0f",	 m_MousePos.x);
//	Text("Y   : %.0f\n", m_MousePos.y);
//
//	//	�}�E�X�̈ړ���
//	XMFLOAT2 velocity = GetMouseVelocity();
//	Text("Velocity");
//	Text("X   : %.0f",	 velocity.x);
//	Text("Y   : %.0f\n", velocity.y);
//
//	End();
//
//#endif // _DEBUG
}