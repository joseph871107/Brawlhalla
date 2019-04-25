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
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

//-----------------CONSTANTS DEFINITIONS-----------------//
const int MATCH_TIME = 180;
const int MAX_weapons = 5;
//-----------------FUNCTIONS DEFINITIONS-----------------//

<<<<<<< HEAD
const vector<GroundPARM> _groundsXY{ GroundPARM(0, 300, 1, 5), GroundPARM(0, 400, 1, 5), GroundPARM(0, 500, 1, 5), GroundPARM(500, 500, 1, 5), GroundPARM(1000, 500, 1, 5) };	// Define Ground position to automatically generate ground objects
=======
const vector<GroundPARM> _groundsXY{ GroundPARM(100, 200, 0.65, 5), GroundPARM(600, 400, 0.65, 5), GroundPARM(1100, 600, 0.65, 5) };	// Define Ground position to automatically generate ground objects
>>>>>>> 0c838080a8cabcf1728bea1928a5a5b80e938756
CInteger integer(2);																												// Used to show current remain time

BattleSystem::BattleSystem(CGame* g) : CGameState(g), background(Background()), _grounds(vector<Ground*>()), _players(vector<Player*>()), _weapons(vector<Weapon*>())
{
}

BattleSystem::~BattleSystem()
{
    ResolveMemoryLeaksOnEndState();

    for (auto element : _grounds)
    {
        delete element;
    }

    for (auto element : _players)
    {
        delete element;
    }
}

int random(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void BattleSystem::OnBeginState()
{
    /*------------------------------INIT PROGRESS STAGE 1------------------------------*/
	CAudio::Instance()->Play(IDS_BATTLE_MUSIC,true);
    start = lastTime = clock();
    nextTimeGenerateWeapon = random(3, 10);
    _weapons.clear();
    _secPerRound = MATCH_TIME;
    vector<GroundPARM> groundXY = _groundsXY;
    vector<vector<long>> playerKeys = { {KEY_W, KEY_D, KEY_S, KEY_A, KEY_C}, {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_COMMA} };

    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        char str[80];
        sprintf(str, "%d", i - _players.begin() + 1);
        (*i)->Initialize(_grounds, &_players, "Player " + (string)str, playerKeys[i - _players.begin()]);
    }
	camera.Reset();
}

void BattleSystem::OnMove()							// 移動遊戲元素
{
    // After certain amount of time, generates Weapon automatically //
    if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon)
    {
        if (_weapons.size() < MAX_weapons)
        {
            Weapon* weapon = new Weapon();
			weapon->AddCamera(&camera);
            weapon->Initialize(_grounds, _players);
            _weapons.push_back(weapon);
        }

        lastTime = clock();
        nextTimeGenerateWeapon = random(5, 8);
    }/////////////////////////////////////////////////////////////////

    for (auto weapon : _weapons)
    {
        weapon->OnMove();
    }
	for (auto player : _players)
	{
		player->OnMove();
	}
	for (auto i : _flyingWeapons)
		i->OnMove();
	vector<Weapon*>::iterator erase = _flyingWeapons.end();
	for (auto i = _flyingWeapons.begin(); i != _flyingWeapons.end(); i++)
	{
		if (!(*i)->BeThrowen())
			erase = i;
	}
	if (erase != _flyingWeapons.end())
	{
		delete (*erase); //Resolve memory leak of weapons
		_flyingWeapons.erase(erase);
	}
	ResizeCamera();
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
	InitializeNum();									// 初始化"resource.h"中點陣圖的資源編號
	InitializeNum("IDS");									// 初始化"resource.h"中音效的資源編號
    ShowInitProgress(13);
    /*------------------------------INIT PROGRESS STAGE 2------------------------------*/
	InitializeFile();									// 初始化"game.rc"中點陣圖的路徑
	InitializeFile("SOUND");									// 初始化"game.rc"中音效的路徑
    ShowInitProgress(25);

    /*------------------------------INIT PROGRESS STAGE 3------------------------------*/
    if (GENERATE_COLLISION_ARRAY)
    {
        InitializeCollideArray();							// 初始化所有點陣圖的布林碰撞矩陣
        TRACE("cArray size : %d\n", cArray.size());
        ShowInitProgress(50);
    }

    /*------------------------------INIT PROGRESS STAGE 4------------------------------*/
	InitializeLoadSound();
	CAudio::Instance()->Play(IDS_MENU_MUSIC, true);
    vector<GroundPARM> groundXY = _groundsXY;
	camera = Camera();
	camera.SetGradual(true);

    // Automatically generate ground objects //
    for (auto i = groundXY.begin(); i != groundXY.end(); i++)
    {
        Ground* ground = new Ground();
		ground->AddCamera(&camera);
        ground->LoadBitmap();
        ground->SetXY(i->point.x, i->point.y);
        ground->SetSize(i->_size);
        ground->SetLen(i->_length);
        _grounds.push_back(ground);
    }//////////////////////////////////////////

	background.AddCamera(&camera);
    background.LoadBitmap(IDB_BACKGROUND, RGB(0, 0, 0));
	background.SetXY( - background.GetWidth(),  - background.GetHeight());
    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
    Player* player = new Player();
    player->LoadBitmap();
	player->AddCamera(&camera);
    _players.push_back(player);				// Player1
    player = new Player();
    player->LoadBitmap();
	player->AddCamera(&camera);
    _players.push_back(player);				// Player2
    integer.LoadBitmap();					// time + life
    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        (*i)->OnKeyDown(nChar);
    }

	Player* player = _players[1];
	if (nChar == KEY_TROW && player->GetHoldWeapon()) {
		Weapon* weapon = new Weapon();
		weapon->AddCamera(&camera);
		weapon->Initialize(_grounds, _players);
		weapon->SetSize(0.04);
		bool dir = player->GetDirection();
		if (!dir)
			weapon->SetXY(player->GetCor(0) - 100, player->GetCor(1) + 10);
		else
			weapon->SetXY(player->GetCor(2) + 20, player->GetCor(1) + 10);
		weapon->Throw(player->GetDirection(), player);
		_flyingWeapons.push_back(weapon);
		player->SetHoldWeapon(false);
		player->ResetWeaponID();
	}

    // If player takes the weapon //
    vector<Weapon*>::iterator erase = _weapons.end();

    for (auto i = _weapons.begin(); i != _weapons.end(); i++)
    {
        (*i)->OnKeyDown(nChar);

		if ((*i)->HasTaken())
			erase = i;
    }

    if (erase != _weapons.end())
    {
        delete (*erase); //Resolve memory leak of weapons
        _weapons.erase(erase);
    }

    //////////////////////////////
    currentKeydown = nChar;
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == KEY_ESC) {								// Demo 關閉遊戲的方法
		CAudio::Instance()->Stop(IDS_BATTLE_MUSIC);
		GotoGameState(GAME_STATE_OVER);	// 關閉遊戲
	}

    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        (*i)->OnKeyUp(nChar);
    }
}

