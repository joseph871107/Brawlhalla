#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "brawlhalla.h"
#include "battleSystem.h"
#include "enemy.h"
#include "boss.h"
#include "map.h"
#include "UIMessage.h"

namespace game_framework
{

/////////////////////////////////////////////////////////////////////////////
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

vector< vector<CMovingBitmap>> louis_l0;
vector< vector<CMovingBitmap>> louis_r0;
vector< vector<CMovingBitmap>> louis_l1;
vector< vector<CMovingBitmap>> louis_r1;
vector< vector<CMovingBitmap>> louis_l2;
vector< vector<CMovingBitmap>> louis_r2;
vector< vector<CMovingBitmap>> louis_ex_l0;
vector< vector<CMovingBitmap>> louis_ex_r0;
vector< vector<CMovingBitmap>> louis_ex_l1;
vector< vector<CMovingBitmap>> louis_ex_r1;
vector< vector<CMovingBitmap>> louis_ex_l2;
vector< vector<CMovingBitmap>> louis_ex_r2;
vector< vector<CMovingBitmap>> louis_ex_l3;
vector< vector<CMovingBitmap>> louis_ex_r3;
vector< vector<CMovingBitmap>> julian_l0;
vector< vector<CMovingBitmap>> julian_r0;
vector< vector<CMovingBitmap>> julian_l1;
vector< vector<CMovingBitmap>> julian_r1;
vector< vector<CMovingBitmap>> julian_l2;
vector< vector<CMovingBitmap>> julian_r2;

//-----------------CONSTANTS DEFINITIONS-----------------//
const int MATCH_TIME = 180;

//-----------------FUNCTIONS DEFINITIONS-----------------//
bool enemyPause = false;
CInteger integer(2); // Used to show current remain time
CString mString;

BattleSystem::BattleSystem(CGame* g) : CGameState(g)
{
    map = nullptr;
    background = &map->background;
    _grounds = map->GetGrounds();
    _weapons = map->GetDroppingWeapons();
}

BattleSystem::BattleSystem(CGame* g, shared_ptr<Map> m) : CGameState(g)
{
    map = m;
}

BattleSystem::~BattleSystem()
{
    ClearPlayers();
    ClearExplosionEffects();
    ClearUIMessages();
}

void BattleSystem::ClearExplosionEffects()
{
    for (auto elementPtr : _explosionEffects)
        delete elementPtr;

    _explosionEffects.clear();
}

void BattleSystem::InitializeExplosionEffectsOnBeginState()
{
    ExplosionEffect* explosionEffectPtr;

    for (auto player : _players)
    {
        explosionEffectPtr = new ExplosionEffect();
        explosionEffectPtr->SetIsTrigger(false); // required
        explosionEffectPtr->LoadBitmap();
        explosionEffectPtr->AddCamera(&camera);
        _explosionEffects.push_back(explosionEffectPtr);
    }
}

void BattleSystem::InitializePlayersOnBeginState(int mode, int enemy, int diff)
{
    Player* player;
	vector<vector<long>> playerKeys ={{KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_COMMA, KEY_PERIOD, KEY_M}};
	// Player
	player = new Player();
	_players.push_back(player);
	_mode = mode;
	if (mode == GAME_MODE_PVP) {
		// Player2
		player = new Player();
		_players.push_back(player);

		// Initialize keys for player2
		playerKeys.push_back({ KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_V, KEY_X });
	}
	else if (mode == GAME_MODE_PVC) {
		// Enemy
		for (int i = 0; i < enemy; i++)
		{
			player = new Enemy(diff);
			_players.push_back(player);
		}
	}
	else if (mode == GAME_MODE_BOSS) {
		// Enemy
		for (int i = 0; i < enemy; i++)
		{
			player = new Enemy(diff);
			_players.push_back(player);
		}
		// Boss
		player = new Boss(diff);
		player->SetRespawn(false);
		for (auto i : _players)
			i->SetAttackList(vector<Player*>{player});
		_players.push_back(player);
	}

	for (int i = 0; i < enemy + (mode == GAME_MODE_BOSS ? 1 : 0); i++)
		playerKeys.push_back({ KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_V, KEY_X });
    // Initialize explosion effects for every players
    InitializeExplosionEffectsOnBeginState();
    // Initialize other attributes of the players
    int pNum = 1, eNum = 1;

    for (auto i = _players.begin(); i != _players.end(); i++)
    {
        //
        (*i)->LoadBitmap();
        (*i)->AddCamera(&camera);
        (*i)->weapons = map->GetDroppingWeapons();
        //
        char str[80];
		switch ((*i)->GetPlayerMode()) {
		case PLAYER_MODE_PLAYER:
			sprintf(str, "Player %d", pNum++);
			(*i)->Initialize(this, *_grounds, &_players, (string)str, playerKeys[i - _players.begin()], _explosionEffects[i - _players.begin()]);
			break;
		case PLAYER_MODE_ENEMY:
			sprintf(str, "AI %d", eNum++);
			(*i)->Initialize(this, *_grounds, &_players, (string)str, playerKeys[i - _players.begin()], _explosionEffects[i - _players.begin()]);
			break;
		case PLAYER_MODE_BOSS:
			(*i)->Initialize(this, *_grounds, &_players, "BOSS", playerKeys[i - _players.begin()], _explosionEffects[i - _players.begin()]);
			break;
		}
    }

    //
    map->AddPlayers(&_players);
}

void BattleSystem::OnBeginState(int mode, int enemy, int diff)
{
    /*	~ Remark:
    	~ This function is called whenever the game state turns into 'CGameStateRun';
    	~ thus, it serves as the initialization of each game match
    */
    // Clear the players, explosion effects, and UI messages from the last match
    ClearPlayers();
    ClearExplosionEffects();
    ClearUIMessages();
    // Initialize the match
    CAudio::Instance()->Play(AUDIO_BATTLE_MUSIC, true);
    _secPerRound = MATCH_TIME;
    //
    map = CGameStateInit::GetMap();
    _grounds = map->GetGrounds();
    _weapons = map->GetDroppingWeapons();
    camera = Camera();
    camera.SetGradual(true);
    map->AddCamera(&camera);

    for (auto ground : *_grounds)
        ground->AddCamera(&camera);

    background = &map->background;
    background->AddCamera(&camera);
    // Player
    InitializePlayersOnBeginState(mode, enemy, diff);
    // UI Messages
    _uiMessages = vector<UIMessage*>();
	//
	EasterEgg(0);
}

void BattleSystem::GetExplosionEffectPosition(Player* deadPlayer, int* posXPtr, int* posYPtr)
{
    /*	~ Remark:
    	~ Set the effect position based on the selected current animation for '_explosionEffect'
    */
    ExplosionEffect* explosionEffectPtr = deadPlayer->GetExplosionEffect();
    int playerX1 = deadPlayer->GetCor(0);
    int playerY1 = deadPlayer->GetCor(1);
    int playerX2 = deadPlayer->GetCor(2);
    int playerY2 = deadPlayer->GetCor(3);
    // Offset variables that makes the position more realistic
    int offsetY = 50;
    int offsetX = 50;

    switch (explosionEffectPtr->GetCurrentAni())
    {
        case ExplosionEffect::ANI_ID_EXPLO_UP:
            *posXPtr = DoubleToInteger(playerX1 + deadPlayer->GetWidth() / 2.0 - explosionEffectPtr->GetCurrentAnimationWidth() / 2.0);
            *posYPtr = 0 - offsetY;
            break;

        case ExplosionEffect::ANI_ID_EXPLO_RIGHT:
            *posXPtr = DoubleToInteger(SIZE_X - explosionEffectPtr->GetCurrentAnimationWidth() - offsetX);
            *posYPtr = DoubleToInteger(playerY1 + deadPlayer->GetHeight() / 2.0 - explosionEffectPtr->GetCurrentAnimationHeight() / 2.0);
            break;

        case ExplosionEffect::ANI_ID_EXPLO_DOWN:
            *posXPtr = DoubleToInteger(playerX1 + deadPlayer->GetWidth() / 2.0 - explosionEffectPtr->GetCurrentAnimationWidth() / 2.0);
            *posYPtr = DoubleToInteger(SIZE_Y - explosionEffectPtr->GetCurrentAnimationHeight() - offsetY);
            break;

        case ExplosionEffect::ANI_ID_EXPLO_LEFT:
            *posXPtr = 0 - offsetX;
            *posYPtr = DoubleToInteger(playerY1 + deadPlayer->GetHeight() / 2.0 - explosionEffectPtr->GetCurrentAnimationHeight() / 2.0);
            break;

        default:
            break;
    }

    // Confine posX and posY to the map
    if (*posXPtr < 0 - offsetX)
        *posXPtr = 0 - offsetX;
    else if (*posXPtr > SIZE_X)
        *posXPtr = DoubleToInteger(SIZE_X - explosionEffectPtr->GetCurrentAnimationWidth());

    if (*posYPtr < 0 - offsetY)
        *posYPtr = 0 - offsetY;
    else if (*posYPtr > SIZE_Y)
        *posYPtr = DoubleToInteger(SIZE_Y - explosionEffectPtr->GetCurrentAnimationHeight());
}

void BattleSystem::TriggerExplosionEffect(Player* deadPlayer)
{
    ExplosionEffect* explosionEffectPtr = deadPlayer->GetExplosionEffect();
    // Trigger the explosion effect
    explosionEffectPtr->SetIsTrigger(true);
    // Set the proper explosion effect direction
    int playerX = deadPlayer->GetCor(0);
    int playerY = deadPlayer->GetCor(1);
    int distanceError = 200;

    if (abs(playerY - MAP_BORDER_Y1) < distanceError)
        explosionEffectPtr->SetCurrentAni(ExplosionEffect::ANI_ID_EXPLO_UP);
    else if (abs(playerY - MAP_BORDER_Y2) < distanceError)
        explosionEffectPtr->SetCurrentAni(ExplosionEffect::ANI_ID_EXPLO_DOWN);
    else if (abs(playerX - MAP_BORDER_X1) < distanceError)
        explosionEffectPtr->SetCurrentAni(ExplosionEffect::ANI_ID_EXPLO_LEFT);
    else if (abs(playerX - MAP_BORDER_X2) < distanceError)
        explosionEffectPtr->SetCurrentAni(ExplosionEffect::ANI_ID_EXPLO_RIGHT);
    else
    {
        /* Do nothing */
    }

    // Set the position of the explosion
    int posX, posY;
    GetExplosionEffectPosition(deadPlayer, &posX, &posY);
    explosionEffectPtr->SetXY(posX, posY);
    // Play explosion effect
    CAudio::Instance()->Play(AUDIO_EXPLOSION_FX);
}


void BattleSystem::OnMove()							// 移動遊戲元素
{
    map->OnMove();

    for (auto playerPtr : _players)
    {
		switch (playerPtr->GetPlayerMode()) {
		case PLAYER_MODE_PLAYER:
			playerPtr->OnMove();
			break;
		case PLAYER_MODE_ENEMY:
			if (!enemyPause)
				playerPtr->OnMove();
			break;
		case PLAYER_MODE_BOSS:
			playerPtr->OnMove();
			break;
		}
    }

    ResizeCamera();

    // Explosion Effect
    for (auto elementPtr : _explosionEffects)
        elementPtr->OnMove();
}

void BattleSystem::LoadSoundOnInit()
{
    CAudio::Instance()->Load(AUDIO_BATTLE_MUSIC, "sounds\\BattleMusic.mp3");
    CAudio::Instance()->Load(AUDIO_DRAW_WEAPON, "sounds\\DrawWeapon.mp3");
    CAudio::Instance()->Load(AUDIO_MENU_MUSIC, "sounds\\Menu.mp3");
    CAudio::Instance()->Load(AUDIO_PUNCH, "sounds\\Punch.mp3");
    CAudio::Instance()->Load(AUDIO_SWING_ATTACK, "sounds\\SwingAttack.mp3");
    CAudio::Instance()->Load(AUDIO_SWOOSH, "sounds\\Swoosh.mp3");
    CAudio::Instance()->Load(AUDIO_EXPLOSION_FX, "sounds\\ExplosionFX.mp3");
    CAudio::Instance()->Load(AUDIO_CLICK_START, "sounds\\ClickStart.mp3");
}

void BattleSystem::OnInit()  								// 遊戲的初值及圖形設定
{
    /*------------------------------INIT PROGRESS STAGE 3------------------------------*/
    if (GENERATE_COLLISION_ARRAY)
    {
        InitializeCollideArray();							// 初始化所有點陣圖的布林碰撞矩陣
        TRACE("cArray size : %d\n", cArray.size());
    }

    /*------------------------------INIT PROGRESS STAGE 4------------------------------*/
    LoadSoundOnInit(); // Load all the soundtracks required
    CAudio::Instance()->Play(AUDIO_MENU_MUSIC, true);
    ShowInitProgress(50);
    ShowInitProgress(75);
    /*------------------------------INIT PROGRESS STAGE 5------------------------------*/
    // Integer (for displaying time and life count)
    integer.LoadBitmap();
    // String
    mString.LoadBitmap();
    // Taken damages
    takenDmgR.LoadBitmap(IDB_TAKEN_DMG_RED, RGB(0, 0, 0));
    takenDmgY.LoadBitmap(IDB_TAKEN_DMG_YELLOW, RGB(0, 0, 0));
    takenDmgG.LoadBitmap(IDB_TAKEN_DMG_GREEN, RGB(0, 0, 0));
	chang = CAnimation(true);
	chang.AddBitmap(IDB_CHANG, RGB(0, 0, 0));
	chang.AddBitmap(IDB_CHANG_OPEN, RGB(0, 0, 0));
    // Explosion Effects
    _explosionEffects = vector<ExplosionEffect*>();
    // Player
    struct cropPARM
    {
        cropPARM(vector<vector<CMovingBitmap>>* _target, int _idbRes, int _row, int _col, bool _flip)
        {
            target = _target;
            idbRes = _idbRes;
            row = _row;
            col = _col;
            flip = _flip;
        }
        vector<vector<CMovingBitmap>>* target;
        int idbRes, row, col;
        bool flip;
    };
    vector<cropPARM> cropPARMs =
    {
        cropPARM(&louis_l0, IDB_P_LOUIS_L0, 7, 10, true),
        cropPARM(&louis_r0, IDB_P_LOUIS_R0, 7, 10, false),
        cropPARM(&louis_l1, IDB_P_LOUIS_L1, 7, 10, true),
        cropPARM(&louis_r1, IDB_P_LOUIS_R1, 7, 10, false),
        cropPARM(&louis_l2, IDB_P_LOUIS_L2, 5, 10, true),
        cropPARM(&louis_r2, IDB_P_LOUIS_R2, 5, 10, false),
        cropPARM(&louis_ex_l0, IDB_P_LOUIS_EX_L0, 7, 10, true),
        cropPARM(&louis_ex_r0, IDB_P_LOUIS_EX_R0, 7, 10, false),
        cropPARM(&louis_ex_l1, IDB_P_LOUIS_EX_L1, 7, 10, true),
        cropPARM(&louis_ex_r1, IDB_P_LOUIS_EX_R1, 7, 10, false),
        cropPARM(&louis_ex_l2, IDB_P_LOUIS_EX_L2, 2, 10, true),
        cropPARM(&louis_ex_r2, IDB_P_LOUIS_EX_R2, 2, 10, false),
        cropPARM(&louis_ex_l3, IDB_P_LOUIS_EX_L3, 3, 6, true),
        cropPARM(&louis_ex_r3, IDB_P_LOUIS_EX_R3, 3, 6, false),
        cropPARM(&julian_l0, IDB_P_JULIAN_L0, 5, 10, true),
        cropPARM(&julian_r0, IDB_P_JULIAN_R0, 5, 10, false),
        cropPARM(&julian_l1, IDB_P_JULIAN_L1, 2, 7, true),
        cropPARM(&julian_r1, IDB_P_JULIAN_R1, 2, 7, false),
        cropPARM(&julian_l2, IDB_P_JULIAN_L2, 5, 10, true),
        cropPARM(&julian_r2, IDB_P_JULIAN_R2, 5, 10, false)
    };
    int progS = 75, progE = 100;

    for (int i = 0; i < (signed int)cropPARMs.size(); i++)
    {
        *(cropPARMs[i].target) = CropSprite(cropPARMs[i].idbRes, cropPARMs[i].row, cropPARMs[i].col, RGB(0, 0, 0));

        if (cropPARMs[i].flip)
            FlipSprite(cropPARMs[i].target);

        ShowInitProgress((progE - progS) / cropPARMs.size() * i + progS);
    }

    ShowInitProgress(100);
}

void BattleSystem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	map->OnKeyDown(nChar);

