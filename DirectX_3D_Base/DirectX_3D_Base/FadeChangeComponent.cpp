/**
 * @file	FadeChangeComponent.cpp
 * @brief	���Ԍo�߂Ń|���S����\������R���|�[�l���g
 * @author	�u���Č�
 * @date	2022/01/06
 */

 //===== �C���N���[�h =====
#include "FadeChangeComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "polygon.h"

#include "imgui.h"

//===== �}�N����` =====
#define FADE_COLOR	(XMFLOAT3(1.0f, 1.0f, 1.0f))		// �|���S���J���[
#define FADE_RATE	(0.02f)

//===== �O���[�o���ϐ� =====
static EChange g_eChange = CHANGE_IN;			// ���

// �R���X�g���N�^
CFadeChange::CFadeChange()
	: m_nTimer(0)
	, m_nCmpTimer(0)
	, m_fAlpha(0.0f)
	, m_fColor(FADE_COLOR)
	, m_bFlg(false)
{
}

// �f�X�g���N�^
CFadeChange::~CFadeChange()
{

}

// ������
void CFadeChange::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw2D = Parent->GetComponent<CDraw2D>();

	g_eChange = CHANGE_IN;
}

// �X�V
void CFadeChange::Update()
{
	m_nTimer--;
	if (!m_bFlg && m_nTimer <= 0)
	{
		g_eChange = CHANGE_OUT;
		m_bFlg = true;
	}

	switch (g_eChange)
	{
		case CHANGE_NONE:
							break;
		case CHANGE_OUT:
							m_fAlpha += FADE_RATE;
							if (m_fAlpha >= 1.0f)
							{
								m_fAlpha = 1.0f;
								g_eChange = CHANGE_IN;
							}
							m_pDraw2D->SetAlpha(m_fAlpha);
							break;
		case CHANGE_IN:
							m_fAlpha -= FADE_RATE;
							if (m_fAlpha <= 0.0f)
							{
								m_fAlpha = 0.0f;
								g_eChange = CHANGE_NONE;
								m_nTimer = 0;
								m_nTimer = m_nCmpTimer;
								m_bFlg = false;
							}
							m_pDraw2D->SetAlpha(m_fAlpha);
							break;
	}
}

// �`��
void CFadeChange::Draw()
{
	////	�������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	//SetZWrite(false);
	//SetBlendState(BS_ALPHABLEND);

	//// ��ʑS�̂ɔ������|���S����`��
	//SetPolygonPos(0.0f, 0.0f);
	//SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//SetPolygonTexture(nullptr);
	//SetPolygonFrameSize(1.0f, 1.0f);
	//SetPolygonUV(0.0f, 0.0f);
	//SetPolygonColor(m_fColor.x, m_fColor.y, m_fColor.z);
	////SetPolygonAlpha(m_fAlpha);
	//DrawPolygon(GetDeviceContext());

	//// ���ɖ߂�
	//SetZWrite(true);
	//SetPolygonColor(1.0f, 1.0f, 1.0f);
	//SetPolygonAlpha(1.0f);

#ifdef _DEBUG
	//	�f�o�b�O�p
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 240), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("ImGuiTest");
	Text("m_nTimer : %d", m_nTimer);
	End();
#endif // _DEBUG
}

// ���Ԑݒ�
void CFadeChange::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
	m_nCmpTimer = nTimer;
}