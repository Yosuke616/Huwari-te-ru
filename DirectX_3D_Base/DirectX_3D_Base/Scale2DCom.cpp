#include "Scale2DCom.h"
#include "Draw2dComponent.h"


#define TIME	(100)

// コンストラクタ
CScale2D::CScale2D()
	: m_pDraw2D(nullptr)
	, m_vDestSize(0.0f, 0.0f)
	, m_vSize(0.0f, 0.0f)
{
}

// デストラクタ
CScale2D::~CScale2D() {
}

// 初期化
void CScale2D::Start()
{
	// オブジェクトの座標を取得
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

// 更新
void CScale2D::Update()
{
	if (!m_pDraw2D) return;

	// 大きくしていく
	m_vSize.x = m_vDestSize.x * (m_nTime / (float)TIME);
	m_vSize.y = m_vDestSize.y * (m_nTime / (float)TIME);

	// 大きさ反映
	m_pDraw2D->SetSize(m_vSize.x, m_vSize.y);

	// 目的の座標に着いたら
	if (m_nTime >= TIME)
	{
		// このコンポーネントを削除する
		this->Delete();
		return;
	}

	m_nTime++;
}

// 描画
void CScale2D::Draw() {
}


// 移動目的の座標の設定
void CScale2D::SetDestSize(XMFLOAT2 size)
{
	m_vDestSize = size;
}