	for (auto i : _players)
	{
		if (i->GetPlayerMode() == PLAYER_MODE_PLAYER)
			i->OnKeyDown(nChar);
	}

	currentKeydown = nChar;

	static string combo = "";
	if (comboCounter > 10)
		combo = "";
	if (keyDebounce) {
		combo += (nChar == 0x4f ? 'O' : nChar == 0x50 ? 'P' : nChar == 0x55 ? 'U' : (char)nChar);
		keyDebounce = false;
		comboCounter = 0;
	}
	if (combo == "OOP")
		EasterEgg(150);
	if (combo == "PP")
		enemyPause = !enemyPause;
	if (combo == "UP")
		for (auto player : _players)
			if (player->GetPlayerMode() == PLAYER_MODE_PLAYER) {
				player->SetSize(5);
				player->Initialize(this, *_grounds, &_players, player->GetName(), player->GetKeys(), player->GetExplosionEffect());
			}
	TRACE("%s\n",combo.c_str());
}

void BattleSystem::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == KEY_ESC)  								// Demo 關閉遊戲的方法
    {
        CAudio::Instance()->Stop(AUDIO_BATTLE_MUSIC);
        GotoGameState(GAME_STATE_OVER);	// 關閉遊戲
    }

    for (auto i : _players)
    {
        if (i->GetPlayerMode() == PLAYER_MODE_PLAYER)
            i->OnKeyUp(nChar);
    }
	keyDebounce = true;
}

