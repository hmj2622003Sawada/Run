#include "DxLib.h"
#include <stdlib.h>
#include "RunGame.h"


// 定数の定義
const int WIDTH = 720, HEIGHT = 1000; // 画面サイズ
const int FloorW = 720, FloorH = 2170; // 背景画像の縦横の高さ
const int FPS = 60; // フレームレート
const int IMG_ITEM_MAX = 2; /// 障害物の数
const int ITEM_MAX = 50; // 障害物の最大数
int stage_rand; // ステージの距離を決める乱数
int stage_distance; // ステージの距離
enum { ITEM_01, ITEM_02 }; // アイテムの種類
enum { TITLE, RULE, PLAY, OVER, CLEAR };
int Timer = 0; // 画像の切り替え用
// グローバル変数
// 配列
int distance = 0; // ゴールまでの距離
int imgFloor, imgEnemy, imgOver, imgClear, imgGoal; // 背景画像
int imgPlayer; // プレイヤーと敵の画像
int imgItem[IMG_ITEM_MAX]; // 障害物の画像
int bgm, jinOver, jinClear; // 音
int scene = TITLE; // シーン管理
int timer = 0; // 時間管理

struct OBJECT Player; // 自機の構造体
struct OBJECT Item[ITEM_MAX]; // 障害物用の構造体

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("Run Game"); // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラー指定
	ChangeWindowMode(TRUE); // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1; // ライブラリの初期化、エラーが起きたら終了
	SetBackgroundColor(0, 0, 0); // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK); //　描画面を裏画面に

	InitGame(); // 初期化用の関数
	InitVariable(); // ゲーム完成時に位置を変える
	RandStage(); // 距離の決定
	int distance = stage_distance;

	while (1) // メインループ
	{
		ClearDrawScreen(); // 画面をクリア

		// ゲームの根幹
		int spd = 1; // スクロールの速さ
		timer++; // タイマー、カウント
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
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.45, "A:右に移動", 0xffffff, 40);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.55, "D:右に移動", 0xffffff, 40);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press S Key to start", 0xffffff, 30);
			if (CheckHitKey(KEY_INPUT_S))
			{
				scene = PLAY;
				InitVariable();
			}
			break;

		case PLAY:
			ScrollBG(spd); // 仮背景スクロール
			MovePlayer(); // 自機の操作
			MoveItem(); // 障害物の操作
			DrawEnemy(); // 背後の化け物の表示
			if (distance == stage_distance)
			{
				PlaySoundMem(bgm, DX_PLAYTYPE_LOOP); // BGMループ再生
			}
			if (distance > 0) distance--;
			if (distance % 60 == 1) // 仮障害物
			{
				int x = 100 + rand() % (WIDTH - 200);
				int y = -30;
				int e = rand() % 2;
				if (e == ITEM_01) SetItem(x, y, 0, 10, ITEM_01, imgItem[0]);
				if (e == ITEM_02) SetItem(x, y, 0, 10, ITEM_02, imgItem[1]);
			}
			// クリアシーン遷移
			if (distance == 0)
			{
				scene = CLEAR;

			}
			// ゲームオーバーシーン遷移

			if (Player.y >= 720)
			{
				timer = 0;
				scene = OVER;
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
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.3, "GAME CLEAR", 0xf0f0f0, 80);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.5, "あなたは逃げ切った", 0xf0f0f0, 80);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.7, "Press to R key", 0xffffff, 30);
			DrawTextC(WIDTH * 0.5, HEIGHT * 0.8, "Return TITLE", 0xffffff, 30);
			if (CheckHitKey(KEY_INPUT_R))
			{
				scene = TITLE;
			}
			break;


		}





		ScreenFlip(); // 裏画面の内容を表画面に反映
		WaitTimer(1000 / FPS); // 一定時間待機
		if (ProcessMessage() == -1) break; // エラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
	}

	DxLib_End(); // DXライブラリの使用終了
	return 0; // 終了
}


// ここから下に自作関数を記述

