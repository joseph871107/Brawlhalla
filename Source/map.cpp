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
const int MAX_WEAPONS = 3;

Map::Map()
{
    maxWeapons = MAX_WEAPONS;
    camera = nullptr;
}
Map::Map(vector<Player*>* players)
{
    maxWeapons = MAX_WEAPONS;
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

    if (camera != nullptr)
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

void Map::ProcessWeaponOutOfMap()
{
    int leftIndex = 0;
    int rightIndex = _weapons.size() - 1;

    // Re-accomodate the pointers to weapons falling off the map into the end of the vector '_weapons'
    while (leftIndex <= rightIndex)
    {
        while ((leftIndex <= rightIndex) && (_weapons[leftIndex]->IsOutMapBorder()))
        {
            /* _weapons[leftIndex] <-> _weapons[rightIndex] */
            iter_swap(_weapons.begin() + leftIndex, _weapons.begin() + rightIndex);
            rightIndex--;
        }

        leftIndex++;
    }

    unsigned int position = 100;

    for (unsigned int index = 0; index < _weapons.size(); index++)
    {
        if (_weapons[index]->IsOutMapBorder())
        {
            position = index;
            break;
        }
    }

    if (position != 100) // If there is at least a weapon falling off the map
    {
        for (unsigned int index = position; index < _weapons.size(); index++)
        {
            delete _weapons[position]; // Free the memory allocated by the weapons falling off the map
        }

        _weapons.erase(_weapons.begin() + position, _weapons.begin() + _weapons.size()); // Erase the weapons falling off the map in the '_weapons' vector
    }
}

void Map::OnMove()
{
    // After certain amount of time, generates Weapon automatically //
    /// DEBUG: It's better to have this condition here instead of nesting it inside
    /// if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon)
    /// because it would not have to re-calculate the 'lastTime' clock so many times
    if ((signed int)_weapons.size() < maxWeapons)
    {
        if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon)
        {
            Weapon* weapon = new Weapon();
            weapon->AddCamera(camera);
            weapon->Initialize(_grounds, *_players);
            _weapons.push_back(weapon);
            //
            lastTime = clock();
            nextTimeGenerateWeapon = random(5, 8);
        }
    }/////////////////////////////////////////////////////////////////

    for (auto weapon : _weapons)
    {
        weapon->OnMove();
    }

    /* WEAPON OUT OF MAP */
    /// DEBUG: Comment for future devs: The weapons flying outside of the map need to be processed
    // ProcessWeaponOutOfMap();
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

void Map::AddCamera(Camera* cam)
{
    camera = cam;
}

void Map::AddBackground(BkPARM* i)
{
    int x = i->point.x, y = i->point.y;
    background.AddCamera(camera);
    background.LoadBitmap(i->_id, RGB(0, 0, 0));

    if (x == 0 && y == 0)
        background.SetXY(-background.GetWidth(), -background.GetHeight());
    else
        background.SetXY(x, y);

    background.SetSize(i->_size);
    background.SetOffset(i->_offset);
}

void Map::AddGround(GroundPARM* i)
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