/**
 * @file	Load.h
 * @brief	���[�h���
 * @author	AT12A ���c���l
 * @date	2021/12/31		�쐬
 */


 //===== �C���N���[�h�K�[�h ======
#ifndef __LOAD_H__
#define __LOAD_H__

//===== �C���N���[�h =====
#include "Scene.h"
#include <vector>

//===== �N���X��` =====
class Load
{
	//=== �����o�ϐ�
private:
	static HANDLE	m_handle;		// �X���b�h�n���h��
	static bool		m_bLoading;		// ���[�h��
	static int		m_nLoadCount;	// ���[�h���Ԃ̃J�E���g
	// ���[�h��ʂ̃I�u�W�F�N�g���X�g
	static std::vector<Object*> m_LoadScreenObjects;
	
	//=== �����o�֐�
private:
	static void Init();			// ���[�h��ʂ̃I�u�W�F�N�g������
	static void Uninit();		// ���[�h��ʂ̃I�u�W�F�N�g�I������
	static unsigned __stdcall LoadScreen();	// ���[�h��ʍX�V���`��
public:
	Load();						// �R���X�g���N�^
	~Load();					// �f�X�g���N�^

	static void Begin();		// ���[�h��ʂ̃X���b�h
	static void End();			// �������������I���߂�����sleep���Ă��炤
	static int GetLoadCount();	// ���[�h��ʂ̃t���[�����擾

};
#endif // __SCENE_LOAD_H__


