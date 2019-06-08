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
int _gameMode = 1, _aiNum = 1, _aiDiff = 2;
const vector<MapPARM> _mapP
{
    // Grounds of Plain Field
    MapPARM("Plain Field", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND1), 1,
            GroundPARM(100, 500, 1, 12, 1, IDB_GROUND1)),
    // Grounds of Arena
    MapPARM("Arena", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND2), 3,
            GroundPARM(400, 400, 0.087, 10, 1, IDB_GROUND2, 0, 900, 900),
            GroundPARM(0, 300, 0.087, 4, 1, IDB_GROUND2, 0, 900, 900),
            GroundPARM(1300, 300, 0.087, 4, 1, IDB_GROUND2, 0, 900, 900)),
    // Grounds of Stair
    MapPARM("Stair", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND1), 3,
            GroundPARM(0, 300, 1, 3, 1, IDB_GROUND1),
            GroundPARM(500, 400, 1, 3, 1, IDB_GROUND1),
            GroundPARM(1000, 500, 1, 3, 1, IDB_GROUND1)),
    // Grounds of Stair
    MapPARM("Arena Stair", BkPARM(0, 0, 1, 0.15, IDB_BACKGROUND2), 3,
            GroundPARM(0, 300, 0.087, 3, 1, IDB_GROUND2, 0, 900, 900),
            GroundPARM(500, 400, 0.087, 3, 1, IDB_GROUND2, 0, 900, 900),
            GroundPARM(1000, 500, 0.087, 3, 1, IDB_GROUND2, 0, 900, 900)),
};

// Initialize static variable
bool CGameStateInit::_fullscreenEnabled = OPEN_AS_FULLSCREEN;
bool CGameStateInit::_closing = false;
bool CGameStateInit::_cameraEnabled = true;
int CGameStateInit::_mapSelected = 0;
vector<shared_ptr<Map>> CGameStateInit::maps;

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
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
    //
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
    int butW = refX - (SIZE_X - ui_info2->GetCor(2)), butH = (ui_info1->GetHeight() + ui_info4->GetHeight()) / 3;
    welcomeWindow.Initialize(3, 1);
    welcomeWindow.GetUI()->AddButton("start", refX - butW, refY, RGB(0, 255, 0), IDB_UI_BUTTON1_OUT, IDB_UI_BUTTON1_HOV, IDB_UI_BUTTON1_CLK, 0, 0);
    welcomeWindow.GetUI()->AddButton("settings", refX - butW, refY + butH, RGB(0, 255, 0), IDB_UI_BUTTON2_OUT, IDB_UI_BUTTON2_HOV, IDB_UI_BUTTON2_CLK, 1, 0);
    welcomeWindow.GetUI()->AddButton("exit", refX - butW, refY + butH * 2, RGB(0, 255, 0), IDB_UI_BUTTON3_OUT, IDB_UI_BUTTON3_HOV, IDB_UI_BUTTON3_CLK, 2, 0);
    windows.push_back(&welcomeWindow);
    //
    Object* temp_cheat = new Object();
    CAnimation* temp = new CAnimation(true, 1, 5);
    CMovingBitmap but;
    temp_cheat->LoadBitmap(IDB_UI_INSTR_CHEAT);
    but.LoadBitmap(IDB_UI_BUTTON0_OUT);
    butW = 10, butH = 200;
    temp->AddBitmap(IDB_UI_INSTR_0);
    temp->AddBitmap(IDB_UI_INSTR_1);
    temp->AddBitmap(IDB_UI_INSTR_2);
    temp->AddBitmap(IDB_UI_INSTR_3);
    temp->AddBitmap(IDB_UI_INSTR_4);
    temp->AddBitmap(IDB_UI_INSTR_5);
    temp->AddBitmap(IDB_UI_INSTR_6);
    temp->AddBitmap(IDB_UI_INSTR_7);
    temp->AddBitmap(IDB_UI_INSTR_8);
    temp->AddBitmap(IDB_UI_INSTR_9);
    temp->AddBitmap(IDB_UI_INSTR_10);
    temp->SetSize(0.3);
    temp_cheat->SetSize(0.3);
    int butWidth = but.Width();
    refX = (SIZE_X - butW * 4 - butWidth * 5) / 2;
    butWidth += butW;
    refY = (SIZE_Y - temp->Height() - butH - 50) / 2;
    int butRefY = SIZE_Y - refY;
    temp->SetTopLeft((SIZE_X - temp->Width()) / 2, refY);
    temp_cheat->SetXY((SIZE_X - temp->Width()) / 2, refY + temp->Height() + 10);
    startWindow.AddItem(temp_cheat);
    startWindow.AddAni(temp);
    startWindow.Initialize(1, 5, false, false);
    startWindow.SetXY(0, 0);
    startWindow.GetUI()->AddButton("back", refX, butRefY, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 0, "BACK");
    startWindow.GetUI()->AddButton("gameMode", refX + butWidth, butRefY, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 1, "Game Mode : PvC");
    startWindow.GetUI()->AddButton("aiNum", refX + butWidth * 2, butRefY, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 2, "How many AI : 1");
    startWindow.GetUI()->AddButton("aiDiff", refX + butWidth * 3, butRefY, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 3, "AI Difficulty : Normal");
    startWindow.GetUI()->AddButton("start", refX + butWidth * 4, butRefY, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 4, "START");
    windows.push_back(&startWindow);
    //
    butW = 350;
    settingWindow.Initialize(2, 2, false, false);
    settingWindow.SetXY((SIZE_X - butW * 2) / 2, 300);
    settingWindow.GetUI()->AddButton("camera", 0, 0, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 0, "CAMERA : TRUE");
    settingWindow.GetUI()->AddButton("maps", butW, 0, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 0, 1, "MAP : " + _mapP[_mapSelected]._name);
    settingWindow.GetUI()->AddButton("fullScreen", 0, 200, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 1, 0, (OPEN_AS_FULLSCREEN ? "FULLSCREEN : TRUE" : "FULLSCREEN : FALSE"));
    settingWindow.GetUI()->AddButton("back", butW, 200, RGB(0, 255, 0), IDB_UI_BUTTON0_OUT, IDB_UI_BUTTON0_HOV, IDB_UI_BUTTON0_CLK, 1, 1, "BACK");
    windows.push_back(&settingWindow);
    //
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
        for (auto window : windows)
            window->GetUI()->Reset();

        CAudio::Instance()->Play(AUDIO_MENU_MUSIC, true);
    }
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    for (auto window : windows)
        window->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    for (auto window : windows)
        window->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    for (auto window : windows)
        window->OnLButtonDown(nFlags, point);
}

