#include "Animation.h"
#include "Draw3dComponent.h"
#include "InputManager.h"
#include <time.h>
#include "imgui.h"

#define DEFAULT_PLAY_SPEED	(0.024)

// コンストラクタ
Animation::Animation()
	: m_pDraw3D(nullptr)
	, m_dStart(0), m_dEnd(0), m_dCurrentAnimTime(0)
	, m_bLoop(false), m_bAnimation(false), m_bIdle(false)
{
}

// デストラクタ
Animation::~Animation(){
}


// 更新
void Animation::Update()
{
#ifdef _DEBUG
	if (m_pDraw3D && m_pDraw3D->Parent->GetName() == PLAYER_NAME)
	{
		if (INPUT->GetKeyTrigger(DIK_SPACE))
		{
			m_bAnimation = true;
		}
		if (INPUT->GetKeyTrigger(DIK_V))
		{
			m_dCurrentAnimTime = m_dStart[m_nCurrerntAnimState];
			// アニメーション反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_N))
		{
			m_dCurrentAnimTime -= DEFAULT_PLAY_SPEED;
			// アニメーション反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_M))
		{
			m_dCurrentAnimTime += DEFAULT_PLAY_SPEED;
			// アニメーション反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (INPUT->GetKeyTrigger(DIK_B))
		{
			this->Stop();
		}
	}
#endif // _DEBUG



	// アニメーション再生
	if (m_bAnimation)
	{
		// 数値が謎
		m_dCurrentAnimTime += m_dPlaySpeed[m_nCurrerntAnimState];

		// アニメーション反映
		m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);

		// アニメーションが一周した時
		if (m_dCurrentAnimTime > m_dEnd[m_nCurrerntAnimState])
		{
			if (m_bLoop[m_nCurrerntAnimState])
			{
				// アニメーションの始まりへ
				m_dCurrentAnimTime = m_dStart[m_nCurrerntAnimState];
			}
			else
			{
				if (m_bIdle)
				{
					// 終わったことを知らせる
					m_bEnd[m_nCurrerntAnimState] = true;

				}
				else	m_bAnimation = false;	// アニメーション終了
			}
		}
	}
}

// 描画
void Animation::Draw(){
}

// 最初から再生
void Animation::Play(int AnimState)
{
	// 再生フラグON
	m_bAnimation = true;

	// 現在のアニメと次のアニメが一緒の場合は何もしない
	if (m_nCurrerntAnimState == AnimState) return;

	// アニメ切り替え
	m_nCurrerntAnimState = AnimState;

	// アニメーションを始まりのフレーム数に設定
	m_dCurrentAnimTime = m_dStart[AnimState];
}

// 停止
void Animation::Stop()
{
	m_bAnimation = false;	// 再生フラグOFF
}


// ループ再生
void Animation::SetLoop(int AnimID, bool loop)
{
	m_bLoop[AnimID] = loop;
}


// モデル設定
void Animation::SetModel(CDrawModel* model)
{
	m_pDraw3D = model;
}


// アニメーション追加
void Animation::AddAnimation(int AnimID, double start, double end, bool loop, double loopStartFrame)
{
	m_dStart[AnimID] = start;
	m_dEnd[AnimID] = end;
	m_bLoop[AnimID] = loop;
	m_dPlaySpeed[AnimID] = DEFAULT_PLAY_SPEED;
	m_bEnd[AnimID] = false;
}

void Animation::AlwaysAnimation(bool always)
{
	m_bIdle = always;
}
void Animation::SetPlaySpeed(int AnimID, double speed)
{
	m_dPlaySpeed[AnimID] = speed;
}
bool Animation::AnimEnd(int AnimID)
{
	if (m_bEnd[AnimID])
	{
		// 終わりフラグのリセット
		m_bEnd[AnimID] = false;

		return true;
	}
	else return false;
}
