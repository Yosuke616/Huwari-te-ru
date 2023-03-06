/**
 * @file	InformationComponent.h
 * @brief	他のオブジェクト情報を知らせる
 * @author	AT12A 成田陸人
 * @date	2021/12/30
 */

//===== インクルードガード ======
#ifndef __UI_H__
#define __UI_H__

//===== インクルード =====
#include "Component.h"


//===== クラス定義 =====
class CInformation : public Component
{
private:
	Object* m_pObject;	// オブジェクト情報
public:
	CInformation();							// コンストラクタ
	~CInformation() override;				// デストラクタ
	void Update() override;					// 更新
	void SetObjectInfo(Object* pObject);	// オブジェクト情報の取得
	Object* GetObjetInfo();					// オブジェクト情報の設定
};
#endif // __COMPONENT_H__

