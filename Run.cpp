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
enum { desk, isu}; // 障害物
enum { TITLE, RULE, PLAY, OVER, CLEAR}; // シーン
int bgY = 0;

// プレイヤーの変数１
int PlayerX = WIDTH / 2;
int PlayerY = 600;
int PlayerW = 79;
int PlayerH = 118;


// 障害物の変数

// グローバル変数
int imgPlayer1, imgPlayer2; // プレイヤー画像
int imgFloor, imgTukue, imgIsu, imgOver, imgClear; // 背景画像
int imgPlayer, imgEnemy; // プレイヤーと敵の画像
int bgm, jinOver, jinClear; // 音
int scene = TITLE; // シーン管理
int timer = 0; // 時間管理
bool Flag = 1;


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("Run Game"); // ウィドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの設定
	ChangeWindowMode(TRUE); // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1; // ライブラリの初期化、エラーが起きたら終了
	SetBackgroundColor(0, 0, 0); // 背景色
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

	InitGame();
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	while (1)
	{
		ClearDrawScreen(); // 画面クリア

		// ゲームの根幹
		
		switch (scene)
		{
		case TITLE:
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.3, "Level !", 0xff0000, 80);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press SPACE", 0xffffff, 30);
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				scene = RULE;
			}
			break;

		case RULE:
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.45, "A:左に移動", 0xffffff, 40);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.55, "D:右に移動", 0xffffff, 40);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press S Key to start.", 0xffffff, 30);
			if (CheckHitKey(KEY_INPUT_S))
			{
				scene = PLAY;
			}
			break;

		case PLAY:
			ScrollBG(1);
			// 仮
			if (CheckHitKey(KEY_INPUT_O))
			{
				StopSoundMem(bgm);
				scene = OVER;
			}
			if (CheckHitKey(KEY_INPUT_C))
			{
				StopSoundMem(bgm);
				scene = CLEAR;
			}


			if (Flag == 1)
			{
				MovePlayer();
				PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
				// MoveItem();
			}
			break;

		case OVER:
			DrawExtendGraph(0, 0, 720, 1000, imgEnemy, TRUE);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.5, "GAME OVER", 0xff0000, 80);
			timer++;
			if (timer > FPS * 3)
			{
				scene = TITLE;
			}
			break;

		case CLEAR:
			DrawExtendGraph(0, 0, 720, 1000, imgClear, FALSE);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.5, "あなたは逃げ切った", 0xf0f0f0, 80);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press to R key", 0xffffff, 30);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.8, "Return TITLE", 0xffffff, 30);
			if (CheckHitKey(KEY_INPUT_R))
			{
				scene = TITLE;
			}
			break;
		}





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
	// プレイヤー画像
	imgPlayer1 = LoadGraphWithCheck("image/run1.png");
	imgPlayer2 = LoadGraphWithCheck("image/run2.png");

	// 背景画像
	imgFloor = LoadGraphWithCheck("image/FloorNew.png");
	imgTukue = LoadGraphWithCheck("image/desk.png");
	imgIsu = LoadGraphWithCheck("image/isu.png");
	imgOver = LoadGraphWithCheck("image/back.png");
	imgClear = LoadGraphWithCheck("image/clear.jpg");

	// 背後の敵(画像が見つかり次第コメント解除)
	imgEnemy = LoadGraphWithCheck("image/back.png");
	
	// サウンドの読み込みと音量設定(音を探したらコメント解除)
	 bgm = LoadSoundMemWithCheck("sound/bgmsound.wav");
	// jinOver = LoadSoundMemWithCheck("sound/    .mp3");
	// jinClear = LoadSoundMemWithCheck("sound/    .mp3");
	 ChangeVolumeSoundMem(128, bgm);
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
	DrawExtendGraph(0, 850, 720, 1243, imgEnemy, FALSE);
	DrawGraph(PlayerX, PlayerY, imgPlayer1, TRUE);
	// DrawGraph(PlayerX, PlayerY, imgPlayer2, TRUE);
	DrawExtendGraph(200, 100,350 , 150, imgIsu, TRUE); // 仮
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

void DrawTextC(int x, int y, const char* txt, int col, int siz)
{
	SetFontSize(siz);
	int strWidth = GetDrawStringWidth(txt, strlen(txt));
	x -= strWidth / 2;
	y -= siz / 2;
	DrawString(x + 1, y + 1, txt, 0x000000);
	DrawString(x, y, txt, col);
}

void MovePlayer()
{
	// プレイヤーの処理
	if (CheckHitKey(KEY_INPUT_A) == 1) // 左入力
	{
		PlayerX = PlayerX - 10;
		if (PlayerX < PlayerW / 2 + 130) PlayerX = PlayerW / 2 + 130;
	}
	if (CheckHitKey(KEY_INPUT_D) == 1) // 右入力
	{
		PlayerX = PlayerX + 10;
		if (PlayerX > WIDTH - 160 - PlayerW / 2) PlayerX = WIDTH - 160 - PlayerW / 2;
	}
}

int SetItem(int x, int y, int ptn, int img)
{

}


void MoveItem()
{

}
// 724 * 2172 画像のサイズ