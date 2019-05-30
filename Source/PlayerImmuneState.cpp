/* File description:
This is the source code file 'PlayerImmuneState.cpp'. This is the implementation for the class 'PlayerImmuneState'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "player.h"
// Triggered Animation
#include "TriggeredAnimation.h"
#include "AirAttackTriggeredAnimation.h"
#include "AirLandDownAttackTriggeredAnimation.h"
#include "AirMoveLeftAttackTriggeredAnimation.h"
#include "AirMoveRightAttackTriggeredAnimation.h"
#include "DodgeTriggeredAnimation.h"
#include "DrawSwordTriggeredAnimation.h"
#include "GroundAttackTriggeredAnimation.h"
#include "GroundLandDownAttackTriggeredAnimation.h"
#include "GroundMoveLeftAttackTriggeredAnimation.h"
#include "GroundMoveRightAttackTriggeredAnimation.h"
//
#include "PlayerImmuneState.h"
#include "PlayerConsciousState.h"

namespace game_framework
{
PlayerImmuneState::~PlayerImmuneState()
{
    /* Body intentionally empty */
}

PlayerImmuneState::PlayerImmuneState() : PlayerImmuneState(nullptr, int())
{
    /* Body intentionally empty*/
}

PlayerImmuneState::PlayerImmuneState(const PlayerImmuneState& objectValue) : PlayerConsciousState(objectValue), _frameCounter(objectValue._frameCounter)
{
    /* Player intentionally empty */
}

PlayerImmuneState::PlayerImmuneState(Player* const playerPtrValue, int frameCounterValue) : PlayerConsciousState(playerPtrValue), _frameCounter(frameCounterValue)
{
    /* Body intentionally empty */
}

PlayerImmuneState& PlayerImmuneState::operator=(const PlayerImmuneState& rightObject)
{
    if (this != &rightObject)
    {
        PlayerConsciousState::operator=(rightObject);
        _frameCounter = rightObject._frameCounter;
    }

    return (*this);
}

void PlayerImmuneState::OnMoveGameLogic()
{
    if (_frameCounter > Player::MAX_IMMUNE_FRAMES)
    {
        _playerPtr->SetIsDodging(false);
        _frameCounter = 0;
        // Change the state
        _playerPtr->SetState(Player::CONSCIOUS_STATE);
    }
    else
    {
        _playerPtr->SetIsDodging(true);
        _frameCounter++;
    }

    PlayerConsciousState::OnMoveGameLogic();
}

const int& PlayerImmuneState::GetFrameCounter() const
{
    return (_frameCounter);
}
}