#include <string>
#include "stdafx.h"
#include "MainFrm.h"
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

const vector<MapPARM> _mapP
{
    // Grounds of Plain Field
    MapPARM("Plain Field", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND1), 1,
            GroundPARM(100, 500, 1, 12, 1, IDB_GROUND1)),
    // Grounds of Arena
    MapPARM("Arena", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND2), 2,
            GroundPARM(560, 500, 0.174, 2, 1, IDB_GROUND2, 0, 900, 900), GroundPARM(400, 340, 0.174, 1, 2, IDB_GROUND2, 0, 900, 900)),
    // Grounds of Stair
    MapPARM("Stair", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND1), 3,
            GroundPARM(0, 300, 1, 3, 1, IDB_GROUND1), GroundPARM(500, 400, 1, 3, 1, IDB_GROUND1), GroundPARM(1000, 500, 1, 3, 1, IDB_GROUND1)),
	// Bounces Off Ground Map Test
	MapPARM("Bounces Test", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND1), 3,
			GroundPARM(0, 100, 1, 3, 5, IDB_GROUND1), GroundPARM(300, 500, 1, 10, 1, IDB_GROUND1), GroundPARM(1200, 100, 1, 3, 5, IDB_GROUND1)),
};

// Initialize static variable
bool CGameStateInit::_fullscreenEnabled = OPEN_AS_FULLSCREEN;
bool CGameStateInit::_closing = false;
bool CGameStateInit::_cameraEnabled = true;
int CGameStateInit::_mapSelected = 0;
vector<shared_ptr<Map>> CGameStateInit::maps;

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), welcomeWindow(Window(g)), settingWindow(Window(g)), aboutWindow(Window(g))
{
    /*camera.SetSize(0.5);
    welcomeWindow.AddCamera(&camera);
    settingWindow.AddCamera(&camera);*/
}

CGameStateInit::~CGameStateInit()
{
}

void CGameStateInit::OnInit()
{
    /*------------------------------INIT PROGRESS STAGE 1------------------------------*/
    InitializeNum();										// 初始化"resource.h"中點陣圖的資源編號
    InitializeNum("IDS");									// 初始化"resource.h"中音效的資源編號
    ShowInitProgress(5);
    /*------------------------------INIT PROGRESS STAGE 2------------------------------*/
    InitializeFile();										// 初始化"game.rc"中點陣圖的路徑
    InitializeFile("SOUND");								// 初始化"game.rc"中音效的路徑
    ShowInitProgress(10);

    // Automatically generate ground objects //
    for (auto map : _mapP)
    {
        shared_ptr<Map> tmap(new Map());

        for (auto ground : map._groundsP)
            tmap->AddGround(&ground);

        tmap->AddBackground(&map._bkP);
        maps.push_back(tmap);
    }

    for (auto map : maps)
        map->OnInit();

    ShowInitProgress(15);
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
    //
    int butW = refX - (SIZE_X - ui_info2->GetCor(2)), butH = (ui_info1->GetHeight() + ui_info4->GetHeight()) / 4;
    welcomeWindow.Initialize(4, 1);
    welcomeWindow.GetUI()->AddButton("start", refX - butW, refY, RGB(0, 255, 0), IDB_UI_BUTTON1_OUT, IDB_UI_BUTTON1_HOV, IDB_UI_BUTTON1_CLK, 0, 0);
    welcomeWindow.GetUI()->AddButton("settings", refX - butW, refY + butH, RGB(0, 255, 0), IDB_UI_BUTTON2_OUT, IDB_UI_BUTTON2_HOV, IDB_UI_BUTTON2_CLK, 1, 0);
    welcomeWindow.GetUI()->AddButton("about", refX - butW, refY + butH * 2, RGB(0, 255, 0), IDB_UI_BUTTON3_OUT, IDB_UI_BUTTON3_HOV, IDB_UI_BUTTON3_CLK, 2, 0);
    welcomeWindow.GetUI()->AddButton("exit", refX - butW, refY + butH * 3, RGB(0, 255, 0), IDB_UI_BUTTON4_OUT, IDB_UI_BUTTON4_HOV, IDB_UI_BUTTON4_CLK, 3, 0);
    settingWindow.Initialize(2, 2, false, false);
    //
    butW = 350;
    settingWindow.SetXY((SIZE_X - butW * 2) / 2, 300);
    settingWindow.GetUI()->AddButton("camera", 0, 0, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 0, "CAMERA : TRUE");
    settingWindow.GetUI()->AddButton("maps", butW, 0, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 1, "MAP : " + _mapP[_mapSelected]._name);
    settingWindow.GetUI()->AddButton("fullScreen", 0, 200, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 1, 0, (OPEN_AS_FULLSCREEN ? "FULLSCREEN : TRUE" : "FULLSCREEN : FALSE"));
    settingWindow.GetUI()->AddButton("back", butW, 200, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 1, 1, "BACK");
    //
    aboutWindow.Initialize(1, 1, false, false);
    aboutWindow.SetXY(300, 300);
    aboutWindow.GetUI()->AddButton("back", 0, 0, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 0, "BACK");
    ShowInitProgress(20);
}

