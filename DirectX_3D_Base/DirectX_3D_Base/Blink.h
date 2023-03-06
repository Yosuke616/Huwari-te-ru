/**
 * @file	Blink.h
 * @brief	文字を点滅させるコンポーネント
 * @author	志水陽祐
 * @date	2022/1/26
 */

 //===== インクルードガード ======
#ifndef __BLINK_H__
#define __BLINK_H__

//===== インクルード =====
#include "Component.h"

//===== 前方宣言 =====
class CDraw2D;

//===== マクロ定義 =====


//===== 列挙体宣言 =====


//===== クラス定義 =====
class CBlink : public Component
{
private:
	CDraw2D* m_pDraw;	//透明度を変化させるための変数
	int m_nBlink;		//点滅させるための変数
	int m_nBlink_Cnt;	//どの位の間隔で点滅させるかの変数
public:
	CBlink();
	~CBlink() override;
	void Start() override;
	void Update() override;
};

#endif // __機能名_H__

//if(FAILED(hr)) MessageBox(GetMainWnd(), _T(""), NULL, MB_OK);
