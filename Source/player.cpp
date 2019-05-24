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
// Others
#include "BattleSystem.h"
#include "ExplosionEffect.h"

namespace game_framework
{
//-----------------CONSTANTS DEFINITIONS-----------------//
const int MAX_JUMP_COUNT = 2;
const int MOVEMENT_UNIT = 10;
const int GND_ATTACK_MOVEMENT_UNIT = 12;
const double INITIAL_VELOCITY = 18;
const double INITIAL_ACCELERATION = 1.2;
const double LANDING_ACCELERATION = 5;
const double EDGE_SLIDING_ACCELERATION = 0.1;
const double COLLISION_ERRORS = 1.0;
const int MAX_LIFE = 3;
const double MOVE_ACCELERATION = 0.5;
const double STOP_ACCELERATION = 1;
const double MAX_MOVE_VELOCITY = 10;
const int INITIAL_TAKEN_DAMAGE = 10;
const int INCREMENT_AMOUNT_OF_TAKEN_DAMAGE = 2;
const int MAX_ATTACK_AFFECTION_FRAMES = 150; // 5 secs
const int RESPAWN_DISTANCE_ABOVE_GROUND = 100;
const int RESPAWN_MOVEMENT_OFFSET_MAGNITUDE = 10;
const int RESPAWN_LEFT_START_POS_X = 0;
const int RESPAWN_LEFT_START_POS_Y = 0;
const int RESPAWN_RIGHT_START_POS_X = SIZE_X;
const int RESPAWN_RIGHT_START_POS_Y = 0;
// States
const int CONSCIOUS_STATE = 0;
const int UNCONSCIOUS_STATE = 1;
const int RESPAWN_STATE = 2;
// Triggered Animation Key ID
const int KEY_GND_ATTACK = 112;
const int KEY_GND_MOVE_RIGHT_ATTACK = 122;
const int KEY_GND_MOVE_LEFT_ATTACK = 132;
const int KEY_GND_LAND_DOWN_ATTACK = 142;
const int KEY_DRAW_SWORD = 113;
const int KEY_DODGE = 114;
const int KEY_AIR_ATTACK = 212;
const int KEY_AIR_MOVE_RIGHT_ATTACK = 222;
const int KEY_AIR_MOVE_LEFT_ATTACK = 232;
const int KEY_AIR_LAND_DOWN_ATTACK = 242;
// Non-triggered Animation Key ID
const int KEY_GND_IDLE = 111;
const int KEY_GND_MOVE_RIGHT = 121;
const int KEY_GND_MOVE_LEFT = 131;
const int KEY_GND_LAND_DOWN = 141;
const int KEY_AIR_IDLE = 211;
const int KEY_AIR_MOVE_RIGHT = 221;
const int KEY_AIR_MOVE_LEFT = 231;
const int KEY_AIR_LAND_DOWN = 241;

//-----------------FUNCTIONS DEFINITIONS-----------------//
Player::Player() :
    _x(int()), _y(int()), ani(vector<CAnimation>()), currentAni(int()),
    bmp_iter(vector<vector<int>*>()), _width(int()),
    _height(int()), _isPressingLeft(bool()),
    _isPressingRight(bool()), _dir(bool()), _isTriggerJump(bool()), _jumpCount(bool()),
    _horizontalVelocity(int()), _isOffsetLeft(bool()), _isOffsetRight(bool()),
    _verticalVelocity(double()), _grounds(vector<Ground*>()), _collision_box(CMovingBitmap()), _life(int()),
    _name(string()), _isPlayer(true) // 我覺得之後應該先不用更改這個constructor，好多喔。。。
{
    /* Body intentionally empty */
}

Player::~Player()
{
    delete _flyingWeapon;

    for (auto elementPtr : _triggeredAnis)
    {
        delete elementPtr;
    }
}

void Player::InitializeTriggeredAnimations()
{
    /* ATTACK ON GROUND */
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundAttackTriggeredAnimation(this, KEY_GND_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundMoveRightAttackTriggeredAnimation(this, KEY_GND_MOVE_RIGHT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundMoveLeftAttackTriggeredAnimation(this, KEY_GND_MOVE_LEFT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundLandDownAttackTriggeredAnimation(this, KEY_GND_LAND_DOWN_ATTACK));
    /* ATTACK ON AIR */
    _triggeredAnis.push_back((TriggeredAnimation*) new AirAttackTriggeredAnimation(this, KEY_AIR_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirMoveRightAttackTriggeredAnimation(this, KEY_AIR_MOVE_RIGHT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirMoveLeftAttackTriggeredAnimation(this, KEY_AIR_MOVE_LEFT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirLandDownAttackTriggeredAnimation(this, KEY_AIR_LAND_DOWN_ATTACK));
    /* OTHERS */
    _triggeredAnis.push_back((TriggeredAnimation*) new DrawSwordTriggeredAnimation(this, KEY_DRAW_SWORD));
    _triggeredAnis.push_back((TriggeredAnimation*) new DodgeTriggeredAnimation(this, KEY_DODGE));
}

void Player::SetAttacker(Player* const& newAttacker, const int& newAttackerAffectionFrameCount)
{
    _attacker = newAttacker;
    _attackerAffectionFrameCount = newAttackerAffectionFrameCount;
}

void Player::Initialize(BattleSystem* battleSystemValue, vector<Ground*> groundsValue, vector<Player*>* playersPtrValue, string nameValue, vector<long> keysValue, ExplosionEffect* const explosionEffectPtrValue)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
    Ground* g = GetRandomGround(&groundsValue);		// Randomly select Ground
    _x = random(g->GetCor(0), g->GetCor(2) - GetWidth());		// Randomly set x coordinate within Ground's width
    _y = g->GetCor(1) - GetHeight();
    //
    _width = (int)(_collision_box.Width() * BITMAP_SIZE);
    _height = (int)(_collision_box.Height() * BITMAP_SIZE);

    //
    if (!keysValue.size())
        _keys = { KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_F, KEY_X };
    else
        _keys = keysValue;

    //
    _playersPtr = playersPtrValue;
    //
    _grounds = groundsValue;
    //
    _life = MAX_LIFE;
    //
    _name = nameValue;
    //
    _roundPrevPickedUpWpnID = 2;
    //
    InitializeTriggeredAnimations();
    //
    _hitTargetPlayers = vector<Player*>();
    //
    _battleSystem = battleSystemValue;
    //
    _explosionEffectPtr = explosionEffectPtrValue;
    //
    _respawnCourier.Initialize(camera);
    //
    InitializeOnRespawn();
}

void Player::LoadBitmap()
{
    _collision_box.LoadBitmap(IDB_P_COLLISION_BOX, RGB(0, 0, 0));
    _respawnCourier.LoadBitmap();
    SetAnimation();
}

bool Player::IsOnEdge()
{
    return (IsOnLeftEdge() || IsOnRightEdge());
}

bool Player::IsFirstTimeOnEdge()
{
    return (IsOnEdge() && _isFirstTimeOnEdge);
}

void Player::InitiateOnEdge()
{
    _isFirstTimeOnEdge = false;
    _verticalVelocity = 0;
}

void Player::DoOnEdge()
{
    _verticalAcceleration = EDGE_SLIDING_ACCELERATION;
}

void Player::DoLeaveEdge()
{
    _isFirstTimeOnEdge = true;
}

void Player::DoOnGround()
{
    _verticalVelocity = 0;
}

void Player::ModifyVerticalOffsetVariablesFunctions()
{
    /*	~ Remark:
    	~ All the codes and functions below modify the vertical offset variables
    	~ '_verticalAcceleration' and '_verticalVelocity'
    */
    //-----------------INITIALIZE VERTICAL ACCELERATION-----------------//
    _verticalAcceleration = INITIAL_ACCELERATION;
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

    if (IsOnEdge())
        DoOnEdge();
    else
        DoLeaveEdge();

    //-----------------STAND ON GROUND-----------------//
    if (IsOnGround())
        DoOnGround();
}

void Player::ConsciouslyOnMoveGameLogic()
{
    /* REPOSITION PLAYER ABOUT GROUNDS */
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
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
    if (!IsOnGround())
    {
        _verticalVelocity += _verticalAcceleration;
        _y += Round(_verticalVelocity);
    }

    /*	~ HORIZONTAL OFFSET
    	~ Wall Jump
    */
    if (IsBeingOffsetHorizontally())
        DoHorizontalOffset(); // Modify the x-coordinate of the player
}


void Player::UnconsciouslyOnMoveGameLogic()
{
    //-----------------POSITION TRANSFORMATION SECTION-----------------//
    /* PLAYER BOUNCES OFF THE GROUNDS */
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
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
    _verticalVelocity += INITIAL_ACCELERATION;
    _y += Round(_verticalVelocity);

    /*	~ HORIZONTAL OFFSET
    	~ Horizontal offset by being hit
    */
    if (IsBeingOffsetHorizontally())
        DoHorizontalOffset(); // Modify the x-coordinate of the player

    //-----------------CONSCIOUS STATE RESOLVE SECTION-----------------//
    /*	~ SET MAX UNCONSCIOUS FRAMES
    	~ Determine the maximum duration for the unconscious state of the player
    	~ when he gets hit, based on '_takenDmg' - the total damages he has taken.
    */
    int maxUnconsciousFrames = 10;

    if (_takenDmg > maxUnconsciousFrames)
        maxUnconsciousFrames = _takenDmg;

    /* ESTIMATE CURRENT UNCONSCIOUS FRAMES COUNT */
    _unconsciousFramesCount++; // Increment the current unconscious frames count

    if (_unconsciousFramesCount == maxUnconsciousFrames)
        SetConscious();
}

void Player::SetTriggeredAnimationSelector()
{
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == _triggeredAniKeyID)
            elementPtr->SetTriggeredAnimationSelector();
}

void Player::SetNonTriggeredAnimationSelector()
{
    SetAnimationSelector(true);
    /// DEBUG: We do not need 'ani' anymore, now all set to '_aniByWpn'
}

void Player::DeleteFlyingWeapon()
{
    if (_flyingWeapon != nullptr)
    {
        if (_flyingWeapon->HasTaken() || _flyingWeapon->IsOutMapBorder())
        {
            delete _flyingWeapon;
            _flyingWeapon = nullptr;
        }
    }
}

void Player::SetCurrentTriggeredAnimationByWeapon()
{
    /*	~ Remark:
    	~ The player is performing a trigger animation
    	~ The animation is dependent on the weapon (decided by the actual sprite of the player)
    */
    SetAnimationStateByWeapon(_triggeredAniAnimationID);
}

void Player::SetCurrentTriggeredAnimation()
{
    /*	~ Remark:
    	~ The player is performing a trigger animation
    	~ The animation is NOT dependent on the weapon (decided by the actual sprite of the player)
    */
    SetAnimationState(_triggeredAniAnimationID);
}

void Player::SetCurrentAniByWeapon()
{
    if (_isTriggeredAni)
        SetCurrentTriggeredAnimationByWeapon();
    else
        SetCurrentNonTriggeredAnimationByWeapon();
}

void Player::SetCurrentAni()
{
    if (_isTriggeredAni)
        SetCurrentTriggeredAnimation();
    else
        SetCurrentNonTriggeredAnimation();
}

void Player::SetCurrentAnimation()
{
    if (_aniSelector)
        SetCurrentAniByWeapon();
    else
        SetCurrentAni();
}

void Player::SetAnimationSelector(bool newAniSelector)
{
    _aniSelector = newAniSelector;
}

void Player::UnconsciouslyOnMoveAnimationLogic()
{
    /*	~ RESET UNCONSCIOUSLY FLYING ANIMATION
    	~ Continuously running the animation until '_unconsciousFramesCount' reaches its maximum value
    */
    /// Comment for future devs: This should be written as "if ani[currentAni].IsFinalBitmap() { ... }" for better clarification
    /// However, by normal logic, since we haven't set the animation selector, we should not refer 'ani[currentAni]'
    if (_aniByWpn[_wpnID][ANI_WPN_ID_UNCONSCIOUS_FLYING_LEFT].IsFinalBitmap() || _aniByWpn[_wpnID][ANI_WPN_ID_UNCONSCIOUS_FLYING_RIGHT].IsFinalBitmap())
    {
        _aniByWpn[_wpnID][ANI_WPN_ID_UNCONSCIOUS_FLYING_LEFT].Reset();
        _aniByWpn[_wpnID][ANI_WPN_ID_UNCONSCIOUS_FLYING_RIGHT].Reset();
    }

    /*	~ OVERRIDE TRIGGERED ANIMATION
    	~ If the player is hit and changes his state to unconscious,
    	~ then his triggered animation (if any) must be forced to stop.
    */
    if (_isTriggeredAni)
        FinishTriggeredAnimationAnimationLogic(); // Compel the triggered animation to finish

    /*	~ SET ANIMATION SELECTOR
    	~ Set the animation selector to 'false'
    */
    /// Comment for future devs: Unconscious animation should be defined as a new animation vector;
    /// that is, it should not be mixed with other conscious animation in 'ani'
    SetAnimationSelector(true);
}

void Player::FinishTriggeredAnimationAnimationLogic()
{
    // Reset the animation of the finished triggered animation
    if (_aniSelector)
        _aniByWpn[_wpnID][_triggeredAniAnimationID].Reset();
    else
        ani[_triggeredAniAnimationID].Reset();

    // Save the finished Triggered Animation's key ID '_triggeredAniKeyID'
    _finishedTriggeredAniKeyID = _triggeredAniKeyID;
    // Mark that the trigger animation has finished by resetting all animation variables
    ResetTriggeredAnimationVariables();
}

void Player::FinishTriggeredAnimationGameLogic()
{
    // Finish other logic regarding the triggered animation
    // Since '_triggeredAniKeyID' has already been reset in 'OnMoveAnimationLogic()'
    // once the triggered animatin is finished, we use '_finishedTriggeredAniKeyID' for our estimation
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == _finishedTriggeredAniKeyID)
            elementPtr->FinishTriggeredAnimationGameLogic();
}

void Player::ConsciouslyOnMoveAnimationLogic()
{
    /*	~ DETECT TRIGGERED ANIMATION
    	~ If there is no animation being triggered in the meantime,
    	~ then detect should there be any
    */
    if (!_isTriggeredAni)
        SetTriggeredAnimationVariables(_currentKeyID);

    /*	~ SET ANIMATION SELECTOR
    	~ The '_aniSelector' decides the currently displayed animation
    */
    if (_isTriggeredAni)
        SetTriggeredAnimationSelector();
    else
        SetNonTriggeredAnimationSelector();

    /*	~ ESTIMATE FINISH OF TRIGGERED ANIMATION
    	~ This estimation is independent of the current animation, but relies
    	~ on the '_aniSelector' and '_triggeredAniAnimationId'.
    */
    if (_isTriggeredAni && IsFinishedTriggeredAnimation())
        FinishTriggeredAnimationAnimationLogic();
}

void Player::MoveCurrentAnimation()
{
    if (_aniSelector)
        if (_currentAniByWpn == ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT || _currentAniByWpn == ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT)
        {
            if (_aniByWpn[_wpnID][_currentAniByWpn].GetCurrentBitmapNumber() < 2)
                _aniByWpn[_wpnID][_currentAniByWpn].OnMove();
            else
            {
                if (IsOnGround())
                    _aniByWpn[_wpnID][_currentAniByWpn].OnMove();
            }
        }
        else
            _aniByWpn[_wpnID][_currentAniByWpn].OnMove();
    else
        ani[currentAni].OnMove();
}

void Player::RespawnOnMoveAnimationLogic()
{
    /*	~ OVERRIDE TRIGGERED ANIMATION
    ~ If the player is dead and changes his state to unconscious,
    ~ then his triggered animation (if any) must be forced to stop.
    */
    if (_isTriggeredAni)
        FinishTriggeredAnimationAnimationLogic(); // Compel the triggered animation to finish

    /*	~ SET ANIMATION SELECTOR
    	~ Set the animation selector to 'false'
    */
    /// Comment for future devs: Respawn animation should be defined as a new animation vector;
    /// that is, it should not be mixed with other conscious animation in 'ani'
    SetAnimationSelector(true);
}

void Player::OnMoveAnimationLogic()
{
    /*	~ Remark:
    	~ This function is responsible for the animation logic of the player.
    	~ Its primary task is to set and "move" the player's current animation.
    */

    //-----------------STATE SECTION-----------------//
    switch (_state)
    {
        case CONSCIOUS_STATE:
            ConsciouslyOnMoveAnimationLogic();
            break;

        case UNCONSCIOUS_STATE:
            UnconsciouslyOnMoveAnimationLogic();
            break;

        case RESPAWN_STATE:
            RespawnOnMoveAnimationLogic();
            break;

        default:
            // Never happen
            return;
    }

    //-----------------COMMON SECTION-----------------//

    /*	~ RESET JUMP ANIMATION
    	~ Reset the jump animation for the player
    */
    /// Comment for future devs: Reset jump animation is not well placed here and should be re-accomodate in the near future
    if (IsOnGround() || IsOnEdge() || (_isTriggerJump && _jumpCount > 0))
        ResetAnimations(ANI_WPN_ID_JUMP_LEFT); // Reset the jump animation so that it keeps displaying while the player is jumping regardless of reaching its final bitmap

    /*	~ SET CURRENT ANIMATION
    	~ Set the current animation based on '_aniSelector'
    	~ and '_triggeredAniAnimationID'
    */
    SetCurrentAnimation();
    /*	~ MOVE CURRENT ANIMATION
    	~ Proceed to the next CMovingBitmap of the current animation,
    	~ which is determined by '_aniSelector'
    */
    MoveCurrentAnimation();
}

void Player::RespawnOnMoveGameLogic()
{
    Vector2 vectorToRespawnDestination;
    vectorToRespawnDestination.SetXY(_x, _y, _resDestPosX, _resDestPosY);
    double distance = vectorToRespawnDestination.GetLength();

    if (distance > _preDistance) // If the player is nearest from the destination, then end respawning
        InitializeOnRespawn();
    else
    {
        _preDistance = distance;
        // Move the player
        _x += _vectorRespawnMovement.GetX();
        _y += _vectorRespawnMovement.GetY();
        // Move the respawn courier of the player
        _respawnCourier.SetXY(Round(_x + GetWidth() / 2.0 - _respawnCourier.GetWidth() / 2.0),
                              _y - Round(_respawnCourier.GetWidth()));
    }
}

void Player::DoReturnHomeRespawnCourier()
{
    int currentX = _respawnCourier.GetX();
    int currentY = _respawnCourier.GetY();
    // Move top left
    _respawnCourier.SetDir(false);
    _respawnCourier.SetXY(currentX - 10, currentY - 10);
}

void Player::OnMoveGameLogic()
{
    /*	~ Remark:
    	~ This function is responsible for the game logic of the player,
    	~ including his positioning and the way he interacts with other
    	~ objects (such as grounds, other players, etc.)
    */

    //-----------------PRIOR COMMON SECTION-----------------//
    /* DEAD & RESPAWN */
    if (!_isDead && IsOutMapBorder())
        DoDead();

    if (_isDead)
    {
        if (_explosionEffectPtr->GetIsTrigger()) // If the dead explosion effect has not been finished, then halt all the movement of the player
            return;
        else // If the dead explosion effect is finished, then mark that the player is dead
        {
            _isDead = false;

            if (!IsOutOfLife())// If the player is respawnable, then respawn him
                DoRespawn();
        }
    }

    //-----------------STATE SECTION-----------------//
    switch (_state)
    {
        case CONSCIOUS_STATE:
            ConsciouslyOnMoveGameLogic();
            break;

        case UNCONSCIOUS_STATE:
            UnconsciouslyOnMoveGameLogic();
            break;

        case RESPAWN_STATE:
            RespawnOnMoveGameLogic();
            break;

        default:
            // Never happen
            return;
    }

    /* MOVE RESPAWN COURIER OUT OF THE MAP */
    /// Comment for future devs: It's a little bit dirty to put the code here
    if (_state != RESPAWN_STATE)
        DoReturnHomeRespawnCourier();

    //-----------------POSTERIOR COMMON SECTION-----------------//

    /*	~ ESTIMATE FINISH OF TRIGGERED ANIMATION
    	~ Do note that both 'UnconsciouslyOnMoveAnimationLogic()' and 'ConsciouslyOnMoveAnimationLogic()'
    	~ can finish the Triggered Animation. Thus, this estimation is in the common section of
    	~ 'OnMoveGameLogic()'
    */
    if (!_isTriggeredAni) // If the "Triggered Animation" is finished (determined by 'FinishTriggeredAnimationAnimationLogic()')
        FinishTriggeredAnimationGameLogic();

    /*	~ RESET JUMP COUNT
    	~ Reset the jump count of the player, so that he can jump when eligible
    */
    if (IsOnGround() || IsOnLeftEdge() || IsOnRightEdge())
        ResetJumpCount();

    /* EVALUATE KILLER AFFECTION */
    if (_attackerAffectionFrameCount >= MAX_ATTACK_AFFECTION_FRAMES)
        SetAttacker(nullptr, 0); // Discard the affection of the attacker
    else
        _attackerAffectionFrameCount++;

    /* UNTITLED 歐陽 */
    if (_flyingWeapon != nullptr)
        _flyingWeapon->OnMove();

    DeleteFlyingWeapon();
}

void Player::DoParseKeyPressed()
{
    /*	~ Remark: When both keys are held, the right key prevails the left key
    	~ Thus, when both keys are held and the right key is released, then
    	~ the left key will come to play
    */
    if (_isTriggerPressingLeft && !_isPressingRight)
    {
        _dir = false;
        _isPressingLeft = true;
        _isTriggerPressingLeft = false;
    }
}

void Player::OnMove()
{
    DoParseKeyPressed();
    _currentKeyID = GetKeyCombination(); // Get the current key combination to re-use in 'OnMoveAnimationLogic()' and 'OnMoveGameLogic()'
    OnMoveAnimationLogic();
    OnMoveGameLogic();
}

void Player::OnShow()
{
    // Show throwing weapons
    if (_flyingWeapon != nullptr)
        _flyingWeapon->OnShow();

    // Show respawn courier
    _respawnCourier.OnShow();
    // Show current animation
    ShowCurrentAnimation();
    // Play current audio
    PlayAudioByState();
    // Set the camera for showing name tag
    CPoint cam = camera->GetXY(Round(_x - 4 * BITMAP_SIZE), Round(_y + _collision_box.Height() * BITMAP_SIZE));
    // Show the name tag
    CString playerName;
    playerName.SetString(_name);
    playerName.SetSize(camera->GetSize() / 2);
    playerName.SetTopLeft(cam.x - 60, cam.y);
    playerName.ShowBitmap();
}

void Player::OnKeyDown(const UINT& nChar)
{
    if (nChar == _keys[0]) // Up
    {
        _isTriggerJump = true;
    }
    else if (nChar == _keys[1]) // Right
    {
        _dir = true;
        _isPressingRight = true;
    }
    else if (nChar == _keys[2]) // Down
    {
        /// Comment for future devs: Later use to slip down from certain terrain
        _isPressingDown = true;
    }
    else if (nChar == _keys[3]) // Left
    {
        _isTriggerPressingLeft = true;
    }
    else if (nChar == _keys[4]) //Attack
    {
        _isTriggerAttack = true;

        if (_flyingWeapon != nullptr)
            _flyingWeapon->OnKeyDown(nChar);

        DeleteFlyingWeapon();
    }
    else if (nChar == _keys[5]) //Dodge
    {
        _isTriggerDodge = true;
    }
    else if (nChar == _keys[6])	//Throw
    {
        if (GetHoldWeapon())
        {
            Weapon* weapon = new Weapon();
            weapon->AddCamera(camera);
            weapon->Initialize(_grounds, *_playersPtr);
            bool dir = GetDirection();

            if (!dir)
                weapon->SetXY(GetCor(0) - 100, GetCor(1) + 10);
            else
                weapon->SetXY(GetCor(2) + 20, GetCor(1) + 10);

            weapon->Throw(GetDirection(), this);
            _flyingWeapon = weapon;
            SetHoldWeapon(false);
            ResetWeaponID();
        }
    }
    else
    {
        // Do nothing
    }
}

void Player::OnKeyUp(const UINT& nChar)
{
    if (nChar == _keys[1]) // Right
    {
        _isPressingRight = false;
    }
    else if (nChar == _keys[2]) // Down
    {
        _isPressingDown = false;
    }
    else if (nChar == _keys[3]) // Left
    {
        _isPressingLeft = false;
    }
    else
    {
        // Do nothing
    }
}

void Player::SetName(string name)
{
    _name = name;
}

void Player::SetHoldWeapon(bool isHolding)
{
    _isHoldingWeapon = isHolding;
    _isTriggerDrawWeapon = isHolding;
    _isTriggerAttack = false; // We are picking weapon, not performing an attack
}

void Player::InitializeUnconsciousState(bool beingAttackedDirection)
{
    SetState(UNCONSCIOUS_STATE);
    _unconsciousFramesCount = 0;
    _unconsciousAniDir = beingAttackedDirection;
}

void Player::BeenAttacked(Vector2 displacementVector, bool beingAttackedDirection)
{
    int displaceX = displacementVector.GetX();
    int displaceY = displacementVector.GetY();

    if (displaceY < 0)
    {
        InitiateOffsetUp(abs(displaceY));
    }
    else if (displaceY > 0)
    {
        InitiateOffsetDown(abs(displaceY));
    }
    else // displayY == 0
    {
        InitiateOffsetUp(10);
    }

    if (displaceX < 0)
    {
        InitiateOffsetLeft(abs(displaceX));
    }
    else if (displaceX > 0)
    {
        InitiateOffsetRight(abs(displaceX));
    }
    else // displayX == 0
    {
        /* Do nothing */
    }

    InitializeUnconsciousState(beingAttackedDirection);
}

bool Player::GetHoldWeapon()
{
    return _isHoldingWeapon;
}

bool Player::GetDirection()
{
    return _dir;
}

int Player::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return _x;

        case 1:
            return _y;

        case 2:
            return _x + (int)(_collision_box.Width() * BITMAP_SIZE);

        case 3:
            return _y + (int)(_collision_box.Height() * BITMAP_SIZE);

        default:
            return NULL;
    }
}

int Player::GetWidth()
{
    return GetCor(2) - GetCor(0);
}

int Player::GetHeight()
{
    return GetCor(3) - GetCor(1);
}

const string& Player::GetName() const
{
    return (_name);
}

const int& Player::GetLife() const
{
    return (_life);
}

const bool Player::IsOutOfLife() const
{
    return (_life == 0);
}

const long& Player::GetAttackKey() const
{
    return (_keys[4]);
}

void Player::GenerateAndSetWeaponID() // This function is intended to be called by 'Weapon::HitPlayer()'
{
    // The player will pick his weapon alternatively through out the round
    if (_roundPrevPickedUpWpnID == 1)
    {
        _roundPrevPickedUpWpnID = 2;
        _wpnID = 2;
    }
    else if (_roundPrevPickedUpWpnID == 2)
    {
        _roundPrevPickedUpWpnID = 1;
        _wpnID = 1;
    }
}

void Player::ResetWeaponID()
{
    _wpnID = 0; // reset to default weapon: punch
}

void Player::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    ani.push_back(temp);
    bmp_iter.push_back(list);
}

