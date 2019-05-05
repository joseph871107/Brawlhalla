#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "map.h"

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// Map class
	/////////////////////////////////////////////////////////////////////////////

	Map::Map()
	{
		maxWeapons = 5;
		camera = nullptr;
	}
	Map::Map(vector<Player*>* players)
	{
		maxWeapons = 5;
		_players = players;
		camera = nullptr;
	}
	Map::~Map()
	{
		for (auto element : _weapons)
			delete element;

		for (auto element : _grounds)
			delete element;
	}
	void Map::OnBeginState()
	{
		start = lastTime = clock();
		nextTimeGenerateWeapon = random(3, 10);
		if(camera != nullptr)
			camera->Reset();
		for (auto element : _weapons)
			delete element;
		_weapons.clear();
	}

	void Map::OnInit()
	{
		OnBeginState();
	}

	void Map::OnKeyDown(UINT nChar)
	{
		// If player takes the weapon //
		vector<Weapon*>::iterator erase = _weapons.end();

		for (auto i = _weapons.begin(); i != _weapons.end(); i++)
		{
			(*i)->OnKeyDown(nChar);

			if ((*i)->HasTaken())
				erase = i;
		}

		if (erase != _weapons.end())
		{
			delete (*erase); //Resolve memory leak of weapons
			_weapons.erase(erase);
		}
	}

	void Map::OnMove()
	{
		// After certain amount of time, generates Weapon automatically //
		if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon)
		{
			if ((signed int)_weapons.size() < maxWeapons)
			{
				Weapon* weapon = new Weapon();
				weapon->AddCamera(camera);
				weapon->Initialize(_grounds, *_players);
				_weapons.push_back(weapon);
			}

			lastTime = clock();
			nextTimeGenerateWeapon = random(5, 8);
		}/////////////////////////////////////////////////////////////////

		for (auto weapon : _weapons)
		{
			weapon->OnMove();
		}
	}

	void Map::OnShow()
	{
		background.OnShow();

		// Show ground
		for (auto i : _grounds)
		{
			i->OnShow();
		}

		// Show weapon
		for (auto i : _weapons)
		{
			i->OnShow();
		}
	}

	void Map::AddCamera(Camera * cam)
	{
		camera = cam;
	}

	void Map::AddBackground(BkPARM * i)
	{
		int x = i->point.x, y = i->point.y;
		background.AddCamera(camera);
		background.LoadBitmap(i->_id, RGB(0, 0, 0));
		if(x == 0 && y == 0)
			background.SetXY(-background.GetWidth(), -background.GetHeight());
		else
			background.SetXY(x, y);
		background.SetSize(i->_size);
		background.SetOffset(i->_offset);
	}

	void Map::AddGround(GroundPARM *i)
	{
		Ground* ground = new Ground();
		ground->AddCamera(camera);
		ground->SetID(i->_id);
		ground->LoadBitmap();
		ground->SetXY(i->point.x, i->point.y);
		ground->SetSize(i->_size);
		ground->SetLen(i->_lengthX, 0);
		ground->SetLen(i->_lengthY, 1);
		ground->SetOffset(i->_offset);
		ground->SetLenOffset(i->_lenOffsetX, 0);
		ground->SetLenOffset(i->_lenOffsetY, 1);
		_grounds.push_back(ground);
	}

	void Map::AddPlayers(vector<Player*>* players)
	{
		_players = players;
	}

	int Map::GetTime()
	{
		return (clock() - start) / CLOCKS_PER_SEC;
	}
	vector<Ground*>* Map::GetGrounds()
	{
		return &_grounds;
	}
	vector<Weapon*>* Map::GetWeapons()
	{
		return &_weapons;
	}
}