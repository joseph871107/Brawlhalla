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
void Enemy::ChaseTarget()
{
    if (GetCor(0) > target->GetCor(2))
        OnKeyDown(keys[3]);
    else
        OnKeyUp(keys[3]);

    if (GetCor(2) < target->GetCor(0))
        OnKeyDown(keys[1]);
    else
        OnKeyUp(keys[1]);

    if (GetCor(1) - target->GetCor(1) > 10 && abs(GetCor(0) - target->GetCor(0)) < GetWidth() * 1.5)
        OnKeyDown(keys[0]);
    else
        OnKeyUp(keys[0]);

    if (GetCor(2) < target->GetCor(0))
        OnKeyDown(keys[1]);
    else
        OnKeyUp(keys[1]);

    if (GetCor(1) - target->GetCor(1) > 10 && abs(GetCor(0) - target->GetCor(0)) < GetWidth() * 1.5)	// Y of Enemy were lower than Y of target and
        OnKeyDown(keys[0]);
    else
        OnKeyUp(keys[0]);
}

void Enemy::OnMove()
{
    static int counter = 0, ptr = 0;
    target = (*_playersPtr)[0];
    ChaseTarget();
    DoAttack();
    DoParseKeyPressed();
    _currentKeyID = GetKeyCombination();
    OnMoveAnimationLogic();
    OnMoveGameLogic();
}
}
