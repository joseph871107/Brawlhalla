#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"
#include "weapon.h"

namespace game_framework
{
const long STATE_NORMAL = 0;
const long STATE_HOLDING = 1;
const long STATE_FLYING = 2;
const long STATE_HITTED = 3;
const long STATE_TIMESUP = 4;
const double INITIAL_ACCELERATION = 1.2;
const int OFFSET_INITIAL_VELOCITY = 45;
const double INITIAL_VELOCITY = 18;
const int WEAPON_EXPIRED_TIME = 2;

/////////////////////////////////////////////////////////////////////////////
// Weapom class
/////////////////////////////////////////////////////////////////////////////

const double ACCELERATION_UNIT = 0.5;
Weapon::Weapon()
{
    _horizontalVelocity = _verticalVelocity = start = 0;
    _size = 0.2;
    _throwDir = false;
    _state = STATE_NORMAL;
    LoadBitmap();
}

void Weapon::LoadBitmap()
{
    vector<int> tlIDB = { IDB_WEAPON_ROT_L, IDB_WEAPON_ROT_L_45, IDB_WEAPON_ROT_L_90, IDB_WEAPON_ROT_L_135, IDB_WEAPON_ROT_L_180, IDB_WEAPON_ROT_L_225, IDB_WEAPON_ROT_L_270, IDB_WEAPON_ROT_L_315 }, trIDB = { IDB_WEAPON_ROT_L, IDB_WEAPON_ROT_R_315, IDB_WEAPON_ROT_R_270, IDB_WEAPON_ROT_R_225, IDB_WEAPON_ROT_R_180, IDB_WEAPON_ROT_R_135, IDB_WEAPON_ROT_R_90, IDB_WEAPON_ROT_R_45 };
    bmp.AddBitmap(IDB_WEAPON_FALLING, RGB(0, 255, 0));

    for (auto i : tlIDB)
        tl.AddBitmap(i, RGB(0, 255, 0));

    for (auto i : trIDB)
        tr.AddBitmap(i, RGB(0, 255, 0));
}

void Weapon::Initialize(vector<Ground*> ground, vector<Player*> player)
{
    _grounds = ground;
    _ground = GetRandomGround(&ground);		// Randomly select Ground
    x = random(_ground->GetCor(0), _ground->GetCor(2) - width);		// Randomly set x coordinate within Ground's width
    y = _ground->GetCor(1) - 400;						// Set y with Ground's top adding 400 pixels
    _player = player;
    sbmp = &bmp;
    //
    _width = DoubleToInteger(bmp.Width() * _size);
    _height = DoubleToInteger(bmp.Height() * _size);
    //
    _horizontalVelocity = 0;
    _isOffsetLeft = _isOffsetRight = false;
    _verticalVelocity = 0;
}

void Weapon::Throw(bool dir, Player* player)
{
    _throwHost = player;
    _throwDir = dir;

    // Set the initial horizontal velocity of the weapon
    if (_throwDir) // right
        InitiateOffsetRight(abs(OFFSET_INITIAL_VELOCITY));
    else // left
        InitiateOffsetLeft(abs(OFFSET_INITIAL_VELOCITY));

    // Set the initial vertical velocity of the weapon
    _verticalVelocity = player->GetVerticalVelocity() < 0 ? player->GetVerticalVelocity() * 2 : 0; // The weapon flys up as the player throws while jumping up
    // Set the state of the weapon
    _state = STATE_FLYING;
    // Set the expired time of the weapon being thrown
    start = clock();
}

Player* Weapon::HitPlayer()
{
    for (auto i = _player.begin(); i != _player.end(); i++)
    {
        if ((*i)->GetCor(2) >= x && (*i)->GetCor(0) <= x + width && (*i)->GetCor(3) >= y && (*i)->GetCor(1) <= y + height)
            return *i;
    }

    return nullptr;
}

void Weapon::SetState(long state)
{
    _state = state;
}

bool Weapon::IsPickedUp()
{
    return (_state == STATE_HOLDING);
}

bool Weapon::IsExpired()
{
    return (_state == STATE_TIMESUP);
}

bool Weapon::BeThrown()
{
    return (_state == STATE_FLYING || _state == STATE_HITTED ? true : false);
}

void Weapon::OnShow()
{
    double size = _size;
    int tempX = x, tempY = y;

    if (camera != nullptr)
    {
        CPoint cam = camera->GetXY(x, y);
        size = _size * camera->GetSize();
        tempX = cam.x;
        tempY = cam.y;
    }

    if (BeThrown() && !_throwDir)
        sbmp = &tl;
    else if (BeThrown() && _throwDir)
        sbmp = &tr;
    else if (_state == STATE_NORMAL)
        sbmp = &bmp;
    else
        sbmp = nullptr;

    if (sbmp != nullptr)
    {
        sbmp->SetTopLeft(tempX, tempY);
        sbmp->OnShow(size);
        width = (int)(sbmp->Width() * _size);
        height = (int)(sbmp->Height() * _size);
    }
}
void Weapon::OnMove()
{
    sbmp->OnMove();

    if (_state == STATE_FLYING || _state == STATE_HITTED)
        DoWeaponBeingThrown();
    else
        DoWeaponDropbox();
}

void Weapon::DoWeaponBeingThrown()
{
    static int distance = 0; // weapon flying animation loop interval
    distance += (int)(_horizontalVelocity * 2);

    if (distance > 20)
    {
        sbmp->NextPtr();
        distance = 0;
    }

    /*	~ VERTICAL OFFSET
    	~ Gravity
    */
    _verticalVelocity += INITIAL_ACCELERATION;
    y += DoubleToInteger(_verticalVelocity);

    /*	~ HORIZONTAL OFFSET
    	~ Horizontal offset by being thrown
    */
    if (IsBeingOffsetHorizontally())
        DoHorizontalOffset(); // Modify the x-coordinate of the waepon

    /* WEAPON BOUNCES OFF THE GROUNDS */
    int weaponX1 = GetCor(0);
    int weaponY1 = GetCor(1);
    int weaponX2 = weaponX1 + _width;
    int weaponY2 = weaponY1 + _height;

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (groundPtr->IsIntersectGround(weaponX1, weaponY1, weaponX2, weaponY2))
            DoBounceOffGround(weaponX1, weaponY1, weaponX2, weaponY2, groundPtr);
    }

