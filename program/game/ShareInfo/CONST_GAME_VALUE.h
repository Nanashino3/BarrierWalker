#ifndef SHAREINFO_CONST_GAME_VALUE_H_
#define SHAREINFO_CONST_GAME_VALUE_H_

namespace{
// 各種サイズ
const int SIZE_4	= 0x0004;
const int SIZE_8	= (SIZE_4  << 1);
const int SIZE_16	= (SIZE_8  << 1);
const int SIZE_32	= (SIZE_16 << 1);

const int SCREEN_WIDTH = 1024;	// 画面幅
const int SCREEN_HEIGHT = 640;	// 画面高さ

// 画面番号
const int TITLE_SCENE	= 1;	// タイトル画面
const int PAUSE_SCENE	= 2;	// ポーズ画面
const int HELP_SCENE	= 3;	// ヘルプ画面
const int LOADING_SCENE = 4;	// ローディング画面
const int RANKING_SCENE = 5;	// ランキング画面

// プレイヤー関連
const int PLAYER_WIDHT	= 128;							// テクスチャサイズ：幅
const int PLAYER_HEIGHT = 48;							// テクスチャサイズ：高さ
const int PLAYER_X_NUM	= 4;							// 画像数：X方向
const int PLAYER_Y_NUM	= 1;							// 画像数：Y方向
const int PLAYER_RECT_W = PLAYER_WIDHT / PLAYER_X_NUM;	// 矩形幅
const int PLAYER_RECT_H = PLAYER_HEIGHT / PLAYER_Y_NUM;	// 矩形高さ

// プレイヤー動作
const unsigned int IS_MOVING	= (1 << 0);	// 移動
const unsigned int IS_JUMPING	= (1 << 1);	// ジャンプ
const unsigned int IS_DASHING	= (1 << 2);	// ダッシュ
const unsigned int IS_AIR		= (1 << 3);	// 空中

// マップチップ(総数)
const int MAX_MAP_CHIP		= 192;		// 最大マップチップ数
const int MAP_CHIP_X_NUM	= SIZE_8;	// 画像数：X方向
const int MAP_CHIP_Y_NUM	= 24;		// 画像数：Y方向
const int MAP_CHIP_W		= SIZE_16;	// 画像幅
const int MAP_CHIP_H		= SIZE_16;	// 画像高さ

// 最大属性種別
const int ATTRIBUTE_TYPE_RED	= 0;	// 属性：赤
const int ATTRIBUTE_TYPE_GREEN	= 1;	// 属性：緑
const int ATTRIBUTE_TYPE_BLUE	= 2;	// 属性：青
const int MAX_ATTRIBUTE_TYPE	= 3;	// 最大属性数

// バリア関連定数
const float MAX_ENERGY = 100.0f;			// 最大エネルギー
const int BARRIER_TOP	  = 0;				// バリア：上段
const int BARRIER_CURRENT = 1;				// バリア：使用中
const int BARRIER_BOTTOM  = 2;				// バリア：下段
const unsigned int CHANGE_ON	= 0x0001;	// 切替ON
const unsigned int CHANGE_UP	= 0x0002;	// 切替：上方向
const unsigned int CHANGE_DOWN	= 0x0004;	// 切替：下方向

// 勝敗関連定数
const int JUDGE_EVEN = 0;	// 判定：引分
const int JUDGE_WIN	 = 1;	// 判定：勝ち
const int JUDGE_LOSE = 2;	// 判定：負け

// HP関連定数
const int MAX_HIT_POINT = 3;	// 最大ヒットポイント

// 衝突関連定数
const int NO_HIT		= 0;	// ヒットなし
const int CORRECT_LEFT	= 1;	// 左方向
const int CORRECT_RIGHT	= 2;	// 右方向
const int CORRECT_UP	= 3;	// 上方向
const int CORRECT_DOWN	= 4;	// 下方向

// 機能フラグ
const unsigned int FUNCTION_TYPE_BARRIER  = (1 << 0);	// バリア機能

// アイテムフラグ
const unsigned int ITEM_COIN = (1 << 0);	// アイテム：コイン

// データにアクセスするための添字
const unsigned int ACCESS_KEY_COIN		= 0;	// コイン
const unsigned int ACCESS_KEY_DEFEAT	= 1;	// 敵撃破数
const unsigned int MAX_ACCESS_KEY		= 2;	// 最大アクセスキ－

// サウンド関連定数
const unsigned int SE_ID_BARRIER		= 0;
const unsigned int SE_ID_GUARD			= 1;
const unsigned int SE_ID_GAME_CLEAR		= 2;
const unsigned int SE_ID_GAME_OVER		= 3;
const unsigned int SE_ID_GET_COIN		= 4;
const unsigned int SE_ID_JUMP			= 5;
const unsigned int SE_ID_SHOOT			= 7;
const unsigned int SE_ID_MENU_DECIDE	= 8;
const unsigned int SE_ID_MENU_SELECT	= 9;
const unsigned int SE_ID_PAUSE_CLOSE	= 10;
const unsigned int SE_ID_PAUSE_OPEN		= 11;
const unsigned int SE_ID_DAMAGE			= 12;
const unsigned int SE_ID_DEFEAT			= 13;
const unsigned int SE_ID_BARRIER_SELECT = 14;

const unsigned int BGM_ID_TITLE		= 0;
const unsigned int BGM_ID_STAGE_1	= 1;
const unsigned int BGM_ID_STAGE_2	= 2;
const unsigned int BGM_ID_STAGE_3	= 3;
}
#endif // #ifndef SHAREINFO_CONST_GAME_VALUE_H_