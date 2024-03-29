
//ヘッダーファイル読み込み
#include "game.h"
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//構造体

//画像の構造体
struct IMAGE
{
	int handle = -1;		//画像のハンドル（管理番号）
	char path[255];			//画像の場所（パス）
	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	BOOL IsDraw = FALSE;	//画像が描画できる？
};

//キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;				//画像構造体

	int speed = -1;			//移動速度

	RECT coll;				//当たり判定の領域（四角）
};

//動画の構造体
struct MOVIE
{
	int handle = -1;		//動画のハンドル
	char path[255];			//動画の場所

	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	int Volume = 255;		//ボリューム（最小）0〜255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//ボリューム(MIN 0〜255 MAX)
	int playType = -1;
};

//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームシーン
GAME_SCENE OldGameScene;	//前回のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//プレイ背景の動画
MOVIE playMovie;

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR Goal;

//ゲームオーバー
CHARACTOR ENEMY;

//画像を読み込む
IMAGE TitleLogo;	//タイトルロゴ
IMAGE TitleEnter;	//エンターキーを押してね
IMAGE EndClear;		//クリアロゴ
IMAGE EndOver;		//ゲームオーバーロゴ

//音楽
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;
AUDIO GameOverBGM;

//効果音
AUDIO playerSE;

//画面の切替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

int fadeTimeMill = 2000;						//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードインのカウンタ
int fadeInCntMax = fadeTimeMax;				//フェードインのカウンタMAX

//PushEnterの点滅
int PushEnterCnt = 0;				//カウンタ
const int PushEnterCntMAX = 60;		//カウンタMAX値
BOOL PushEnterBrink = FALSE;		//点滅しているか？

//プロトタイプ宣言
VOID Title(VOID);			//タイトル画面
VOID TitleProc(VOID);		//タイトル画面(処理)
VOID TitleDraw(VOID);		//タイトル画面(描画)

VOID Play(VOID);			//プレイ画面
VOID PlayProc(VOID);		//プレイ画面(処理)
VOID PlayDraw(VOID);		//プレイ画面(描画)

VOID End(VOID);				//エンド画面
VOID EndProc(VOID);			//エンド画面(処理)
VOID EndDraw(VOID);			//エンド画面(描画)

VOID GameOver(VOID);		//ゲームオーバー画面
VOID GameOverProc(VOID);		//ゲームオーバー画面(処理)
VOID GameOverDraw(VOID);		//ゲームオーバー画面(描画)

VOID Change(VOID);			//切り替え画面
VOID ChangeProc(VOID);		//切り替え画面(処理)
VOID ChangeDraw(VOID);		//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);		//シーンを切り替え

VOID CollUpdateplayer(CHARACTOR* chara);		//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);				//当たり判定

BOOL Collision(RECT player, RECT Goal);			//矩形と矩形の衝突判定

BOOL GameLoad(VOID);		//ゲームのデータを読み込み

BOOL LoadImageMem(IMAGE* image, const char* path);							//ゲームの画像を読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//ゲームの音楽を読み込み
VOID GameInit(VOID);		//ゲームのデータを初期化

