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
    //background.SetXY((int)(-player.GetX1() * 0.3), (int)(-player.GetY1() * 0.2));
    //ground.SetXY((int)(-player.GetX1() * 0.5 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2 + 0.5) + 400, (int)(-player.GetY1() * 0.5 + 0.5) + 600);
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
    /*------------------------------INIT PROGRESS STAGE 1------------------------------*/
    start = chrono::high_resolution_clock::now();
    //InitializeAllBMP();								// 初始化點陣圖運算資源
    InitializeIDB();									// 初始化"resource.h"中點陣圖的資源編號
    TRACE("idbList size : %d\n", idbList.size());
    ShowInitProgress(13);
    /*------------------------------INIT PROGRESS STAGE 2------------------------------*/
    InitializeFile();									// 初始化"game.rc"中點陣圖的路徑
    TRACE("fileList size : %d\n", fileList.size());
    ShowInitProgress(25);
    /*------------------------------INIT PROGRESS STAGE 3------------------------------*/
    InitializeCollideArray();							// 初始化所有點陣圖的布林碰撞矩陣
    TRACE("cArray size : %d\n", cArray.size());
    ShowInitProgress(50);
    /*------------------------------INIT PROGRESS STAGE 4------------------------------*/
    //CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
    ground.LoadBitmap();
    ground.SetSize(0.65);
    background.SetSize(0.8);
    background.LoadBitmap(&vector<int> { IDB_BACKGROUND }, RGB(0, 0, 0));
    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
    ground.SetXY(300, 400);
    ground.SetLen(5);
    player.Initialize(&ground);
    player.LoadBitmap();
    //player.SetXY((int)(2000 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2), 400);
    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    player.OnKeyDown(nChar);
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;

    if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲

    player.OnKeyUp(nChar);
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
    OnShowText(str, 0, 0, 10);
    sprintf(str, "Hit ground:%d, Player (x1:%d, y1:%d, x2:%d, y2:%d)", ground.Collision(&cArray.find(IDB_GROUND)->second, 1.0, ground.GetCor(0), ground.GetCor(1), player.GetX1(), player.GetY1()), player.GetX1(), player.GetY1(), player.GetX2(), player.GetY2());
    OnShowText(str, 0, 12, 10);
    sprintf(str, "                      , Ground (x1:%d, y1:%d, x2:%d, y2:%d)", ground.GetCor(0), ground.GetCor(1), ground.GetCor(2), ground.GetCor(3));
    OnShowText(str, 0, 24, 10);
    sprintf(str, "%s", GetNameFromIDB(player.ShowAnimationState()).c_str());
    OnShowText(str, 0, 36, 10);
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - start;
    auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();
    sprintf(str, "Current run time : %f\n", ms / 1000.0);
    OnShowText(str, 0, 48, 20);
    ground.OnShow();
    player.OnShow();
}
}