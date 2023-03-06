//===== インクルード
#include "StageManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "imgui.h"
#include "Camera.h"
// コンポーネント
#include "TransformComponent.h"
#include "BillboardComponent.h"
#include "ColliderComponent.h"
#include "DrawEffectComponent.h"
#include "WindComponent.h"
#include "Draw3dComponent.h"
#include "Draw2dComponent.h"
#include "ModelManager.h"
#include "WindManager.h"
#include "SeedComponent.h"
#include "CloverComponent.h"
#include "GravityComponent.h"
#include "InformationComponent.h"
#include "FollowCompomemt.h"
#include "PostCollisionComponent.h"
#include "IcicleComponent.h"
#include "SpiderWebComponent.h"
#include "FallenLeavesComponent.h"
#include "BillboardComponent.h"
#include "DeleteTimerCom.h"
#include "FallComponent.h"
#include "BudComponent.h"
#include "IceBlockCom.h"
#include "OutRangeCom.h"
#include "MenuComponent.h"
#include "TutorialComponent.h"
#include "TagCom.h"

//===== 列挙体宣言
enum MAP_ARRAY
{
	N = -1,					// 描画なし-1
	B = 0,					// ブロック0
	CLOVER,					// クローバー1
	GOAL,					// 植木鉢(ゴール2
	KEY,					// 種(キー3
	ICICLE,					// つらら4
	WEB,					// クモの巣5
	LEAVES,					// 落ち葉6
	ICE_BLOCK,				// 氷7
	CLOVER_AND_LEAVES,		// 落ち葉とクローバー8
	CLOVER_AND_ICE_BLOCK,	// 氷とクローバー9
	BLOCK_LOW,				// ブロックの下の方10
	

	MAX_CHIP
};

//=== 静的メンバ変数
StageManager* StageManager::m_pInstance = nullptr;	//	インスタンス

#ifdef _DEBUG
//ResourceCSV::IntRow g_editorStage;
#endif // _DEBUG

// シングルトン-------------------------------------
/**
 * @fn			StageManager::GetInstance
 * @brief		インスタンス取得
*/
StageManager* StageManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new StageManager();
	}

	return m_pInstance;
}

/**
 * @fn			StageManager::Destroy
 * @brief		インスタンス破棄
*/
void StageManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//-----------------------------------------------


// load save-------------------------------------
/**
 * @fn			StageManager::Save
 * @brief		ステージの書き込み
 * @param[in]	int		季節
 * @detail		引数の季節によって書き込み先のcsvを決める
*/
bool StageManager::Save(int Season)
{
	// 保存するCSVファイルを決める
	std::string file;
	switch (Season)
	{
	case SPRING_1: file =SPRING_1_STAGE_CSV; break;	// 春1
	case SPRING_2: file =SPRING_2_STAGE_CSV; break;	// 春2
	case SUMMER_1: file =SUMMER_1_STAGE_CSV; break;	// 夏1
	case SUMMER_2: file =SUMMER_2_STAGE_CSV; break;	// 夏2
	case FALL_1:   file =FALL_1_STAGE_CSV;   break;	// 秋1
	case FALL_2:   file =FALL_2_STAGE_CSV;   break;	// 秋2
	case WINTER_1: file =WINTER_1_STAGE_CSV; break;	// 冬1
	case WINTER_2: file =WINTER_2_STAGE_CSV; break;	// 冬2
	default: break;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
	if (CSV == nullptr) return false;

	// save
	if (CSV->Save(file, m_StageGrid) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("stage_save"), NULL, MB_OK);
		return false;
	}
	else
	{
		switch (Season)
		{
		case SPRING_1:	MessageBox(GetMainWnd(), L"spring_1.csvに保存しました", _T("成功"), MB_OK); break;	// 春1
		case SPRING_2:	MessageBox(GetMainWnd(), L"spring_2.csvに保存しました", _T("成功"), MB_OK); break;	// 春2
		case SUMMER_1:	MessageBox(GetMainWnd(), L"summer_1.csvに保存しました", _T("成功"), MB_OK); break;	// 夏1
		case SUMMER_2:	MessageBox(GetMainWnd(), L"summer_2.csvに保存しました", _T("成功"), MB_OK); break;	// 夏2
		case FALL_1:	MessageBox(GetMainWnd(), L"fall_1.csvに保存しました",   _T("成功"), MB_OK); break;	// 秋1
		case FALL_2:	MessageBox(GetMainWnd(), L"fall_2.csvに保存しました",   _T("成功"), MB_OK); break;	// 秋2
		case WINTER_1:	MessageBox(GetMainWnd(), L"winter_1.csvに保存しました", _T("成功"), MB_OK); break;	// 冬1
		case WINTER_2:	MessageBox(GetMainWnd(), L"winter_2.csvに保存しました", _T("成功"), MB_OK); break;	// 冬2
		default: break;
		}
	}

	return true;
}

/**
 * @fn			StageManager::Load
 * @brief		ステージの読み込み
 * @param[in]	string	ファイル名
*/
bool StageManager::Load(std::string file)
{
	// load
	if (FileManager::GetInstance()->Load(file) == false)
	{
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("stage_load"), NULL, MB_OK);
		return false;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);

	//	ゲーム用の配列に格納する
	m_StageGrid.resize(CSV->GetRowSize());				// 行の数を設定
	for (int y = 0; y < CSV->GetRowSize(); ++y)
	{
		m_StageGrid[y].resize(CSV->GetColumnSize(y));	// 列の数を設定

		for (int x = 0; x < CSV->GetColumnSize(y); x++)
		{
			// 要素を格納する
			m_StageGrid[y][x] = CSV->GetInt(x, y);
		}
	}

#ifdef _DEBUG
	//g_editorStage.resize(CSV->GetRowSize());				// 行の数を設定
	//for (int y = 0; y < CSV->GetRowSize(); ++y)
	//{
	//	g_editorStage[y].resize(CSV->GetColumnSize(y));	// 列の数を設定
	//
	//	for (int x = 0; x < CSV->GetColumnSize(y); x++)
	//	{
	//		// 要素を格納する
	//		g_editorStage[y][x] = CSV->GetInt(x, y);
	//	}
	//}
#endif // _DEBUG

	return true;
}
//-----------------------------------------------



