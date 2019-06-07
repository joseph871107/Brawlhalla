#pragma   once
#ifndef GAMECOMFUNC_H
#define GAMECOMFUNC_H
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace game_framework
{
/* KEY */
const long KEY_A = 0x41;
const long KEY_S = 0x53;
const long KEY_D = 0x44;
const long KEY_W = 0x57;
const long KEY_LEFT = 0x25;
const long KEY_UP = 0x26;
const long KEY_RIGHT = 0x27;
const long KEY_DOWN = 0x28;
const long KEY_C = 0x43;
const long KEY_COMMA = 0xbc;
const long KEY_U = 0x55;
const long KEY_V = 0x56;
const long KEY_X = 0x58;
const long KEY_M = 0x4d;
const long KEY_ENTER = 0x0d;
const long KEY_ESC = 0x1b;
const long KEY_PERIOD = 0xbe;
/* MAP BORDER */
const int MAP_BORDER_OFFSET = 1000;
const int MAP_BORDER_X1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_Y1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_X2 = SIZE_X + MAP_BORDER_OFFSET;
const int MAP_BORDER_Y2 = SIZE_Y + MAP_BORDER_OFFSET;
/* SOUND */
enum AUDIO_ID  				// 定義各種音效的編號
{
    AUDIO_BATTLE_MUSIC,				// 0
    AUDIO_DRAW_WEAPON,				// 1
    AUDIO_MENU_MUSIC,				// 2
    AUDIO_PUNCH,					// 3
    AUDIO_SWING_ATTACK,				// 4
    AUDIO_SWOOSH,					// 5
    AUDIO_EXPLOSION_FX,				// 6
    AUDIO_CLICK_START				// 7
};

class Ground;
struct ColArray
{
    int width;
    int height;
    string fileName;
    vector<vector<bool>> pixel;
};

struct ColBmp
{
    int width;
    int height;
    vector< vector<COLORREF>> pixel;
};

class CRAnimation : public CAnimation
{
    public:
        void NextPtr();
        void PreviousPtr();
};

/////////////////////////
// 碰撞矩陣全域變數
/////////////////////////
extern map<string, int> idbList;
extern map<string, int> idsList;
extern map<int, string> fileList;
extern map<int, string> soundList;
extern map<int, ColBmp> colBmp;
extern map<int, ColArray> cArray;

/////////////////////////
// 碰撞矩陣處理副程式
/////////////////////////
void InitializeAll(bool = false, string = "Source\\resource.h", string = "Source\\game.rc");	// 初始化點陣圖運算資源
void InitializeNum(string = "IDB", string = "Source\\resource.h");								// 初始化"resource.h"中的資源編號
void InitializeFile(string = "BITMAP", string = "Source\\game.rc");								// 初始化"game.rc"中點陣圖的路徑
void InitializeLoadSound();																		// 初始化聲音資源
void InitializeCollideArray(bool = false);														// 初始化點陣圖的布林碰撞矩陣

/////////////////////////
// 低層輔助副程式
/////////////////////////
string GetPathFromResource(int, string type = "IDB");										// 以IDB取得資源路徑
string GetNameFromResource(int, string type = "IDB");										// 以IDB取得IDB名稱
void TraceCollideArray(int, bool = true);						// 顯示點陣圖布林矩陣在Debugger中
ColBmp readBMP(int file, int = 1000, int = 1000);				// file 為檔案路徑
void GetCollideArray(int file, ColBmp*);						// file 為IDB名稱


/////////////////////////
// 通用輔助副程式
/////////////////////////
void OnShowText(string, int, int, int = 20, COLORREF = RGB(255, 255, 255), COLORREF = RGB(256, 256, 256), LPCTSTR = "Agency FB");	// 簡化原本Game Framework 的字串顯示
void DrawRectangleBlock(int x, int y, int width, int height, COLORREF color = RGB(255, 255, 255));
void DrawRectangle(int x, int y, int width, int height, COLORREF color = RGB(0, 0, 0));
void DrawLine(int x1, int y1, int x2, int y2, COLORREF color = RGB(255, 255, 255));
SIZE GetStringSize(string str, int textSize);
char* ToCharPtr(string);
int random(int min, int max);
Ground* GetRandomGround(vector<Ground*>* ground);
bool IsCollide(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2);
vector<vector<CMovingBitmap>> CropSprite(int IDB, int row, int column, COLORREF color);
void FlipSprite(vector<vector<CMovingBitmap>>*);

/////////////////////////
// Others
/////////////////////////
int DoubleToInteger(double value);
}

#endif //define GAMECOMFUNC_H