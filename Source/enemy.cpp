#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"
#include "enemy.h"
namespace game_framework
{
const vector<long> keys{ KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_F, KEY_X };

Enemy::Enemy()
{
    _isPlayer = false;
    _difficulty = 0;
}

Enemy::Enemy(int diff)
{
    _isPlayer = false;
    _difficulty = diff;
}

void Enemy::DoAttack()
{
    if (IsCollide(GetCor(0), GetCor(1), GetCor(2), GetCor(3), target->GetCor(0), target->GetCor(1), target->GetCor(2), target->GetCor(3)))
        OnKeyDown(keys[4]);
    else
        OnKeyUp(keys[4]);
}
bool Enemy::ChaseTarget(CPoint point, int width, int height)
{
	bool tri = true;
	if (GetCor(0) > point.x + width) {
		OnKeyDown(keys[3]);
		tri = false;
	}
    else
        OnKeyUp(keys[3]);

    if (GetCor(2) < point.x) {
        OnKeyDown(keys[1]);
	tri = false;
	}
    else
        OnKeyUp(keys[1]);

    if (GetCor(1) - point.y > 10 && abs(GetCor(0) - point.x) < GetWidth() * 1.5) {
        OnKeyDown(keys[0]);
	tri = false;
	}
    else
        OnKeyUp(keys[0]);

    if (GetCor(2) < point.x){
        OnKeyDown(keys[1]);
	tri = false;
	}
    else
        OnKeyUp(keys[1]);

    if (GetCor(1) - point.y > 10 && abs(GetCor(0) - point.x) < GetWidth() * 1.5) {	// Y of Enemy were lower than Y of target and
        OnKeyDown(keys[0]);
	tri = false;
	}
    else
        OnKeyUp(keys[0]);
	return tri;
}

void Enemy::OnMove()
{
    static int counter = 0, ptr = 0;
    target = (*_playersPtr)[0];
	CPoint targetPos;
	int width, height;
	if (!_isHoldingWeapon && weapons->size()>0) {
		vector<Weapon*>::iterator tar = WeaponNearby();
		if (ChaseTarget(CPoint((*tar)->GetCor(0), (*tar)->GetCor(3)), (*tar)->GetWidth(), (*tar)->GetHeight())) {
			(*tar)->OnKeyDown(KEY_C);
			delete *tar;
			weapons->erase(tar);
		}
	}
	else {
		targetPos = CPoint(target->GetCor(0), target->GetCor(1));
		width = target->GetWidth();
		height = target->GetHeight();
		ChaseTarget(targetPos, width, height);
	}
    DoAttack();
    DoParseKeyPressed();
    _currentKeyID = GetKeyCombination();
    OnMoveAnimationLogic();
    OnMoveGameLogic();
}
vector<Weapon*>::iterator Enemy::WeaponNearby()
{
	vector<Weapon*>::iterator tar = weapons->begin();
	if (weapons->size() > 0)
		tar = weapons->begin();
	for (auto weapon = weapons->begin(); weapon != weapons->end(); weapon++) {
		Vector2 diffO((*tar)->GetCor(0) - GetCor(0), (*tar)->GetCor(1) - GetCor(1));
		Vector2 diffT((*weapon)->GetCor(0) - GetCor(0), (*weapon)->GetCor(1) - GetCor(1));
		if (diffT.GetLength() < diffO.GetLength())
			tar = weapon;
	}
	return tar;
}
}
