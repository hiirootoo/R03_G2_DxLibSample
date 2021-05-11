//キーボードのソースファイル

//ヘッダファイル読み込み
#include "keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数

/// <summary>
/// キーの入力状態を更新する
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_KIND_MAX; i++) {
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];
	}

	//すべてのキーの入力状態を得る
	GetHitKeyStateAll(keyboard.TempKeyState);

	for (int i = 0; i < KEY_KIND_MAX; i++) {
		//キーコードを押しているとき
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;		//押している時間を加算
		}
		else
		{
			keyboard.AllKeyState[i] = 0;	//押している時間を0に戻す
		}
	}

	return;
}

/// <summary>
/// キーを押しているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>押していたらTRUE</returns>
BOOL KeyDown(int KEY_INPUT)
{
	if (keyboard.AllKeyState[KEY_INPUT] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーを上げているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>上げていたらTRUE</returns>
BOOL KeyUp(int KEY_INPUT)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT] != 0			//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT] == 0			//現在は押していない
		) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーをクリックしたか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL KeyClick(int KEY_INPUT)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT] != 0			//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT] == 0			//現在は押していない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーを押し続けているか、キーコードで判断する 
/// </summary>
/// <param name="MillTime">キーコード</param>
/// <returns>キーを押し続けているミリ秒</returns>
BOOL KeyDownKeep(int KEY_INPUT, int MilliTime)
{
	//１秒は１０００ミリ秒
	float MilliSec = 1000.0f;

	//押し続けている時間は、ミリ秒数ｘＦＰＳ値
	//例）１５００ミリ秒押す/１０００ミリ→１．５秒 x ６０ＦＰＳ＝９０
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT] > UpdateTime)
	{
		return TRUE;		//押し続けている
	}
	else
	{
		return FALSE;		//押し続けていない
	}
}