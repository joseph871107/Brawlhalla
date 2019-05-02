/* File description:
This is the source code file 'TriggeredAnimation.cpp'. This is the implementation for the class 'TriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "TriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
TriggeredAnimation::~TriggeredAnimation()
{
    /* Body intentionally empty */
}

TriggeredAnimation::TriggeredAnimation() : TriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

TriggeredAnimation::TriggeredAnimation(const TriggeredAnimation& objectValue) : TriggeredAnimation(objectValue._playerPtr, objectValue._keyID)
{
    /* Body intentionally empty */
}

TriggeredAnimation::TriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : _playerPtr(playerPtrValue), _keyID(keyIDValue)
{
    /* Body intentionally empty */
}

TriggeredAnimation& TriggeredAnimation::operator=(const TriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        _playerPtr = rightObject._playerPtr;
        _keyID = rightObject._keyID;
    }

    return (*this);
}

const int& TriggeredAnimation::GetKeyID() const
{
    return (_keyID);
}

void TriggeredAnimation::SetTriggeredAnimationVariables()
{
    _playerPtr->SetTriggeredAnimation(true);
    _playerPtr->SetTriggeredAnimationKeyID(_keyID);
    _playerPtr->SetTriggeredAnimationDir(_playerPtr->GetDirection());
}

}