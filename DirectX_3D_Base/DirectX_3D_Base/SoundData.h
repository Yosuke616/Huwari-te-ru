// BGM/SE��` (SoundData.h)

enum eBGM {
	BGM_TITLE = 0,		// TITLE
	BGM_STAGESELECT,	// �X�e�[�W�Z���N�g
	BGM_SPRING,			// �t�X�e�[�W
	BGM_SUMMER,			// �ăX�e�[�W
	BGM_FALL,			// �H�X�e�[�W
	BGM_WINTER,			// �~�X�e�[�W
	BGM_ENDING,			// �G���f�B���OBGM

	MAX_BGM
};

enum eSE {
	SE_DECIDE = 0,		// ����
	SE_SELECT,			// �Z���N�g
	SE_ERROR,			// �G���[
	SE_GAME_START,		// �Q�[���X�^�[�g
	SE_WIND,			// ������
	SE_GAME_CLEAR,		// �Q�[���N���A
	SE_GAME_OVER,		// �Q�[���I�[�o�[(��)
	SE_PAUSE,			// �|�[�Y
	SE_GET_SEED,		// ��擾
	SE_GET_CLOVER,		// �N���[�o�[�擾
	SE_ICICLE_FALL,		// ��炪������
	SE_ICICLE_DESTROY,	// ��炪����
	SE_BIND,			// �w偂̑��ɂЂ�������
	SE_WEB_DESTROY,		// �w偂̑���j��
	SE_FALLENLEAVES,	// �����t���΂�
	SE_FINISH,			// �G���f�B���O�̃~�j�Q�[���I��se
	MAX_SE
};