void Player::AddCAnimationWithSprite(vector<CAnimation>* tempAniByWpn, vector< vector<CMovingBitmap>>* sprite, vector<CPoint>* list, double size, int delay, bool repeat, int times)
{
    CAnimation temp(repeat, times);

    for (auto i : *list)
        temp.AddBitmap((*sprite)[i.x][i.y]);

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    tempAniByWpn->push_back(temp);
}

void Player::ResetAnimations(int leftAnimationID)
{
    _aniByWpn[_wpnID][leftAnimationID].Reset(); // Reset left animation
    _aniByWpn[_wpnID][leftAnimationID + 1].Reset(); // Reset right animation
}
void Player::SetAnimation()
{
    //-----------------ANIMATION BY WEAPONS-----------------//
    _aniByWpn = vector<vector<CAnimation>>();
    /*	~ WEAPON 0
    	~ Fist (Default)
    */
    vector<CPoint> s;	// bmps of standing
    vector<CPoint> a;	// bmps of attacking
    vector<CPoint> gma;	// bmps of on-ground-moving attack
    vector<CPoint> sa;	// bmps of slide-attack
    vector<CPoint> aa;	// bmps of air-attack
    vector<CPoint> ama;	// bmps of on-air-moving attack
    vector<CPoint> ada;	// bmps of on-air-down attack
    vector<CPoint> sd;	// bmps of drawing sword
    vector<CPoint> r;	// bmps of running
    vector<CPoint> j;	// bmps of jumping
    vector<CPoint> l;	// bmps of leaning
    vector<CPoint> lf;	// bmps of landing falling
    vector<CPoint> uf;	// bmps of unconsciously flying
    vector<CPoint> dg;	// bmps of dodging
    //
    s = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3) };
    a = vector<CPoint> { CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
    gma = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3) };
    sa = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5), CPoint(0, 6), CPoint(0, 7), CPoint(0, 8), CPoint(0, 9) };
    aa = vector<CPoint> { CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
    ama = vector<CPoint> { CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
    ada = vector<CPoint> { CPoint(6, 2), CPoint(6, 3), CPoint(6, 4), CPoint(6, 5), CPoint(6, 6) };
    sd = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3), CPoint(3, 4), CPoint(3, 5), CPoint(3, 6), CPoint(3, 7), CPoint(3, 8), CPoint(3, 9), CPoint(4, 0), CPoint(4, 1), CPoint(4, 2), CPoint(4, 3), CPoint(4, 4), CPoint(4, 5), CPoint(4, 6), CPoint(4, 7), CPoint(4, 8) };
    r = vector<CPoint> { CPoint(2, 0), CPoint(2, 1), CPoint(2, 2), CPoint(2, 1) };
    j = vector<CPoint> { CPoint(6, 2) };
    l = vector<CPoint> { CPoint(5, 2) };
    lf = vector<CPoint> { CPoint(6, 4) };
    uf = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3), CPoint(3, 4) };
    dg = vector<CPoint> { CPoint(5, 6), CPoint(5, 7) };
    //
    vector<CAnimation> tempAniByWpn = vector<CAnimation>();
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &s, BITMAP_SIZE); //ani[0] Stand (Idle) Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &s, BITMAP_SIZE); //ani[1] Stand (Idle) Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &a, BITMAP_SIZE, 5, false); //ani[2] Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &a, BITMAP_SIZE, 5, false); //ani[3] Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l1, &gma, BITMAP_SIZE, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r1, &gma, BITMAP_SIZE, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sa, BITMAP_SIZE, 3, false); //ani[6] Slide Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sa, BITMAP_SIZE, 3, false); //ani[7] Slide Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &aa, BITMAP_SIZE, 3, false); //ani[8] Air Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &aa, BITMAP_SIZE, 3, false); //ani[9] Air Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &ama, BITMAP_SIZE, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &ama, BITMAP_SIZE, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l1, &ada, BITMAP_SIZE, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r1, &ada, BITMAP_SIZE, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sd, BITMAP_SIZE, 3, false); //ani[14] Draw sword Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sd, BITMAP_SIZE, 3, false); //ani[15] Draw sword Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &r, BITMAP_SIZE); //ani[16] Run Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &r, BITMAP_SIZE); //ani[17] Run Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &j, BITMAP_SIZE, 5, false); //ani[18] Jump Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &j, BITMAP_SIZE, 5, false); //ani[19] Jump Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &l, BITMAP_SIZE); //ani[20] Lean Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &l, BITMAP_SIZE); //ani[21] Lean Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &lf, BITMAP_SIZE); //ani[22] Landing Falling Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &lf, BITMAP_SIZE); //ani[23] Landing Falling Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &uf, BITMAP_SIZE); //ani[24] Unconsciously Flying Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &uf, BITMAP_SIZE); //ani[25] Unconsciously Flying Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &dg, BITMAP_SIZE, 15); //ani[26] Dodging Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &dg, BITMAP_SIZE, 15); //ani[27] Dodging Right
    _aniByWpn.push_back(tempAniByWpn);
    /*	~ WEAPON 1
    	~ Sword
    */
    gma = a;
    a = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5) };
    sa = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5), CPoint(0, 6), CPoint(0, 7), CPoint(0, 8), CPoint(0, 9), CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
    aa = sa;
    ama = vector<CPoint> { CPoint(4, 2), CPoint(4, 3), CPoint(4, 4) };
    //
    tempAniByWpn = vector<CAnimation>();
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &s, BITMAP_SIZE); //ani[0] Stand (Idle) Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &s, BITMAP_SIZE); //ani[1] Stand (Idle) Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l3, &a, BITMAP_SIZE, 5, false); //ani[2] Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r3, &a, BITMAP_SIZE, 5, false); //ani[3] Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &gma, BITMAP_SIZE, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &gma, BITMAP_SIZE, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l2, &sa, BITMAP_SIZE, 3, false); //ani[6] Slide Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r2, &sa, BITMAP_SIZE, 3, false); //ani[7] Slide Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l2, &aa, BITMAP_SIZE, 2, false); //ani[8] Air Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r2, &aa, BITMAP_SIZE, 2, false); //ani[9] Air Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l1, &ama, BITMAP_SIZE, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r1, &ama, BITMAP_SIZE, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l1, &ada, BITMAP_SIZE, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r1, &ada, BITMAP_SIZE, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sd, BITMAP_SIZE, 3, false); //ani[14] Draw sword Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sd, BITMAP_SIZE, 3, false); //ani[15] Draw sword Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &r, BITMAP_SIZE); //ani[16] Run Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &r, BITMAP_SIZE); //ani[17] Run Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &j, BITMAP_SIZE, 5, false); //ani[18] Jump Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &j, BITMAP_SIZE, 5, false); //ani[19] Jump Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &l, BITMAP_SIZE); //ani[20] Lean Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &l, BITMAP_SIZE); //ani[21] Lean Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &lf, BITMAP_SIZE); //ani[22] Landing Falling Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &lf, BITMAP_SIZE); //ani[23] Landing Falling Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &uf, BITMAP_SIZE); //ani[24] Unconsciously Flying Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &uf, BITMAP_SIZE); //ani[25] Unconsciously Flying Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &dg, BITMAP_SIZE, 15); //ani[26] Dodging Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &dg, BITMAP_SIZE, 15); //ani[27] Dodging Right
    _aniByWpn.push_back(tempAniByWpn);
    /*	~ WEAPON 2
    	~ N/A
    */
    _aniByWpn.push_back(tempAniByWpn);
}
void Player::SetAnimationStateLeftRight(int leftAnimationID)
{
    if (_dir) // Player is facing right
    {
        SetAnimationState(leftAnimationID + 1);
    }
    else // Player is facing left
    {
        SetAnimationState(leftAnimationID);
    }
}

