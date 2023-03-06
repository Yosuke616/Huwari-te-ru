#include "AnimationComponent.h"
#include "imgui.h"
#include "Draw3dComponent.h"
#include "InputManager.h"

static int frame = 0;

//	�R���X�g���N�^
CAnimation::CAnimation()
	: m_nAnim(0)
{

}
//	�f�X�g���N�^
CAnimation::~CAnimation()
{
	m_AnimationList.clear();
}
//	������
void CAnimation::Start()
{

}
//	�X�V
void CAnimation::Update()
{
	// ���f���`��@�\�̒�~
	for (auto animation : m_AnimationList)
	{
		animation.second->m_bDrawFlag = false;
	}

	// ��������f���`��@�\��ON�ɂ���
	switch (m_nAnim)
	{
	case 0:	m_AnimationList[0]->m_bDrawFlag = true;	break;	//	idle
	case 1:	m_AnimationList[1]->m_bDrawFlag = true;	break;	//	���ɐ�����Ă���
	default:
		break;
	}
}
//	�`��
void CAnimation::Draw()
{
#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));

	Begin("Player");
	Text("Animation : %d", m_nAnim);
	Text("AimFrame  : %d", frame);
	//Text("Duration  : %lf", m_AnimationList[m_nAnim]->GetAnimDuration());
	End();
#endif // _DEBUG

}


void CAnimation::ChangeAnimation(int anim)
{
	m_nAnim = anim;
}

//	�A�j���[�V�����C���f�b�N�X�ɓo�^
void CAnimation::AddAnimation(CDrawModel* component, int number)
{
	//*** map�ɒǉ�
	m_AnimationList[number] = component;
}

// �w�肵���ԍ��̃��f���`��@�\���擾�ł���
CDrawModel* CAnimation::GetMoel(int fbx_number)
{
	return m_AnimationList[fbx_number];
}