    /* WEAPON HITS ENEMY */
    Player* _hitPlayer = HitPlayer();

    if (_hitPlayer != nullptr && _hitPlayer != _throwHost)
    {
        _throwHost->PerformAttack(_hitPlayer, _throwDir);

        // Weapon bounces off after hit player
        if (_throwDir) // right
        {
            InitiateOffsetLeft(abs(_horizontalVelocity));
            _throwDir = false; // left
        }
        else // left
        {
            InitiateOffsetRight(abs(_horizontalVelocity));
            _throwDir = true; // right
        }

        start = clock(); // If hit, then reset the expired time of the weapon
        _state = STATE_HITTED;
    }

    // Delete the weapon being thrown after its expiry time
    if (start != 0 && (clock() - start) / CLOCKS_PER_SEC > WEAPON_EXPIRED_TIME)
    {
        _state = STATE_TIMESUP;
        start = 0;
    }
}

void Weapon::DoWeaponDropbox()
{
    /* REPOSITION PLAYER ABOUT GROUNDS */
    int weaponX1 = GetCor(0);
    int weaponY1 = GetCor(1);
    int weaponX2 = weaponX1 + _width;
    int weaponY2 = weaponY1 + _height;

    for (auto groundPtr : _grounds)
        if (groundPtr->IsOnGround(weaponX1, weaponY1, weaponX2, weaponY2))
            y = groundPtr->GetCor(1) - _height;

    // Gravity
    if (!IsOnGround())
    {
        _verticalVelocity += INITIAL_ACCELERATION;
        y += DoubleToInteger(_verticalVelocity);
    }
    else
    {
        _verticalVelocity = 0;
    }
}