void Player::SetAnimationState(int num)
{
    currentAni = num;
}

void Player::ShowCurrentAnimation()
{
    if (_PLAYER_DEBUG)
    {
        CPoint cam = camera->GetXY(_x, _y);
        _collision_box.SetTopLeft(cam.x, cam.y);		//actual player blocks
        _collision_box.ShowBitmap(BITMAP_SIZE * camera->GetSize());
    }

    if (_aniSelector) // If '_aniByWpn' is chosen for showing the animation
    {
        //Calculate and set the position of the player current animation in respect to the collision box's
        CPoint cam = camera->GetXY(_x - (int)(_OFFSET_X * BITMAP_SIZE), _y - (int)(_OFFSET_Y * BITMAP_SIZE));
        _aniByWpn[_wpnID][_currentAniByWpn].SetSize(BITMAP_SIZE * camera->GetSize());
        _aniByWpn[_wpnID][_currentAniByWpn].SetTopLeft(cam.x, cam.y);
        _aniByWpn[_wpnID][_currentAniByWpn].OnShow();
    }
    else // If '_ani' is chosen for showing the animation
    {
        vector<CAnimation>::iterator ani_iter = ani.begin() + currentAni;
        //Calculate and set the position of the player current animation in respect to the collision box's
        CPoint cam = camera->GetXY(_x - (int)(_OFFSET_X * BITMAP_SIZE), _y - (int)(_OFFSET_Y * BITMAP_SIZE));
        ani_iter->SetSize(BITMAP_SIZE * camera->GetSize());
        ani_iter->SetTopLeft(cam.x, cam.y);
        ani_iter->OnShow();
    }
}

