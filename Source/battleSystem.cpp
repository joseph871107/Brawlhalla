﻿#include "stdafx.h"
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
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

//-----------------CONSTANTS DEFINITIONS-----------------//
const int MATCH_TIME = 180;
//-----------------FUNCTIONS DEFINITIONS-----------------//

const vector<GroundPARM> _groundXY{ GroundPARM(0, 300, 0.65, 5), GroundPARM(500, 400, 0.65, 5), GroundPARM(1000, 500, 0.65, 5) };	// Define Ground position to automatically generate ground objects
CInteger integer(2);																												// Used to show current remain time

BattleSystem::BattleSystem(CGame* g) : CGameState(g), background(Object()), _ground(vector<Ground*>())
{
}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::OnBeginState()
{
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    /*------------------------------INIT PROGRESS STAGE 1------------------------------*/
    start = chrono::high_resolution_clock::now();

	_secPerRound = MATCH_TIME;
	vector<GroundPARM> groundXY = _groundXY;
	player1.Initialize(_ground, "Player 1", 1);
	player2.Initialize(_ground, "Player 2", 2);
	weapon.Initialize(_ground, vector<Player*>{&player1, &player2});
}

void BattleSystem::OnMove()							// 移動遊戲元素
{
	weapon.OnMove();
    player1.OnMove();
    player2.OnMove();
    /*background.SetXY(-(int)((player1.GetCor(0) + player2.GetCor(0)) / 2 * 0.2) - 100, -(int)((player1.GetCor(1) + player2.GetCor(1)) / 2 * 0.2) - 50);
    vector<CPoint> groundXY = _groundXY;
    for (vector<CPoint>::iterator i = groundXY.begin(); i != groundXY.end(); i++)
    {
    	_ground[i-groundXY.begin()]->SetXY(i->x-(int)((player1.GetCor(0) + player2.GetCor(0)) / 2 * 0.2), i->y-(int)((player1.GetCor(1) + player2.GetCor(1)) / 2 * 0.2));
    }*/
    //background.SetXY((int)(-player.GetX1() * 0.3), (int)(-player.GetY1() * 0.2));
    //ground.SetXY((int)(-player.GetX1() * 0.5 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2 + 0.5) + 400, (int)(-player.GetY1() * 0.5 + 0.5) + 600);
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
    //InitializeAllBMP();								// 初始化點陣圖運算資源
    InitializeIDB();									// 初始化"resource.h"中點陣圖的資源編號
    TRACE("idbList size : %d\n", idbList.size());
    ShowInitProgress(13);
    /*------------------------------INIT PROGRESS STAGE 2------------------------------*/
    InitializeFile();									// 初始化"game.rc"中點陣圖的路徑
    TRACE("fileList size : %d\n", fileList.size());
    ShowInitProgress(25);

    /*------------------------------INIT PROGRESS STAGE 3------------------------------*/
    if (GENERATE_COLLISION_ARRAY)
    {
        InitializeCollideArray();							// 初始化所有點陣圖的布林碰撞矩陣
        TRACE("cArray size : %d\n", cArray.size());
        ShowInitProgress(50);
    }

    /*------------------------------INIT PROGRESS STAGE 4------------------------------*/
    //CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3

	vector<GroundPARM> groundXY = _groundXY;

	for (auto i = groundXY.begin(); i != groundXY.end(); i++)		// Automatically generate ground objects
	{
		Ground* ground = new Ground();
		ground->LoadBitmap();
		ground->SetXY(i->point.x, i->point.y);
		ground->SetSize(i->_size);
		ground->SetLen(i->_length);
		_ground.push_back(ground);
	}

    background.SetSize(1);
    background.SetXY(-250, 0);
    background.LoadBitmap(IDB_BACKGROUND, RGB(0, 0, 0));
	weapon.LoadBitmap();
    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
    player1.LoadBitmap();				// Player1
    player2.LoadBitmap();				// Player2
    integer.LoadBitmap();				// time + life
    //player.SetXY((int)(2000 + (background.GetCor(2) - background.GetCor(0) - ground.GetCor(2) + ground.GetCor(0)) / 2), 400);
    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    player1.OnKeyDown(nChar);
    player2.OnKeyDown(nChar);
    currentKeydown = nChar;
	weapon.OnKeyDown(nChar);
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;

    if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲

    player1.OnKeyUp(nChar);
    player2.OnKeyUp(nChar);
}

