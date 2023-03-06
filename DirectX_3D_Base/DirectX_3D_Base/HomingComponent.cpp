#include "HomingComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2dComponent.h"
#include "TextureManager.h"
#include "Objectmanager.h"
#include "EffectManager.h"
#include "Camera.h"

//===== マクロ定義 =====
#define CREATE_EFFECT_TIME		(20)
#define CREATE_EFFECT_DIFF		(30)

#define CURSOR_EFFECT_SIZE_X	(100.0f)
#define CURSOR_EFFECT_SIZE_Y	(100.0f)
#define CURSOR_EFFECT_SPLIT_X	(5)
#define CURSOR_EFFECT_SPLIT_Y	(2)
#define CURSOR_EFFECT_FRAME		(3)

InputManager* g_Input = nullptr;
EffectManager* g_Effect = nullptr;

//	平方根(正確ではない)
float squareRoot_Cursor(float x);
//	2乗関数
float pow_Cursor(float x);

/*****

タイトル、ロード画面、セレクト画面　…　CURSOR_EFFECT_NUM(黄色)

春　…　CURSOR_EFFECT_SPRING_NUM(桃色)

夏　…　CURSOR_EFFECT_SUMMER_NUM(緑色)

秋　…　CURSOR_EFFECT_AUTUMN_NUM(茶色)

冬　…　CURSOR_EFFECT_WINTER_NUM(水色)

*****/

//	***コンストラクタ
CHoming::CHoming()
	: m_pTransform(nullptr)
	, m_vCmpMousePos(0.0f, 0.0f)
	, m_nStopTime(0)
	, m_nMoveTime(0)
	, m_nEffectID(CURSOR_EFFECT_NUM)
{
}
//	***デストラクタ
CHoming::~CHoming()
{
}
//	***初期化
void CHoming::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	g_Input = INPUT;
	g_Effect = EffectManager::GetInstance();
}
//	***更新
void CHoming::Update()
{
	// 座標取得
	XMFLOAT2 MousePos = g_Input->GetMousePos();														// マウス座標取得
	CCamera* pCamera = CCamera::Get();																// カメラ取得
	XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
	XMFLOAT3 vWorldPos;
	CalcScreenToXY(&vWorldPos, MousePos.x, MousePos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);		// ワールド座標からスクリーン座標へ
	// カーソルエフェクトの座標
	vWorldPos.z = CURSOR_POS_Z;


	if (Parent->GetName() == ENDING_CLOVER_NAME)
	{
		vWorldPos = Parent->GetComponent<CTransform>()->Pos;
	}
	else
	{
		// 現在の座標を保存
		m_pTransform->Pos.x = MousePos.x;
		m_pTransform->Pos.y = MousePos.y;
	}

	// 時間更新
	m_nStopTime++;				// 止まっているときの時間更新
	m_nMoveTime++;				// 動いているときの時間更新

	// マウスを動かしていなかったら
	if (m_pTransform->Pos.x - m_vCmpMousePos.x == 0 &&
		m_pTransform->Pos.y - m_vCmpMousePos.y == 0)
	{
		// 生成間隔
		if (m_nStopTime > CREATE_EFFECT_TIME)
		{
			// カーソルエフェクト再生
			//g_Effect->Play(m_nEffectID, vWorldPos, XMFLOAT3(3.0f, 3.0f, 3.0f));
			m_nStopTime = 0;														// 時間補正
		}
		return;
	}

	// マウスが動いていたら
	float fDistance = squareRoot_Cursor(pow_Cursor(m_pTransform->Pos.x - m_vCmpMousePos.x) + pow_Cursor(m_pTransform->Pos.y - m_vCmpMousePos.y));

	// 生成間隔
	if (m_nMoveTime > 1)
	{
		// カーソルエフェクト再生
		g_Effect->Play(m_nEffectID, vWorldPos, XMFLOAT3(3.0f, 3.0f, 3.0f));
		m_nMoveTime = 0;														// 時間補正
	}

	// 過去座標更新
	m_vCmpMousePos = XMFLOAT2(m_pTransform->Pos.x, m_pTransform->Pos.y);
}
//	***描画
void CHoming::Draw()
{
}
//	***衝突処理
void CHoming::OnCollisionEnter(Object* pObject)
{
}

void CHoming::SetEffectNum(int nEffectNum)
{
	m_nEffectID = nEffectNum;
}

//	平方根(正確ではない)
float squareRoot_Cursor(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2乗関数
float pow_Cursor(float x)
{
	return x * x;
}