void BattleSystem::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void BattleSystem::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void BattleSystem::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void BattleSystem::TriggerDisplayMessage(const string& message, const int& posX, const int& posY, const int& durationByFrame)
{
    UIMessage* ptr = new UIMessage();
    ptr->Initialize(message, posX, posY, durationByFrame);
    _uiMessages.push_back(ptr);
}

void BattleSystem::RemoveFinishedDisplayMessages()
{
    int leftIndex = 0;
    int rightIndex = _uiMessages.size() - 1;

    // Re-accomodate the pointers to finished messages into the end of the vector '_uiMessages'
    while (leftIndex <= rightIndex)
    {
        while ((leftIndex <= rightIndex) && (_uiMessages[leftIndex]->GetCurrentFrame() >= _uiMessages[leftIndex]->GetMaxFrame()))
        {
            /* _uiMessages[leftIndex] <-> _uiMessages[rightIndex] */
            iter_swap(_uiMessages.begin() + leftIndex, _uiMessages.begin() + rightIndex);
            rightIndex--;
        }

        leftIndex++;
    }

    unsigned int position = 100;

    for (unsigned int index = 0; index < _uiMessages.size(); index++)
    {
        if (_uiMessages[index]->GetCurrentFrame() >= _uiMessages[index]->GetMaxFrame())
        {
            position = index;
            break;
        }
    }

    if (position != 100) // If there is at least a message having finished its showcase
    {
        for (unsigned int index = position; index < _uiMessages.size(); index++)
        {
            delete _uiMessages[position]; // Free the memory allocated by the messages that has finished displaying
        }

        _uiMessages.erase(_uiMessages.begin() + position, _uiMessages.begin() + _uiMessages.size()); // Erase the finished message in the '_uiMessages' vector
    }
}

