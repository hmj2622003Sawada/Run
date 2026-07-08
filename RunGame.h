#pragma once

// 構造体の宣言
struct OBJECT // 自機用
{
	int x;	// x座標
	int y;	// y座標
	int vx; // x座標方向の速さ
	int vy; // y座標方向の速さ
	int state; // 存在しているか
	int pattern;
	int image;
	int wid;   // 画像の横幅
	int hei;   // 画像の立幅
	int timer;
};

// 関数宣言
void InitGame(void);
void ScrollBG(int sod);
void DrawTextC(int x, int y, const char* txt, int col, int siz);
int LoadGraphWithCheck(const char* file);
int LoadSoundMemWithCheck(const char* file);
void DrawTextC(int x, int y, const char* txt, int col, int siz);
void MovePlayer(void);
void SetItem(void);
void MoveItem(void);
void DrawPlayer(void);
void RandStage(void);
void DrawImage(int img, int x, int y);
