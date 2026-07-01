#pragma once

// \‘¢‘Ì‚ÌéŒ¾
struct OBJECT // ©‹@—p
{

};

// ŠÖ”éŒ¾
void InitGame(void);
void ScrollBG(int sod);
void DrawTextC(int x, int y, const char* txt, int col, int siz);
int LoadGraphWithCheck(const char* file);
int LoadSoundMemWithCheck(const char* file);
