//===== インクルードガード =====
#ifndef __OBJECT_INFO_H__
#define __OBJECT_INFO_H__

// 季節テクスチャ
//#define	SCENE_CHANGE

#define WIMD_GAUGE_MAX		(250)		// 使用できる風の量
#define WIND_CREATE_DIFF	(40)		// 風を生成するために必要なマウスの移動量

#define WIND_POWER_X		(0.05f)
#define WIND_POWER_Y		(0.05f)

#define WINDENDING_POWER_X		(0.1f)
#define WINDENDING_POWER_Y		(0.1f)

//===== 列挙体 ======

//	オブジェクトの更新の順番
enum UPDATE_ORDER
{
	UPDATE_BG,			//	背景
	UPDATE_PLAYER,		//	プレイヤー
	UPDATE_FIELD,		//	地形
	UPDATE_EFFECT,		//	エフェクト
	UPDATE_MODEL,		//	モデル
	UPDATE_DEBUG,		//	debug
	UPDATE_UI,			//	UI
	UPDATE_OPTION,		//	オプションオブジェクト
	UPDATE_GOLETTER,

	MAX_UPDATE_ORDER
};

//	オブジェクトの描画の順番
enum DRAW_ORDER
{
	DRAW_BG,		//	背景
	DRAW_FIELD,		//	地形
	DRAW_MODEL,		//	プレイヤー
	DRAW_PLAYER,	//	プレイヤー
	DRAW_EFFECT,	//	エフェクト

	DRAW_DEBUG,		//	debug
	DRAW_UI_FRAME,	//	UI枠
	DRAW_GOLETTER,
	DRAW_UI,		//	UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//	オプションオブジェクト
	DRAW_OPTION,	//	オプションオブジェクト

	MAX_DRAW_ORDER
};

//@@@@@ シルフィ------------------------------------------------
#define PLAYER_NAME				("shilfy")
#define PLAYER_POS_X			(-200.0f)				//posX
#define PLAYER_POS_Y			(0.0f)					//posY
#define PLAYER_POS_Z			(0.0f)					//PosZ
#define PLAYER_OFFSET_X			(0.0f)					//Center offset
#define PLAYER_OFFSET_Y			(15.0f)					//Center offset
#define PLAYER_SIZE_X			(20.0f)
#define PLAYER_SIZE_Y			(30.0f)

	/***** アニメーション ******/

// プレイヤーのFBXの数
enum PLAYER_FBX
{
	FBX_ONE = 0,	// 一つ目(待機、風に吹かれている、落ちている、ゲットしたと
	FBX_TWO,		// 二つ目(ゴールした時
};

// 1つ目のfbx
enum ANIM_ID
{
	PLAYER_IDLE = 0,
	PLAYER_CARRIED,
	PLAYER_FALL,
	PLAYER_GET,
	PLAYER_BIND,
	PLAYER_GAMEOVER,

	MAX_ANIMATION
};
// 2つ目のfbx
#define PLAYER_GOAL	(0)

// アニメーション時間
#define PLAYER_ANIM_IDLE_S		(0)		// 待機Start
#define PLAYER_ANIM_IDLE_E		(7.95)	// 待機End

#define PLAYER_ANIM_CARRIED_S	(8.34)	// 風に運ばれているStart
#define PLAYER_ANIM_CARRIED_E	(9.03)	// 風に運ばれているEnd

#define PLAYER_ANIM_FALL_S		(9.3)	// 落ちているStart
#define PLAYER_ANIM_FALL_E		(11.3)	// 落ちているEnd

#define PLAYER_ANIM_GET_S		(11.67)	// 種、クローバー取得時Start
#define PLAYER_ANIM_GET_E		(12)	// 種、クローバー取得時End

#define PLAYER_ANIM_BIND_S		(12.1)	// クモの巣に引っかかるStart
#define PLAYER_ANIM_BIND_E		(12.31)	// クモの巣に引っかかるEnd

#define PLAYER_ANIM_GAMEOVER_S	(12.336)// ゲームオーバーStart
#define PLAYER_ANIM_GAMEOVER_E	(16.2)	// ゲームオーバーEnd


#define PLAYER_ANIM_GOAL_S		(0)	// 種、クローバー取得時Start
#define PLAYER_ANIM_GOAL_E		(4)	// 種、クローバー取得時End

//-----------------------------------------------------------

//@@@@@ サウンド関連
#define BGM_NAME				("config_bgm")
#define SE_NAME					("config_se")
#define BACK_NAME				("back")
#define BGM_POS_Y			(0.0f)
#define SE_POS_Y			(-150.0f)
#define STICK_WIDTH				(400.0f)
#define STICK_HEIGHT			(180.0f)
#define CONFIG_STICK_WIDTH		(15.0f)
#define CONFIG_STICK_HEIGHT		(50.0f)
#define GAUGE_LEFT				(0.0f - (STICK_WIDTH / 2))



