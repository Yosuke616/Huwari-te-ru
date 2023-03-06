// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_TITLE = 0,		// TITLE
	BGM_STAGESELECT,	// ステージセレクト
	BGM_SPRING,			// 春ステージ
	BGM_SUMMER,			// 夏ステージ
	BGM_FALL,			// 秋ステージ
	BGM_WINTER,			// 冬ステージ
	BGM_ENDING,			// エンディングBGM

	MAX_BGM
};

enum eSE {
	SE_DECIDE = 0,		// 決定
	SE_SELECT,			// セレクト
	SE_ERROR,			// エラー
	SE_GAME_START,		// ゲームスタート
	SE_WIND,			// 風発射
	SE_GAME_CLEAR,		// ゲームクリア
	SE_GAME_OVER,		// ゲームオーバー(仮)
	SE_PAUSE,			// ポーズ
	SE_GET_SEED,		// 種取得
	SE_GET_CLOVER,		// クローバー取得
	SE_ICICLE_FALL,		// つららが落ちる
	SE_ICICLE_DESTROY,	// つららが壊れる
	SE_BIND,			// 蜘蛛の巣にひっかかる
	SE_WEB_DESTROY,		// 蜘蛛の巣を破壊
	SE_FALLENLEAVES,	// 落ち葉を飛ばす
	SE_FINISH,			// エンディングのミニゲーム終了se
	MAX_SE
};
