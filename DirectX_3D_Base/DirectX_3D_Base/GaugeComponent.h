/**
 * @file Gauge.h
 * @brief ���̗͂̏�������m����R���|�[�l���g
 * @author �u���@�z�S
 * @date 2021/12/11
 */

//===== �C���N���[�h�K�[�h ======
#ifndef __GAUGE_H__
#define __GAUGE_H__

//===== �C���N���[�h =====
#include "Component.h"


////===== �O���錾 =====
class CTransform;
class CDraw2D;


////===== �N���X��` =====
class CTransform;
class CDraw2D;

class CGauge : public Component
{
private:
	CTransform*		m_pTransform;		//	�g�����X�t�H�[�����������悤��	
	CDraw2D*		m_pDraw;			//	�`����������悤��

	XMFLOAT2        m_InitPosition;		//	�|�W�V�����̏����l(��ő傫����ύX����ۂɎg�p)
	XMFLOAT2        m_MaxSize;			//	�T�C�Y�̍ő�l(��Ōv�Z���邽�߂ɕK�v)
	int m_nMaxGauge;					//�@	���̃Q�[�W�̃o�[�̒�����ۑ�����ϐ�
	bool m_bBarFlg;						//�@	�Q�[�W�������Ȃ�����I���ɂ���
	int m_nCurrentGauge;				//�@	�Q�[�W��������茸�炵�Ă������߂̕ϐ�

protected:

public:
	CGauge();							//	�R���X�g���N�^
	~CGauge();							//	�f�X�g���N�^
	void Start() override;				//	������
	void Update() override;				//	�X�V

	//	�Q�[�W�̑傫���̍ő�l��ݒ肷��
	void SetMaxGauge(int max);
	//	�|�W�V�������Z�b�g����@����Ȃ��񂶂�Ȃ�����
	void SetInitPos(float fx,float fy);
	//	�傫�����Z�b�g����
	void SetSize(float fx,float fy);

	// �Q�[�W�ʂ�ݒ肷��
	static void SetGauge(CGauge* pGauge, int value);
	//	�Q�[�W�����炷
	static void CalcGauge(CGauge* pGauge, int value);
	//getter
	static int GetCurrentGauge(CGauge* pGauge);
};

#endif // __WIND_GAUGE_H__