bool Player::IsOnLeftEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if (((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS)) && (_x == groundX1 - _width))
            return true;
    }

    return false;
}

bool Player::IsOnRightEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if ((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS) && (_x == groundX2))
            return true;
    }

    return false;
}

bool Player::IsOutMapBorder()
{
    return (!(
                ((MAP_BORDER_X1 <= GetCor(0)/*x1*/) && (GetCor(2)/*x2*/ <= MAP_BORDER_X2))
                &&
                ((MAP_BORDER_Y1 <= GetCor(1)/*y1*/) && (GetCor(3)/*y2*/ <= MAP_BORDER_Y2))
            ));
}

void Player::InitiateOffsetUp(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = -initialOffsetVelocityMagnitude;
    _y -= Round(initialOffsetVelocityMagnitude); //Trick explaination: By intuition, '_y' of the player should not be
    // modified here, because it is expected to be modified whenever 'Player::OnMove()' is called. However,
    // since the player is currently on the ground, 'Player::OnMove()' will fix its '_y' onto the surface
    // instead of modifying it as expectation. Thus, '_y' must be altered here to set the player jump his ass up!!
}
void Player::InitiateOffsetDown(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = initialOffsetVelocityMagnitude;
}
void Player::InitiateOffsetLeft(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetLeft = true;
    _isOffsetRight = false;
}
void Player::InitiateOffsetRight(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetRight = true;
    _isOffsetLeft = false;
}
bool Player::IsBeingOffsetHorizontally()
{
    return (_isOffsetLeft || _isOffsetRight);
}
void Player::DoHorizontalOffset()
{
    if (_isOffsetLeft)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            _x -= Round(_horizontalVelocity);
        }
        else
        {
            _isOffsetLeft = false;
        }
    }
    else if (_isOffsetRight)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            _x += Round(_horizontalVelocity);
        }
        else
        {
            _isOffsetRight = false;
        }
    }
}
void Player::DoMoveLeft(int movementUnit)
{
    _x -= movementUnit;
}
void Player::DoMoveRight(int movementUnit)
{
    _x += movementUnit;
}
void Player::DoLand()
{
    _verticalAcceleration = LANDING_ACCELERATION;
}
void Player::DoJump()
{
    if (_jumpCount > 0)   //If the player is able to jump more
    {
        InitiateOffsetUp(INITIAL_VELOCITY);
        _jumpCount--; //Decrement the jumps available
    }
}
void Player::ResetJumpCount()
{
    _jumpCount = MAX_JUMP_COUNT;
}
void Player::InitiateWallJump()
{
    if (_jumpCount > 0) //If the player is able to jump more
    {
        if (IsOnLeftEdge())
        {
            InitiateOffsetLeft(OFFSET_INITIAL_VELOCITY);
        }
        else if (IsOnRightEdge())
        {
            InitiateOffsetRight(OFFSET_INITIAL_VELOCITY);
        }
    }
}