//@@@@@ 風
#define WIND_NAME				("Wind")
#define WIND_WIDTH				(42.0f)
#define WIND_HEIGHT				(34.0f)
#define WIND_SPLIT_X			(4)
#define WIND_SPLIT_Y			(4)
#define SWAP_FRAME				(2)

//@@@@@ 風ゲージ
#define WIND_GAUGE_WIDTH		(500.0f)
#define WIND_GAUGE_HEIGHT		(500.0f)
#define WIND_GAUGE_POS_X		(-(SCREEN_WIDTH - WIND_GAUGE_WIDTH) / 2)
#define WIND_GAUGE_POS_Y		((SCREEN_HEIGHT - WIND_GAUGE_HEIGHT) / 2 + 150.0f)

//@@@@@ ブロック
#define BLOCK_NAME				("Block")
#define MAPCHIP_WIDTH			(30.0f)			// マップチップの横幅
#define MAPCHIP_HEIGHT			(25.0f)			// マップチップの縦幅
#define BLOCK_OFFSET_X			(0.0f)			//Center offset
#define BLOCK_OFFSET_Y			(-8.0f)			//Center offset
#define BLOCK_OFFSET_Z			(10.0f)			//Center offset
#define BLOCK_SCALE_X			(2.35f)			//scale
#define BLOCK_SCALE_Y			(1.94f)			//scale
#define BLOCK_SCALE_Z			(6.5f)			//scale

//@@@@@ 種
#define SEED_NAME				("Seed")
#define SEED_SIZE_X				(50.0f)
#define SEED_SIZE_Y				(50.0f)
#define SEED_POS_OFFSET_X		(0.0f)
#define SEED_POS_OFFSET_Y		(10.0f)	
#define SEED_SCALE_X			(5.0f)
#define SEED_SCALE_Y			(5.0f)
#define SEED_SCALE_Z			(5.0f)

//@@@@@ つぼみ
#define BUD_NAME				("FlowerBud")
#define BUD_POS_OFFSET_X		(0.0f)
#define BUD_POS_OFFSET_Y		(-20.0f)
#define BUD_POS_OFFSET_Z		(10.0f)
#define BUD_COLLISION_X			(120.0f)
#define BUD_COLLISION_Y			(120.0f)
#define BUD_ANIM_1_START		(0.0)
#define BUD_ANIM_1_END			(1.0)
#define BUD_ANIM_2_START		(1.0)
#define BUD_ANIM_2_END			(1.6)

//@@@@@ 植木鉢(ゴール
#define POT_NAME				("PlantPot")
#define POT_POS_Z				(22.0f)			// 奥に置かないと変
#define POT_POS_OFFSET_Y		(-20.0f)		// ステージ配列の座標とのずれ
#define POT_SCALE_X				(15.0f)	
#define POT_SCALE_Y				(10.0f)
#define POT_SCALE_Z				(10.0f)

//@@@@@ 花
#define FLOAER_ROTATE_X			(-10.0f)
//#define FLOAER_ROTATE_Y		(6.0f)
#define FLOAER_POS_X_OFFSET		(0)
#define FLOAER_POS_Y_OFFSET		(0)
#define FLOAER_POS_Z_OFFSET		(8.0f)
#define FLOAER_SCALE_X			(6.0f)
#define FLOAER_SCALE_Y			(6.0f)
#define FLOAER_SCALE_Z			(6.0f)
#define FLOAER_ANIM_S			(0.0)
#define FLOAER_ANIM_E			(4.8)

//@@@@@ 背景
#define BG_NAME					("BG")
#define BG_POS_Y				(100.0f)
#define BG_LENGTH				(SCREEN_HEIGHT)

//@@@@@	カーソル
#define CURSOR_NAME				("menuobject_cursor")
#define GAME_CURSOR_NAME		("gameobject_cursor")
#define CURSOR_WIDTH			(30.0f)
#define CURSOR_HEIGHT			(30.0f)
#define CURSOR_WIDTH_COL		(30.0f)
#define CURSOR_HEIGHT_COL		(30.0f)

//@@@@@ 四角い氷
#define BLOCK_MOVE
#define ICE_BLOCK_NAME					("ice_block")
#define ICE_BLOCK_POS_Y_OFFSET			(-6.0f)
#define ICE_BLOCK_POS_Z_OFFSET			(-6.0f)
#define ICE_BLOCK_COLLISION_WIDTH		(30.0f)			// マップチップの横幅
#define ICE_BLOCK_COLLISION_HEIGHT		(28.0f)			// マップチップの縦幅
#define ICE_BLOCK_SCALE_X				(6.9f)
#define ICE_BLOCK_SCALE_Y				(6.0f)
#define ICE_BLOCK_SCALE_Z				(2.0f)
#define MAX_ICE_VELOCITY				(1.0f)	// 氷ブロックの速度




