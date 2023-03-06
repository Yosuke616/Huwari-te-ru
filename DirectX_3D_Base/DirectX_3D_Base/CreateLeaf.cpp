#include "CreateLeaf.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "ColliderComponent.h"
#include "fallleafComponent.h"
#include "DrawEffectComponent.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "sceneGame.h"
#include "fallleafComponent.h"
#include "SceneSelect.h"
#include "SceneManager.h"
#include "sceneTitle.h"

//	コンストラクタ
CLeaf::CLeaf()
	:cnt(0)
{

}
//	デストラクタ
CLeaf::~CLeaf()
{
}

//	ゲームでの更新
void CLeaf::Update()
{
	//カウント加算
	cnt++;

	//2022/1/9 Shimizu Yosuke --------------------------------------------------
	switch (SceneGame::GetInstance()->GetSeason()) {
	case SPRING_1:	case SPRING_2:	//春＊＊＊10フレームごとにはっぱを描画
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case SUMMER_1:	case SUMMER_2:	//夏＊＊＊40フレームごとにはっぱを描画
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case FALL_1:	case FALL_2:	//秋＊＊＊20フレームごとにはっぱを描画
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case WINTER_1:	case WINTER_2:	//冬＊＊＊5フレームごとに雪を描画
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
}

// タイトルでの更新
void CLeaf::UpdateTitleScene()
{
	//カウント加算
	cnt++;
	int nSeason;
	nSeason = SceneTitle::m_nTitleSeason;

	//2022/1/9 Shimizu Yosuke --------------------------------------------------
	switch (nSeason) {
	case 0:
		//春＊＊＊10フレームごとにはっぱを描画
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 1:
		//夏＊＊＊40フレームごとにはっぱを描画
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 2:
		//秋＊＊＊20フレームごとにはっぱを描画
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 3:
		//冬＊＊＊フェードを貫通している
		//5フレームごとに雪を描画
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
	//--------------------------------------------------------------------------
}

// セレクトでの更新
void CLeaf::UpdateSelectScene()
{
	//カウント加算
	cnt++;

	switch (SceneSelect::m_nCurrentSeason)
	{
	case 0:	//春＊＊＊10フレームごとにはっぱを描画
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 1:	//夏＊＊＊40フレームごとにはっぱを描画
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 2:	//秋＊＊＊20フレームごとにはっぱを描画
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 3:	//冬＊＊＊5フレームごとに雪を描画
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
}

//	葉っぱ生成
void CLeaf::CreateLeaf()
{
	switch (SceneManager::Instance()->GetScene())
	{
	case SCENE_TITLE:	//	タイトル画面
		//一番上
	//ここでも春夏秋冬で変えれるようにしたい
	//2022/1/7 Shimizu Yosuke -------------------------------------------------
		int nSeason;
		nSeason = SceneTitle::m_nTitleSeason;

		//冬以外
		if (nSeason != 3)
		{
			//ランダムで葉っぱの出す位置の決定
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	回転速度をランダムにする
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//オブジェクトの生成
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//トランスフォーム
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//描画
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pLeaf);

			//もう一つ裏側のオブジェクトを作ろうとしたやつ（オブジェクトを作った下に貼る）
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//ここに裏側のオブジェクトを作成する
			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//コンポーネントの追加
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//トランスフォーム
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//描画
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //葉っぱの落下	 
			//オブジェクトの設定
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			//--------------------------------------------------------------------------------------------------------

			//オブジェクト生成した後に貼る
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//ここを変更できるようにする（テクスチャをここで変更する）
			switch (nSeason) {
				//春
			case 0:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//夏
			case 1:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//秋
			case 2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
			//---------------------------------------------------------------------------------------------------------

		}
		//冬のとき
		else
		{
			//===== 雪ver

			//ランダムの場所から出るようにする
			//ｶﾒﾗで写っている場所のワールド座標を出す
			XMFLOAT2 WorldSize = CalcWorldSize();
			//移っている場所にだけ降らせるようにする
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//ｶﾒﾗの移っている範囲のエックス座標に移す
			//元の式のままでは画面の右側しか出てこない
			//今は右半分には出続けるけど左に出ない
			float RandLeafX = rand() % ((int)WorldSize.x) + CameraScroll.x - 200.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//回転させるやつはいらない

			//オブジェクトの作成
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transSnow = pSnow->AddComponent<CTransform>();			//トランスフォーム
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//描画
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//テクスチャの設定
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//ループフラグをオンにする
			drawSnow->SetLoop(true);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pSnow);
		}

		break;

	case SCENE_GAME:	//	ゲーム画面
		//一番上
	//ここでも春夏秋冬で変えれるようにしたい
	//2022/1/7 Shimizu Yosuke -------------------------------------------------
		Season Season;
		Season = SceneGame::GetInstance()->GetSeason();

		//春夏秋と冬で作るものを分ける(if (Season != WINTER_1 && Season != WINTER_2) を追加してほしい先頭に)
		if (Season != WINTER_1 && Season != WINTER_2)
		{
			//ランダムで葉っぱの出す位置の決定
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	回転速度をランダムにする
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//オブジェクトの生成
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//トランスフォーム
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//描画
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pLeaf);

			//もう一つ裏側のオブジェクトを作ろうとしたやつ（オブジェクトを作った下に貼る）
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//ここに裏側のオブジェクトを作成する
			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//コンポーネントの追加
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//トランスフォーム
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//描画
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //葉っぱの落下	 
			//オブジェクトの設定
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			//--------------------------------------------------------------------------------------------------------

			//オブジェクト生成した後に貼る
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//ここを変更できるようにする（テクスチャをここで変更する）
			switch (Season) {
				//春
			case SPRING_1:
			case SPRING_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//夏
			case SUMMER_1:
			case SUMMER_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//秋
			case FALL_1:
			case FALL_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
			//---------------------------------------------------------------------------------------------------------

		}
		else
		{
			//===== 雪ver

			//ランダムの場所から出るようにする
			//ｶﾒﾗで写っている場所のワールド座標を出す
			XMFLOAT2 WorldSize = CalcWorldSize();
			//移っている場所にだけ降らせるようにする
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//ｶﾒﾗの移っている範囲のエックス座標に移す
			//元の式のままでは画面の右側しか出てこない
			//今は右半分には出続けるけど左に出ない
			float RandLeafX = rand() % ((int)WorldSize.x)*2 + CameraScroll.x - 300.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//回転させるやつはいらない

			//オブジェクトの作成
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transSnow = pSnow->AddComponent<CTransform>();			//トランスフォーム
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//描画
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//テクスチャの設定
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//ループフラグをオンにする
			drawSnow->SetLoop(true);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pSnow);
		}
		//---------------------------------------------------------------------------------------------

		break;

	case SCENE_SELECT:
		nSeason = SceneSelect::m_nCurrentSeason;

		//冬以外
		if (nSeason != 3)
		{
			//ランダムで葉っぱの出す位置の決定
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	回転速度をランダムにする
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//オブジェクトの生成
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//トランスフォーム
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//描画
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pLeaf);

			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//コンポーネントの追加
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//トランスフォーム
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//描画
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //葉っぱの落下	 
			//オブジェクトの設定
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			switch (nSeason) {
				//春
			case 0:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//夏
			case 1:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//秋
			case 2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
		}
		//冬のとき
		else
		{
			//===== 雪ver

			//ランダムの場所から出るようにする
			//ｶﾒﾗで写っている場所のワールド座標を出す
			XMFLOAT2 WorldSize = CalcWorldSize();
			//移っている場所にだけ降らせるようにする
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//ｶﾒﾗの移っている範囲のエックス座標に移す
			//元の式のままでは画面の右側しか出てこない
			//今は右半分には出続けるけど左に出ない
			float RandLeafX = rand() % ((int)WorldSize.x) + CameraScroll.x-200.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//回転させるやつはいらない

			//オブジェクトの作成
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	コンポーネントの追加
			auto transSnow = pSnow->AddComponent<CTransform>();			//トランスフォーム
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//描画
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//葉っぱの落下
			//	オブジェクトの設定
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//テクスチャの設定
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//ループフラグをオンにする
			drawSnow->SetLoop(true);

			//リストに追加
			ObjectManager::GetInstance()->AddObject(pSnow);
		}

		break;

	default: break;
	}
}