void BattleSystem::ClearUIMessages()
{
    for (auto elementPtr : _uiMessages)
        delete elementPtr;

    _uiMessages.clear();
}

void BattleSystem::EasterEgg(int set)
{
	static int counter = 0;
	static int _set = 0;
	if (set != -1) {
		counter = set;
		_set = set;
	}
	if (counter > 0) {
		double size = (_set - counter) / 5.0;
		chang.SetSize(size);
		int centerX = SIZE_X / 2, centerY = SIZE_Y / 2, width = chang.Width(), height = chang.Height();
		int actWidth = (int)(60 * size), actHeight = (int)(80 * size);
		CPoint cam = camera.GetReverseXY(centerX, centerY + (int)(26 * size));
		if (_set - counter > 0) {
			chang.SetTopLeft(centerX - width / 2, centerY - height / 2);
			for (auto player : _players) {
				CPoint tcam = camera.GetXY(player->GetCor(0), player->GetCor(1));
				int x = tcam.x, y = tcam.y;
				if (player->GetPlayerMode() != PLAYER_MODE_PLAYER && IsCollide(x, y, x + (int)(player->GetWidth() * camera.GetSize()), y + (int)(player->GetHeight() * camera.GetSize()), centerX - actWidth / 2, centerY - actHeight / 2, centerX + actWidth / 2, centerY + actHeight / 2))
				{
					player->SetAttacker(_players[0], 0);
					// Determine the attack "direction", which is a 2D Vector
					Vector2 vectorAttackerToTargetPlayer;
					vectorAttackerToTargetPlayer.SetXY(centerX, centerY, player->GetCor(0), player->GetCor(1));
					// Increment the taken damage of the target player
					player->SetTakenDmg(100);
					// Set the offset magnitude of the attack 2D Vector
					int attackOffsetMagnitude = player->GetSpecializedTakenDamage();
					// Determine the attack 2D vector
					double multiplier = (vectorAttackerToTargetPlayer.GetLength() == 0 ? attackOffsetMagnitude : attackOffsetMagnitude / vectorAttackerToTargetPlayer.GetLength()); // Avoid division by 0
					Vector2 targetPlayerDisplacementVector(DoubleToInteger(vectorAttackerToTargetPlayer.GetX() * multiplier),
						DoubleToInteger(vectorAttackerToTargetPlayer.GetY() * multiplier));
					//
					player->BeenAttacked(targetPlayerDisplacementVector, !player->GetDirection());
				}
			}
			chang.OnMove();
			chang.OnShow();
			for(int i = 0; i < 5; i++)
				OnShowText("爽一下啊！", centerX + (int)(random(-100, 100) * size), centerY + (int)(random(-100, 100) * size), (int)(size * 10), RGB(random(0, 255), random(0, 255), random(0, 255)));
		}
		if (counter % 3 == 0)
			counter -= 2;
		else
			counter--;
	}
}