// 初期化、終了、更新、描画----------------------------
/**
 * @fn			StageManager::StageManager
 * @brief		初期化
*/
StageManager::StageManager()
	: m_block_num_x(0), m_block_num_y(0), m_nMap_Sta(N)
	, m_pShowObject(nullptr)
{
	//画面に映っているワールドの横幅と高さを計算
	m_WorldSize = CalcWorldSize();

	//	ステージ描画の起点
	m_BasePos.x = -(m_WorldSize.x - MAPCHIP_WIDTH)  * 0.5f - MAPCHIP_WIDTH;
	m_BasePos.y =  (m_WorldSize.y - MAPCHIP_HEIGHT) * 0.5f;
}

/**
 * @fn			StageManager::~StageManager
 * @brief		終了
*/
StageManager::~StageManager()
{
	if (m_pShowObject)
	{
		delete m_pShowObject;
		m_pShowObject = nullptr;
	}
}

/**
 * @fn			StageManager::Update
 * @brief		更新
 * @dtail		画面外のブロックを消す、画面内に入ってきたブロックを作る
*/
void StageManager::Update()
{
	// 編集モード
	if (SceneGame::GetInstance()->m_bEditMode)
	{
		//2021/12/29 Shimizu Yosuke -------------------------------------------------------------------------------
		//マウスのスクリーン座標をワールド座標に変換する
		InputManager* Input = INPUT;
		XMFLOAT2 Mouse_Pos = Input->GetMousePos();
		XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
		XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
		XMFLOAT3 worldPos;		// 算出結果を格納する
		CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);
		
		// 配置するオブジェクトの選択
		if (Input->GetKeyTrigger(DIK_0))	m_nMap_Sta = N;						// 配置しない
		if (Input->GetKeyTrigger(DIK_1))	m_nMap_Sta = B;						// ブロック(上側)
		if (Input->GetKeyTrigger(DIK_2))	m_nMap_Sta = CLOVER;				// クローバー
		if (Input->GetKeyTrigger(DIK_3))	m_nMap_Sta = GOAL;					// 植木鉢
		if (Input->GetKeyTrigger(DIK_4))	m_nMap_Sta = KEY;					// 種
		if (Input->GetKeyTrigger(DIK_5))	m_nMap_Sta = ICICLE;				// つらら
		if (Input->GetKeyTrigger(DIK_6))	m_nMap_Sta = WEB;					// クモの巣
		if (Input->GetKeyTrigger(DIK_7))	m_nMap_Sta = LEAVES;				// 落ち葉
		if (Input->GetKeyTrigger(DIK_8))	m_nMap_Sta = ICE_BLOCK;				// 氷
		if (Input->GetKeyTrigger(DIK_9))	m_nMap_Sta = CLOVER_AND_LEAVES;		// クローバー＆落ち葉
		if (Input->GetKeyTrigger(DIK_W))	m_nMap_Sta = CLOVER_AND_ICE_BLOCK;	//　氷＆落ち葉
		if (Input->GetKeyTrigger(DIK_Q))	m_nMap_Sta = BLOCK_LOW;				// ブロック(下側
	
		// 要素数を出すための計算をする
		m_ElementNumX = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));
		m_ElementNumY = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));
		// 生成座標を計算
		float CreatePosX = m_BasePos.x + (m_ElementNumX) * MAPCHIP_WIDTH;
		float CreatePosY = m_BasePos.y - (m_ElementNumY) * MAPCHIP_HEIGHT;

		// カーソルの更新
		this->UpdateShowObject(CreatePosX, CreatePosY);

		// クリックで配置
		if (Input->GetMouseTrigger(MOUSE_L))
		{
			// 生成する場所のブロックIDを計算（yの要素サイズ × xの要素サイズ + xの要素）
			int nMapChipID = (m_ElementNumY * m_block_num_x) + m_ElementNumX;
			
			// 生成場所にオブジェクトが存在する場合は削除して新しく配置し直す
			if (m_StageGrid[m_ElementNumY][m_ElementNumX] != -1)
			{
				// オブジェクト存在
				Object* pDeleteObject = CTag::Find(nMapChipID);
				if(pDeleteObject)
				{
					pDeleteObject->Delete();
				}
			}

			// ステージブロックのステータスの更新
			m_StageGrid[m_ElementNumY][m_ElementNumX] = m_nMap_Sta;

			// オブジェクトの生成
			CreateBlock(CreatePosX, CreatePosY, m_nMap_Sta, nMapChipID);

			//オブジェクトを作る(生成座標x, 生成座標y, ステージ配列)
			//CleateBlock_Deb(CreatePosX, CreatePosY, nMapChipID, m_ElementNumX, m_ElementNumY, m_nMap_Sta);
			//2021/12/30 narita
			// ファイル書き換え
			//std::string file;
			//switch (SceneGame::GetInstance()->GetSeason())
			//{
			//case SPRING_1:	file = SPRING_1_STAGE_CSV; break;	// 春1
			//case SPRING_2:	file = SPRING_2_STAGE_CSV; break;	// 春2
			//case SUMMER_1:	file = SUMMER_1_STAGE_CSV; break;	// 夏1
			//case SUMMER_2:	file = SUMMER_2_STAGE_CSV; break;	// 夏2
			//case FALL_1:	file = FALL_1_STAGE_CSV;   break;	// 秋1
			//case FALL_2:	file = FALL_2_STAGE_CSV;   break;	// 秋2
			//case WINTER_1:	file = WINTER_1_STAGE_CSV; break;	// 冬1
			//case WINTER_2:	file = WINTER_2_STAGE_CSV; break;	// 冬2
			//default: break;
			//}
			//ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
			//CSV->SetInt(m_ElementNumX, m_ElementNumY, m_nMap_Sta);
		}
	
		// ステージをcsvに保存
		if (Input->GetKeyPress(DIK_LCONTROL) && Input->GetKeyTrigger(DIK_S))
		{
			this->Save(SceneGame::GetInstance()->GetSeason());
		}
	
		// カーソルオブジェクトの更新
		if(m_pShowObject)	m_pShowObject->Update();
		//-----------------------------------------------------------------------------------------------------
	}
	else
	{
		// 編集モードの終わり
		if (m_pShowObject)
		{
			delete m_pShowObject;
			m_pShowObject = nullptr;
		}
	}
}

