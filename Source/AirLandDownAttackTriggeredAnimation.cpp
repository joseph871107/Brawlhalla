/* File description:
This is the source code file 'AirLandDownAttackTriggeredAnimation.cpp'. This is the implementation for the class 'AirLandDownAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "AirLandDownAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
AirLandDownAttackTriggeredAnimation::~AirLandDownAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

AirLandDownAttackTriggeredAnimation::AirLandDownAttackTriggeredAnimation() : AirLandDownAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

AirLandDownAttackTriggeredAnimation::AirLandDownAttackTriggeredAnimation(const AirLandDownAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

AirLandDownAttackTriggeredAnimation::AirLandDownAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

AirLandDownAttackTriggeredAnimation& AirLandDownAttackTriggeredAnimation::operator=(const AirLandDownAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void AirLandDownAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void AirLandDownAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
	_playerPtr->EmptyHitTargetPlayers();
}

void AirLandDownAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
	TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT);
}

void AirLandDownAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
}

void AirLandDownAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}