/**
 * @file	GetUIComponent.h
 * @brief	�A�C�e���Q�b�g����UI�R���|�[�l���g
 * @author	AT12A �u���Č�
 * @date	2022/01/29
 */

 //===== �C���N���[�h =====
#include "GetUIComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Sound.h"
#include "SceneSelect.h"
#include "SceneManager.h"

//===== �}�N����` =====
#define LARGE_RATE	(6.0f)

// �R���X�g���N�^
CGetUI::CGetUI()
	: m_vSize(XMFLOAT2(0.0f, 0.0f))
	, m_vMaxSize(XMFLOAT2(0.0f, 0.0f))
	, m_fSizeRate(0.0f)
	, m_nTimer(-1)
{

}

// �f�X�g���N�^
CGetUI::~CGetUI()
{

}

// ������
void CGetUI::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();	// ���W
	m_pDraw2D = Parent->GetComponent<CDraw2D>();		// �`��
}

// �X�V
void CGetUI::Update()
{
	// �x��
	if (m_nTimer > 0)
	{
		m_nTimer--;

		return;
	}

	// �g��
	m_vSize.x += m_fSizeRate;				// ���̑傫��
	m_vSize.y += m_fSizeRate;				// �c�̑傫��

	// �g��␳
	if (m_vSize.x >= m_vMaxSize.x)			// �l�̕␳(��)
	{
		m_vSize.x = m_vMaxSize.x;			// ��
	}
	if (m_vSize.y >= m_vMaxSize.y)			// �l�̕␳(�c)
	{
		m_vSize.y = m_vMaxSize.y;			// ��
	}
	if (m_vSize.x >= m_vMaxSize.x && m_vSize.y >= m_vMaxSize.y)
	{
		this->Delete();
		if (SceneManager::Instance()->GetScene() == SCENE_SELECT)
		{
			if (Parent->GetName() == CLOVER_UI_NAME ||
				Parent->GetName() == CLEAR_UI_1_NAME ||
				Parent->GetName() == CLEAR_UI_2_NAME ||
				Parent->GetName() == CLEAR_FLOWER_UI_1_NAME ||
				Parent->GetName() == CLEAR_FLOWER_UI_1_NAME) CSound::Play(SE_GET_CLOVER);
		}
	}

	// �傫���ݒ�
	m_pDraw2D->SetSize(m_vSize.x, m_vSize.y);
}

// �`��
void CGetUI::Draw()
{

}

// �Z�b�g�֐�
void CGetUI::SetUISize(XMFLOAT2 vSize)
{
	m_vMaxSize = vSize;		// �傫���ݒ�
}

// �Z�b�g�֐�
void CGetUI::SetUIRate(float fRate)
{
	m_fSizeRate = fRate;	// �g��X�s�[�h�ݒ�
}

void CGetUI::SetDelayTimer(int delay)
{
	m_nTimer = delay;
}