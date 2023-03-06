/**
 * @file    TextureManager.h
 * @brief	テクスチャ一元管理プログラム
 * @date	2021/05/26
 * @version	1.0.0
 * @author	AT12A 33 なかがみ
 */

//===== インクルードガード ======
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

//===== インクルード =====
#include <unordered_map>
#include "main.h"

//===== マクロ定義 =====

// 春bg
#define BG_A0_TEX_NUM				(0)
#define PATH_TEX_BG_A0				("data/texture/A0.png")
#define BG_A1_TEX_NUM				(1)
#define PATH_TEX_BG_A1				("data/texture/A1.png")
#define BG_A2_TEX_NUM				(2)
#define PATH_TEX_BG_A2				("data/texture/A2.png")
#define BG_A3_TEX_NUM				(3)
#define PATH_TEX_BG_A3				("data/texture/A3.png")
#define BG_A4_TEX_NUM				(4)
#define PATH_TEX_BG_A4				("data/texture/A4.png")
#define BG_A5_TEX_NUM				(5)
#define PATH_TEX_BG_A5				("data/texture/A5.png")
#define BG_A6_TEX_NUM				(6)
#define PATH_TEX_BG_A6				("data/texture/A6.png")
#define BG_A7_TEX_NUM				(7)
#define PATH_TEX_BG_A7				("data/texture/A7.png")
#define TITLE_SPRING_TEX_NUM		(8)
#define PATH_TEX_TITLE_SPRING		("data/texture/A.png")
// 夏bg
#define BG_B0_TEX_NUM				(10)
#define PATH_TEX_BG_B0				("data/texture/B0.png")
#define BG_B1_TEX_NUM				(11)
#define PATH_TEX_BG_B1				("data/texture/B1.png")
#define BG_B2_TEX_NUM				(12)
#define PATH_TEX_BG_B2				("data/texture/B2.png")
#define BG_B3_TEX_NUM				(13)
#define PATH_TEX_BG_B3				("data/texture/B3.png")
#define BG_B4_TEX_NUM				(14)
#define PATH_TEX_BG_B4				("data/texture/B4.png")
#define BG_B5_TEX_NUM				(15)
#define PATH_TEX_BG_B5				("data/texture/B5.png")
#define BG_B6_TEX_NUM				(16)
#define PATH_TEX_BG_B6				("data/texture/B6.png")
#define BG_B7_TEX_NUM				(17)
#define PATH_TEX_BG_B7				("data/texture/B7.png")
#define TITLE_SUMMER_TEX_NUM		(18)
#define PATH_TEX_TITLE_SUMMER		("data/texture/B.png")
// 秋bg
#define BG_C0_TEX_NUM				(20)
#define PATH_TEX_BG_C0				("data/texture/C0.png")
#define BG_C1_TEX_NUM				(21)
#define PATH_TEX_BG_C1				("data/texture/C1.png")
#define BG_C2_TEX_NUM				(22)
#define PATH_TEX_BG_C2				("data/texture/C2.png")
#define BG_C3_TEX_NUM				(23)
#define PATH_TEX_BG_C3				("data/texture/C3.png")
#define BG_C4_TEX_NUM				(24)
#define PATH_TEX_BG_C4				("data/texture/C4.png")
#define BG_C5_TEX_NUM				(25)
#define PATH_TEX_BG_C5				("data/texture/C5.png")
#define BG_C6_TEX_NUM				(26)
#define PATH_TEX_BG_C6				("data/texture/C6.png")
#define BG_C7_TEX_NUM				(27)
#define PATH_TEX_BG_C7				("data/texture/C7.png")
#define TITLE_AUTUMN_TEX_NUM		(28)
#define PATH_TEX_TITLE_AUTUMN		("data/texture/C.png")
// 冬bg
#define BG_D0_TEX_NUM				(30)
#define PATH_TEX_BG_D0				("data/texture/D0.png")
#define BG_D1_TEX_NUM				(31)
#define PATH_TEX_BG_D1				("data/texture/D1.png")
#define BG_D2_TEX_NUM				(32)
#define PATH_TEX_BG_D2				("data/texture/D2.png")
#define BG_D3_TEX_NUM				(33)
#define PATH_TEX_BG_D3				("data/texture/D3.png")
#define BG_D4_TEX_NUM				(34)
#define PATH_TEX_BG_D4				("data/texture/D4.png")
#define BG_D5_TEX_NUM				(35)
#define PATH_TEX_BG_D5				("data/texture/D5.png")
#define BG_D6_TEX_NUM				(36)
#define PATH_TEX_BG_D6				("data/texture/D6.png")
#define BG_D7_TEX_NUM				(37)
#define PATH_TEX_BG_D7				("data/texture/D7.png")
#define TITLE_WINTER_TEX_NUM		(38)
#define PATH_TEX_TITLE_WINTER		("data/texture/D.png")
// load_silfy
#define LOAD_SILFY_TEX_NUM			(39)
#define PATH_TEX_LOAD_SILFY			("data/texture/loadsilfy.png")
// クリックしてね
#define CLICK_TEX_NUM				(40)
#define PATH_TEX_CLICK				("data/texture/ClickTo.png")
// ビネット
#define VINETTO_TEX_NUM				(41)
#define PATH_TEX_VINETTO			("data/texture/Vinetto.png")
// title logo
#define TITLE_LOGO_TEX_NUM			(42)
#define PATH_TEX_TITLE_LOGO			("data/texture/logo_A.png")
// 風
#define WIND_TEX_NUM				(43)
#define	PATH_TEX_WIND				("data/texture/TestWind01.png"	)	// 風
#define WIND_GAUGE_TEX_NUM			(44)
#define	PATH_TEX_WIND_GAUGE			("data/texture/InGage_sp.png"	)	// wind gauge
#define FRAME_GAUGE_TEX_NUM			(45)
#define	PATH_TEX_FRAME_GAUGE		("data/texture/OutGage_sp.png"	)	// wind gauge frame
// ui
#define SEED_UI_TEX_NUM				(46)
#define PATH_SEED_UI				("data/texture/UI_SeedBase.png"	)	// 種のui
#define SEED_UI_FRAME_TEX_NUM		(47)
#define PATH_SEED_UI_FRAME			("data/texture/UI_SeedTrans.png")	// 種のui	 枠
#define CLOVER_UI_TEX_NUM			(48)
#define PATH_TEX_CLOVER_UI			("data/texture/UI_CroverBase.png")	// クローバーのui
#define CLOVER_UI_FRAME_TEX_NUM		(49)
#define PATH_TEX_CLOVER_UI_FRAME	("data/texture/UI_CroverTrans.png")	// クローバーのui 枠

