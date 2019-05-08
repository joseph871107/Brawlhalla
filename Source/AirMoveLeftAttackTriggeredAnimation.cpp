/* File description:
This is the source code file 'AirMoveLeftAttackTriggeredAnimation.cpp'. This is the implementation for the class 'AirMoveLeftAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "AirMoveLeftAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
AirMoveLeftAttackTriggeredAnimation::~AirMoveLeftAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

AirMoveLeftAttackTriggeredAnimation::AirMoveLeftAttackTriggeredAnimation() : AirMoveLeftAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

AirMoveLeftAttackTriggeredAnimation::AirMoveLeftAttackTriggeredAnimation(const AirMoveLeftAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

AirMoveLeftAttackTriggeredAnimation::AirMoveLeftAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

AirMoveLeftAttackTriggeredAnimation& AirMoveLeftAttackTriggeredAnimation::operator=(const AirMoveLeftAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void AirMoveLeftAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void AirMoveLeftAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    _playerPtr->EmptyHitTargetPlayers();
}

void AirMoveLeftAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT);
}

void AirMoveLeftAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
    _playerPtr->InitiateOffsetLeft(Player::OFFSET_INITIAL_VELOCITY);
}

void AirMoveLeftAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}