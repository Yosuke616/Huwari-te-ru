#include "FollowCompomemt.h"
#include "TransformComponent.h"

//*** コンストラクタ
CFollow::CFollow()
	: m_pTransform(nullptr), m_pFollowObject(nullptr)
{
}

//*** デストラクタ
CFollow::~CFollow(){
}

//*** 開始
void CFollow::Start()
{
	// オブジェクトの座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
}

//*** 更新
void CFollow::Update()
{
	// ついていく対象がいなくなった場合消す
	if (m_pFollowObject->GetDeleteFlag())
	{
		Parent->Delete();
		return;
	}

	// 対象のオブジェクトの速度を加える
	if (m_pFollowObject)
	{
		auto pFollowTransform = m_pFollowObject->GetComponent<CTransform>();
		if (pFollowTransform)
		{
			m_pTransform->Pos.x += pFollowTransform->Vel.x;
			m_pTransform->Pos.y += pFollowTransform->Vel.y;
			m_pTransform->Pos.z += pFollowTransform->Vel.z;
		}
	}
}

//*** 描画
void CFollow::Draw(){
}

//*** 衝突処理
void CFollow::OnCollisionEnter(Object* pObject){
}

//*** このオブジェクトがついていく対象の座標の設定
void CFollow::SetParent(Object* pParent)
{
	m_pFollowObject = pParent;
}