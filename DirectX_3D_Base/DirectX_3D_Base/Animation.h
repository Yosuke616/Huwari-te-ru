/**
 * @file	AinmChangeCom.h
 * @brief	�A�j���[�V�����Đ��̃t���[�����w�肷��
 * @author	AT12A ���c���l
 * @date	2022/01/09
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __ANIM_CHANGE_H__
#define __ANIM_CHANGE_H__

//===== �C���N���[�h =====
#include "Component.h"
#include <unordered_map>

//===== �O���錾 =====
class CDrawModel;


//===== �N���X��` =====
class Animation
{
	//=== �����o�ϐ�
private:
	std::unordered_map<int, double>	m_dStart;	// �A�j���[�V�����̎n�܂�̃t���[����
	std::unordered_map<int, double>	m_dEnd;		// �A�j���[�V�����̏I���̃t���[����
	std::unordered_map<int, double>	m_dPlaySpeed;// �Đ����x
	std::unordered_map<int, bool>	m_bLoop;	// ���[�v�Đ�
	std::unordered_map<int, int>	m_nOrder;	// �A�j���[�V�����̗D��x
	//std::unordered_map<int, bool>	m_bCall;	// Play�֐����Ă΂�Ă��邩
	std::unordered_map<int, bool>	m_bEnd;		// Play�֐����Ă΂�Ă��邩
	double		m_dCurrentAnimTime;				// ���݂̃A�j���[�V��������
	int			m_nCurrerntAnimState;			// �����̃A�j���[�V�������
	bool		m_bIdle;						// ture ���[�v���I����0�Ԗڂ̃A�j���[�V���� false �A�j���[�V�������I���ΏI��
	bool		m_bAnimation;					// �Đ��t���O
	CDrawModel*	m_pDraw3D;						// ���f��
public:

	//=== �����o�֐�
public:
	void Play(int AnimState);			// �Đ�
	void Stop();						// ��~

	Animation();						// �R���X�g���N�^
	~Animation();						// �f�X�g���N�^
	void Update();						// �X�V
	void Draw();						// �`��
	//setter
	void SetModel(CDrawModel* model);

	// �A�j���[�V�����̒ǉ�
	void AddAnimation(int AnimID, double start, double end, bool loop, double loopStartFrame);

	// ���[�v�Đ��̐ݒ�
	void SetLoop(int AnimID, bool loop);
	// �Đ����x�̐ݒ�
	void SetPlaySpeed(int AnimID, double speed);
	// �A�j���[�V�������I��������̃t���O
	bool AnimEnd(int AnimID);
	// ���
	void AlwaysAnimation(bool always);
};
#endif // __ANIM_CHANGE_H__