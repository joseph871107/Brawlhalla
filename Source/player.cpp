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
const int MOVEMENT_UNIT = 5;
const double ACCELERATION_UNIT = 0.5;
const double INITIAL_VELOCITY = 10.0;
const int OFFSET_INITIAL_VELOCITY = 10;
const char KEY_A = 0x41;
const char KEY_D = 0x44;
const char KEY_L = 0x4C;
const char KEY_W = 0x57;
const char KEY_LEFT = 0x25; // keyboard左箭頭
const char KEY_UP = 0x26; // keyboard上箭頭
const char KEY_RIGHT = 0x27; // keyboard右箭頭
const char KEY_DOWN = 0x28; // keyboard下箭頭

//-----------------FUNCTIONS DEFINITIONS-----------------//

Player::Player() :
    _x(int()), _y(int()), ani(vector<CAnimation>()), currentAni(int()),
    rr(vector<int>()), rl(vector<int>()), jr(vector<int>()), jl(vector<int>()),
    sr(vector<int>()), sl(vector<int>()), ll(vector<int>()), lr(vector<int>()),
    bmp_iter(vector<vector<int>*>()), _width(int()), _height(int()), _isMovingLeft(bool()),
    _isMovingRight(bool()), _dir(bool()), _isTriggerJump(bool()), _jumpCount(bool()),
    _offsetVelocity(int()), _isOffsetLeft(bool()), _isOffsetRight(bool()), _velocity(int()),
    _ground(nullptr)
{
    /* Body intentionally empty */
}

Player::~Player()
{
    _ground = nullptr;
}

void Player::Initialize(Ground* groundPtrValue)
{
    /* Remarks:
    Variables that are later initialized in 'LoadBitmap()'
    'ani', 'currentAni', '_width', '_height', 'bmp_iter' */
    _x = 600;
    _y = 100;
    //
    rr = vector<int> { IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };
    rl = vector<int> { IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };
    jr = vector<int> { IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };
    jl = vector<int> { IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };
    sr = vector<int> { IDB_P1_IDLE0, IDB_P1_IDLE1, IDB_P1_IDLE2 };
    sl = vector<int> { IDB_P1_IDLE0M, IDB_P1_IDLE1M, IDB_P1_IDLE2M };
    ll = vector<int> { IDB_LEAN_LEFT };
    lr = vector<int> { IDB_LEAN_RIGHT };
    //
    _isMovingLeft = _isMovingRight = _dir = false;
    //
    _isTriggerJump = false;
    ResetJumpCount();
    //
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = _isOffsetRight = false;
    //
    _velocity = INITIAL_VELOCITY;
    //
    _ground = groundPtrValue;
}

void Player::LoadBitmap()
{
    AddCAnimation(&rl, 2.5, 7); //ani[0] Run Left
    AddCAnimation(&rr, 2.5, 7); //ani[1] Run Right
    AddCAnimation(&jl, 2.5, 7, false); //ani[2] Jump Left
    AddCAnimation(&jr, 2.5, 7, false); //ani[3] Jump Right
    AddCAnimation(&sl, 2.5); //ani[4] Stand (Idle) Left
    AddCAnimation(&sr, 2.5); //ani[5] Stand (Idle) Right
    AddCAnimation(&ll, 2.5); //ani[6] Lean Left
    AddCAnimation(&lr, 2.5); //ani[7] Lean Right
    //
    _width = ani[0].Width();
    _height = ani[0].Height();
}

void Player::OnShow()
{
    if (IsOnGround())
    {
        if (_isMovingLeft || _isMovingRight) //Player is moving
        {
            if (_dir) //Player is facing right
            {
                SetAnimationState(1);
            }
            else //Player is facing left
            {
                SetAnimationState(0);
            }
        }
        else //Player is idling
        {
            if (_dir) //Player is facing right
            {
                SetAnimationState(5);
            }
            else //Player is facing left
            {
                SetAnimationState(4);
            }
        }
    }
    else
    {
        if (IsOnLeftEdge()) //Player is leaning on left edge
        {
            SetAnimationState(6);
        }
        else if (IsOnRightEdge()) //Player is leaning on left edge
        {
            SetAnimationState(7);
        }
        else   //Player is falling
        {
            if (_dir) //Player is facing right
            {
                SetAnimationState(3);
            }
            else //Player is facing left
            {
                SetAnimationState(2);
            }
        }
    }

    ShowAnimation();
}