void Weapon::OnKeyDown(UINT nChar)
{
    Player* _hitPlayer = HitPlayer();

    if (_hitPlayer != nullptr)
    {
        if (nChar == _hitPlayer->GetAttackKey())
        {
            if (!_hitPlayer->GetHoldWeapon())
            {
                _state = STATE_HOLDING;
                _hitPlayer->SetHoldWeapon(true);

                if (_state != STATE_HITTED)
                    _hitPlayer->GenerateAndSetWeaponID();
            }
        }
    }
}

bool Weapon::IsOutMapBorder()
{
    return (!(
                ((MAP_BORDER_X1 <= GetCor(0)/*x1*/) && (GetCor(2)/*x2*/ <= MAP_BORDER_X2))
                &&
                ((MAP_BORDER_Y1 <= GetCor(1)/*y1*/) && (GetCor(3)/*y2*/ <= MAP_BORDER_Y2))
            ));
}

void Weapon::DoBounceOffGround(int weaponX1, int weaponY1, int weaponX2, int weaponY2, Ground* groundPtr)
{
    if (groundPtr->IsOnGroundLeftEdge(weaponX1, weaponY1, weaponX2, weaponY2))
    {
        InitiateOffsetLeft(abs(_horizontalVelocity));
        _throwDir = false; // left
    }
    else if (groundPtr->IsOnGroundRightEdge(weaponX1, weaponY1, weaponX2, weaponY2))
    {
        InitiateOffsetRight(abs(_horizontalVelocity));
        _throwDir = true; // right
    }
    else if (groundPtr->IsOnGroundUnderside(weaponX1, weaponY1, weaponX2, weaponY2))
    {
        InitiateOffsetDown(abs(_verticalVelocity));
    }
    else if (groundPtr->IsOnGround(weaponX1, weaponY1, weaponX2, weaponY2))
    {
        InitiateOffsetUp(abs(_verticalVelocity));
    }
}

void Weapon::InitiateOffsetUp(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = -initialOffsetVelocityMagnitude;
    y -= DoubleToInteger(initialOffsetVelocityMagnitude); //Trick explaination: By intuition, 'y' of the player should not be
    // modified here, because it is expected to be modified whenever 'Player::OnMove()' is called. However,
    // since the player is currently on the ground, 'Player::OnMove()' will fix its 'y' onto the surface
    // instead of modifying it as expectation. Thus, 'y' must be altered here to set the player jump his ass up!!
}

void Weapon::InitiateOffsetDown(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = initialOffsetVelocityMagnitude;
}

void Weapon::InitiateOffsetLeft(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetLeft = true;
    _isOffsetRight = false;
}

void Weapon::InitiateOffsetRight(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetRight = true;
    _isOffsetLeft = false;
}

void Weapon::DoHorizontalOffset()
{
    if (_isOffsetLeft)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            x -= DoubleToInteger(_horizontalVelocity);
        }
        else
        {
            _isOffsetLeft = false;
        }
    }
    else if (_isOffsetRight)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            x += DoubleToInteger(_horizontalVelocity);
        }
        else
        {
            _isOffsetRight = false;
        }
    }
}

bool Weapon::IsBeingOffsetHorizontally()
{
    return (_isOffsetLeft || _isOffsetRight);
}

bool Weapon::IsOnGround()
{
    int weaponX1 = GetCor(0);
    int weaponY1 = GetCor(1);
    int weaponX2 = weaponX1 + _width;
    int weaponY2 = weaponY1 + _height;

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (groundPtr->IsOnGround(weaponX1, weaponY1, weaponX2, weaponY2))
            return true;
    }

    return false;
}
}