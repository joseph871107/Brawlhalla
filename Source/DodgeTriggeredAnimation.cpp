/* File description:
This is the source code file 'DodgeTriggeredAnimation.cpp'. This is the implementation for the class 'DodgeTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "DodgeTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
DodgeTriggeredAnimation::~DodgeTriggeredAnimation()
{
    /* Body intentionally empty */
}

DodgeTriggeredAnimation::DodgeTriggeredAnimation() : DodgeTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

DodgeTriggeredAnimation::DodgeTriggeredAnimation(const DodgeTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

DodgeTriggeredAnimation::DodgeTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

DodgeTriggeredAnimation& DodgeTriggeredAnimation::operator=(const DodgeTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void DodgeTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void DodgeTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    _playerPtr->SetIsDodging(false);
}

void DodgeTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_DODGE_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_DODGE_LEFT);
}

void DodgeTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerDodge(false);
    _playerPtr->SetIsDodging(true);
}

void DodgeTriggeredAnimation::DoTriggeredAction()
{
    // Do nothing
}

}