void BattleSystem::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
    mousePoint = point;
    // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void BattleSystem::OnShow()
{
    background.OnShow(0.15);
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
    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        (*i)->OnShow();
    }

    // Show weapon
    for (auto i = _weapons.begin(); i != _weapons.end(); i++)
    {
        (*i)->OnShow();
    }
	for (auto i : _flyingWeapons)
		i->OnShow();

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
        sprintf(str, "(%d, %d) KeyDown:%s", mousePoint.x, mousePoint.y, ("0x" + oss.str()).c_str());
        OnShowText(str, 0, 0, 10);

        for (auto i = _players.begin(); i != _players.end(); i++)
        {
            sprintf(str, "Player1 (x1:%d, y1:%d, x2:%d, y2:%d)", (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
            OnShowText(str, 0, 12 + 12 * (i - _players.begin()), 10);
        }

        for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
        {
            sprintf(str, "                      , Ground%d (x1:%d, y1:%d, x2:%d, y2:%d)", i - _grounds.begin(), (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
            OnShowText(str, 0, 12 + 12 * _players.size() + 12 * (i - _grounds.begin()), 10);
        }

        for (auto i = _players.begin(); i != _players.end(); i++)
        {
			int temp = (*i)->ShowAnimationState();
            sprintf(str, "%s", GetNameFromResource((*i)->ShowAnimationState()).c_str());
            OnShowText(str, 0, 12 + 12 * _players.size() + 12 * _grounds.size() + 12 * (i - _players.begin()), 10);
        }
    }

    //------------------End of Test Text------------------//
}

int t(int k, double kk) { return (int)(k * kk); }

void BattleSystem::ResizeCamera()
{
	if (CGameStateInit::GetCameraEnable()) {
		int totalX = 0, totalY = 0;
		for (auto i = _players.begin(); i != _players.end(); i++)
		{
			totalX += (*i)->GetCor(0);
			totalY += (*i)->GetCor(1);
		}
		int minX = totalX / (signed int)_players.size(), maxX = minX, \
			minY = totalY / (signed int)_players.size(), maxY = minY, \
			minWidth = 800, \
			paddingX = 500, paddingY = 300, \
			centerX = minX + (maxX - minX) / 2, centerY = minY + (maxY - minY) / 2;
		for (auto i = _players.begin(); i != _players.end(); i++)
		{
			minX = ((*i)->GetCor(0) < minX ? (*i)->GetCor(0) : minX);
			maxX = ((*i)->GetCor(2) > maxX ? (*i)->GetCor(2) : maxX);
			minY = ((*i)->GetCor(1) < minY ? (*i)->GetCor(1) : minY);
			maxY = ((*i)->GetCor(3) > maxY ? (*i)->GetCor(3) : maxY);
		}
		minX -= paddingX; maxX += paddingX; minY -= paddingY; maxY += paddingY;
		int width = (maxX - minX < minWidth ? minWidth : maxX - minX), height = maxY - minY;
		width = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? width : height * SIZE_X / SIZE_Y);
		height = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? height : width * SIZE_Y / SIZE_X);
		double sizeX = SIZE_X / (double)(width), sizeY = SIZE_Y / (double)(height);
		double size = (sizeX < sizeY ? sizeX : sizeY);
		camera.SetCameraXY(centerX, centerY);
		camera.SetSize(size);
	}
}

bool BattleSystem::IsGameOver()
{
    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        if ((*i)->IsOutOfLife())
            return true;
    }

    return (GetCurrentRemainTime() == 0);
}

string BattleSystem::GetGameResult()
{
    Player* max = *(_players.begin());
    bool draw = true;

    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        if ((*i)->GetLife() > max->GetLife())
        {
            max = *i;
        }
    }

    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        if ((*i)->GetLife() < max->GetLife())
        {
            draw = false;
        }
    }

    if (draw)
        return ("Draw.");
    else
        return (max->GetName() + " win.");
}

void BattleSystem::ResolveMemoryLeaksOnEndState()
{
    for (auto element : _weapons)
    {
        delete element;
    }

	_weapons.clear();
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
    return MATCH_TIME - (clock() - start) / CLOCKS_PER_SEC;
}

}