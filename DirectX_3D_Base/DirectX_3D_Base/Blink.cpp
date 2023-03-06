//�C���N���[�h��
#include "Blink.h"
#include "Draw2dComponent.h"

//�R���X�g���N�^
CBlink::CBlink()
:m_nBlink(0),m_nBlink_Cnt(0){

}

//�f�X�g���N�^
CBlink::~CBlink() {

}

//������
void CBlink::Start() {
	m_pDraw = Parent->GetComponent<CDraw2D>();
}

//�X�V
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



