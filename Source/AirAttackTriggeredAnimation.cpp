/* File description:
This is the source code file 'AirAttackTriggeredAnimation.cpp'. This is the implementation for the class 'AirAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "AirAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
AirAttackTriggeredAnimation::~AirAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

AirAttackTriggeredAnimation::AirAttackTriggeredAnimation() : AirAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

AirAttackTriggeredAnimation::AirAttackTriggeredAnimation(const AirAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

AirAttackTriggeredAnimation::AirAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

AirAttackTriggeredAnimation& AirAttackTriggeredAnimation::operator=(const AirAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void AirAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void AirAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
	_playerPtr->EmptyHitTargetPlayers();
}

void AirAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
	TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_AIR_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_AIR_ATTACK_LEFT);
}

void AirAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
}

void AirAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}