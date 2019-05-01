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

/////////////////////////////////////////////////////////////////////////////
// Weapom class
/////////////////////////////////////////////////////////////////////////////

const double ACCELERATION_UNIT = 0.5;
Weapon::Weapon()
{
    _velocityX = _velocityY = start = 0;
    _size = 0.2;
    _tDir = false;
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
	_ground = GetRandomGround(&ground);		// Randomly select Ground
    x = random(_ground->GetCor(0), _ground->GetCor(2) - width);		// Randomly set x coordinate within Ground's width
    y = _ground->GetCor(1) - 400;						// Set y with Ground's top adding 400 pixels
    _player = player;
	sbmp = &bmp;
}

void Weapon::Throw(bool dir, Player* player)
{
	_velocityX = 20;
	_velocityY = 20;
    _throwHost = player;
    _tDir = dir;
	_state = STATE_FLYING;
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

bool Weapon::HasTaken()
{
    return (_state == STATE_HOLDING || _state == STATE_TIMESUP ? true : false);
}

bool Weapon::BeThrown()
{
    return (_state == STATE_FLYING || _state == STATE_HITTED ? true : false);
}

void Weapon::OnShow()
{
	double size = _size;
	int _x = x, _y = y;
	if (camera != nullptr) {
		CPoint cam = camera->GetXY(x, y);
		size = _size * camera->GetSize();
		_x = cam.x; _y = cam.y;
	}

	if (BeThrown() && !_tDir)
		sbmp = &tl;
	else if (BeThrown() && _tDir)
		sbmp = &tr;
	else if (_state == STATE_NORMAL)
		sbmp = &bmp;
	else
		sbmp = nullptr;
	if (sbmp != nullptr) {
		sbmp->SetTopLeft(_x, _y);
		sbmp->OnShow(size);
		width = (int)(sbmp->Width() * _size);
		height = (int)(sbmp->Height() * _size);
	}
}
void Weapon::OnMove()
{
	sbmp->OnMove();
	int groundCmpY = _ground->GetCor(1) - height + 3;
	if (y >= groundCmpY && x >= _ground->GetCor(0) && x < _ground->GetCor(2) - width) // On the ground
	{
		_velocityY = 0;
		if (_velocityX > 0)
			_velocityX -= ACCELERATION_UNIT;
		else {
			_velocityX = 0;
			_state = STATE_NORMAL;
		}
		y = groundCmpY;
	}
	else	// If not yet hitting the ground
	{
		_velocityY += ACCELERATION_UNIT;
		y += (int)(_velocityY * (BeThrown() && x > _ground->GetCor(0) && x < _ground->GetCor(2) - width ? 0.1 : 1.0));
	}
	if (_state == STATE_FLYING)
	{
		static int distance = 0;
		distance += (int)(_velocityX * 2);
		if (distance > 20) {
			sbmp->NextPtr();
			distance = 0;
		}
		Player* _hitPlayer = HitPlayer();
		if (_hitPlayer != nullptr && _hitPlayer != _throwHost)
		{
			_throwHost->PerformAttack(_hitPlayer, _tDir);
			_tDir = !_tDir;
			_velocityX = 5;
			start = clock();
			_state = STATE_HITTED;
		}

		if (_tDir)
			x += (int)(_velocityX * 2);
		else
			x -= (int)(_velocityX * 2);
	}
	if (start != 0 && (clock() - start) / CLOCKS_PER_SEC > 1)
		_state = STATE_TIMESUP;
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
                if(_state != STATE_HITTED)
					_hitPlayer->GenerateAndSetWeaponID();
            }
        }
    }
}
}