// プログラムは WinMain から始まります
//Windowsのプログラミング法＝(WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log,txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化
	if (!GameLoad())
	{
		//データの読み込みに失敗したとき
		DxLib_End();	//DxLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ
	while (1)
	{
		//メッセージを受け取り続ける
		if (ProcessMessage() != 0) { break; }		//無限ループを抜ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_GAMEOVER:
			GameOver();			//ゲームオーバー画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;			//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;		//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();


		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//終わるときの処理
	DeleteGraph(playMovie.handle);	//画像をメモリから削除
	DeleteGraph(player.img.handle);		//画像をメモリ上から削除
	DeleteGraph(Goal.img.handle);		//画像をメモリ上から削除
	DeleteGraph(ENEMY.img.handle);		//画像をメモリ上から削除

	DeleteGraph(TitleLogo.handle);		//画像をメモリ上から削除
	DeleteGraph(TitleEnter.handle);		//画像をメモリ上から削除
	DeleteGraph(EndClear.handle);		//画像をメモリ上から削除
	DeleteGraph(EndOver.handle);		//画像をメモリ上から削除

	DeleteSoundMem(TitleBGM.handle);	//音楽をメモリ上から削除
	DeleteSoundMem(PlayBGM.handle);		//音楽をメモリ上から削除
	DeleteSoundMem(EndBGM.handle);		//音楽をメモリ上から削除
	DeleteSoundMem(GameOverBGM.handle);	//音楽をメモリ上から削除

	DeleteSoundMem(playerSE.handle);	//音楽をメモリ上から削除
		
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad() {

	//プレイ動画の背景を読み込み
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	//パスのコピー
	playMovie.handle = LoadGraph(playMovie.path);	//動画を読み込み

	//画像が読み込まれなかったときは、エラー（-1）が入る
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//メインのウィンドウハンドル
			playMovie.path,				//メッセージ本文
			"動画読み込みエラー！",		//メッセージタイトル
			MB_OK						//ボタン
		);

		return FALSE;		//読み込み失敗
	}

	//動画の幅と高さを取得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;

	//画像を読み込み
	if (!LoadImageMem(&player.img, ".\\image\\Player.png")) { return FALSE; }
	if (!LoadImageMem(&Goal.img, ".\\image\\goal.png")) { return FALSE; }
	if (!LoadImageMem(&ENEMY.img, ".\\image\\ENEMY.png")) { return FALSE; }

	//ロゴを読み込む
	if (!LoadImageMem(&TitleLogo, ".\\image\\タイトルロゴ.png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter, ".\\image\\プッシュエンター.png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\image\\ゲームクリア.png")) { return FALSE; }
	if (!LoadImageMem(&EndOver, ".\\image\\ゲームオーバー.png")) { return FALSE; }

	//音楽の読み込み
	if (!LoadAudio(&TitleBGM, ".\\Audio\\TitleBgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\PlayBgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\EndBgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&GameOverBGM, ".\\Audio\\GameOverBgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }

	if (!LoadAudio(&playerSE, ".\\Audio\\PlayerSE.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }

	return TRUE;	//全て読み込めた！
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(image->path, path);	//パスのコピー
	image->handle = LoadGraph(image->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,			//メッセージ本文
			"画像読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//読み込めた
	return TRUE;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);					//パスのコピー
	audio->handle = LoadSoundMem(audio->path);		//音楽の読み込み

	//音楽が読み込めなかったときは、エラー(-1)が入る
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,			//メッセージ本文
			"音楽読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}
	return TRUE;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	player.img.x = 0;		//中央寄せ
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;		//中央寄せ
	player.speed = 500;		//スピード
	player.img.IsDraw = TRUE;	//描画できる！

	//当たり判定を更新する
	CollUpdateplayer(&player);		//プレイヤーの当たり判定のアドレス

	//ゴールを初期化
	Goal.img.x = GAME_WIDTH - Goal.img.width;		//中央寄せ
	Goal.img.y = 0;		//中央寄せ
	Goal.img.IsDraw = TRUE;	//描画できる！

	//当たり判定を更新する
	CollUpdate(&Goal);		//ゴールの当たり判定のアドレス
							
	//敵を初期化
	ENEMY.img.x = GAME_WIDTH - ENEMY.img.width;		//中央寄せ
	ENEMY.img.y = GAME_HEIGHT- ENEMY.img.height;		//中央寄せ
	ENEMY.img.IsDraw = TRUE;	//描画できる！

	//当たり判定を更新する
	CollUpdate(&ENEMY);		//ゲームオーバーの当たり判定のアドレス

	//タイトルロゴの位置を決める
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;	//中央揃え
	TitleLogo.y = 100;

	//PushEnterの位置を決める
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;	//中央揃え
	TitleEnter.y = GAME_HEIGHT - TitleEnter.height - 100;

	//PushEnterの点滅
	PushEnterCnt = 0;
	//PushEnterCntMAX = 60;	//初期化しなくて良い？
	PushEnterBrink = FALSE;

	//クリアロゴの位置を決める
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;	//中央揃え
	EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2;	//中央揃え
														
	//ゲームオーバーの位置を決める
	EndOver.x = GAME_WIDTH / 2 - EndOver.width / 2;	//中央揃え
	EndOver.y = GAME_HEIGHT / 2 - EndOver.height / 2;	//中央揃え
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
		GameScene = scene;	//シーンを切り替え
		IsFadeIn = FALSE;				//フェードインしない
		IsFadeOut = TRUE;				//フェードアウトする
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(TitleBGM.handle);

		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	//タイトルロゴの描画
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	//PushEnterの描画
	//MAX値まで待つ
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタを初期化
	}

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//BGMが流れていないとき
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//プレイヤーがゴールに当たったときは
	if (Collision(player.coll,Goal.coll) == TRUE) {
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		//処理を強制終了
		return;
	}

	//プレイヤーが敵に当たったときは
	if (Collision(player.coll, ENEMY.coll) == TRUE) {
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		//ゲームオーバー画面に切り替え
		ChangeScene(GAME_SCENE_GAMEOVER);

		//処理を強制終了
		return;
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;
		
		//動くときの効果音を追加
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			PlaySoundMem(playerSE.handle, playerSE.playType);
		}
	}

	//当たり判定を更新する
	CollUpdateplayer(&player);

	//ゴールの当たり判定を更新する
	CollUpdate(&Goal);

	//敵の当たり判定を更新する
	CollUpdate(&ENEMY);

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//動画背景を描画

	//もし、動画が再生されていないとき
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);		//動画を再生
	}
	//動画を描画（画像を引き延ばす）
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//プレイヤーを描画
	if (player.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//デバックのときは、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//ゴールを描画
	if (Goal.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);

		//デバックのときは、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//敵を描画
	if (ENEMY.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(ENEMY.img.x, ENEMY.img.y, ENEMY.img.handle, TRUE);

		//デバックのときは、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(ENEMY.coll.left, ENEMY.coll.top, ENEMY.coll.right, ENEMY.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//BGMを止める
		StopSoundMem(EndBGM.handle);

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	//EndClearの描画
	DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// ゲームオーバー画面
/// </summary>
VOID GameOver(VOID)
{
	GameOverProc();	//処理
	GameOverDraw();	//描画

	return;
}

/// <summary>
/// ゲームオーバー画面の処理
/// </summary>
VOID GameOverProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//BGMを止める
		StopSoundMem(GameOverBGM.handle);

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(GameOverBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(GameOverBGM.handle, GameOverBGM.playType);
	}

	return;
}

/// <summary>
/// ゲームオーバー画面の描画
/// </summary>
VOID GameOverDraw(VOID)
{
	//EndOverの描画
	DrawGraph(EndOver.x, EndOver.y, EndOver.handle, TRUE);

	DrawString(0, 0, "ゲームオーバー画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//処理
	ChangeDraw();	//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;		//カウンタ初期化
			IsFadeIn = FALSE;				//フェードイン処理終了
		}
	}
	
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;		//カウンタ初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();			//エンド画面の描画
		break;
	case GAME_SCENE_GAMEOVER:
		GameOverDraw();			//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="player">矩形A</param>
/// <param name="Goal">矩形B</param>
/// <returns>当たったらTRUE/当たらないならFALSE</returns>
BOOL Collision(RECT player,RECT Goal) {
	if (player.left < Goal.right
		&&player.right > Goal.left
		&&player.top < Goal.bottom
		&&player.bottom > Goal.top) 
	{
			return TRUE;
	}
	else
	{
		return FALSE;
	}
}