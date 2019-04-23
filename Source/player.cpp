#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"

namespace game_framework
{
//-----------------CONSTANTS DEFINITIONS-----------------//
const int PLAYER_INIT_X = 700;
const int PLAYER_INIT_Y = 100;
const int MAX_JUMP_COUNT = 2;
const int MOVEMENT_UNIT = 10;
const int GND_ATTACK_MOVEMENT_UNIT = 12;
const double INITIAL_VELOCITY = 18;
const double INITIAL_ACCELERATION = 1.2;
const double LANDING_ACCELERATION = 10;
const int OFFSET_INITIAL_VELOCITY = 15;
const double COLLISION_ERRORS = 1.0;
const int _OFFSET_X = 20;
const int _OFFSET_Y = 7;
const int MAX_LIFE = 3;
const int MAP_BORDER_OFFSET = 300;
const int MAP_BORDER_X1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_Y1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_X2 = SIZE_X + MAP_BORDER_OFFSET;
const int MAP_BORDER_Y2 = SIZE_Y + MAP_BORDER_OFFSET;
const double BITMAP_SIZE = 2.5;
const int MAX_ANIMATION_DURATION = 25;
// Triggered Animation Key ID
const int KEY_GND_ATTACK = 112;
const int KEY_GND_MOVE_RIGHT_ATTACK = 122;
const int KEY_GND_MOVE_LEFT_ATTACK = 132;
const int KEY_GND_LAND_DOWN_ATTACK = 142;
const int KEY_AIR_ATTACK = 212;
const int KEY_AIR_MOVE_RIGHT_ATTACK = 222;
const int KEY_AIR_MOVE_LEFT_ATTACK = 232;
const int KEY_AIR_LAND_DOWN_ATTACK = 242;
const int KEY_DRAW_SWORD = 113;
// Non-triggered Animation Key ID
const int KEY_GND_IDLE = 111;
const int KEY_GND_MOVE_RIGHT = 121;
const int KEY_GND_MOVE_LEFT = 131;
const int KEY_GND_LAND_DOWN = 141;
const int KEY_AIR_IDLE = 211;
const int KEY_AIR_MOVE_RIGHT = 221;
const int KEY_AIR_MOVE_LEFT = 231;
const int KEY_AIR_LAND_DOWN = 241;
//Animations ID of '_ani'
const int ANI_ID_RUN_LEFT = 0;
const int ANI_ID_RUN_RIGHT = 1;
const int ANI_ID_JUMP_LEFT = 2;
const int ANI_ID_JUMP_RIGHT = 3;
const int ANI_ID_STAND_LEFT = 4;
const int ANI_ID_STAND_RIGHT = 5;
const int ANI_ID_LEAN_LEFT = 6;
const int ANI_ID_LEAN_RIGHT = 7;
const int ANI_ID_LAND_FALL_LEFT = 8;
const int ANI_ID_LAND_FALL_RIGHT = 9;
//Animations ID of '_aniByWpn'
const int ANI_WPN_ID_STAND_LEFT = 0;
const int ANI_WPN_ID_STAND_RIGHT = 1;
const int ANI_WPN_ID_ATTACK_LEFT = 2;
const int ANI_WPN_ID_ATTACK_RIGHT = 3;
const int ANI_WPN_ID_GND_MOVE_ATTACK_LEFT = 4;
const int ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT = 5;
const int ANI_WPN_ID_SLIDE_ATTACK_LEFT = 6;
const int ANI_WPN_ID_SLIDE_ATTACK_RIGHT = 7;
const int ANI_WPN_ID_AIR_ATTACK_LEFT = 8;
const int ANI_WPN_ID_AIR_ATTACK_RIGHT = 9;
const int ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT = 10;
const int ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT = 11;
const int ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT = 12;
const int ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT = 13;
const int ANI_WPN_ID_DRAW_SWORD_LEFT = 14;
const int ANI_WPN_ID_DRAW_SWORD_RIGHT = 15;

//-----------------FUNCTIONS DEFINITIONS-----------------//
Player::Player() :
    _x(int()), _y(int()), ani(vector<CAnimation>()), currentAni(int()),
    bmp_iter(vector<vector<int>*>()), _width(int()),
    _height(int()), _isPressingLeft(bool()),
    _isPressingRight(bool()), _dir(bool()), _isTriggerJump(bool()), _jumpCount(bool()),
    _offsetVelocity(int()), _isOffsetLeft(bool()), _isOffsetRight(bool()),
    _velocity(double()), _grounds(vector<Ground*>()), _collision_box(CMovingBitmap()), _life(int()),
    _name(string()) // 我覺得之後應該先不用更改這個constructor，好多喔。。。
{
    /* Body intentionally empty */
}

Player::~Player()
{
    /* Body intentionally empty */
}

void Player::Initialize(vector<Ground*> groundsValue, vector<Player*>* playerPtrValue, string nameValue, vector<long> keysValue)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
	Ground *_ground = groundsValue[(rand() * 100) % groundsValue.size()];								// Randomly choose one of the ground object
	_x = (rand() * 1000) % (int)(_ground->GetWidth() * _ground->GetSize()) + _ground->GetCor(0);		// Randomly set x coordinate within Ground's width
	_y = _ground->GetCor(1) - GetHeight();
    //
    ResetTriggeredAnimationVariables();
    //
    _width = (int)(_collision_box.Width() * BITMAP_SIZE);
    _height = (int)(_collision_box.Height() * BITMAP_SIZE);
    //
    _keys = keysValue;
    //
    _isPressingLeft = _isPressingRight = _dir = false;
    //
    _isPressingDown = false;
    //
    _isTriggerJump = false;
    ResetJumpCount();
    //
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = _isOffsetRight = false;
    //
    _velocity = INITIAL_VELOCITY;
    _acceleration = INITIAL_ACCELERATION;
    //
    _isDrawingWeapon = false;
    //
    _isHoldingWeapon = _isTriggerAttack = false;
    _player = playerPtrValue;
    //
    _grounds = groundsValue;
    //
    _life = MAX_LIFE;
    //
    _name = nameValue;
    //
    ResetWeaponID();
    _roundPrevPickedUpWpnID = 2;
    //
    _aniSelector = false;
    //
    _currentAniByWpn = 0;
}