void CGameStateInit::OnBeginState()
{
    for (auto map : maps)
        map->OnBeginState();

    static bool first = true;

    if (first)
    {
        first = false;
    }
    else
    {
        welcomeWindow.GetUI()->Reset();
        settingWindow.GetUI()->Reset();
        aboutWindow.GetUI()->Reset();
        CAudio::Instance()->Play(AUDIO_MENU_MUSIC, true);
    }
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    welcomeWindow.OnKeyDown(nChar, nRepCnt, nFlags);
    settingWindow.OnKeyDown(nChar, nRepCnt, nFlags);
    aboutWindow.OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    welcomeWindow.OnKeyUp(nChar, nRepCnt, nFlags);
    settingWindow.OnKeyUp(nChar, nRepCnt, nFlags);
    aboutWindow.OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    welcomeWindow.OnLButtonDown(nFlags, point);
    settingWindow.OnLButtonDown(nFlags, point);
    aboutWindow.OnLButtonDown(nFlags, point);
}

void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
    welcomeWindow.OnLButtonUp(nFlags, point);
    settingWindow.OnLButtonUp(nFlags, point);
    aboutWindow.OnLButtonUp(nFlags, point);
}

void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
{
    welcomeWindow.OnMouseMove(nFlags, point);
    settingWindow.OnMouseMove(nFlags, point);
    aboutWindow.OnMouseMove(nFlags, point);
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
    aboutWindow.OnShow();

    if (_closing)
    {
        static int count = 0;
        DrawRectangle(0, 0, SIZE_X, count);
        DrawRectangle(0, 0, count, SIZE_Y);
        DrawRectangle(SIZE_X - count, 0, count, SIZE_Y);
        DrawRectangle(0, SIZE_Y - count, SIZE_X, count);
        count += 20;

        if (count > SIZE_X / 2 || count > SIZE_Y / 2)
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
    }
}

bool CGameStateInit::GetCameraEnable()
{
    return _cameraEnabled;
}

bool CGameStateInit::GetFullscreenEnabled()
{
    return _fullscreenEnabled;
}

shared_ptr<Map> CGameStateInit::GetMap()
{
    return maps[CGameStateInit::_mapSelected];
}