// 降ってくるもの
#define LEAF_SPRING_NUM				(50)
#define PATH_TEX_SPRING_LEAF		("data/texture/sample_sakura.png")	// 春
#define SUMMER_LEAF_TEX_NUM			(51)
#define PATH_SUMMER_LEAF			("data/texture/test_leaf.png")		// 夏
#define FALL_LEAF_TEX_NUM			(52)
#define PATH_FALL_LEAF				("data/texture/test_ochiba.png")	// 秋
#define SNOW_TEX_NUM				(53)
#define PATH_TEX_SNOW				("data/texture/snow.png")		// 雪

// ギミック
#define SPIDER_TEX_NUM				(54)
#define	PATH_TEX_SPIDER				("data/texture/Spider001.png")	// クモの巣1
#define SPIDER_TWO_TEX_NUM			(55)
#define	PATH_TEX_SPIDER_TWO			("data/texture/Spider002.png")	// クモの巣2
#define SPIDER_THREE_TEX_NUM		(56)
#define	PATH_TEX_SPIDER_THREE		("data/texture/Spider003.png")	// クモの巣3
#define OTIBA_TEX_NUM				(57)
#define	PATH_TEX_OTIBA				("data/texture/otiba_sp.png")	// 落ち葉

//ゲーム停止用ボタン
#define STOP_GAME_TEX_NUM			(58)
#define PATH_TEX_STOP_GAME			("data/texture/stop_game.png")

