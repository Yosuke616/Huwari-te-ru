//===== �C���N���[�h =====
#include "ComponentSoundConfig.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "ObjectInfo.h"
#include "GaugeComponent.h"
#include "ObjectManager.h"

#include "InputManager.h"
#include "Sound.h"

//=== �ÓI�����o�ϐ�
float CSoundConfig::m_fBGM_Volume = 1.0f;
float CSoundConfig::m_fSE_Volume = 1.0f;

// �R���X�g���N�^
CSoundConfig::CSoundConfig()
	: m_bProcessing(false)
	, m_pGauge(nullptr)
{
}

// �f�X�g���N�^
CSoundConfig::~CSoundConfig(){
}

// ������
void CSoundConfig::Start()
{
	// �I�u�W�F�N�g���W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
}

// �X�V
void CSoundConfig::Update()
{
	// �J�[�\���Ɠ������Ă�����
	if (m_bProcessing)
	{
		// X���W���J�[�\���ƈꏏ�ɂ���
		m_pTransform->Pos.x = m_Cursor->Pos.x;

		// ���W�̕␳
		if (m_pTransform->Pos.x < GAUGE_LEFT)				// ���[
			m_pTransform->Pos.x = GAUGE_LEFT;
		if (m_pTransform->Pos.x > GAUGE_LEFT + STICK_WIDTH)	// �E�[
			m_pTransform->Pos.x = GAUGE_LEFT + STICK_WIDTH;

		// ���ʐݒ�
		if (m_eSoundType == CONFIG_BGM)
		{
			/*
			�{�����[��			0					 BGM�{�����[��					   1
							|------------------------+-------------------------|
			X���W			�Q�[�W�̍���X���W				����X���W					�Q�[�W�̉E��X���W
			*/
			// BGM�{�����[�� = (����X���W - �Q�[�W�̍���X���W) �� �Q�[�W�̉���
			m_fBGM_Volume = (m_pTransform->Pos.x - GAUGE_LEFT) / STICK_WIDTH;

			// �ړ����E
			if (m_fBGM_Volume < 0.0f) m_fBGM_Volume = 0.0f;
			if (m_fBGM_Volume > 1.0f) m_fBGM_Volume = 1.0f;

			// BGM�̃{�����[���ݒ�
			CSound::SetVolume_BGM(m_fBGM_Volume);

			// �Q�[�W�ݒ�
			if (m_pGauge)
			{
				int value = (int)(m_fBGM_Volume * STICK_WIDTH);
				CGauge::SetGauge(m_pGauge, value);
			}
		}
		else if(m_eSoundType == CONFIG_SE)
		{
			/*
			�{�����[��			0					 SE�{�����[��					   1
							|------------------------+-------------------------|
			X���W			�Q�[�W�̍���X���W				����X���W					�Q�[�W�̉E��X���W
			*/
			// SE�{�����[�� = (����X���W - �Q�[�W�̍���X���W) �� �Q�[�W�̉���
			m_fSE_Volume = (m_pTransform->Pos.x - GAUGE_LEFT) / STICK_WIDTH;

			// �ړ����E
			if (m_fSE_Volume < 0.0f) m_fSE_Volume = 0.0f;
			if (m_fSE_Volume > 1.0f) m_fSE_Volume = 1.0f;

			// SE�̃{�����[���ݒ�
			CSound::SetVolume_SE(m_fSE_Volume);

			// �Q�[�W�ݒ�
			if (m_pGauge)
			{
				int value = (int)(m_fSE_Volume * STICK_WIDTH);
				CGauge::SetGauge(m_pGauge, value);
			}
		}

		//	�h���b�O�I����
		if (INPUT->GetMouseRelease(MOUSE_L))
		{
			// ����I��
			m_bProcessing = false;

			// SE�̐ݒ�̏ꍇ
			if(m_eSoundType == CONFIG_SE) CSound::Play(SE_DECIDE);	// ���艹���o��
		}
	}

	//	���W�ݒ�
	//Parent->GetComponent<CDraw2D>()->SetPosition(m_pTransform->Pos.x, m_pTransform->Pos.y);
}

// �Փˏ���
void CSoundConfig::OnCollisionEnter(Object* pObject)
{
	// �J�[�\���ƏՓ�
	if (pObject->GetName() == CURSOR_NAME)
	{
		// ���N���b�N���Ă�����
		if (INPUT->GetMouseButton(MOUSE_L))
		{
			// �J�[�\���̍��W���𔲂����
			m_Cursor = pObject->GetComponent<CTransform>();

			// ���ʑ���\
			m_bProcessing = true;
		}
	}
}

// ���ʐݒ�^�C�v�̐ݒ�
void CSoundConfig::SetSoundType(SOUND_CONFIG type)
{
	m_eSoundType = type;
}

// �Q�[�W�̐ݒ�
void CSoundConfig::SetGauge(CGauge* pGauge)
{
	m_pGauge = pGauge;
}