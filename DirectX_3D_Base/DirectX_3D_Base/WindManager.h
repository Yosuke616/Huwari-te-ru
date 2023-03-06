/**
 * @file	MouseManager.h
 * @brief	�}�E�X���W���Ǘ�����N���X�ɂ���
 * @author	�u���z�S
 * @date	2021/12/08
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

//�C���N���[�h��
#include "main.h"
#include <list>

//�O���錾
class Object;
class CTransform;
class InputManager;
class CGauge;

//�񋓑��錾

//�N���X��`
class WindManager
{
	//=== �����o�ϐ�
private:
	static WindManager* m_pInstance;		//	�C���X�^���X�p
	CGauge* m_pWindGauge;					//	�Q�[�W�̒l�����ł���悤

	std::list<XMFLOAT2> m_WindCenterList;	//�@�}�E�X���W��ۑ����郊�X�g
	XMFLOAT2 m_P;							//�@�}�E�X�̍��W������ϐ�
	std::list<XMFLOAT2> m_WindVectorList;	//	���̗͂�ۑ�����
	XMFLOAT2 m_WindPower;					//	���W�Ԃ̍���ۑ�����ϐ�

	int m_nFrameCnt;						//  �t���[����ۑ����邽�߂̕ϐ�
	int m_nDeleteCnt;						//	�������߂̃J�E���g�i�K�v�Ȃ������j
	bool m_bDeleteFlg;						//	���ꂪ�I���ɂȂ����畗�����n�߂�

	// 2022/01/22 yousuke
	bool m_bTutorialFlg;

	// 2022/1/26/Touch_Tutorial Shinmizu Yosuke ----------------------------------------
	bool m_bSpawnWind;						//�@���̃t���O��false�Ȃ畗�͏o���Ȃ�
	//----------------------------------------------------------------------------------

	//=== �����o�֐�
protected:
	WindManager();							//	�R���X�g���N�^
public:
	~WindManager();							//	�f�X�g���N�^
	void Update();							//	�X�V
	void Draw();							//	�`��

	static WindManager* GetInstance();		//	�C���X�^���X����
	static void Destroy();					//	�C���X�^���X�j��

	//2022/1/26/Touch_Tutorial Shimizu Yosuke ---------------
	//�����o����悤�ɂ��邽�߂̃Z�b�^�[
	void SetSpawnWind(bool bSpawnWind);
	//-------------------------------------------------------

	//	�����̕�
	static void CreateWind(XMFLOAT2 pos, XMFLOAT2 vector);

	// ���Q�[�W�̐ݒ�
	void SetWindGauge(CGauge* pGauge);

	// ���Q�[�W�̎擾
	int GetWindGauge();

	//2022/1/21/Tutorial Shimizu Yosuke ---------------------
	//�@���j���[�ɐG��Ă悢���ǂ���
	void SetTutorial(bool bTutorial);
	bool GetTutorial();
	//-------------------------------------------------------
};
#endif