#include "NumberCom.h"
#include "polygon.h"
#include "TransformComponent.h"

CDrawNumber::CDrawNumber()
	: m_nSpace(30)
	, m_pNum(nullptr)
	, m_nNum(0)
{
	m_nAnimSplitY = 1;
	m_nAnimSplitX = 10;
	m_fTexFrameSize.x = 1.0f / m_nAnimSplitX;
	m_fTexFrameSize.y = 1.0f / m_nAnimSplitY;
}

CDrawNumber::~CDrawNumber()
{
}

void CDrawNumber::Update()
{
	m_nNumbers.clear();
	int num;
	int tmp = 0;

	// ポインタの場合
	if(m_pNum)	tmp = *m_pNum;
	// ポインタではない場合
	else tmp = m_nNum;

	for (; tmp != 0;)
	{
		num = tmp % 10;
		m_nNumbers.push_back(num);

		tmp /= 10;
	}


}
void CDrawNumber::Draw()
{
	//	デバイスコンテキストの取得
	ID3D11DeviceContext* pDC = GetDeviceContext();
	int i = 0;
	for (auto&& number : m_nNumbers)
	{
		SetZBuffer(false);						//	Zバッファを更新しない
		SetBlendState(m_BlendState);			//	アルファブレンド有効

		SetPolygonTexture(m_pTexture);								//	テクスチャ
		SetPolygonPos(m_pTransform->Pos.x - i * m_nSpace, m_pTransform->Pos.y);	//	座標
		SetPolygonSize(m_fPolygonSize.x, m_fPolygonSize.y);			//	サイズ
		SetPolygonFrameSize(m_fTexFrameSize.x, m_fTexFrameSize.y);	//	テクスチャ割り当て
		SetPolygonUV(
			(number % m_nAnimSplitX) / (float)m_nAnimSplitX,
			(number / m_nAnimSplitX) / (float)m_nAnimSplitY);	//	アニメーション番号
		SetPolygonColor(m_fVertexColor.x,
			m_fVertexColor.y, m_fVertexColor.z);					//	色
		SetPolygonAlpha(m_fVertexColor.w);							//	不透明度
		SetPolygonAngle(m_fAngle);									//	角度

		//	描画
		DrawPolygon(pDC);

		i++;
	}

	//	設定を元に戻す
	SetPolygonColor(1, 1, 1);
	SetPolygonAngle(0.0f);
	SetPolygonAlpha(1.0f);							//	不透明度
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}


void CDrawNumber::SetNum(int* num)
{
	m_pNum = num;
}

void CDrawNumber::SetNum(int num)
{
	m_nNum = num;
}