// 種、クローバー取得時のエフェクト(uiの方
#define EFFECT_CLOVER_TEX_NUM			(59)
#define PATH_TEX_EFFECT_CLOVER			("data/texture/cloverUI.png")
#define EFFECT_SPRING_TEX_NUM			(60)
#define PATH_TEX_EFFECT_SPRING			("data/texture/springUI.png")
#define EFFECT_SUMMER_TEX_NUM			(61)
#define PATH_TEX_EFFECT_SUMMER			("data/texture/summerUI.png")
#define EFFECT_AUTUMN_TEX_NUM			(62)
#define PATH_TEX_EFFECT_AUTUMN			("data/texture/autumnUI.png")
#define EFFECT_WINTER_TEX_NUM			(63)
#define PATH_TEX_EFFECT_WINTER			("data/texture/winterUI.png")

// 音量調整のゲージ
#define CONFIG_GAUGE_TEX_NUM			(64)
#define PATH_TEX_CONFIG_GAUGE			("data/texture/config_gauge.png")		// wind gauge

// 文字
#define GAMEOVER_TEX_NUM				(65)
#define PATH_TEX_GAMEOVER				("data/moji/gameorver.png")			//ゲームオーバー
//枠は奇数番目
#define CONTINUE_TEX_NUM				(66)
#define PATH_TEX_CONTINUE				("data/moji/continue.png")		//続きから
#define CONTINUE_FRAME_TEX_NUM			(67)
#define PATH_TEX_CONTINUE_FRAME			("data/moji/continue_frame.png")//続きから 枠
#define RESUME_TEX_NUM					(68)
#define PATH_TEX_RESUME					("data/moji/restart.png")		//さいかい
#define RESUME_FRAME_TEX_NUM			(69)
#define PATH_TEX_RESUME_FRAME			("data/moji/restart_frame.png")	//さいかい 枠
#define EXIT_TEX_NUM					(70)
#define PATH_TEX_EXIT					("data/moji/exit.png")			//おわり
#define EXIT_FRAME_TEX_NUM				(71)
#define PATH_TEX_EXIT_FRAME				("data/moji/exit_frame.png")	//おわり 枠
#define OPTION_TEX_NUM					(72)
#define PATH_TEX_OPTION					("data/moji/option.png")		//せってい
#define OPTION_FRAME_TEX_NUM			(73)
#define PATH_TEX_OPTION_FRAME			("data/moji/option_frame.png")	//せってい 枠
#define NEW_GAME_TEX_NUM				(74)
#define PATH_TEX_NEW_GAME				("data/moji/new_game.png")		//にゅーげーむ
#define NEW_GAME_FRAME_TEX_NUM			(75)
#define PATH_TEX_NEW_GAME_FRAME			("data/moji/new_game_frame.png")//にゅーげーむ　枠
#define BGM_TEX_NUM						(76)
#define PATH_TEX_BGM					("data/moji/bgm.png")			//BGM
#define BGM_FRAME_TEX_NUM				(77)
#define PATH_TEX_BGM_FRAME				("data/moji/bgm_frame.png")		//BGM　枠
#define SE_TEX_NUM						(78)					
#define PATH_TEX_SE						("data/moji/se.png")			//SE
#define SE_FRAME_TEX_NUM				(79)
#define PATH_TEX_SE_FRAME				("data/moji/se_frame.png")		//SE　枠
#define BACK_TEX_NUM					(80)
#define PATH_TEX_BACK					("data/moji/transition_back.png")			//もどる
#define BACK_FRAME_TEX_NUM				(81)
#define PATH_TEX_BACK_FRAME				("data/moji/transition_back_frame.png")		//もどる　枠
#define SELECT_TEX_NUM					(82)
#define PATH_TEX_SELECT					("data/moji/transition_select.png")			//せれくとへ
#define SELECT_FRAME_TEX_NUM			(83)
#define PATH_TEX_SELECT_FRAME			("data/moji/transition_select_frame.png")	//せれくとへ　枠
#define TITLE_TEX_NUM					(84)
#define PATH_TEX_TITLE					("data/moji/transition_title.png")			//たいとるへ
#define TITLE_FRAME_TEX_NUM				(85)
#define PATH_TEX_TITLE_FRAME			("data/moji/transition_title_frame.png")	//たいとるへ　枠
#define RESTART_TEX_NUM					(86)
#define PATH_TEX_RESTART				("data/moji/true_restart.png")				//やりなおし
#define RESTART_FRAME_TEX_NUM			(87)
#define PATH_TEX_RESTART_FRAME			("data/moji/true_restart_frame.png")		//やりなおし　枠