void BattleSystem::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    mousePoint = point;
    // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void BattleSystem::OnShow()
{
    background.OnShow();

    //------------------Test Text------------------//
    if (_PLAYER_DEBUG)
    {
        char str[80], str2[80];
        ostringstream oss;
        oss << hex << currentKeydown;
        sprintf(str, "(%d, %d) KeyDown:%s", mousePoint.x, mousePoint.y, ("0x" + oss.str()).c_str());
        OnShowText(str, 0, 0, 10);
        sprintf(str, "Player1 (x1:%d, y1:%d, x2:%d, y2:%d)", player1.GetCor(0), player1.GetCor(1), player1.GetCor(2), player1.GetCor(3));
        OnShowText(str, 0, 12, 10);
        sprintf(str, "Player2 (x1:%d, y1:%d, x2:%d, y2:%d)", player2.GetCor(0), player2.GetCor(1), player2.GetCor(2), player2.GetCor(3));
        OnShowText(str, 0, 24, 10);

        for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
        {
            sprintf(str, "                      , Ground%d (x1:%d, y1:%d, x2:%d, y2:%d)", i - _ground.begin(), (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
            OnShowText(str, 0, 36 + 12 * (i - _ground.begin()), 10);
        }

        sprintf(str, "%s", GetNameFromIDB(player1.ShowAnimationState()).c_str());
        OnShowText(str, 0, SIZE_Y - 24, 20);
        sprintf(str, "%s", GetNameFromIDB(player2.ShowAnimationState()).c_str());
        OnShowText(str, 200, SIZE_Y - 24, 20);
        sprintf(str, "Remain Time : %s : %s\n", (GetCurrentRemainTime() / 600 == 0 ? ("0" + (string)str).c_str() : str), ((GetCurrentRemainTime() % 60) / 10 == 0 ? ("0" + (string)str2).c_str() : str2));
        OnShowText(str, 650, 0, 30);
    }

    //------------------End of Test Text------------------//
    // Showing the remain time
    // Display minute
    int now_time = GetCurrentRemainTime();
    integer.SetInteger(now_time / 60);
    integer.SetTopLeft(735, 0);
    integer.ShowBitmap();
    // Display second
    integer.SetInteger(now_time % 60);
    integer.SetTopLeft(805, 0);
    integer.ShowBitmap();

    // Show ground
    for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
    {
        (*i)->OnShow();
    }
	weapon.OnShow();

    // Show player
    player2.OnShow();
    player1.OnShow();
    // Show player's life
    ShowPlayerLife(player1, 1200, 0);
    ShowPlayerLife(player2, 1400, 0);
}

bool BattleSystem::IsGameOver()
{
    return (player1.IsOutOfLife() || player2.IsOutOfLife() || (GetCurrentRemainTime() == 0));
}

string BattleSystem::GetGameResult()
{
    if (player1.GetLife() > player2.GetLife())
    {
        return ("Player 1 wins.");
    }
    else if (player1.GetLife() < player2.GetLife())
    {
        return ("Player 2 wins.");
    }
    else
    {
        return ("Draw.");
    }
}

void BattleSystem::ShowPlayerLife(const Player& player, int posXValue, int posYValue)
{
    // Display player's name
    char playerName[80];
    sprintf(playerName, (player.GetName() + " Life").c_str());
    OnShowText(playerName, posXValue, posYValue + 20);
    // Displayer player's life
    integer.SetInteger(player.GetLife()); //CInteger integer
    integer.SetTopLeft(posXValue + 120, posYValue);
    integer.ShowBitmap();
}

int BattleSystem::GetCurrentRemainTime()
{
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - start;
    auto ms = chrono::duration_cast<chrono::milliseconds>(dur).count();
    return (int)(_secPerRound - (ms / 1000));
}

}