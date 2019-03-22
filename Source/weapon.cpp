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
const int FLASHING[5] = { 4,3,2,1,0 };
Weapon::Weapon()
{
    _bmpID = IDB_WEAPON_FALLING;
    _color = RGB(0, 255, 0);
    _velocity = 0;
    _isHolding = false;
}

void Weapon::LoadBitmap()
{
    bmp.LoadBitmap(_bmpID, _color);
    width = bmp.Width();
    height = bmp.Height();
}

void Weapon::Initialize(vector<Ground*> ground, vector<Player*> player)
{
	_ground = ground[(rand() * 100) % ground.size()];												// Randomly choose one of the ground object
    x = (rand() * 1000) % (int)(_ground->GetWidth() * _ground->GetSize()) + _ground->GetCor(0);		// Randomly set x coordinate within Ground's width
    y = _ground->GetCor(1) - 400;																	// Set y with Ground's top adding 400 pixels
	_player = player;
}

Player * Weapon::HitPlayer()
{
	for (auto i = _player.begin(); i != _player.end(); i++) {
		if (HitRectangle((*i)->GetCor(0), (*i)->GetCor(1), (*i)->GetCor(2), (*i)->GetCor(3)))
			return *i;
	}
	return nullptr;
}

void Weapon::OnShow()
{
    if (!_isHolding)
    {
        bmp.SetTopLeft(x, y - (int)(height * _size));
        bmp.ShowBitmap(_size);
    }
}
void Weapon::OnMove()
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
void Weapon::OnKeyDown(UINT nChar)
{
	if (nChar == 0x43 || nChar == 0xbc) {
		Player* _hitPlayer = HitPlayer();
		if (_hitPlayer != nullptr) {
			_isHolding = true;
			_hitPlayer->SetWeapon(true);
		}
	}
}
}