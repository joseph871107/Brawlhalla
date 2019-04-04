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
const int MAX_JUMP_COUNT = 2;
const int MOVEMENT_UNIT = 6;
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
const int MAX_ANIMATION_DURATION = 20;

//-----------------FUNCTIONS DEFINITIONS-----------------//

int _round(double i)
{
    return (int)(i - 0.5);
}

Player::Player() :
    _x(int()), _y(int()), ani(vector<CAnimation>()), currentAni(int()),
    rr(vector<int>()), rl(vector<int>()), jr(vector<int>()), jl(vector<int>()),
    sr(vector<int>()), sl(vector<int>()), ll(vector<int>()), lr(vector<int>()),
    al(vector<int>()), ar(vector<int>()), bmp_iter(vector<vector<int>*>()), _width(int()),
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
}

void Player::ResetTriggeredAnimationVariables()
{
    _isTriggeredAni = false;
    _triggeredAniID = -1;
    _triggeredAniCount = 0;
}

void Player::Initialize(vector<Ground*> groundsValue, vector<Player*>* playerPtrValue, string nameValue, vector<long> keysValue)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
    //_x = (int)((groundPtrValue->GetCor(2) + groundPtrValue->GetCor(0)) / 2);
    _x = 700;
    _y = 100;
    /*_size = 2.5;*/
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
}