void Player::LoadBitmap()
{
    //-----------------NORMAL ANIMATION-----------------//
    vector<int> rl;	// bmps of running left
    vector<int> rr;	// bmps of running right
    vector<int> jl;	// bmps of jumping left
    vector<int> jr;	// bmps of jumping right
    vector<int> sl;	// bmps of standing left
    vector<int> sr;	// bmps of standing right
    vector<int> ll; // bmps of leaning left
    vector<int> lr; // bmps of leaning right
    vector<int>	lfl;// bmps of landing falling left
    vector<int> lfr;// bmps of landing falling right
    rl = vector<int> { IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };
    rr = vector<int> { IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };
    jl = vector<int> { IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };
    jr = vector<int> { IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };
    sl = vector<int> { IDB_P1_IDLE0M, IDB_P1_IDLE1M, IDB_P1_IDLE2M };
    sr = vector<int> { IDB_P1_IDLE0, IDB_P1_IDLE1, IDB_P1_IDLE2 };
    ll = vector<int> { IDB_P1_WALL0, IDB_P1_WALL1 };
    lr = vector<int> { IDB_P1_WALL0M, IDB_P1_WALL1M };
    lfl = vector<int> { IDB_P1_FALL0M, IDB_P1_FALL1M };
    lfr = vector<int> { IDB_P1_FALL0, IDB_P1_FALL1 };
    AddCAnimation(&rl, BITMAP_SIZE); //ani[0] Run Left
    AddCAnimation(&rr, BITMAP_SIZE); //ani[1] Run Right
    AddCAnimation(&jl, BITMAP_SIZE, 3, false); //ani[2] Jump Left
    AddCAnimation(&jr, BITMAP_SIZE, 3, false); //ani[3] Jump Right
    AddCAnimation(&sl, BITMAP_SIZE); //ani[4] Stand (Idle) Left
    AddCAnimation(&sr, BITMAP_SIZE); //ani[5] Stand (Idle) Right
    AddCAnimation(&ll, BITMAP_SIZE); //ani[6] Lean Left
    AddCAnimation(&lr, BITMAP_SIZE); //ani[7] Lean Right
    AddCAnimation(&lfl, BITMAP_SIZE); //ani[8] Landing Falling Left
    AddCAnimation(&lfr, BITMAP_SIZE); //ani[9] Landing Falling Right
    _collision_box.LoadBitmap(IDB_P1_TEST, RGB(0, 0, 0));
    //-----------------ANIMATION BY WEAPONS-----------------//
    /// Remark: Technically, if '_aniSelector' is true, '_aniByWpn' is displayed instead of the traditional 'ani', and vice versa.
    _aniByWpn = vector<vector<CAnimation>>();
    vector<int> s2l;// bmps of standing left with sword
    vector<int> s2r;// bmps of standing right with sword
    vector<int> al; // bmps of attacking left
    vector<int> ar; // bmps of attacking right
    vector<int> gmal;// bmps of on-ground-moving attack left
    vector<int> gmar;// bmps of on-ground-moving attack right
    vector<int> sal;// bmps of slide-attack left
    vector<int> sar;// bmps of slide-attack right
    vector<int> aal;// bmps of air-attack left
    vector<int> aar;// bmps of air-attack right
    vector<int> amal;// bmps of on-air-moving attack left
    vector<int> amar;// bmps of on-air-moving attack right
    vector<int> adal;// bmps of on-air-down attack left
    vector<int> adar;// bmps of on-air-down attack right
    vector<int> sdl;// bmps of drawing sword left
    vector<int> sdr;// bmps of drawing sword right
    // ~
    // ~ Weapon 0 - default
    s2l = vector<int> { IDB_P1_IDLE2_0M, IDB_P1_IDLE2_1M, IDB_P1_IDLE2_2M, IDB_P1_IDLE2_3M };
    s2r = vector<int> { IDB_P1_IDLE2_0, IDB_P1_IDLE2_1, IDB_P1_IDLE2_2, IDB_P1_IDLE2_3 };
    al = vector<int> { IDB_P1_WPN0_PUNCH0M, IDB_P1_WPN0_PUNCH1M, IDB_P1_WPN0_PUNCH2M, IDB_P1_WPN0_PUNCH3M, IDB_P1_WPN0_PUNCH4M, IDB_P1_WPN0_PUNCH5M, IDB_P1_WPN0_PUNCH6M, IDB_P1_WPN0_PUNCH7M, IDB_P1_WPN0_PUNCH8M };
    ar = vector<int> { IDB_P1_WPN0_PUNCH0, IDB_P1_WPN0_PUNCH1, IDB_P1_WPN0_PUNCH2, IDB_P1_WPN0_PUNCH3, IDB_P1_WPN0_PUNCH4, IDB_P1_WPN0_PUNCH5, IDB_P1_WPN0_PUNCH6, IDB_P1_WPN0_PUNCH7, IDB_P1_WPN0_PUNCH8 };
    gmal = vector<int> { IDB_P1_WPN0_KICK0M, IDB_P1_WPN0_KICK1M, IDB_P1_WPN0_KICK2M, IDB_P1_WPN0_KICK3M, IDB_P1_WPN0_KICK4M, IDB_P1_WPN0_KICK5M, IDB_P1_WPN0_KICK6M, IDB_P1_WPN0_KICK7M };
    gmar = vector<int> { IDB_P1_WPN0_KICK0, IDB_P1_WPN0_KICK1, IDB_P1_WPN0_KICK2, IDB_P1_WPN0_KICK3, IDB_P1_WPN0_KICK4, IDB_P1_WPN0_KICK5, IDB_P1_WPN0_KICK6, IDB_P1_WPN0_KICK7 };
    sal = vector<int> { IDB_P1_SLIDE0M, IDB_P1_SLIDE1M };
    sar = vector<int> { IDB_P1_SLIDE0, IDB_P1_SLIDE1 };
    aal = gmal;
    aar = gmar;
    amal = vector<int> { IDB_P1_WPN0_RUN_PUNCH0M, IDB_P1_WPN0_RUN_PUNCH1M, IDB_P1_WPN0_RUN_PUNCH2M, IDB_P1_WPN0_RUN_PUNCH3M, IDB_P1_WPN0_RUN_PUNCH4M, IDB_P1_WPN0_RUN_PUNCH5M, IDB_P1_WPN0_RUN_PUNCH6M };
    amar = vector<int> { IDB_P1_WPN0_RUN_PUNCH0, IDB_P1_WPN0_RUN_PUNCH1, IDB_P1_WPN0_RUN_PUNCH2, IDB_P1_WPN0_RUN_PUNCH3, IDB_P1_WPN0_RUN_PUNCH4, IDB_P1_WPN0_RUN_PUNCH5, IDB_P1_WPN0_RUN_PUNCH6 };
    adal = vector<int> { IDB_P1_AIR_DOWN_ATTACK_R0M, IDB_P1_AIR_DOWN_ATTACK_L0M, IDB_P1_AIR_DOWN_ATTACK_L1M, IDB_P1_AIR_DOWN_ATTACK_E0M, IDB_P1_AIR_DOWN_ATTACK_E1M, IDB_P1_AIR_DOWN_ATTACK_E2M };
    adar = vector<int> { IDB_P1_AIR_DOWN_ATTACK_R0, IDB_P1_AIR_DOWN_ATTACK_L0, IDB_P1_AIR_DOWN_ATTACK_L1, IDB_P1_AIR_DOWN_ATTACK_E0, IDB_P1_AIR_DOWN_ATTACK_E1, IDB_P1_AIR_DOWN_ATTACK_E2 };
    sdl = vector<int> { IDB_P1_SWD_DWR0M, IDB_P1_SWD_DWR1M, IDB_P1_SWD_DWR2M, IDB_P1_SWD_DWR3M, IDB_P1_SWD_DWR3M };
    sdr = vector<int> { IDB_P1_SWD_DWR0, IDB_P1_SWD_DWR1, IDB_P1_SWD_DWR2, IDB_P1_SWD_DWR3, IDB_P1_SWD_DWR3 };
    AddCollectionOfAnimationsByWeapon(
        s2l, s2r, al, ar,
        gmal, gmar, sal, sar,
        aal, aar, amal, amar,
        adal, adar, sdl, sdr);
    // ~
    // ~ Weapon 1
    al = vector<int> { IDB_P1_ATTACK0M, IDB_P1_ATTACK1M, IDB_P1_ATTACK2M, IDB_P1_ATTACK3M, IDB_P1_ATTACK4M };
    ar = vector<int> { IDB_P1_ATTACK0, IDB_P1_ATTACK1, IDB_P1_ATTACK2, IDB_P1_ATTACK3, IDB_P1_ATTACK4 };
    gmal = vector<int> { IDB_P1_GND_MOVE_ATTACK0M, IDB_P1_GND_MOVE_ATTACK1M, IDB_P1_GND_MOVE_ATTACK2M, IDB_P1_GND_MOVE_ATTACK3M, IDB_P1_GND_MOVE_ATTACK4M, IDB_P1_GND_MOVE_ATTACK5M };
    gmar = vector<int> { IDB_P1_GND_MOVE_ATTACK0, IDB_P1_GND_MOVE_ATTACK1, IDB_P1_GND_MOVE_ATTACK2, IDB_P1_GND_MOVE_ATTACK3, IDB_P1_GND_MOVE_ATTACK4, IDB_P1_GND_MOVE_ATTACK5 };
    aal = vector<int> { IDB_P1_AIR_ATTACK0M, IDB_P1_AIR_ATTACK1M, IDB_P1_AIR_ATTACK2M };
    aar = vector<int> { IDB_P1_AIR_ATTACK0, IDB_P1_AIR_ATTACK1, IDB_P1_AIR_ATTACK2 };
    amal = vector<int> { IDB_P1_AIR_MOVE_ATTACK0M, IDB_P1_AIR_MOVE_ATTACK1M, IDB_P1_AIR_MOVE_ATTACK2M, IDB_P1_AIR_MOVE_ATTACK3M };
    amar = vector<int> { IDB_P1_AIR_MOVE_ATTACK0, IDB_P1_AIR_MOVE_ATTACK1, IDB_P1_AIR_MOVE_ATTACK2, IDB_P1_AIR_MOVE_ATTACK3 };
    AddCollectionOfAnimationsByWeapon(
        s2l, s2r, al, ar,
        gmal, gmar, sal, sar,
        aal, aar, amal, amar,
        adal, adar, sdl, sdr);
    // ~
    // ~ Weapon 2
    s2l = vector<int> { IDB_P1_WPN2_IDLE0M, IDB_P1_WPN2_IDLE1M, IDB_P1_WPN2_IDLE2M, IDB_P1_WPN2_IDLE3M };
    s2r = vector<int> { IDB_P1_WPN2_IDLE0, IDB_P1_WPN2_IDLE1, IDB_P1_WPN2_IDLE2, IDB_P1_WPN2_IDLE3 };
    al = vector<int> { IDB_P1_WPN2_ATTACK0M, IDB_P1_WPN2_ATTACK1M, IDB_P1_WPN2_ATTACK2M, IDB_P1_WPN2_ATTACK3M, IDB_P1_WPN2_ATTACK4M };
    ar = vector<int> { IDB_P1_WPN2_ATTACK0, IDB_P1_WPN2_ATTACK1, IDB_P1_WPN2_ATTACK2, IDB_P1_WPN2_ATTACK3, IDB_P1_WPN2_ATTACK4 };
    gmal = vector<int> { IDB_P1_WPN2_GND_MOVE_ATTACK0M, IDB_P1_WPN2_GND_MOVE_ATTACK1M, IDB_P1_WPN2_GND_MOVE_ATTACK2M, IDB_P1_WPN2_GND_MOVE_ATTACK3M, IDB_P1_WPN2_GND_MOVE_ATTACK4M, IDB_P1_WPN2_GND_MOVE_ATTACK5M };
    gmar = vector<int> { IDB_P1_WPN2_GND_MOVE_ATTACK0, IDB_P1_WPN2_GND_MOVE_ATTACK1, IDB_P1_WPN2_GND_MOVE_ATTACK2, IDB_P1_WPN2_GND_MOVE_ATTACK3, IDB_P1_WPN2_GND_MOVE_ATTACK4, IDB_P1_WPN2_GND_MOVE_ATTACK5 };
    aal = vector<int> { IDB_P1_WPN2_AIR_ATTACK0M, IDB_P1_WPN2_AIR_ATTACK1M, IDB_P1_WPN2_AIR_ATTACK2M };
    aar = vector<int> { IDB_P1_WPN2_AIR_ATTACK0, IDB_P1_WPN2_AIR_ATTACK1, IDB_P1_WPN2_AIR_ATTACK2 };
    amal = vector<int> { IDB_P1_WPN2_AIR_MOVE_ATTACK0M, IDB_P1_WPN2_AIR_MOVE_ATTACK1M, IDB_P1_WPN2_AIR_MOVE_ATTACK2M, IDB_P1_WPN2_AIR_MOVE_ATTACK3M };
    amar = vector<int> { IDB_P1_WPN2_AIR_MOVE_ATTACK0, IDB_P1_WPN2_AIR_MOVE_ATTACK1, IDB_P1_WPN2_AIR_MOVE_ATTACK2, IDB_P1_WPN2_AIR_MOVE_ATTACK3 };
    adal = vector<int> { IDB_P1_WPN2_AIR_DOWN_ATTACK_R0M, IDB_P1_WPN2_AIR_DOWN_ATTACK_L0M, IDB_P1_WPN2_AIR_DOWN_ATTACK_L1M, IDB_P1_WPN2_AIR_DOWN_ATTACK_E0M, IDB_P1_WPN2_AIR_DOWN_ATTACK_E1M, IDB_P1_WPN2_AIR_DOWN_ATTACK_E2M };
    adar = vector<int> { IDB_P1_WPN2_AIR_DOWN_ATTACK_R0, IDB_P1_WPN2_AIR_DOWN_ATTACK_L0, IDB_P1_WPN2_AIR_DOWN_ATTACK_L1, IDB_P1_WPN2_AIR_DOWN_ATTACK_E0, IDB_P1_WPN2_AIR_DOWN_ATTACK_E1, IDB_P1_WPN2_AIR_DOWN_ATTACK_E2 };
    sdl = vector<int> { IDB_P1_WPN2_DRAW0M, IDB_P1_WPN2_DRAW1M, IDB_P1_WPN2_DRAW2M, IDB_P1_WPN2_DRAW3M };
    sdr = vector<int> { IDB_P1_WPN2_DRAW0, IDB_P1_WPN2_DRAW1, IDB_P1_WPN2_DRAW2, IDB_P1_WPN2_DRAW3 };
    AddCollectionOfAnimationsByWeapon(
        s2l, s2r, al, ar,
        gmal, gmar, sal, sar,
        aal, aar, amal, amar,
        adal, adar, sdl, sdr);
}

