/**
 * @file	fade.cpp
 * @brief	�t�F�[�h�C��/�A�E�g����
 * @author	�u���Č�
 * @date	2021/12/23
 */

//===== �C���N���[�h =====
#include "fade.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "polygon.h"
#include "fadepolygon.h"
#include "Sound.h"

//===== �}�N����` ======
//#define FADE_RATE (0.02f)	// �t�F�[�h�C��/�A�E�g�̍���
#define FADE_OUT_SPEED		(15.0f)				// �t�F�[�h�A�E�g�̃X�s�[�h
#define FADE_IN_SPEED		(15.0f)				// �t�F�[�h�C���̃X�s�[�h
#define START_FADE_OUT_POS	(SCREEN_WIDTH)		// �t�F�[�h�A�E�g�J�n�ʒu
#define START_FADE_IN_POS	(0.0f)				// �t�F�[�h�C���J�n�ʒu

//===== �O���[�o���ϐ� =====
static float g_fRed = 0.0f;				// �t�F�[�h�A�E�g�F
static float g_fGreen = 0.0f;
static float g_fBlue = 0.0f;
static float g_fAlpha = 1.0f;			// �s�����x
static EFade g_eFade = FADE_IN;			// ���
static EScene g_eNext = SCENE_TITLE;	// ���̃V�[��
static int g_nStage;					//�@���̃X�e�[�W

static int g_nDeleyCnt = 0;

static float	g_fFadeIN;				// �t�F�[�h�C��
static float	g_fFadeOUT;				// �t�F�[�h�A�E�g

// ������
void InitFade()
{
	//g_fAlpha = 1.0f;
	g_eFade = FADE_IN;
	g_eNext = SCENE_TITLE;

	g_fFadeOUT = START_FADE_OUT_POS;	// �t�F�[�h�A�E�g�J�n�ʒu
	g_fFadeIN = START_FADE_IN_POS;		// �t�F�[�h�C���J�n�ʒu
}

// �I������
void UninitFade()
{
	//g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}

// �X�V
void UpdateFade()
{
	switch (g_eFade)
	{
	case FADE_NONE:	//	�t�F�[�h�Ȃ�
		break;

	case FADE_OUT_SCENE_CHANGE:	//	�t�F�[�h�A�E�g

		if (g_nDeleyCnt > 0)
		{
			g_nDeleyCnt--;
			break;
		}

		g_fFadeIN = 0.0f;
		g_fFadeOUT -= FADE_OUT_SPEED;
		UpdateFadePolygon(g_fFadeOUT);

		// �{�����[�����t�F�[�h�A�E�g
		CSound::SetVolume(g_fFadeOUT / SCREEN_WIDTH);		// ���ʂ����X�ɏ�����

		if (g_fFadeOUT <= 0.0f)
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_fFadeOUT = SCREEN_WIDTH;
			g_eFade = FADE_IN;

			// �V�[���ؑ�
			auto pSceneMsnager = SceneManager::Instance();
			pSceneMsnager->SetScene(g_eNext);
		}
		break;

	case FADE_IN:	// �t�F�[�h�C��
		g_fFadeOUT = SCREEN_WIDTH;
		g_fFadeIN += FADE_IN_SPEED;
		UpdateFadePolygon(g_fFadeIN);

		// �{�����[�����t�F�[�h�C��
		CSound::SetVolume(g_fFadeIN / SCREEN_WIDTH);	// ���ʂ����X�ɑ傫��

		if (g_fFadeIN >= SCREEN_WIDTH)
		{
			// �t�F�[�h�C���I��
			g_fFadeIN = 0.0f;
			g_eFade = FADE_NONE;
		}
		break;
	}
}

// �`��
void DrawFade()
{
	//	�������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	SetZWrite(false);
	SetBlendState(BS_ALPHABLEND);

	// ��ʑS�̂ɔ������|���S����`��
	SetFadePolygonPos(0.0f, 0.0f);
	SetFadePolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetFadePolygonTexture(nullptr);
	SetFadePolygonFrameSize(1.0f, 1.0f);
	SetFadePolygonUV(0.0f, 0.0f);
	SetFadePolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetFadePolygonAlpha(1.0f);
	DrawFadePolygon(GetDeviceContext());

	// ���ɖ߂�
	SetZWrite(true);
	SetFadePolygonColor(1.0f, 1.0f, 1.0f);
	SetFadePolygonAlpha(1.0f);
}



void StartFadeOutSceneChange(EScene eNext, int deley)
{
	if (g_eFade != FADE_OUT_SCENE_CHANGE)
	{
		g_eFade = FADE_OUT_SCENE_CHANGE;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
		g_nDeleyCnt = deley;
	}
}

// �t�F�[�h��Ԏ擾
EFade GetFade() 
{
	  return g_eFade;
}

// �t�F�[�h�A�E�g�F�ݒ�
void SetFadeColor(float fR, float fG, float fB)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}