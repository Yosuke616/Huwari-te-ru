/**
 * @file	BGChangeComponent.h
 * @brief	�w�i�����Ԍo�߂ŕς��R���|�[�l���g
 * @author	�u���Č�
 * @date	2021/12/28
 */

//===== �C���N���[�h =====
#include "BGChangeComponent.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "TextureManager.h"
#include "sceneTitle.h"

//===== �}�N����` =====
#define CHANGE_TIME	(750)

// �R���X�g���N�^
CBgChange::CBgChange()
	: m_nTime(0)
	, m_nCmpTimer(0)
	, m_nTexNum(0)
	, m_bFlg(false)
{

}

// �`��
CBgChange::~CBgChange()
{

}

// �`��
void CBgChange::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// �`��
void CBgChange::Update()
{
	m_nTime--;
	if (!m_bFlg && m_nTime <= 50)
	{
		m_bFlg = true;
		m_nTexNum++;
		ChangeTexture();
	}
	if (m_nTime <= 0)
	{
		m_nTime = m_nCmpTimer;
		m_bFlg = false;
	}
}

// �`��
void CBgChange::Draw()
{

}

// �e�N�X�`���ؑ�
void CBgChange::ChangeTexture()
{
	switch (m_nTexNum % 4)
	{
		case 0:
				SceneTitle::SetSeason(0);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SPRING_TEX_NUM));	// �t
				break;
		case 1:
				SceneTitle::SetSeason(1);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_SUMMER_TEX_NUM));	// ��
				break;
		case 2:
				SceneTitle::SetSeason(2);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_AUTUMN_TEX_NUM));	// �H
				break;
		case 3:
				SceneTitle::SetSeason(3);
				m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_WINTER_TEX_NUM));	// �~
				break;
	}
}

// ���Ԑݒ�
void CBgChange::SetTimer(int nTimer)
{
	//m_nTimer = nTimer * 60;		// ���Ԑݒ�
	//m_nCmpTimer = nTimer * 60;	// ���ԕۑ�
	m_nTime = nTimer;
	m_nCmpTimer = nTimer;
}