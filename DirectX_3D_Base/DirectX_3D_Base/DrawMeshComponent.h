/**
 * @file	DrawMeshComponent.h
 * @brief	メッシュ描画コンポーネント
 * @author	AT12A 成田陸人
 * @date	2021/12/01
 */

 //===== インクルードガード ======
#ifndef __DRAW_MESH_H__
#define __DRAW_MESH_H__

//===== インクルード =====
#include "Component.h"
#include "mesh.h"
#include "main.h"

//===== 前方宣言 =====
class CTransform;


//===== クラス定義 =====
class CDrawMesh : public Component
{
	//=== メンバ変数
private:
	float m_fTexSizeX;
	float m_fTexSizeZ;

protected:
	XMFLOAT2 m_Offset;			//	オフセット
	float	m_fWidth;				//	1マスの横幅
	float	m_fHeight;				//	1マスの縦幅
	XMFLOAT3		m_fNormal;		//	法線ベクトル
	MESH			m_Mesh;			//	メッシュ
	MATERIAL		m_Material;		//	マテリアル
	CTransform*		m_pTransform;	//	座標
	int m_nAnim;					//	アニメーション番号
	int m_nSplitX;					//	分割数X
	int m_nSplitY;					//	分割数Y


	//=== メンバ関数
private:
	//	頂点情報作成
	HRESULT MakeVertex(
		//int nNumBlockX, int nNumBlockZ,
		float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ,
		XMFLOAT3 normal);

protected:

public:
	CDrawMesh();					//	コンストラクタ
	~CDrawMesh() override;			//	デストラクタ
	void Start() override;			//	スタート
	void Update() override;			//	更新
	void Draw() override;			//	描画
	//setter
	void SetTexture(ID3D11ShaderResourceView* pTex);//	テクスチャ
	void SetUVsize(XMFLOAT2 uv);					//	テクスチャ座標
	void SetSize(float width, float height);		//	マスの大きさ
	void SetDiffuse(XMFLOAT4 Diffuse);				//	反射光
	void SetAmbient(XMFLOAT4 Ambient);				//	環境光
	void SetSpecular(XMFLOAT4 Specular);			//	鏡面反射の設定
	void SetEmissive(XMFLOAT4 Emissive);			//	自己発光の設定
	void SetPower(float power);						//	光沢の設定
	void SetNormal(float fx, float fy, float fz);	//	法線ベクトルの設定
	void SetAnimFrame(int anim);					//	アニメーション番号
	void SetAnimSplit(int nx, int ny);				//	分割数の設定
	void SetOffset(XMFLOAT2 offset);					//	オフセット
	void SetColor(float x, float y, float z);	// 色

	//getter
	MESH* GetMesh();								//	メッシュ情報の取得
};


class CSphere : public CDrawMesh
{
	//=== メンバ変数
private:
	static bool m_bVisible;		//	見えるかどうか
	float m_fRadius;			//	球の半径
	XMFLOAT4X4* m_pWorld;		//	ワールド座標
	
	//=== メンバ関数
private:
	void InitSphere();								//	頂点情報の作成

public:
	CSphere();										//	コンストラクタ
	~CSphere() override;								//	デストラクタ
	void Start() override;								//	スタート
	void Update() override;								//	更新
	void Draw() override;								//	描画
	//setter
	void SetRadius(float rad);							//	球の半径
	void SetTransform(CTransform* pTransform);			//	表示する座標
	void SetWorldPos(XMFLOAT4X4* pWorld);				//	ワールド座標
	

	static void SetSphereVisible(bool visible = true);	//	表示フラグの設定
	static void CreateSphere(CTransform* transform, float radius, XMFLOAT4X4* world);	//	デバッグ球の生成
};
#endif // __DRAW_MESH_H__