void BattleSystem::OnShow()
{
    map->OnShow();
    /* DISPLAY MATCH TIME */
    // Display minute
    int now_time = GetCurrentRemainTime();
    integer.SetInteger(now_time / 60);
    integer.SetTopLeft(735, 0);
    integer.ShowBitmap();
    // Display second
    integer.SetInteger(now_time % 60);
    integer.SetTopLeft(805, 0);
    integer.ShowBitmap();

	comboCounter++;
	EasterEgg();

    // Show player
    for (unsigned int index = 0; index < _players.size(); index++)
    {
        if (!_players[index]->IsOutOfLife())
            _players[index]->OnShow();

        ShowPlayerLife((*_players[index]), 1300, 100 * index);
    }

    // Explosion Effect
    for (auto elementPtr : _explosionEffects)
        elementPtr->OnShow();

    // Display UI Message
    for (auto elementPtr : _uiMessages)
    {
        if (elementPtr->GetCurrentFrame() < elementPtr->GetMaxFrame())
        {
            elementPtr->IncrementCurrentFrame();
            // Display the message
            mString.SetString(elementPtr->GetMessage());
            mString.SetSize(1.0);
            mString.SetTopLeft(elementPtr->GetX(), elementPtr->GetY());
            mString.ShowBitmap();
            // Avoid conflicts of UI messages with a break
            break;
        }
    }

    RemoveFinishedDisplayMessages();

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

        int minX = totalX / (signed int)_players.size();
        int maxX = minX;
        int minY = totalY / (signed int)_players.size();
        int maxY = minY;
        int minWidth = 800;
        int maxWidth = 2000;
        int paddingX = 500;
        int paddingY = 300;
        int offsetX = 100;
        int minPlayerX = -offsetX;
        int maxPlayerX = SIZE_X + offsetX;
        int offsetY = 200;
        int minPlayerY = -offsetY;
        int maxPlayerY = SIZE_Y + offsetY;

        for (auto i : _players) // Find max and minimum position among players
        {
            minX = (i->GetCor(0) < minX && i->GetCor(0) > minPlayerX ? i->GetCor(0) : minX);
            maxX = (i->GetCor(2) > maxX && i->GetCor(2) < maxPlayerX ? i->GetCor(2) : maxX);
            minY = (i->GetCor(1) < minY && i->GetCor(1) > minPlayerY ? i->GetCor(1) : minY);
            maxY = (i->GetCor(3) > maxY && i->GetCor(3) < maxPlayerY ? i->GetCor(3) : maxY);
        }

        minX -= paddingX;
        maxX += paddingX; // Enlarge view horizentally
        minY -= paddingY;
        maxY += paddingY; // Enlarge view vertically
        // Constraint the camera's x-coordinate
        minX = (minX < 0) ? 0 : minX;
        maxX = (maxX > SIZE_X) ? SIZE_X : maxX;
        //
        int width = (maxX - minX < minWidth ? minWidth : maxX - minX > maxWidth ? maxWidth : maxX - minX);
        int height = maxY - minY;
        width = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? width : height * SIZE_X / SIZE_Y);
        height = (SIZE_X / (double)(width) < SIZE_Y / (double)(height) ? height : width * SIZE_Y / SIZE_X);
        double sizeX = SIZE_X / (double)(width);
        double sizeY = SIZE_Y / (double)(height);
        double size = (sizeX < sizeY ? sizeX : sizeY);
        camera.SetSize(size);
        //
        int centerX = minX + (maxX - minX) / 2;
        int centerY = minY + (maxY - minY) / 2;
        camera.SetCameraXY(centerX, centerY - 100);
    }
}

