#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "battleSystem.h"
#include "map.h"

namespace game_framework
{

/////////////////////////////////////////////////////////////////////////////
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

//-----------------CONSTANTS DEFINITIONS-----------------//
const int MATCH_TIME = 180;
const int MAX_weapons = 5;
//-----------------FUNCTIONS DEFINITIONS-----------------//

/// DEBUG
CInteger integer(2);																												// Used to show current remain time

BattleSystem::BattleSystem(CGame* g) : CGameState(g)
{
	map = nullptr;
	background = &map->background;
	_grounds = map->GetGrounds();
	_weapons = map->GetWeapons();
}

BattleSystem::BattleSystem(CGame * g, shared_ptr<Map> m) : CGameState(g)
{
	map = m;
}

BattleSystem::~BattleSystem()
{
	ClearPlayers();
}

void BattleSystem::OnBeginState()
{
	/*------------------------------INIT PROGRESS STAGE 1------------------------------*/
	ClearPlayers();
	map = CGameStateInit::GetMap();
	_grounds = map->GetGrounds();
	_weapons = map->GetWeapons();
	CAudio::Instance()->Play(IDS_BATTLE_MUSIC, true);
	_secPerRound = MATCH_TIME;

	camera = Camera();
	camera.SetGradual(true);
	map->AddCamera(&camera);
	for (auto ground : *_grounds)
		ground->AddCamera(&camera);

	background = &map->background;
	background->AddCamera(&camera);

	Player* player = new Player();
	player->LoadBitmap();
	player->AddCamera(&camera);
	_players.push_back(player);				// Player1
	player = new Player();
	player->LoadBitmap();
	player->AddCamera(&camera);
	_players.push_back(player);				// Player2
	map->AddPlayers(&_players);

	vector<vector<long>> playerKeys = { {KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_F, KEY_X}, {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_COMMA, KEY_PERIOD, KEY_M} };
	for (auto i = _players.begin(); i != _players.end(); i++)
	{
		char str[80];
		sprintf(str, "%d", i - _players.begin() + 1);
		(*i)->Initialize(*_grounds, &_players, "Player " + (string)str, playerKeys[i - _players.begin()]);
	}
}

void BattleSystem::OnMove()							// 移動遊戲元素
{
	map->OnMove();
    for (auto player : _players)
    {
        player->OnMove();
    }

    ResizeCamera();
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
    InitializeNum();										// 初始化"resource.h"中點陣圖的資源編號
    InitializeNum("IDS");									// 初始化"resource.h"中音效的資源編號
    ShowInitProgress(13);
    /*------------------------------INIT PROGRESS STAGE 2------------------------------*/
    InitializeFile();										// 初始化"game.rc"中點陣圖的路徑
    InitializeFile("SOUND");								// 初始化"game.rc"中音效的路徑
    ShowInitProgress(25);
    /*------------------------------INIT PROGRESS STAGE 3------------------------------*/
    if (GENERATE_COLLISION_ARRAY)
    {
        InitializeCollideArray();							// 初始化所有點陣圖的布林碰撞矩陣
        TRACE("cArray size : %d\n", cArray.size());
    }
    /*------------------------------INIT PROGRESS STAGE 4------------------------------*/
    InitializeLoadSound();
    CAudio::Instance()->Play(IDS_MENU_MUSIC, true);
	ShowInitProgress(50);

    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
    integer.LoadBitmap();					// time + life
    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	map->OnKeyDown(nChar);
    for (auto i : _players)
    {
        i->OnKeyDown(nChar);
    }
    currentKeydown = nChar;
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == KEY_ESC)  								// Demo 關閉遊戲的方法
    {
        CAudio::Instance()->Stop(IDS_BATTLE_MUSIC);
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲
    }

    for (auto i : _players)
    {
        i->OnKeyUp(nChar);
    }
}

void BattleSystem::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void BattleSystem::OnShow()
{
	map->OnShow();

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

    // Show player
    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        // Show player
        (*i)->OnShow();
        // Show player's life
        ShowPlayerLife((**i), 1200 + 200 * (i - _players.begin()), 0);
    }

    //------------------Test Text------------------//
    if (_PLAYER_DEBUG)
    {
        char str[80];
        ostringstream oss;
        oss << hex << currentKeydown;
        sprintf(str, "KeyDown:%s", ("0x" + oss.str()).c_str());
        OnShowText(str, 0, 0, 10, RGB(255, 255, 255), RGB(0, 0, 0));

        for (auto i = _players.begin(); i != _players.end(); i++)
        {
            sprintf(str, "%s (x1:%d, y1:%d, x2:%d, y2:%d)", (*i)->GetName().c_str(), (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
            OnShowText(str, 0, 12 + 12 * (i - _players.begin()), 10, RGB(255, 255, 255), RGB(0, 0, 0));
        }
    }

    //------------------End of Test Text------------------//
}

void BattleSystem::ResizeCamera()
{
    if (CGameStateInit::GetCameraEnable())
    {
        int totalX = 0, totalY = 0;
        for (auto i : _players) // Find average position among players
        {
            totalX += i->GetCor(0);
            totalY += i->GetCor(1);
        }
        int minX = totalX / (signed int)_players.size(), maxX = minX, minY = totalY / (signed int)_players.size(), maxY = minY, minWidth = 800, paddingX = 500, paddingY = 300, centerX = minX + (maxX - minX) / 2, centerY = minY + (maxY - minY) / 2;
        for (auto i : _players) // Find max and minimum position among players
        {
            minX = (i->GetCor(0) < minX ? i->GetCor(0) : minX);
            maxX = (i->GetCor(2) > maxX ? i->GetCor(2) : maxX);
            minY = (i->GetCor(1) < minY ? i->GetCor(1) : minY);
            maxY = (i->GetCor(3) > maxY ? i->GetCor(3) : maxY);
        }
        minX -= paddingX; maxX += paddingX; // Enlarge view horizentally
        minY -= paddingY; maxY += paddingY; // Enlarge view vertically
        int width = (maxX - minX < minWidth ? minWidth : maxX - minX), height = maxY - minY;
        width = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? width : height * SIZE_X / SIZE_Y);
        height = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? height : width * SIZE_Y / SIZE_X);
        double sizeX = SIZE_X / (double)(width), sizeY = SIZE_Y / (double)(height);
        double size = (sizeX < sizeY ? sizeX : sizeY);
        camera.SetCameraXY(centerX, centerY);
        camera.SetSize(size);
    }
}

void BattleSystem::AddMap(shared_ptr<Map> m)
{
	map = m;
}

bool BattleSystem::IsGameOver()
{
    for (auto i : _players)
		if (i->IsOutOfLife())
            return true;
    return (GetCurrentRemainTime() == 0);
}

string BattleSystem::GetGameResult()
{
    Player* max = *(_players.begin());
    bool draw = true;

    for (auto i : _players)
        if (i->GetLife() > max->GetLife())
            max = i;

    for (auto i : _players)
        if (i->GetLife() < max->GetLife())
            draw = false;

    if (draw)
        return ("Draw.");
    else
        return (max->GetName() + " win.");
}

void BattleSystem::ClearPlayers()
{
	for (auto element : _players)
	{
		delete element;
	}
	_players.clear();
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
    return MATCH_TIME - map->GetTime();
}

}