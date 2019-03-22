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
const int MOVEMENT_UNIT = 7;
const double ACCELERATION_UNIT = 0.2;
const double INITIAL_VELOCITY = 8;
const int OFFSET_INITIAL_VELOCITY = 8;
const long KEY_A = 0x41;
const long KEY_D = 0x44;
const long KEY_L = 0x4C;
const long KEY_W = 0x57;
const long KEY_LEFT = 0x25; // keyboard左箭頭
const long KEY_UP = 0x26; // keyboard上箭頭
const long KEY_RIGHT = 0x27; // keyboard右箭頭
const long KEY_DOWN = 0x28; // keyboard下箭頭
const long KEY_SPACE = 0x20;
const long KEY_C = 0x43;
const long KEY_COMMA = 0xbc;
const double COLLISION_ERRORS = 1.0;
const int _OFFSET_X = 20;
const int _OFFSET_Y = 7;
const int MAX_LIFE = 3;
const int MAP_BORDER_OFFSET = 100;
const int MAP_BORDER_X1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_Y1 = -MAP_BORDER_OFFSET;
const int MAP_BORDER_X2 = SIZE_X + MAP_BORDER_OFFSET;
const int MAP_BORDER_Y2 = SIZE_Y + MAP_BORDER_OFFSET;
const double BITMAP_SIZE = 2.5;

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
    _height(int()), _keyMode(int()), _keyModeBool(vector<bool>()), _isMovingLeft(bool()),
    _isMovingRight(bool()), _dir(bool()), _isTriggerJump(bool()), _jumpCount(bool()),
    _offsetVelocity(int()), _isOffsetLeft(bool()), _isOffsetRight(bool()), _isAttacking(bool()),
    _velocity(double()), _ground(vector<Ground*>()), _collision_box(CMovingBitmap()), _life(int()),
    _name(string()) // 我覺得之後應該先不用更改這個constructor，好多喔。。。
{
    /* Body intentionally empty */
}

Player::~Player()
{
}

void Player::Initialize(vector<Ground*> groundPtrValue, string nameValue, int i)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
    //_x = (int)((groundPtrValue->GetCor(2) + groundPtrValue->GetCor(0)) / 2);
    _x = 700;
    _y = 100;
    /*_size = 2.5;*/
    SetKeyMode(i);
    //
    //
    _isMovingLeft = _isMovingRight = _isAttacking = _dir = false;
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
    //
    _life = MAX_LIFE;
    //
    _name = nameValue;
    //
    _width = (int)(_collision_box.Width() * BITMAP_SIZE);
    _height = (int)(_collision_box.Height() * BITMAP_SIZE);
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
    _collision_box.LoadBitmap(IDB_P1_TEST, RGB(0, 0, 0));
}

void Player::OnShow()
{
    if (_isAttacking)
    {
        if (_dir) //Player is attacking right
        {
            SetAnimationState(9);
        }
        else //Player is attcking left
        {
            SetAnimationState(8);
        }
    }
    else if (IsOnGround())
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
            SetAnimationState(7);
        }
        else if (IsOnRightEdge()) //Player is leaning on left edge
        {
            SetAnimationState(6);
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

    // For showing the "name tag"
    if (_PLAYER_DEBUG)
    {
        char str[80];
        sprintf(str, "Player%d", _keyMode);
        OnShowText(str, GetCor(0) - 10, GetCor(3), 15);
    }
}

