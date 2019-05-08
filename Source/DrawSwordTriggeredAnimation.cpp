/* File description:
This is the source code file 'DrawSwordTriggeredAnimation.cpp'. This is the implementation for the class 'DrawSwordTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "DrawSwordTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
DrawSwordTriggeredAnimation::~DrawSwordTriggeredAnimation()
{
    /* Body intentionally empty */
}

DrawSwordTriggeredAnimation::DrawSwordTriggeredAnimation() : DrawSwordTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

DrawSwordTriggeredAnimation::DrawSwordTriggeredAnimation(const DrawSwordTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

DrawSwordTriggeredAnimation::DrawSwordTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

DrawSwordTriggeredAnimation& DrawSwordTriggeredAnimation::operator=(const DrawSwordTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void DrawSwordTriggeredAnimation::SetTriggeredAnimationSelector()
{
    _playerPtr->SetAnimationSelector(true);
}

void DrawSwordTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    // Do nothing
}

void DrawSwordTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_DRAW_SWORD_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_DRAW_SWORD_LEFT);
}

void DrawSwordTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
    _playerPtr->SetIsTriggerDrawWeapon(false);
}

void DrawSwordTriggeredAnimation::DoTriggeredAction()
{
    // Do nothing
}

}