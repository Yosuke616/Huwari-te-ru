#include "AnimationComponent.h"
#include "imgui.h"
#include "Draw3dComponent.h"
#include "InputManager.h"

static int frame = 0;

//	コンストラクタ
CAnimation::CAnimation()
	: m_nAnim(0)
{

}
//	デストラクタ
CAnimation::~CAnimation()
{
	m_AnimationList.clear();
}
//	初期化
void CAnimation::Start()
{

}
//	更新
void CAnimation::Update()
{
	// モデル描画機能の停止
	for (auto animation : m_AnimationList)
	{
		animation.second->m_bDrawFlag = false;
	}

	// 一つだけモデル描画機能をONにする
	switch (m_nAnim)
	{
	case 0:	m_AnimationList[0]->m_bDrawFlag = true;	break;	//	idle
	case 1:	m_AnimationList[1]->m_bDrawFlag = true;	break;	//	風に吹かれている
	default:
		break;
	}
}
//	描画
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

//	アニメーションインデックスに登録
void CAnimation::AddAnimation(CDrawModel* component, int number)
{
	//*** mapに追加
	m_AnimationList[number] = component;
}

// 指定した番号のモデル描画機能を取得できる
CDrawModel* CAnimation::GetMoel(int fbx_number)
{
	return m_AnimationList[fbx_number];
}