bool Player::IsAttackable(Player* potentialTargetPlayer)
{
    for (auto elementPtr : _hitTargetPlayers)
        if (elementPtr == potentialTargetPlayer) // If the potential target player has already been attacked by "this" player in his attack duration
            return (false);

    return ((potentialTargetPlayer != this) // Not the "this" player himself
            &&
            (!potentialTargetPlayer->_isDodging) // The potential target player is not dodging
            &&
            (HitPlayer(potentialTargetPlayer, _triggeredAniDir)) // The potential player is in hit range
           );
}

void Player::DoAttack()
{
    for (auto eachPlayerPtr : (*_playersPtr))
    {
        if (IsAttackable(eachPlayerPtr))
        {
            if (_isHoldingWeapon)
                CAudio::Instance()->Play(AUDIO_SWOOSH);
            else
                CAudio::Instance()->Play(AUDIO_PUNCH);

            PerformAttack(eachPlayerPtr, _triggeredAniDir);
        }
    }
}
void Player::PerformAttack(Player* targetPlayer, bool attackDirection)
{
    // Set the attacker for the target player
    targetPlayer->SetAttacker(this, 0);
    // Put the target player in the list, so as not to advertently hit him in a repeated manner during a single attack animation
    _hitTargetPlayers.push_back(targetPlayer);
    // Determine the attack "direction", which is a 2D Vector
    Vector2 vectorAttackerToTargetPlayer;
    vectorAttackerToTargetPlayer.SetXY(GetCor(0), GetCor(1), targetPlayer->GetCor(0), targetPlayer->GetCor(1));
    // Increment the taken damage of the target player
    targetPlayer->_takenDmg += INCREMENT_AMOUNT_OF_TAKEN_DAMAGE;
    // Set the offset magnitude of the attack 2D Vector
    int attackOffsetMagnitude = targetPlayer->_takenDmg;
    // Determine the attack 2D vector
    double multiplier = (vectorAttackerToTargetPlayer.GetLength() == 0 ? attackOffsetMagnitude : attackOffsetMagnitude / vectorAttackerToTargetPlayer.GetLength()); // Avoid division by 0
    Vector2 targetPlayerDisplacementVector(Round(vectorAttackerToTargetPlayer.GetX() * multiplier),
                                           Round(vectorAttackerToTargetPlayer.GetY() * multiplier));
    //
    targetPlayer->BeenAttacked(targetPlayerDisplacementVector, attackDirection);
}
bool Player::HitPlayer(Player* targetPlayer, bool attackDirection)
{
    int attackerX1 = GetCor(0);
    int attackerY1 = GetCor(1);
    int attackerX2 = GetCor(2);
    int attackerY2 = GetCor(3);
    int attackRangeX1, attackRangeY1, attackRangeX2, attackRangeY2;
    attackRangeY1 = attackerY1;
    attackRangeY2 = attackerY2;

    if (attackDirection)   // Player in triggered animation is facing right
    {
        attackRangeX1 = (attackerX1 + attackerX2) / 2;
        attackRangeX2 = attackerX2 + 50;
    }
    else   // Player in triggered animation is facing left
    {
        attackRangeX1 = attackerX1 - 50;
        attackRangeX2 = (attackerX1 + attackerX2) / 2;
    }

    return (targetPlayer->GetCor(2) >= attackRangeX1 && targetPlayer->GetCor(0) <= attackRangeX2
            &&
            targetPlayer->GetCor(3) >= attackRangeY1 && targetPlayer->GetCor(1) <= attackRangeY2);
}
void Player::DoThrowingWeapon()
{
    Weapon throwing;
    throwing.Initialize(vector<Ground*> {}, vector<Player*> {this});
}
void Player::PlayAudioByState()
{
    int aboutToPlay = -1;

    if (WpnStateChanged() && _isTriggeredAni)
    {
        switch (_triggeredAniAnimationID)
        {
            case ANI_WPN_ID_DRAW_SWORD_LEFT:
            case ANI_WPN_ID_DRAW_SWORD_RIGHT:
                aboutToPlay = AUDIO_DRAW_WEAPON;
                break;

            case ANI_WPN_ID_STAND_LEFT:
            case ANI_WPN_ID_STAND_RIGHT:
            case ANI_WPN_ID_ATTACK_LEFT:
            case ANI_WPN_ID_ATTACK_RIGHT:
            case ANI_WPN_ID_GND_MOVE_ATTACK_LEFT:
            case ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT:
            case ANI_WPN_ID_SLIDE_ATTACK_LEFT:
            case ANI_WPN_ID_SLIDE_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT:
                aboutToPlay = AUDIO_SWING_ATTACK;
                break;
        }
    }

    if (StateChanged() && !_isTriggeredAni)
    {
        switch (_currentKeyID)
        {
            case KEY_GND_IDLE:
                break;
        }
    }

    if (aboutToPlay != -1)
        CAudio::Instance()->Play(aboutToPlay);
}

