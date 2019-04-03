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
const double INITIAL_VELOCITY = 18;
const double INITIAL_ACCELERATION = 1.2;
const double LANDING_ACCELERATION = 2;
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
    _offsetVelocity(int()), _isOffsetLeft(bool()), _isOffsetRight(bool()), _isAttacking(bool()),
    _velocity(double()), _grounds(vector<Ground*>()), _collision_box(CMovingBitmap()), _life(int()),
    _name(string()) // 我覺得之後應該先不用更改這個constructor，好多喔。。。
{
    /* Body intentionally empty */
}

Player::~Player()
{
}

void Player::Initialize(vector<Ground*> groundsValue, vector<Player*>* playerPtrValue, string nameValue, vector<long> keysValue)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
    //_x = (int)((groundPtrValue->GetCor(2) + groundPtrValue->GetCor(0)) / 2);
    _x = 700;
    _y = 100;
    /*_size = 2.5;*/
    //
    _isPressingLeft = _isPressingRight = _isPressingDown  = _isAttacking = _isHoldingWeapon = _isDrawingWeapon = _dir = false;
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
    _grounds = groundsValue;
    _player = playerPtrValue;
    //
    _life = MAX_LIFE;
    //
    _name = nameValue;
    //
    _width = (int)(_collision_box.Width() * BITMAP_SIZE);
    _height = (int)(_collision_box.Height() * BITMAP_SIZE);
    //
    _keys = keysValue;
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
    _collision_box.LoadBitmap(IDB_P1_TEST, RGB(0, 0, 0));
}

void Player::OnShow()
{
    if (_isDrawingWeapon)
    {
        if (_dir) //Player is attacking right
        {
            SetAnimationState(11);
        }
        else //Player is attcking left
        {
            SetAnimationState(10);
        }

        if (ani[currentAni].IsFinalBitmap())
            _isDrawingWeapon = false;
    }
    else if (_isHoldingWeapon && _isAttacking)
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
        if (_isPressingLeft || _isPressingRight) //Player is moving
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
            if (_isHoldingWeapon) //With sword
            {
                if (_dir) //Player is facing right
                {
                    SetAnimationState(13);
                }
                else //Player is facing left
                {
                    SetAnimationState(12);
                }
            }
            else //Without sword
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
    }
    else if (_isPressingDown)
    {
        if (_dir) //Player is facing right
        {
            SetAnimationState(15);
        }
        else //Player is facing left
        {
            SetAnimationState(14);
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
        else   //Player is falling or jumping
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

bool Player::IsWallJumping()
{
    return (_isOffsetLeft || _isOffsetRight);
}

void Player::DoWallJump()
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

void Player::OnMove()
{
    for (auto i = ani.begin(); i != ani.end(); i++) //For all CAnimation objects in 'ani'
        i->OnMove(); //Proceed to the next CMovingBitmap in the CAnimation 'i'

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

    /* LANDING DOWN - Modifying acceleration */
    if (!IsOnGround() && _isPressingDown)
    {
        _acceleration = LANDING_ACCELERATION;
    }
    else
    {
        _acceleration = INITIAL_ACCELERATION;
    }

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

    /* JUMP */
    if (IsOnGround() || IsOnLeftEdge() || IsOnRightEdge())
    {
        ResetJumpCount();
        ResetJumpAnimations();
    }

    if (_isTriggerJump) // Game logic 'OnMove()' catches the signal jump
    {
        DoJump();

        if (IsOnLeftEdge() || IsOnRightEdge())
        {
            SetWallJump();
        }

        _isTriggerJump = false; // Turn off the jump trigger
    }

    if (IsWallJumping()) // Wall Jump
    {
        DoWallJump(); // Modify the x-coordinate of the player
    }

    /* MOVING LEFT / RIGHT */
    if (_isPressingLeft && !(_isHoldingWeapon && _isAttacking || (_isDrawingWeapon && !ani[currentAni].IsFinalBitmap())))
    {
        _x -= MOVEMENT_UNIT;
    }

    if (_isPressingRight && !(_isHoldingWeapon && _isAttacking || (_isDrawingWeapon && !ani[currentAni].IsFinalBitmap())))
    {
        _x += MOVEMENT_UNIT;
    }

    /* ATTACK */
    if (_isHoldingWeapon && _isAttacking)
    {
        DoAttack();
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
        /// Later use to fall down from certain terrain
        _isPressingDown = true;
    }
    else if (nChar == _keys[3]) // Left
    {
        _dir = false;
        _isPressingLeft = true;
    }
    else if (nChar == _keys[4]) //Attack
    {
        _isAttacking = true;
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
    _isAttacking = false;
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

void Player::SetOffsetUp()
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
        SetOffsetUp();
        _jumpCount--; //Decrement the jumps available
        ResetJumpAnimations();
    }
}

void Player::SetOffsetLeft()
{
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = true;
}

void Player::SetOffsetRight()
{
    _offsetVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetRight = true;
}

void Player::SetWallJump()
{
    if (_jumpCount > 0) //If the player is able to jump more
    {
        if (IsOnLeftEdge())
        {
            SetOffsetLeft();
        }
        else if (IsOnRightEdge())
        {
            SetOffsetRight();
        }
    }
}

void Player::DoAttack()
{
    for (auto i = _player->begin(); i != _player->end(); i++)
    {
        if (*i != this && HitPlayer(*i))
        {
            (*i)->SetOffsetUp();

            if (GetCor(0) > (*i)->GetCor(0))
                (*i)->SetOffsetLeft();
            else
                (*i)->SetOffsetRight();
        }
    }

    ResetAttackAnimations();
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