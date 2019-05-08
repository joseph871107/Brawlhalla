/* File description:
This is the source code file 'GroundAttackTriggeredAnimation.cpp'. This is the implementation for the class 'GroundAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GroundAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
GroundAttackTriggeredAnimation::~GroundAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

GroundAttackTriggeredAnimation::GroundAttackTriggeredAnimation() : GroundAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

GroundAttackTriggeredAnimation::GroundAttackTriggeredAnimation(const GroundAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

GroundAttackTriggeredAnimation::GroundAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

GroundAttackTriggeredAnimation& GroundAttackTriggeredAnimation::operator=(const GroundAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void GroundAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void GroundAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    _playerPtr->EmptyHitTargetPlayers();
}

void GroundAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_ATTACK_LEFT);
}

void GroundAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
}

void GroundAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}