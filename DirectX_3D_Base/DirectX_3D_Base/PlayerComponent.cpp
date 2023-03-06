#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"
#include "fade.h"

#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "FallComponent.h"
#include "WindComponent.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "GaugeComponent.h"
#include "MenuManager.h"
#include "sceneGame.h"
#include "ModelManager.h"
#include "SceneSelect.h"
#include "Sound.h"
#include "Camera.h"
#include "WindManager.h"
#include "IceBlockCom.h"
#include "IcicleComponent.h"
#include "InformationComponent.h"
#include "CGameover.h"
#include "EffectManager.h"
#include "CloverComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"

//2022/1/26/Touch_Tutorial Shimizu Yosuke ----------------------
#include "WindManager.h"
#include "sceneGame.h"
#include "sceneTitle.h"
//--------------------------------------------------------------

#include "GetUIComponent.h"

//***静的メンバ変数
CGoletter Goletter;


#define MAX_PLAYER_VELOCITY_Y	(1.3f)		// y軸の速度制限
#define MAX_PLAYER_GRAVITY		(1.0f)		// プレイヤーの重力
#define	RATE_MOVE_MODEL			(0.03f)		// 移動慣性係数

#define	VALUE_MOVE_MODEL	(1.0f)		// 移動速度
#define	VALUE_ROTATE_MODEL	(9.0f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.10f)		// 回転慣性係数



#ifdef _DEBUG
#include "Camera.h"
#endif // _DEBUG


// コンストラクタ
CPlayer::CPlayer()
	: m_pPlayer(nullptr), m_pDraw3D(nullptr)
	, m_fDiifRotY(0.0f)
	, m_bGoalFlg(false), m_bGround(false)
	, m_OldPos(0.0f,0.0f)
	, m_nAnimState(0)
	, m_nColWindCnt(-1)
{
	this->m_eUpdateOrder = COM_UPDATE_1;

	for (int i = 0; i < 4; i++)
	{
		m_bClover[i] = false;
	}

	// クローバー取得状況の確認
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	int nStage = SceneGame::GetInstance()->GetSeason() * 4;
	for (int i = 0; i < 4; i++)
	{
		m_bClover[i] = CloverCSV->GetInt(nStage + i, 0);

		if (m_bClover[i])
		{
			//UIを追加する
			Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
			//コンポーネントをついかする
			auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
			auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
			//オブジェクトの設定
			//ここでポジションの設定をする
			switch (i) {
			case 0:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
				break;
			case 1:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
				break;
			case 2:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
				break;
			case 3:
				TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
				DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
				break;
			default: break;
			}

			DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_TEX_NUM));
			DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pUI_Clover);
		}
	}

	m_Delete_UI.resize(0);
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 開始
void CPlayer::Start()
{
	m_pDraw3D = Parent->GetComponent<CDrawModel>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_Collider = Parent->GetComponent<CCollider>();
	m_pPlayer->Rotate.y = -45.0f;
	m_rotDestModel.y = -45.0f;
}