void Player::DoDead()
{
    // Reset weapon
    SetHoldWeapon(false);
    ResetWeaponID(); // reset to the default weapon - punch
    // Set the player to be dead
    _isDead = true;
    // Decrement the player's life
    _life--;
    // Activate dead effect
    _battleSystem->TriggerExplosionEffect(this);

    // Display the attacker
    if (_attacker == nullptr)
        _battleSystem->TriggerDisplayMessage(_name + " suicided!", 350, 200, 150); // 5 secs
    else
        _battleSystem->TriggerDisplayMessage(_attacker->_name + " killed " + _name + "!", 300, 200, 150); // 5 secs
}

void Player::SetRespawnMovementVector(const int& startPosX, const int& startPosY, const int& destinationPosX, const int& destinationPosY)
{
    Vector2 vectorStartToDestination;
    vectorStartToDestination.SetXY(startPosX, startPosY, destinationPosX, destinationPosY);
    double multiplier = (vectorStartToDestination.GetLength() == 0 ? RESPAWN_MOVEMENT_OFFSET_MAGNITUDE : RESPAWN_MOVEMENT_OFFSET_MAGNITUDE / vectorStartToDestination.GetLength()); // Avoid division by 0
    _vectorRespawnMovement = Vector2(Round(vectorStartToDestination.GetX() * multiplier),
                                     Round(vectorStartToDestination.GetY() * multiplier));
}

void Player::DoRespawn()
{
    // Set the state of the player to be 'RESPAWN_STATE'
    SetState(RESPAWN_STATE);
    // Set prev length to max integer value
    _preDistance = 10E5;
    // Set the player to be able to dodge while respawning
    SetIsDodging(true);

    // Get the dead position of the player, which is now the current position
    if (_x < SIZE_X / 2) // Respawn from left side
    {
        _x = RESPAWN_LEFT_START_POS_X;
        _y = RESPAWN_LEFT_START_POS_Y;
        _respawnCourier.SetDir(true); // Set the respawn courier direction to face left
    }
    else // Respawn from right side
    {
        _x = RESPAWN_RIGHT_START_POS_X;
        _y = RESPAWN_RIGHT_START_POS_Y;
        _respawnCourier.SetDir(false); // Set the respawn courier direction to face left
    }

    // Set the respawn position
    Ground* g = GetRandomGround(&_grounds);		// Randomly select Ground
    _resDestPosX = random(g->GetCor(0), g->GetCor(2) - GetWidth());		// Randomly set x coordinate within Ground's width
    _resDestPosY = g->GetCor(1) - GetHeight() - RESPAWN_DISTANCE_ABOVE_GROUND;
    //
    SetRespawnMovementVector(_x, _y, _resDestPosX, _resDestPosY);
}

void Player::InitializeOnRespawn()
{
    //
    ResetTriggeredAnimationVariables();
    _finishedTriggeredAniKeyID = 0;
    //
    _isPressingLeft = _isPressingRight = _dir = false;
    _isTriggerPressingLeft = false;
    //
    _isPressingDown = false;
    //
    _isTriggerJump = false;
    ResetJumpCount();
    //
    _horizontalVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = _isOffsetRight = false;
    //
    _verticalVelocity = INITIAL_VELOCITY;
    _verticalAcceleration = INITIAL_ACCELERATION;
    //
    _isTriggerDrawWeapon = false;
    //
    _isHoldingWeapon = _isTriggerAttack = false;
    _takenDmg = INITIAL_TAKEN_DAMAGE;
    //
    _isTriggerDodge = false;
    _isDodging = false;
    //
    ResetWeaponID();
    //
    SetAnimationSelector(true);
    //
    _currentAniByWpn = 0;
    //
    SetConscious();
    //
    _isFirstTimeOnEdge = true;
    //
    _flyingWeapon = nullptr;
    //
    ResetMovementVelocity();
    //
    SetAttacker(nullptr, 0);
    //
    _isDead = false;
    //
    SetState(CONSCIOUS_STATE);
    //
    _vectorRespawnMovement = Vector2();
    _resDestPosX = _resDestPosY = 0;
    //_prevLength is initialized whenever the player 'DoRespawn()'
}

void Player::SetAnimationStateByWeapon(int num)
{
    _currentAniByWpn = num;
}

