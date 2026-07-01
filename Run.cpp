#include "DxLib.h"
#include <stdlib.h>
#include "RunGame.h"

// 定数の定義
const int WIDTH = 720, HEIGHT = 1000; // ウィンドウの幅と高さ
const int FloorW = 720, FloorH = 2170;
const int FPS = 60;
const int ITEM_MAX = 0; // 出現する障害物の数
const int STAGE_DISTANCE = FPS * 60; // ステージの長さ
const int PLAYER_SPEED = 10; // プレイヤーの速度
enum { TITLE, PLAY, OVER, CLEAR};
int bgY = 0;


// グローバル変数
int imgFloor, imgWallL, imgWallR; // 背景画像
int imgPlayer, imgEnemy; // プレイヤーと敵の画像
int bgm, jinOver, jinClear; // 音
int scene = TITLE; // シーン管理
int timer = 0; // 時間管理

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("Run Game"); // ウィドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの設定
	ChangeWindowMode(TRUE); // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1; // ライブラリの初期化、エラーが起きたら終了
	SetBackgroundColor(0, 0, 0); // 背景色
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

	InitGame();


	while (1)
	{
		ClearDrawScreen(); // 画面クリア

		// ゲームの根幹
		

		ScrollBG(1);



		ScreenFlip();
		WaitTimer(1000 / FPS);
		if (ProcessMessage() == -1) break; // エラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキー入力で終了
	}

	DxLib_End(); // DxLibの終了
	return 0; // ソフト終了
}

// 自作の関数

void InitGame(void)
{
	// 背景画像
	imgFloor = LoadGraphWithCheck("image/floor.png");

	// 背後の敵(画像が見つかり次第コメント解除)
	// imgEnemy = LoadGraphWithCheck("image/enemy.png");
	
	// サウンドの読み込みと音量設定(音を探したらコメント解除)
	// bgm = LoadSoundMemWithCheck("sound/    .mp3");
	// jinOver = LoadSoundMemWithCheck("sound/    .mp3");
	// jinClear = LoadSoundMemWithCheck("sound/    .mp3");
	// ChangeVolumeSoundMem(128, bgm);
	// ChangeVolumeSoundMem(128, jinOver);
	// ChangeVolumeSoundMem(128, jinClear);
}

// 背景のスクロール
void ScrollBG(int spd)
{
	bgY += 10;
	if (bgY >= FloorH) bgY = bgY - FloorH;
	DrawGraph(0, bgY - FloorH, imgFloor, FALSE);
	DrawGraph(0, bgY, imgFloor, FALSE);
}



void DrawTextC(int x, int y, const char* txt, int col, int siz)
{
	SetFontSize(siz);
}

// 画像読み込み失敗通知
int LoadGraphWithCheck(const char* file)
{
	int res = LoadGraph(file);
	if (res == -1)
	{
		MessageBox(GetMainWindowHandle(), file, "画像読み込みに失敗", MB_OK | MB_ICONSTOP);
	}
	return res;
}

// 音声読み込み失敗通知
int LoadSoundMemWithCheck(const char* file)
{
	int res = LoadSoundMem(file);
	if (res == -1)
	{
		MessageBox(GetMainWindowHandle(), file, "音声の読み込みに失敗", MB_OK | MB_LEN_MAX);
		return res;
	}
}

// 724 * 2172 画像のサイズ