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
const int MAX_WEAPON = 5;
//-----------------FUNCTIONS DEFINITIONS-----------------//

const vector<GroundPARM> _groundXY{ GroundPARM(0, 300, 0.65, 5), GroundPARM(500, 400, 0.65, 5), GroundPARM(1000, 500, 0.65, 5) };	// Define Ground position to automatically generate ground objects
CInteger integer(2);																												// Used to show current remain time

BattleSystem::BattleSystem(CGame* g) : CGameState(g), background(Object()), _ground(vector<Ground*>()), _player(vector<Player*>()), _weapon(vector<Weapon*>())
{
}

BattleSystem::~BattleSystem()
{
}

int random(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void BattleSystem::OnBeginState()
{
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    /*------------------------------INIT PROGRESS STAGE 1------------------------------*/
	start = lastTime = clock();
	nextTimeGenerateWeapon = random(3,10);
	_weapon.clear();

	_secPerRound = MATCH_TIME;
	vector<GroundPARM> groundXY = _groundXY;
	for (auto i = _player.begin(); i != _player.end(); i++) {
		char str[80];
		sprintf(str, "%d", i - _player.begin() + 1);
		(*i)->Initialize(_ground, &_player, "Player " + (string)str, i - _player.begin() + 1);
	}
}

void BattleSystem::OnMove()							// 移動遊戲元素
{
	// After certain amount of time, generates Weapon automatically //
	if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon) {
		if (_weapon.size() < MAX_WEAPON) {
			Weapon* weapon = new Weapon();
			weapon->Initialize(_ground, _player);
			_weapon.push_back(weapon);
		}
		lastTime = clock();
		nextTimeGenerateWeapon = random(5, 8);
	}/////////////////////////////////////////////////////////////////

	for (auto i = _weapon.begin(); i != _weapon.end(); i++) {
		(*i)->OnMove();
	}

	for (auto i = _player.begin(); i != _player.end(); i++) {
		(*i)->OnMove();
	}
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
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

	// Automatically generate ground objects //
	for (auto i = groundXY.begin(); i != groundXY.end(); i++)
	{
		Ground* ground = new Ground();
		ground->LoadBitmap();
		ground->SetXY(i->point.x, i->point.y);
		ground->SetSize(i->_size);
		ground->SetLen(i->_length);
		_ground.push_back(ground);
	}//////////////////////////////////////////

    background.SetSize(1);
    background.SetXY(-250, 0);
    background.LoadBitmap(IDB_BACKGROUND, RGB(0, 0, 0));

    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
	
	Player* player = new Player();
	player->LoadBitmap();
	_player.push_back(player);				// Player1
	player = new Player();
	player->LoadBitmap();
	_player.push_back(player);				// Player2

    integer.LoadBitmap();				// time + life
    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	for (auto i = _player.begin(); i != _player.end(); i++) {
		(*i)->OnKeyDown(nChar);
	}
	
	// If player takes the weapon //
	vector<Weapon*>::iterator erase = _weapon.end();
	for (auto i = _weapon.begin(); i != _weapon.end(); i++) {
		(*i)->OnKeyDown(nChar);
		if ((*i)->HasTaken())
			erase = i;
	}
	if (erase != _weapon.end())
		_weapon.erase(erase);
	//////////////////////////////

    currentKeydown = nChar;
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;

    if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲

	for (auto i = _player.begin(); i != _player.end(); i++) {
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
    background.OnShow();
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
	// Show weapon
	for (auto i = _weapon.begin(); i != _weapon.end(); i++) {
		(*i)->OnShow();
	}

	// Show player
	for (auto i = _player.begin(); i != _player.end(); i++) {
		// Show player
		(*i)->OnShow();
		// Show player's life
		ShowPlayerLife((**i), 1200 + 200 * (i - _player.begin()), 0);
	}

	//------------------Test Text------------------//
	if (_PLAYER_DEBUG)
	{
		char str[80], str2[80];
		ostringstream oss;
		oss << hex << currentKeydown;
		sprintf(str, "(%d, %d) KeyDown:%s", mousePoint.x, mousePoint.y, ("0x" + oss.str()).c_str());
		OnShowText(str, 0, 0, 10);
		for (auto i = _player.begin(); i != _player.end(); i++) {
			sprintf(str, "Player1 (x1:%d, y1:%d, x2:%d, y2:%d)", (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
			OnShowText(str, 0, 12 + 12 * (i - _player.begin()), 10);
		}

		for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
		{
			sprintf(str, "                      , Ground%d (x1:%d, y1:%d, x2:%d, y2:%d)", i - _ground.begin(), (*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3));
			OnShowText(str, 0, 36 + 12 * (i - _ground.begin()), 10);
		}

		for (auto i = _player.begin(); i != _player.end(); i++) {
			sprintf(str, "%s", GetNameFromIDB((*i)->ShowAnimationState()).c_str());
			OnShowText(str, 0 + 200 * (i - _player.begin()), SIZE_Y - 24, 20);
		}
		sprintf(str, "Remain Time : %s : %s\n", (GetCurrentRemainTime() / 600 == 0 ? ("0" + (string)str).c_str() : str), ((GetCurrentRemainTime() % 60) / 10 == 0 ? ("0" + (string)str2).c_str() : str2));
		OnShowText(str, 650, 0, 30);
	}

	//------------------End of Test Text------------------//
}

bool BattleSystem::IsGameOver()
{
	for (auto i = _player.begin(); i != _player.end(); i++) {
		if ((*i)->IsOutOfLife())
			return true;
	}
    return (GetCurrentRemainTime() == 0);
}

string BattleSystem::GetGameResult()
{
	Player* max = *(_player.begin());
	for (auto i = _player.begin(); i != _player.end(); i++) {
		if ((*i)->GetLife() > max->GetLife())
			max = *i;
	}
	if (max != nullptr)
		return (max->GetName() + " win.");
	else
        return ("Draw.");
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