#define CLOVER_NUMBER_NAME	("clover_num")



//2021/12/21 Shimizu Yosuke----------------------------
//@@@@@ クローバー
#define CLOVER_NAME				("Clover")
#define CLOVER_POS_Z			(5.0f)
#define CLOVER_OFFSET_Y			(-8.0f)		// 2022/01/22 登場時、地面に接しているようにする
#define CLOVER_ROTATE_X			(-70.0f)
#define CLOVER_ROTATE_Y			(0.0f)
#define CLOVER_ROTATE_Z			(-45.0f)
#define CLOVER_SCALE_X			(3.0f)
#define CLOVER_SCALE_Y			(3.0f)
#define CLOVER_SCALE_Z			(3.0f)
#define MAX_CLOVER_VELOCITY		(1.5f)

//@@@@@ クローバーUI
#define CLOVER_UI_NAME		("Clover_UI")
#define CLOVER_UI_POS_X		(-550.0f)
#define CLOVER_UI_POS_Y		(-50.0f)
#define CLOVER_UI_SIZE_X	(45.0f)
#define CLOVER_UI_SIZE_Y	(45.0f)
#define CLOVER_POS_CHENGE	(40.0f)
//2022/1/30/Clover_UI ---------------------------
#define CLOVER_UI_GAP_X		(1.5f)
#define CLOVER_UI_GAP_Y		(3.0f)
//-----------------------------------------------

//クローバーUIを回転させるやつ
#define CLOVER_ROTATE_BASE  	(0.0f)
#define CLOVER_ROTATE		(90.0f)

//@@@@@ 種のUI
#define	SEED_UI_NAME		("ssed_ui")
#define	SEED_UI_POS_X		(-540.0f)
#define	SEED_UI_POS_Y		(100.0f)
#define	SEED_UI_SIZE_X		(125.0f)
#define	SEED_UI_SIZE_Y		(125.0f)

//@@@@@ 雪
#define SNOW_NAME			("snow")

//@@@@@ ポーズメニュー
#define STOP_GAME_NAME				("stop_game")
#define STOP_GAME_SIZE_X			(100.0f)
#define STOP_GAME_SIZE_Y			(100.0f)
#define STOP_GAME_POS_X				(-565.0f)
#define STOP_GAME_POS_Y				(-300.0f)
#define STOP_GAME_COLLSION_WIDTH	(100.0f)
#define STOP_GAME_COLLSION_HEIGHT	(100.0f)

//@@@@@ チュートリアル
#define TUTORIAL_NAME				("tutorial")
#define TUTORIAL_COLL_SIZE_HEIGHT	(50.0f)
#define TUTORIAL_COLL_SIZE_WIDTH	(50.0f)
//#define TUTORIAL_SIZE_Y				(50.0f)
//#define TUTORIAL_SIZE_X				(50.0f)

//チュートリアル
//サイズを少し小さくする
//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------
#define TUTORIAL_SIZE_X				(SCREEN_WIDTH - 160.0f)
#define TUTORIAL_SIZE_Y				(SCREEN_HEIGHT - 160.0f)
//-----------------------------------------------------

//上記のチュートリアルの下に追加
//2022/1/26/WindCar Shimizu Yosuke --------------------
//風車(チュートリアル)
#define WINDCAR_NAME				("windcar")
#define WINDCAR_POS_X				(-450.0f)
#define WINDCAR_POS_Y				(-285.0f)
#define WINDCAR_SIZE_X				(100.0f)
#define WINDCAR_SIZE_Y				(100.0f)
#define WINDCAR_COLL_WIDTH			(50.0f)
#define WINDCAR_COLL_HEIGHT			(50.0f)

//風車の棒
#define WINDCAR_STICK_NAME			("stick")
#define WINDCAR_STICK_POS_X			(-450.0f)
#define WINDCAR_STICK_POS_Y			(-300.0f)
#define WINDCAR_STICK_SIZE_X		(100.0f)
#define WINDCAR_STICK_SIZE_Y		(100.0f)

//-----------------------------------------------------


//2022/1/30/Touch_Tutorial Shimizu Yosuke -------------
#define CLICK_TEXT_NAME				("clicl_text")
#define CLICK_TEXT_SIZE_X			(500.0f)
#define CLICK_TEXT_SIZE_Y			(250.0f)
#define CLICK_TEXT_POS_X			(0.0f)
#define CLICK_TEXT_POS_Y			(-300.0f)

