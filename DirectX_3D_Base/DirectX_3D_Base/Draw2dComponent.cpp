//===== インクルード =====
#include "Draw2dComponent.h"
#include "TransformComponent.h"
#include "polygon.h"
#include "Light.h"
#include "ObjectManager.h"

// コンストラクタ
CDraw2D::CDraw2D()
	:m_pTexture(0), m_fAngle(0.0f)
	, m_nAnimNum(0) , m_nAnimSplitX(1), m_nAnimSplitY(1)
	, m_BlendState(BS_ALPHABLEND)

{
	m_fPolygonSize = XMFLOAT2(0.0f, 0.0f);
	m_fTexFrameSize = XMFLOAT2(1.0f, 1.0f);
	m_fVertexColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Scroll = XMFLOAT2(0.0f, 0.0f);
	m_eUpdateOrder = COM_UPDATE_DRAW;
}

// デストラクタ
CDraw2D::~CDraw2D()
{
}

// 開始
void CDraw2D::Start(void)
{
	m_pTransform = Parent->GetComponent < CTransform>();
}

// 更新
void CDraw2D::Update(void)
{
}

//	セット関数 テクスチャ
void CDraw2D::SetTexture(ID3D11ShaderResourceView* pTex)
{
	m_pTexture = pTex;
}

//	セット関数 アニメーション番号
void CDraw2D::SetAnimNum(int anim)
{
	m_nAnimNum = anim;
}



//	セット関数 UVサイズ
void CDraw2D::SetUVsize(float UVSizeX, float UVSizeY)
{
	m_fTexFrameSize.x = UVSizeX;
	m_fTexFrameSize.y = UVSizeY;
}

//	セット関数 UVのU
void CDraw2D::SetU(float u)
{
	m_fTexFrameSize.x = u;
}

//	セット関数 UVのV
void CDraw2D::SetV(float v)
{
	m_fTexFrameSize.y = v;
}

//	セット関数 ポリゴンサイズ
void CDraw2D::SetSize(float SizeX, float SizeY)
{
	m_fPolygonSize.x = SizeX;
	m_fPolygonSize.y = SizeY;
}

//	セット関数 カラー
void CDraw2D::SetColor(float red, float green, float blue)
{
	m_fVertexColor.x = red;
	m_fVertexColor.y = green;
	m_fVertexColor.z = blue;
}

//	セット関数 アルファ値
void CDraw2D::SetAlpha(float alpha)
{
	m_fVertexColor.w = alpha;
}

//	セット関数	 角度
void CDraw2D::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}


//	セット関数 分割数セット
void CDraw2D::SetAnimSplit(int x, int y)
{
	m_nAnimSplitX = x;
	m_nAnimSplitY = y;
	m_fTexFrameSize.x = 1.0f / x;
	m_fTexFrameSize.y = 1.0f / y;
}

//	セット関数 スクロール量
void CDraw2D::SetScroll(XMFLOAT2 scroll)
{
	m_Scroll.x = scroll.x;
	m_Scroll.y = scroll.y;
}

//	セット関数 ブレンドステート
void CDraw2D::SetBlend(EBlendState state)
{
	m_BlendState = state;
}

// げっと関数
int CDraw2D::GetAnimSplitX()
{
	return m_nAnimSplitX;
}

