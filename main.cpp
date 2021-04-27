//ヘッダーファイルの読み込み
#include "DxLib.h"//DxLibを使うときは必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"		//ゲームタイトル
#define GAME_WIDTH 1280					//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT 720					//ゲーム画面の高さ(ハイト)
#define GAME_COLOR 32					//ゲームの色域

#define GAME_ICON_ID 333					//ゲームのICONのID

// プログラムは WinMain から始まります
//Windowsのプログラミング法＝(WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//無限ループ
	while (1) 
	{
		//何かしらキーが押されたとき
		if (CheckHitKeyAll()!=0)
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられたとき
		{
			break;	//無限ループを抜ける
		}
	}

	DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	WaitKey();				// キー入力待ち

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}