//ステージナンバー
#define SPRING_1_TEX_NUM				(88)
#define PATH_TEX_SPRING_1				("data/moji/spring_1.png")					//春-１
#define SPRING_2_TEX_NUM				(89)
#define PATH_TEX_SPRING_2				("data/moji/spring_2.png")					//春-２
#define SUMMER_1_TEX_NUM				(90)
#define PATH_TEX_SUMMER_1				("data/moji/summer_1.png")					//夏-１
#define SUMMER_2_TEX_NUM				(91)
#define PATH_TEX_SUMMER_2				("data/moji/summer_2.png")					//夏-２
#define AUTUMN_1_TEX_NUM				(92)
#define PATH_TEX_AUTUMN_1				("data/moji/fall_1.png")					//秋-１
#define AUTUMN_2_TEX_NUM				(93)
#define PATH_TEX_AUTUMN_2				("data/moji/fall_2.png")					//秋-２
#define WINTER_1_TEX_NUM				(94)
#define PATH_TEX_WINTER_1				("data/moji/winter_1.png")					//冬-１
#define WINTER_2_TEX_NUM				(95)
#define PATH_TEX_WINTER_2				("data/moji/winter_2.png")					//冬-２


//----------------------ステージセレクト

// 左上の季節の漢字
#define SPRING_UI_TEX_NUM				(96)
#define PATH_TEX_SPRING_UI				("data/moji/contents_spring.png")	// 春
#define SUMMER_UI_TEX_NUM				(97)
#define PATH_TEX_SUMMER_UI				("data/moji/contents_summer.png")	// 夏
#define AUTUMN_UI_TEX_NUM				(98)
#define PATH_TEX_AUTUMN_UI				("data/moji/contents_autumn.png")	// 秋
#define WINTER_UI_TEX_NUM				(99)
#define PATH_TEX_WINTER_UI				("data/moji/contents_winter.png")	// 冬

// くりあ表示
#define CLEAR_UI_TEX_NUM				(100)
#define PATH_TEX_CLEAR_UI				("data/moji/clear.png")				// くりあ
#define PARFECT_UI_TEX_NUM				(101)
#define PATH_TEX_PARFECT_UI				("data/moji/perfect.png")			// ぱーふぇくと

// 矢印
#define ARROW_UI_TEX_NUM				(102)
#define PATH_TEX_ARROW_UI				("data/texture/sampleAllow001.png")

// セレクト画面の見栄え
#define STAGE_FRAME_TEX_NUM				(103)
#define PATH_TEX_STAGE_FRAME			("data/texture/StageFrame001.png")	// ステージセレクトの枠
#define CLEAR_FLOWER_SPRING_TEX_NUM		(104)
#define PATH_TEX_CLEAR_FLOWER_SPRING	("data/texture/clear_A.png")		// セレクトの花(春
#define CLEAR_FLOWER_SUMMER_TEX_NUM		(105)
#define PATH_TEX_CLEAR_FLOWER_SUMMER	("data/texture/clear_B.png")	// セレクトの花(夏
#define CLEAR_FLOWER_AUTUMN_TEX_NUM		(106)
#define PATH_TEX_CLEAR_FLOWER_AUTUMN	("data/texture/clear_C.png")	// セレクトの花(秋
#define CLEAR_FLOWER_WINTER_TEX_NUM		(107)
#define PATH_TEX_CLEAR_FLOWER_WINTER	("data/texture/clear_D.png")		// セレクトの花(冬