void Player::OnMove()
{
    //-----------------ANIMATIONS SECTION-----------------//
    for (auto i = ani.begin(); i != ani.end(); i++) //For all CAnimation objects in 'ani'
        i->OnMove(); //Proceed to the next CMovingBitmap in the CAnimation 'i'

    /// DEBUG
    for (unsigned int i = 0; i < _aniByWpn.size(); i++)
    {
        for (unsigned int j = 0; j < _aniByWpn[i].size(); j++)
        {
            _aniByWpn[i][j].OnMove();
        }
    }

    /* JUMP ANIMATION */
    if (IsOnGround() || IsOnLeftEdge() || IsOnRightEdge() || (_isTriggerJump && _jumpCount > 0))
        ResetAnimations(ANI_ID_JUMP_LEFT);

    //-----------------POSITION TRANSFORMATION SECTION-----------------//
    /* REPOSITION PLAYER ABOUT GROUNDS */
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (IsIntersectGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
        {
            DoRepositionAboutGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2);
        }
    }

    /* INITIALIZATION FOR GRAVITY and PREPARATION FOR LANDING DOWN */
    _acceleration = INITIAL_ACCELERATION;
    /// Warning: 'DoLand()' inside 'ProcessKeyCombinationOnMove()' modifies the member variable '_acceleration'. Thus, the function affects the below codes regarding gravity and as a result must be placed here!!!
    ProcessKeyCombinationOnMove();

    /* GRAVITY */
    if (IsOnGround())
    {
        _velocity = 0.0;
    }
    else
    {
        _velocity += _acceleration;
        _y += Round(_velocity);
    }

    /* WALL JUMP */
    if (IsBeingOffsetHorizontally())
        DoHorizontalOffset(); // Modify the x-coordinate of the player

    /* FALL OFF THE MAP */
    if (IsOutMapBorder())
    {
        DoDead();
        DoRespawn();
    }

    //-----------------UNTITLED SECTION-----------------//
    if (IsOnGround() || IsOnLeftEdge() || IsOnRightEdge())
    {
        ResetJumpCount();
    }
}

