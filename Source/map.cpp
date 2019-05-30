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
const int MAX_droppingWeapons = 3;

Map::Map()
{
    maxWeapons = MAX_droppingWeapons;
    camera = nullptr;
}
Map::Map(vector<Player*>* players)
{
    maxWeapons = MAX_droppingWeapons;
    _players = players;
    camera = nullptr;
}
Map::~Map()
{
    for (auto element : _grounds)
        delete element;

    ClearDroppingWeapons();
    ClearFlyingWeapons();
}
void Map::OnBeginState()
{
    // Reset the clock
    start = lastTime = clock();
    nextTimeGenerateWeapon = random(3, 10);

    // Reset the camera
    if (camera != nullptr)
        camera->Reset();

    // Clear all the weapons
    ClearDroppingWeapons();
    ClearFlyingWeapons();
}

void Map::ClearFlyingWeapons()
{
    for (auto elementPtr : _flyingWeapons)
        delete elementPtr;

    _flyingWeapons.clear();
}

void Map::ClearDroppingWeapons()
{
    for (auto element : _droppingWeapons)
        delete element;

    _droppingWeapons.clear();
}

void Map::OnInit()
{
    OnBeginState();
}

void Map::EvaluatePickingWeaponOnKeyDown(vector<Weapon*>& weapons, const UINT& nChar)
{
    vector<Weapon*>::iterator erase = weapons.end();

    for (auto i = weapons.begin(); i != weapons.end(); i++)
    {
        (*i)->OnKeyDown(nChar);

        if ((*i)->IsPickedUp())
            erase = i;
    }

    if (erase != weapons.end())
    {
        delete (*erase); // Resolve memory leak of weapons
        weapons.erase(erase);
    }
}

void Map::OnKeyDown(UINT nChar)
{
    /*	~ Remark:
    	~ This function is responsible for implementing players picking up weapons
    */
    /* DROPPING WEAPONS */
    EvaluatePickingWeaponOnKeyDown(_droppingWeapons, nChar);
    /* FLYING WEAPONS */
    EvaluatePickingWeaponOnKeyDown(_flyingWeapons, nChar);
}

void Map::ProcessWeaponsOutOfMap(vector<Weapon*>& weapons)
{
    int leftIndex = 0;
    int rightIndex = weapons.size() - 1;

    // Re-accomodate the pointers to weapons falling off the map into the end of the vector '_droppingWeapons'
    while (leftIndex <= rightIndex)
    {
        while ((leftIndex <= rightIndex) && (weapons[leftIndex]->IsOutMapBorder()))
        {
            /* _droppingWeapons[leftIndex] <-> _droppingWeapons[rightIndex] */
            iter_swap(weapons.begin() + leftIndex, weapons.begin() + rightIndex);
            rightIndex--;
        }

        leftIndex++;
    }

    unsigned int position = 100;

    for (unsigned int index = 0; index < weapons.size(); index++)
    {
        if (weapons[index]->IsOutMapBorder())
        {
            position = index;
            break;
        }
    }

    if (position != 100) // If there is at least a weapon falling off the map
    {
        for (unsigned int index = position; index < weapons.size(); index++)
        {
            delete weapons[position]; // Free the memory allocated by the weapons falling off the map
        }

        weapons.erase(weapons.begin() + position, weapons.begin() + weapons.size()); // Erase the weapons falling off the map in the '_droppingWeapons' vector
    }
}

void Map::OnMove()
{
    /* DROPPPING WEAPONS */
    // After certain amount of time, generates Weapon automatically
    if ((signed int)_droppingWeapons.size() < maxWeapons)
    {
        if ((clock() - lastTime) / CLOCKS_PER_SEC > nextTimeGenerateWeapon)
        {
            Weapon* weapon = new Weapon();
            weapon->AddCamera(camera);
            weapon->Initialize(_grounds, *_players);
            _droppingWeapons.push_back(weapon);
            //
            lastTime = clock();
            nextTimeGenerateWeapon = random(5, 8);
        }
    }

    // Processing the dropping weapons falling out of the map
    ProcessWeaponsOutOfMap(_droppingWeapons);

    for (auto droppingWeaponPtr : _droppingWeapons)
        droppingWeaponPtr->OnMove();

    /* FLYING WEAPONS */
	// Processing the flying weapons falling out of the map
    ProcessWeaponsOutOfMap(_flyingWeapons);
	// Processing the flying weapons expired
    ProcessFlyingWeaponsExpired();

    for (auto flyingWeaponPtr : _flyingWeapons)
        flyingWeaponPtr->OnMove();
}

void Map::ProcessFlyingWeaponsExpired()
{
    int leftIndex = 0;
    int rightIndex = _flyingWeapons.size() - 1;

    // Re-accomodate the pointers to weapons falling off the map into the end of the vector '_droppingWeapons'
    while (leftIndex <= rightIndex)
    {
        while ((leftIndex <= rightIndex) && (_flyingWeapons[leftIndex]->IsExpired()))
        {
            /* _droppingWeapons[leftIndex] <-> _droppingWeapons[rightIndex] */
            iter_swap(_flyingWeapons.begin() + leftIndex, _flyingWeapons.begin() + rightIndex);
            rightIndex--;
        }

        leftIndex++;
    }

    unsigned int position = 100;

    for (unsigned int index = 0; index < _flyingWeapons.size(); index++)
    {
        if (_flyingWeapons[index]->IsExpired())
        {
            position = index;
            break;
        }
    }

    if (position != 100) // If there is at least a weapon falling off the map
    {
        for (unsigned int index = position; index < _flyingWeapons.size(); index++)
        {
            delete _flyingWeapons[position]; // Free the memory allocated by the weapons falling off the map
        }

        _flyingWeapons.erase(_flyingWeapons.begin() + position, _flyingWeapons.begin() + _flyingWeapons.size()); // Erase the weapons falling off the map in the '_droppingWeapons' vector
    }
}

void Map::OnShow()
{
    background.OnShow();

    // Show ground
    for (auto groundPtr : _grounds)
        groundPtr->OnShow();

    // Show weapon
    for (auto droppingWeaponPtr : _droppingWeapons)
        droppingWeaponPtr->OnShow();

    // Show throwing weapons
    for (auto flyingWeaponPtr : _flyingWeapons)
        flyingWeaponPtr->OnShow();
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
vector<Weapon*>* Map::GetDroppingWeapons()
{
    return &_droppingWeapons;
}

void Map::PlayerThrowWeapon(Player* thrower)
{
    Weapon* weaponPtr = new Weapon();
    weaponPtr->AddCamera(camera);
    weaponPtr->Initialize(_grounds, *_players);
    bool throwDir = thrower->GetDirection();

    if (!throwDir) // left
        weaponPtr->SetXY(thrower->GetCor(0) - 100, thrower->GetCor(1) + 10);
    else // right
        weaponPtr->SetXY(thrower->GetCor(2) + 20, thrower->GetCor(1) + 10);

    weaponPtr->Throw(throwDir, thrower);
    // Add the flying weapon to the flying weapons vector
    _flyingWeapons.push_back(weaponPtr);
}
}