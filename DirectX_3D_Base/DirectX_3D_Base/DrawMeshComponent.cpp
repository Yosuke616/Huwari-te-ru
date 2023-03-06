#include "DrawMeshComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Light.h"
#include "ObjectManager.h"
#include "Object.h"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(10.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

// コンストラクタ
CDrawMesh::CDrawMesh()
	:m_pTransform(nullptr)					//	座標
	, m_fWidth(100.0f), m_fHeight(100.0f)	//	大きさ
	, m_Offset(0.0f, 0.0f)
	, m_nAnim(0)							//	アニメーションフレーム
	, m_nSplitX(1), m_nSplitY(1)			//	分割数
	, m_fTexSizeX(1.0f), m_fTexSizeZ(1.0f)
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	m_Mesh.primitiveType = PT_TRIANGLESTRIP;
	// ワールドマトリックス初期化
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	// 位置・回転の初期設定
	m_Mesh.pos	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Mesh.rot	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fNormal	= XMFLOAT3(0.0f, -1.0f, 0.0f);

	// マテリアルの初期設定
	m_Material.Diffuse	= M_DIFFUSE;
	m_Material.Ambient	= M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power	= M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial	= &m_Material;
}

// デストラクタ
CDrawMesh::~CDrawMesh()
{
	// メッシュ解放
	ReleaseMesh(&m_Mesh);
}

// 開始
void CDrawMesh::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();

	// 頂点情報の作成
	 MakeVertex(
		//m_NumX, m_NumZ,
		m_fWidth, m_fHeight,
		m_fTexSizeX, m_fTexSizeZ,
		m_fNormal);

	 CDrawMesh::Update();
}

// 更新
void CDrawMesh::Update()
{
	m_Mesh.pos.x = m_pTransform->Pos.x + m_Offset.x;
	m_Mesh.pos.y = m_pTransform->Pos.y + m_Offset.y;
	m_Mesh.pos.z = m_pTransform->Pos.z;
	m_Mesh.rot = m_pTransform->Rotate;

	// メッシュ更新
	UpdateMesh(&m_Mesh);
}

// 描画
void CDrawMesh::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//static float u = 0.0f;
	//	u += 0.01f;
	////	テクスチャ マトリクス更新
	//XMMATRIX mtxTex = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation(u / 4, 1.0f, 0.0f));
	//XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	// メッシュの描画
	DrawMesh(pDC, &m_Mesh);
}


 /**
 * @fn		CDrawMesh::MakeVertex
 * @brief	頂点情報の作成
 * @param	(float) 横の大きさ
 * @param	(float) 縦の大きさ
 * @param	(float) UVの横幅(0～1)
 * @param	(float) UVの縦幅(0～1)
 * @param	(XMFLOAT3) 法線ベクトル
 * @return	成功情 S_OK
 */
HRESULT CDrawMesh::MakeVertex(
	float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ,
	XMFLOAT3 normal)
{
	//	プリミティブ種別設定
	m_Mesh.primitiveType = PT_LINE;

	//	頂点数の設定
	m_Mesh.nNumVertex = (1 + 1) * (1 + 1);

	//	インデックス数の設定(縮退ポリゴン用を考慮する)
	m_Mesh.nNumIndex = 8;

	//	頂点配列の作成
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	//	インデックス配列の作成
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	//	頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	//	頂点座標の設定
	pVtx[0].vtx.x = m_fWidth *-0.5f;
	pVtx[1].vtx.x = m_fWidth * 0.5f;
	pVtx[2].vtx.x = m_fWidth * 0.5f;
	pVtx[3].vtx.x = m_fWidth *-0.5f;
	pVtx[0].vtx.y =  m_fHeight * 0.5f;
	pVtx[1].vtx.y =  m_fHeight *-0.5f;
	pVtx[2].vtx.y =  m_fHeight * 0.5f;
	pVtx[3].vtx.y =  m_fHeight *-0.5f;
	pVtx[0].vtx.z =  0.0f;	pVtx[1].vtx.z = 0.0f;	pVtx[2].vtx.z = 0.0f;  pVtx[3].vtx.z = 0.0f;

	for (int z = 0; z < 2; ++z)
	{
		for (int x = 0; x < 2; ++x)
		{
			//	法線の設定
			pVtx->nor = XMFLOAT3(normal.x, normal.y, normal.z);

			//	反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			//	テクスチャ座標の設定
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//	インデックス配列の中身を埋める
	int* pIdx = pIndexWk;
	pIdx[0] = 0;
	pIdx[1] = 2;

	pIdx[2] = 2;
	pIdx[3] = 1;

	pIdx[4] = 1;
	pIdx[5] = 3;
	
	pIdx[6] = 3;
	pIdx[7] = 0;


	//	頂点バッファ/インデックス バッファ生成
	HRESULT hr = MakeMeshVertex(GetDevice(), &m_Mesh, pVertexWk, pIndexWk);

	//	一時配列の解放
	delete[] pVertexWk;
	delete[] pIndexWk;

	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("メッシュ作成失敗"), NULL, MB_OK);
	}

	return hr;
}

