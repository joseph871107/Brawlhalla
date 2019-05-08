/* File description:
This is the source code file 'GroundLandDownAttackTriggeredAnimation.cpp'. This is the implementation for the class 'GroundLandDownAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GroundLandDownAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
GroundLandDownAttackTriggeredAnimation::~GroundLandDownAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

GroundLandDownAttackTriggeredAnimation::GroundLandDownAttackTriggeredAnimation() : GroundLandDownAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

GroundLandDownAttackTriggeredAnimation::GroundLandDownAttackTriggeredAnimation(const GroundLandDownAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

GroundLandDownAttackTriggeredAnimation::GroundLandDownAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

GroundLandDownAttackTriggeredAnimation& GroundLandDownAttackTriggeredAnimation::operator=(const GroundLandDownAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void GroundLandDownAttackTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void GroundLandDownAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
	_playerPtr->EmptyHitTargetPlayers();
}

void GroundLandDownAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
	TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_SLIDE_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(ANI_WPN_ID_SLIDE_ATTACK_LEFT);
}

void GroundLandDownAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
	if (_playerPtr->GetTriggeredAnimationDirection())
		_playerPtr->InitiateOffsetRight(OFFSET_INITIAL_VELOCITY);
	else
		_playerPtr->InitiateOffsetLeft(OFFSET_INITIAL_VELOCITY);
}

void GroundLandDownAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();
}

}