void Player::LoadBitmap()
{
    rl = vector<int> { IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };
    rr = vector<int> { IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };
    jl = vector<int> { IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };
    jr = vector<int> { IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };
    sl = vector<int> { IDB_P1_IDLE0M, IDB_P1_IDLE1M, IDB_P1_IDLE2M };
    sr = vector<int> { IDB_P1_IDLE0, IDB_P1_IDLE1, IDB_P1_IDLE2 };
    ll = vector<int> { IDB_P1_WALL0, IDB_P1_WALL1 };
    lr = vector<int> { IDB_P1_WALL0M, IDB_P1_WALL1M };
    al = vector<int> { IDB_P1_ATTACK0M, IDB_P1_ATTACK1M, IDB_P1_ATTACK2M, IDB_P1_ATTACK3M, IDB_P1_ATTACK4M };
    ar = vector<int> { IDB_P1_ATTACK0, IDB_P1_ATTACK1, IDB_P1_ATTACK2, IDB_P1_ATTACK3, IDB_P1_ATTACK4 };
    sdl = vector<int> { IDB_P1_SWD_DWR0M, IDB_P1_SWD_DWR1M, IDB_P1_SWD_DWR2M, IDB_P1_SWD_DWR3M, IDB_P1_SWD_DWR3M };
    sdr = vector<int> { IDB_P1_SWD_DWR0, IDB_P1_SWD_DWR1, IDB_P1_SWD_DWR2, IDB_P1_SWD_DWR3, IDB_P1_SWD_DWR3 };
    s2l = vector<int> { IDB_P1_IDLE2_0M, IDB_P1_IDLE2_1M, IDB_P1_IDLE2_2M, IDB_P1_IDLE2_3M };
    s2r = vector<int> { IDB_P1_IDLE2_0, IDB_P1_IDLE2_1, IDB_P1_IDLE2_2, IDB_P1_IDLE2_3 };
    lfl = vector<int> { IDB_P1_FALL0M, IDB_P1_FALL1M };
    lfr = vector<int> { IDB_P1_FALL0, IDB_P1_FALL1 };
    gmal = vector<int> { IDB_P1_GND_MOVE_ATTACK0M, IDB_P1_GND_MOVE_ATTACK1M, IDB_P1_GND_MOVE_ATTACK2M, IDB_P1_GND_MOVE_ATTACK3M, IDB_P1_GND_MOVE_ATTACK4M, IDB_P1_GND_MOVE_ATTACK5M };
    gmar = vector<int> { IDB_P1_GND_MOVE_ATTACK0, IDB_P1_GND_MOVE_ATTACK1, IDB_P1_GND_MOVE_ATTACK2, IDB_P1_GND_MOVE_ATTACK3, IDB_P1_GND_MOVE_ATTACK4, IDB_P1_GND_MOVE_ATTACK5 };
    AddCAnimation(&rl, BITMAP_SIZE); //ani[0] Run Left
    AddCAnimation(&rr, BITMAP_SIZE); //ani[1] Run Right
    AddCAnimation(&jl, BITMAP_SIZE, 5, false); //ani[2] Jump Left
    AddCAnimation(&jr, BITMAP_SIZE, 5, false); //ani[3] Jump Right
    AddCAnimation(&sl, BITMAP_SIZE); //ani[4] Stand (Idle) Left
    AddCAnimation(&sr, BITMAP_SIZE); //ani[5] Stand (Idle) Right
    AddCAnimation(&ll, BITMAP_SIZE); //ani[6] Lean Left
    AddCAnimation(&lr, BITMAP_SIZE); //ani[7] Lean Right
    AddCAnimation(&al, BITMAP_SIZE, 4, false); //ani[8] Attack Left
    AddCAnimation(&ar, BITMAP_SIZE, 4, false); //ani[9] Attack Right
    AddCAnimation(&sdl, BITMAP_SIZE, 5, false); //ani[10] Draw sword Left
    AddCAnimation(&sdr, BITMAP_SIZE, 5, false); //ani[11] Draw sword Right
    AddCAnimation(&s2l, BITMAP_SIZE); //ani[12] Stand (Idle) Left with sword
    AddCAnimation(&s2r, BITMAP_SIZE); //ani[13] Stand (Idle) Right with sword
    AddCAnimation(&lfl, BITMAP_SIZE); //ani[14] Landing Falling Left
    AddCAnimation(&lfr, BITMAP_SIZE); //ani[15] Landing Falling Right
    AddCAnimation(&gmal, BITMAP_SIZE, 4, false); //ani[16] On-Ground-Moving Attack Left
    AddCAnimation(&gmar, BITMAP_SIZE, 4, false); //ani[17] On-Ground-Moving Attack Right
    _collision_box.LoadBitmap(IDB_P1_TEST, RGB(0, 0, 0));
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

void Player::ShowTriggeredAnimation()
{
    SetAnimationState(_triggeredAniID);
}

void Player::ShowNonTriggerAnimations()
{
    if (_isDrawingWeapon) // Special case: Player is drawing weapon
        SetAnimationStateLeftRight(10);
    else if (IsOnGround()) // Player is on ground
    {
        if (_isPressingLeft || _isPressingRight) // Player is moving
            SetAnimationStateLeftRight(0);
        else // Player is idling
            if (_isHoldingWeapon) // With sword
                SetAnimationStateLeftRight(12);
            else // Without sword
                SetAnimationStateLeftRight(4);
    }
    else // Player is NOT on ground
    {
        if (IsOnLeftEdge()) // Player is leaning on left edge
            SetAnimationState(7);
        else if (IsOnRightEdge()) // Player is leaning on left edge
            SetAnimationState(6);
        else if (_isPressingDown) // Player is intentionally landing down
            SetAnimationStateLeftRight(14);
        else   // Player is jumping
            SetAnimationStateLeftRight(2);
    }
}

void Player::OnShow()
{
    if (_isTriggeredAni)
    {
        ShowTriggeredAnimation();
    }
    else
    {
        ShowNonTriggerAnimations();
    }

    ShowAnimation();

    // For showing the "name tag" //
    if (_PLAYER_DEBUG || 1)
    {
        OnShowText(_name.c_str(), GetCor(0) - 10, GetCor(3), 15, RGB(255, 255, 255));
    }///////////////////////////////
}

bool Player::IsIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (playerX2 >= groundX1 && playerX1 <= groundX2 && playerY2 >= groundY1 && playerY1 <= groundY2);
}

bool Player::IsExplicitlyVerticallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (!(playerY2 <= groundY1 || playerY1 >= groundY2));
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

bool Player::IsExplicitlyHorizontallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2)
{
    return (!((playerX2 <= groundX1) || (playerX1 >= groundX2)));
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

bool Player::IsAttacking()
{
    return (_isHoldingWeapon && _isTriggerAttack);
}

bool Player::IsDrawingWeapon()
{
    return (_isDrawingWeapon && !ani[currentAni].IsFinalBitmap());
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

    if (IsOnGround()) /// Comment for future devs: If the player is on edge?
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

    if (IsAttacking())
        keyCombString = keyCombString + "2";
    else
        keyCombString = keyCombString + "1";

    if (IsDrawingWeapon()) /// Comment for future devs: Drawing the weapon is a special case. Do we need to tackle it?
        keyCombString = "0"; // Do nothing

    return (stoi(keyCombString));
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

bool Player::IsFinishedDrawingAnimation()
{
    return (ani[10].IsFinalBitmap() || ani[11].IsFinalBitmap());
}

void Player::ResetAnimations(int leftAnimationID)
{
    ani[leftAnimationID].Reset(); // Reset left animation
    ani[leftAnimationID + 1].Reset(); // Reset right animation
}

void Player::SetTriggeredAnimationVariables(int leftAnimationID)
{
    _isTriggeredAni = true;

    if (_dir)
        _triggeredAniID = leftAnimationID + 1;
    else
        _triggeredAniID = leftAnimationID;

    _triggeredAniCount = 0;
}

void Player::GetTriggeredAnimation()
{
    /*	~ Remarks:
    	~ We care only about key combinations that determines the attack button is pressed and the player is holding a weapon
    	~ That is, only the key combinations ending with '2' are taken into account
    */
    int keyCombInt = GetKeyCombination();

    switch (keyCombInt)
    {
        /* ON GROUND */
        case 112: // on ground, not move, attack
            SetTriggeredAnimationVariables(8);
            break;

        case 122: // on ground, move right, attack
            SetTriggeredAnimationVariables(16);
            break;

        case 132: // on ground, move left, attack
            SetTriggeredAnimationVariables(16);
            break;

        case 142: // on ground, land down, attack
            ///
            break;

        /* ON AIR */
        case 212: // on air, not move, attack
            ///
            break;

        case 222: // on air, move right, attack
            ///
            break;

        case 232: // on air, move left, attack
            ///
            break;

        case 242: // on air, land down, attack
            ///
            break;

        default:
            break;
    }
}

bool Player::IsFinishedTriggeredAnimation()
{
    return (_triggeredAniCount > MAX_ANIMATION_DURATION);
}

void Player::DoTriggeredAnimation()
{
    switch (_triggeredAniID)
    {
        case 8: // Attack Left
            DoAttack();
            break;

        case 9: // Attack Right
            DoAttack();
            break;

        case 16: // On-Ground-Moving Attack Left
            DoAttack();
            //DoMoveLeft(GND_ATTACK_MOVEMENT_UNIT);
            break;

        case 17: // On-Ground-Moving Attack Right
            DoAttack();
            //DoMoveRight(GND_ATTACK_MOVEMENT_UNIT);
            break;

        default:
            break;
    }
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
        case 111: // on ground, not move, not attack
            // Do nothing
            break;

        case 121: // on ground, move right, not attack
            DoMoveRight(MOVEMENT_UNIT);
            break;

        case 131: // on ground, move left, not attack
            DoMoveLeft(MOVEMENT_UNIT);
            break;

        case 141: // on ground, land down, not attack
            // Do nothing
            break;

        /* ON AIR */
        case 211: // on air, not move, not attack
            // Do nothing
            break;

        case 221: // on air, move right, not attack
            DoMoveRight(MOVEMENT_UNIT);
            break;

        case 231: // on air, move left, not attack
            DoMoveLeft(MOVEMENT_UNIT);
            break;

        case 241: // on air, land down, not attack
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

void Player::InitiateTriggeredAnimation()
{
    switch (_triggeredAniID)
    {
        case 8: // Attack Left
            _isTriggerAttack = false;
            break;

        case 9: // Attack Right
            _isTriggerAttack = false;
            break;

        case 16: // On-Ground-Moving Attack Left
            _isTriggerAttack = false;
            InitiateOffsetLeft();
            break;

        case 17: // On-Ground-Moving Attack Right
            _isTriggerAttack = false;
            InitiateOffsetRight();
            break;

        default:
            break;
    }
}

void Player::FinishTriggeredAnimation()
{
    ani[_triggeredAniID].Reset();
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
        GetTriggeredAnimation();

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

void Player::OnMove()
{
    //-----------------ANIMATIONS SECTION-----------------//
    for (auto i = ani.begin(); i != ani.end(); i++) //For all CAnimation objects in 'ani'
        i->OnMove(); //Proceed to the next CMovingBitmap in the CAnimation 'i'

    /* DRAWING ANIMATION */
    if (IsFinishedDrawingAnimation())
    {
        _isDrawingWeapon = false;
        ResetAnimations(10);
    }

    /* JUMP ANIMATION */
    if (IsOnGround() || IsOnLeftEdge() || IsOnRightEdge() || (_isTriggerJump && _jumpCount > 0))
        ResetAnimations(2);

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
        _y += _round(_velocity);
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

//void Player::SetSize(double s)
//{
//    _size = s;
//}

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
}

bool Player::GetHoldWeapon()
{
    return _isHoldingWeapon;
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
            return _y + (int)(_collision_box.Height() * BITMAP_SIZE) ;

        default:
            return NULL;
    }
}

const long& Player::GetAttackKey() const
{
    return (_keys[4]);
}

int Player::ShowAnimationState()
{
    return (*bmp_iter[currentAni])[ani[currentAni].GetCurrentBitmapNumber()];
}

void Player::InitiateOffsetUp()
{
    _velocity = -INITIAL_VELOCITY;
    _y -= _round(INITIAL_VELOCITY); //Trick explaination: By intuition, '_y' of the player should not be
    // modified here, because it is expected to be modified whenever 'Player::OnMove()' is called. However,
    // since the player is currently on the ground, 'Player::OnMove()' will fix its '_y' onto the surface
    // instead of modifying it as expectation. Thus, '_y' must be altered here to set the player jump his ass up!!
}

void Player::DoJump()
{
    if (_jumpCount > 0)   //If the player is able to jump more
    {
        InitiateOffsetUp();
        _jumpCount--; //Decrement the jumps available
    }
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
            (*i)->InitiateOffsetUp();

            if (GetCor(0) > (*i)->GetCor(0))
                (*i)->InitiateOffsetLeft();
            else
                (*i)->InitiateOffsetRight();
        }
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

void Player::ResetJumpCount()
{
    _jumpCount = MAX_JUMP_COUNT;
}

bool Player::IsFinishedAttackAnimation()
{
    return (ani[8].IsFinalBitmap() || ani[9].IsFinalBitmap()
            ||
            ani[16].IsFinalBitmap() || ani[17].IsFinalBitmap());
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

void Player::SetAnimationState(int num)
{
    currentAni = num;

    for (auto i = ani.begin(); i != ani.end(); i++)
    {
        if (i == ani.begin() + num)
            i->SetPause(false);
        else
            i->SetPause(true);
    }
}

void Player::ShowAnimation()
{
    vector<CAnimation>::iterator ani_iter = ani.begin() + currentAni;
    //Calculate and set the position of the player current animation in respect to the collision box's
    ani_iter->SetTopLeft(_x - (int)(_OFFSET_X * BITMAP_SIZE), _y - (int)(_OFFSET_Y * BITMAP_SIZE));

    if (_PLAYER_DEBUG)
    {
        _collision_box.SetTopLeft(_x, _y);		//actual player blocks
        _collision_box.ShowBitmap(BITMAP_SIZE);
    }

    ani_iter->OnShow();
}

bool Player::HitPlayer(Player* player)
{
    int x1 = GetCor(0) - 50, y1 = GetCor(1), x2 = GetCor(2) + 50, y2 = GetCor(3);
    return (player->GetCor(2) >= x1 && player->GetCor(0) <= x2 && player->GetCor(3) >= y1 && player->GetCor(1) <= y2);
}

bool Player::IsOutMapBorder()
{
    return (!(
                ((MAP_BORDER_X1 <= GetCor(0)/*x1*/) && (GetCor(2)/*x2*/ <= MAP_BORDER_X2))
                &&
                ((MAP_BORDER_Y1 <= GetCor(1)/*y1*/) && (GetCor(3)/*y2*/ <= MAP_BORDER_Y2))
            ));
}

void Player::DoDead()
{
    SetHoldWeapon(false);
    _life--;
    /// activate dead effect
}

void Player::DoRespawn()
{
    //If the player is out of the screen, then he will be set on the highest position
    _y = 0;
    _velocity = INITIAL_VELOCITY;
}

bool Player::IsOutOfLife()
{
    return (_life == 0);
}

const int& Player::GetLife() const
{
    return (_life);
}

const string& Player::GetName() const
{
    return (_name);
}

}