// 初期化用の関数
void InitGame(void)
{
	// 背景画像の読み込み
	imgFloor = LoadGraphWithCheck("image/FloorNew.png");
	imgEnemy = LoadGraphWithCheck("image/back.png");
	imgClear = LoadGraphWithCheck("image/clear.jpg");
	imgOver = LoadGraphWithCheck("image/back.png");
	// プレイヤーの画像
	imgPlayer = LoadGraphWithCheck("image/run1.png");
	// 障害物の読み込み
	imgItem[0] = LoadGraphWithCheck("image/item1.png");
	imgItem[1] = LoadGraphWithCheck("image/item2.png");

	// サウンドの読み込みと音量設定
	bgm = LoadSoundMemWithCheck("sound/bgmsound.wav");
	ChangeVolumeSoundMem(128, bgm);
}

// 背景のスクロール
void ScrollBG(int spd)
{
	static int FloorY; // スクロール位置を管理する変数
	FloorY = (FloorY + spd * 10) % FloorH;
	for (int i = -1; i < 3; i++) DrawGraph(0, FloorY + i * FloorH, imgFloor, FALSE);
}

void DrawEnemy(void)
{
	DrawExtendGraph(0, 850, 720, 1243, imgEnemy, FALSE);
}
// ゲーム開始時に初期値を代入する
void InitVariable(void)
{
	Player.x = WIDTH / 2;
	Player.y = 600;
	Player.vx = 10;
	Player.vy = 0;
	GetGraphSize(imgPlayer, &Player.wid, &Player.hei); // プレイヤーの画像の幅と高さ 
	for (int i = 0; i < ITEM_MAX; i++)
	{
		Item[i].state = 0;
	}
}

// 中心座標を指定して、画像を表示する関数
void DrawImage(int img, int x, int y)
{
	int w, h;
	GetGraphSize(img, &w, &h);
	DrawGraph(x - w / 2, y - h / 2, img, TRUE);
}

// 自機を動かす
void MovePlayer(void)
{
	if (CheckHitKey(KEY_INPUT_A))
	{ // 左入力
		Player.x -= Player.vx;
		if (Player.x < 175) Player.x = 175;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{ // 右入力
		Player.x += Player.vx;
		if (Player.x > WIDTH - 175) Player.x = WIDTH - 175;
	}

	Timer++;
	if (Timer % 30 < 15)
	{
		DrawGraph(Player.x, Player.y, imgPlayer, TRUE); // 自機の描画
	}
	if (Timer % 30 >= 15)
	{
		DrawTurnGraph(Player.x, Player.y, imgPlayer, TRUE);
	}
}

// 障害物のセット
int SetItem(int x, int y, int vx, int vy, int ptn, int img)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (Item[i].state == 0)
		{
			Item[i].x = x;
			Item[i].y = y;
			Item[i].vx = 0;
			Item[i].vy = 10;
			Item[i].state = 1;
			Item[i].pattern = ptn;
			Item[i].image = img;
			GetGraphSize(img, &Item[i].wid, &Item[i].hei); // 画像の幅と高さを代入
			return i;
		}
	}
	return -1;
}

// 障害物を動かす
void MoveItem(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (Item[i].state == 0) continue; // 空いている配列なら処理しない
		Item[i].x += Item[i].vx; // 移動
		Item[i].y += Item[i].vy; // 移動
		DrawImage(Item[i].image, Item[i].x, Item[i].y); // 障害物の描画
		// 画面外処理
		if (HEIGHT + 10 < Item[i].y) Item[i].state = 0;

		int dx = abs(Item[i].x - Player.x); // 中心座標
		int dy = abs(Item[i].y - Player.y); // 中心座標
		if (dx < Item[i].wid / 2 + Player.wid / 2 && dy < Item[i].hei / 2 + Player.hei / 2)
		{
			Player.y = Item[i].y;
		}
	}

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

void RandStage()
{
	stage_rand = GetRand(43) + 17;
	stage_distance = FPS * stage_rand; // ステージの長さ
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