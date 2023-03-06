#include "Animation.h"
#include "Draw3dComponent.h"
#include "InputManager.h"
#include <time.h>
#include "imgui.h"

#define DEFAULT_PLAY_SPEED	(0.024)

// �R���X�g���N�^
Animation::Animation()
	: m_pDraw3D(nullptr)
	, m_dStart(0), m_dEnd(0), m_dCurrentAnimTime(0)
	, m_bLoop(false), m_bAnimation(false), m_bIdle(false)
{
}

// �f�X�g���N�^
Animation::~Animation(){
}


// �X�V
void Animation::Update()
{
#ifdef _DEBUG
	if (m_pDraw3D && m_pDraw3D->Parent->GetName() == PLAYER_NAME)
	{
		if (INPUT->GetKeyTrigger(DIK_SPACE))
		{
			m_bAnimation = true;
		}
		if (INPUT->GetKeyTrigger(DIK_V))
		{
			m_dCurrentAnimTime = m_dStart[m_nCurrerntAnimState];
			// �A�j���[�V�������f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_N))
		{
			m_dCurrentAnimTime -= DEFAULT_PLAY_SPEED;
			// �A�j���[�V�������f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_M))
		{
			m_dCurrentAnimTime += DEFAULT_PLAY_SPEED;
			// �A�j���[�V�������f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_B))
		{
			this->Stop();
		}
	}
#endif // _DEBUG



	// �A�j���[�V�����Đ�
	if (m_bAnimation)
	{
		// ���l����
		m_dCurrentAnimTime += m_dPlaySpeed[m_nCurrerntAnimState];

		// �A�j���[�V�������f
		m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);

		// �A�j���[�V���������������
		if (m_dCurrentAnimTime > m_dEnd[m_nCurrerntAnimState])
		{
			if (m_bLoop[m_nCurrerntAnimState])
			{
				// �A�j���[�V�����̎n�܂��
				m_dCurrentAnimTime = m_dStart[m_nCurrerntAnimState];
			}
			else
			{
				if (m_bIdle)
				{
					// �I��������Ƃ�m�点��
					m_bEnd[m_nCurrerntAnimState] = true;

				}
				else	m_bAnimation = false;	// �A�j���[�V�����I��
			}
		}
	}
}

// �`��
void Animation::Draw(){
}

// �ŏ�����Đ�
void Animation::Play(int AnimState)
{
	// �Đ��t���OON
	m_bAnimation = true;

	// ���݂̃A�j���Ǝ��̃A�j�����ꏏ�̏ꍇ�͉������Ȃ�
	if (m_nCurrerntAnimState == AnimState) return;

	// �A�j���؂�ւ�
	m_nCurrerntAnimState = AnimState;

	// �A�j���[�V�������n�܂�̃t���[�����ɐݒ�
	m_dCurrentAnimTime = m_dStart[AnimState];
}

// ��~
void Animation::Stop()
{
	m_bAnimation = false;	// �Đ��t���OOFF
}


// ���[�v�Đ�
void Animation::SetLoop(int AnimID, bool loop)
{
	m_bLoop[AnimID] = loop;
}


// ���f���ݒ�
void Animation::SetModel(CDrawModel* model)
{
	m_pDraw3D = model;
}


// �A�j���[�V�����ǉ�
void Animation::AddAnimation(int AnimID, double start, double end, bool loop, double loopStartFrame)
{
	m_dStart[AnimID] = start;
	m_dEnd[AnimID] = end;
	m_bLoop[AnimID] = loop;
	m_dPlaySpeed[AnimID] = DEFAULT_PLAY_SPEED;
	m_bEnd[AnimID] = false;
}

void Animation::AlwaysAnimation(bool always)
{
	m_bIdle = always;
}
void Animation::SetPlaySpeed(int AnimID, double speed)
{
	m_dPlaySpeed[AnimID] = speed;
}
bool Animation::AnimEnd(int AnimID)
{
	if (m_bEnd[AnimID])
	{
		// �I���t���O�̃��Z�b�g
		m_bEnd[AnimID] = false;

		return true;
	}
	else return false;
}
