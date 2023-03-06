//インクルード部
#include "Blink.h"
#include "Draw2dComponent.h"

//コンストラクタ
CBlink::CBlink()
:m_nBlink(0),m_nBlink_Cnt(0){

}

//デストラクタ
CBlink::~CBlink() {

}

//初期化
void CBlink::Start() {
	m_pDraw = Parent->GetComponent<CDraw2D>();
}

//更新
void CBlink::Update() {
	m_nBlink_Cnt++;
	if (m_nBlink_Cnt > 60.0f) {
		if (m_nBlink == 1) {
			m_nBlink = 0;
		}
		else if(m_nBlink == 0){
			m_nBlink = 1;
		}
		m_pDraw->SetAlpha((float)m_nBlink);
		m_nBlink_Cnt = 0;
	}
}



