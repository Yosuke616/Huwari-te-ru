//===== インクルード =====
#include "InformationComponent.h"
#include "Object.h"


// コンストラクタ
CInformation::CInformation()
	: m_pObject(nullptr)
{
}

// デストラクタ
CInformation::~CInformation(){
}

// 更新
void CInformation::Update(){
}

// オブジェクト情報を設定
void CInformation::SetObjectInfo(Object* pObject)
{
	m_pObject = pObject;
}

// オブジェクト情報の取得
Object* CInformation::GetObjetInfo()
{
	return m_pObject;
}