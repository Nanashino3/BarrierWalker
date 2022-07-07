#include <chrono>
#pragma comment(lib,"winmm.lib")

#include "DxLib.h"
#include "../library/tnl_input.h"
#include "../game/game_main.h"
#include "../game/Sound/CSoundManager.h"

static std::chrono::system_clock::time_point clock_start, clock_end ; 
static std::chrono::system_clock::time_point fps_clock_start, fps_clock_end;


// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// ウィンドウモードで起動
	ChangeWindowMode( true ) ;

	// ウィンドウサイズ設定
	SetGraphMode( 1024, 640, 32 ) ;

	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		
	{
		// エラーが起きたら直ちに終了
		return -1 ;				
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// 計測開始時間
	clock_start = std::chrono::system_clock::now();

	// キー入力制御の初期化
	HWND hWnd = GetMainWindowHandle();
	tnl::Input::Initialize(hInstance,hWnd);

	// メッセージループ
	while( 1 )
	{
		// フレーム間の経過時間
		// マイクロ秒で計測して秒に変換
		clock_end = std::chrono::system_clock::now();
		double micro_seconds = static_cast<double>( std::chrono::duration_cast<std::chrono::microseconds>(clock_end - clock_start).count() );
		float delta_time = static_cast<float>( micro_seconds / 1000.0 / 1000.0 );
		clock_start = clock_end;

		if( ProcessMessage() == -1 ){
			break ;
		}

		fps_clock_start = std::chrono::system_clock::now();

		// 画面をクリア
		ClearDrawScreen() ;

		// キー状態の更新
		tnl::Input::Update();

		// ゲームメインルーチン
		gameMain(delta_time);

		// バックバッファをフリップ
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

		// フレームレートコントロール
		fps_clock_end = std::chrono::system_clock::now();
		double fps_mic = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(fps_clock_end - fps_clock_start).count());
		float fps_mils = static_cast<float>(fps_mic / 1000.0);
		float fps_lim = 1000.0f / 60.0f;

		if (fps_lim > fps_mils) {
			timeBeginPeriod(1);
			Sleep(DWORD(fps_lim - fps_mils));
			timeEndPeriod(1);
		}

	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End() ;			

	// ソフトの終了
	return 0 ;				 
}
