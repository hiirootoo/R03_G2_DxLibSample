//ヘッダーファイルの読み込み
#include "DxLib.h"//DxLibを使うときは必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"		//ゲームタイトル
#define GAME_WIDTH 1280					//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT 720					//ゲーム画面の高さ(ハイト)
#define GAME_COLOR 32					//ゲームの色域

#define GAME_ICON_ID 333				//ゲームのICONのID

#define GAME_WINDOW_BAR 0				//ウィンドウバーの種類

// プログラムは WinMain から始まります
//Windowsのプログラミング法＝(WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log,txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetWindowStyleMode(GAME_WINDOW_BAR);								//ウィンドウバーの状態
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetAlwaysRunFlag(TRUE);


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//四角の位置を決める
	int x = 0;
	int y = 0;
	/*int x = GAME_WIDTH / 2;*/
	/*int y = GAME_HEIGHT / 2;*/
	
	//四角の大きさを決める
	int width = 32;		//幅
	int height = 32;	//高さ

	int AL = 0;
	int BL = 0;

	//速度
	int xSpeed = 1;
	int ySpeed = 1;

	//円の半径を決める
	int radius = 100;

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1)
	{
		//何かしらキーが押されたとき
		if (CheckHitKeyAll() != 0)
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられたとき
		{
			break;	//無限ループを抜ける
		}

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描画
		DrawBox(
			x, y, width, height,
			GetColor(255, 0, 0),
			TRUE
		);

		//円を描画
		DrawCircle(
			x, y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		if (AL == 0) {
			x++; width++;
		}
		else if (AL == 1) {
			x-- + width--;
		}
		if (BL == 0) {
		y++; height++;
		}
		else if (BL == 1) {
			y--; height--;
		}

		if (x < 0)
			AL = 0;
		else if (x + 32 > GAME_WIDTH)
			AL = 1;

		if (y < 0)
			BL = 0;
		else if (y + 32 > GAME_HEIGHT)
			BL = 1;

		/*x += xSpeed;
		y += ySpeed;
		if (x<0 || x + width>GAME_WIDTH)
			xSpeed = -xSpeed;
		if (y<0 || y + height>GAME_HEIGHT)
			ySpeed - ySpeed;*/

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

		
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}