/**
 * @fn			StageManager::Draw
 * @brief		debug描画
*/
void StageManager::Draw()
{
	// カーソルオブジェクトの描画
	if (m_pShowObject)	m_pShowObject->Draw();

#ifdef _DEBUG
	using namespace ImGui;
	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Stage");
	for (int y = 0; y < m_StageGrid.size(); ++y)
	{
		NewLine();
		for (int x = 0; x < m_StageGrid[y].size(); x++)
		{
			Text("%d ", m_StageGrid[y][x]);
			SameLine();
		}
	}
	End();

#endif // _DEBUG
}
//-----------------------------------------------



// 生成-------------------------------------------
/**
 * @fn			StageManager::CreateStage
 * @brief		ステージ全体の生成
 * @param[in]	stage_state		季節
*/
void StageManager::CreateStage(int stage_state)
{

	// ステージ読み込み
	switch (stage_state)	// 季節によって読み込むcsvを変える
	{
	case SPRING_1:	this->Load(SPRING_1_STAGE_CSV); break;	// 春1
	case SPRING_2:	this->Load(SPRING_2_STAGE_CSV); break;	// 春2
	case SUMMER_1:	this->Load(SUMMER_1_STAGE_CSV); break;	// 夏1
	case SUMMER_2:	this->Load(SUMMER_2_STAGE_CSV); break;	// 夏2
	case FALL_1:	this->Load(FALL_1_STAGE_CSV);   break;	// 秋1
	case FALL_2:	this->Load(FALL_2_STAGE_CSV);   break;	// 秋2
	case WINTER_1:	this->Load(WINTER_1_STAGE_CSV); break;	// 冬1
	case WINTER_2:	this->Load(WINTER_2_STAGE_CSV); break;	// 冬2
	default: break;
	}

	float fBaseX = m_BasePos.x;
	float fBaseY = m_BasePos.y;
	int BlockID = 0;
	// 2022/01/26
	std::list<Object*> Clovers;	// クローバーに番号を割り振るための一時的な配列

	//	マップの配列だけループ
	for (int y = 0; y < (int)m_StageGrid.size(); y++)
	{
		for (int x = 0; x < (int)m_StageGrid[y].size(); x++)
		{
			Object* pObject;
			// ブロック生成
			pObject = CreateBlock(fBaseX, fBaseY, m_StageGrid[y][x], BlockID);

			// 2022/01/26 クローバーを生成するオブジェクトだった場合配列に追加していく
			if (m_StageGrid[y][x] == CLOVER ||
				m_StageGrid[y][x] == CLOVER_AND_LEAVES ||
				m_StageGrid[y][x] == CLOVER_AND_ICE_BLOCK)
			{
				Clovers.push_back(pObject);
			}

			fBaseX += MAPCHIP_WIDTH;	//	次のブロックの座標へ
			BlockID++;					//	次のブロックIDへ
		}

		fBaseX = m_BasePos.x;			//	X座標リセット
		fBaseY -= MAPCHIP_HEIGHT;		//	次のブロックの座標へ
	}

	// ステージの要素数を格納
	m_block_num_y = (int)m_StageGrid.size();	// yの要素数
	for (auto row : m_StageGrid)
	{
		// 1番サイズが大きい列を反映
		if (m_block_num_x < (int)row.size())
			m_block_num_x = (int)row.size();	// xの要素数
	}

	// ステージのサイズを格納
	m_vStageSize.x = m_block_num_x * MAPCHIP_WIDTH;		// 要素数 x ブロックの大きさ
	m_vStageSize.y = m_block_num_y * MAPCHIP_HEIGHT;

	/*	2022/01/22	カメラの移動限界の設定
		クリア時に生じる画面外の虚無空間が映る対処 スクロール範囲外に左端に１行、右端に６行分のブロックを入れる
	*/
	CCamera::Get()->SetLimit(XMFLOAT2(0.0f, m_vStageSize.x - m_WorldSize.x - (MAPCHIP_WIDTH * 7)));

	// クローバーに番号を割り振る
	SetCloverNumber(Clovers);
}