//setter
// メッシュのサイズ設定
void CDrawMesh::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;
}
// テクスチャの設定
void CDrawMesh::SetTexture(ID3D11ShaderResourceView* pTex)
{
	if(pTex)	pTex->AddRef();				//	参照カウンタ＋１
	m_Mesh.pTexture = pTex;
	if (m_Mesh.pTexture)
		XMStoreFloat4x4(&m_Mesh.mtxTexture, XMMatrixIdentity());
}
// 反射光の設定
void CDrawMesh::SetDiffuse(XMFLOAT4 Diffuse)
{
	m_Material.Diffuse = Diffuse;
}
// 環境光の設定
void CDrawMesh::SetAmbient(XMFLOAT4 Ambient)
{
	m_Material.Ambient = Ambient;
}
// 鏡面反射の設定
void CDrawMesh::SetSpecular(XMFLOAT4 Specular)
{
	m_Material.Specular = Specular;
}
// 自己発光の設定
void CDrawMesh::SetEmissive(XMFLOAT4 Emissive)
{
	m_Material.Emissive = Emissive;
}
// 光沢の設定
void CDrawMesh::SetPower(float power)
{
	m_Material.Power = power;
}
// 法線ベクトルの設定
void CDrawMesh::SetNormal(float fx, float fy, float fz)
{
	m_fNormal.x = fx;
	m_fNormal.y = fy;
	m_fNormal.z = fz;
}
// テクスチャ座標
void CDrawMesh::SetUVsize(XMFLOAT2 uv)
{
	m_fTexSizeX = uv.x;
	m_fTexSizeZ = uv.y;
}
// アニメーション番号
void CDrawMesh::SetAnimFrame(int anim)
{
	m_nAnim = anim;
}
// 分割数の設定
void CDrawMesh::SetAnimSplit(int nx, int ny)
{
	m_nSplitX = nx;
	m_nSplitY = ny;
}


void CDrawMesh::SetOffset(XMFLOAT2 offset)
{
	m_Offset = offset;
}

//getter
// メッシュ情報の取得
MESH* CDrawMesh::GetMesh()
{
	return &m_Mesh;
}

// 色
void CDrawMesh::SetColor(float x, float y, float z)
{
	m_Mesh.pMaterial->Diffuse = XMFLOAT4(x, y, z, 1.0f);
}



//===== デバッグ球クラス
bool CSphere::m_bVisible = true;

// コンストラクタ
CSphere::CSphere()
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;
}

// デストラクタ
CSphere::~CSphere()
{
	ReleaseMesh(&m_Mesh);
}

// 初期化
void CSphere::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();

	//	球体の頂点作成
	InitSphere();
}

// 更新
void CSphere::Update()
{
	if (INPUT->GetKeyTrigger(DIK_B))
		m_bVisible = !m_bVisible;
}

