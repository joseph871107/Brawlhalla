/* File description:
This is the source code file 'PlayerConsciousState.cpp'. This is the implementation for the class 'PlayerConsciousState'. */

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
#include "PlayerConsciousState.h"

namespace game_framework
{
PlayerConsciousState::~PlayerConsciousState()
{
    /* Body intentionally empty */
}

PlayerConsciousState::PlayerConsciousState() : PlayerConsciousState(nullptr)
{
    /* Body intentionally empty*/
}

PlayerConsciousState::PlayerConsciousState(const PlayerConsciousState& objectValue) : PlayerConsciousState(objectValue._playerPtr)
{
    /* Player intentionally empty */
}

PlayerConsciousState::PlayerConsciousState(Player* const playerPtrValue) : _playerPtr(playerPtrValue)
{
    /* Body intentionally empty */
}

PlayerConsciousState& PlayerConsciousState::operator=(const PlayerConsciousState& rightObject)
{
    if (this != &rightObject)
    {
        _playerPtr = rightObject._playerPtr;
    }

    return (*this);
}

void PlayerConsciousState::OnMoveAnimationLogic()
{
    /*	~ DETECT TRIGGERED ANIMATION
    	~ If there is no animation being triggered in the meantime,
    	~ then detect should there be any
    */
    if (!_playerPtr->_isTriggeredAni)
        _playerPtr->SetTriggeredAnimationVariables(_playerPtr->_currentKeyID);

    /*	~ ESTIMATE FINISH OF TRIGGERED ANIMATION
    	~ This estimation is independent of the current animation, but
    	~ relies on the '_triggeredAniAnimationId'.
    */
    if (_playerPtr->_isTriggeredAni && _playerPtr->IsFinishedTriggeredAnimation())
        _playerPtr->FinishTriggeredAnimationAnimationLogic();
}

void PlayerConsciousState::OnMoveGameLogic()
{
    /* REPOSITION PLAYER ABOUT GROUNDS */
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
            DoRepositionAboutGround(playerX1, playerY1, playerX2, playerY2, groundPtr);
    }

    /* VERTICAL OFFSET */
    // First, we must implement the codes and functions that does modify the vertical offset
    // variables! Note that these functions not only configure vertical offset variables,
    // they also do a miscellaneous collection of other tasks
    ModifyVerticalOffsetVariablesFunctions();

    // Secondly, we do the vertical offset (in this case, gravity)
    if (!_playerPtr->IsOnGround())
    {
        _playerPtr->_verticalVelocity += _playerPtr->_verticalAcceleration;
        _playerPtr->_y += DoubleToInteger(_playerPtr->_verticalVelocity);
    }

    /*	~ HORIZONTAL OFFSET
    	~ Wall Jump
    */
    if (_playerPtr->IsBeingOffsetHorizontally())
        _playerPtr->DoHorizontalOffset(); // Modify the x-coordinate of the player
}