void Player::OnShow()
{
    // Set current animation
    if (_isTriggeredAni)
    {
        SetCurrentTriggeredAnimation();
    }
    else
    {
        SetCurrentNonTriggerAnimation();
    }

    // Show current animation
    ShowAnimation();
	// Play current audio
	PlayAudioByState();

    // For showing the "name tag" //
    if (_PLAYER_DEBUG || 1)
    {
		CPoint cam = camera->GetXY(Round(_x - 4 * BITMAP_SIZE), Round(_y + _collision_box.Height() * BITMAP_SIZE));
        OnShowText(_name.c_str(),cam.x ,cam.y , Round(15 * camera->GetSize()), RGB(255, 255, 255));
    }///////////////////////////////
}

void Player::OnKeyDown(const UINT& nChar)
{
    if (nChar == _keys[0]) // Up
    {
        _isTriggerJump = true;
    }
    else if (nChar == _keys[1]) // Right
    {
        _dir = true;
        _isPressingRight = true;
    }
    else if (nChar == _keys[2]) // Down
    {
        /// Comment for future devs: Later use to slip down from certain terrain
        _isPressingDown = true;
    }
    else if (nChar == _keys[3]) // Left
    {
        _dir = false;
        _isPressingLeft = true;
    }
    else if (nChar == _keys[4]) //Attack
    {
        _isTriggerAttack = true;
    }
    else
    {
        // Do nothing
    }
}

void Player::OnKeyUp(const UINT& nChar)
{
    if (nChar == _keys[1]) // Right
    {
        _isPressingRight = false;
    }
    else if (nChar == _keys[2]) // Down
    {
        _isPressingDown = false;
    }
    else if (nChar == _keys[3]) // Left
    {
        _isPressingLeft = false;
    }
    else
    {
        // Do nothing
    }
}

void Player::SetHoldWeapon(bool isHolding)
{
    _isHoldingWeapon = isHolding;
    _isDrawingWeapon = isHolding;
    _isTriggerAttack = false; // We are picking weapon, not performing an attack
}

void Player::BeenAttacked(bool dir)
{
    InitiateOffsetUp();

    if (!dir)
        InitiateOffsetLeft();
    else
        InitiateOffsetRight();
}

bool Player::GetHoldWeapon()
{
    return _isHoldingWeapon;
}

bool Player::GetDirection()
{
    return _dir;
}

int Player::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return _x;

        case 1:
            return _y;

        case 2:
            return _x + (int)(_collision_box.Width() * BITMAP_SIZE);

        case 3:
            return _y + (int)(_collision_box.Height() * BITMAP_SIZE);

        default:
            return NULL;
    }
}

