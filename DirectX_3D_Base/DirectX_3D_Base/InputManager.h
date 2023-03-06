/**
 * @file	InputManager.h
 * @brief	キーボード入力を管理するプログラム
 * @author	AT12A 成田陸人
 * @date	成田	2021/11/17	キーボードのプレス、トリガ、リリース状態を取得する関数の追加
 * 
 * 			成田 2021/11/20	マウスのプレス、トリガ、リリース状態を取得する関数の追加
 * 							マウス座標を取得する関数の追加
 * 							マウスの移動量を取得する関数の追加
 * 
 * @detail	入力の引数について
			キーボード		DIK_ + 押すキー	例）	DIK_1, DIK_RETURN（ENTERキー）
			マウス		MOUSE_ + (L、 R、 C)
 */


//===== インクルードガード ======
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

//===== インクルード =====
#include "main.h"
#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン指定
#include <dinput.h>


//===== マクロ定義 =====
#define KEY_MAX	(256)

#define INPUT (InputManager::Instance())

//=====	列挙体 =====
enum MOUSE_BUTTON
{
	MOUSE_L,	//	左クリック
	MOUSE_R,	//	右クリック
	MOUSE_C		//	ホイール
};

//===== クラス定義 =====
class InputManager
{
private:
	//=== メンバ変数
	static InputManager* m_pInstance;			//	インスタンス用
	unsigned char m_CurrentKeyState[KEY_MAX];	//	現在のキーボード入力情報の格納先
	unsigned char m_PrevKeyState[KEY_MAX];		//	前フレームのキーボード入力情報の格納先
	DIMOUSESTATE m_CurrentMouseState;			//	マウスの現在の入力情報
	DIMOUSESTATE m_PrevMouseState;				//	マウスの一フレーム前の入力情報
	XMFLOAT2	 m_MousePos;					//	マウス座標

	HRESULT InitKeyBoard();						//	キーボード初期化
	void UpdateKeyBoard();						//	キーボード更新
	HRESULT InitMouse();						//	マウス初期化
	void UpdateMouse();							//	マウス更新

protected:
	//=== メンバ関数
	InputManager();								//	コンストラクタ

public:
	static InputManager* Instance();			//	インスタンス生成
	~InputManager();							//	デストラクタ
	HRESULT Init();								//	初期化
	void Uninit();								//	終了
	void Update();								//	更新
	void Draw();								//	描画(debug用)

	bool GetKeyPress(unsigned char   key);		//	キーボードのプレス状態を取得
	bool GetKeyTrigger(unsigned char key);		//	キーボードのトリガ状態を取得
	bool GetKeyRelease(unsigned char key);		//	キーボードのリリース状態を取得

	bool GetMouseButton( MOUSE_BUTTON Btn);		//	マウスのプレス状態を取得
	bool GetMouseTrigger(MOUSE_BUTTON Btn);		//	マウスのトリガ状態を取得
	bool GetMouseRelease(MOUSE_BUTTON Btn);		//	マウスのリリース状態を取得

	XMFLOAT2 GetMousePos() const;				//	マウス座標の取得
	XMFLOAT2 GetMouseVelocity() const;			//	マウスの移動量の取得
};
#endif // __INPUT_MANAGER_H__

