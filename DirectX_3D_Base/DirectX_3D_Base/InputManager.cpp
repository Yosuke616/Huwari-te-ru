//===== インクルード =====
#include "InputManager.h"
#include <memory.h>
#include "imgui.h"

//===== ライブラリリンク =====
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//===== グローバル変数 =====
LPDIRECTINPUT8		 lpDI			= nullptr;	//	DirectInputオブジェクト
LPDIRECTINPUTDEVICE8 lpKeyboard		= nullptr;	//	キーボードデバイス
LPDIRECTINPUTDEVICE8 lpMousewDevice = nullptr;	//	マウスデバイス

//===== 静的メンバ変数 =====
InputManager* InputManager::m_pInstance = nullptr;

// コンストラクト
InputManager::InputManager()
{
	//	メンバ変数の初期化
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));
	SecureZeroMemory(m_PrevKeyState, sizeof(m_PrevKeyState));
	m_MousePos = XMFLOAT2(0.0f, 0.0f);
}

// デストラクタ
InputManager::~InputManager()
{
}

// インスタンス生成
InputManager* INPUT
{
	if (!m_pInstance)
	{
		m_pInstance = new InputManager();
	}
	return m_pInstance;
}

// 終了
void InputManager::Uninit()
{
	//	デバイス制御停止
	lpKeyboard->Unacquire();
	lpMousewDevice->Unacquire();

	//	開放
	lpMousewDevice->Release();
	lpKeyboard->Release();
	lpDI->Release();

	//	インスタンス破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// 初期化
HRESULT InputManager::Init()
{
	HRESULT ret = S_OK;

	// IDirectInput8の作成(DirectInput8の構築に成功した場合にそのポインタの入るのはLPDIRECTINPUT8の変数のポインタ)
	ret = DirectInput8Create(GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(ret)) {
		// 作成に失敗
		return -1;
	}

	//	キーボード初期化
	ret = InitKeyBoard();
	if (FAILED(ret)) {
		return -1;
	}

	//	マウス初期化
	ret = InitMouse();
	if (FAILED(ret)) {
		return -1;
	}

	return ret;
}
// キーボード初期化
HRESULT InputManager::InitKeyBoard()
{
	//	IDirectInputDevice8の取得
	HRESULT ret = lpDI->CreateDevice(
		GUID_SysKeyboard,	//第1引数は入力デバイスのGUID、ここではデフォルトのシステムキーボードということでGUID_SysKeyboardを定義
		&lpKeyboard,		//第2引数には取得したデバイスオブジェクトが入る変数のポインタ
		NULL);
	if (FAILED(ret))
	{
		lpDI->Release();
		return -1;
	}

	//	入力データ形式のセット
	ret = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(ret)) {
		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	//	排他制御のセット
	ret = lpKeyboard->SetCooperativeLevel(GetMainWnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(ret)) {

		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	// 動作開始
	lpKeyboard->Acquire();

	return ret;
}
// マウス初期化
HRESULT InputManager::InitMouse()
{
	//	マウス用にデバイスオブジェクトを作成
	HRESULT ret = lpDI->CreateDevice(
		GUID_SysMouse, 
		&lpMousewDevice, 
		NULL);
	if (FAILED(ret))
	{
		lpDI->Release();
		return -1;
	}

	//	入力データ形式のセット
	ret = lpMousewDevice->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {// データフォーマットに失敗
		return -1;
	}

	//	排他制御のセット
	/*
		DISCL_EXCLUSIVE		他のアプリケーションはその入力デバイスを取得できない
		DISCL_NONEXCLUSIVE	他のアプリケーションでもそのまま入力デバイスの取得ができる

		DISCL_FOREGROUND	ウィンドウがバックグラウンドに移動したらデバイスの取得ができない
		DISCL_BACKGROUND	ウィンドウが非アクティブ状態でもデバイスが取得できる

	*/
	ret = lpMousewDevice->SetCooperativeLevel(GetMainWnd(),
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(ret)) {
		return -1;
	}

	//	デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = lpMousewDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	//	動作開始
	lpMousewDevice->Acquire();

	return ret;
}

// 更新
void InputManager::Update()
{
	//	キーボード入力更新
	UpdateKeyBoard();

	//	マウス更新
	UpdateMouse();
}
// キーボード更新
void InputManager::UpdateKeyBoard()
{
	//	キー入力情報の退避
	memcpy_s(m_PrevKeyState, KEY_MAX, m_CurrentKeyState, KEY_MAX);

	//	キー入力情報のクリア
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));

	//	キー入力情報の取得
	HRESULT ret = lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	if (FAILED(ret))	// 失敗なら再開させてもう一度取得
	{
		//	動作再開
		lpKeyboard->Acquire();

		//	キー入力情報の取得
		lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	}

}
// マウス更新
void InputManager::UpdateMouse()
{
	//	マウス情報の退避
	m_PrevMouseState = m_CurrentMouseState;

	//	マウス情報の取得
	HRESULT	hr = lpMousewDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		//	動作再開
		lpMousewDevice->Acquire();

		//	マウス情報を再取得
		hr = lpMousewDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	//	マウス座標を取得する
	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(GetMainWnd(), &p);

	m_MousePos = XMFLOAT2((float)(p.x - SCREEN_CENTER_X), -(float)(p.y - SCREEN_CENTER_Y));
}

