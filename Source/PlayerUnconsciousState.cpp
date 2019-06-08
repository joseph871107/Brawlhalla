/* File description:
This is the source code file 'PlayerUnconsciousState.cpp'. This is the implementation for the class 'PlayerUnconsciousState'. */

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
#include "PlayerUnconsciousState.h"

namespace game_framework
{
PlayerUnconsciousState::~PlayerUnconsciousState()
{
    /* Body intentionally empty */
}

PlayerUnconsciousState::PlayerUnconsciousState() : PlayerUnconsciousState(nullptr)
{
    /* Body intentionally empty*/
}

PlayerUnconsciousState::PlayerUnconsciousState(const PlayerUnconsciousState& objectValue) : PlayerUnconsciousState(objectValue._playerPtr)
{
    /* Player intentionally empty */
}

PlayerUnconsciousState::PlayerUnconsciousState(Player* const playerPtrValue) : _playerPtr(playerPtrValue)
{
    /* Body intentionally empty */
}

PlayerUnconsciousState& PlayerUnconsciousState::operator=(const PlayerUnconsciousState& rightObject)
{
    if (this != &rightObject)
    {
        _playerPtr = rightObject._playerPtr;
    }

    return (*this);
}

void PlayerUnconsciousState::OnMoveGameLogic()
{
    //-----------------POSITION TRANSFORMATION SECTION-----------------//
    /* PLAYER BOUNCES OFF THE GROUNDS */
    int playerX1 = _playerPtr->GetCor(0);
    int playerY1 = _playerPtr->GetCor(1);
    int playerX2 = _playerPtr->GetCor(2);
    int playerY2 = _playerPtr->GetCor(3);

    for (auto groundPtr : _playerPtr->_grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (groundPtr->IsIntersectGround(playerX1, playerY1, playerX2, playerY2))
            DoBounceOffGround(playerX1, playerY1, playerX2, playerY2, groundPtr);
    }

    /*	~ VERTICAL OFFSET
    	~ Gravity
    */
    _playerPtr->_verticalVelocity += Player::INITIAL_ACCELERATION;
    _playerPtr->_y += DoubleToInteger(_playerPtr->_verticalVelocity);

    /*	~ HORIZONTAL OFFSET
    	~ Horizontal offset by being hit
    */
    if (_playerPtr->IsBeingOffsetHorizontally())
        _playerPtr->DoHorizontalOffset(); // Modify the x-coordinate of the player

    //-----------------CONSCIOUS STATE RESOLVE SECTION-----------------//
    /*	~ SET MAX UNCONSCIOUS FRAMES
    	~ Determine the maximum duration for the unconscious state of the player
    	~ when he gets hit, based on '_takenDmg' - the total damages he has taken.
    */
    int maxUnconsciousFrames = Player::INITIAL_MAX_CONSCIOUS_FRAME;
    int specializedTakenDamage = _playerPtr->GetSpecializedTakenDamage();

    if (specializedTakenDamage > maxUnconsciousFrames)
        maxUnconsciousFrames = specializedTakenDamage;

    /* ESTIMATE CURRENT UNCONSCIOUS FRAMES COUNT */
    _playerPtr->_unconsciousFramesCount++; // Increment the current unconscious frames count

    if (_playerPtr->_unconsciousFramesCount == maxUnconsciousFrames)
        _playerPtr->SetConscious();
}

void PlayerUnconsciousState::OnMoveAnimationLogic()
{
    /*	~ RESET UNCONSCIOUSLY FLYING ANIMATION
    	~ Continuously running the animation until '_unconsciousFramesCount' reaches its maximum value
    */
    if (_playerPtr->_aniByWpn[_playerPtr->_wpnID][_playerPtr->_currentAniByWpn]
            .IsFinalBitmap())
    {
        _playerPtr->_aniByWpn[_playerPtr->_wpnID][_playerPtr->_currentAniByWpn].Reset();
    }

    /*	~ OVERRIDE TRIGGERED ANIMATION
    	~ If the player is hit and changes his state to unconscious,
    	~ then his triggered animation (if any) must be forced to stop.
    */
    if (_playerPtr->_isTriggeredAni)
        _playerPtr->FinishTriggeredAnimationAnimationLogic(); // Compel the triggered animation to finish
}

void PlayerUnconsciousState::DoBounceOffGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr)
{
    if (groundPtr->IsOnGroundLeftEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->InitiateOffsetLeft(abs(_playerPtr->_horizontalVelocity));
        _playerPtr->_unconsciousAniDir = false; // left
    }
    else if (groundPtr->IsOnGroundRightEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->InitiateOffsetRight(abs(_playerPtr->_horizontalVelocity));
        _playerPtr->_unconsciousAniDir = true; // right
    }
    else if (groundPtr->IsOnGroundUnderside(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->InitiateOffsetDown(abs(_playerPtr->_verticalVelocity));
    }
    else if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->InitiateOffsetUp(abs(_playerPtr->_verticalVelocity));
    }
}

}