int Player::ShowAnimationState()
{
    return (_aniSelector ? _aniByWpn[_wpnID][_currentAniByWpn].GetCurrentBitmapNumber() : (ani.begin() + currentAni)->GetCurrentBitmapNumber());
}

int Player::GetWidth()
{
	return GetCor(2) - GetCor(0);
}

int Player::GetHeight()
{
	return GetCor(3) - GetCor(1);
}

const string& Player::GetName() const
{
    return (_name);
}

const int& Player::GetLife() const
{
    return (_life);
}

const bool Player::IsOutOfLife() const
{
    return (_life == 0);
}

const long& Player::GetAttackKey() const
{
    return (_keys[4]);
}

void Player::GenerateAndSetWeaponID() // This function is intended to be called by 'Weapon::HitPlayer()'
{
    // The player will pick his weapon alternatively through out the round
    if (_roundPrevPickedUpWpnID == 1)
    {
        _roundPrevPickedUpWpnID = 2;
        _wpnID = 2;
    }
    else if (_roundPrevPickedUpWpnID == 2)
    {
        _roundPrevPickedUpWpnID = 1;
        _wpnID = 1;
    }
}

void Player::ResetWeaponID()
{
    _wpnID = 0; // reset to default weapon: punch
}

void Player::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    ani.push_back(temp);
    bmp_iter.push_back(list);
}



void Player::ResetAnimations(int leftAnimationID)
{
    ani[leftAnimationID].Reset(); // Reset left animation
    ani[leftAnimationID + 1].Reset(); // Reset right animation
}

void Player::SetAnimationStateLeftRight(int leftAnimationID)
{
    if (_dir) // Player is facing right
    {
        SetAnimationState(leftAnimationID + 1);
    }
    else // Player is facing left
    {
        SetAnimationState(leftAnimationID);
    }
}

void Player::SetAnimationState(int num)
{
    _aniSelector = false; // Choose '_ani' for showing the animation
    currentAni = num;

    for (auto i = ani.begin(); i != ani.end(); i++)
    {
        if (i == ani.begin() + num)
            i->SetPause(false);
        else
            i->SetPause(true);
    }

    // Pause everything of '_aniByWpn'
    for (unsigned int i = 0; i < _aniByWpn.size(); i++)
    {
        for (unsigned int j = 0; j < _aniByWpn[i].size(); j++)
        {
            _aniByWpn[i][j].SetPause(true);
        }
    }
}

void Player::ShowAnimation()
{
	if (_PLAYER_DEBUG)
	{
		CPoint cam = camera->GetXY(_x, _y);
		_collision_box.SetTopLeft(cam.x, cam.y);		//actual player blocks
		_collision_box.ShowBitmap(BITMAP_SIZE * camera->GetSize());
	}

    if (_aniSelector) // If '_aniByWpn' is chosen for showing the animation
    {
        //Calculate and set the position of the player current animation in respect to the collision box's
		CPoint cam = camera->GetXY(_x - (int)(_OFFSET_X * BITMAP_SIZE), _y - (int)(_OFFSET_Y * BITMAP_SIZE));
		_aniByWpn[_wpnID][_currentAniByWpn].SetSize(BITMAP_SIZE * camera->GetSize());
        _aniByWpn[_wpnID][_currentAniByWpn].SetTopLeft(cam.x, cam.y);
        _aniByWpn[_wpnID][_currentAniByWpn].OnShow();
    }
    else // If '_ani' is chosen for showing the animation
    {
        vector<CAnimation>::iterator ani_iter = ani.begin() + currentAni;
        //Calculate and set the position of the player current animation in respect to the collision box's
		CPoint cam = camera->GetXY(_x - (int)(_OFFSET_X * BITMAP_SIZE), _y - (int)(_OFFSET_Y * BITMAP_SIZE));
		ani_iter->SetSize(BITMAP_SIZE * camera->GetSize());
        ani_iter->SetTopLeft(cam.x, cam.y);
        ani_iter->OnShow();
    }
}

bool Player::IsIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (playerX2 >= groundX1 && playerX1 <= groundX2 && playerY2 >= groundY1 && playerY1 <= groundY2);
}

bool Player::IsExplicitlyVerticallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (!(playerY2 <= groundY1 || playerY1 >= groundY2));
}

bool Player::IsExplicitlyHorizontallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (!((playerX2 <= groundX1) || (playerX1 >= groundX2)));
}

bool Player::IsOnGroundLeftEdge(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (IsExplicitlyVerticallyIntersectGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2)
            &&
            (playerX1 < groundX1 && groundX1 < playerX2 && playerX2 < groundX2));
}

bool Player::IsOnGroundRightEdge(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (IsExplicitlyVerticallyIntersectGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2)
            &&
            (groundX1 < playerX1 && playerX1 < groundX2 && groundX2 < playerX2));
}

bool Player::IsOnGroundUnderside(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (IsExplicitlyHorizontallyIntersectGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2)
            &&
            (groundY1 < playerY1 && playerY1 <= groundY2 && groundY2 < playerY2));
}

bool Player::IsOnParticularGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (playerY1 < groundY1 && groundY1 <= playerY2 && playerY2 < groundY2
            &&
            groundX1 <= playerX1 && playerX2 <= groundX2);
}

void Player::DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    if (IsOnGroundLeftEdge(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
    {
        _x = groundX1 - _width;
    }

    if (IsOnGroundRightEdge(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
    {
        _x = groundX2;
    }

    if (IsOnGroundUnderside(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
    {
        _y = groundY2;
    }

    if (IsOnParticularGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
    {
        _y = groundY1 - _height;
    }
}

bool Player::IsOnGround()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (IsOnParticularGround(playerX1, playerY1, playerX2, playerY2, groundX1, groundY1, groundX2, groundY2))
            return true;
    }

    return false;
}

bool Player::IsOnLeftEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if (((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS)) && (_x == groundX1 - _width))
            return true;
    }

    return false;
}

bool Player::IsOnRightEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if ((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS) && (_x == groundX2))
            return true;
    }

    return false;
}

bool Player::IsOutMapBorder()
{
    return (!(
                ((MAP_BORDER_X1 <= GetCor(0)/*x1*/) && (GetCor(2)/*x2*/ <= MAP_BORDER_X2))
                &&
                ((MAP_BORDER_Y1 <= GetCor(1)/*y1*/) && (GetCor(3)/*y2*/ <= MAP_BORDER_Y2))
            ));
}

