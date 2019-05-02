/* File description:
This is the source code file 'GroundMoveLeftAttackTriggeredAnimation.cpp'. This is the implementation for the class 'GroundMoveLeftAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GroundMoveLeftAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
GroundMoveLeftAttackTriggeredAnimation::~GroundMoveLeftAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

GroundMoveLeftAttackTriggeredAnimation::GroundMoveLeftAttackTriggeredAnimation() : GroundMoveLeftAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

GroundMoveLeftAttackTriggeredAnimation::GroundMoveLeftAttackTriggeredAnimation(const GroundMoveLeftAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

GroundMoveLeftAttackTriggeredAnimation::GroundMoveLeftAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

GroundMoveLeftAttackTriggeredAnimation& GroundMoveLeftAttackTriggeredAnimation::operator=(const GroundMoveLeftAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void GroundMoveLeftAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void GroundMoveLeftAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    // Do nothing
}

void GroundMoveLeftAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
	TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_GND_MOVE_ATTACK_LEFT);
}

void GroundMoveLeftAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
	_playerPtr->InitiateOffsetLeft(OFFSET_INITIAL_VELOCITY);
}

void GroundMoveLeftAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}