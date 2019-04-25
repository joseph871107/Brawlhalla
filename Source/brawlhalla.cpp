/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include <string>
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

bool CGameStateInit::_cameraEnabled = true; //initialize

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), welcomeWindow(Window(g)), settingWindow(Window(g))
{
    /*Camera *camera = new Camera(400,400);
    camera->SetSize(0.5);
    welcomeWindow.AddCamera(camera);*/
}

CGameStateInit::~CGameStateInit()
{
}

void CGameStateInit::OnInit()
{
    ShowInitProgress(0);
    Object* uiPtr, *ui_info1, *ui_info2, *ui_info3, *ui_info4;
    uiPtr = new Object();
    uiPtr->LoadBitmap(IDB_UI_BACKGROUND);
    uiPtr->SetSize(uiPtr->GetWidth() / SIZE_X);
    uiPtr->SetXY((SIZE_X - uiPtr->GetWidth()) / 2, 0);
    welcomeWindow.AddItem(uiPtr);
    uiPtr = new Object(0.8);
    uiPtr->LoadBitmap(IDB_UI_TITLE, RGB(0, 255, 0));
    uiPtr->SetXY((SIZE_X - uiPtr->GetWidth()) / 2, 0);
    welcomeWindow.AddItem(uiPtr);
    ui_info1 = new Object(0.7);
    ui_info1->LoadBitmap(IDB_UI_INFO1);
    int refX = (SIZE_X - ui_info1->GetWidth()) / 2, refY = 200;
    ui_info1->SetXY(refX, refY);
    welcomeWindow.AddItem(ui_info1);
    ui_info2 = new Object(ui_info1->GetSize());
    ui_info2->SetXY(refX + ui_info1->GetWidth(), refY);
    ui_info2->LoadBitmap(IDB_UI_INFO2);
    welcomeWindow.AddItem(ui_info2);
    ui_info3 = new Object();
    ui_info3->LoadBitmap(IDB_UI_INFO3);
    ui_info3->SetSize((float)(ui_info2->GetWidth()) / (float)(ui_info3->GetWidth()));
    ui_info3->SetXY(refX + ui_info1->GetWidth(), refY + ui_info2->GetHeight());
    welcomeWindow.AddItem(ui_info3);
    ui_info4 = new Object();
    ui_info4->LoadBitmap(IDB_UI_INFO4);
    ui_info4->SetSize((float)(ui_info1->GetWidth()) / (float)(ui_info4->GetWidth()));
    ui_info4->SetXY(refX, refY + ui_info1->GetHeight());
    welcomeWindow.AddItem(ui_info4);
    int butW = refX - (SIZE_X - ui_info2->GetCor(2)), butH = (ui_info1->GetHeight() + ui_info4->GetHeight()) / 3;
    welcomeWindow.Initialize(1, 3);
    welcomeWindow.GetUI()->AddButton("start", refX - butW, refY, butW, butH, 0, 0);
    welcomeWindow.GetUI()->AddButton("settings", refX - butW, refY + butH, butW, butH, 0, 1);
    welcomeWindow.GetUI()->AddButton("exit", refX - butW, refY + butH * 2, butW, butH, 0, 2);
    settingWindow.Initialize(1, 2, false, false);
    settingWindow.SetXY((SIZE_X - butH) / 2, 300);
    settingWindow.GetUI()->AddButton("camera", 0, 0, butH, butH, 0, 0, "True");
    settingWindow.GetUI()->AddButton("back", 0, butH, butH, butH, 0, 1);
    ShowInitProgress(10);
}

void CGameStateInit::OnBeginState()
{
    static bool first = true;

    if (first)
    {
        first = false;
    }
    else
    {
        welcomeWindow.GetUI()->Reset();
        settingWindow.GetUI()->Reset();
        CAudio::Instance()->Play(IDS_MENU_MUSIC, true);
    }
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    welcomeWindow.OnKeyDown(nChar, nRepCnt, nFlags);
    settingWindow.OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    welcomeWindow.OnKeyUp(nChar, nRepCnt, nFlags);
    settingWindow.OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    welcomeWindow.OnLButtonDown(nFlags, point);
    settingWindow.OnLButtonDown(nFlags, point);
}

void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
    welcomeWindow.OnLButtonUp(nFlags, point);
    settingWindow.OnLButtonUp(nFlags, point);
}

void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
{
    welcomeWindow.OnMouseMove(nFlags, point);
    settingWindow.OnMouseMove(nFlags, point);
}

void CGameStateInit::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnShow()
{
    welcomeWindow.OnShow();
    settingWindow.OnShow();
}

bool CGameStateInit::GetCameraEnable()
{
    return _cameraEnabled;
}

void CGameStateInit::OnMove()
{
    welcomeWindow.OnMove();
    settingWindow.OnMove();
    string chosenBut = welcomeWindow.GetUI()->ChosenButton();

    if (chosenBut == "start")
    {
        CAudio::Instance()->Stop(IDS_MENU_MUSIC);
        GotoGameState(GAME_STATE_RUN);
    }
    else if (chosenBut == "settings")
    {
        welcomeWindow.SetButtonEnable(false);
        settingWindow.SetButtonEnable(true);
        settingWindow.SetVisible(true);
    }
    else if (chosenBut == "exit")
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲

    chosenBut = settingWindow.GetUI()->ChosenButton();

    if (chosenBut == "back")
    {
        welcomeWindow.SetButtonEnable(true);
        settingWindow.SetButtonEnable(false);
        settingWindow.SetVisible(false);
        settingWindow.GetUI()->Reset();
    }
    else if (chosenBut == "camera")
    {
        if (_cameraEnabled)
            _cameraEnabled = false;
        else
            _cameraEnabled = true;

        (*settingWindow.GetUI()->Index("camera"))->str = (_cameraEnabled ? "True" : "False");
    }
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = (int)(180 * 0.5); // 0.5 seconds
}

void CGameStateOver::OnInit()
{
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over ! (%d)", counter / 30);
    OnShowText(str, 240, 210);
    char gameResultStr[80];
    sprintf(gameResultStr, CGameStateRun::GetLegacyString().c_str());
    OnShowText(gameResultStr, 240, 300);
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

string CGameStateRun::_legacyString = ""; //initialize

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), battleSystem(BattleSystem(g))
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
    battleSystem.OnBeginState();
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
    if (battleSystem.IsGameOver())								// Demo 關閉遊戲的方法
    {
        CAudio::Instance()->Stop(IDS_BATTLE_MUSIC);
        SetLegacyString(battleSystem.GetGameResult());
        battleSystem.ResolveMemoryLeaksOnEndState();
        GotoGameState(GAME_STATE_OVER);					// 關閉遊戲
    }
    else
    {
        battleSystem.OnMove();
    }
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
    battleSystem.OnInit();
    /*test.LoadBitmap(IDB_GROUND,RGB(0,255,0));
    test.SetAlpha(100);
    test.SetTopLeft(0, 0);*/
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    battleSystem.OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    battleSystem.OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    battleSystem.OnMouseMove(nFlags, point);
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
    if (!battleSystem.IsGameOver())
        battleSystem.OnShow();

    static int deg = 0;
    /*test.Rotate(deg++);
    test.ShowBitmap();*/
}

string CGameStateRun::GetLegacyString()
{
    return (_legacyString);
}

void CGameStateRun::SetLegacyString(string newLegacyString)
{
    _legacyString = newLegacyString;
}

}