// 更新
void CPlayer::Update()
{
	m_OldPos = m_Collider->GetCenterPos();

	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

#ifdef _DEBUG
	if (INPUT->GetKeyPress(DIK_A))
	{
		// 左移動
		m_pPlayer->Vel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
	
		m_rotDestModel.y = rotCamera.y + 45.0f;
	}
	else if (INPUT->GetKeyPress(DIK_D))
	{
		//	右移動
		m_pPlayer->Vel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
	
		m_rotDestModel.y = rotCamera.y + -45.0f;
	}

	if (INPUT->GetKeyPress(DIK_W))
	{
		m_pPlayer->Vel.y = 1.0f;
	}
	else if (INPUT->GetKeyPress(DIK_W))
	{
		m_pPlayer->Vel.y = -1.0f;
	}
#endif // _DEBUG

	// 目的の角度までの差分
	m_fDiifRotY = m_rotDestModel.y - m_pPlayer->Rotate.y;
	if (m_fDiifRotY >= 180.0f) m_fDiifRotY -= 360.0f;
	if (m_fDiifRotY < -180.0f) m_fDiifRotY += 360.0f;
	// 目的の角度まで慣性をかける
	m_pPlayer->Rotate.y += m_fDiifRotY * RATE_ROTATE_MODEL;
	if (m_pPlayer->Rotate.y > 360.0f)	m_pPlayer->Rotate.y -= 360.0f;
	if (m_pPlayer->Rotate.y < 0.0f)		m_pPlayer->Rotate.y += 360.0f;

	// X移動量に慣性をかける
	m_pPlayer->Vel.x += -m_pPlayer->Vel.x * RATE_MOVE_MODEL;

	//	速度上限
	if (m_pPlayer->Vel.x > MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.x = MAX_PLAYER_VELOCITY_Y;
	if (m_pPlayer->Vel.x < -MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.x =  -MAX_PLAYER_VELOCITY_Y;

	if (m_pPlayer->Vel.y > MAX_PLAYER_VELOCITY_Y) m_pPlayer->Vel.y = MAX_PLAYER_VELOCITY_Y;
	if (m_pPlayer->Vel.y < -MAX_PLAYER_GRAVITY)	 m_pPlayer->Vel.y = -MAX_PLAYER_GRAVITY;


	//	左右に揺れるフラグ
	if (m_pPlayer->Vel.y < 0.0f)
	{
		auto Leaf = Parent->GetComponent<CFall>();
		if (Leaf) Leaf->SetActive(true);
	}
	else
	{
		//	地上フラグOFF
		m_bGround = false;
	}

	// アニメーション制御	2022/01/11
	if (m_nColWindCnt > 0)
	{
		m_nColWindCnt--;
		if (m_nColWindCnt == 0)
		{
			// 風に吹かれている状態だったら
			if (m_nAnimState == PLAYER_CARRIED)
			{
				//	地上フラグOFF
				m_bGround = false;
			}

			// リセット
			m_nColWindCnt = -1;
		}
	}

	// アニメーションが終わった
	CAnimation* pAnim = Parent->GetComponent<CAnimation>();
	if (pAnim && pAnim->GetMoel(FBX_ONE)->AnimEnd(PLAYER_GET) == true)
	{
		SetAnimState(PLAYER_IDLE);
	}

	if (m_nAnimState == PLAYER_GET)
	{
		m_pPlayer->Vel.x = 0.0f;
		m_pPlayer->Vel.y = 0.0f;
	}
	
	if (pAnim)
	{
		// アニメーション再生
		pAnim->GetMoel(FBX_ONE)->Play(m_nAnimState);
	}
	
	
}

//	描画
void CPlayer::Draw()
{
#ifdef _DEBUG
	//	debug
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Player");
	Text("Pos	: %3.0f %3.0f %3.0f", m_pPlayer->Pos.x, m_pPlayer->Pos.y, m_pPlayer->Pos.z);
	Text("Vel	: %.0f %.0f", m_pPlayer->Vel.x, m_pPlayer->Vel.y);
	Text("goal   %d", m_bGoalFlg);
	Text("ground %d", m_bGround);
	End();
#endif // _DEBUG
}

//	衝突処理
void CPlayer::OnCollisionEnter(Object* pObject)
{
	//2022/1/26/Touch_Tutorial Shimizu Yosuke ----------------------------------------------
	//チュートリアルを出すための場所
	//一回しか通らず、風を出せるようにする
	//ここを通らないと風は出せない
	//このフラグがfalseで地面に当たったときにオンにする
	int nSeason = SceneGame::GetInstance()->GetSeason();
	if (SceneTitle::FirstPlay(nSeason) == true)
	{
		if (!m_bTouch_Tutorial) {
			if (pObject->GetName() == BLOCK_NAME)
			{
				SceneTitle::SaveTutorial();

				//  チュートリアルフラグON
				m_bTouch_Tutorial = true;

				//チュートリアルのフラグをオンにしてチュートリアルを出せるようにする
				SceneGame::GetInstance()->SetTutorial(true);
			}
		}
	}
	//---------------------------------------------------------------------------------------

#pragma region ---植木鉢ゴール
	if (pObject->GetName() == POT_NAME)
	{
		//	ゴールできる状態だったら
		if (m_bGoalFlg && m_bGround)
		{
			m_bGoalFlg = false;

			m_rotDestModel.y = -45.0f;

			// 2022/01/16
			// カメラの位置を変える
			CCamera::Get()->SetDestPos(GOAL_POS_Z);

			//	アニメーション = クリア
			auto Anime = Parent->GetComponent<CAnimation>();
			if (Anime)
			{
				Anime->ChangeAnimation(FBX_TWO);
				Anime->Update();	//	アニメーション更新
			}
			//	プレイヤー機能停止
			Parent->StopUpdate();
			// アニメーション機能だけ機能させる
			Parent->GetComponent<CAnimation>()->GetMoel(FBX_TWO)->m_bUpdateFlag = true;
			Parent->GetComponent<CAnimation>()->GetMoel(FBX_TWO)->Play(PLAYER_GOAL);

#pragma region ---花を咲かせる
			Object* pFlower = new Object("Flower", UPDATE_MODEL, DRAW_MODEL);
			// コンポーネント追加
			auto Trans = pFlower->AddComponent<CTransform>();		// オブジェクトの座標
			auto Draw = pFlower->AddComponent<CDrawModel>();			// モデル描画
			// オブジェクト設定
			Trans->Pos = pObject->GetComponent<CTransform>()->Pos;	// 座標
			Trans->Pos.x += FLOAER_POS_X_OFFSET;					// 座標
			Trans->Pos.y += FLOAER_POS_Y_OFFSET;					// 座標
			Trans->Pos.z += FLOAER_POS_Z_OFFSET;					// 座標
			Trans->SetScale(FLOAER_SCALE_X, FLOAER_SCALE_Y, FLOAER_SCALE_Z);// スケール
			Trans->SetRotate(FLOAER_ROTATE_X, 0.0f, 0.0f);
			// 四季によってテクスチャを変える
			ModelManager* pModelManager = ModelManager::GetInstance();
			switch (SceneGame::GetInstance()->GetSeason())
			{
			case SPRING_1:	case SPRING_2:	// 春
				Draw->SetModel(pModelManager->GetModel(SPRING_FLOWER_MODEL_NUM));
				break;
			case SUMMER_1:	case SUMMER_2:	// 夏
				Draw->SetModel(pModelManager->GetModel(SUMMER_FLOWER_MODEL_NUM));
				break;
			case FALL_1:	case FALL_2:	// 秋
				Draw->SetModel(pModelManager->GetModel(FALL_FLOWER_MODEL_NUM));
				break;
			case WINTER_1:	case WINTER_2:	// 冬
				Draw->SetModel(pModelManager->GetModel(WINTER_FLOWER_MODEL_NUM));
				break;
			default:break;
			}
			// アニメーション
			Draw->AddAnimation(0, FLOAER_ANIM_S, FLOAER_ANIM_E, false);
			Draw->Play(0);
			// リストに追加
			ObjectManager::GetInstance()->AddObject(pFlower);
#pragma endregion

			// 次のステージ解放
			SceneSelect::SaveClearInfo(SceneGame::GetInstance()->GetSeason(), m_bClover);


			//	BGM停止
			CSound::Stop(BGM_SPRING);
			CSound::Stop(BGM_SUMMER);
			CSound::Stop(BGM_FALL);
			CSound::Stop(BGM_WINTER);

			// クリアの音 
			CSound::Play((SE_GAME_CLEAR));


			// ゴールエフェクト
			// 通常Ver.
			EffectManager::GetInstance()->Play(SIMPLE_GOAL_EFFECT_NUM, Trans->Pos, XMFLOAT3(20.0f, 20.0f, 20.0f));
			bool bAllClover = true;
			for (int i = 0; i < 4; i++)
			{
				if (m_bClover[i] == false) bAllClover = false;
			}
			if (bAllClover)
			{
				// クローVer.
				EffectManager::GetInstance()->Play(CLOVER_GOAL_EFFECT_NUM, Trans->Pos, XMFLOAT3(20.0f, 20.0f, 20.0f));
			}


			// 最後のステージだったら
			if (SceneGame::GetInstance()->GetSeason() == WINTER_2)
			{
				//	シーン遷移予約
				StartFadeOutSceneChange(SCENE_ENDING, 240);
			}
			else
			{
				//	シーン遷移予約
				StartFadeOutSceneChange(SCENE_SELECT, 240);
			}

			

			

			// メニューを開くボタンを消す
			Object* pMenu = ObjectManager::GetInstance()->GetGameObject(STOP_GAME_NAME);
			if (pMenu) pMenu->Delete();

			//メニューを開くボタンを消すの下に追加
//2022/1/26/Touch_Tutorial Shimizu Yosuke --------------------------------
			Object* pGauge = ObjectManager::GetInstance()->GetGameObject("Gauge");
			if (pGauge) pGauge->Delete();
			Object* pGauge_frame = ObjectManager::GetInstance()->GetGameObject("frame");
			if (pGauge_frame) pGauge_frame->Delete();
			//新しくリストを作る
			//作ってくれた関数でリストの中に追加する
			ObjectManager::GetInstance()->GetGameObjects(SEED_UI_NAME, m_Delete_UI);
			//中身のデリートフラグをオンにしていく
			ObjectManager::GetInstance()->DeleteObjectByName(SEED_UI_NAME, m_Delete_UI);

			//使いまわしたいから0に戻す
			m_Delete_UI.resize(0);

			//作ってくれた関数でリストの中に追加する2
			ObjectManager::GetInstance()->GetGameObjects(CLOVER_UI_NAME, m_Delete_UI);
			//中身のデリートフラグをオンにしていく
			ObjectManager::GetInstance()->DeleteObjectByName(CLOVER_UI_NAME, m_Delete_UI);

			Object* pWindCar = ObjectManager::GetInstance()->GetGameObject(WINDCAR_NAME);
			if (pWindCar) pWindCar->Delete();
			Object* pWindCar_Stick = ObjectManager::GetInstance()->GetGameObject(WINDCAR_STICK_NAME);
			if (pWindCar_Stick) pWindCar_Stick->Delete();
			//------------------------------------------------------------------------


		}

		return;
	}
#pragma endregion

#pragma region ---ブロック
	if (pObject->GetName() == BLOCK_NAME)
	{
		//	プレイヤーの情報を取得
		auto Owner = Parent->GetComponent<CCollider>();		//	あたり判定の大きさが欲しい
		auto OwnerPos = Owner->GetCenterPos();				// 中心座標
		auto OwnerSize = Owner->GetColliderSize();			// あたり判定の大きさ
		auto OwnerOffset = Owner->GetOffSet();				// 中心とのずれ
		//	ぶつかった相手の情報の取得
		auto Other = pObject->GetComponent<CCollider>();	// あたり判定の大きさが欲しい
		auto OtherPos = Other->GetCenterPos();				// 中心座標
		auto OtherSize = Other->GetColliderSize();			// あたり判定の大きさ
		auto OtherOffset = Other->GetOffSet();				// 中心とのずれ

		//	それぞれの半分の大きさを格納
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize  = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);


		//	ブロックのあたり判定ライン
		float BlockLeftLine	 = OtherPos.x - BlockHalfSize.x;	//	ブロックの左端
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;	//		　  右端
		float BlockUpLine	 = OtherPos.y + BlockHalfSize.y;	//		　  上端
		float BlockDownLine	 = OtherPos.y - BlockHalfSize.y;	//		　  下端

		/*		   __
			　	〇 |P|				
				　 |_|			 __
			_________________ × |P|
		  　|←　ブロックの横幅 →|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// 左の点
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// 右の点
		{
			//	P
			//	↓
			//	□
			if (//m_pPlayer->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&				// 前はめり込んでいない
							   OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// 今はめり込んでる
			{
				// yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;

				// 座標補正
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

				// 一回のみ
				if (!m_bGround)
				{
					//ここで風を出せるようにする
					WindManager::GetInstance()->SetSpawnWind(true);

					// アニメーション = 待機モーション
					SetAnimState(PLAYER_IDLE);
				}

				// 地上フラグON
				m_bGround = true;

				// 揺れるフラグOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);

				// ゲームオーバーの確認
				if (WindManager::GetInstance()->GetWindGauge() <= 0)
				{
					//	BGM停止
					CSound::Stop(BGM_SPRING);
					CSound::Stop(BGM_SUMMER);
					CSound::Stop(BGM_FALL);
					CSound::Stop(BGM_WINTER);

					CCamera::Get()->SetDestPos(GOAL_POS_Z);		//ズーム

					//ゲームオーバー文字の表示
					Goletter.CreateGoletter();

					ObjectManager::GetInstance()->NoFunction();

					//アニメーション
					CAnimation* pAnim = Parent->GetComponent<CAnimation>();
					if (pAnim)
					{
						pAnim->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);

						// 常にアニメーションさせるのをやめる
						pAnim->GetMoel(FBX_ONE)->AlwaysAnimation(false);
						//	プレイヤー機能停止
						Parent->StopUpdate();
						// アニメーションだけさせる
						pAnim->GetMoel(FBX_ONE)->m_bUpdateFlag = true;
					}

					// game over
					CSound::Play(SE_GAME_OVER);

					// メニュー関連
					MenuManager::GetInstance()->Create(STATE_GAMEOVER, 350);
				}
			}
			//	□
			//	↑
			//	P
			else if (//m_pPlayer->Vel.y > 0 &&
					 BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// 前はめり込んでいない
									  OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// 今はめり込んでる
			{
				// yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;

				// 座標補正
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// 中心座標

		/*
				× __
				  |P|
				  |_|
		  __
		 　ブ|
		 　ロ|	〇 __
		　 ッ|	  |P|
		　 ク|	  |_|
		  __|	
		  		
		*/		
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// P→□
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&				// 前はめり込んでいない
								 OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)	// 今はめり込んでる
			{
				//	x移動量を消す
				m_pPlayer->Vel.x = 0.0f;
			
				// 座標補正
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
			// □←P
			else if(BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
									  OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
			{
				//	x移動量を消す
				m_pPlayer->Vel.x = 0.0f;
			
				// 座標補正
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
		}


		return;
	}
#pragma endregion

#pragma region ---つらら
	if (pObject->GetName() == ICICLE_UP_NAME ||
		pObject->GetName() == ICICLE_DOWN_NAME)
	{
		//	プレイヤーの情報を取得
		auto Owner = Parent->GetComponent<CCollider>();		//	あたり判定の大きさが欲しい
		auto OwnerPos = Owner->GetCenterPos();				// 中心座標
		auto OwnerSize = Owner->GetColliderSize();			// あたり判定の大きさ
		auto OwnerOffset = Owner->GetOffSet();				// 中心とのずれ
		//	ぶつかった相手の情報の取得
		auto OtherTrans = pObject->GetComponent<CTransform>();
		auto Other = pObject->GetComponent<CCollider>();	// あたり判定の大きさが欲しい
		auto OtherPos = Other->GetCenterPos();				// 中心座標
		auto OtherSize = Other->GetColliderSize();			// あたり判定の大きさ
		auto OtherOffset = Other->GetOffSet();				// 中心とのずれ

		//	それぞれの半分の大きさを格納
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);


		//	ブロックのあたり判定ライン
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;		//	ブロックの左端
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;	//		　  右端
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;		//		　  上端
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;		//		　  下端

		// 落下中か調べる
		CIcicle* pIcicle;
		if (pObject->GetName() == ICICLE_DOWN_NAME)	pIcicle = pObject->GetComponent<CInformation>()->GetObjetInfo()->GetComponent<CIcicle>();
		else										pIcicle = nullptr;
		if (pObject->GetName() == ICICLE_UP_NAME)	pIcicle = pObject->GetComponent<CIcicle>();

		// つららが落下中　もしくは　落ちきったあとならプレーヤーを左右に追い出す
		if (pIcicle && pIcicle->IsFalling() && pIcicle->IsMoving())
		{
			// つららより右側にいるとき右にずらしていく
			if (OwnerPos.x > OtherPos.x)
			{
				m_pPlayer->Vel.x += 0.5f;
			}
			else if (OwnerPos.x < OtherPos.x)
			{
				m_pPlayer->Vel.x += -0.5f;
			}
		}
		// つららが落下中でないときは普通のブロックと同じ処理
		else
		{
			/*		   __
			　	〇 |P|
				　 |_|			 __
			_________________ × |P|
		  　|←　ブロックの横幅 →|   |_|
		*/
			if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
				(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// 左の点
				(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// 右の点
			{
				//	P
				//	↓
				//	□
				if (//m_pPlayer->Vel.y < 0 &&
					BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&				// 前はめり込んでいない
					OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// 今はめり込んでる
				{
					// yの速度をなくす
					m_pPlayer->Vel.y = 0.0f;

					// 座標補正
					m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

					// 一回のみ
					if (!m_bGround)
					{
						// アニメーション = 待機モーション
						SetAnimState(PLAYER_IDLE);
					}

					// 地上フラグON
					m_bGround = true;

					// 揺れるフラグOFF
					auto Leaf = Parent->GetComponent<CFall>();
					if (Leaf) Leaf->SetActive(false);

					// ゲームオーバーの確認
					if (WindManager::GetInstance()->GetWindGauge() <= 0)
					{
						//アニメーション
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);
						// 常にアニメーションさせるのをやめる
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->AlwaysAnimation(false);
						//	プレイヤー機能停止
						Parent->StopUpdate();
						// アニメーションだけさせる
						Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->m_bUpdateFlag = true;

						// game over
						CSound::Play(SE_GAME_OVER);

						// メニュー関連
						MenuManager::GetInstance()->Create(STATE_GAMEOVER, 240);
					}
				}
				//	□
				//	↑
				//	P
				else if (//m_pPlayer->Vel.y > 0 &&
					BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// 前はめり込んでいない
					OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// 今はめり込んでる
				{
					// yの速度をなくす
					m_pPlayer->Vel.y = 0.0f;

					// 座標補正
					m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
				}
			}


			OwnerPos = Owner->GetCenterPos();				// 中心座標

			/*
					× __
					  |P|
					  |_|
			  __
			 　ブ|
			 　ロ|	〇 __
			　 ッ|	  |P|
			　 ク|	  |_|
			  __|

			*/
			if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
				(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
			{
				// P→□
				if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&				// 前はめり込んでいない
					OwnerPos.x + PlayerHalhSize.x > BlockLeftLine)	// 今はめり込んでる
				{
					//	x移動量を消す
					m_pPlayer->Vel.x = 0.0f;

					// 座標補正
					m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
				}
				// □←P
				else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
					OwnerPos.x - PlayerHalhSize.x < BlockRightLine)
				{
					//	x移動量を消す
					m_pPlayer->Vel.x = 0.0f;

					// 座標補正
					m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
				}
			}
		}

		

		return;
	}
#pragma endregion

#pragma region ---氷ブロック
	if (pObject->GetName() == ICE_BLOCK_NAME)
	{
		//	プレイヤーの情報を取得
		auto Owner = Parent->GetComponent<CCollider>();		//	あたり判定の大きさが欲しい
		auto OwnerPos = Owner->GetCenterPos();				// 中心座標
		auto OwnerSize = Owner->GetColliderSize();			// あたり判定の大きさ
		auto OwnerOffset = Owner->GetOffSet();				// 中心とのずれ
		//	ぶつかった相手の情報の取得
		auto Other = pObject->GetComponent<CCollider>();	// あたり判定の大きさが欲しい
		auto OtherPos = Other->GetCenterPos();				// 中心座標
		auto OtherSize = Other->GetColliderSize();			// あたり判定の大きさ
		auto OtherOffset = Other->GetOffSet();				// 中心とのずれ

		//	それぞれの半分の大きさを格納
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(OwnerSize.x * 0.5f, OwnerSize.y * 0.5f);
		XMFLOAT2 BlockHalfSize = XMFLOAT2(OtherSize.x * 0.5f, OtherSize.y * 0.5f);

		//	ブロックのあたり判定ライン
		float BlockLeftLine = OtherPos.x - BlockHalfSize.x;	//	ブロックの左端
		float BlockRightLine = OtherPos.x + BlockHalfSize.x;//		　  右端
		float BlockUpLine = OtherPos.y + BlockHalfSize.y;	//		　  上端
		float BlockDownLine = OtherPos.y - BlockHalfSize.y;	//		　  下端

		/*		   __
			　	〇 |P|
				　 |_|			 __
			_________________ × |P|
		  　|←　ブロックの横幅 →|   |_|
		*/
		if ((BlockLeftLine < OwnerPos.x && OwnerPos.x < BlockRightLine) ||
			(BlockLeftLine < OwnerPos.x - PlayerHalhSize.x && OwnerPos.x - PlayerHalhSize.x < BlockRightLine) ||	// 左の点
			(BlockLeftLine < OwnerPos.x + PlayerHalhSize.x && OwnerPos.x + PlayerHalhSize.x < BlockRightLine))		// 右の点
		{
			//	P
			//	↓
			//	□
			if (m_pPlayer->Vel.y < 0 &&
				BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&	// 前はめり込んでいない
				OwnerPos.y - PlayerHalhSize.y < BlockUpLine)	// 今はめり込んでる
			{
				// yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;

				// 座標補正
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - OwnerOffset.y;

				// 一回のみ
				if (!m_bGround)
				{
					// アニメーション = 待機モーション
					SetAnimState(PLAYER_IDLE);
				}

				// 地上フラグON
				m_bGround = true;

				// 揺れるフラグOFF
				auto Leaf = Parent->GetComponent<CFall>();
				if (Leaf) Leaf->SetActive(false);

				// ゲームオーバーの確認
				if (WindManager::GetInstance()->GetWindGauge() <= 0)
				{
					//アニメーション
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->Play(PLAYER_GAMEOVER);
					// 常にアニメーションさせるのをやめる
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->AlwaysAnimation(false);
					//	プレイヤー機能停止
					Parent->StopUpdate();
					// アニメーションだけさせる
					Parent->GetComponent<CAnimation>()->GetMoel(FBX_ONE)->m_bUpdateFlag = true;

					// game over
					CSound::Play(SE_GAME_OVER);

					// メニュー関連
					MenuManager::GetInstance()->Create(STATE_GAMEOVER, 240);
				}
			}
			//	□
			//	↑
			//	P
			else if (//m_pPlayer->Vel.y > 0 &&
				BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&				// 前はめり込んでいない
				OwnerPos.y + PlayerHalhSize.y > BlockDownLine)// 今はめり込んでる
			{
				// yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;

				// 座標補正
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - OwnerOffset.y;
			}
		}


		OwnerPos = Owner->GetCenterPos();				// 中心座標

		/*
				× __
				  |P|
				  |_|
		  __
		 　ブ|
		 　ロ|	〇 __
		　 ッ|	  |P|
		　 ク|	  |_|
		  __|

		*/
		if ((BlockDownLine < OwnerPos.y && OwnerPos.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y - PlayerHalhSize.y && OwnerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < OwnerPos.y + PlayerHalhSize.y && OwnerPos.y + PlayerHalhSize.y < BlockUpLine))
		{
			// おはじき
			m_pPlayer->Vel.x = pObject->GetComponent<CTransform>()->Vel.x;	// 速度を衝突したオブジェクトに移す
			pObject->GetComponent<CTransform>()->Vel.x = 0.0f;				// 移した側の速度を消す

			// 座標補正
			// 氷ブロックの速度で決める
			if (OwnerPos.x > OtherPos.x)		// 氷が→向き
			{
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - OwnerOffset.x;
			}
			else if (OwnerPos.x < OtherPos.x)	// 氷が←向き
			{
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - OwnerOffset.x;
			}
		}

		return;
	}
#pragma endregion

#pragma region ---種
	if (m_bGround && pObject->GetName() == SEED_NAME)
	{
		//種を取得したから消せるようにする
		pObject->Delete();

		EffectManager::GetInstance()->Play(GET_ITEM_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(10.0f, 10.0f, 10.0f));

		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// 春
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_SPRING_TEX_NUM),	// テクスチャ
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// 座標
				XMFLOAT2(150.0f, 150.0f),					// 大きさ
				5, 6);										// 分割数(横)
			break;
		case SUMMER_1:	case SUMMER_2:	// 夏
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_SUMMER_TEX_NUM),	// テクスチャ
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// 座標
				XMFLOAT2(150.0f, 150.0f),					// 大きさ
				5, 6);										// 分割数(横)
			break;
		case FALL_1:	case FALL_2:	// 秋
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_AUTUMN_TEX_NUM),	// テクスチャ
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// 座標
				XMFLOAT2(150.0f, 150.0f),					// 大きさ
				5, 6);										// 分割数(横)
			break;
		case WINTER_1:	case WINTER_2:	// 冬
			CDrawEffect2D::StartEffect(TextureManager::GetInstance()->GetTexture(EFFECT_WINTER_TEX_NUM),	// テクスチャ
				XMFLOAT2(SEED_UI_POS_X, SEED_UI_POS_Y),		// 座標
				XMFLOAT2(150.0f, 150.0f),					// 大きさ
				5, 6);										// 分割数(横)
			break;
		default:break;
		}

		//オブジェクトの生成
		Object* pSeed_UI = new Object(SEED_UI_NAME, UPDATE_UI, DRAW_UI);
		//  コンポネントの追加
		auto TransSeed_UI = pSeed_UI->AddComponent<CTransform>();			//　座標
		auto DrawSeed_UI = pSeed_UI->AddComponent<CDraw2D>();				//　描画
		auto GetSeed_UI = pSeed_UI->AddComponent<CGetUI>();					//　UI演出
		// オブジェクトの設定
		// ↓0.0fに変更しています。
		DrawSeed_UI->SetSize(0.0f, 0.0f);				//  サイズ
		GetSeed_UI->SetUISize(XMFLOAT2(SEED_UI_SIZE_X, SEED_UI_SIZE_Y));	//　UI最大サイズ
		GetSeed_UI->SetUIRate(9.0f);										//　拡大スピード

		//  オブジェクトの設定
		TransSeed_UI->SetPosition(SEED_UI_POS_X, SEED_UI_POS_Y);			//  座標
		DrawSeed_UI->SetTexture(TextureManager::GetInstance()->GetTexture(SEED_UI_TEX_NUM));	//  テクスチャのセット
		//DrawSeed_UI->SetSize(SEED_UI_SIZE_X, SEED_UI_SIZE_Y);				//  サイズ
		//  リストに追加
		ObjectManager::GetInstance()->AddObject(pSeed_UI);
		// アニメーション = ゲット
		SetAnimState(PLAYER_GET);
		// se
		CSound::Play(SE_GET_CLOVER);	// ←ここに追加

		// 速度をなくす
		m_pPlayer->Vel.x = 0.0f;

		//	ゴールフラグON
		m_bGoalFlg = true;

		return;
	}