void CGameStateInit::OnMove()
{
    CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;	// Get MainFrm instance
    _fullscreenEnabled = pMainWnd->IsFullScreen();					// Fix state of _fullscreenEnabled was not changed after ctrl+F (afx btn event triggered fullscreen)
    welcomeWindow.OnMove();
    settingWindow.OnMove();
    aboutWindow.OnMove();
    string chosenBut = welcomeWindow.GetUI()->ChosenButton();

    if (chosenBut == "start")
    {
        CAudio::Instance()->Stop(AUDIO_MENU_MUSIC);
        CAudio::Instance()->Play(AUDIO_CLICK_START);
        GotoGameState(GAME_STATE_RUN);
    }
    else if (chosenBut == "settings")
    {
        welcomeWindow.SetButtonEnable(false);
        settingWindow.SetButtonEnable(true);
        settingWindow.SetVisible(true);
    }
    else if (chosenBut == "about")
    {
        welcomeWindow.SetButtonEnable(false);
        aboutWindow.SetButtonEnable(true);
        aboutWindow.SetVisible(true);
    }
    else if (chosenBut == "exit")
        _closing = true;

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

        (*settingWindow.GetUI()->Index("camera"))->SetStr(_cameraEnabled ? "CAMERA : TRUE" : "CAMERA : FALSE");
    }
    else if (chosenBut == "maps")
    {
        _mapSelected++;

        if (_mapSelected == (signed int)maps.size())
            _mapSelected = 0;

        (*settingWindow.GetUI()->Index("maps"))->SetStr("MAP : " + _mapP[_mapSelected]._name);
    }
    else if (chosenBut == "fullScreen")
    {
        if (_fullscreenEnabled)
            _fullscreenEnabled = false;
        else
            _fullscreenEnabled = true;
    }

    chosenBut = aboutWindow.GetUI()->ChosenButton();

    if (chosenBut == "back")
    {
        welcomeWindow.SetButtonEnable(true);
        aboutWindow.SetButtonEnable(false);
        aboutWindow.SetVisible(false);
        aboutWindow.GetUI()->Reset();
    }

    static bool _fullscreenEnabledLast = _fullscreenEnabled;

    if (_fullscreenEnabledLast != _fullscreenEnabled)  													// _fullscreenEnabled changed state
    {
        (*settingWindow.GetUI()->Index("fullScreen"))->SetStr(_fullscreenEnabled ? "FULLSCREEN : TRUE" : "FULLSCREEN : FALSE");
        pMainWnd->SetFullScreen(_fullscreenEnabled);													// Implemente actull fullscreen that hide toolbar and menu etc
    }

    _fullscreenEnabledLast = _fullscreenEnabled;
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g), settingWindow(Window(g))
{
}

void CGameStateOver::OnMove()
{
    settingWindow.OnMove();
    string chosenBut = settingWindow.GetUI()->ChosenButton();

    if (chosenBut == "back")
    {
        GotoGameState(GAME_STATE_INIT);
    }
    else if (chosenBut == "exit")
    {
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
    }

    if (counter > 0)
        counter--;

    //    GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = (int)(180 * 0.5); // 0.5 seconds
}

void CGameStateOver::OnInit()
{
    Object* uiPtr = new Object();
    uiPtr->LoadBitmap(IDB_UI_BACKGROUND);
    uiPtr->SetSize(uiPtr->GetWidth() / SIZE_X);
    uiPtr->SetXY((SIZE_X - uiPtr->GetWidth()) / 2, 0);
    settingWindow.AddItem(uiPtr);
    int butH = 400, x = (SIZE_X - butH * 2) / 2, y = 400;
    settingWindow.Initialize(1, 2);
    settingWindow.GetUI()->AddButton("back", x, y, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 0, "BACK");
    settingWindow.GetUI()->AddButton("exit", x + butH, y, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 1, "EXIT");
    ShowInitProgress(100);
}

void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    settingWindow.OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    settingWindow.OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
{
    settingWindow.OnLButtonDown(nFlags, point);
}

void CGameStateOver::OnLButtonUp(UINT nFlags, CPoint point)
{
    settingWindow.OnLButtonUp(nFlags, point);
}

void CGameStateOver::OnMouseMove(UINT nFlags, CPoint point)
{
    settingWindow.OnMouseMove(nFlags, point);
}

void CGameStateOver::OnShow()
{
    settingWindow.OnShow();
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over ! (%d)", counter / 30);
    int textSize = 50;
    SIZE strSize = GetStringSize(str, textSize);
    OnShowText(str, (SIZE_X - strSize.cx) / 2, 210, textSize);
    char gameResultStr[80];
    sprintf(gameResultStr, CGameStateRun::GetLegacyString().c_str());
    strSize = GetStringSize(gameResultStr, textSize);
    OnShowText(gameResultStr, (SIZE_X - strSize.cx) / 2, 650, textSize);
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
        CAudio::Instance()->Stop(AUDIO_BATTLE_MUSIC);
        SetLegacyString(battleSystem.GetGameResult());
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
    battleSystem.OnLButtonDown(nFlags, point);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    battleSystem.OnLButtonUp(nFlags, point);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    battleSystem.OnMouseMove(nFlags, point);
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
    battleSystem.OnLButtonDown(nFlags, point);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    battleSystem.OnLButtonUp(nFlags, point);
}

void CGameStateRun::OnShow()
{
    if (!battleSystem.IsGameOver())
        battleSystem.OnShow();

    /*static int deg = 0;
    test.Rotate(deg++);
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