void Player::OnMove()
{
    for (auto i = ani.begin(); i != ani.end(); i++) //For all CAnimation objects in 'ani'
        i->OnMove(); //Proceed to the next CMovingBitmap in the CAnimation 'i'

    if (_isMovingLeft)
    {
        _x -= MOVEMENT_UNIT;
    }

    if (_isMovingRight)
    {
        _x += MOVEMENT_UNIT;
    }

    /* JUMP */
    if (_isTriggerJump) //Game logic 'OnMove()' catches the signal jump
    {
        DoJump();
        //Turn off the jump trigger
        _isTriggerJump = false;
    }

    /* WALL JUMP */
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

    /* PLAYER - GROUND */
    int playerX1 = GetX1();
    int playerY1 = GetY1();
    int playerX2 = GetX2();
    int playerY2 = GetY2();
    int groundX1 = _ground->GetCor(0);
    int groundY1 = _ground->GetCor(1);
    int groundX2 = _ground->GetCor(2);
    int groundY2 = _ground->GetCor(3);

    if (!((playerY2 <= groundY1) || (playerY1 >= groundY2))) //If in y-coordinate intersection range
    {
        if ((playerX1 < groundX1) && (groundX1 < playerX2) && (playerX2 < groundX2)) //Out on the left
        {
            _x = groundX1 - _width;
            DoFall();
        }
        else if ((groundX1 < playerX1) && (playerX1 < groundX2) && (groundX2 < playerX2)) //Out on the right
        {
            _x = groundX2;
            DoFall();
        }
        else if (IsOnLeftEdge() || IsOnRightEdge())
        {
            DoOnEdge();
        }
        else if ((groundX1 <= playerX1) && (playerX2 <= groundX2))   //If in ground x-coordinate range, then push up the ground
        {
            _y = groundY1 - _height;
            DoOnGround();
        }
        else //If completely outside the ground
        {
            DoFall();
        }
    }
    else if (playerY2 == groundY1) //If the player has already standed firmly on the ground
    {
        if ((groundX1 <= playerX1) && (playerX2 <= groundX2))
        {
            DoOnGround();
        }
        else
        {
            DoFall();
        }
    }
    else //If neither in y-coordinate intersection range nor on the ground
    {
        DoFall();
    }

    /* FALL OFF THE MAP */

    if (GetY1() > 1000) //If the player is out of the screen, then he will be set on the highest position
    {
        _y = 0;
        _velocity = INITIAL_VELOCITY;
    }
}

void Player::OnKeyDown(const UINT& nChar)
{
    switch (nChar)
    {
        case KEY_A:
            _dir = false;
            _isMovingLeft = true;
            break;

        case KEY_D:
            _dir = true;
            _isMovingRight = true;
            break;

        case KEY_W:
            _isTriggerJump = true;
            break;

        case KEY_LEFT:
            _dir = false;
            _isMovingLeft = true;
            break;

        case KEY_RIGHT:
            _dir = true;
            _isMovingRight = true;
            break;

        case KEY_UP:
            _isTriggerJump = true;
            break;

        default:
            break;
    }
}

void Player::OnKeyUp(const UINT& nChar)
{
    switch (nChar)
    {
        case KEY_A:
            _isMovingLeft = false;
            break;

        case KEY_D:
            _isMovingRight = false;
            break;

        case KEY_LEFT:
            _isMovingLeft = false;
            break;

        case KEY_RIGHT:
            _isMovingRight = false;
            break;

        default:
            break;
    }
}

int Player::GetX1()
{
    return (_x);
}

int Player::GetY1()
{
    return (_y);
}

int Player::GetX2()
{
    return (_x + _width);
}

int Player::GetY2()
{
    return (_y + _height);
}

int Player::ShowAnimationState()
{
    return (*bmp_iter[currentAni])[ani[currentAni].GetCurrentBitmapNumber()];
}

void Player::DoJump()
{
    if (_jumpCount > 0)   //If the player is able to jump more
    {
        _velocity = -INITIAL_VELOCITY;
        _y -= (int)INITIAL_VELOCITY;

        //Wall Jump
        if (IsOnLeftEdge())
        {
            _offsetVelocity = OFFSET_INITIAL_VELOCITY;
            _isOffsetLeft = true;
        }
        else if (IsOnRightEdge())
        {
            _offsetVelocity = OFFSET_INITIAL_VELOCITY;
            _isOffsetRight = true;
        }

        _jumpCount--; //Decrement the jumps available
        ResetJumpAnimations();
    }
}

void Player::DoFall()
{
    _velocity += ACCELERATION_UNIT;
    _y += (int)_velocity;
}

void Player::DoOnGround()
{
    _velocity = 0.0;
    ResetJumpCount();
    ResetJumpAnimations();
}

void Player::DoOnEdge()
{
    _velocity += ACCELERATION_UNIT;
    _y += (int)_velocity;
    ResetJumpCount();
    ResetJumpAnimations();
}

bool Player::IsOnGround()
{
    int playerY2 = GetY2();
    int groundY1 = _ground->GetCor(1);
    return (playerY2 == groundY1);
}

bool Player::IsOnLeftEdge()
{
    int playerX1 = GetX1();
    int playerY1 = GetY1();
    int playerX2 = GetX2();
    int playerY2 = GetY2();
    int groundX1 = _ground->GetCor(0);
    int groundY1 = _ground->GetCor(1);
    int groundX2 = _ground->GetCor(2);
    int groundY2 = _ground->GetCor(3);
    return ((!((playerY2 <= groundY1) || (playerY1 >= groundY2))) && (_x == groundX1 - _width));
}

bool Player::IsOnRightEdge()
{
    int playerX1 = GetX1();
    int playerY1 = GetY1();
    int playerX2 = GetX2();
    int playerY2 = GetY2();
    int groundX1 = _ground->GetCor(0);
    int groundY1 = _ground->GetCor(1);
    int groundX2 = _ground->GetCor(2);
    int groundY2 = _ground->GetCor(3);
    return ((!((playerY2 <= groundY1) || (playerY1 >= groundY2))) && (_x == groundX2));
}

void Player::ResetJumpAnimations()
{
    ani[2].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[2]' (which is Jump Left)
    ani[3].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[2]' (which is Jump Right)
}

void Player::ResetJumpCount()
{
    _jumpCount = 2;
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
    ani_iter->SetTopLeft(_x, _y);
    ani_iter->OnShow();
}

}