#pragma endregion

	//2021/12/24 Shimizu Yosuke
#pragma region ---クローバー
	// 2022/01/24 クモの巣に拘束されていない条件を追加
	if (pObject->GetName() == CLOVER_NAME && pObject->GetComponent<CloverComponent>()->IsBinded() == false)
	{
		// クローバー削除
		pObject->Delete();

		// ゲットSE
		CSound::Play(SE_GET_CLOVER);

		// エフェクト
		EffectManager::GetInstance()->Play(GET_ITEM_EFFECT_NUM, pObject->GetComponent<CTransform>()->Pos, XMFLOAT3(10.0f, 10.0f, 10.0f));

		// 地上にいるとき
		if (m_bGround)
		{
			// アニメーション = ゲット
			SetAnimState(PLAYER_GET);
		}

		// 速度をなくす
		m_pPlayer->Vel.x = 0.0f;

		//クローバーの番号
		int CloverNumber = pObject->GetComponent<CloverComponent>()->GetNumber();
		if (CloverNumber >= 4)	return;	// 例外処理
		
		m_bClover[CloverNumber] = true;

		//UIを追加する
		Object* pUI_Clover = new Object(CLOVER_UI_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントをついかする
		auto TransClov_UI = pUI_Clover->AddComponent<CTransform>();
		auto DrawClov_UI = pUI_Clover->AddComponent<CDraw2D>();
		auto GetClov_UI = pUI_Clover->AddComponent<CGetUI>();
		// オブジェクトの設定
		// ↓0.0fに変更しています。
		DrawClov_UI->SetTexture(TextureManager::GetInstance()->GetTexture(CLOVER_UI_TEX_NUM));
		//DrawClov_UI->SetSize(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y);
		DrawClov_UI->SetSize(0.0f, 0.0f);
		GetClov_UI->SetUISize(XMFLOAT2(CLOVER_UI_SIZE_X, CLOVER_UI_SIZE_Y));
		GetClov_UI->SetUIRate(3.0f);
		//オブジェクトの設定
		//ここでポジションの設定をする
		switch (CloverNumber) {
		case 0:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE);
			break;
		case 1:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE);
			break;
		case 2:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE - CLOVER_ROTATE * 2);
			break;
		case 3:
			TransClov_UI->SetPosition(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			DrawClov_UI->SetAngle(CLOVER_ROTATE_BASE + CLOVER_ROTATE * 3);
			break;
		default: break;
		}
		//リストに追加
		ObjectManager::GetInstance()->AddObject(pUI_Clover);

		// クローバー
		switch (CloverNumber)
		{
		case 0:
			//TransClov_UI->SetPosition(CLOVER_UI_POS_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE);
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// テクスチャ
				XMFLOAT2(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE),	// 座標
				XMFLOAT2(80.0f, 80.0f),										// 大きさ
				5,																// 分割数(横)
				6);																// 分割数(縦)
			break;
		case 1:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// テクスチャ
				XMFLOAT2(CLOVER_UI_POS_X - CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y),	// 座標
				XMFLOAT2(80.0f, 80.0f),										// 大きさ
				5,																// 分割数(横)
				6);																// 分割数(縦)
			break;
		case 2:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// テクスチャ
				XMFLOAT2(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y - CLOVER_POS_CHENGE / CLOVER_UI_GAP_Y),	// 座標
				XMFLOAT2(80.0f, 80.0f),										// 大きさ
				5,																// 分割数(横)
				6);																// 分割数(縦)
			break;
		case 3:
			CDrawEffect2D::StartEffect(
				TextureManager::GetInstance()->GetTexture(EFFECT_CLOVER_TEX_NUM),	// テクスチャ
				XMFLOAT2(CLOVER_UI_POS_X + CLOVER_POS_CHENGE / CLOVER_UI_GAP_X, CLOVER_UI_POS_Y + CLOVER_POS_CHENGE),	// 座標
				XMFLOAT2(80.0f, 80.0f),										// 大きさ
				5,																// 分割数(横)
				6);																// 分割数(縦)
			break;
		default:
			break;
		}

		return;
	}
#pragma endregion

#pragma region ---風
	if (pObject->GetName() == WIND_NAME)
	{
		m_nColWindCnt = 20;
	}
#pragma endregion

#pragma region ---エンディングのクローバー
	// 2022/01/24 クモの巣に拘束されていない条件を追加
	if (pObject->GetName() == ENDING_CLOVER_NAME)
	{
		// クローバー削除
		pObject->Delete();

		CSound::Play(SE_GET_CLOVER);	// ←ここに追加

		return;
	}
#pragma endregion
}

// 向いてる方向の設定
void CPlayer::SetDestRot(float angle)
{
	m_rotDestModel.y = CCamera::Get()->GetPos().y + angle;
}

// 地上フラグの設定
void CPlayer::SetGround(bool ground)
{
	m_bGround = ground;
}

// アニメーション
void CPlayer::SetAnimState(int next_state)
{
	// 拘束状態の時は待機モーションにしか移行できない
	if (m_nAnimState == PLAYER_BIND && next_state != PLAYER_IDLE) return;

	// ゲットモーションの時は待機モーションにしか移行できない
	if (m_nAnimState == PLAYER_GET && next_state != PLAYER_IDLE) return;

	// アニメーションstate切り替え
	m_nAnimState = next_state;
}