#include"FallenLeavesComponent.h"
#include"WindComponent.h"
#include"TransformComponent.h"
#include"Sound.h"
#include "DrawEffectComponent.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "CloverComponent.h"

// コンストラクタ
CFallenLeaves::CFallenLeaves()
	: m_pTransform(nullptr), m_bClover(false)
	, m_nCloverNumber(100)
{
	this->m_eUpdateOrder = COM_UPDATE_1;
}

// デストラクタ
CFallenLeaves::~CFallenLeaves()
{

}

//  開始
void CFallenLeaves::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
}

//  更新
void CFallenLeaves::Update()
{

}

//  
void CFallenLeaves::Draw()
{

}
void CFallenLeaves::OnCollisionEnter(Object* pObject)
{
	if (pObject->GetName() == WIND_NAME)
	{
		Parent->Delete();
		//効果音
		//CSound::Play(SE_FALLENLEAVES);

#pragma region--- 落ち葉
		//	オブジェクト生成
		Object* obj = new Object("fallen_leaves", UPDATE_BG, DRAW_DEBUG);
		//	コンポーネントの追加
		auto Transobj = obj->AddComponent<CTransform>();			//	トランスフォーム
		auto Draw_obj = obj->AddComponent<CDrawEffect>();			//	エフェクト描画機能
		//	オブジェクトの設定
		Transobj->SetPosition(m_pTransform->Pos.x, m_pTransform->Pos.y, OTIBA_POS_Z);//	座標
		Draw_obj->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);				//	サイズ
		Draw_obj->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));	//	テクスチャ
		Draw_obj->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		Draw_obj->SetSwapFrame(OTIBA_ANIM_SPEED);
		//	リストに追加
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion

		if (m_bClover)
		{
			XMFLOAT3 create_pos = Parent->GetComponent<CTransform>()->Pos;
			create_pos.y += 8.0f;

			// クローバー生成
			Object* pClover;
			pClover = CloverComponent::CreateClover(create_pos, -1);

			// クローバーに番号を割り振る（氷ブロックに設定されている番号を与える
			pClover->GetComponent<CloverComponent>()->SetNumber(this->GetNumber());
		}
	}
}

// 落ち葉が消えた時にクローバーつくるフラグ
void CFallenLeaves::Clover()
{
	m_bClover = true;
}

void CFallenLeaves::SetNumber(int num)
{
	m_nCloverNumber = num;
}
int CFallenLeaves::GetNumber()
{
	return m_nCloverNumber;
}