// チュートリアル画像
#define TUTORIAL_UI_TEX_NUM				(108)
#define PATH_TEX_TUTORIAL_UI			("data/texture/UITutorial.png")		// 春
#define TUTORIAL_SPIDER_TEX_NUM			(109)
#define PATH_TEX_TUTORIAL_SPIDER		("data/texture/SpiderTutorial.png")	// 夏
#define TUTORIAL_FALL_TEX_NUM			(110)
#define PATH_TEX_TUTORIAL_FALL			("data/texture/FallTutorial.png")	// 秋
#define TUTORIAL_WINTER_TEX_NUM			(111)
#define PATH_TEX_TUTORIAL_WINTER		("data/texture/SnowTutorial.png")	// 冬

// エンドロール
#define END_ROLL_TEX_NUM				(112)
#define PATH_TEX_END_ROLL				("data/texture/light.png")

//2022/1/26/WindCar Shimzu Yosuke ---------------------------------------------------
//風車
#define WIND_CAR_TEX_NUM					(113)
#define PATH_TEX_WIND_CAR					("data/texture/Windcar.png")
//風車スティック
#define WINDCAR_STICK_TEX_NUM				(114)
#define PATH_TEX_WINDCAR_STICK				("data/texture/Stick.png")
//-----------------------------------------------------------------------------------


// 数字0~9
#define SUUJI_TEX_NUM					(115)
#define PATH_TEX_SUUJI					("data/moji/suuji.png")

// セレクト画面のムービー
#define MV_SPRING_1_NUM					(116)
#define PATH_MV_SPRING_1				("data/mv/MV_Spuring_1.jpg")
#define MV_SPRING_2_NUM					(117)
#define PATH_MV_SPRING_2				("data/mv/MV_Spuring_2.jpg")
#define MV_SUMMER_1_NUM					(118)
#define PATH_MV_SUMMER_1				("data/mv/MV_Summer_1.jpg")
#define MV_SUMMER_2_NUM					(119)
#define PATH_MV_SUMMER_2				("data/mv/MV_Summer_2.jpg")
#define MV_FALL_1_NUM					(120)
#define PATH_MV_FALL_1					("data/mv/MV_Fall_1.jpg")
#define MV_FALL_2_NUM					(121)
#define PATH_MV_FALL_2					("data/mv/MV_Fall_2.jpg")
#define MV_WINTER_1_NUM					(122)
#define PATH_MV_WINTER_1				("data/mv/MV_Winter_1.jpg")
#define MV_WINTER_2_NUM					(123)
#define PATH_MV_WINTER_2				("data/mv/MV_Winter_2.jpg")

//2022/1/31/Load_Tutorial Shimizu Yosuke -------------------------------------
#define PLAYER_TUTORIAL_TEX_NUM			(124)
#define PATH_PLAYER_TUTORIAL			("data/texture/PlayerTutorial.png")
//----------------------------------------------------------------------------

#define THANK_TEX_NUM					(125)
#define PATH_TEX_THANK					("data/moji/thank.png")
#define THANK_FRAME_TEX_NUM				(126)
#define PATH_TEX_THANK_FRAME			("data/moji/thank_frame.png")

//===== クラス定義 =====

/**
 * @class	TextureManager
 * @brief	テクスチャマネージャー
*/
class TextureManager
{
private:
	//=== メンバ変数
	static TextureManager*	m_pInstance;	// インスタンス
	std::unordered_map<int, ID3D11ShaderResourceView*> 	m_TextureList; 	// テクスチャリスト

	//=== メソッド
protected:
	TextureManager();

public:
	
	~TextureManager();
	static TextureManager* GetInstance(void);				// インスタンス化
	static void Uninit(void);								// テクスチャマネージャー解放
	bool AddTexture(const char* pszFileName, int nTexID);	// テクスチャ追加
	ID3D11ShaderResourceView* GetTexture(int nTexID);		// テクスチャ取得
	bool EraseTexture(int nTexID);							// テクスチャ削除
	int GetSize();											//	読み込み数
};
#endif // __TEXTURE_MANAGER_H__