void BattleSystem::AddMap(shared_ptr<Map> m)
{
    map = m;
}

int BattleSystem::GetNumberOfRemainingPlayers()
{
    int remainingPlayers = 0;

    for (auto playerPtr : _players)
        if (!playerPtr->IsOutOfLife())
            remainingPlayers++;

    return (remainingPlayers);
}

bool BattleSystem::IsFinishedPlayingAllEffects()
{
    for (auto explosionEffectPtr : _explosionEffects)
        if (explosionEffectPtr->GetIsTrigger())
            return (false);
    return (true);
}

bool BattleSystem::IsGameOver()
{
    int remainingPlayers = GetNumberOfRemainingPlayers();
	Boss* boss = nullptr;
	for (auto player : _players)
		if (player->GetPlayerMode() == PLAYER_MODE_BOSS)
			boss = (Boss*)player;
    return (
               (IsFinishedPlayingAllEffects() && remainingPlayers == 1) // There is a winner
               ||
               remainingPlayers == 0 // Draw
               ||
               GetCurrentRemainTime() == 0 // Out of time - Draw
			   ||
			   (boss != nullptr && boss->GetHealth() <= 0)
           );
}

string BattleSystem::GetGameResult()
{
    int remainingPlayers = GetNumberOfRemainingPlayers();

    if (remainingPlayers == 1) // There is a winner
    {
        string winnerName;

        for (auto playerPtr : _players)
        {
            if (!playerPtr->IsOutOfLife())
            {
                winnerName = playerPtr->GetName();
                break;
            }
        }

        return (winnerName + " Wins.");
    }
	else if (_mode == GAME_MODE_BOSS)
	{
		return ("Boss Defeated. ");
	}
    else
    {
        return ("Draw.");
    }
}