/**
 * @fn			StageManager::SetCloverNumber
 * @brief		クローバーに番号を割り振る
*/
void StageManager::SetCloverNumber(std::list<Object*>& Clovers)
{
	/*	2022/01/25	クローバーに番号を割り振る
		左から順番に0, 1, 2, 3
	*/
	// 座標によって並び替える
	Clovers.sort([=](Object* A, Object* B)
		{
			// 座標を取得する
			auto PosA = A->GetComponent<CTransform>()->Pos;
			auto PosB = B->GetComponent<CTransform>()->Pos;

			// 座標を比較
			return PosA.x < PosB.x;
		});

	// 番号を割り振る
	int number = 0;
	for (auto&& clover : Clovers)
	{
		// クローばーオブジェクトだった場合
		auto ComClover = clover->GetComponent<CloverComponent>();
		if (ComClover)
		{
			// 番号を割り振る
			ComClover->SetNumber(number);
		}
		// クローバーを生成する落ち葉だった場合
		auto ComFallLeaves = clover->GetComponent<CFallenLeaves>();
		if (ComFallLeaves)
		{
			// 番号を割り振る
			ComFallLeaves->SetNumber(number);
		}
		// クローバーを生成する氷ブロックだった場合
		auto ComIceBlock = clover->GetComponent<CIceBlock>();
		if (ComIceBlock)
		{
			// 番号を割り振る
			ComIceBlock->SetNumber(number);
		}

		// 番号加算
		number++;
	}

	// クローバー取得状況の確認
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	int nStage = SceneGame::GetInstance()->GetSeason() * 4;
	bool bClover[4];
	for (int nCloverNumber = 0; nCloverNumber < 4; nCloverNumber++)
	{
		bClover[nCloverNumber] = CloverCSV->GetInt(nStage + nCloverNumber, 0);

		// すでにクローバーを取得済みなら削除する
		if (bClover[nCloverNumber] == true)
		{
			// 番号の数だけイテレーション
			std::list<Object*>::iterator itr = Clovers.begin();
			for (int n = 0; n < nCloverNumber; n++) itr++;

			// 削除
			(*itr)->Delete();

			if ((*itr)->GetName() == OTIBA_NAME)
			{
#pragma region--- 落ち葉
				Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
				// components
				auto trans = obj->AddComponent<CTransform>();					// オブジェクトの座標
				auto draw = obj->AddComponent<CBillboard>();					// エフェクト描画機能
				auto collider = obj->AddComponent<CCollider>();					// あたり判定
				auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// 落ち葉の機能
				obj->AddComponent<COutRange>();									// 画面外処理
				// settings
				XMFLOAT3 pos = (*itr)->GetComponent<CTransform>()->Pos;
				trans->SetPosition(pos.x, pos.y, OTIBA_POS_Z);					// 座標
				draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// サイズ
				draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// 画像の分割数
				draw->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));	// テクスチャ
				collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// あたり判定の大きさ
				// add to list
				ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion
			}
			else if ((*itr)->GetName() == ICE_BLOCK_NAME)
			{
#pragma region ---氷
				ModelManager* pModelManager = ModelManager::GetInstance();
				//　オブジェクトの生成
				Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
				// Components
				auto trans = pIceBlock->AddComponent<CTransform>();			// オブジェクトの座標
				auto draw = pIceBlock->AddComponent<CDrawModel>();				// モデル機能
				auto collider = pIceBlock->AddComponent<CCollider>();		// あたり判定
				auto ice = pIceBlock->AddComponent<CIceBlock>();			// 氷ブロックの機能
				auto gravity = pIceBlock->AddComponent<CGravity>();			// 重力
				auto timer = pIceBlock->AddComponent<CTimer>();				// 時間を指定して削除できるようにする
				auto out_range = pIceBlock->AddComponent<COutRange>();		// 画面外処理
				// Settings
				XMFLOAT3 pos = (*itr)->GetComponent<CTransform>()->Pos;
				trans->SetPosition(pos.x, pos.y, ICE_BLOCK_POS_Z_OFFSET);						// 座標
				trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);		// 大きさ
				collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);// 当たり判定の大きさ
				out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);						  // どの位置から処理を省くかの設定
				// 仮措置	(氷ブロックのアニメーション関係)
				// アニメーションがあるモデルだからインスタンスを分けないといけない
				// 使用していないインスタンスが見つかった場合それをセットする
				if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//　モデル1
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//　モデル2
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//　モデル3
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//　モデル4
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//　モデル5
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//　モデル6
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//　モデル7
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//　モデル8
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//　モデル9
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//　モデル10
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
				}
				else
				{
					// 全てのインスタンスが現在使用中の場合
					MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
				}
				draw->AddAnimation(0, 0.0, 8.0, true);
				draw->SetAnimTime(0);
				//　オブジェクトの追加
				ObjectManager::GetInstance()->AddObject(pIceBlock);
				draw->Update();
#pragma endregion
			}
		}
	}
}

/**
 * @fn			StageManager::CreateBlock
 * @brief		ブロック生成
 * @param[in]	PosX		x座標
 * @param[in]	PosY		y座標
 * @param[in]	state		オブジェクトナンバー
 * @param[in]	nBlockID	ブロックと紐付いているキー
*/
Object* StageManager::CreateBlock(float PosX, float PosY, int state, int nBlockID)
{
	// Nは何も生成しない
	if (state == N)	return nullptr;
	
	// モデルとテクスチャマネージャー
	ModelManager* pModelManager = ModelManager::GetInstance();
	TextureManager* pTexManager = TextureManager::GetInstance();

	// ブロック(上の方
	if (state == B)
	{
#pragma region ---ブロック(上の方
		Object* obj = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		// components
		auto trans		= obj->AddComponent<CTransform>();			// オブジェクト座標
		auto Draw		= obj->AddComponent<CDrawModel>();				// モデル描画
		auto Collider	= obj->AddComponent<CCollider>();			// あたり判定
		auto comID		= obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		obj->AddComponent<COutRange>();								// 画面外処理
		// settings
		trans->SetPosition(PosX, PosY, BLOCK_OFFSET_Z);					// 座標
		trans->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);	// スケール
		Collider->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);		// あたり判定の大きさ
		Collider->SetOffset(BLOCK_OFFSET_X, BLOCK_OFFSET_Y);			// 中心座標とモデル座標の中心のずれ
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);

		// 四季によってテクスチャを変える
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// 春
			Draw->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
			break;
		case SUMMER_1:	case SUMMER_2:	// 夏
			Draw->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
			break;
		case FALL_1:	case FALL_2:	// 秋
			Draw->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
			break;
		case WINTER_1:	case WINTER_2:	// 冬
			Draw->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
			break;
		default:break;
		}
		// ワールドマトリックスを更新しておく
		Draw->Update();
#pragma endregion

		return obj;
	}
	// 種＆つぼみ
	else if (state == KEY)
	{
#pragma region  ---種
		//　オブジェクトの生成
		Object* Seed = new Object(SEED_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto TransSeed = Seed->AddComponent<CTransform>();		//　トランスフォーム
		auto DrawSeed = Seed->AddComponent<CDrawModel>();			//　3D描画機能
		auto CollisionSeed = Seed->AddComponent<CCollider>();   //　衝突機能
		auto ComSeed = Seed->AddComponent<CSeed>();				//　種
		auto comID = Seed->AddComponent<CTag>();
		comID->SetID(nBlockID);
		Seed->AddComponent<COutRange>();
		// settings
		DrawSeed->SetModel(ModelManager::GetInstance()->GetModel(SEED_MODEL_NUM));//　モデル
		TransSeed->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);	//　スケール
		TransSeed->SetPosition(PosX, PosY + SEED_POS_OFFSET_Y, BUD_POS_OFFSET_Z);//　ポジション
		CollisionSeed->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT); //　当たり判定の大きさ
		// add to list
		ObjectManager::GetInstance()->AddObject(Seed);
		// ワールドマトリックスを更新しておく
		DrawSeed->Update();
#pragma endregion

