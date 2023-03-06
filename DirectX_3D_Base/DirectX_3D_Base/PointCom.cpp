#include "PointCom.h"
#include "TransformComponent.h"

#define VELOCITY		(1.0f)


//	平方根(正確ではない)
float squareRoot2(float x);
//	2乗関数
float pow2(float x);

// コンストラクタ
CPoint::CPoint()
	: m_pTransform(nullptr)
	, m_vDestPos(0.0f, 0.0f)
{
}

// デストラクタ
CPoint::~CPoint(){
}

// 初期化
void CPoint::Start()
{
	// オブジェクトの座標を取得
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CPoint::Update()
{
	if (!m_pTransform) return;

	// 現在座標と目的座標の差を計算
	XMFLOAT2 diff;
	diff.x = m_pTransform->Pos.x - m_vDestPos.x;
	diff.y = m_pTransform->Pos.y - m_vDestPos.y;


	float fDistance = squareRoot2(pow2(diff.x) + pow2(diff.y));

	// 目的の座標に着いたら
	if (fDistance < 1)
	{
		m_pTransform->Vel.x = 0.0f;
		m_pTransform->Vel.y = 0.0f;

		// このコンポーネントを削除する
		this->Delete();
		return;
	}


	//--- 移動方向の計算
	XMVECTOR xmvDiff = XMLoadFloat2(&diff);	//	XMFLOAT2をXMVECTOR型にを変換
	xmvDiff = XMVector2Normalize(xmvDiff);		//	ベクトルの正規化
	XMFLOAT2 vDiff;									//	正規化されたXMVECTOR型をXMFLOAT2型に入れる箱の用意
	DirectX::XMStoreFloat2(&vDiff, xmvDiff);	//	XMVECTORをXMFLOAT2に変換
	//	ラジアンで角度を出してくれる
	float fRad = atan2f(vDiff.y, vDiff.x);			//	ラジアン	(πの方)

	m_pTransform->Vel.x = -VELOCITY * cosf(fRad);
	m_pTransform->Vel.y = -VELOCITY * sinf(fRad);
}

// 描画
void CPoint::Draw(){
}


// 移動目的の座標の設定
void CPoint::SetDestPos(XMFLOAT2 pos)
{
	m_vDestPos = pos;
}

//	平方根(正確ではない)
float squareRoot2(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2乗関数
float pow2(float x)
{
	return x * x;
}