void Player::AddCollectionOfAnimationsByWeapon(
    vector<int>& s2l, vector<int>& s2r, vector<int>& al, vector<int>& ar,
    vector<int>& gmal, vector<int>& gmar, vector<int>& sal, vector<int>& sar,
    vector<int>& aal, vector<int>& aar, vector<int>& amal, vector<int>& amar,
    vector<int>& adal, vector<int>& adar, vector<int>& sdl, vector<int>& sdr)
{
    vector<CAnimation> tempAniByWpn = vector<CAnimation>();
    AddCAnimationByWeapon(tempAniByWpn, &s2l, BITMAP_SIZE); //ani[0] Stand (Idle) Left with sword
    AddCAnimationByWeapon(tempAniByWpn, &s2r, BITMAP_SIZE); //ani[1] Stand (Idle) Right with sword
    AddCAnimationByWeapon(tempAniByWpn, &al, BITMAP_SIZE, 3, false); //ani[2] Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &ar, BITMAP_SIZE, 3, false); //ani[3] Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &gmal, BITMAP_SIZE, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &gmar, BITMAP_SIZE, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &sal, BITMAP_SIZE, 3, false); //ani[6] Slide Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &sar, BITMAP_SIZE, 3, false); //ani[7] Slide Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &aal, BITMAP_SIZE, 3, false); //ani[8] Air Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &aar, BITMAP_SIZE, 3, false); //ani[9] Air Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &amal, BITMAP_SIZE, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &amar, BITMAP_SIZE, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &adal, BITMAP_SIZE, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationByWeapon(tempAniByWpn, &adar, BITMAP_SIZE, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationByWeapon(tempAniByWpn, &sdl, BITMAP_SIZE, 3, false); //ani[14] Draw sword Left
    AddCAnimationByWeapon(tempAniByWpn, &sdr, BITMAP_SIZE, 3, false); //ani[15] Draw sword Right
    _aniByWpn.push_back(tempAniByWpn);
}

void Player::AddCAnimationByWeapon(vector<CAnimation>& tempAniByWpn, vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    tempAniByWpn.push_back(temp);
}

int Player::GetKeyCombination()
{
    /* Key combination structure:
    [ 1 ][ 2 ][ 3 ]

    [ 1 ] determines the player is on air or on ground
    [ 2 ] determine the right button is pressed, the left button is pressed, the down button is pressed, or none of the movement keys are pressed
    [ 3 ] determine whether it is the case that the attack button is pressed and the player is holding a weapon
    */
    string keyCombString = "";

    if (IsOnGround())
        keyCombString = keyCombString + "1";
    else
        keyCombString = keyCombString + "2";

    if (_isPressingRight)
        keyCombString = keyCombString + "2";
    else if (_isPressingLeft)
        keyCombString = keyCombString + "3";
    else if (_isPressingDown)
        keyCombString = keyCombString + "4";
    else // All movement keys are up
        keyCombString = keyCombString + "1";

    if (_isTriggerAttack)
        keyCombString = keyCombString + "2";
    else
        keyCombString = keyCombString + "1";

    /* SPECIAL CASES */
    if (_isTriggerDrawWeapon) // The player draws his weapon
        keyCombString = "113";
    else if (_isTriggerDodge) // The player dodges
        keyCombString = "114";

    return (stoi(keyCombString));
}

void Player::ResetMovementVelocity()
{
    _moveVelocity = 0;
}

void Player::ProcessCurrentKeyCombinationGameLogic()
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
    if (_isTriggeredAni) // If an animation is triggered and has not finished (decided by 'OnMoveAnimationLogic()'), then
    {
        // Initiate the triggered action (if haven't)
        if (!_isInitiatedTriggeredAni)
        {
            InitiateTriggeredAction();
            _isInitiatedTriggeredAni = true; // Mark that the triggered animation has been triggered
        }

        // Process the triggered animation
        DoTriggeredAction();
        // Reset all movement inertia
        ResetMovementVelocity();
    }
    else // If there is no "Triggered Animation" in the meantime, then do the other "Non-Triggered Animation(s)"
        DoNonTriggeredAction();
}

void Player::ResetTriggeredAnimationVariables()
{
    SetTriggeredAnimation(false);
    SetTriggeredAnimationKeyID(0);
    SetTriggeredAnimationDir(false);
    SetTriggeredAnimationAnimationID(-1);
    _isInitiatedTriggeredAni = false;
}

void Player::SetTriggeredAnimation(bool newIsTriggeredAni)
{
    _isTriggeredAni = newIsTriggeredAni;
}

void Player::SetTriggeredAnimationKeyID(int newTriggeredAniKeyID)
{
    _triggeredAniKeyID = newTriggeredAniKeyID;
}

void Player::SetTriggeredAnimationDir(bool newTriggeredAniDir)
{
    _triggeredAniDir = newTriggeredAniDir;
}

void Player::SetTriggeredAnimationVariables(int keyCombInt)
{
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == keyCombInt)
            elementPtr->SetTriggeredAnimationVariables();
}

void Player::InitiateTriggeredAction()
{
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == _triggeredAniKeyID)
            elementPtr->InitiateTriggeredAction();
}

void Player::DoTriggeredAction()
{
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == _triggeredAniKeyID)
            elementPtr->DoTriggeredAction();
}

bool Player::IsFinishedTriggeredAnimation()
{
    if (_aniSelector)
        return (_aniByWpn[_wpnID][_triggeredAniAnimationID].IsFinalBitmap());
    else
        return (ani[_triggeredAniAnimationID].IsFinalBitmap());
}

void Player::DoMoveLeftWithAcceleration()
{
    if (_moveVelocity > -MAX_MOVE_VELOCITY)
        _moveVelocity -= MOVE_ACCELERATION;

    _x += Round(_moveVelocity);
}

void Player::DoMoveRightWithAcceleration()
{
    if (_moveVelocity < MAX_MOVE_VELOCITY)
        _moveVelocity += MOVE_ACCELERATION;

    _x += Round(_moveVelocity);
}

void Player::DoNonTriggeredAction()
{
    /*	~ Remarks:
    ~ We care only about key combinations that does NOT determine the attack button is pressed and the player is holding a weapon
    ~ That is, only the key combinations ending with '1' are taken into account
    ~ Jump is a special case and thus is added into the bottom of the function
    */
    switch (_currentKeyID)
    {
        /* ON GROUND */
        case KEY_GND_IDLE: // on ground, not move, not attack
            // Do nothing
            break;

        case KEY_GND_MOVE_RIGHT: // on ground, move right, not attack
            DoMoveRightWithAcceleration();
            break;

        case KEY_GND_MOVE_LEFT: // on ground, move left, not attack
            DoMoveLeftWithAcceleration();
            break;

        case KEY_GND_LAND_DOWN: // on ground, land down, not attack
            // Do nothing
            break;

        /* ON AIR */
        case KEY_AIR_IDLE: // on air, not move, not attack
            // Do nothing
            break;

        case KEY_AIR_MOVE_RIGHT: // on air, move right, not attack
            DoMoveRight(MOVEMENT_UNIT);
            break;

        case KEY_AIR_MOVE_LEFT: // on air, move left, not attack
            DoMoveLeft(MOVEMENT_UNIT);
            break;

        case KEY_AIR_LAND_DOWN: // on air, land down, not attack
            DoLand();
            break;

        default:
            break;
    }

    /* JUMP */
    if (_isTriggerJump) // Game logic 'OnMove()' catches the signal jump
    {
        DoJump();

        if (IsOnLeftEdge() || IsOnRightEdge())
            InitiateWallJump();

        _isTriggerJump = false; // Turn off the jump trigger
    }

    /*	~ MOVEMENT INERTIA
    	~ The movement inertia of the player happens only when the player
    	~ stops after running, or turns running direction
    */
    if (_currentKeyID == KEY_GND_IDLE)
    {
        if (_moveVelocity > 2)
        {
            _moveVelocity -= STOP_ACCELERATION;
            _x += Round(_moveVelocity);
        }
        else if (_moveVelocity < -2)
        {
            _moveVelocity += STOP_ACCELERATION;
            _x += Round(_moveVelocity);
        }
        else // -2 <= _moveVelocity <= 2
        {
            ResetMovementVelocity(); // Reset all movement inertia
        }
    }
    else if (!((_currentKeyID == KEY_GND_MOVE_LEFT) || (_currentKeyID == KEY_GND_MOVE_RIGHT)))
    {
        ResetMovementVelocity(); // Reset all movement inertia
    }
}
void Player::SetCurrentNonTriggeredAnimation()
{
    /*	~ Remark:
    	~ The player is NOT performing a trigger animation
    	~ The animation is NOT dependent on the weapon (decided by the actual sprite of the player)
    */
}