#pragma region ---つぼみ
		Object* FlowerBud = new Object(BUD_NAME, UPDATE_MODEL, DRAW_MODEL);
		// compponents
		auto transFlowerBud = FlowerBud->AddComponent<CTransform>();			//	オブジェクト座標
		auto drawFlowerBud = FlowerBud->AddComponent<CDrawModel>();				//	モデル描画
		auto collider = FlowerBud->AddComponent<CCollider>();					//	モデル描画
		auto bud = FlowerBud->AddComponent<CBud>();								//	つぼみ
		FlowerBud->AddComponent<COutRange>();
		// settings
		transFlowerBud->SetPosition(PosX, PosY + BUD_POS_OFFSET_Y, BUD_POS_OFFSET_Z);//	座標
		transFlowerBud->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);		//	スケール
		collider->SetCollisionSize(BUD_COLLISION_X, BUD_COLLISION_Y);			// あたり判定の大きさ
		//	リストに追加
		ObjectManager::GetInstance()->AddObject(FlowerBud);

		// 四季によってテクスチャを変える
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:
		case SPRING_2:	// 春
			drawFlowerBud->SetModel(pModelManager->GetModel(SPRING_BUD_MODEL_NUM));
			break;
		case SUMMER_1:
		case SUMMER_2:	// 夏
			drawFlowerBud->SetModel(pModelManager->GetModel(SUMMER_BUD_MODEL_NUM));
			break;
		case FALL_1:
		case FALL_2:	// 秋
			drawFlowerBud->SetModel(pModelManager->GetModel(FALL_BUD_MODEL_NUM));
			break;
		case WINTER_1:
		case WINTER_2:	// 冬
			drawFlowerBud->SetModel(pModelManager->GetModel(WINTER_BUD_MODEL_NUM));
			break;
		default:break;
		}
		// アニメーションの追加
		drawFlowerBud->AddAnimation(0, BUD_ANIM_1_START, BUD_ANIM_1_END, false);	// つぼみ開く
		drawFlowerBud->AddAnimation(1, BUD_ANIM_2_START, BUD_ANIM_2_END, false);	// つぼみ閉じる

		// ワールドマトリックスを更新しておく
		drawFlowerBud->Update();