void BattleSystem::ClearPlayers()
{
    for (auto element : _players)
        delete element;

    _players.clear();
}

void BattleSystem::ShowPlayerLife(const Player& player, int posXValue, int posYValue)
{
    // Display player's name
    mString.SetString(player.GetName());
    mString.SetSize(0.5);
    mString.SetTopLeft(posXValue, posYValue + 20);
    mString.ShowBitmap();
    // Display player's life
    integer.SetInteger(player.GetLife()); //CInteger integer
    integer.SetTopLeft(posXValue + 200, posYValue);
    integer.ShowBitmap();
    // Display player's taken damage

    if (player.GetState() == Player::RESPAWN_STATE)
    {
        takenDmgG.SetTopLeft(posXValue + 200, posYValue);
        takenDmgG.ShowBitmap();
    }
    else
    {
        int playerTakenDmg = player.GetTakenDamage();

        if (playerTakenDmg >= Player::TAKEN_DMG_DANGER_HIGH)
        {
            takenDmgR.SetTopLeft(posXValue + 200, posYValue);
            takenDmgR.ShowBitmap();
        }
        else if (playerTakenDmg >= Player::TAKEN_DMG_DANGER_MEDIUM)
        {
            takenDmgY.SetTopLeft(posXValue + 200, posYValue);
            takenDmgY.ShowBitmap();
        }
        else if (playerTakenDmg >= Player::TAKEN_DMG_DANGER_LOW)
        {
            takenDmgG.SetTopLeft(posXValue + 200, posYValue);
            takenDmgG.ShowBitmap();
        }
    }
}

int BattleSystem::GetCurrentRemainTime()
{
    return MATCH_TIME - map->GetTime();
}

shared_ptr<Map> BattleSystem::GetReferenceMap()
{
    return (map);
}
}