int Player::GetCurrentAniNum()
{
    return (ani.begin() + currentAni)->GetCurrentBitmapNumber();
}

int Player::GetCurrentAniByWeaponNum()
{
    return _aniByWpn[_wpnID][_currentAniByWpn].GetCurrentBitmapNumber();
}

void Player::SetCurrentNonTriggeredAnimationByWeapon()
{
    /*	~ Remark:
    	~ The player is NOT performing a trigger animation
    	~ The animation is dependent on the weapon (decided by the actual sprite of the player)
    */
    switch (_state)
    {
        case CONSCIOUS_STATE:
            switch (_currentKeyID)
            {
                /* ON GROUND */
                case KEY_GND_IDLE:
                    if (_dir) // If the player is facing right
                        SetAnimationStateByWeapon(ANI_WPN_ID_STAND_RIGHT);
                    else
                        SetAnimationStateByWeapon(ANI_WPN_ID_STAND_LEFT);

                    break;

                case KEY_GND_MOVE_LEFT:
                    SetAnimationStateByWeapon(ANI_WPN_ID_MOVE_LEFT);
                    break;

                case KEY_GND_MOVE_RIGHT:
                    SetAnimationStateByWeapon(ANI_WPN_ID_MOVE_RIGHT);
                    break;

                case KEY_GND_LAND_DOWN:
                    // Do nothing
                    break;

                /* ON AIR */
                case KEY_AIR_IDLE:
                    if (IsOnLeftEdge()) // Player is leaning on left edge
                        SetAnimationStateByWeapon(ANI_WPN_ID_LEAN_RIGHT); // Set the leaning animation of player facing right
                    else if (IsOnRightEdge()) // Player is leaning on left edge
                        SetAnimationStateByWeapon(ANI_WPN_ID_LEAN_LEFT); // Set the leaning animation of player facing left
                    else // Player is jumping
                        if (_dir)
                            SetAnimationStateByWeapon(ANI_WPN_ID_JUMP_RIGHT);
                        else
                            SetAnimationStateByWeapon(ANI_WPN_ID_JUMP_LEFT);

                    break;

                case KEY_AIR_MOVE_RIGHT:
                    SetAnimationStateByWeapon(ANI_WPN_ID_JUMP_RIGHT);
                    break;

                case KEY_AIR_MOVE_LEFT:
                    SetAnimationStateByWeapon(ANI_WPN_ID_JUMP_LEFT);
                    break;

                case KEY_AIR_LAND_DOWN:
                    if (_dir)
                        SetAnimationStateByWeapon(ANI_WPN_ID_LAND_FALL_RIGHT);
                    else
                        SetAnimationStateByWeapon(ANI_WPN_ID_LAND_FALL_LEFT);

                    break;

                default:
                    break;
            }

            break;

        case UNCONSCIOUS_STATE:

            /// Comment for future devs: This special case overrides the others
            /// where the player is unconscious should be separated as another
            /// animation vector, not being put in 'ani'
            if (_unconsciousAniDir)
                SetAnimationStateByWeapon(ANI_WPN_ID_UNCONSCIOUS_FLYING_RIGHT);
            else
                SetAnimationStateByWeapon(ANI_WPN_ID_UNCONSCIOUS_FLYING_LEFT);

            break;

        case RESPAWN_STATE:

            /// Comment for future devs: This special case overrides the others
            /// where the player is unconscious should be separated as another
            /// animation vector, not being put in 'ani'
            if (_dir)
                SetAnimationStateByWeapon(ANI_WPN_ID_STAND_RIGHT);
            else
                SetAnimationStateByWeapon(ANI_WPN_ID_STAND_LEFT);

            break;

        default:
            // Never happen
            return;
    }
}

void Player::AddCamera(Camera* cam)
{
    camera = cam;
}

void Player::SetPlayer(bool tri)
{
    _isPlayer = tri;
}

bool Player::IsPlayer()
{
    return _isPlayer;
}

void Player::SetSize(double size)
{
    BITMAP_SIZE = size;
}

double Player::GetSize()
{
    return BITMAP_SIZE;
}

int Player::Round(double i)
{
    return (int)(i - 0.5);
}

bool Player::StateChanged()
{
    bool ret = false;

    if (_lastTriggeredAniKeyID != _currentKeyID)
        ret = true;

    _lastTriggeredAniKeyID = _currentKeyID;
    return ret;
}

bool Player::WpnStateChanged()
{
    bool ret = false;

    if (_lastTriggeredAniByWpnID != _triggeredAniAnimationID)
        ret = true;

    _lastTriggeredAniByWpnID = _triggeredAniAnimationID;
    return ret;
}

void Player::SetConscious()
{
    SetState(CONSCIOUS_STATE);
    _unconsciousFramesCount = 0;
    _unconsciousAniDir = false;
}

const bool& Player::GetTriggeredAnimationDirection() const
{
    return (_triggeredAniDir);
}

void Player::SetTriggeredAnimationAnimationID(const int& newTriggeredAniAnimationID)
{
    _triggeredAniAnimationID = newTriggeredAniAnimationID;
}

void Player::SetIsTriggerAttack(const bool& newIsTriggerAttack)
{
    _isTriggerAttack = newIsTriggerAttack;
}

void Player::SetIsDodging(const bool& newIsDodging)
{
    _isDodging = newIsDodging;
}

void Player::SetIsTriggerDodge(const bool& newIsTriggerDodge)
{
    _isTriggerDodge = newIsTriggerDodge;
}

void Player::SetIsTriggerDrawWeapon(const bool& newIsTriggerDrawWeapon)
{
    _isTriggerDrawWeapon = newIsTriggerDrawWeapon;
}

void Player::EmptyHitTargetPlayers()
{
    _hitTargetPlayers.clear();
}

void Player::SetX(const int& newX)
{
    _x = newX;
}

void Player::SetY(const int& newY)
{
    _y = newY;
}

void Player::DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr)
{
    int groundX1 = groundPtr->GetCor(0);
    int groundY1 = groundPtr->GetCor(1);
    int groundX2 = groundPtr->GetCor(2);
    int groundY2 = groundPtr->GetCor(3);

    if (groundPtr->IsOnGroundLeftEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _x = groundX1 - _width;
    }
    else if (groundPtr->IsOnGroundRightEdge(playerX1, playerY1, playerX2, playerY2))
    {
        _x = groundX2;
    }
    else if (groundPtr->IsOnGroundUnderside(playerX1, playerY1, playerX2, playerY2))
    {
        _y = groundY2;
    }
    else if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
    {
        _y = groundY1 - _height;
    }
}

bool Player::IsOnGround()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
            return true;
    }

    return false;
}


void Player::DoBounceOffGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr)
{
    if (groundPtr->IsOnGroundLeftEdge(playerX1, playerY1, playerX2, playerY2))
    {
        InitiateOffsetLeft(abs(_horizontalVelocity));
        _unconsciousAniDir = false; // left
    }
    else if (groundPtr->IsOnGroundRightEdge(playerX1, playerY1, playerX2, playerY2))
    {
        InitiateOffsetRight(abs(_horizontalVelocity));
        _unconsciousAniDir = true; // right
    }
    else if (groundPtr->IsOnGroundUnderside(playerX1, playerY1, playerX2, playerY2))
    {
        InitiateOffsetDown(abs(_verticalVelocity));
    }
    else if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
    {
        InitiateOffsetUp(abs(_verticalVelocity));
    }
}

ExplosionEffect* Player::GetExplosionEffect()
{
    return (_explosionEffectPtr);
}

void Player::SetState(const int& newState)
{
    _state = newState;
}

}