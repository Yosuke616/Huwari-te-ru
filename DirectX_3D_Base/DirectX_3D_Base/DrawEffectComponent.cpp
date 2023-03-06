#include "DrawEffectComponent.h"
#include "TransformComponent.h"
#include "Light.h"
#include "Camera.h"
#include "ObjectManager.h"

// コンストラクタ
CDrawEffect::CDrawEffect()
	: m_nAnimCount(10)
	, m_nFrame(10)
	, m_bLoop(false)
{
}

// デストラクタ
CDrawEffect::~CDrawEffect()
{
}

// 頂点情報作成
void CDrawEffect::CreateEffectVertex()
{
	// ワールドマトリックス初期化
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	// 一時的な頂点配列を生成
	m_Mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];
	VERTEX_3D* pVtx = pVertexWk;
	pVtx[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVtx[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);
	m_Mesh.nNumIndex = 4;
	int* pIndexWk = new int[m_Mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	HRESULT hr = MakeMeshVertex(GetDevice(), &m_Mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

	if(FAILED(hr)) MessageBox(GetMainWnd(), _T("CreateEffectVertex"), NULL, MB_OK);
}


// スタート
void CDrawEffect::Start()
{
	m_nAnimCount = m_nFrame;

	m_pTransform = Parent->GetComponent<CTransform>();

	//	頂点の作成
	CreateEffectVertex();
}

// 更新
void CDrawEffect::Update()
{
	//	カウンタ更新
	--m_nAnimCount;
	if (m_nAnimCount <= 0)
	{
		//	アニメーション切り替え
		++m_nAnim;

		//	最後のアニメーションの場合
		if (m_nAnim >= m_nSplitX * m_nSplitY)
		{
			if (m_bLoop)
			{
				//	アニメーションリセット
				m_nAnim = 0;
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

// 描画
void CDrawEffect::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//	光源無効
	SetBlendState(BS_ALPHABLEND);	//	αブレンディング有効

	//	テクスチャ マトリクス更新
	XMMATRIX mtxTex = XMMatrixScaling(1.0f / m_nSplitX, 1.0f / m_nSplitY, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex,
		XMMatrixTranslation(
			m_nAnim % m_nSplitX / (float)m_nSplitX,
			m_nAnim / m_nSplitX / (float)m_nSplitY, 0.0f));
	XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	//	ビュー行列の回転成分の転置行列を設定
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();		//	ビューマトリックスの取得
	m_Mesh.mtxWorld._11 = mView._11 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._12 = mView._21 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._13 = mView._31 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._14 = 0.0f;
	m_Mesh.mtxWorld._21 = mView._12 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._22 = mView._22 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._23 = mView._32 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._24 = 0.0f;
	m_Mesh.mtxWorld._31 = mView._13;
	m_Mesh.mtxWorld._32 = mView._23;
	m_Mesh.mtxWorld._33 = mView._33;
	m_Mesh.mtxWorld._34 = 0.0f;

	//	回転の反映
	XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		XMConvertToRadians(m_pTransform->Rotate.y),
		XMConvertToRadians(m_pTransform->Rotate.z));
	XMMATRIX MeshMatrix = DirectX::XMLoadFloat4x4(&(m_Mesh.mtxWorld));	//	XMFLOAT4x4→XMMATRIX
	MeshMatrix = XMMatrixMultiply(MeshMatrix, mtxRot);					//	MATRIX同士で計算
	DirectX::XMStoreFloat4x4(&m_Mesh.mtxWorld, MeshMatrix);				//	XMMATRIX→XMFLOAT4x4

	//	位置を反映
	m_Mesh.mtxWorld._41 = m_pTransform->Pos.x;
	m_Mesh.mtxWorld._42 = m_pTransform->Pos.y;
	m_Mesh.mtxWorld._43 = m_pTransform->Pos.z;
	m_Mesh.mtxWorld._44 = 1.0f;

	//	描画
	DrawMesh(pDC, &m_Mesh);

	SetBlendState(BS_ALPHABLEND);	//	αブレンディングON
	CLight::Get()->SetEnable();		//	ライトON
}

// アニメーションカウンタ
void CDrawEffect::SetSwapFrame(int nCount)
{
	m_nFrame = nCount;
}

// ループフラグの設定
void CDrawEffect::SetLoop(bool loop)
{
	m_bLoop = loop;
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
void CDrawEffect::StartEffect(ID3D11ShaderResourceView* pTex,
	XMFLOAT3 pos, XMFLOAT2 size , int split_x, int split_y, int swap_frame, XMFLOAT3 color)
{
	// generate
	Object* pEffect = new Object("effect", UPDATE_EFFECT, DRAW_EFFECT);
	// components
	auto trans = pEffect->AddComponent<CTransform>();	// オブジェクト座標
	auto draw = pEffect->AddComponent<CDrawEffect>();	// エフェクト描画
	// settiongs
	trans->Pos = pos;									// 座標
	draw->SetSize(size.x, size.y);						// 描画サイズ
	draw->SetColor(color.x, color.y, color.z);			// 色
	draw->SetLoop(false);								// ループなしに設定
	draw->SetTexture(pTex);								// テクスチャ
	draw->SetAnimSplit(split_x, split_y);				// テクスチャの分割数
	draw->SetSwapFrame(swap_frame);						// テクスチャ切り替えフレーム数
	// add to list
	ObjectManager::GetInstance()->AddObject(pEffect);
}