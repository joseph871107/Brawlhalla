/* File description:
This is the source code file 'GroundMoveRightAttackTriggeredAnimation.cpp'. This is the implementation for the class 'GroundMoveRightAttackTriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GroundMoveRightAttackTriggeredAnimation.h"
#include "player.h"

namespace game_framework
{
GroundMoveRightAttackTriggeredAnimation::~GroundMoveRightAttackTriggeredAnimation()
{
    /* Body intentionally empty */
}

GroundMoveRightAttackTriggeredAnimation::GroundMoveRightAttackTriggeredAnimation() : GroundMoveRightAttackTriggeredAnimation(nullptr, int())
{
    /* Body intentionally empty */
}

GroundMoveRightAttackTriggeredAnimation::GroundMoveRightAttackTriggeredAnimation(const GroundMoveRightAttackTriggeredAnimation& objectValue) : TriggeredAnimation(objectValue)
{
    /* Body intentionally empty */
}

GroundMoveRightAttackTriggeredAnimation::GroundMoveRightAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue) : TriggeredAnimation(playerPtrValue, keyIDValue)
{
    /* Body intentionally empty */
}

GroundMoveRightAttackTriggeredAnimation& GroundMoveRightAttackTriggeredAnimation::operator=(const GroundMoveRightAttackTriggeredAnimation& rightObject)
{
    if (this != &rightObject)
    {
        TriggeredAnimation::operator=(rightObject);
    }

    return (*this);
}

void GroundMoveRightAttackTriggeredAnimation::FinishTriggeredAnimationGameLogic()
{
    _playerPtr->EmptyHitTargetPlayers();
}

void GroundMoveRightAttackTriggeredAnimation::SetTriggeredAnimationVariables()
{
    TriggeredAnimation::SetTriggeredAnimationVariables();

    if (_playerPtr->GetTriggeredAnimationDirection())
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT);
    else
        _playerPtr->SetTriggeredAnimationAnimationID(Player::ANI_WPN_ID_GND_MOVE_ATTACK_LEFT);
}

void GroundMoveRightAttackTriggeredAnimation::InitiateTriggeredAction()
{
    _playerPtr->SetIsTriggerAttack(false);
}

void GroundMoveRightAttackTriggeredAnimation::DoTriggeredAction()
{
    _playerPtr->DoAttack();

    if (_playerPtr->GetCurrentAniByWeaponNum() == 2)
        _playerPtr->InitiateOffsetRight(Player::OFFSET_INITIAL_VELOCITY);
}

}