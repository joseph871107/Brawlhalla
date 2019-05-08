/* File description:
This is the source code file 'AirMoveRightAttackTriggeredAnimation.cpp'. This is the implementation for the class 'AirMoveRightAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "AirMoveRightAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
AirMoveRightAttackTriggeredAnimation::~AirMoveRightAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

AirMoveRightAttackTriggeredAnimation::AirMoveRightAttackTriggeredAnimation() : AirMoveRightAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

AirMoveRightAttackTriggeredAnimation::AirMoveRightAttackTriggeredAnimation(const AirMoveRightAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

AirMoveRightAttackTriggeredAnimation::AirMoveRightAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

AirMoveRightAttackTriggeredAnimation& AirMoveRightAttackTriggeredAnimation::operator=(const AirMoveRightAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void AirMoveRightAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void AirMoveRightAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    _playerPtr->EmptyHitTargetPlayers();
}

void AirMoveRightAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT);
}

void AirMoveRightAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
    _playerPtr->InitiateOffsetRight(Player::OFFSET_INITIAL_VELOCITY);
}

void AirMoveRightAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}