#pragma endregion

		return Seed;
	}
	// 植木鉢
	else if (state == GOAL)
	{
#pragma region ---植木鉢(ゴール
	Object* pPlantPot = new Object(POT_NAME, UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto transPot		= pPlantPot->AddComponent<CTransform>();	// オブジェクトの座標
	auto drawPot		= pPlantPot->AddComponent<CDrawModel>();		// モデル描画
	auto colliderPot	= pPlantPot->AddComponent<CCollider>();		// あたり判定
	auto comID = pPlantPot->AddComponent<CTag>();
	comID->SetID(nBlockID);
	pPlantPot->AddComponent<COutRange>();							// 画面外処理
	// settings
	transPot->SetPosition(PosX, PosY + POT_POS_OFFSET_Y, POT_POS_Z);// 座標
	transPot->SetScale(POT_SCALE_X, POT_SCALE_Y, POT_SCALE_Z);		// スケール
	drawPot->SetModel(ModelManager::GetInstance()->GetModel(POT_MODEL_NUM));// モデル
	colliderPot->SetCollisionSize(MAPCHIP_WIDTH * 3, 5.0f);// あたり判定の大きさ
	colliderPot->SetOffset(0.0f, 25.0f);
	// add to list
	ObjectManager::GetInstance()->AddObject(pPlantPot);
	// ワールドマトリックスを更新しておく
	drawPot->Update();
#pragma endregion
	return pPlantPot;
	}
	// クローバー
	else if (state == CLOVER)
	{
#pragma region ---クローバー
		return CloverComponent::CreateClover(XMFLOAT3(PosX, PosY, 0.0f), nBlockID);
#pragma endregion
	}
	// つらら
	else if (state == ICICLE)
	{
#pragma region	---つらら上の部分
		Object* pUpIcicle = new Object(ICICLE_UP_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto transIcicle = pUpIcicle->AddComponent<CTransform>();		// オブジェクトの座標
		auto drawIcicle = pUpIcicle->AddComponent<CDrawModel>();			// 描画機能
		auto upcolliderIcicle = pUpIcicle->AddComponent<CCollider>();	// 衝突機能
		pUpIcicle->AddComponent<CIcicle>();								// つららの機能
		auto outRange = pUpIcicle->AddComponent<COutRange>();
		auto comID = pUpIcicle->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		transIcicle->SetPosition(PosX, PosY);							// 中心座標
		transIcicle->SetScale(ICICLE_SCALE_X, ICICLE_SCALE_Y, ICICLE_SCALE_Z);	//　大きさ
		drawIcicle->SetModel(ModelManager::GetInstance()->GetModel(ICICLE_MODEL_NUM));// テクスチャ
		upcolliderIcicle->SetCollisionSize(ICICLE_UP_COLLISION_SIZE_X, ICICLE_UP_COLLISION_SIZE_Y);	// あたり判定の大きさ
		upcolliderIcicle->SetOffset(ICICLE_UP_COLLISION_OFFSET_X, ICICLE_UP_COLLISION_OFFSET_Y);	// あたり判定の中心座標のずれ
		// add to list
		ObjectManager::GetInstance()->AddObject(pUpIcicle);
#pragma endregion
#pragma region	---当たり判定を取るオブジェクトの生成(つららの下部)
		Object* pdownIcicle = new Object(ICICLE_DOWN_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto transdownIcicle = pdownIcicle->AddComponent<CTransform>();		// オブジェクトの座標
		auto downcolliderIcicle = pdownIcicle->AddComponent<CCollider>();	// 衝突機能
		auto FollowIce = pdownIcicle->AddComponent<CFollow>();				// 追従機能
		auto InformationIce = pdownIcicle->AddComponent<CInformation>();	// 他のオブジェクト情報を取得する機能
		auto CollisionIce = pdownIcicle->AddComponent<CPostCollision>();	// 衝突内容
		pdownIcicle->AddComponent<COutRange>();
		// settings
		transdownIcicle->SetPosition(PosX, PosY);							// 中心座標
		downcolliderIcicle->SetCollisionSize(20.0f, 60.0f);					// あたり判定の大きさ
		downcolliderIcicle->SetOffset(0, -20);								// あたり判定の中心座標のずれ
		FollowIce->SetParent(pUpIcicle);									// 追従するオブジェクトの設定
		InformationIce->SetObjectInfo(pUpIcicle);							// 上の部分の情報を取得しておく
		// add to list
		ObjectManager::GetInstance()->AddObject(pdownIcicle);
#pragma endregion

		return pUpIcicle;
	}
	// クモの巣
	else if (state == WEB)
	{
#pragma region ---クモの巣
	Object* pSpiderWeb = new Object("spider", UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto TransSpider = pSpiderWeb->AddComponent<CTransform>();			// オブジェクトの座標
	auto ColSpider = pSpiderWeb->AddComponent<CCollider>();				// 当たり判定
	auto DrawSpider = pSpiderWeb->AddComponent<CDrawEffect>();			// クモの巣描画
	auto SpiderCom = pSpiderWeb->AddComponent<CSpiderWeb>();			// クモの巣の機能
	pSpiderWeb->AddComponent<COutRange>();								// 画面外処理
	auto comID = pSpiderWeb->AddComponent<CTag>();
	comID->SetID(nBlockID);
	// settings
	DrawSpider->SetTexture(pTexManager->GetTexture(SPIDER_TEX_NUM));	// テクスチャ
	TransSpider->SetPosition(PosX, PosY - SPIDER_OFFESET_Y);			// 座標
	TransSpider->SetRotate(0.0f, SPIDER_ROT_Y, 0.0f);					// 傾き
	DrawSpider->SetSize(SPIDER_WIDTH, SPIDER_HEIGHT);					// サイズ
	DrawSpider->SetLoop(true);											// アニメーションループ
	SpiderCom->SetBindSize(SPIDER_BIND_WIDTH, SPIDER_BIND_HEIGHT);		// 拘束範囲の設定
	ColSpider->SetCollisionSize(SPIDER_WIDTH, SPIDER_HEIGHT);			// あたり判定の大きさ
	// add to list
	ObjectManager::GetInstance()->AddObject(pSpiderWeb);
#pragma endregion
		return pSpiderWeb;
	}
	// 落ち葉
	else if (state == LEAVES)
	{
#pragma region--- 落ち葉
		Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
		// components
		auto trans		  = obj->AddComponent<CTransform>();			// トランスフォーム
		auto draw		  = obj->AddComponent<CBillboard>();			// エフェクト描画機能
		auto collider	  = obj->AddComponent<CCollider>();				// あたり判定
		auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// 落ち葉の機能
		obj->AddComponent<COutRange>();									// 画面外処理
		auto comID = obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		trans->SetPosition(PosX, PosY + OTIBA_OFFESET_Y, OTIBA_POS_Z);	// 座標
		draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// サイズ
		draw->SetTexture(pTexManager->GetTexture(OTIBA_TEX_NUM));		// テクスチャ
		draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// 画像の分割数
		collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// あたり判定の大きさ
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion
		return obj;
	}
	// 氷ブロック
	else if (state == ICE_BLOCK)
	{
#pragma region ---氷
		Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
		// Components
		auto trans		= pIceBlock->AddComponent<CTransform>();		// オブジェクトの座標
		auto draw		= pIceBlock->AddComponent<CDrawModel>();			// モデル機能
		auto collider	= pIceBlock->AddComponent<CCollider>();			// あたり判定
		auto gravity	= pIceBlock->AddComponent<CGravity>();			// 重力
		auto ice		= pIceBlock->AddComponent<CIceBlock>();			// 氷ブロックの機能
		auto Timer		= pIceBlock->AddComponent<CTimer>();			// 時間を指定して削除できるようにする
		auto out_range	= pIceBlock->AddComponent<COutRange>();			// 画面外処理
		auto comID = pIceBlock->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// Settings
		trans->SetPosition(PosX, PosY + ICE_BLOCK_POS_Y_OFFSET, ICE_BLOCK_POS_Z_OFFSET);	// 座標
		trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);			// スケール
		collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);	// 当たり判定の大きさ
		out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);							// どの位置から処理を省くかの設定
		// 仮措置	(氷ブロックのアニメーション関係)
		// アニメーションがあるモデルだからインスタンスを分けないといけない
		// 使用していないインスタンスが見つかった場合それをセットする
		if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//　モデル1
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//　モデル2
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//　モデル3
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//　モデル4
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//　モデル5
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//　モデル6
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//　モデル7
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//　モデル8
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//　モデル9
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//　モデル10
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
		}
		else
		{
			// 全てのインスタンスが現在使用中の場合
			MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
		}
		draw->AddAnimation(0, 0.0, 8.0, true);	// アニメーションの設定
		draw->SetAnimTime(0);					// アニメーション時間初期化
		// Add to list
		ObjectManager::GetInstance()->AddObject(pIceBlock);
		draw->Update();
#pragma endregion
		return pIceBlock;
	}
	// 落ち葉＆クローバー
	else if (state == CLOVER_AND_LEAVES)
	{
#pragma region--- 落ち葉
		Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
		// components
		auto trans		  = obj->AddComponent<CTransform>();			// オブジェクトの座標
		auto draw		  = obj->AddComponent<CBillboard>();			// エフェクト描画機能
		auto collider	  = obj->AddComponent<CCollider>();				// あたり判定
		auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// 落ち葉の機能
		obj->AddComponent<COutRange>();									// 画面外処理
		auto comID = obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		trans->SetPosition(PosX, PosY + OTIBA_OFFESET_Y, OTIBA_POS_Z);	// 座標
		draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// サイズ
		draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// 画像の分割数
		draw->SetTexture(pTexManager->GetTexture(OTIBA_TEX_NUM));		// テクスチャ
		collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// あたり判定の大きさ
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion

#pragma region ---落ち葉が消えた後にクローバーを出す
		FallenLeaves->Clover();
#pragma endregion

		return obj;
	}
	// 氷ブロック＆クローバー
	else if (state == CLOVER_AND_ICE_BLOCK)
	{
#pragma region ---氷
		//　オブジェクトの生成
		Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
		// Components
		auto trans		= pIceBlock->AddComponent<CTransform>();		// オブジェクトの座標
		auto draw		= pIceBlock->AddComponent<CDrawModel>();			// モデル機能
		auto collider	= pIceBlock->AddComponent<CCollider>();			// あたり判定
		auto ice		= pIceBlock->AddComponent<CIceBlock>();			// 氷ブロックの機能
		auto gravity	= pIceBlock->AddComponent<CGravity>();			// 重力
		auto timer		= pIceBlock->AddComponent<CTimer>();			// 時間を指定して削除できるようにする
		auto out_range	= pIceBlock->AddComponent<COutRange>();			// 画面外処理
		auto comID = pIceBlock->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// Settings
		trans->SetPosition(PosX, PosY + ICE_BLOCK_POS_Y_OFFSET, ICE_BLOCK_POS_Z_OFFSET);  // 座標
		trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);		  // 大きさ
		collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);// 当たり判定の大きさ
		out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);						  // どの位置から処理を省くかの設定
		// 仮措置	(氷ブロックのアニメーション関係)
		// アニメーションがあるモデルだからインスタンスを分けないといけない
		// 使用していないインスタンスが見つかった場合それをセットする
		if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//　モデル1
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//　モデル2
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//　モデル3
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//　モデル4
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//　モデル5
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//　モデル6
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//　モデル7
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//　モデル8
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//　モデル9
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//　モデル10
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
		}
		else
		{
			// 全てのインスタンスが現在使用中の場合
			MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
		}
		draw->AddAnimation(0, 0.0, 8.0, true);
		draw->SetAnimTime(0);
		//　オブジェクトの追加
		ObjectManager::GetInstance()->AddObject(pIceBlock);
		draw->Update();