void Player::InitiateOffsetUp()
{
    _velocity = -INITIAL_VELOCITY;
    _y -= Round(INITIAL_VELOCITY); //Trick explaination: By intuition, '_y' of the player should not be
    // modified here, because it is expected to be modified whenever 'Player::OnMove()' is called. However,
    // since the player is currently on the ground, 'Player::OnMove()' will fix its '_y' onto the surface
    // instead of modifying it as expectation. Thus, '_y' must be altered here to set the player jump his ass up!!
}

void Player::InitiateOffsetLeft()
{
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = true;
}

void Player::InitiateOffsetRight()
{
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetRight = true;
}

bool Player::IsBeingOffsetHorizontally()
{
    return (_isOffsetLeft || _isOffsetRight);
}

void Player::DoHorizontalOffset()
{
    if (_isOffsetLeft)
    {
        if (_offsetVelocity > 0)
        {
            _offsetVelocity--;
            _x -= _offsetVelocity;
        }
        else
        {
            _isOffsetLeft = false;
        }
    }
    else if (_isOffsetRight)
    {
        if (_offsetVelocity > 0)
        {
            _offsetVelocity--;
            _x += _offsetVelocity;
        }
        else
        {
            _isOffsetRight = false;
        }
    }
}

void Player::DoMoveLeft(int movementUnit)
{
    _x -= movementUnit;
}

void Player::DoMoveRight(int movementUnit)
{
    _x += movementUnit;
}

void Player::DoLand()
{
    _acceleration = LANDING_ACCELERATION;
}

void Player::DoJump()
{
    if (_jumpCount > 0)   //If the player is able to jump more
    {
        InitiateOffsetUp();
        _jumpCount--; //Decrement the jumps available
    }
}

void Player::ResetJumpCount()
{
    _jumpCount = MAX_JUMP_COUNT;
}

void Player::InitiateWallJump()
{
    if (_jumpCount > 0) //If the player is able to jump more
    {
        if (IsOnLeftEdge())
        {
            InitiateOffsetLeft();
        }
        else if (IsOnRightEdge())
        {
            InitiateOffsetRight();
        }
    }
}

void Player::DoAttack()
{
    for (auto i = _player->begin(); i != _player->end(); i++)
    {
        if (*i != this && HitPlayer(*i))
        {
			if (_isHoldingWeapon)
				CAudio::Instance()->Play(IDS_SWOOSH);
			else
				CAudio::Instance()->Play(IDS_PUNCH);
			(*i)->BeenAttacked(_dir);
        }
    }
}

bool Player::IsAttacking()
{
    return (_isTriggerAttack);
}

bool Player::HitPlayer(Player* player)
{
    int x1 = GetCor(0) - 50, y1 = GetCor(1), x2 = GetCor(2) + 50, y2 = GetCor(3);
    return (player->GetCor(2) >= x1 && player->GetCor(0) <= x2 && player->GetCor(3) >= y1 && player->GetCor(1) <= y2);
}

bool Player::IsDrawingWeapon()
{
    return (_isDrawingWeapon && !IsFinishedDrawingAnimation());
}

bool Player::IsFinishedDrawingAnimation()
{
    return (_aniByWpn[_wpnID][ANI_WPN_ID_DRAW_SWORD_LEFT].IsFinalBitmap() || _aniByWpn[_wpnID][ANI_WPN_ID_DRAW_SWORD_RIGHT].IsFinalBitmap());
}

void Player::DoThrowingWeapon()
{
    Weapon throwing;
    throwing.Initialize(vector<Ground*> {}, vector<Player*> {this});
}

void Player::PlayAudioByState()
{
	int aboutToPlay = -1;

	if (WpnStateChanged() && _isTriggeredAni) {
		switch (_triggeredAniByWpnID) {

		case ANI_WPN_ID_DRAW_SWORD_LEFT:
		case ANI_WPN_ID_DRAW_SWORD_RIGHT:

			aboutToPlay = IDS_DRAW_WEAPON;
			break;

		case ANI_WPN_ID_STAND_LEFT:
		case ANI_WPN_ID_STAND_RIGHT:
		case ANI_WPN_ID_ATTACK_LEFT:
		case ANI_WPN_ID_ATTACK_RIGHT:
		case ANI_WPN_ID_GND_MOVE_ATTACK_LEFT:
		case ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT:
		case ANI_WPN_ID_SLIDE_ATTACK_LEFT:
		case ANI_WPN_ID_SLIDE_ATTACK_RIGHT:
		case ANI_WPN_ID_AIR_ATTACK_LEFT:
		case ANI_WPN_ID_AIR_ATTACK_RIGHT:
		case ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT:
		case ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT:
		case ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT:
		case ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT:

			aboutToPlay = IDS_SWING_ATTACK;
			break;

		}
	}

	if (StateChanged() && !_isTriggeredAni) {
		switch (GetKeyCombination()) {
		case KEY_GND_IDLE:
			break;

		}
	}

	if (aboutToPlay != -1)
		CAudio::Instance()->Play(aboutToPlay);
}

void Player::DoDead()
{
    SetHoldWeapon(false);
    ResetWeaponID(); // reset to the default weapon - punch
    _life--;
    /// activate dead effect
}

void Player::DoRespawn()
{
    //If the player is out of the screen, then he will be set on the highest position
    _y = 0;
    _velocity = INITIAL_VELOCITY;
}

void Player::SetAnimationStateByWeapon(int num)
{
    _aniSelector = true; // Choose '_aniByWpn' for showing the animation
    _currentAniByWpn = num;

    for (unsigned int i = 0; i < _aniByWpn.size(); i++)
    {
        for (unsigned int j = 0; j < _aniByWpn[i].size(); j++)
        {
            if ((i == _wpnID) && (j == _currentAniByWpn))
            {
                _aniByWpn[i][j].SetPause(false);
            }
            else
            {
                _aniByWpn[i][j].SetPause(true);
            }
        }
    }

    // Pause everything of 'ani'
    for (auto i = ani.begin(); i != ani.end(); i++)
    {
        i->SetPause(true);
    }
}

