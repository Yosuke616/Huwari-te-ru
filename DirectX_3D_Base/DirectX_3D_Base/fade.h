/**
 * @file	fade.h
 * @brief	�t�F�[�h�C��/�A�E�g
 * @author	AT12A ���c���l
 * @date	2021/07/xx
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __FADE_H__
#define __FADE_H__

//===== �C���N���[�h =====
#include "SceneManager.h"

//===== �񋓑̐錾 =====
enum EFade			// �t�F�[�h���
{
	FADE_NONE = 0,				// �������Ă��Ȃ�
	FADE_IN,					// �t�F�[�h�C��������
	FADE_OUT_SCENE_CHANGE,		// �t�F�[�h�A�E�g������(�V�[���؂�ւ�)
	MAX_FADE
};

//===== �v���g�^�C�v�錾 =====
void InitFade();					// ������
void UninitFade();					// �I������
void UpdateFade();					// �X�V
void DrawFade();					// �`��
void StartFadeOutSceneChange(EScene eNext, int deley = 0);	// �t�F�[�h�A�E�g�J�n(deley)

EFade GetFade();					// ��Ԏ擾
void SetFadeColor(float fR, float fG, float fB); // �t�F�[�h�A�E�g�F�ݒ�
#endif // __FADE_H__

