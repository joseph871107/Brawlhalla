/* File description:
This is the source code file 'PlayerRespawnState.cpp'. This is the implementation for the class 'PlayerRespawnState'. */

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
#include "PlayerRespawnState.h"

namespace game_framework
{
PlayerRespawnState::~PlayerRespawnState()
{
    /* Body intentionally empty */
}

PlayerRespawnState::PlayerRespawnState() : PlayerRespawnState(nullptr)
{
    /* Body intentionally empty*/
}

PlayerRespawnState::PlayerRespawnState(const PlayerRespawnState& objectValue) : PlayerRespawnState(objectValue._playerPtr)
{
    /* Player intentionally empty */
}

PlayerRespawnState::PlayerRespawnState(Player* const playerPtrValue) : _playerPtr(playerPtrValue)
{
    /* Body intentionally empty */
}

PlayerRespawnState& PlayerRespawnState::operator=(const PlayerRespawnState& rightObject)
{
    if (this != &rightObject)
    {
        _playerPtr = rightObject._playerPtr;
    }

    return (*this);
}

void PlayerRespawnState::OnMoveGameLogic()
{
    Vector2 vectorToRespawnDestination;
    vectorToRespawnDestination.SetXY(_playerPtr->_x, _playerPtr->_y, _playerPtr->_resDestPosX, _playerPtr->_resDestPosY);
    double distance = vectorToRespawnDestination.GetLength();

    if (distance > _playerPtr->_preDistance) // If the player is nearest from the destination, then end respawning
        _playerPtr->InitializeOnRespawn();
    else
    {
        _playerPtr->_preDistance = distance;
        // Move the player
        _playerPtr->_x += _playerPtr->_vectorRespawnMovement.GetX();
        _playerPtr->_y += _playerPtr->_vectorRespawnMovement.GetY();
        // Move the respawn courier of the player
        _playerPtr->_respawnCourier.SetXY(DoubleToInteger(_playerPtr->_x + _playerPtr->GetWidth() / 2.0 - _playerPtr->_respawnCourier.GetWidth() / 2.0),
                                          _playerPtr->_y - DoubleToInteger(_playerPtr->_respawnCourier.GetWidth()));
    }
}

void PlayerRespawnState::OnMoveAnimationLogic()
{
    /*	~ OVERRIDE TRIGGERED ANIMATION
    ~ If the player is dead and changes his state to unconscious,
    ~ then his triggered animation (if any) must be forced to stop.
    */
    if (_playerPtr->_isTriggeredAni)
        _playerPtr->FinishTriggeredAnimationAnimationLogic(); // Compel the triggered animation to finish
}
}