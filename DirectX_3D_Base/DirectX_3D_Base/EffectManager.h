/**
 * @file	EffekseerManager.h
 * @brief	�G�t�F�N�V�A�̃G�t�F�N�g���Ǘ�
 * @author	�u���Č�
 * @date	2021/12/18		�쐬
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

//===== �C���N���[�h =====
#include <unordered_map>
#include <vector>
#include "main.h"

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <Effekseer.Modules.h>
#include <Effekseer.SIMD.h>

#if _DEBUG

#pragma comment(lib, "Effekseer_2017_d.lib")
#pragma comment(lib, "Effekseer_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_d.lib")

#else

#pragma comment(lib, "Effekseer_2017.lib")
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")

#endif // _DEBUG


//===== �O���錾 =====

//===== �}�N����` =====

// �J�[�\���̃G�t�F�N�g
#define CURSOR_EFFECT_NUM			(0)
#define PATH_CURSOR_EFFECT			(u"data/Effect/sampleCursor002.efk")
#define CURSOR_EFFECT_SPRING_NUM	(1)
#define PATH_CURSOR_EFFECT_SPRING	(u"data/Effect/sampleCursor_SPRING002.efk")
#define CURSOR_EFFECT_SUMMER_NUM	(2)
#define PATH_CURSOR_EFFECT_SUMMER	(u"data/Effect/sampleCursor_SUMMER002.efk")
#define CURSOR_EFFECT_AUTUMN_NUM	(3)
#define PATH_CURSOR_EFFECT_AUTUMN	(u"data/Effect/sampleCursor_AUTUMN002.efk")
#define CURSOR_EFFECT_WINTER_NUM	(4)
#define PATH_CURSOR_EFFECT_WINTER	(u"data/Effect/sampleCursor_WINTER002.efk")
#define CURSOR_POS_Z				(-10.0f)	// ��O�ɕ\�����邽��

#define ICICLE_EFFECT_NUM			(5)
#define PATH_EFFECT_ICICLE			(u"data/Effect/sampleTsurara.efk")

#define CLOVER_GOAL_EFFECT_NUM		(6)
#define PATH_CLOVER_GOAL_EFFECT		(u"data/Effect/sampleGoal.efk")

#define SIMPLE_GOAL_EFFECT_NUM		(7)
#define PATH_SIMPLE_GOAL_EFFECT		(u"data/Effect/simpleGoal.efk")
// �A�C�e���l���G�t�F�N�g
#define GET_ITEM_EFFECT_NUM			(8)
#define PATH_GET_ITEM_EFFECT		(u"data/Effect/GetItem.efk")

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class EffectManager
{
	//=== �����o�ϐ�
private:
	Effekseer::ManagerRef m_manager;								// �G�t�F�N�g�Ǘ�
	EffekseerRendererDX11::RendererRef m_renderer;					// �����_�����O
	std::unordered_map<int, Effekseer::EffectRef> 	m_EffectList; 	// �G�t�F�N�g���X�g
	std::unordered_map<int, Effekseer::Handle>		m_handle;		// �n���h��
	static EffectManager* m_pInstance;								// ���̃N���X�̃C���X�^���X

	//=== �����o�֐�
protected:
	EffectManager();							// �R���X�g���N�^
public:
	~EffectManager();							// �f�X�g���N�^
	static EffectManager* GetInstance(void);	// �C���X�^���X��
	static void Destroy();						// �C���X�^���X�j��
	void Start();								// ������
	void Uninit();								// �G�t�F�N�g�}�l�[�W���[���
	void Update();								// �X�V
	void Draw();								// �`��

	// �G�t�F�N�g�̒ǉ�(�G�t�F�N�g��ǉ����Ĕԍ�������U��)
	Effekseer::EffectRef AddEffect(const char16_t* pszFilname, int effectID);

	//	�G�t�F�N�g�擾
	Effekseer::EffectRef GetEffect(int effectID);

	// �G�t�F�N�g�Đ�
	void Play(int effectID, XMFLOAT3 pos, XMFLOAT3 size);

	/* setter */
	// ���W
	void SetPosition(int effectID, XMFLOAT3 pos);

	// ��]
	void SetRotation(int effectID, XMFLOAT3 rot);

	// �g��k��
	void SetScale(int effectID, XMFLOAT3 scale);

	// �w�肵���G�t�F�N�g�̃p�[�e�B�N�����ꎞ��~�A�������͍ĊJ����B
	void SetPaused(int effectID, bool pause);

};
#endif // __EFFECT_MANAGER_H__