void Player::OnMove()
{
    for (auto i = ani.begin(); i != ani.end(); i++) //For all CAnimation objects in 'ani'
        i->OnMove(); //Proceed to the next CMovingBitmap in the CAnimation 'i'

    /* PLAYER - GROUND */
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

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
            else if ((IsOnLeftEdge() || IsOnRightEdge()) && !IsOnGround())
            {
                DoOnEdge();
            }
            else if ((groundX1 <= playerX1) && (playerX2 <= groundX2))   //If in ground x-coordinate range
            {
                if ((groundY1 - playerY1 < COLLISION_ERRORS) && (playerY1 - groundY2 < COLLISION_ERRORS) && (groundY2 - playerY2 < COLLISION_ERRORS)) //If the player is trying to jump up from beneath the ground, then denies it
                {
                    _y = groundY2;
                    DoFall();
                }
                else   //If the player is falling down onto the ground, then make him stand firmly on the ground
                {
                    _y = groundY1 - _height;
                    DoOnGround();
                }
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
                _y = groundY1 - _height;
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
    }

    if (_isMovingLeft && !_isAttacking)
    {
        _x -= MOVEMENT_UNIT;
    }

    if (_isMovingRight && !_isAttacking)
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

    /* ATTACK */
    if (_isAttacking)
    {
        DoAttack();
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

    /* FALL OFF THE MAP */

    if (IsOutMapBorder())
    {
        DoDead();
        DoRespawn();
    }
}

//void Player::SetSize(double s)
//{
//    _size = s;
//}

void Player::OnKeyDown(const UINT& nChar)
{
    switch (nChar)
    {
        case KEY_A:
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _dir = false;
                _isMovingLeft = true;
            }

            break;

        case KEY_D:
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _dir = true;
                _isMovingRight = true;
            }

            break;

        case KEY_W:
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _isTriggerJump = true;
            }

            break;

        case KEY_LEFT:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _dir = false;
                _isMovingLeft = true;
            }

            break;

        case KEY_RIGHT:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _dir = true;
                _isMovingRight = true;
            }

            break;

        case KEY_UP:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _isTriggerJump = true;
            }

            break;

        case KEY_SPACE:
            if (_keyModeBool[0])
                _isTriggerJump = true;

            break;

        case KEY_C:
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _isAttacking = true;
            }

            break;

        case KEY_COMMA:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _isAttacking = true;
            }

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
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _isMovingLeft = false;
            }

            break;

        case KEY_D:
            if (_keyModeBool[0] || _keyModeBool[1])
            {
                _isMovingRight = false;
            }

            break;

        case KEY_LEFT:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _isMovingLeft = false;
            }

            break;

        case KEY_RIGHT:
            if (_keyModeBool[0] || _keyModeBool[2])
            {
                _isMovingRight = false;
            }

            break;

        default:
            break;
    }
}

void Player::SetKeyMode(int i)
{
    _keyMode = i;

    for (int i = 0; i < 3; i++)
        _keyModeBool.push_back(false);

    _keyModeBool[i] = true;
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

int Player::ShowAnimationState()
{
    return (*bmp_iter[currentAni])[ani[currentAni].GetCurrentBitmapNumber()];
}

void Player::DoJump()
{
    if (_jumpCount > 0)   //If the player is able to jump more
    {
        _velocity = -INITIAL_VELOCITY;
        _y -= _round(INITIAL_VELOCITY);

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
    //for (int i = 0; i < (int)_velocity;i++) {
    _y += _round(_velocity) ;
    //}
}

void Player::DoAttack()
{
    ResetAttackAnimations();
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
    _y += _round(_velocity);
    ResetJumpCount();
    ResetJumpAnimations();
}

bool Player::IsOnGround()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0), groundX2 = (*i)->GetCor(2), groundY1 = (*i)->GetCor(1);

        if (abs(playerY2 - groundY1) <= COLLISION_ERRORS && playerX1 >= groundX1 && playerX2 <= groundX2)
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

    for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
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

    for (vector<Ground*>::iterator i = _ground.begin(); i != _ground.end(); i++)
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

void Player::ResetJumpAnimations()
{
    ani[2].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[2]' (which is Jump Left)
    ani[3].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[3]' (which is Jump Right)
}

void Player::ResetJumpCount()
{
    _jumpCount = MAX_JUMP_COUNT;
}

void Player::ResetAttackAnimations()
{
    if (ani[8].IsFinalBitmap() || ani[9].IsFinalBitmap())
    {
        ani[8].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[8]' (which is Attack Left)
        ani[9].Reset(); //Reset to the first CMovingBitmap in the CAnimation 'ani[9]' (which is Attack Right)
        //Turn off the attack trigger
        _isAttacking = false;
    }
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