void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
    for (auto window : windows)
        window->OnLButtonUp(nFlags, point);
}

void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
{
    for (auto window : windows)
        window->OnMouseMove(nFlags, point);
}

void CGameStateInit::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnShow()
{
    for (auto window : windows)
        window->OnShow();

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

string CGameStateInit::GetGameMode(int mode)
{
    vector<string> modeList =
    {
        "PvP",
        "PvC",
        "Boss"
    };
    return modeList[mode];
}

string CGameStateInit::GetAiDifficulty(int diff)
{
    vector<string> diffList =
    {
        "Super Easy",
        "Easy",
        "Normal",
        "Hard",
        "Super Hard"
    };
    return diffList[diff];
}

void CGameStateInit::OnMove()
{
    CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;	// Get MainFrm instance
    _fullscreenEnabled = pMainWnd->IsFullScreen();					// Fix state of _fullscreenEnabled was not changed after ctrl+F (afx btn event triggered fullscreen)

    for (auto window : windows)
        window->OnMove();

    string chosenBut = welcomeWindow.GetUI()->ChosenButton();

    if (chosenBut == "start")
    {
        welcomeWindow.SetButtonEnable(false);
        startWindow.SetButtonEnable(true);
        startWindow.SetVisible(true);
    }
    else if (chosenBut == "settings")
    {
        welcomeWindow.SetButtonEnable(false);
        settingWindow.SetButtonEnable(true);
        settingWindow.SetVisible(true);
    }
    else if (chosenBut == "exit")
        _closing = true;

    chosenBut = startWindow.GetUI()->ChosenButton();

    if (chosenBut == "start")
    {
        welcomeWindow.SetButtonEnable(true);
        startWindow.SetButtonEnable(false);
        startWindow.SetVisible(false);
        startWindow.GetUI()->Reset();
        CAudio::Instance()->Stop(AUDIO_MENU_MUSIC);
        CAudio::Instance()->Play(AUDIO_CLICK_START);
        GotoGameState(GAME_STATE_RUN);
    }
    else if (chosenBut == "back")
    {
        welcomeWindow.SetButtonEnable(true);
        startWindow.SetButtonEnable(false);
        startWindow.SetVisible(false);
        startWindow.GetUI()->Reset();
    }
    else if (chosenBut == "aiNum")
    {
        _aiNum = (_aiNum + 1) % 7;
        stringstream ss;
        ss << _aiNum;
        (*startWindow.GetUI()->Index("aiNum"))->SetStr("How many AI : " + ss.str());
    }
    else if (chosenBut == "aiDiff")
    {
        _aiDiff = (_aiDiff + 1) % 5;
        (*startWindow.GetUI()->Index("aiDiff"))->SetStr("AI Difficulty : " + GetAiDifficulty(_aiDiff));
    }
    else if (chosenBut == "gameMode")
    {
        _gameMode = (_gameMode + 1) % 3;
        (*startWindow.GetUI()->Index("gameMode"))->SetStr("Game Mode : " + GetGameMode(_gameMode));
    }

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
    : CGameState(g)
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
    sprintf(str, "Game Over !");// (%d)", counter / 30);
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
    battleSystem.OnBeginState(_gameMode, _aiNum, _aiDiff);
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