#pragma endregion

#pragma region ---氷ブロックが壊れた後にクローバーを出す
		timer->SetFunction(FUNC_CREATE_CLOVER);
#pragma endregion

		return pIceBlock;
	}
	// ブロックの下の方
	else if (state == BLOCK_LOW)
	{
#pragma region ---ブロック(下の方
		//	オブジェクト生成
		Object* block = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		//	コンポーネントの追加
		auto transBlock = block->AddComponent<CTransform>();		//	座標
		auto DrawBlock = block->AddComponent<CDrawModel>();			//	3dモデル描画
		auto ColliderBlock = block->AddComponent<CCollider>();		//	あたり判定
		block->AddComponent<COutRange>();
		auto comID = block->AddComponent<CTag>();
		comID->SetID(nBlockID);
		//	オブジェクト設定
		transBlock->SetPosition(PosX, PosY, BLOCK_OFFSET_Z);				// 座標
		transBlock->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);	// スケール
		ColliderBlock->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);		// あたり判定の大きさ
		ColliderBlock->SetOffset(BLOCK_OFFSET_X, BLOCK_OFFSET_Y);			// 中心座標とモデル座標の中心のずれ
		//	リストに追加
		ObjectManager::GetInstance()->AddObject(block);

		// テクスチャ切り替え（モデル切り替え)
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1: case SPRING_2:	// 春
			DrawBlock->SetModel(pModelManager->GetModel(SPRING_BLOCK_LOW_MODEL_NUM));
			break;
		case SUMMER_1: case SUMMER_2:	// 夏
			DrawBlock->SetModel(pModelManager->GetModel(SUMMER_BLOCK_LOW_MODEL_NUM));
			break;
		case FALL_1: case FALL_2:	// 秋
			DrawBlock->SetModel(pModelManager->GetModel(FALL_BLOCK_LOW_MODEL_NUM));
			break;
		case WINTER_1: case WINTER_2:	// 冬
			DrawBlock->SetModel(pModelManager->GetModel(WINTER_BLOCK_LOW_MODEL_NUM));
			break;
		default:break;
		}

		DrawBlock->Update();
#pragma endregion
		return block;
	}
	return nullptr;
 }
 
//-----------------------------------------------