//2022/1/31/Load_Tutorial Shimizu Yosuke --------------------------------------
#define LOAD_TUTORIAL_NAME			("tutorial_name")
//-----------------------------------------------------------------------------
//-----------------------------------------------------


//-----------------------------------------------------


// naito-----------------------------------------------
//@@@@@	つらら
#define ICICLE_UP_NAME					("Icicle")
#define ICICLE_DOWN_NAME				("Down")	// つららの当たり判定
#define ICICLE_SCALE_X					(7.0f)		//scale
#define ICICLE_SCALE_Y					(7.0f)		//scale
#define ICICLE_SCALE_Z					(7.0f)		//scale
#define ICICLE_SIZE_X					(40.0f)		// つらら全体の大きさ
#define ICICLE_SIZE_Y					(100.0f)	// つらら全体の大きさ
#define ICICLE_UP_COLLISION_OFFSET_X	(0.0f)		// 上のあたり判定の大きさ
#define ICICLE_UP_COLLISION_OFFSET_Y	(30.0f)		// 上のあたり判定の大きさ
#define ICICLE_UP_COLLISION_SIZE_X		(40.0f)		// 下のあたり判定の大きさ	
#define ICICLE_UP_COLLISION_SIZE_Y		(40.0f)		// 下のあたり判定の大きさ

//@@@@@ ひらひら
#define	LEAF_NAME			("leaf")

//@@@@@ ゲームオーバー文字
#define GOLETTER_NAME		("goletter")		
#define GOLETTER_POS_X		(0.0f)			//文字の位置
#define GOLETTER_POS_Y		(400.0f)		//文字の位置
#define GOLETTER_SIZE_X		(600.0f)		//文字のサイズ
#define GOLETTER_SIZE_Y		(300.0f)		//文字のサイズ

//@@@@@ エンディングのクローバー
#define ENDING_CLOVER_NAME	("ending_clover")
//----------------------------------------------------


// sawada---------------------------------------------
//@@@@@	メニュー一覧
#define TITLE_NAME				("title")
#define SELECT_NAME				("select")
#define CONTINUE_NAME			("continue")
#define RESTART_NAME			("restart")	
#define NEW_GAME_NAME			("new_game")
#define OPTION_NAME				("option")
#define EXIT_GAME_NAME			("exit_game")
//#define MENU_NAME				("Menu")
#define MENU_COLLISION_WIDTH	(450.0f)
#define MENU_COLLISION_HEIGHT	(100.0f)
#define MENU_WIDTH				(450.0f)
#define MENU_HEIGHT				(250.0f)
#define MENU_POS_X				(  0.0f)
#define MENU_POS_Y				(-10.0f)
#define TITLEMENU_POS_X			(300.0f)
#define TITLEMENU_POS_Y			(-50.0f)
// --------------------------------------------------


// simisho-------------------------------------------
//@@@@@ 季節ロゴ
#define SEASON_LOGO_X		(0.0f)
#define SEASON_LOGO_Y		(-50.0f)
#define SEASON_LOGO_WIDTH	(512.0f)
#define SEASON_LOGO_HEIGHT	(288.0f)
// --------------------------------------------------


// tomoya--------------------------------------------
//@@@@@ セレクトメニュー
#define RIGHT_ARROW_NAME	("RightArrow")
#define LEFT_ARROW_NAME		("LeftArrow")
#define STAGE1_NAME			("StageMap1")
#define STAGE2_NAME			("StageMap2")

//@@@@@ クモの巣
#define SPIDER_WIDTH		(80.0f)
#define SPIDER_HEIGHT		(80.0f)
#define SPIDER_OFFESET_Y	(10.0f)
#define SPIDER_BIND_WIDTH	(35.0f)
#define SPIDER_BIND_HEIGHT	(35.0f)
#define SPIDER_ROT_Y		(30.0f)
#define SPIDER_ROT_Y		(30.0f)
// --------------------------------------------------

// ryo-ga--------------------------------------------
#define OTIBA_NAME				("fallen_leaves")
#define OTIBA_POS_Z				(-7.0f)
#define OTIBA_WIDTH				(40.0f)
#define OTIBA_HEIGHT			(30.0f)
#define OTIBA_OFFESET_Y			(-8.0f)
#define OTIBA_COLLISION_WIDTH	(20.0f)
#define OTIBA_COLLISION_HEIGHT	(20.0f)
#define OTIBA_SPLIT_X			(4)
#define OTIBA_SPLIT_Y			(4)
#define OTIBA_ANIM_SPEED		(2)
// --------------------------------------------------
#endif // !__OBJECT_INFO_H__