// 描画
void CDraw2D::Draw(void)
{
	//	デバイスコンテキストの取得
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);						//	Zバッファを更新しない
	//SetZWrite(false);
	//CLight::Get()->SetEnable();
	SetBlendState(m_BlendState);			//	アルファブレンド有効

	SetPolygonTexture(m_pTexture);								//	テクスチャ
	SetPolygonPos(m_pTransform->Pos.x, m_pTransform->Pos.y);	//	座標
	SetPolygonSize(m_fPolygonSize.x, m_fPolygonSize.y);			//	サイズ
	SetPolygonFrameSize(m_fTexFrameSize.x, m_fTexFrameSize.y);	//	テクスチャ割り当て
	SetPolygonUV(
		(m_nAnimNum % m_nAnimSplitX) / (float)m_nAnimSplitX + m_Scroll.x,
		(m_nAnimNum / m_nAnimSplitX) / (float)m_nAnimSplitY + m_Scroll.y);	//	アニメーション番号
	SetPolygonColor(m_fVertexColor.x,
		m_fVertexColor.y, m_fVertexColor.z);					//	色
	SetPolygonAlpha(m_fVertexColor.w);							//	不透明度
	SetPolygonAngle(m_fAngle);									//	角度
	
	//	描画
	DrawPolygon(pDC);

	//	設定を元に戻す
	SetPolygonColor(1, 1, 1);
	SetPolygonAngle(0.0f);
	SetPolygonAlpha(1.0f);							//	不透明度
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}




//effect
CDrawEffect2D::CDrawEffect2D()
	: m_nAnimCount(1)
	, m_nFrame(10)
	, m_bLoop(false)
	, m_bPlay(true)
{
}

CDrawEffect2D::~CDrawEffect2D(){
}

//	更新
void CDrawEffect2D::Update()
{
	//	カウンタ更新
	if (m_bPlay == true)
	{
		//	カウンタ更新
		--m_nAnimCount;
	}

	if (m_nAnimCount <= 0)
	{
		//	アニメーション切り替え
		++m_nAnimNum;

		//	最後のアニメーションの場合
		if (m_nAnimNum >= m_nAnimSplitX * m_nAnimSplitY)
		{
			if (m_bLoop)
			{
				//	アニメーションリセット
				m_nAnimNum = 0;
			}
			else
			{
				//	オブジェクト削除
				Parent->Delete();
			}
		}
		m_nAnimCount = m_nFrame;
	}
}

//setter
void CDrawEffect2D::SetSwapFrame(int nCount)
{
	m_nAnimCount = m_nFrame = nCount;
}
void CDrawEffect2D::SetLoop(bool loop)
{
	m_bLoop = loop;
}
void CDrawEffect2D::SetPlayONOFF(bool onoff)
{
	m_bPlay = onoff;
}

/**
* @fn		CDrawEffect::StartEffect
* @brief	エフェクト描画開始
* @param[in]	(ID3D11ShaderResourceView*)	pTex テクスチャ
* @param[in]	(XMFLOAT3)	pos					 オブジェクトの座標
* @param[in]	(XMFLOAT2)	size				 オブジェクトの描画サイズ
* @param[in]	(int)		split_x				 テクスチャの分割数X
* @param[in]	(int)		split_y				 テクスチャの分割数Y
* @param[in]	(int)		swap_frame			 テクスチャ切り替えフレーム数
* @param[in]	(XMFLOAT3)	color				 色
* @detail
*			色のデフォルト設定は(1.0f,1.0f,1.0f)
*			テクスチャ切り替えフレーム数のデフォルト設定は1フレーム
*/
void CDrawEffect2D::StartEffect(ID3D11ShaderResourceView* pTex,
	XMFLOAT2 pos, XMFLOAT2 size, int split_x, int split_y, int swap_frame, XMFLOAT3 color)
{
	// generate
	Object* pEffect = new Object("effect", UPDATE_EFFECT, DRAW_UI);
	// components
	auto trans = pEffect->AddComponent<CTransform>();	// オブジェクト座標
	auto draw = pEffect->AddComponent<CDrawEffect2D>();	// エフェクト描画
	// settiongs
	trans->Pos.x = pos.x;								// X座標
	trans->Pos.y = pos.y;								// Y座標
	draw->SetSize(size.x, size.y);						// 描画サイズ
	draw->SetColor(color.x, color.y, color.z);			// 色
	draw->SetLoop(false);								// ループなしに設定
	draw->SetTexture(pTex);								// テクスチャ
	draw->SetAnimSplit(split_x, split_y);				// テクスチャの分割数
	draw->SetSwapFrame(swap_frame);						// テクスチャ切り替えフレーム数
	// add to list
	ObjectManager::GetInstance()->AddObject(pEffect);
}