// 描画
void CSphere::Draw()
{
	if (!m_bVisible) return;
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//	ライトOFF
	SetBlendState(BS_ADDITIVE);		//	加算合成
	SetZWrite(false);				//	半透明描画はZバッファを更新しない(Zチェックは行う)
	
	// 中心座標を移動
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVector3TransformCoord(XMLoadFloat3(&m_pTransform->Pos), XMLoadFloat4x4(m_pWorld)));
	// サイズを反映
	m_Mesh.mtxWorld._11 = m_Mesh.mtxWorld._22 = m_Mesh.mtxWorld._33 = m_fRadius;
	// 位置を反映
	m_Mesh.mtxWorld._41 = pos.x + m_Offset.x;
	m_Mesh.mtxWorld._42 = pos.y + m_Offset.y;
	m_Mesh.mtxWorld._43 = pos.z;

	// 描画
	DrawMesh(pDC, &m_Mesh);
	
	SetZWrite(true);				//	Zバッファ更新有り
	SetBlendState(BS_NONE);			//	ブレンドステート
	CLight::Get()->SetEnable();		//	ライトON
}

// 半径の設定
void CSphere::SetRadius(float rad)
{
	m_fRadius = rad;
}

// トランスフォームの設定
void CSphere::SetTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
}

// ワールド座標の設定
void CSphere::SetWorldPos(XMFLOAT4X4* pWorld)
{
	m_pWorld = pWorld;
}

 /**
 * @fn		CSphere::SetSphereVisible
 * @brief	球の頂点作成
 * @return	なし
 * @detail	CSphere::Start関数以外では使わない
 */
void CSphere::InitSphere()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// マテリアルの初期設定
	m_Material.Diffuse = M_DIFFUSE;
	m_Material.Ambient = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power = M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial = &m_Material;

	// ワールドマトリックス初期化
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());


	// プリミティブの指定
	m_Mesh.primitiveType = PT_LINE;

	// オブジェクトの頂点配列を生成
	m_Mesh.nNumVertex = 384 * 3;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	for (int axis = 0; axis < 3; ++axis) {
		for (int i = 0; i < 384; ++i, ++pVtx) {
			// 頂点座標の設定
			switch (axis) {
			case 0:
				pVtx->vtx.x = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = 0.0f;
				break;
			case 1:
				pVtx->vtx.y = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = 0.0f;
				break;
			case 2:
				pVtx->vtx.z = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = 0.0f;
				break;
			}

			// 法線の設定
			pVtx->nor = pVtx->vtx;

			// 反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex = XMFLOAT2(0.0f, 0.0f);
		}
	}

	// インデックス配列を生成
	m_Mesh.nNumIndex = (384 * 2) * 3;
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	// インデックス配列の中身を埋める
	for (int axis = 0; axis < 3; ++axis)
	{
		int* pIdx = &pIndexWk[axis * (384 * 2)];
		for (int i = 0; i < 384; ++i)
		{
			*pIdx++ = axis * 384 + i;
			*pIdx++ = axis * 384 + (i + 1) % 384;
		}
	}

	// 頂点バッファ生成
	hr = MakeMeshVertex(pDevice, &m_Mesh, pVertexWk, pIndexWk);

	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("頂点情報作成失敗"), NULL, MB_OK);
	}

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;
}

 /**
 * @fn		CSphere::SetSphereVisible
 * @brief	すべての球体の表示ON/OFF
 * @param	(bool) 球体を表示 true : 非表示 false
 * @return	なし
 */
void CSphere::SetSphereVisible(bool visible)
{
	m_bVisible = visible;
}

/**
* @fn		CSphere::CreateSphere
* @brief	デバッグ用の球体をリストに追加
* @param	(CTransform*) 追尾するオブジェクトのトランスフォーム
* @param	(float) 球体の半径
* @param	(XMFLOAT4x4*) 追尾するオブジェクトのワールド座標
* @return	なし
*/
void CSphere::CreateSphere(CTransform* pTransform, float radius, XMFLOAT4X4* world)
{
	//	オブジェクト生成
	Object* pSphereObject = new Object("Sphere", UPDATE_DEBUG, DRAW_DEBUG);

	//	コンポーネントの追加
	auto transform = pSphereObject->AddComponent<CTransform>();
	auto sphere = pSphereObject->AddComponent<CSphere>();

	//	オブジェクトリストに追加
	ObjectManager::GetInstance()->AddObject(pSphereObject);

	//	オブジェクト情報の設定
	sphere->SetRadius(radius);
	sphere->SetTransform(pTransform);
	sphere->SetWorldPos(world);
}