// キーボードプレス
bool InputManager::GetKeyPress(unsigned char key)
{
	return (m_CurrentKeyState[key] & 0x80);
}
// キーボードトリガ
bool InputManager::GetKeyTrigger(unsigned char key)
{
	//	前フレームに押されていない && 現在押されている
	return (m_CurrentKeyState[key] & 0x80) &&
			!(m_PrevKeyState[key] & 0x80);
}
// キーボードリリース
bool InputManager::GetKeyRelease(unsigned char key)
{
	//	前フレームに押されている && 現在押されていない
	return ((m_PrevKeyState[key] & 0x80) &&
			!(m_CurrentKeyState[key] & 0x80));
}

// マウスプレス
bool InputManager::GetMouseButton(MOUSE_BUTTON Btn)
{
	return (m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}
// マウストリガ
bool InputManager::GetMouseTrigger(MOUSE_BUTTON Btn)
{
	//	前フレームにクリックされていない && 現在クリックされている
	return (!(m_PrevMouseState.rgbButtons[Btn] & 0x80) &&
			m_CurrentMouseState.rgbButtons[Btn] & 0x80);
	
}
// マウスリリース
bool InputManager::GetMouseRelease(MOUSE_BUTTON Btn)
{
	//	前フレームにクリックされている && 現在クリックされていない
	return (m_PrevMouseState.rgbButtons[Btn] & 0x80 &&
			!(m_CurrentMouseState.rgbButtons[Btn] & 0x80));
}

// マウス座標の取得
XMFLOAT2 InputManager::GetMousePos() const
{
	return m_MousePos;
}
// マウスの移動量の取得
XMFLOAT2 InputManager::GetMouseVelocity() const
{
	return XMFLOAT2((float)m_CurrentMouseState.lX, (float)-m_CurrentMouseState.lY);
}

// デバッグ描画
void InputManager::Draw()
{
//#ifdef _DEBUG
//	using namespace ImGui;
//
//	//	ウィンドウ作成
//	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	SetNextWindowPos(ImVec2(1000, 300), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	Begin("InputTest");
//
//	//	キーボード入力
//	unsigned char TriggerTest1 = 0;
//	unsigned char ReleaseTest1 = 0;
//	if (GetKeyTrigger(DIK_RETURN))	TriggerTest1 = 0x80;
//	if (GetKeyRelease(DIK_RETURN))	ReleaseTest1 = 0x80;
//
//	//	マウス座標
//	Text("POSSITION");
//	Text("X   : %.0f",	 m_MousePos.x);
//	Text("Y   : %.0f\n", m_MousePos.y);
//
//	//	マウスの移動量
//	XMFLOAT2 velocity = GetMouseVelocity();
//	Text("Velocity");
//	Text("X   : %.0f",	 velocity.x);
//	Text("Y   : %.0f\n", velocity.y);
//
//	End();
//
//#endif // _DEBUG
}