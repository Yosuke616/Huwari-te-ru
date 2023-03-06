#include "LotationComponent.h"
#include "Draw2dComponent.h"

// コンストラクタ
CLotation::CLotation()
	: m_pDraw2D(nullptr)
	, m_nDegree(0)
{

}

// デストラクタ
CLotation::~CLotation()
{

}

// 初期化
void CLotation::Start()
{
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// 更新
void CLotation::Update()
{
	// 角度の設定
	if (m_pDraw2D) m_pDraw2D->SetAngle((float)m_nDegree);

	// 角度の更新
	m_nDegree -= 3;
	if (m_nDegree < 0) m_nDegree += 360;
}