void Player::AddCollectionOfAnimationsByWeapon(
    vector<int>& s2l, vector<int>& s2r, vector<int>& al, vector<int>& ar,
    vector<int>& gmal, vector<int>& gmar, vector<int>& sal, vector<int>& sar,
    vector<int>& aal, vector<int>& aar, vector<int>& amal, vector<int>& amar,
    vector<int>& adal, vector<int>& adar, vector<int>& sdl, vector<int>& sdr)
{
    vector<CAnimation> tempAniByWpn = vector<CAnimation>();
    AddCAnimationByWeapon(tempAniByWpn, &s2l, BITMAP_SIZE); //ani[0] Stand (Idle) Left with sword
    AddCAnimationByWeapon(tempAniByWpn, &s2r, BITMAP_SIZE); //ani[1] Stand (Idle) Right with sword
    AddCAnimationByWeapon(tempAniByWpn, &al, BITMAP_SIZE, 3, false); //ani[2] Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &ar, BITMAP_SIZE, 3, false); //ani[3] Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &gmal, BITMAP_SIZE, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &gmar, BITMAP_SIZE, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &sal, BITMAP_SIZE, 3, false); //ani[6] Slide Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &sar, BITMAP_SIZE, 3, false); //ani[7] Slide Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &aal, BITMAP_SIZE, 3, false); //ani[8] Air Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &aar, BITMAP_SIZE, 3, false); //ani[9] Air Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &amal, BITMAP_SIZE, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &amar, BITMAP_SIZE, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &adal, BITMAP_SIZE, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &adar, BITMAP_SIZE, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &sdl, BITMAP_SIZE, 3, false); //ani[14] Draw sword Left
    AddCAnimationByWeapon(tempAniByWpn, &sdr, BITMAP_SIZE, 3, false); //ani[15] Draw sword Right
    _aniByWpn.push_back(tempAniByWpn);
}

void Player::AddCAnimationByWeapon(vector<CAnimation>& tempAniByWpn, vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    tempAniByWpn.push_back(temp);
}

int Player::GetKeyCombination()
{
    /* Key combination structure:
    [ 1 ][ 2 ][ 3 ]

    [ 1 ] determines the player is on air or on ground
    [ 2 ] determine the right button is pressed, the left button is pressed, the down button is pressed, or none of the movement keys are pressed
    [ 3 ] determine whether it is the case that the attack button is pressed and the player is holding a weapon
    */
    string keyCombString = "";

    if (IsOnGround())
        keyCombString = keyCombString + "1";
    else
        keyCombString = keyCombString + "2";

    if (_isPressingRight)
        keyCombString = keyCombString + "2";
    else if (_isPressingLeft)
        keyCombString = keyCombString + "3";
    else if (_isPressingDown)
        keyCombString = keyCombString + "4";
    else // All movement keys are up
        keyCombString = keyCombString + "1";

    if (IsDrawingWeapon())
		keyCombString = keyCombString + "3";
	else if (IsAttacking())
		keyCombString = keyCombString + "2";
    else
        keyCombString = keyCombString + "1";

    return (stoi(keyCombString));
}

void Player::ProcessKeyCombinationOnMove()
{
    /*	~ Remarks: Explaination of the triggered animation concept
    	~ When an animation is said to be "triggered", it prevails all other animations by means of depiction for a period of time;
    	~ that is, when an animation is triggered, it becomes the only animation being shown for a time interval.
    	~ In this case, it is often the key combo (key combination of the attack button with one movement button) that triggers an animation
    */
    if (!_isTriggeredAni) // If there is no animation being triggered in the meantime, then detect should there be any
    {
        GetAndSetTriggeredAnimation();

        if (_isTriggeredAni) // If an animation is found to be triggered, then we firstly initiate it (there is no process here)
        {
            InitiateTriggeredAnimation();
        }
    }

    if (_isTriggeredAni) // If an animation is triggered, then we process it (there is no initiation here)
    {
        if (!IsFinishedTriggeredAnimation()) // If an animation is triggered and the time duration dedicated for it has not elapsed, then increment '_triggeredAniCount' (representing the elapsed animation time) and finish doing the triggered animation
        {
            _triggeredAniCount++;
            DoTriggeredAnimation();
        }
        else // If an animation is triggered and it has done its showcase, then thoroughly finish it and reset the triggered animation's variables
        {
            FinishTriggeredAnimation();
            ResetTriggeredAnimationVariables();
        }
    }
    else // If there is no triggered animation in the meantime, then do the other animations
    {
        DoNonTriggeredAnimation();
    }
}

void Player::ResetTriggeredAnimationVariables()
{
	_isDrawingWeapon = false;
    _isTriggeredAni = false;
    _triggeredAniKeyID = 0;
    _triggeredAniCount = 0;
    _triggeredAniByWpnID = -1;
}

void Player::SetFirstThreeTriggeredAnimationVariables(int keyCombInt)
{
    _isTriggeredAni = true;
    _triggeredAniKeyID = keyCombInt;
    _triggeredAniCount = 0;
}

void Player::SetTriggeredAnimationVariables(int keyCombInt)
{
    switch (keyCombInt)
    {
        /* ON GROUND */
        case KEY_GND_ATTACK: // on ground, not move, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_ATTACK_LEFT;

            break;

        case KEY_GND_MOVE_RIGHT_ATTACK: // on ground, move right, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_GND_MOVE_ATTACK_LEFT;

            break;

        case KEY_GND_MOVE_LEFT_ATTACK: // on ground, move left, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_GND_MOVE_ATTACK_LEFT;

            break;

        case KEY_GND_LAND_DOWN_ATTACK: // on ground, land down, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_SLIDE_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_SLIDE_ATTACK_LEFT;

            break;

        /* ON AIR */
        case KEY_AIR_ATTACK: // on air, not move, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_ATTACK_LEFT;

            break;

        case KEY_AIR_MOVE_RIGHT_ATTACK: // on air, move right, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT;

            break;

        case KEY_AIR_MOVE_LEFT_ATTACK: // on air, move left, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT;

            break;

        case KEY_AIR_LAND_DOWN_ATTACK: // on air, land down, attack
            SetFirstThreeTriggeredAnimationVariables(keyCombInt);

            if (_dir)
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT;
            else
                _triggeredAniByWpnID = ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT;

            break;

		case KEY_DRAW_SWORD:
			SetFirstThreeTriggeredAnimationVariables(keyCombInt);

			if (_dir) // If the player is facing right
				_triggeredAniByWpnID = ANI_WPN_ID_DRAW_SWORD_RIGHT;
			else
				_triggeredAniByWpnID = ANI_WPN_ID_DRAW_SWORD_LEFT;

			break;

        default:
            // Do nothing
            break;
    }
}

void Player::GetAndSetTriggeredAnimation()
{
    int keyCombInt = GetKeyCombination();
    SetTriggeredAnimationVariables(keyCombInt);
}