void PlayerConsciousState::DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr)
{
    int groundX1 = groundPtr->GetCor(0);
    int groundY1 = groundPtr->GetCor(1);
    int groundX2 = groundPtr->GetCor(2);
    int groundY2 = groundPtr->GetCor(3);

    if (groundPtr->IsOnGroundLeftEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->_x = groundX1 - _playerPtr->_width;
    }
    else if (groundPtr->IsOnGroundRightEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->_x = groundX2;
    }
    else if (groundPtr->IsOnGroundUnderside(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->_y = groundY2;
    }
    else if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
    {
        _playerPtr->_y = groundY1 - _playerPtr->_height;
    }
}

void PlayerConsciousState::ModifyVerticalOffsetVariablesFunctions()
{
    /*	~ Remark:
    	~ All the codes and functions below modify the vertical offset variables
    	~ '_verticalAcceleration' and '_verticalVelocity'
    */
    //-----------------INITIALIZE VERTICAL ACCELERATION-----------------//
    _playerPtr->_verticalAcceleration = Player::INITIAL_ACCELERATION;
    //-----------------PROCESS CURRENT KEY COMBINATION GAME LOGIC-----------------//
    /*	~ Remark:
    	~ Perform the game logic of the player based on the current key pressed '_currentKeyID',
    	~ which consists of 2 parts: Triggered Animation and Non-Triggered Animation
    */
    ProcessCurrentKeyCombinationGameLogic();

    //-----------------SLIDE ON EDGES-----------------//
    // 'InitializeOnEdge()' modifies '_verticalVelocity', thus the function must be placed here
    if (IsFirstTimeOnEdge())
        InitiateOnEdge();

    if (_playerPtr->IsOnEdge())
        DoOnEdge();
    else
        DoLeaveEdge();

    //-----------------STAND ON GROUND-----------------//
    if (_playerPtr->IsOnGround())
        DoOnGround();
}

void PlayerConsciousState::ProcessCurrentKeyCombinationGameLogic()
{
    /*	~ Remark:
    	~ To begin with, the current key combination '_currentKeyID' has already defined
    	~ the action to be performed by the player as either "Triggered Animation" or
    	~ "Non-Triggered Animation" in the function 'OnMoveAnimationLogic()'.
    	~
    	~ Thus, in this function, we process the game logic for the player based on either
    	~ the selected "Triggered Animation" or "Non-Triggered Animation".
    	~
    	~ Explanation of the "Triggered Animation" concept:
    	~ When an animation is said to be "triggered", it prevails all other animations by
    	~ means of depiction; that is, when an animation is triggered, it becomes the only
    	~ animation being shown.
    */
    if (_playerPtr->_isTriggeredAni) // If an animation is triggered and has not finished (decided by 'OnMoveAnimationLogic()'), then
    {
        // Initiate the triggered action (if haven't)
        if (!_playerPtr->_isInitiatedTriggeredAni)
        {
            InitiateTriggeredAction();
            _playerPtr->_isInitiatedTriggeredAni = true; // Mark that the triggered animation has been triggered
        }

        // Process the triggered animation
        DoTriggeredAction();
        // Reset all movement inertia
        _playerPtr->ResetMovementVelocity();
    }
    else // If there is no "Triggered Animation" in the meantime, then do the other "Non-Triggered Animation(s)"
        DoNonTriggeredAction();
}


bool PlayerConsciousState::IsFirstTimeOnEdge()
{
    return (_playerPtr->IsOnEdge() && _playerPtr->_isFirstTimeOnEdge);
}

void PlayerConsciousState::InitiateOnEdge()
{
    _playerPtr->_isFirstTimeOnEdge = false;
    _playerPtr->_verticalVelocity = 0;
}

void PlayerConsciousState::DoOnEdge()
{
    _playerPtr->_verticalAcceleration = Player::EDGE_SLIDING_ACCELERATION;
}



void PlayerConsciousState::DoLeaveEdge()
{
    _playerPtr->_isFirstTimeOnEdge = true;
}

void PlayerConsciousState::DoOnGround()
{
    _playerPtr->_verticalVelocity = 0;
}

void PlayerConsciousState::InitiateTriggeredAction()
{
    for (auto elementPtr : _playerPtr->_triggeredAnis)
        if (elementPtr->GetKeyID() == _playerPtr->_triggeredAniKeyID)
            elementPtr->InitiateTriggeredAction();
}

void PlayerConsciousState::DoTriggeredAction()
{
    for (auto elementPtr : _playerPtr->_triggeredAnis)
        if (elementPtr->GetKeyID() == _playerPtr->_triggeredAniKeyID)
            elementPtr->DoTriggeredAction();
}

void PlayerConsciousState::DoNonTriggeredAction()
{
    /*	~ Remarks:
    ~ We care only about key combinations that does NOT determine the attack button is pressed and the player is holding a weapon
    ~ That is, only the key combinations ending with '1' are taken into account
    ~ Jump is a special case and thus is added into the bottom of the function
    */
    switch (_playerPtr->_currentKeyID)
    {
        /* ON GROUND */
        case Player::KEY_GND_IDLE: // on ground, not move, not attack
            // Do nothing
            break;

        case Player::KEY_GND_MOVE_RIGHT: // on ground, move right, not attack
            DoMoveRightWithAcceleration();
            break;

        case Player::KEY_GND_MOVE_LEFT: // on ground, move left, not attack
            DoMoveLeftWithAcceleration();
            break;

        case Player::KEY_GND_LAND_DOWN: // on ground, land down, not attack
            // Do nothing
            break;

        /* ON AIR */
        case Player::KEY_AIR_IDLE: // on air, not move, not attack
            // Do nothing
            break;

        case Player::KEY_AIR_MOVE_RIGHT: // on air, move right, not attack
            DoMoveRight(Player::MOVEMENT_UNIT);
            break;

        case Player::KEY_AIR_MOVE_LEFT: // on air, move left, not attack
            DoMoveLeft(Player::MOVEMENT_UNIT);
            break;

        case Player::KEY_AIR_LAND_DOWN: // on air, land down, not attack
            _playerPtr->DoLand();
            break;

        default:
            break;
    }

    /* JUMP */
    if (_playerPtr->_isTriggerJump) // Game logic 'OnMove()' catches the signal jump
    {
        DoJump();

        if (_playerPtr->IsOnEdge())
            InitiateWallJump();

        _playerPtr->_isTriggerJump = false; // Turn off the jump trigger
    }

    /*	~ MOVEMENT INERTIA
    	~ The movement inertia of the player happens only when the player
    	~ stops after running, or turns running direction
    */
    if (_playerPtr->_currentKeyID == Player::KEY_GND_IDLE)
    {
        if (_playerPtr->_moveVelocity > 2)
        {
            _playerPtr->_moveVelocity -= Player::STOP_ACCELERATION;
            _playerPtr->_x += DoubleToInteger(_playerPtr->_moveVelocity);
        }
        else if (_playerPtr->_moveVelocity < -2)
        {
            _playerPtr->_moveVelocity += Player::STOP_ACCELERATION;
            _playerPtr->_x += DoubleToInteger(_playerPtr->_moveVelocity);
        }
        else // -2 <= _moveVelocity <= 2
        {
            _playerPtr->ResetMovementVelocity(); // Reset all movement inertia
        }
    }
    else if (!
             ((_playerPtr->_currentKeyID == Player::KEY_GND_MOVE_LEFT)
              ||
              (_playerPtr->_currentKeyID == Player::KEY_GND_MOVE_RIGHT)))
    {
        _playerPtr->ResetMovementVelocity(); // Reset all movement inertia
    }
}

void PlayerConsciousState::DoMoveRightWithAcceleration()
{
    if (_playerPtr->_moveVelocity < Player::MAX_MOVE_VELOCITY)
        _playerPtr->_moveVelocity += Player::MOVE_ACCELERATION;

    _playerPtr->_x += DoubleToInteger(_playerPtr->_moveVelocity);
}

void PlayerConsciousState::DoMoveLeftWithAcceleration()
{
    if (_playerPtr->_moveVelocity > - Player::MAX_MOVE_VELOCITY)
        _playerPtr->_moveVelocity -= Player::MOVE_ACCELERATION;

    _playerPtr->_x += DoubleToInteger(_playerPtr->_moveVelocity);
}

void PlayerConsciousState::DoMoveLeft(int movementUnit)
{
    _playerPtr->_x -= movementUnit;
}
void PlayerConsciousState::DoMoveRight(int movementUnit)
{
    _playerPtr->_x += movementUnit;
}



void PlayerConsciousState::DoJump()
{
    if (_playerPtr->_jumpCount > 0)   //If the player is able to jump more
    {
        _playerPtr->InitiateOffsetUp(Player::INITIAL_VELOCITY);
        _playerPtr->_jumpCount--; //Decrement the jumps available
    }
}

void PlayerConsciousState::InitiateWallJump()
{
    if (_playerPtr->_jumpCount > 0) //If the player is able to jump more
    {
        if (_playerPtr->IsOnLeftEdge())
        {
            _playerPtr->InitiateOffsetLeft(Player::OFFSET_INITIAL_VELOCITY);
        }
        else if (_playerPtr->IsOnRightEdge())
        {
            _playerPtr->InitiateOffsetRight(Player::OFFSET_INITIAL_VELOCITY);
        }
    }
}

void PlayerConsciousState::SetCurrentNonTriggeredAnimationByWeapon()
{
    switch (_playerPtr->_currentKeyID)
    {
        /* ON GROUND */
        case Player::KEY_GND_IDLE:
            if (_playerPtr->_dir) // If the player is facing right
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_STAND_RIGHT);
            else
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_STAND_LEFT);

            break;

        case Player::KEY_GND_MOVE_LEFT:
            _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_MOVE_LEFT);
            break;

        case Player::KEY_GND_MOVE_RIGHT:
            _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_MOVE_RIGHT);
            break;

        case Player::KEY_GND_LAND_DOWN:
            // Do nothing
            break;

        /* ON AIR */
        case Player::KEY_AIR_IDLE:
            if (_playerPtr->IsOnLeftEdge()) // Player is leaning on left edge
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_LEAN_RIGHT); // Set the leaning animation of player facing right
            else if (_playerPtr->IsOnRightEdge()) // Player is leaning on left edge
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_LEAN_LEFT); // Set the leaning animation of player facing left
            else // Player is jumping
                if (_playerPtr->_dir)
                    _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_JUMP_RIGHT);
                else
                    _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_JUMP_LEFT);

            break;

        case Player::KEY_AIR_MOVE_RIGHT:
            _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_JUMP_RIGHT);
            break;

        case Player::KEY_AIR_MOVE_LEFT:
            _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_JUMP_LEFT);
            break;

        case Player::KEY_AIR_LAND_DOWN:
            if (_playerPtr->_dir)
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_LAND_FALL_RIGHT);
            else
                _playerPtr->SetAnimationStateByWeapon(Player::ANI_WPN_ID_LAND_FALL_LEFT);

            break;

        default:
            break;
    }
}
}