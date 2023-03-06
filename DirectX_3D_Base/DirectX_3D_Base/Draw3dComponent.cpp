#include "Draw3dComponent.h"
#include "TransformComponent.h"
#include "Animation.h"

#include "InputManager.h"
#include "ModelManager.h"
#include "ObjectInfo.h"
#include "sceneGame.h"

// コンストラクタ
CDrawModel::CDrawModel()
	: m_pTransform(nullptr), m_pAnim(nullptr), m_pMaterial(nullptr)
{
	this->m_eUpdateOrder = COM_UPDATE_DRAW;


	//mat.Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// アンビエント+テクスチャ有無
	//mat.Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// ディフューズ
	//mat.Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// スペキュラ+スペキュラ強度
	//mat.Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// エミッシブ
	//mat.pTexture = nullptr;
	//mat.pTexEmmisive = nullptr;
	//mat.pTexSpecular = nullptr;
	//mat.pTexTransparent = nullptr;
}

// デストラクタ
CDrawModel::~CDrawModel()
{
	// 仮措置
	if (Parent->GetName() == ICE_BLOCK_NAME)
	{
		// 仮措置
		SceneGame::IceManager[this->GetModel()] = false;
	}

	if (m_pMaterial)
	{
		delete m_pMaterial;
		m_pMaterial = m_pMaterial;
	}

	if (m_pAnim)
	{
		delete m_pAnim;
		m_pAnim = nullptr;
	}
}

// 開始
void CDrawModel::Start()
{
	// オブジェクトの座標を取得
	m_pTransform = Parent->GetComponent<CTransform>();
}

// 更新
void CDrawModel::Update()
{
	if (m_pAnim) m_pAnim->Update();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	//	拡縮を反映
	mtxScale = XMMatrixScaling(
		m_pTransform->Scale.x,
		m_pTransform->Scale.y,
		m_pTransform->Scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	// X回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		0.0f,
		0.0f);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	// Y回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.y),
		0.0f);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	// Z回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.z));
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);



	// 移動を反映
	mtxTranslate = XMMatrixTranslation(
		m_pTransform->Pos.x,
		m_pTransform->Pos.y,
		m_pTransform->Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);
}

// 描画
void CDrawModel::Draw()
{
#ifdef _DEBUG
	if (m_pAnim)
	{
		m_pAnim->Draw();
	}
#endif // _DEBUG


	ID3D11DeviceContext* pDC = GetDeviceContext();	//	デバイスコンテキストの取得

	// 不透明部分を描画
	CLight::Get()->SetDisable();	//	ライトOFF
	SetBlendState(BS_NONE);			//	アルファブレンド有効
	SetZBuffer(true);				//	Zバッファ有効(Zチェック有&Z更新有)
	m_model->Draw(pDC, m_mtxWorld, eOpacityOnly);
	
	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	//	アルファブレンド有効
	SetZWrite(false);				//	Zバッファ更新しない
	m_model->Draw(pDC, m_mtxWorld, eTransparentOnly);
	
	
	CLight::Get()->SetEnable();		//	ライトOFF
	SetZWrite(true);				//	Zバッファ更新する
	SetBlendState(BS_NONE);			//	アルファブレンド無効
}



// モデルのプリミティブ設定
void CDrawModel::SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type)
{
	if (m_model != nullptr)
		m_model->SetPrimitiveType(type);
}

// ワールド行列の取得
XMFLOAT4X4* CDrawModel::GetWorldPos()
{
	if (m_model != nullptr)
	{
		return &(m_model->GetWorldMatrix());
	}
	return nullptr;
}

// テクスチャの設定
void CDrawModel::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	if (pTexture)
	{
		if (!m_pMaterial)
		{
			m_pMaterial = new TAssimpMaterial();
		}
		m_pMaterial->pTexture = pTexture;
		//m_pMaterial->pTexture->AddRef();		//	参照カウンタ＋１
		m_model->SetMaterial(m_pMaterial);		// モデルにテクスチャをセット
	}
}

// 大きさの取得
XMFLOAT3 CDrawModel::GetBox()
{
	return m_model->GetBBox();
}




CAssimpModel* CDrawModel::GetModel()
{
	return m_model;
}


//----------------------アニメーション関連
//	モデル情報の設定
void CDrawModel::SetModel(CAssimpModel* model)
{
	// モデルのセット
	m_model = model;

	// アニメーションがあったら追加
	if (model->HasAnimation())
	{
		// アニメーションクラス作成
		if (!m_pAnim)	m_pAnim = new Animation();

		// モデル情報を送る
		m_pAnim->SetModel(this);
	}

}

// アニメーション追加(id, 開始時間、終了時間、ロープの始まり時間)
void CDrawModel::AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame)
{
	if (m_pAnim)
	{
		m_pAnim->AddAnimation(AnimID, Start, end, loop, loopStartFrame);
	}
}

// アニメーション再生
void CDrawModel::Play(int AnimID)
{
	if (m_pAnim)
	{
		m_pAnim->Play(AnimID);
	}
}

// アニメーションのフレーム数設定
void CDrawModel::SetAnimTime(double dTime)
{
	m_model->SetAnimTime(dTime);
}

double CDrawModel::GetAnimDuration()
{
	// ？
	return 0;
}

void CDrawModel::AlwaysAnimation(bool always)
{
	if (m_pAnim)
	{
		m_pAnim->AlwaysAnimation(always);
	}
}

// 再生速度
void CDrawModel::SetPlaySpeed(int AnimID, double speed)
{
	if (m_pAnim)
	{
		m_pAnim->SetPlaySpeed(AnimID, speed);
	}
}

bool CDrawModel::AnimEnd(int AnimID)
{
	if (m_pAnim)
	{
		return m_pAnim->AnimEnd(AnimID);
	}
	else return false;
}
