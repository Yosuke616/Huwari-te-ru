//=============================================================================
//
//  �R���|�[�l���g�̊��N���X[Component.h]
// Author : RIKUTO NARITA
//
//=============================================================================

//===== �C���N���[�h�K�[�h ======
#ifndef __SOUND_CONFIG_H__
#define __SOUND_CONFIG_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

//===== �O���錾 =====
class CTransform;
class CGauge;

//===== �񋓑� =====

enum SOUND_CONFIG
{
	// �ݒ�^�C�v
	CONFIG_MASTER,	// �}�X�^�[�{�����[��
	CONFIG_BGM,		// BGM�{�����[��
	CONFIG_SE,		// SE�{�����[��

	MAX_CONFIG
};

//===== �}�N����` =====


//#define SE_CONFIG_POS_X (STICK02_POS_X - ((STICK_WIDTH - CONFIG_STICK_WIDTH) / 2))

//===== �񋓑̐錾 =====

//===== �N���X��` =====
class CSoundConfig : public Component
{
	//=== �����o�ϐ�
private:
	// �{�����[���̒l
	static float m_fBGM_Volume;			// BGM
	static float m_fSE_Volume;			// SE

	CTransform* m_pTransform = nullptr; // ���̃I�u�W�F�N�g�̍��W
	CTransform* m_Cursor = nullptr;		// �J�[�\�����W
	CGauge* m_pGauge;
	bool m_bProcessing;					//	����t���O

	SOUND_CONFIG m_eSoundType;			// �T�E���h�̎��
	//bool m_bBGM;						//	true BGM�̐ݒ�, false SE�̐ݒ�

	//=== �����o�֐�
public:
	CSoundConfig();						// �R���X�g���N�^
	~CSoundConfig();					// �f�X�g���N�^
	void Start();						// ������
	void Update();						// �X�V
	void OnCollisionEnter(Object*);		// �Փˏ���

	// ���ʐݒ�^�C�v�̐ݒ�
	void SetSoundType(SOUND_CONFIG type);

	// �Q�[�W�̐ݒ�
	void SetGauge(CGauge* pGauge);

};
#endif // __SOUND_CONFIG_H__

