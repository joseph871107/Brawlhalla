#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "weapon.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Weapom class
/////////////////////////////////////////////////////////////////////////////

const double ACCELERATION_UNIT = 0.8;
Weapon::Weapon()
{
    _bmpID = IDB_WEAPON_FALLING;
    _throwLID = IDB_WEAPON_THROWING_L;
    _throwRID = IDB_WEAPON_THROWING_R;
    _color = RGB(0, 255, 0);
    _velocity = 0;
    _size = 0.2;
    _isHolding = _isThrowing = _tDir = false;
    LoadBitmap();
}

void Weapon::LoadBitmap()
{
    bmp.LoadBitmap(_bmpID, _color);
    tl.LoadBitmap(_throwLID, _color);
    tr.LoadBitmap(_throwRID, _color);
    width = bmp.Width();
    height = bmp.Height();
}

void Weapon::Initialize(vector<Ground*> ground, vector<Player*> player)
{
    _ground = ground[(rand() * 100) % ground.size()];												// Randomly choose one of the ground object
    x = (rand() * 1000) % (int)(_ground->GetWidth() * _ground->GetSize()) + _ground->GetCor(0);		// Randomly set x coordinate within Ground's width
    y = _ground->GetCor(1) - 400;																	// Set y with Ground's top adding 400 pixels
    _player = player;
    _flyingDistance = 0;
}

void Weapon::Throw(bool dir, Player* player)
{
    _velocity = 30;
    _throwHost = player;
    _isThrowing = true;
    _tDir = dir;

    if (_tDir)
    {
        width = tl.Width();
        height = tl.Height();
    }
    else
    {
        width = tr.Width();
        height = tr.Height();
    }
}

Player* Weapon::HitPlayer()
{
    for (auto i = _player.begin(); i != _player.end(); i++)
    {
        if (HitRectangle((*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3)))
            return *i;
    }

    return nullptr;
}

bool Weapon::HasTaken()
{
    return _isHolding;
}

bool Weapon::BeThrowen()
{
    return _isThrowing;
}

void Weapon::OnShow()
{
	CPoint cam = camera->GetXY(x, y);
    if (_isThrowing && !_tDir)
    {
        tl.SetTopLeft(cam.x, cam.y);
        tl.ShowBitmap(_size * camera->GetSize());
    }
    else if (_isThrowing && _tDir)
    {
        tr.SetTopLeft(cam.x, cam.y);
        tr.ShowBitmap(_size * camera->GetSize());
    }
    else if (!_isHolding)
    {
		cam = camera->GetXY(x, y - (int)(height * _size));
        bmp.SetTopLeft(cam.x, cam.y);
        bmp.ShowBitmap(_size * camera->GetSize());
    }
}
void Weapon::OnMove()
{
    if (!_isThrowing)
    {
        if (y < _ground->GetCor(1))	// If not yet hitting the ground
        {
            _velocity += ACCELERATION_UNIT;
            y += (int)_velocity;
        }
        else // On the ground
        {
            _velocity = 0;
            y = _ground->GetCor(1);
        }
    }
    else
    {
        Player* _hitPlayer = HitPlayer();
        _flyingDistance += (int)_velocity;

        if (_flyingDistance > 500)
            _isThrowing = false;
        else if (_hitPlayer != nullptr && _hitPlayer != _throwHost)
        {
            _isThrowing = false;
            _hitPlayer->BeenAttacked(_tDir);
        }

        if (_tDir)
        {
            _velocity -= ACCELERATION_UNIT;
            x += (int)_velocity;
        }
        else
        {
            _velocity -= ACCELERATION_UNIT;
            x -= (int)_velocity;
        }
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
                _isHolding = true;
                _hitPlayer->SetHoldWeapon(true);
                _hitPlayer->GenerateAndSetWeaponID();
            }
        }
    }
}
}