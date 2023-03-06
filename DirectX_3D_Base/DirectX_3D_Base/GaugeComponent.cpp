//　インクルード部
#include "GaugeComponent.h"
#include "WindManager.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"



//  コンストラクタ
CGauge::CGauge()
	: m_pTransform(nullptr), m_pDraw(nullptr)
	, m_nMaxGauge(0), m_nCurrentGauge(0)
{
}

//  デストラクタ
CGauge::~CGauge() {
}

//　初期化
void CGauge::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw2D>();
}

//　更新処理
void CGauge::Update()
{
	if (m_pTransform)
	{
		//	x座標				= ベース位置			+ (横幅 / 2)			   * HP割合			- (横幅 / 2)
		m_pTransform->Pos.x = m_InitPosition.x  
			+ (m_MaxSize.x * 0.5f) * (m_nCurrentGauge / (float)m_nMaxGauge) - (m_MaxSize.x * 0.5f);
	}

	//	描画の更新
	if (m_pDraw)
	{
		m_pDraw->SetSize(m_MaxSize.x * (m_nCurrentGauge / (float)m_nMaxGauge), m_MaxSize.y);
		m_pDraw->SetU(m_nCurrentGauge / (float)m_nMaxGauge * 1.0f / m_pDraw->GetAnimSplitX());
	}
}

//　初期ポジションをセットする
void CGauge::SetInitPos(float fx,float fy)
{
	m_InitPosition.x = fx;
	m_InitPosition.y = fy;
}

//　大きさをセットする
void CGauge::SetSize(float fx, float fy)
{
	m_MaxSize.x = fx;
	m_MaxSize.y = fy;
}

//　ゲージの最大値をセットする
void CGauge::SetMaxGauge(int max)
{
	m_nMaxGauge = m_nCurrentGauge  = max;
}

//	ゲージを減らす関数
void CGauge::CalcGauge(CGauge* pGauge,int value)
{
	if (!pGauge) return;
	pGauge->m_nCurrentGauge += value;

	if (pGauge->m_nCurrentGauge < 0)					pGauge->m_nCurrentGauge = 0;
	if (pGauge->m_nCurrentGauge > pGauge->m_nMaxGauge)	pGauge->m_nCurrentGauge = pGauge->m_nMaxGauge;
}

// ゲージ量を設定する
void CGauge::SetGauge(CGauge* pGauge, int value)
{
	if(pGauge) pGauge->m_nCurrentGauge = value;
}

int CGauge::GetCurrentGauge(CGauge* pGauge)
{
	if (!pGauge) return 0;
	return pGauge->m_nCurrentGauge;
}
