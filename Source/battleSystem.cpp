#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "battleSystem.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// CGround : ground class
/////////////////////////////////////////////////////////////////////////////

//static GameComFunc func;// = GameComFunc(0);					// 自訂函式

BattleSystem::BattleSystem(CGame* g) : CGameState(g)
{
}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::OnBeginState()
{
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
}

void BattleSystem::OnMove()							// 移動遊戲元素
{
    player.OnMove();
    background.SetXY((int)(-player.GetX1() * 0.5), (int)(-player.GetY1() * 0.5));
    ground.SetXY((int)(-player.GetX1() * 0.5 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2), (int)(-player.GetY1() * 0.5) + 600);

    if (player.IsOnGround())
    {
        player.SetMovingUp(false);
        player.SetMovingDown(false);
    }
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
    InitializeAllBMP();
    TRACE("idbList size : %d\n", idbList.size());
    TRACE("fileList size : %d\n", fileList.size());
    TRACE("cArray size : %d\n", cArray.size());
    //CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
    ShowInitProgress(50);
    Sleep(300);
    ground.LoadBitmap();
    ground.SetSize(0.65);
    background.SetSize(0.8);
    background.LoadBitmap(IDB_BACKGROUND, RGB(0, 0, 0));
    ShowInitProgress(75);
    Sleep(300);
    ground.SetXY(150, 400);
    ground.SetLen(30);
    player.LoadBitmap();
    player.SetXY((int)(-player.GetX1() * 0.5 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2), 400);
    ShowInitProgress(100);
    /*
    func.collide("RES\\player1\\adventurer-run-00.bmp");
    func.collide("RES\\player1\\adventurer-run-01.bmp");
    func.collide("RES\\player1\\adventurer-run-02.bmp");
    func.collide("RES\\player1\\adventurer-run-03.bmp");
    func.collide("RES\\player1\\adventurer-run-04.bmp");
    */
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
    currentKeydown = nChar;

    if (nChar == KEY_LEFT)
        player.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        player.SetMovingRight(true);

    if (nChar == KEY_UP || nChar == 32)
    {
        player.Jump(20);
        player.SetMovingUp(true);
    }

    //player.SetMovingUp(true);
    //if (nChar == KEY_DOWN)
    //player.SetMovingDown(true);
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
    const char KEY_ESC = 27;

    if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲

    if (nChar == KEY_LEFT)
        player.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        player.SetMovingRight(false);

    //if (nChar == KEY_UP)
    //player.SetMovingUp(false);
    //if (nChar == KEY_DOWN)
    //player.SetMovingDown(false);
}

void BattleSystem::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    mousePoint = point;
    // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void BattleSystem::OnShow()
{
    background.OnShow();
    char str[80];
    sprintf(str, "(%d, %d) KeyDown:%d", mousePoint.x, mousePoint.y, currentKeydown);
    OnShowText(str, 0, 0);
    sprintf(str, "Hit ground:%d, Player (x1:%d, y1:%d, x2:%d, y2:%d)", ground.Collision(&player.array, ground.GetCor(0), ground.GetCor(1), player.GetX1(), player.GetY1()), player.GetX1(), player.GetY1(), player.GetX2(), player.GetY2());
    OnShowText(str, 0, 20);
    sprintf(str, "                     , Ground (x1:%d, y1:%d, x2:%d, y2:%d)", ground.GetCor(0), ground.GetCor(1), ground.GetCor(2), ground.GetCor(3));
    OnShowText(str, 0, 40);
    ground.OnShow();
    player.OnShow();
}
}