void Player::InitiateTriggeredAnimation()
{
    switch (_triggeredAniKeyID)
    {
        /* ON GROUND */
        case KEY_GND_ATTACK: // on ground, not move, attack
            _isTriggerAttack = false;
            break;

        case KEY_GND_MOVE_RIGHT_ATTACK: // on ground, move right, attack
            _isTriggerAttack = false;
            InitiateOffsetRight();
            break;

        case KEY_GND_MOVE_LEFT_ATTACK: // on ground, move left, attack
            _isTriggerAttack = false;
            InitiateOffsetLeft();
            break;

        case KEY_GND_LAND_DOWN_ATTACK: // on ground, land down, attack
            if (_dir)
                InitiateOffsetRight();
            else
                InitiateOffsetLeft();

            _isTriggerAttack = false;
            break;

        /* ON AIR */
        case KEY_AIR_ATTACK: // on air, not move, attack
            _isTriggerAttack = false;
            break;

        case KEY_AIR_MOVE_RIGHT_ATTACK: // on air, move right, attack
            _isTriggerAttack = false;
            InitiateOffsetRight();
            break;

        case KEY_AIR_MOVE_LEFT_ATTACK: // on air, move left, attack
            _isTriggerAttack = false;
            InitiateOffsetLeft();
            break;

        case KEY_AIR_LAND_DOWN_ATTACK: // on air, land down, attack
            _isTriggerAttack = false;
            break;

        default:
            break;
    }
}

void Player::DoTriggeredAnimation()
{
    switch (_triggeredAniKeyID)
    {
        /* ON GROUND */
        case KEY_GND_ATTACK: // on ground, not move, attack
            DoAttack();
            break;

        case KEY_GND_MOVE_RIGHT_ATTACK: // on ground, move right, attack
            DoAttack();
            break;

        case KEY_GND_MOVE_LEFT_ATTACK: // on ground, move left, attack
            DoAttack();
            break;

        case KEY_GND_LAND_DOWN_ATTACK: // on ground, land down, attack
            DoAttack();
            break;

        /* ON AIR */
        case KEY_AIR_ATTACK: // on air, not move, attack
            DoAttack();
            break;

        case KEY_AIR_MOVE_RIGHT_ATTACK: // on air, move right, attack
            DoAttack();
            break;

        case KEY_AIR_MOVE_LEFT_ATTACK: // on air, move left, attack
            DoAttack();
            break;

        case KEY_AIR_LAND_DOWN_ATTACK: // on air, land down, attack
            DoAttack();
            break;

        default:
            break;
    }
}

bool Player::IsFinishedTriggeredAnimation()
{
    return (_triggeredAniCount > MAX_ANIMATION_DURATION);
}

void Player::FinishTriggeredAnimation()
{
    _aniByWpn[_wpnID][_triggeredAniByWpnID].Reset();
}

void Player::SetCurrentTriggeredAnimation()
{
    SetAnimationStateByWeapon(_triggeredAniByWpnID);
}

void Player::DoNonTriggeredAnimation()
{
    /*	~ Remarks:
    ~ We care only about key combinations that does NOT determine the attack button is pressed and the player is holding a weapon
    ~ That is, only the key combinations ending with '1' are taken into account
    ~ Jump is a special case and thus is added into the bottom of the function
    */
    int keyCombInt = GetKeyCombination();

    switch (keyCombInt)
    {
        /* ON GROUND */
        case KEY_GND_IDLE: // on ground, not move, not attack
            // Do nothing
            break;

        case KEY_GND_MOVE_RIGHT: // on ground, move right, not attack
            DoMoveRight(MOVEMENT_UNIT);
            break;

        case KEY_GND_MOVE_LEFT: // on ground, move left, not attack
            DoMoveLeft(MOVEMENT_UNIT);
            break;

        case KEY_GND_LAND_DOWN: // on ground, land down, not attack
            // Do nothing
            break;

        /* ON AIR */
        case KEY_AIR_IDLE: // on air, not move, not attack
            // Do nothing
            break;

        case KEY_AIR_MOVE_RIGHT: // on air, move right, not attack
            DoMoveRight(MOVEMENT_UNIT);
            break;

        case KEY_AIR_MOVE_LEFT: // on air, move left, not attack
            DoMoveLeft(MOVEMENT_UNIT);
            break;

        case KEY_AIR_LAND_DOWN: // on air, land down, not attack
            DoLand();
            break;

        default:
            break;
    }

    /* JUMP */
    if (_isTriggerJump) // Game logic 'OnMove()' catches the signal jump
    {
        DoJump();

        if (IsOnLeftEdge() || IsOnRightEdge())
        {
            InitiateWallJump();
        }

        _isTriggerJump = false; // Turn off the jump trigger
    }
}

void Player::SetCurrentNonTriggerAnimation()
{
	if (IsOnGround()) // Player is on ground
    {
        if (_isPressingLeft || _isPressingRight) // Player is moving
            SetAnimationStateLeftRight(ANI_ID_RUN_LEFT);
        else // Player is idling
            if (_isHoldingWeapon) // With sword
                if (_dir) // If the player is facing right
                    SetAnimationStateByWeapon(ANI_WPN_ID_STAND_RIGHT);
                else
                    SetAnimationStateByWeapon(ANI_WPN_ID_STAND_LEFT);
            else // Without sword
                SetAnimationStateLeftRight(ANI_ID_STAND_LEFT);
    }
    else // Player is NOT on ground
    {
        if (IsOnLeftEdge()) // Player is leaning on left edge
            SetAnimationState(ANI_ID_LEAN_RIGHT); // Set the leaning animation of player facing right
        else if (IsOnRightEdge()) // Player is leaning on left edge
            SetAnimationState(ANI_ID_LEAN_LEFT); // Set the leaning animation of player facing left
        else if (_isPressingDown) // Player is intentionally landing down
            SetAnimationStateLeftRight(ANI_ID_LAND_FALL_LEFT);
        else   // Player is jumping
            SetAnimationStateLeftRight(ANI_ID_JUMP_LEFT);
    }
}

void Player::AddCamera(Camera * cam)
{
	camera = cam;
}

int Player::Round(double i)
{
    return (int)(i - 0.5);
}

bool Player::StateChanged()
{
	bool ret = false;
	if (_lastTriggeredAniKeyID != GetKeyCombination())
		ret = true;
	_lastTriggeredAniKeyID = GetKeyCombination();
	return ret;
}

bool Player::WpnStateChanged()
{
	bool ret = false;
	if (_lastTriggeredAniByWpnID != _triggeredAniByWpnID)
		ret = true;
	_lastTriggeredAniByWpnID = _triggeredAniByWpnID;
	return ret;
}

}