/**
 * @fn			StageManager::UpdateShowObject
 * @brief		配置するオブジェクトの表示の更新
  * @param[in]	fPosX	生成するx座標
 * @param[in]	fPosY	生成するy座標
*/
void StageManager::UpdateShowObject(float fPosX, float fPosY)
{
	if (!m_pShowObject)
	{
		// オブジェクト生成
		m_pShowObject = new Object("debug", UPDATE_DEBUG, DRAW_DEBUG);
		// コンポーネントの追加
		auto trans = m_pShowObject->AddComponent<CTransform>();	// オブジェクトの座標
		auto draw = m_pShowObject->AddComponent<CDrawModel>();		// モデル
		auto draw2 = m_pShowObject->AddComponent<CDraw2D>();	// テクスチャ
		draw2->SetSize(30.0f, 30.0f);
		draw2->SetTexture(TextureManager::GetInstance()->GetTexture( SPIDER_TEX_NUM));
		m_pShowObject->Start();
	}

	// 座標設定
	auto Transform = m_pShowObject->GetComponent<CTransform>();
	auto drawModel = m_pShowObject->GetComponent<CDrawModel>();
	auto draw2D = m_pShowObject->GetComponent<CDraw2D>();
	Transform->SetPosition(fPosX, fPosY);
	Transform->SetScale(1.0f, 1.0f, 1.0f);
	Transform->SetRotate(0.0f, 0.0f, 0.0f);
	draw2D->SetColor(1,1,1);
	draw2D->SetAlpha(1.0f);

#pragma region ----------ワールド座標をスクリーン座標に変換する
	// ビルボード用ワールド変換
	XMFLOAT4X4& mP = CCamera::Get()->GetProjMatrix();
	XMFLOAT4X4& mV = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4 mW(
		mV._11, mV._21, mV._31, 0.0f,
		mV._12, mV._22, mV._32, 0.0f,
		mV._13, mV._23, mV._33, 0.0f,
		fPosX, fPosY, 0.0f, 1.0f);
	// ビューポート変換
	XMFLOAT4X4 mVP(
		SCREEN_WIDTH / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, SCREEN_HEIGHT / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	// マトリックスを合成
	XMMATRIX matrix = XMLoadFloat4x4(&mW) * XMLoadFloat4x4(&mV) * XMLoadFloat4x4(&mP) * XMLoadFloat4x4(&mVP);

	// 中心座標、左下座標、右上座標を2D座標に変換
	static const XMFLOAT3 v[] =
	{
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
	};
	XMFLOAT4 vPos[3];
	for (int i = 0; i < 3; ++i)
	{
		XMStoreFloat4(&vPos[i], XMVector3Transform(XMLoadFloat3(&v[i]), matrix));
		// 同次頂点を非同次頂点に変換
		vPos[i].x /= vPos[i].w;
		vPos[i].y /= vPos[i].w;
		vPos[i].z /= vPos[i].w;
	}
	DirectX::XMFLOAT3 screenPos = { vPos[0].x, vPos[0].y, vPos->z };
#pragma endregion

	// 配置するもののステータス別に設定
	switch (m_nMap_Sta)
	{
	case N: // 配置なし
	{
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// スクリーン座標の設定
		Transform->SetPosition(screenPos.x, screenPos.y);
		// テクスチャ
		draw2D->SetTexture(nullptr);
		// 色
		draw2D->SetColor(0.7f, 0.2f, 0.2f);
		draw2D->SetAlpha(0.7f);
		// サイズ
		draw2D->SetSize(50.0f, 50.0f);
	}
		break;

	case B:// ブロック（表面）
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// スケール
		Transform->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);
		// 四季によってテクスチャを変える
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:
		case SPRING_2:	// 春
			drawModel->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
			break;
		case SUMMER_1:
		case SUMMER_2:	// 夏
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
			break;
		case FALL_1:
		case FALL_2:	// 秋
			drawModel->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
			break;
		case WINTER_1:
		case WINTER_2:	// 冬
			drawModel->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;

	case CLOVER:// クローバー
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// 回転
		Transform->SetRotate(CLOVER_ROTATE_X, CLOVER_ROTATE_Y, CLOVER_ROTATE_Z);
		// スケール
		Transform->SetScale(CLOVER_SCALE_X, CLOVER_SCALE_Y, CLOVER_SCALE_Z);
		// モデル
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(CLOVER_MODEL_NUM));
	}
		break;

	case GOAL:// 植木鉢
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// スケール
		Transform->SetScale(POT_SCALE_X, POT_SCALE_Y, POT_SCALE_Z);
		// 座標のオフセット
		Transform->SetPosition(fPosX, fPosY + +POT_POS_OFFSET_Y, POT_POS_Z);
		// モデル
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(POT_MODEL_NUM));
	}
		break;
	case KEY:// 種
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// スケール
		Transform->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);
		// 座標のオフセット
		Transform->SetPosition(fPosX, fPosY + BUD_POS_OFFSET_Y);
		// 四季によってテクスチャを変える
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1: case SPRING_2:	// 春
			drawModel->SetModel(pModelManager->GetModel(SPRING_BUD_MODEL_NUM));
			break;
		case SUMMER_1: case SUMMER_2:	// 夏
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BUD_MODEL_NUM));
			break;
		case FALL_1: case FALL_2:		// 秋
			drawModel->SetModel(pModelManager->GetModel(FALL_BUD_MODEL_NUM));
			break;
		case WINTER_1: case WINTER_2:	// 冬
			drawModel->SetModel(pModelManager->GetModel(WINTER_BUD_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;
	case ICICLE:// つらら
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// モデル
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICICLE_MODEL_NUM));
		// スケール
		Transform->SetScale(ICICLE_SCALE_X, ICICLE_SCALE_Y, ICICLE_SCALE_Z);
	}
		break;
	case WEB: // 蜘蛛の巣
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// テクスチャ
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_TEX_NUM));
		// サイズ
		draw2D->SetSize(160.0f, 160.0f);
		// uv
		draw2D->SetAnimSplit(1, 1);
		// スクリーン座標の設定
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case LEAVES:// 落ち葉
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// テクスチャ
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));
		// サイズ
		draw2D->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);
		// uv
		draw2D->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		// スクリーン座標の設定
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case ICE_BLOCK:// 氷ブロック
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;
		// スケール
		Transform->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);
		// モデル
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICE_BLOCK_MODEL_NUM1));
	}
		break;
	case CLOVER_AND_LEAVES:// 落ち葉（クローバー入り）
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// テクスチャ
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));
		// サイズ
		draw2D->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);
		// uv
		draw2D->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		// スクリーン座標の設定
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case CLOVER_AND_ICE_BLOCK:// 氷ブロック（クローバー入り）
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;
		// スケール
		Transform->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);
		// モデル
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICE_BLOCK_MODEL_NUM1));
	}
		break;

	case BLOCK_LOW:// ブロックの下の方
	{
		// 描画フラグ
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// スケール
		Transform->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);
		// 四季によってテクスチャを変える
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// 春
			drawModel->SetModel(pModelManager->GetModel(SPRING_BLOCK_LOW_MODEL_NUM));
			break;
		case SUMMER_1:	case SUMMER_2:	// 夏
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BLOCK_LOW_MODEL_NUM));
			break;
		case FALL_1:	case FALL_2:	// 秋
			drawModel->SetModel(pModelManager->GetModel(FALL_BLOCK_LOW_MODEL_NUM));
			break;
		case WINTER_1:	case WINTER_2:	// 冬
			drawModel->SetModel(pModelManager->GetModel(WINTER_BLOCK_LOW_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;

	case MAX_CHIP:
	default:
		break;
	}

	//// モデル設定
	//if (m_nMap_Sta == N)
	//{
	//	
	//}
	//
	//else if (m_nMap_Sta == B)
	//{
	//	
	//}
	//else if (m_nMap_Sta == BLOCK_LOW)
	//{
	//	
	//}
	//else if (m_nMap_Sta == CLOVER)
	//{
	//	
	//}
	//else if (m_nMap_Sta == KEY)
	//{
	//	
	//}
	//else if (m_nMap_Sta == GOAL)
	//{
	//	
	//}
	//else if (m_nMap_Sta == ICICLE)
	//{
	//	
	//}
	//else if (m_nMap_Sta == ICE_BLOCK)
	//{
	//	
	//}
	//else
	//{
	//	m_pShowObject->GetComponent<CTransform>()->SetScale(0.3f, 0.3f, 0.3f);
	//	// 四季によってテクスチャを変える
	//	ModelManager* pModelManager = ModelManager::GetInstance();
	//	switch (SceneGame::GetInstance()->GetSeason())
	//	{
	//	case SPRING_1:
	//	case SPRING_2:	// 春
	//		Draw->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
	//		break;
	//	case SUMMER_1:
	//	case SUMMER_2:	// 夏
	//		Draw->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
	//		break;
	//	case FALL_1:
	//	case FALL_2:	// 秋
	//		Draw->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
	//		break;
	//	case WINTER_1:
	//	case WINTER_2:	// 冬
	//		Draw->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
	//		break;
	//	default:break;
	//	}
	//}
}
//-----------------------------------------------